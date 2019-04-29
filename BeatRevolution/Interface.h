#ifndef Interface_h
#define Interface_h
#include "Game.h"

#define USERNAME_LENGTH_LIMIT 10

#define USERNAME_STATE 0
#define SONGSELECT_STATE 1
#define GAMEPLAY_STATE 2
#define SCORE_STATE 3

class Interface 
{
  private:

  Game* game;

  TFT_eSPI* screen;
  int cs_pin_left;
  int cs_pin_right;
  int button1; // consider instead copying copy from classy button into a Button class and using Button*
  int button2;

  // set default colors
  const uint16_t BACKGROUND = TFT_BLACK;
  const uint16_t TEXT = TFT_WHITE;

  int state; // state of the interface (username, song selection, score display)

  // variables for username selection
  char alphabet[50] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  char username[USERNAME_LENGTH_LIMIT] = {0}; 

  char** song_list;
  int song_index;

  void select_username(); // handles selecting a username
  void select_song();
  boolean Interface::update_song_index();
  void display_score(); // handles displaying high scores
  void upload_score(/* params tbd */); // upload score, probably want to take score and maybe song name or username as params

  public:

  Interface(Game* game_pointer, TFT_eSPI* tft, int tft_pin1, int tft_pin2, int button_pin1, int button_pin2, char** songs);
  void process(); // handles state changes
};

#endif
