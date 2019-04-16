#include "Arduino.h"
#include "Saber.h"

Saber::Saber(/* parameters such as various pins */){
  
}

// Read stored acceleration data to match against expected movement, return 0 if inconclusive, 1 if hit,
// -1 if missed
int8_t match(uint32_t expected_time, char expected_dir) {
  
}

void Saber::start(uint32_t* time_list, char* dir_list, boolean* hit_list) {
  note_times = time_list;
  note_dirs = dir_list;
  note_hit = hit_list;
  note_index = 0;
  start_time = millis();
}

/**
 * Read imu data and update stored accelerate data if needed (depending on time), match against expected 
 * movement (based on index into lists), update index and note_hit if note hit or missed
 */
void Saber::process(/* parameters tbd */) {
  
}
