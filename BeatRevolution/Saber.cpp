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
  if (millis()%ACCE_RECORD_PERIOD == 0) { // only functions properly if Saber::process is called at least once each ms
    imu->readAccelData(imu->accelCount);
    float x = (imu->accelCount)[0]*(imu->aRes);
    float z = (imu->accelCount)[2]*(imu->aRes);
    x_acce[acce_index] = x;
    z_acce[acce_index] = z;
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
int8_t Saber::match(uint32_t expected_time, char expected_dir) {
  // TODO
  return 0;
}
