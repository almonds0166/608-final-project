#ifndef Saber_h
#define Saber_h
#include "Arduino.h"
#include <mpu9255_esp32.h>

#define ACCE_HISTORY_SIZE 100
#define ACCE_RECORD_PERIOD 2 // ms

class Saber
{
  private:

  // TODO: which of these fields should be fields, and which should just be arguments passed into process()? 
  uint32_t* note_times;
  char* note_dirs;
  boolean* note_hit;
  uint8_t total_num_notes;
  uint8_t note_index;

  MPU9255* imu;
  
  uint16_t* score;

  uint32_t start_time;

  // y direction is toward/away from body, irrelevant
  double x_acce[ACCE_HISTORY_SIZE];
  double z_acce[ACCE_HISTORY_SIZE]; // content shifted by 1g to account for gravity
  int acce_index;

  int8_t match(uint32_t expected_time, char expected_dir); // return 0 for inconclusive, 1 for hit, -1 for missed
  
  public:
  
  Saber(MPU9255* imu_pointer); // note, need to change pointer type if we make another imu class
  void load(uint32_t* time_list, char* dir_list, boolean* hit_list, uint8_t num_notes);
  void start();
  void process(/* parameters tbd */); 
};

#endif
