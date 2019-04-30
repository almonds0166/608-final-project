#ifndef Interface_h
#define Interface_h
#include "Game.h"
#include "Button.h"

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

  Button* button1;
  Button* button2;

  // set default colors
  const uint16_t BACKGROUND = TFT_BLACK;
  const uint16_t TEXT = TFT_WHITE;

  // set display size
  const int LEFT_EDGE = 0; //left side of screen limit
  const int RIGHT_EDGE = 12; //right side of screen limit
  const int TOP_EDGE = 0; //top of screen limit
  const int BOTTOM_EDGE = 159; //bottom of screen limit
  const int MIDDLE_HEIGHT = BOTTOM_EDGE/2;

  int state; // state of the interface (username, song selection, score display)

  // variables for username selection
  char alphabet[50] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  char username[USERNAME_LENGTH_LIMIT+1] = {0}; 

  char** song_list;
  int song_index;

  void select_username(); // handles selecting a username
  boolean update_song_index();
  void update_song_display();
  void display_recent_scores();
  void display_player_score(uint16_t score);
  void display_high_scores(); 
  void upload_score(/* params tbd */); // upload score, probably want to take score and maybe song name or username as params

  public:

  Interface(Game* game_pointer, TFT_eSPI* tft, int tft_pin_left, int tft_pin_right, Button* button1_pointer, Button* button2_pointer, char** songs);
  void process(); // handles state changes
};

#endif
