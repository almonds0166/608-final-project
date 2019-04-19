#ifndef Game_h
#define Game_h
#include "Arduino.h"
#include "Display.h"
#include "Saber.h"

#define MAX_NOTES 500 // possibly bad practice, oops

/**
 * A game of Beat Revolution, ultimately consisting of two sabers and two displays
 * UI and song selection is not a part of this class, it is just a game of Beat Revolution for a given song name
 * Helper classes, Saber and Display, generally interact with the Game by directly updating and reading data stored in Game
 * The same Game object will be used for the entire time that Beat Revolution runs -- not one Game per song. load and reset
 * are used to set up new songs. Not totally sure if this is the correct design choice. 
 */
class Game
{
  private:

  TFT_eSPI tft = TFT_eSPI(); 
  MPU9255 imu;

  int cs_pin_left = 12;
  int cs_pin_right = 0;
  float rate = 0.05;
  
  // NOTE: start with one saber and one display
  Saber* saber_left = new Saber(&imu);
  Display* display_left = new Display(&tft, rate, cs_pin_left);
  uint32_t note_times_left[MAX_NOTES];
  char note_dirs_left[MAX_NOTES];
  boolean note_hit_left[MAX_NOTES]; // initialize to all False, since special treatment (removing note from display) is for hit note

  Display* display_right = new Display(&tft, rate, cs_pin_right);

  uint8_t bpm;
  uint32_t offset;

  uint16_t score;

  uint32_t song_duration;
  uint32_t start_time;
  
  // TODO: other fields, such as buttons, pointer to where music file is stored, timers, any indices, song length, etc etc

  // TODO: any private helper methods

  
  public:
  
  Game();
  void load(char* song_name);
  void start(char* song_name);
  boolean process();
  void reset();
};

#endif
