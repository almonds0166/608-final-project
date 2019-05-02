#ifndef Button_h
#define Button_h
#include "Arduino.h"

class Button {
  private:
  uint32_t t_of_state_2;
  uint32_t t_of_button_change;    
  uint32_t debounce_time;
  uint32_t long_press_time;
  uint32_t last_updated_time;
  uint32_t update_timeout;
  uint8_t pin;
  uint8_t flag;
  bool button_pressed;

  int state; 

  public:
  
  Button(int p);
  void read();
  int update();
};

#endif
