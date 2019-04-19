#include "Arduino.h"
#include "Game.h"
#include <TFT_eSPI.h> 
#include <SPI.h>
#include <mpu9255_esp32.h>

#define MAX_NOTES 500

/**
 * Create game by setting up pins, pointers to arrays, etc. The same Game object will be used the entire time, with the methods 
 * of reset and load to set the game up for different new songs. 
 */
Game::Game(int cs_pin_left, TFT_eSPI* tft_pointer, MPU9255* imu_pointer/* parameters such as various pins */){
  // TODO initialize values, various pins, Sabers and Displays, etc
  float rate_left = 0.05;
  saber_left = Saber(imu_pointer);
  display_left = Display(tft_pointer, rate_left, cs_pin_left);
}

/**
 * Load music, chart, and other data corresponding to the given song_name into fields. If files not found or wifi error, fail gracefully
 */
void Game::load(char* song_name) {
  // TODO @Matthew 
  // parse charts into Game's fields
  // initialize: note_times_left, note_dirs_left, bpm, offset, song_duration from server chart data
  // initialize: note_hit_left to all False, score to 0
  // below is a basic test version that doesn't involve the server
  int total_num_notes_left = 20;
  for (int i=0; i<total_num_notes_left; i++) {
    note_times_left[i] = 1000*(i+1);
    note_dirs_left[i] = 'd';
    note_hit_left[i] = false;
  }

  // load display and saber
  display_left.load(note_times_left, note_dirs_left, note_hit_left, total_num_notes_left);
  saber_left.load(note_times_left, note_dirs_left, note_hit_left, total_num_notes_left);
}

/**
 * Start the game. Set up timers, start playing music. From now on, process() will be called every few milliseconds until the song 
 * is over.
 */
void Game::start(char* song_name) {
  // TODO @Diana start playing music
  // initialize timers
  display_left.start();
  saber_left.start();
}

/**
 * Update the game one timestep (read acce data, match note, update display, etc)
 * return false iff game ended
 */
boolean Game::process() {
  if (millis()-start_time > song_duration) {
    return false;
  } else {
    saber_left.process(/* parameters tbd */);
    display_left.process(/* parameters tbd */);
    return true;
  }
}

void Game::reset() {
  // TODO 
  // clear all arrays, reset all values to defaults, etc, to be ready for next song/game
}
 
