#include "Interface.h"

Interface::Interface(Game* game_pointer, TFT_eSPI* tft, int tft_pin_left, int tft_pin_right, Button* button1_pointer, Button* button2_pointer, char** songs) {
  game = game_pointer;
  screen = tft;
  cs_pin_left = tft_pin_left;
  cs_pin_right = tft_pin_right;
  button1 = button1_pointer;
  button2 = button2_pointer;

  state = USERNAME_STATE;

  song_list = songs;
  song_index = -1; // initialize to invalid value, will be made valid when transitioning into SONGSELECT stage
}

/**
   Game UI state machine. Note that this function will not be called constantly -- for instance, observe the code for the gameplay
   state; it doesn't exit one single call of Interface::process() until the game is over
*/
void Interface::process() {
  switch (state) {
    case USERNAME_STATE: {
      Serial.println("username selection");
      select_username();
      break;
    }
    case SONGSELECT_STATE: {
      // display list of songs on left screen
      // display recent scores of currently selected song on right screen (lower priority)
      // if short press left button, move up in song list (unless on first song, in which case do nothing)
      // if short press right button, move down in song list (unless on last song, in which case do nothing)
      // if long press right button, do game->load() and game->start(), then move into GAMEPLAY state

      // NOTE: do button readings here so that we avoid reading it twice and missing signals in our second reading, which was a bug previously
      int flag1 = button1->update();
      int flag2 = button2->update();
      
      if (flag2 == 2) {
        Serial.println("long press, enter gameplay state");
        clear_screens();
        boolean load_success = game->load(song_index);
        while (!load_success) { // if can't load, retry until we load
            digitalWrite(cs_pin_left, LOW);
            screen->setCursor(0, MIDDLE_HEIGHT - 20, 1);
            screen->println("Having connection problems, reloading chart");
            digitalWrite(cs_pin_left, HIGH);
            load_success = game->load(song_index);
        }
        game->start(song_index);
        state = GAMEPLAY_STATE;
        break;
      }

      boolean updated = update_song_index(flag1, flag2);
      if (updated) {
        update_song_display();
        display_recent_scores(song_index); 
      }
      break;
    }
    case GAMEPLAY_STATE: {
      boolean complete = false;
      while (!complete) {
        complete = game->process(); // game.process will do things such as update display, detect motion, play music, etc
      }
      // reach this point after game has completed
//      uint16_t score = game->get_score();
//      upload_score(score);
      clear_screens();
      state = SCORE_STATE;
      break;
    }
    case SCORE_STATE: {
      // display player score and possibly some data about play on left screen
      // display list of high scores on right screen
      // long press either button to move into SONGSELECT state
      display_player_score(game->get_score());
//      char high_scores[4][2][10];
//      get_high_scores(high_scores);
//      display_high_scores(high_scores);
      while (true) {
        int flag1 = button1->update();
        int flag2 = button2->update();
        
        if (flag1 == 2 || flag2 == 2) {
          clear_screens();
          song_index = -1; // hacky method to get screen to display, will improve later
          state = SONGSELECT_STATE;
          break;
        }
        
      }
      break;
    }
  }
}

void Interface::select_username() {
  // display instructions for selecting user name on right screen (what buttons do what, username length restriction)
  digitalWrite(cs_pin_right, LOW);
  screen->setCursor(0, 0, 1);
  screen->println("Select your username (<= 8 characters)\n");
  screen->println("Press left to go to the previous letter\n");
  screen->println("Press right to go to the next letter\n");
  screen->println("Hold left to go to the next letter\n");
  screen->println("Hold right to save your username\n");
  digitalWrite(cs_pin_right, HIGH);

  // display current username on left screen
  // if short press up button, roll backward in char list
  // if short press down button, roll forward in chart list
  // if long press up button, move onto next letter
  // if long press down button, save username into field, move to SONGSELECT state
  // flag is 0 for nothing detected, 1 for short press, 2 for long press
  uint8_t flag1 = 0; // output of button 1
  uint8_t flag2 = 0; // output of button 2
  int char_index = 0; // which char we're at in the alphabet
  int username_index = 0; // which username character we're working on
  char username_display[USERNAME_LENGTH_LIMIT + 1];
  char prev_username_display[USERNAME_LENGTH_LIMIT + 1];
  memset(username_display, 0, sizeof(username_display));
  memset(prev_username_display, 0, sizeof(prev_username_display));
  while (true) { //
    flag1 = button1->update();
    flag2 = button2->update();
    if (flag1 == 1) { // button 1 short press
      Serial.println("buttons 1 short press");
      if (username_index == 8) { // can't add anymore characters
        // ignore input
      } else {
        char_index--;
        if (char_index < 0) {
          char_index = char_index + 27;
        }
      }
    } else if (flag1 == 2) { // button 1 long press
      Serial.println("buttons 1 long press");
      if (username_index == 8) { // can't add anymore characters
        // ignore input
      } else {
        username_display[username_index] = alphabet[char_index];
        username_display[username_index + 1] = ' ';
        username_display[username_index + 2] = '\0';
        username[username_index] = alphabet[char_index];
        username[username_index + 1] = '\0';
        username_index++;
        char_index = 0;
      }
    }

    if (flag2 == 1) { // button 2 short press
      Serial.println("buttons 2 short press");
      if (username_index == 8) { // can't add anymore characters
        // ignore input
      } else {
        char_index = (char_index + 1) % 27;
      }
    } else if (flag2 == 2) { // button 2 long press
      Serial.println("buttons 2 long press");
      
      state = SONGSELECT_STATE;
      break;
    }

    username_display[username_index] = alphabet[char_index];
    username_display[username_index + 1] = '\0';

    if (strcmp(username_display, prev_username_display) != 0) { // only draw if display changed!
      Serial.println("display changed");
      Serial.println(username_display);
      digitalWrite(cs_pin_left, LOW);
      screen->fillScreen(BACKGROUND);
      screen->setCursor(0, 0, 1);
      screen->println(username_display);
      digitalWrite(cs_pin_left, HIGH);
    }

    memset(prev_username_display, 0, sizeof(prev_username_display));
    strcat(prev_username_display, username_display);
  }
}

