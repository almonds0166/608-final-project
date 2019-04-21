#include "Saber.h"

#define ACCE_HISTORY_SIZE 100
#define ACCE_RECORD_PERIOD 2 // ms

Saber::Saber(MPU9255* imu_pointer) {
  imu = imu_pointer;
}

/**
 * num_notes is the total number of notes in the chart for this hand (to prevent oob errors)
 */
void Saber::load(uint32_t* time_list, char* dir_list, boolean* hit_list, uint8_t num_notes) {
  note_times = time_list;
  note_dirs = dir_list;
  note_hit = hit_list;
  total_num_notes = num_notes;
  note_index = 0;
  
  memset(x_acce, 0, ACCE_HISTORY_SIZE); 
  memset(z_acce, 0, ACCE_HISTORY_SIZE);
  acce_index=0;
}

void Saber::start() {
  start_time = millis();
}

/**
 * Read imu data and update stored accelerate data if needed (depending on time), match against expected 
 * movement (based on index into lists), update index and note_hit if note hit or missed
 */
void Saber::process(/* parameters tbd */) {
  if (note_index >= total_num_notes) {
    return; // index oob, entire chart has been processed
  }

  // read new acceleration data if needed
  if (millis() - last_acce_time >= ACCE_RECORD_PERIOD) { // works best if function called at least once per ms
    imu->readAccelData(imu->accelCount);
    last_acce_time = millis();
    
    float x = (imu->accelCount)[0]*(imu->aRes);
    float z = (imu->accelCount)[2]*(imu->aRes) - 1;
    x_acce[acce_index] = x;
    z_acce[acce_index] = z;

//    char output[40];
//    sprintf(output,"%4.2f,%4.2f",x,z); //render numbers with %4.2 float formatting
//    Serial.println(output); //print to serial for plotting
    delay(0); // NOTE: don't take this line out, it fixes a bug for some reason
    
    // update acce_index, so that it now points to the oldest acceleration
    acce_index++; 
    if (acce_index >= ACCE_HISTORY_SIZE) {
      acce_index=0; // acceleration arrays are cyclic
    }
  }
  
  int8_t outcome = match(note_times[note_index], note_dirs[note_index]);

  if (outcome == 1) { // note hit
    note_hit[note_index] = true;
    note_index++;
    // TODO: update score
  } else if (outcome == -1) { // note missed
    note_index++; // no need to change note_hit, as all contents are initialized to 0
  }
}

// Read stored acceleration data to match against expected movement, return 0 if inconclusive, 1 if hit,
// -1 if missed
// expected time is relative to the start time, so a note at the beginning of the song should have expected
// time of 0
int8_t Saber::match(uint32_t expected_time, char expected_dir) {
//  Serial.println("matching note");
//  Serial.println(expected_time);
//  Serial.println(expected_dir);
  const int beat_lateness_limit = 200;
  
  if (expected_time > millis() - start_time) {
//    Serial.println("too far in future");
    return 0; // beat too far in the future
  }

  if (millis() - start_time - expected_time > beat_lateness_limit) {
//    Serial.println("too far past");
    return -1; // beat too far past
  }
  
  /*
     state 0: default
     state 1: detected initial acceleration in the correct direction and sufficient magnitude
     state 2: detected acceleration in the correct direction and sufficient magnitude for sufficient time, now waiting 
              for acceleration to go from one extreme to the other
     state 3: detected acceleration in the opposite direction and sufficient magnitude
     state 4: detected acceleration in the opposite direction and sufficient magnitude for sufficient time, 
              return hit. state transition doesn't actually happen, just return 
  */
  int state = 0;
  int entered_correct_dir_index = -1;
  int entered_reverse_dir_index = -1;

  const int measurements_above_limit_count = 3; 
  // must spend this many measurements above limit, aka measurements_above_limit_count*ACCE_RECORD_PERIOD ms,
  // above the threshold to count as state transition
  const double acce_limit = 2;
  double cur_acce;
  
  for (int i = acce_index; i < acce_index+ACCE_HISTORY_SIZE; i++) {
//    Serial.println("state");
//    Serial.println(state);
    int clipped_i = i; // usable index into arrays
    if (clipped_i >= ACCE_HISTORY_SIZE) {
      clipped_i -= ACCE_HISTORY_SIZE;
    }
    if (expected_dir == 'u' || expected_dir == 'd') {
      cur_acce = z_acce[clipped_i];
    } else {
      cur_acce = x_acce[clipped_i];
    }
    switch (state) {
      case 0:
      if (expected_dir == 'u' || expected_dir == 'r') {
        // initial acceleration should be positive for those directions
        if (cur_acce >= acce_limit) {
          state = 1;
          entered_correct_dir_index = i;
        }
      } else {
        if (cur_acce <= -acce_limit) {
          state = 1;
          entered_correct_dir_index = i;
        }
      }
      break;

      case 1:
      if (expected_dir == 'u' || expected_dir == 'r') {
        if (cur_acce < acce_limit) { // dropped too soon
          state = 0;
        } else if (i - entered_correct_dir_index >= measurements_above_limit_count) {
          // enough time passed with high acce
          state = 2;
        }
      } else {
        if (cur_acce > -acce_limit) {
          state = 0;
        } else if (i - entered_correct_dir_index >= measurements_above_limit_count) {
          // enough time passed with high acce
          state = 2;
        }
      }
      break;

      case 2:
      if (expected_dir == 'u' || expected_dir == 'r') {
        // reverse acceleration should be negative for those directions
        if (cur_acce <= -acce_limit) {
          state = 3;
          entered_reverse_dir_index = i;
        }
      } else {
        if (cur_acce >= acce_limit) {
          state = 3;
          entered_reverse_dir_index = i;
        }
      }
      break;

      case 3:
      if (expected_dir == 'u' || expected_dir == 'r') {
        if (cur_acce > -acce_limit) { // dropped too soon
          state = 2;
        } else if (i - entered_reverse_dir_index >= measurements_above_limit_count) {
          // enough time passed with high acce
          Serial.println("correct motion detected");
          Serial.println(expected_dir);
          Serial.println();
          return 1;
        }
      } else {
        if (cur_acce < acce_limit) {
          state = 2;
        } else if (i - entered_reverse_dir_index >= measurements_above_limit_count) {
          // enough time passed with high acce
          Serial.println("correct motion detected");
          Serial.println(expected_dir);
          Serial.println();
          return 1;
        }
      }
      break;
    }
  }
  return 0; // beat not too early or too late, but correct motion not detected
}
