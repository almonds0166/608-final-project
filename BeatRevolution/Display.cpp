#include "Display.h"

/**
   tft is a pointer to the tft that ths display is writing to
   rate is the pixels-per-ms movement of arrows from bottom up
   cs is the pin number of the chip select correponding to tft -- changes to the display will be reflected
   iff the cs pin is set to low
*/
Display::Display(TFT_eSPI* tft, int cs) {
  cs_pin = cs;
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

  // initialize past_ycoors
  for (int i = 0; i < 10; i++)
  {
    past_ycoors[i] = -4;
  }
  // initialize glows
  for (int i = 0; i < 4; i++)
  {
    make_glow[i] = false;
  }
}

void Display::load(uint32_t beats_per_minute, uint32_t off, float rate, uint32_t* time_list, char* dir_list, boolean* hit_list, int num_notes, uint16_t* score_loc) {
  ppm = rate;
  mspb = 60000.0/beats_per_minute;
  offset = off;
  note_times = time_list;
  note_dirs = dir_list;
  note_hit = hit_list;
  buff_size = num_notes;
  score_ptr = score_loc;
  prev_score = -1; // so that 0 will be displayed initially

  ind = 0;
  for (int i = 0; i < 10; i++)
  {
    past_ycoors[i] = -4;
  }
  // initialize glows
  for (int i = 0; i < 4; i++)
  {
    make_glow[i] = false;
  }
  start_time = millis();
}

// start song/timer
void Display::start() {
  start_time = millis();
}

// display song title
void Display::print_song(char* song_name) {
  digitalWrite(cs_pin, LOW);
  screen->setCursor(0,pixel_thresh+28,1);
  screen->println(song_name);
  digitalWrite(cs_pin, HIGH);
}

// display current score
void Display::update_score() {
  uint16_t cur_score = *score_ptr;
  // rewrite display only if score changed
  if(cur_score != prev_score)
  {
    digitalWrite(cs_pin, LOW);
    screen->fillRect(0,pixel_thresh+28,120,10,BACKGROUND);
    screen->setCursor(0,pixel_thresh+28,1);
    screen->print("Score: ");
    screen->println(cur_score);
    digitalWrite(cs_pin, HIGH);
    prev_score = cur_score;
  }
}

void Display::process() {
  // use constant current time for all arrows
  uint32_t cur_timer = millis();

  digitalWrite(cs_pin, LOW);
  // arrows at top of screen
  for (int i = 0; i < 4; i++)
  {
    if(make_glow[i]) //arrow should glow and fade
    {
      // check that too much time has not elapsed
      uint16_t faded_color = glow_arrow(recent_hits[i], accuracies[i]);
      if (faded_color > 0) {
        // draw arrow in appropriate color at this point in time
        draw_arrow(int_to_char[i], arr_x[i], arr_y, faded_color);
      } else {
        make_glow[i] = false;
      }
    } else { // draw normal arrow
      draw_arrow(int_to_char[i], arr_x[i], arr_y, OFF_WHITE);
    }
  }

  //translate arrows on screen
  for (int i = 0; i < 10; i++)
  {
    if (past_ycoors[i] > -2) // valid upcoming note, possibly displayed
    {
      int y_coor = calc_center(cur_dir[i], cur_times[i], cur_timer); // new center
      if (y_coor > -2 and (not note_hit[cur_ind[i]])) // note not missed or hit
      {
        if (y_coor > -1 and past_ycoors[i] == -1) //first time arrow appears on screen
        {
          draw_arrow(int_to_char[cur_dir[i]], arr_x[cur_dir[i]], y_coor, find_color(cur_times[i]));
        } else if (past_ycoors[i] > -1) { // arrow already on screen, translate
          for (int y_cent = past_ycoors[i]; y_cent > y_coor; y_cent--) // iterate over centers
          {
            translate_arrow(int_to_char[cur_dir[i]], arr_x[cur_dir[i]], y_cent, find_color(cur_times[i]));
          }
        }
        //update past center
        past_ycoors[i] = y_coor;
      } else { // note missed or hit, go to next note in list
        //blank previous arrow
        draw_arrow(int_to_char[cur_dir[i]], arr_x[cur_dir[i]], past_ycoors[i], BACKGROUND);
        // note hit, tell bottom white arrow to do stuff
        if (note_hit[cur_ind[i]])
        { 
          make_glow[cur_dir[i]] = true;
          recent_hits[cur_dir[i]] = millis();
          accuracies[cur_dir[i]] = 2; //flash white
        } else { // note missed, show purple for now; this will be taken out later
          make_glow[cur_dir[i]] = true;
          recent_hits[cur_dir[i]] = millis();
          accuracies[cur_dir[i]] = 0;
        }
        if (ind < buff_size)
        {
          cur_dir[i] = char_to_int[note_dirs[ind]];
          cur_times[i] = note_times[ind];
          cur_ind[i] = ind;
          ind++;
          past_ycoors[i] = -1; // handle on next loop
        } else { // no more valid arrows
          past_ycoors[i] = -4;
        }
      }
    } else if (ind < buff_size) { // switch to next valid note
      cur_dir[i] = char_to_int[note_dirs[ind]];
      cur_times[i] = note_times[ind];
      cur_ind[i] = ind;
      ind++;
      past_ycoors[i] = -1; // handle next loop
    } else { // no more valid arrows
      past_ycoors[i] = -4;
    }
  }
  digitalWrite(cs_pin, HIGH);
}

