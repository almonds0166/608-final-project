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
  uint16_t* score_ptr;
  uint16_t prev_score;
  float mspb; //ms per beat
  uint32_t offset;
  uint32_t recent_hits[4]; // time of most recent hits in each direction
  boolean make_glow[4]; // determine if arrow should glow
  
  uint8_t buff_size;
  TFT_eSPI* screen;
  int cs_pin;
  float ppm; // pixels per ms
  
  const int thresh = 200; // maximum ms tolerance in the late direction
  const int pixel_thresh = 122; // limit at bottom of screen for arrow display
  // convert char and int versions of direction
  std::map<char, int> char_to_int;
  std::map<int, char> int_to_char;
  // coordinates for arrows at top of screen indicating correct hit time
  const int arr_x[4] = {18,48,78,108};
  const int arr_y = 20;
  
  // background color
  const uint16_t BACKGROUND = TFT_BLACK;
  // off-white default arrow color
  const uint16_t OFF_WHITE = 23275;

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

  // private helpers for displaying
  void draw_arrow(char dir, int x, int y,uint16_t color);
  int calc_center(int dir, uint32_t beat, uint32_t timer);
  uint16_t find_color(uint32_t beat);
  void translate_arrow(char dir, int x, int y, uint16_t color);
  uint16_t glow_arrow(uint32_t full_bright);

  public:
  
  Display(TFT_eSPI* tft, float rate, int cs); //rate is pixels per ms
  void load(uint32_t beats_per_minute, uint32_t off, uint32_t* time_list, char* dir_list, boolean* hit_list, int num_notes, uint16_t* score_loc); 
  void start(); // start timer
  void print_song(char* song_name);
  void update_score();
  void process();
};

#endif
