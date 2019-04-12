#ifndef Saber_h
#define Saber_h
#include "Arduino.h"

#define ACCE_HISTORY_SIZE 50

class Saber
{
  private:

  // TODO: which of these fields should be fields, and which should just be arguments passed into process()? 
  long* note_times;
  char* note_dirs;
  int* note_index;
  boolean* last_note_was_hit;
  uint16_t* score;

  uint32_t start_time;

  double x_acce[ACCE_HISTORY_SIZE];
  double y_acce[ACCE_HISTORY_SIZE];
  double x_acce[ACCE_HISTORY_SIZE];
  int acce_index;
  
  public:
  
  Saber(/* parameters such as various pins */);
  void process(/* parameters tbd */);
};

#endif