// given time of beat and direction, find current y-center of arrow
int Display::calc_center(int dir, uint32_t beat, uint32_t timer) {
  int32_t time_until = beat + start_time - timer;
  int disp = int(time_until * ppm);
  if (disp > pixel_thresh)
  {
    return -1; // too far away to display on screen
  } else if (time_until < -1 * thresh or disp < -10) // too far in other direction
  {
    return -2;
  }
  return (arr_y + disp);
}

//draw arrow on screen with direction dir, center (x,y)
void Display::draw_arrow(char dir, int x, int y, uint16_t color) {
  //triangle points
  int x0;
  int x1;
  int x2;
  int y0;
  int y1;
  int y2;
  //rectangle stuff
  int rx;
  int ry;
  int rl;
  int rw;
  switch (dir) {
    case 'u':
      {
        x0 = x - 6;
        x1 = x + 6;
        x2 = x;
        y0 = y;
        y1 = y;
        y2 = y - 6;
        rx = x - 1;
        ry = y;
        rl = 3;
        rw = 6;
      }
      break;
    case 'd':
      {
        x0 = x - 6;
        x1 = x + 6;
        x2 = x;
        y0 = y;
        y1 = y;
        y2 = y + 6;
        rx = x - 1;
        ry = y - 5;
        rl = 3;
        rw = 6;
      }
      break;
    case 'l':
      {
        x0 = x;
        x1 = x;
        x2 = x - 6;
        y0 = y - 6;
        y1 = y + 6;
        y2 = y;
        rx = x;
        ry = y - 1;
        rl = 6;
        rw = 3;
      }
      break;
    case 'r':
      {
        x0 = x;
        x1 = x;
        x2 = x + 6;
        y0 = y - 6;
        y1 = y + 6;
        y2 = y;
        rx = x - 5;
        ry = y - 1;
        rl = 6;
        rw = 3;
      }
      break;
  }
  screen->fillRect(rx, ry, rl, rw, color);
  screen->fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

//given arrow with center (x,y), translate arrow one pixel up
void Display::translate_arrow(char dir, int x, int y, uint16_t color) {
  switch (dir)
  {
    case 'u':
      {
        //blank bottom outline
        screen->drawLine(x - 1, y + 5, x + 1, y + 5, BACKGROUND);
        screen->drawLine(x - 6, y, x - 2, y, BACKGROUND);
        screen->drawLine(x + 2, y, x + 6, y, BACKGROUND);
        //extend top of arrow
        screen->drawLine(x - 6, y - 1, x - 1, y - 6, color);
        screen->drawLine(x, y - 7, x + 6, y - 1, color);
      }
      break;
    case 'd':
      {
        //blank bottom outline
        screen->drawLine(x - 6, y, x - 1, y + 5, BACKGROUND);
        screen->drawLine(x, y + 6, x + 6, y, BACKGROUND);
        //extend top outline
        screen->drawLine(x - 6, y - 1, x + 6, y - 1, color);
        screen->drawLine(x - 1, y - 6, x + 1, y - 6, color);
      }
      break;
    case 'l':
      {
        //blank bottom outline
        screen->drawLine(x - 6, y, x, y + 6, BACKGROUND);
        screen->drawLine(x + 1, y + 1, x + 5, y + 1, BACKGROUND);
        //extend top outline
        screen->drawLine(x - 6, y - 1, x, y - 7, color);
        screen->drawLine(x + 1, y - 2, x + 5, y - 2, color);
      }
      break;
    case 'r':
      {
        //blank bottom outline
        screen->drawLine(x, y + 6, x + 6, y, BACKGROUND);
        screen->drawLine(x - 5, y + 1, x - 1, y + 1, BACKGROUND);
        //extend top outline
        screen->drawLine(x, y - 7, x + 6, y - 1, color);
        screen->drawLine(x - 5, y - 2, x - 1, y - 2, color);
      }
      break;
  }
}

// given the timing of a beat, determine the color of the arrow
uint16_t Display::find_color(uint32_t beat) {
  // time of on-beat right before beat, with offset
  uint32_t floor_beat_time = (uint32_t)(((uint32_t)((beat-offset)/mspb))*mspb);
  // relative location of note wrt beat
  float placement = (beat-offset-floor_beat_time)/mspb;
  int subdivision = (int)(round(12*placement))%12;
  switch(subdivision) 
  {
    case 0:
      return TFT_ORANGE;
    break;
    case 6:
      return TFT_BLUE;
    break;
    case 3:
      return TFT_GREEN;
    break;
    case 9:
      return TFT_GREEN;
    break;
    case 4:
      return TFT_PURPLE;
    break;
    case 8:
      return TFT_PURPLE;
    break;
    default:
      return TFT_RED;
    break;
  }
}

//color of glow and fade arrow when note is hit with some accuracy at time full_bright
// eventually change this to early, late, on time?
// red is also a color option but it doesn't look as nice
uint16_t Display::glow_arrow(uint32_t full_bright, uint8_t accuracy) {
  uint32_t diff = millis() - full_bright;
  diff = diff/30;
  if(diff < 21) {
    uint16_t color;
    switch(accuracy) {
      case 0: // note missed
        color = (31-diff) + (33-diff/2)*32 + (31-diff)*2048; // purple
      break;
      case 1: // good
        color = 11 + (63-2*diff)*32 + 11*2048; // bright green
      break;
      case 2: // perfect
        color = (31-diff) * 2113 + 32; //white
      break;
    }
    return(color);
  }
  return(0);
}
