#include "Saber.h"

Saber::Saber(MPU9250* imu_pointer, int cs) {
  imu = imu_pointer;
  cs_pin = cs;
}

/**
 * num_notes is the total number of notes in the chart for this hand (to prevent oob errors)
 */
void Saber::load(uint32_t* time_list, char* dir_list, int8_t* hit_list, uint8_t num_notes, uint16_t* score_pointer, uint16_t* combo_pointer, 
    uint16_t* num_notes_perfect_pointer, uint16_t* num_notes_decent_pointer, uint16_t* num_notes_missed_pointer) {
  note_times = time_list;
  note_dirs = dir_list;
  note_hit = hit_list;
  total_num_notes = num_notes;
  score = score_pointer;
  combo = combo_pointer;
  num_notes_perfect = num_notes_perfect_pointer;
  num_notes_decent = num_notes_decent_pointer;
  num_notes_missed = num_notes_missed_pointer;
  
  note_index = 0;
  acce_index=0;
}

void Saber::start() {
  start_time = millis();
  last_acce_time = millis();
}

/**
 * Read imu data and update stored accelerate data if needed (depending on time), match against expected 
 * movement (based on index into lists), update index and note_hit if note hit or missed
 */
void Saber::process() {
  if (note_index >= total_num_notes) {
    return; // index oob, entire chart has been processed
  }

  // read new acceleration data if needed
  if (millis() - last_acce_time >= ACCE_RECORD_PERIOD) { // works best if function called at least once per ms
    // the imu library already handles writing LOW/HIGH to the CS select pin
    imu->fastReadSensor();
//    imu->readSensor();
    float x = imu->getAccelY_mss(); // what was previous the x direction on the imu is now the y direction in the new library
                                    // a motion to the right starts positive and ends negative
    float z = -(imu->getAccelZ_mss() /*+ 9.8*/); // a motion up starts positive and ends negative
    x_acce[acce_index] = x;
    z_acce[acce_index] = z;

    last_acce_time = millis();

    char output[40];
    sprintf(output,"%d,%4.2f,%4.2f",cs_pin,x,z); //render numbers with %4.2 float formatting
    Serial.println(output); //print to serial for plotting
    
    // update acce_index, so that it now points to the oldest acceleration
    acce_index++; 
    if (acce_index >= ACCE_HISTORY_SIZE) {
      acce_index=0; // acceleration arrays are cyclic
    }
  }

  boolean disconnected = true;
  // check whether the saber has disconnected, reconnect if so 
  // not really a good solution, but at least startup only takes less than 0.1 seconds now
  for (int i = 0; i < ACCE_HISTORY_SIZE - 1; i++) {
    if (x_acce[i] != x_acce[i+1] || z_acce[i] != z_acce[i+1]) {
      // readings not broken
      disconnected = false;
      break;
    }
  }
  if (disconnected) { 
    Serial.println("Reconnecting saber with cs pin");
    Serial.println(cs_pin);
    calibrate();
  }
  
  int8_t outcome = match(note_times[note_index], note_dirs[note_index]);

  note_hit[note_index] = outcome;
  if (outcome == 1) { // perfect hit
    note_index++;
    *score = *score + 100 + *combo;
    *combo = *combo + 1;
    *num_notes_perfect = *num_notes_perfect + 1;
  } else if (outcome == 2) { // decent hit
    note_index++;
    *score = *score + 70 + *combo;
    *combo = *combo + 1;
    *num_notes_decent = *num_notes_decent + 1;
  } else if (outcome == -1) { // note missed
    note_index++; 
    *combo = 0;
    *num_notes_missed = *num_notes_missed + 1;
  }
}

