// NOTE: this will not work until all the parts come together, probably test your code in your own .ino file with Serial prints or such

#include <TFT_eSPI.h> 
#include <SPI.h>
#include "Game.h"

TFT_eSPI tft = TFT_eSPI();  // TODO: how to have two displays?

char song_name[50];

Game game = Game(12,0,&tft,);

boolean complete = false;

void setup(void) {
  Serial.begin(115200);
  // TODO: connect to wifi
  game.load(song_name); // once there is UI, this will be in a state machine transition in loop(), rather than in setup
  game.start(song_name); // once there is UI, this will be in a state machine transition in loop(), rather than in setup
}

void loop() {
  // TODO: state machine of UI
  // for now, game just starts right away with hardcoded song name, and dies once the song has been played
  
  if (!complete) {
    complete = game.process(); // game.process will do things such as update display, detect motion, play music, etc; it returns whether or not game is over
  }
}