/**
   Read button inputs, update song_index if needed
   return true iff an update occurred. In particular, if the up button is pressed but we were already at the first song
   and can't move up further, don't return true.
   Also returns true if the song index is invalid (aka, just entered SONGSELECT state) and makes song index valid
*/
boolean Interface::update_song_index(int flag1, int flag2) {
  if (song_index == -1) {
    song_index = 1;
    return true;
  }

  if (flag1 == 1 && song_index > 1) { // go up the list
    Serial.println("decrement song_index");
    song_index--;
    return true;
  } 
  if (flag2 == 1 && song_index < 4) { // go down the list
    Serial.println("increment song_index");
    song_index++;
    return true;
  }

  return false;
}

void Interface::update_song_display() {
  // selected song is in the middle of left screen, and up to n (where n is probably like 3) songs before and after the
  // selected song are also displayed

  digitalWrite(cs_pin_left, LOW);

  screen->fillScreen(BACKGROUND);
  screen->setCursor(0, MIDDLE_HEIGHT - 35, 1);
  for (int i = -3; i < 4; i++) {
    if (i == 0) { // currently selected "middle"
      screen->setTextColor(TFT_BLUE);
      screen->setTextSize(2);
      screen->println(song_list[song_index]);
      screen->setTextColor(TEXT);
      screen->setTextSize(1);
    } else {
      if (song_index + i >= 1 && song_index + i <= 4) { // replace 4 by number of songs
        screen->println(song_list[song_index + i]);
      }
    }
  }
  digitalWrite(cs_pin_left, HIGH);
}

/**
   char*** high_scores is of the form char[4][2][10]
*/
void Interface::display_high_scores(char*** high_scores) {
  // display on right screen
  // TODO @matt
  digitalWrite(cs_pin_right, LOW);
  // do stuff
  screen->fillScreen(BACKGROUND);
  digitalWrite(cs_pin_right, HIGH);
}

/**
   Read data stored in recent_scores
*/
void Interface::display_recent_scores(int index) {
  // display on right screen
  // TODO @matt
  digitalWrite(cs_pin_right, LOW);
  // do stuff
  screen->fillScreen(BACKGROUND);
  digitalWrite(cs_pin_right, HIGH);
}

/**
   Pull all recent scores into recent_scores
*/
void Interface::get_all_recent_scores() {

}

/**
   Pull high scores of song song_index into the temporary char[4][2][10] of high_scores
*/
void Interface::get_high_scores(char*** high_scores) {

}

/**
   Display score of the game just played on the left screen
*/
void Interface::display_player_score(uint16_t score) {
  digitalWrite(cs_pin_left, LOW);
  screen->setCursor(0, MIDDLE_HEIGHT - 20, 1);
  screen->println("You earned:");
  screen->setTextSize(2);
  screen->print(score);
  screen->setTextSize(1);
  digitalWrite(cs_pin_left, HIGH);
}

/**
   Upload score of score for user username for song song_index
*/
void Interface::upload_score(uint16_t score) {
  // TODO @matt
}

/**
 * Paint both screens with background color
 */
void Interface::clear_screens() {
  digitalWrite(cs_pin_left, LOW);
  digitalWrite(cs_pin_right, LOW);
  screen->fillScreen(BACKGROUND);
  digitalWrite(cs_pin_left, HIGH);
  digitalWrite(cs_pin_right, HIGH);
}
