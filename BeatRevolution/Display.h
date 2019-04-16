#ifndef Display_h
#define Display_h
#include "Arduino.h"
#include <TFT_eSPI.h> 
#include <SPI.h>
#include <map>

class Display
{
  private:

  uint32_t* note_times;
  char* note_dirs;
  boolean* note_hit;
  int buff_size;
  TFT_eSPI* screen;
  float ppm; // pixels per ms
  const int thresh = 200; // maximum ms tolerance in the late direction
  // convert char and int versions of direction
  std::map<char, int> char_to_int;
  std::map<int, char> int_to_char;
  // coordinates for arrows at top of screen indicating correct hit time
  /*
  const int arr_x[4] = {23,48,78,103};
  const int arr_y[4] = {20,18,22,20};
  */
  const int arr_x[4] = {18,48,78,108};
  const int arr_y = 20;
  
  // background color
  const uint16_t BACKGROUND = TFT_BLACK;

  //past locations of current 10 arrows to be displayed
  int past_ycoors[10];
  // times for current arrows to be displayed
  uint32_t cur_times[10];
  // directions for current arrows to be displayed
  int cur_dir[10];
  // indices of current arrows
  int cur_ind[10];
  //next insertion into list of 10
  int ind;
  // index of time and direction of next note to be added
  int time_ind;
  int dir_ind;

  uint32_t start_time;

  // TODO: private helpers for displaying
  void draw_arrow(char dir, int x, int y,uint16_t color);
  int calc_center(int dir, uint32_t beat, uint32_t timer);
  uint16_t find_color(uint32_t beat);

  public:
  
  Display(TFT_eSPI* tft, float rate); //rate is pixels per ms
  void start(uint32_t* time_list, char* dir_list, boolean* hit_list); // start timer
  void process(/* parameters tbd */);
};

#endif
