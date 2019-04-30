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
  song_index = 0;

  digitalWrite(cs_pin_left, LOW);
  screen->init();
  screen->setRotation(2);
  screen->setTextSize(1); // default font size
  screen->setTextColor(TEXT, BACKGROUND);
  screen->fillScreen(BACKGROUND);
  digitalWrite(cs_pin_left, HIGH);

  digitalWrite(cs_pin_right, LOW);
  screen->init();
  screen->setRotation(2);
  screen->setTextSize(1); // default font size
  screen->setTextColor(TEXT, BACKGROUND);
  screen->fillScreen(BACKGROUND);
  digitalWrite(cs_pin_right, HIGH);
}

/**
 * Game UI state machine. Note that this function will not be called constantly -- for instance, observe the code for the gameplay
 * state; it doesn't exit one single call of Interface::process() until the game is over
 */
void Interface::process() {
  switch (state) {
    case USERNAME_STATE: {
      select_username();
      break;
    }
    case SONGSELECT_STATE: {
      // display list of songs on left screen
      // display recent scores of currently selected song on right screen (lower priority)
      // if short press up button, move up in song list (unless on first song, in which case do nothing)
      // if short press down button, move down in song list (unless on last song, in which case do nothing)
      // if long press down button, do game->load() and game->start(), then move into GAMEPLAY state

      if (button2->update() == 2) { 
        game->load();
        game->start();
        state = GAMEPLAY_STATE;
        break;
      }

      boolean updated = update_song_index();
      if (updated) {
        update_song_display();
        display_recent_scores();
      }
      break;
    }
    case GAMEPLAY_STATE: {
      boolean complete = false;
      while (!complete) {
        complete = game->process(); // game.process will do things such as update display, detect motion, play music, etc
      }
      // reach this point after game has completed
      upload_score(/* params tbd */);
      state = SCORE_STATE;
      break;
    }
    case SCORE_STATE: {
      // display player score and possibly some data about play on left screen
      // display list of high scores on right screen
      // long press either button to move into SONGSELECT state
      display_player_score(game->get_score());
      display_high_scores();
      while (true) {
      /*
        if (either button long pressed) {
          update_song_display();
          display_recent_scores();
          state = SONGSELECT_STATE;
          break;
        }
       */
      }
      break;
    }
  }
}

void Interface::select_username() {
  // display instructions for selecting user name on right screen (what buttons do what, username length restriction)
  digitalWrite(cs_pin_right, LOW);
  screen->println("Select your username (<= 10 characters)\n");
  screen->println("Press up to go to the previous letter");
  screen->println("Press down to go to the next letter\n");
  screen->println("Hold up to go to the next letter");
  screen->println("Hold down to save your username");
  digitalWrite(cs_pin_right, HIGH);
  
  // display current username on left screen
  // if short press up button, roll backward in char list
  // if short press down button, roll forward in chart list
  // if long press up button, move onto next letter 
  // if long press down button, save username into field, move to SONGSELECT state
  uint8_t flag1 = 0;
  uint8_t flag2 = 0;
  int char_index = 0;
  int username_index = 0;
  char old_display[USERNAME_LENGTH_LIMIT+1]
  char new_display[USERNAME_LENGTH_LIMIT+1]
  while (true) { //
    flag1 = button1->update();
    flag2 = button2->update();

    if (username_index == USERNAME_LENGTH_LIMIT - 1) { // can't add anymore characters
      if (flag2 == 2) {
        state = SONGSELECT_STATE;
        // TODO: where to store username??

        // initialize the display
        update_song_display();
        display_recent_scores();
      }
    }

    if (flag1 == 1) { // button 1 short press
      char_index--;
      if (char_index < 0) { 
        char_index = char_index + 27; 
      }
    } else if (flag1 == 2) { // button 1 long press
      char current[2] = {alphabet[char_index], '\0'};
      strcat(username, current);
      username_index++;
      char_index = 0;
      continue;
    }
    
    if (flag2 == 1) { // button 2 short press
      char_index = (char_index + 1)%27;
    } else if (flag2 == 2) { // button 2 long press
      state = SONGSELECT_STATE;
      char current[2] = {alphabet[char_index], '\0'};
      strcat(username, current);
      // TODO: where to store username??

      // initialize the display
      update_song_display();
      display_recent_scores();
    }

    strcpy(new_display, username);
    strcat(new_display, {alphabet[char_index], '\0'});

    if (strcmp(new_display, old_display) != 0) {//only draw if changed!
      digitalWrite(cs_pin_left, LOW);
      tft.fillScreen(BACKGROUND);
      tft.setCursor(0, 0, 1);
      tft.println(new_display);
      digitalWrite(cs_pin_left, HIGH);
    }

    memset(old_display, 0, sizeof(old_display));
    strcat(old_display, new_display); 
  }
}

/**
 * Read button inputs, update song_index if needed
 * return true iff an update occurred. In particular, if the up button is pressed but we were already at the first song 
 * and can't move up further, don't return true.
 */
boolean Interface::update_song_index() {
  uint8_t flag1 = button1->update();
  uint8_t flag2 = button2->update();

  if (flag1 == 1 && song_index > 0) { // go up the list
    song_index--;
    return true;
  } else if (flag2 == 1 && song_index < 3) { // go down the list
    song_index++;
    return true;
  }

  return false;
}

void Interface::update_song_display() {
  // selected song is in the middle of left screen, and up to n (where n is probably like 3) songs before and after the
  // selected song are also displayed

  digitalWrite(cs_pin_left, LOW);
  char* display_list[7];
  switch(song_index) { // there's probably a better way to do this but i am tired
    case(0):
        display_list = {"\0", "\0", "\0", song_list[1], song_list[2], song_list[3], song_list[4]};
        break;
    case(1):
        display_list = {"\0", "\0", song_list[1], song_list[2], song_list[3], song_list[4], "\0"};
        break;
    case(2):
        display_list = {"\0", song_list[1], song_list[2], song_list[3], song_list[4], "\0", "\0"};
        break;
    case(3):
        display_list = {song_list[1], song_list[2], song_list[3], song_list[4], "\0", "\0", "\0"};
        break;
  }

  screen->fillScreen(BACKGROUND);
  screen->setCursor(0, MIDDLE_HEIGHT - 35, 1);
  for (int i = 0; i < 7; i++) {
    if (i == 3) { // currently selected "middle"
        screen->setTextColor(TFT_BLUE);
        screen->setTextSize(2);
        screen->println(display_list[i]);
        screen->setTextColor(TEXT);
        screen->setTextSize(1);
    } else {
        screen->println(display_list[i]);
    }
  }
  digitalWrite(cs_pin_left, HIGH);
}

void Interface::display_recent_scores() {
  // display on right screen
  // TODO @matt
  digitalWrite(cs_pin_right, LOW);
  // do stuff
  digitalWrite(cs_pin_right, HIGH);
}

/**
 * Display score of the game just played
 */
void Interface::display_player_score(uint16_t score) {
  screen->setCursor(0, MIDDLE_HEIGHT - 20, 1);
  screen->println("You earned:");
  screen->setTextSize(2);
  screen->print(score);
  screen->setTextSize(1);
}

/**
 * Pull high scores and display it on right screen
 */
void Interface::display_high_scores() {
  // TODO @matt
}

void Interface::upload_score(/* params tbd */) {
  // TODO @matt
}
