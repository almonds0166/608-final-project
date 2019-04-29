#include "Interface.h"

Interface::Interface(Game* game_pointer, TFT_eSPI* tft, int tft_pin1, int tft_pin2, int button_pin1, int button_pin2, char** songs) {
  game = game_pointer;
  screen = tft;
  cs_pin_left = tft_pin1;
  cs_pin_right = tft_pin2;
  button1 = button_pin1;
  button2 = button_pin2;

  state = USERNAME_STATE;

  song_list = songs;
  song_index = 0;
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

      /*
      if (down button long pressed) { 
        game->load();
        game->start();
        state = GAMEPLAY_STATE;
        break;
      }
       */
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
  // display current username on left screen
  
  // if short press up button, roll backward in char list
  // if short press down button, roll forward in chart list
  // if long press up button, move onto next letter 
  // if long press down button, save username into field, move to SONGSELECT state

  // it's probably fine to do a while loop in here that doesn't exit until username is selected, then global variables are not needed

  // TODO @diana
}

/**
 * Read button inputs, update song_index if needed
 * return true iff an update occurred. In particular, if the up button is pressed but we were already at the first song 
 * and can't move up further, don't return true.
 */
boolean Interface::update_song_index() {
  // TODO @diana
}

void Interface::update_song_display() {
  // selected song is in the middle of left screen, and up to n (where n is probably like 3) songs before and after the
  // selected song are also displayed
  // TODO @diana
  digitalWrite(cs_pin_left, LOW);
  // do stuff
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
  // TODO @diana
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
