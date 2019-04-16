#ifndef Saber_h
#define Saber_h
#include "Arduino.h"

#define ACCE_HISTORY_SIZE 50

class Saber
{
  private:

  // TODO: which of these fields should be fields, and which should just be arguments passed into process()? 
  uint32_t* note_times;
  char* note_dirs;
  boolean* note_hit;
  int note_index;
  
  uint16_t* score;

  uint32_t start_time;

  double x_acce[ACCE_HISTORY_SIZE];
  double y_acce[ACCE_HISTORY_SIZE];
  double x_acce[ACCE_HISTORY_SIZE];
  int acce_index;

  int8_t match(uint32_t expected_time, char expected_dir); // return 0 for inconclusive, 1 for hit, -1 for missed
  
  public:
  
  Saber(/* parameters such as various pins */);
  void start(uint32_t* time_list, char* dir_list, boolean* hit_list);
  void process(/* parameters tbd */); 
};

#endif
