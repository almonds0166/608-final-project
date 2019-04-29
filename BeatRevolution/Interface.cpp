#include "Interface.h"

Interface::Interface(Game* game_pointer, TFT_eSPI* tft, int tft_pin1, int tft_pin2, int button_pin1, int button_pin2, char* songs) {
  game = game_pointer;
  screen = tft;
  cs_pin_left = tft_pin1;
  cs_pin_right = tft_pin2;
  button1 = button_pin1;
  button2 = button_pin2;

  state = USERNAME_STATE;
  char_index = 0;

  song_list = songs;
}

void Interface::process() {
  switch (state) {
    case USERNAME_STATE: {
      select_username();
      break;
    }
    case SONGSELECT_STATE: {
      select_song();
      break;
    }
    case GAMEPLAY_STATE: {
      boolean complete = game->process();
      if (complete) {
        upload_score(/* params tbd */);
        state = SCORE_STATE;
      }
      break;
    }
    case SCORE_STATE: {
      display_score();
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

  // TODO
}

void Interface::select_song() {
  // display list of songs on left screen
  // display recent scores of currently selected song on right screen (lower priority)

  // if short press up button, move up in song list (unless on first song, in which case do nothing)
  // if short press down button, move down in song list (unless on last song, in which case do nothing)
  // if long press down button, do game->load() and game->start(), then move into GAMEPLAY state

  // TODO
}

void Interface::display_score() {
  // display player score and possibly some data about play on left screen
  // display list of high scores on right screen

  // long press either button to move into SONGSELECT state

  // TODO
}

void upload_score(/* params tbd */) {
  // TODO
}
