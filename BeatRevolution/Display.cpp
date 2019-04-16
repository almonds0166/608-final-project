#include "Arduino.h"
#include "Display.h"

Display::Display(TFT_eSPI* tft, float rate){
  ppm = rate;
  //initialize map
  char_to_int['l'] = 0;
  char_to_int['d'] = 1;
  char_to_int['u'] = 2;
  char_to_int['r'] = 3;
  int_to_char[0] = 'l';
  int_to_char[1] = 'd';
  int_to_char[2] = 'u';
  int_to_char[3] = 'r';
  ind = 0;
  screen = tft;
  screen->fillScreen(BACKGROUND);

  // for testing, change later
  for(int i = 0; i<10; i++)
  {
    cur_dir[i] = i % 4;
    cur_times[i] = 1000*(i+1);
    past_ycoors[i] = -100;
  }
}

// start song/timer
void Display::start() {
  start_time = millis();
}

void Display::process(/* parameters tbd */) {
  // use constant current time for all arrows
  long cur_timer = millis();
  //blank the previous arrows
  for(int i = 0; i<10; i++)
  {
    if(past_ycoors[i]>0)
    {
      draw_arrow(int_to_char[cur_dir[i]],arr_x[cur_dir[i]],past_ycoors[i],BACKGROUND);
    }
    //new y coordinate of arrow
    int y_coor = calc_center(cur_dir[i],cur_times[i],cur_timer);
    if(y_coor > -2)
    {
      past_ycoors[i] = y_coor;
    } else {
      // will be to take the next note
      past_ycoors[i] = -100;
    }
  }
  Serial.println("new y values");
  for(int i = 0; i<10; i++)
  {
    Serial.println(past_ycoors[i]);
  }
  // arrows at top of screen
  for(int i = 0; i<4; i++)
  {
    draw_arrow(int_to_char[i],arr_x[i],arr_y[i],TFT_WHITE);
  }
  //draw arrows in new locations
  for(int i = 0; i<10; i++)
  {
    if(past_ycoors[i] > 0)
    {
      draw_arrow(int_to_char[cur_dir[i]],arr_x[cur_dir[i]],past_ycoors[i],find_color(cur_times[i]));
    }
  }
}

// given time of beat and direction, find current y-center of arrow
int Display::calc_center(int dir, long beat, long timer) {
  long time_until = beat-timer+start_time;
  Serial.println(time_until);
  int disp = int(time_until*ppm);
  if(disp > 132)
  {
    return -1; // too far away to display on screen
  } else if (time_until < -1*thresh) // too far in other direction
  {
    return -2;
  }
  return (arr_y[dir] + disp);
}

// draw arrow on screen with direction dir, rectangle center (x,y)
void Display::draw_arrow(char dir, int x, int y,uint16_t color){
  // triangle points
  int x0;
  int x1;
  int x2;
  int y0;
  int y1;
  int y2;
  switch(dir) {
    case 'u':
    {
      x0 = x-6;
      x1 = x+6;
      x2 = x;
      y0 = y-4;
      y1 = y-4;
      y2 = y-10;
    }
    break;
    case 'd':
    {
      x0 = x-6;
      x1 = x+6;
      x2 = x;
      y0 = y+4;
      y1 = y+4;
      y2 = y+10;
    }
    break;
    case 'l':
    {
      x0 = x-4;
      x1 = x-4;
      x2 = x-10;
      y0 = y-6;
      y1 = y+6;
      y2 = y;
    }
    break;
    case 'r':
    {
      x0 = x+4;
      x1 = x+4;
      x2 = x+10;
      y0 = y-6;
      y1 = y+6;
      y2 = y;
    }
    break;
  }
  screen->fillRect(x-3,y-3,7,7,color);
  screen->fillTriangle(x0,y0,x1,y1,x2,y2,color);
}

// given the timing of a beat, determine the color of the arrow
uint16_t Display::find_color(long beat) {
  return TFT_GREEN;
}
