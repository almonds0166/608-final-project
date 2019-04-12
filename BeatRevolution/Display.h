#ifndef Display_h
#define Display_h
#include "Arduino.h"

class Display
{
  private:

  long* note_times;
  char* note_dirs;

  uint32_t start_time;

  // TODO: some kind of array for keeping track of which notes are hit and which are not

  // TODO: private helpers for displaying

  public:
  
  Display(/* parameters such as various pins */);
  void process(/* parameters tbd */);
  // TODO: other display methods as needed
};

#endif
