#ifndef Game_h
#define Game_h
#include "Arduino.h"
#include "Display.h"
#include "Saber.h"
#include <DFRobotDFPlayerMini.h>

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

  // index of 0 is left side
  // index of 1 is right side
  Saber*   sabers[2];
  Display* displays[2];
  DFRobotDFPlayerMini* mp3;
  
  uint16_t total_num_notes[2];
  uint32_t note_times[2][MAX_NOTES]; // note, note_times already account for offset, so that the first note is equal to offset not 0
  char     note_dirs[2][MAX_NOTES];
  boolean  note_hit[2][MAX_NOTES];

  char** song_list;

  uint8_t bpm;
  uint32_t offset;

  uint16_t score;

  uint32_t song_duration;
  uint32_t start_time;
  
  public:
  
  Game(Saber** saber_pointers, Display** display_pointers, DFRobotDFPlayerMini* mp3_pointer, char** song_names);
  void load(int song_index); 
  void start(int song_index);
  boolean process();
  void reset();
  uint16_t get_score();
};

#endif
