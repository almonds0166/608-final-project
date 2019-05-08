#include "Interface.h"

#define SCORES_ENDPOINT "http://608dev.net/sandbox/sc/almonds/br/scoreboard"
//#define SCORES_ENDPOINT_TEST "http://608dev.net/sandbox/sc/almonds/testdir2/testo.py"
#ifndef MAX_BODY_LENGTH
#define MAX_BODY_LENGTH 3000
#endif

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
      // if long press left button, go back to username selection
      // if long press right button, do game->load() and game->start(), then move into GAMEPLAY state

      // NOTE: do button readings here so that we avoid reading it twice and missing signals in our second reading, which was a bug previously
      int flag1 = button1->update();
      int flag2 = button2->update();

      if (flag1 == 2) {
        Serial.println("long press left, back to username");
        clear_screens();
        memset(username, 0, sizeof(username));
        state = USERNAME_STATE;
      }
      
      if (flag2 == 2) {
        Serial.println("long press right, enter gameplay state");
        clear_screens();
        boolean load_success = game->load(song_index);
        while (!load_success) { // if can't load, retry until we load
            digitalWrite(cs_pin_left, LOW);
            screen->setCursor(0, MIDDLE_HEIGHT - 20, 1);
            screen->println("Having connection problems, reloading chart");
            digitalWrite(cs_pin_left, HIGH);
            load_success = game->load(song_index);
        }
        clear_screens();
        game->start(song_index);
        state = GAMEPLAY_STATE;
        break;
      }

      boolean updated = update_song_index(flag1, flag2);
      if (updated) {
        update_song_display();
        display_high_scores();
      }
      break;
    }
    case GAMEPLAY_STATE: {   
      boolean complete = false;
      while (!complete) {
        int flag1 = button1->update();
        if (flag1 == 2) { break; } // long press left, go back to song selection

        complete = game->process(); // game.process will do things such as update display, detect motion, play music, etc
      }
      clear_screens();

      if (complete) { // finished game
        uint16_t score = game->get_score();
        upload_score(score);
        state = SCORE_STATE;
      } else { // ended game early, don't upload score
        game->stop();
        state = SONGSELECT_STATE;
        song_index = -1; // hacky method to get screen to display
      }
      break;
    }
    case SCORE_STATE: {
      // display player score on left screen
      // display details about play on right screen
      // long press either button to move into SONGSELECT state
      display_play_data();
      
      while (true) {
        int flag1 = button1->update();
        int flag2 = button2->update();
        
        if (flag1 == 2 || flag2 == 2) {
          clear_screens();
          get_high_scores(song_index); // incorporate the new score just created by the last play
          song_index = -1; // hacky method to get screen to display, will (maybe) improve later
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
  screen->println("Select your username (up to 8 characters)\n");
  screen->println("Press left to go to the previous letter in the alphabet\n");
  screen->println("Press right to go to the next letter in the alphabet\n");
  screen->println("Hold left to confirm the current letter\n");
  screen->println("Hold right to save your username\n");
  digitalWrite(cs_pin_right, HIGH);

  // display current username on left screen
  // if short press up button, roll backward in char list
  // if short press down button, roll forward in chart list
  // if long press up button, move onto next letter
  // if long press down button, save username into field, move to SONGSELECT state
  // flag is 0 for nothing detected, 1 for short press, 2 for long press
  int char_index = 0; // which char we're at in the alphabet
  int username_index = 0; // which username character we're working on
  char username_display[USERNAME_LENGTH_LIMIT + 1];
  char prev_username_display[USERNAME_LENGTH_LIMIT + 1];

  boolean warning_displayed = false;
  uint32_t warning_timer = 0; // timer for warning message
  uint32_t warning_timeout = 2000; // timeout for warning message

  memset(username_display, 0, sizeof(username_display));
  memset(prev_username_display, 0, sizeof(prev_username_display));
  while (true) {
    int flag1 = button1->update();
    int flag2 = button2->update();
    if (flag1 == 1) { // button 1 short press
      Serial.println("button 1 short press");
      if (username_index == 8) { // can't add anymore characters
        warning_timer = millis();
        warning_displayed = true;
        display_warning("Maximum character limit!");
      } else {
        char_index--;
        if (char_index < 0) {
          char_index = char_index + 27;
        }
      }
    } else if (flag1 == 2) { // button 1 long press
      Serial.println("button 1 long press");
      if (username_index == 8) { // can't add anymore characters
        warning_timer = millis();
        warning_displayed = true;
        display_warning("Maximum character limit!");
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
      Serial.println("button 2 short press");
      if (username_index == 8) { // can't add anymore characters
        warning_timer = millis();
        warning_displayed = true;
        display_warning("Maximum character limit!");
      } else {
        char_index = (char_index + 1) % 27;
      }
    } else if (flag2 == 2) { // button 2 long press
      Serial.println("button 2 long press");
      
      if (username_index == 0) { // empty username
        warning_timer = millis();
        warning_displayed = true;
        display_warning("Can't have an empty username!");
      } else {
        state = SONGSELECT_STATE;
        song_index = -1; 
        break;
      }
    }

    if (warning_displayed && millis() >= warning_timeout + warning_timer) { // clear warning message
      clear_warning();
      warning_displayed = false;
    }

    username_display[username_index] = alphabet[char_index];
    username_display[username_index + 1] = '\0';

    if (strcmp(username_display, prev_username_display) != 0) { // only draw if display changed!
      Serial.println("display changed");
      Serial.println(username_display);
      digitalWrite(cs_pin_left, LOW);
      screen->setTextSize(2);
      screen->setTextDatum(MC_DATUM);
      screen->fillRect(
        0, MIDDLE_HEIGHT - 20,
        RIGHT_EDGE, MIDDLE_HEIGHT + 20,
        BACKGROUND);
      screen->setTextColor(TEXT);
      screen->drawString(
         username_display,
         MIDDLE_WIDTH,
         MIDDLE_HEIGHT);
      screen->setTextColor(FADE[1]);
      screen->drawString(
        &username_display[username_index],
        MIDDLE_WIDTH + CHAR_WIDTH*strlen(username_display) - CHAR_WIDTH,
        MIDDLE_HEIGHT);
      screen->setTextColor(TEXT);
      screen->setTextSize(1);
      screen->setTextDatum(BL_DATUM);
      screen->drawString(username, 0, BOTTOM_EDGE);
      screen->setTextDatum(TL_DATUM);
      digitalWrite(cs_pin_left, HIGH);
    }

    memset(prev_username_display, 0, sizeof(prev_username_display));
    strcat(prev_username_display, username_display);
  }
}

void Interface::display_warning(char* warning_message) {
  digitalWrite(cs_pin_left, LOW);
  screen->fillRect(0, MIDDLE_HEIGHT, RIGHT_EDGE, 20, BACKGROUND);
  screen->setCursor(0, MIDDLE_HEIGHT, 1);
  screen->println(warning_message);
  digitalWrite(cs_pin_left, HIGH);
}

void Interface::clear_warning() {
  Serial.println("Clear warning message");
  digitalWrite(cs_pin_left, LOW);
  screen->fillRect(0, MIDDLE_HEIGHT, RIGHT_EDGE, 20, BACKGROUND);
  digitalWrite(cs_pin_left, HIGH);
}

/**
   Given button inputs, update song_index if needed
   return true iff an update occurred. In particular, if the up button is pressed but we were already at the first song
   and can't move up further, don't return true.
   Also returns true if the song index is invalid (aka, just entered SONGSELECT state) and makes song index valid (change 
   to first song)
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
  if (flag2 == 1 && song_index < NUM_SONGS - 1) { // go down the list
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
  // before
  for (int i = -3; i < 0; i++) {
    if (song_index + i >= 1 && song_index + i < NUM_SONGS) {
      screen->setTextColor(FADE[i+3]);
      screen->setCursor(
        max(0, (int)(RIGHT_EDGE - strlen(song_list[song_index+i]) * CHAR_WIDTH) / 2),
        MIDDLE_HEIGHT - 15 + i * 15, 1);
      screen->println(song_list[song_index + i]);
    }
  }
  // middle
  screen->setTextColor(TFT_WHITE);
  screen->setTextSize(2);
  if(strcmp("Bloody Purity", song_list[song_index]) == 0) {
     // Hacky fix for Bloody Purity song title, which takes
     // up two lines (and looks ugly) without this fix.
     screen->setTextDatum(MC_DATUM);
     screen->drawString("Bloody", MIDDLE_WIDTH, MIDDLE_HEIGHT - 8);
     screen->drawString("Purity", MIDDLE_WIDTH, MIDDLE_HEIGHT + 8);
     screen->setTextDatum(ML_DATUM);
  } else {
     screen->setCursor(
       max(0, (int)(RIGHT_EDGE / 2 - strlen(song_list[song_index]) * CHAR_WIDTH)),
       MIDDLE_HEIGHT - ((strlen(song_list[song_index]) >= 10) ? 14 : 7), 1);
     screen->println(song_list[song_index]);
  }
  screen->setTextColor(TEXT);
  screen->setTextSize(1);
  // after
  for (int i = 1; i < 4; i++) {
    if (song_index + i >= 1 && song_index + i < NUM_SONGS) { 
      screen->setTextColor(FADE[i+3]);
      screen->setCursor(
        max(0, (int)(RIGHT_EDGE - strlen(song_list[song_index+i]) * CHAR_WIDTH) / 2),
        MIDDLE_HEIGHT + 10 + i * 15, 1);
      screen->println(song_list[song_index + i]);
    }
  }
  screen->setTextColor(TEXT);
  screen->setTextDatum(BL_DATUM);
  screen->drawString(username, LEFT_EDGE, BOTTOM_EDGE);
  screen->setTextDatum(TL_DATUM);
  digitalWrite(cs_pin_left, HIGH);
}

/**
   Read data stored in high_scores
   Display high scores for song song_index
*/
void Interface::display_high_scores() {
  digitalWrite(cs_pin_right, LOW);
  const uint8_t VERTICAL_OFFSET = 30; // where the score display starts
  const uint8_t LINE_HEIGHT     = 23; // distance between consecutive strings drawn
  screen->fillScreen(BACKGROUND);
  screen->setTextDatum(MC_DATUM); // Middle center text datum, makes things easier
  uint8_t y = VERTICAL_OFFSET;
  screen->drawString("Player", FIRST_QTR, y);
  screen->drawString("Score",  LAST_QTR,  y);
  for (int player_num = 0; player_num < NUM_PLAYER_SCORES_TO_PULL; player_num++) {
    y += LINE_HEIGHT;
    screen->drawString(high_scores[song_index][player_num][0], FIRST_QTR, y);
    screen->drawString(high_scores[song_index][player_num][1], LAST_QTR,  y);
  }
  screen->setTextDatum(TL_DATUM); // set datum back to the default
  digitalWrite(cs_pin_right, HIGH);
}

/**
   Pull all high scores into high_scores
*/
void Interface::get_all_high_scores() {
  for (int song_num = 1; song_num < NUM_SONGS; song_num++) {
    Serial.println("setting high scores for song");
    Serial.println(song_num);
    get_high_scores(song_num);
  }
}

/**
   Pull high scores of song index into the corresponding high_scores[index]
   Use both as a helper for get_all_high_scores() and separately after a player finishes a song (to update our stored scores)
*/
void Interface::get_high_scores(int index) {
  // Get ready to download the scores
  HTTPClient http;
  char url[200]; char temp[MAX_BODY_LENGTH];
  char* ptr;
  String response;
  strcpy(url, SCORES_ENDPOINT);
  sprintf(temp, "?sortby=top&song=%i&esp32", index);
  strcat(url, temp);
  
  // GET
  http.begin(url);
  int http_code = http.GET();
  Serial.println("____");
  if (http_code > 0) {
    response = http.getString(); // offset,bpm:ts,dir;ts,dir;ts,dir;...
    int i = 0;
    while (i < MAX_BODY_LENGTH) {
      temp[i] = response[i];
      if(response[i++] == '\0') break;
    }
    Serial.printf("GET %s\n", url);
    Serial.printf("Response:\n%s\n", temp);
  } else {
    Serial.println("Something went wrong with the GET request!");
    Serial.println(url);
    Serial.println("...");
  }
  Serial.println("%%%%");
  
  for (int player_num = 0; player_num < NUM_PLAYER_SCORES_TO_PULL; player_num++) {
    Serial.printf("Setting high scores for player #%d\n", player_num);
    
    if (player_num == 0) {
      ptr = strtok(temp, ",");
    } else {
      ptr = strtok(NULL, ",");
    }
    
    if (ptr) {
      strcpy(high_scores[index][player_num][0], ptr);
      ptr = strtok(NULL, ";");
      strcpy(high_scores[index][player_num][1], ptr);
    } else {
      strcpy(high_scores[index][player_num][0], "");
      strcpy(high_scores[index][player_num][1], "");
    }
  }
}

/**
   Display score of the game just played on the left screen
   Display combo and other details on the right screen
*/
void Interface::display_play_data() {
  char temp[10];
  digitalWrite(cs_pin_left, LOW);
  screen->setTextDatum(MC_DATUM);
  screen->drawString("Your score", MIDDLE_WIDTH, MIDDLE_HEIGHT - 20);
  screen->setTextSize(2);
  sprintf(temp, "%d", game->get_score());
  screen->drawString(temp, MIDDLE_WIDTH, MIDDLE_HEIGHT + 10);
  screen->setTextSize(1);
  digitalWrite(cs_pin_left, HIGH);
  
  digitalWrite(cs_pin_right, LOW);
  screen->drawString("Max combo:", MIDDLE_WIDTH, MIDDLE_HEIGHT - 60);
  screen->drawString("Perfects:",  MIDDLE_WIDTH, MIDDLE_HEIGHT - 25);
  screen->drawString("Decents:",   MIDDLE_WIDTH, MIDDLE_HEIGHT + 10);
  screen->drawString("Misses:",    MIDDLE_WIDTH, MIDDLE_HEIGHT + 45);
  screen->setTextSize(2);
  sprintf(temp, "%d", game->get_max_combo());
  screen->drawString(temp, MIDDLE_WIDTH, MIDDLE_HEIGHT - 47);
  sprintf(temp, "%d", game->get_num_perfect());
  screen->drawString(temp, MIDDLE_WIDTH, MIDDLE_HEIGHT - 12);
  sprintf(temp, "%d", game->get_num_decent());
  screen->drawString(temp, MIDDLE_WIDTH, MIDDLE_HEIGHT + 23);
  sprintf(temp, "%d", game->get_num_missed());
  screen->drawString(temp, MIDDLE_WIDTH, MIDDLE_HEIGHT + 58);
  screen->setTextSize(1);
  screen->setTextDatum(TL_DATUM);
  digitalWrite(cs_pin_right, HIGH);
}

/**
   Upload score of score for user username for song song_index
*/
void Interface::upload_score(uint16_t score) {
  // Get ready to upload your score
  HTTPClient http;
  char url[200];
  strcpy(url, SCORES_ENDPOINT);
  String response;
  char payload[40];
  sprintf(payload, "user=%s&song=%i&score=%i", username, song_index, score);

  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int http_code = http.POST(payload);
  Serial.printf("POST %s\n", payload);
  Serial.println("____");
  if (http_code > 0) {
    Serial.println(http.getString());
  } else {
    Serial.println("Something went wrong with the POST request!");
    Serial.println(url);
    Serial.println("...");
  }
  Serial.println("%%%%");
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
