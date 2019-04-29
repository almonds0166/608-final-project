#ifndef Interface_h
#define Interface_h
#include "Arduino.h"
#include <TFT_eSPI.h> 
#include <SPI.h>

class Interface {
    private:

    TFT_eSPI* screen;
    int cs_pin_left;
    int cs_pin_right;
    int button1;
    int button2;

    // set default colors
    const uint16_t BACKGROUND = TFT_BLACK;
    const uint16_t TEXT = TFT_WHITE;

    int state; // state of the interface (username, song selection, score display)

    // variables for username selection
    char alphabet[50] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char username[50] = {0};
    int char_index;

    // @Matt this is where you put your variables?
    char* song_list;

    public:

    Interface(TFT_eSPI* tft, int tft_pin1, int tft_pin2, int button_pin1, int button_pin2, char* songs);
    void update(); // handles state changes
    void select_username(); // handles selecting a username
    void handle_highscore(); // handles displaying high scores
}

#endif
