#ifndef Saber_h
#define Saber_h
#include "Arduino.h"
#include "MPU9250.h"

#define ACCE_HISTORY_SIZE 35
#define ACCE_RECORD_PERIOD 4 // in ms, IMUs can only sample around 250Hz 

class Saber
{
  private:

  uint32_t* note_times;
  char* note_dirs;
  int8_t* note_hit;
  uint8_t total_num_notes;
  uint8_t note_index;

  MPU9250* imu;
  int cs_pin;
  
  uint16_t* score;
  uint16_t* combo;

  uint32_t start_time;
  uint32_t last_acce_time; // last time that acceleration was recorded

  // y direction is toward/away from body, irrelevant
  double x_acce[ACCE_HISTORY_SIZE];
  double z_acce[ACCE_HISTORY_SIZE]; // content shifted by 1g to account for gravity
  int acce_index;

  int8_t match(uint32_t expected_time, char expected_dir); // return 0 for inconclusive, 1 for perfect, 2 for decent, -1 for missed
  uint32_t get_movement_start_time(int entered_correct_dir_index);
  int8_t hit_type(uint32_t expected_time, uint32_t hit_time);
  
  public:
  
  Saber(MPU9250* imu_pointer, int cs); 
  void load(uint32_t* time_list, char* dir_list, int8_t* hit_list, uint8_t num_notes, uint16_t* score_pointer, uint16_t* combo_pointer); 
  void start();
  void process(); 
};

#endif