// Read stored acceleration data to match against expected movement, return 0 if inconclusive, 1 if perfect, 2 if decent
// -1 if missed
// expected time is relative to the start time, so a note at the beginning of the song should have expected
// time of 0
int8_t Saber::match(uint32_t expected_time, char expected_dir) {
  const int beat_earliness_limit = 100;
  // a beat can be matched as early as beat_earliness_limit ms before the expected time (since a movement takes time, it's detected after it's made
  const int beat_lateness_limit = 140;
  // if a beat is not detected for beat_lateness_limit ms after the expected timestamp, mark as missed
  const int first_measurements_above_limit_count = 7; 
  const int second_measurements_above_limit_count = 5; 
  const double first_acce_limit = 15; // in m/s/s
  const double second_acce_limit = 12;
  // must spend this many measurements above limit, aka measurements_above_limit_count*ACCE_RECORD_PERIOD ms,
  // above the 1st/2nd threshold to count as 1st(0->1)/2nd(2->3) state transitions

  // don't rearrange to have minuses in the inequality, all numbers involves are unsigned
  if (start_time + expected_time > millis() + beat_earliness_limit) { 
    return 0; // beat too far in the future
  }

  if (millis() > start_time + expected_time + beat_lateness_limit) {
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
        if (cur_acce >= first_acce_limit) {
          state = 1;
          entered_correct_dir_index = i;
        }
      } else {
        if (cur_acce <= -first_acce_limit) {
          state = 1;
          entered_correct_dir_index = i;
        }
      }
      break;

      case 1:
      if (expected_dir == 'u' || expected_dir == 'r') {
        if (cur_acce < first_acce_limit) { // dropped too soon
          state = 0;
        } else if (i - entered_correct_dir_index >= first_measurements_above_limit_count) {
          // enough time passed with high acce
          state = 2;
        }
      } else {
        if (cur_acce > -first_acce_limit) {
          state = 0;
        } else if (i - entered_correct_dir_index >= first_measurements_above_limit_count) {
          // enough time passed with high acce
          state = 2;
        }
      }
      break;

      case 2:
      if (expected_dir == 'u' || expected_dir == 'r') {
        // reverse acceleration should be negative for those directions
        if (cur_acce <= -second_acce_limit) {
          state = 3;
          entered_reverse_dir_index = i;
        }
      } else {
        if (cur_acce >= second_acce_limit) {
          state = 3;
          entered_reverse_dir_index = i;
        }
      }
      break;

      case 3:
      if (expected_dir == 'u' || expected_dir == 'r') {
        if (cur_acce > -second_acce_limit) { // dropped too soon
          state = 2;
        } else if (i - entered_reverse_dir_index >= second_measurements_above_limit_count) {
          // enough time passed with high acce
          uint32_t entered_correct_dir_time = get_movement_start_time(entered_reverse_dir_index);
          return hit_type(expected_time, entered_correct_dir_time);
        }
      } else {
        if (cur_acce < second_acce_limit) {
          state = 2;
        } else if (i - entered_reverse_dir_index >= second_measurements_above_limit_count) {
          // enough time passed with high acce
          uint32_t entered_correct_dir_time = get_movement_start_time(entered_reverse_dir_index);
          return hit_type(expected_time, entered_correct_dir_time);
        }
      }
      break;
    }
  }
  return 0; // beat not too early or too late, but correct motion not detected
}

/**
 * Given the index in the acceleration array of where a movement was first detected, return when the movement started 
 * (offset by some amount, since detection is earlier than actual movement)
 */
uint32_t Saber::get_movement_start_time(int correct_hit_index) {
  return millis() - ACCE_RECORD_PERIOD * (ACCE_HISTORY_SIZE + acce_index - correct_hit_index - 1); // plus offset if needed
}

/**
 * Return -1 for missed (too far), 1 for perfect (within 60 ms in either direction), 2 for decent (within 100 ms in either direction)
 * expected_time is relative to start time
 */
int8_t Saber::hit_type(uint32_t expected_time, uint32_t hit_time) {
  uint32_t absolute_expected_time = expected_time + start_time;

  Serial.println("detecting hit type with expected time, hit time, and diff:");
  Serial.println(absolute_expected_time);
  Serial.println(hit_time);
  
  if (absolute_expected_time >= hit_time) {
    uint32_t diff = absolute_expected_time - hit_time;
    Serial.println(diff);
    if (diff <= 70) {
      return 1;
    } else {
      return 0;
    }
  } else {
    uint32_t diff = hit_time - absolute_expected_time;
    Serial.println(diff);
    if (diff <= 70) {
      return 1;
    } else if (diff <= 120) {
      return 2;
    } else {
      return -1;
    }
  }
}

void Saber::calibrate() {
  Serial.println("Start up time");
  Serial.println(millis());
  int status = imu->fastBegin(0); // setting srd to 0 now actually works in the custom begin function i think? no longer get defaulted to 19
//  int status = imu->begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Status");
    Serial.println(status);
    while(true) {}
  }
//  imu->fastCalibrateAccel();
  imu->defaultCalibrateAccel();
  
  Serial.println("IMU connected and set up!");
  float x_bias = imu->getAccelBiasY_mss();
  float z_bias = imu->getAccelBiasZ_mss();
  Serial.println("x bias:");
  Serial.println(x_bias);
  Serial.println("z bias:");
  Serial.println(z_bias);

  memset(x_acce, 0, ACCE_HISTORY_SIZE); 
  x_acce[0] = 0.01; // very hacky sorry, this is to avoid the IMUs being reset due to acceleration history being all the same
  memset(z_acce, 0, ACCE_HISTORY_SIZE);

  Serial.println("End time");
  Serial.println(millis());
}
