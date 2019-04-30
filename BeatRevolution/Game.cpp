#include "Game.h"
#include <HTTPClient.h>
#include <string.h>
#include <stdlib.h> // atoi

#define CHARTS_ENDPOINT "http://608dev.net/sandbox/sc/almonds/br/chart.py?song=%d&side=%d"
#define MAX_BODY_LENGTH 3000 /* don't set this too high, we only have so much memory */

/**
 * The same Game object will be used the entire time, with the methods of reset and load to set the game up for different new songs. 
 * Objects were already declared statically in the main ino, now set pointers to correct places
 */
Game::Game(Saber** saber_pointers, Display** display_pointers, DFRobotDFPlayerMini* mp3_pointer, char** song_names) {
  for (uint8_t side = 0; side < 2; side++) {
    sabers[side]   = saber_pointers[side];
    displays[side] = display_pointers[side];
  }
  mp3 = mp3_pointer;
  song_list = song_names;
}

/**
 * Load music, chart, and other data corresponding to the given song_index into fields. If files not found or wifi error, fail gracefully
 */
void Game::load(int song_index) {
  // initialize the player's score to zero, happens before loading sabers and displays with score's address
  score = 0;
  
  // Get ready to download chart list
  HTTPClient http;
  char url[200];
  String response;
  char body[MAX_BODY_LENGTH];
  char* ptr;

  // get charts for and initialize both the left & right sides!
  for (uint8_t side = 0; side < 2; side++) {
    strcpy(body, "");
    sprintf(url, CHARTS_ENDPOINT, song_index, side);
    http.begin(url);
    int http_code = http.GET();
    Serial.println("____");
    if (http_code > 0) {
      response = http.getString(); // offset,bpm:ts,dir;ts,dir;ts,dir;...
      int i = 0;
      while (i < MAX_BODY_LENGTH) {
        body[i] = response[i];
        if(response[i++] == '\0') break;
      }
      Serial.printf("GET %s\n", url);
      Serial.printf("Response:\n%s\n", body);
    } else {
      Serial.println("Something went wrong with the GET request!");
      Serial.println(url);
      Serial.println("...");
      delay(500000);
    }
    Serial.println("%%%%");
    
    ptr = strtok(body, ","); offset = atoi(ptr); // first number returned is the offset
    ptr = strtok(NULL, ":"); bpm    = atoi(ptr); // second number returned is the BPM
    // the remaining numbers are timestamp,direction pairs
    ptr = strtok(NULL, ",");
    while (ptr != NULL) {
      note_times[side][total_num_notes[side]]  = atoi(ptr) + offset;
      note_dirs[side][total_num_notes[side]++] = (strtok(NULL, ";"))[0];
      ptr = strtok(NULL, ",");
    }
  
    for (int i = 0; i < total_num_notes[side]; i++)
      note_hit[side][i] = false;

    // Load sabers and displays
    sabers[side]->load(note_times[side], note_dirs[side], note_hit[side], total_num_notes[side], &score);
    displays[side]->load(bpm, offset, note_times[side], note_dirs[side], note_hit[side], total_num_notes[side], &score);
  }
  
  // TODO: include song_duration into the song's text file (similar to bpm and offset) and load that
  song_duration = 123000;
}

/**
 * Start the game. Set up timers, start playing music. From now on, process() will be called every few milliseconds until the song 
 * is over.
 */
void Game::start(int song_index) {
  mp3->play(song_index);
  start_time = millis();
  
  sabers[0]->start(); 
  sabers[1]->start();
  displays[0]->start(); 
  displays[0]->print_song(song_list[song_index]);
  displays[1]->start();
  displays[1]->update_score();
}

/**
 * Update the game one timestep (read acce data, match note, update display, etc)
 * return true iff game ended
 */
boolean Game::process() {
  if (millis()-start_time > song_duration) {
    return true;
  } else {
    sabers[0]->process();
    sabers[1]->process();
    displays[0]->process();
    displays[1]->process();
    displays[1]->update_score();
    return false;
  }
}

void Game::reset() {
  // TODO 
  // clear all arrays, reset all values to defaults, etc, to be ready for next song/game
  // might not actually need this function
}

uint16_t Game::get_score() {
  return score;
}
 
