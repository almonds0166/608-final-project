#include "Interface.h"
// state definitions
#define USERNAME 0
#define SONGSELECT 1
// state for gameplay??
#define HIGHSCORE 2

Interface::Interface(TFT_eSPI* tft, int tft_pin1, int tft_pin2, int button_pin1, int button_pin2, char* songs){
    screen = tft;
    cs_pin_left = tft_pin1;
    cs_pin_right = tft_pin2;
    button1 = button_pin1;
    button2 = button_pin2;

    state = 0;
    char_index = 0;

    song_list = songs;
}

void Interface::update() {
    switch(state) {
        case USERNAME {
            // if ??? state = SONGSELECT;
            Interface::select_username();
            break;
        }
        case SONGSELECT {
            // if ??? state = ???
            // TODO
            break;
        }
        case HIGHSCORE {
            // if ??? state = SONGSELECT;
            // TODO
            break;
        }
    }
}

void Interface::select_username() {
    //TODO
}

void Interface::handle_highscore() {
    //TODO
}
