#include "Game.h"
#include <string.h>
#include <stdlib.h> // atoi

#define CHARTS_ENDPOINT "http://608dev.net/sandbox/sc/almonds/br/chart.py?song=%d&side=%d"
#define MAX_BODY_LENGTH 4000 /* don't set this too high, we only have so much memory */

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
 * Load music, chart, and other data corresponding to the given song_index into fields. If files not found or wifi error, return false,
 * otherwise, return true
 */
boolean Game::load(int song_index) {
  // initialize these values before loading sabers and displays with their addresses
  score = 0;
  cur_combo = 0;
  max_combo = 0;
  
  // Get ready to download chart list
  HTTPClient http;
  char url[200];
  String response;
  char body[MAX_BODY_LENGTH];
  char* ptr;

  // get charts for and initialize both the left & right sides!
  for (uint8_t side = 0; side < 2; side++) {
    total_num_notes[side] = 0;
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
      return false; // load failed 
    }
    Serial.println("%%%%");
    
    ptr = strtok(body, ","); song_duration = atoi(ptr); // first number returned is the duration
    ptr = strtok(NULL, ","); offset = atoi(ptr); // second number returned is the offset
    ptr = strtok(NULL, ":"); bpm    = atoi(ptr); // third number returned is the BPM
    // the remaining numbers are timestamp,direction pairs
    ptr = strtok(NULL, ",");
    while (ptr != NULL) {
      note_times[side][total_num_notes[side]]  = atoi(ptr) + offset;
      note_dirs[side][total_num_notes[side]++] = (strtok(NULL, ";"))[0];
      ptr = strtok(NULL, ",");
    }
  
    for (int i = 0; i < total_num_notes[side]; i++)
      note_hit[side][i] = 0;

    // Load sabers and displays
    float rate = 0.12;
    sabers[side]->load(note_times[side], note_dirs[side], note_hit[side], total_num_notes[side], &score, &cur_combo);
    displays[side]->load(bpm, offset, rate, note_times[side], note_dirs[side], note_hit[side], total_num_notes[side], &score, &cur_combo);
  }
  
  return true;
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
    if (cur_combo > max_combo) {
      max_combo = cur_combo;
    }
    return false;
  }
}

void Game::stop() {
  mp3->pause();
}

uint16_t Game::get_score() {
  return score;
}

uint16_t Game::get_max_combo() {
  return max_combo;
}
 
