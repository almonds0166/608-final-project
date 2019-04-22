#include "Game.h"

TFT_eSPI tft = TFT_eSPI(); 
MPU9255 imu;

const int cs_pin_left = 12;
const int cs_pin_right = 0;
float rate = 0.05;

Saber saber_left(&imu);
Display display_left(&tft, rate, cs_pin_left);
Display display_right(&tft, rate, cs_pin_right);
Game game(&saber_left, &display_left, &display_right);

char song_name[50] = "test";
boolean complete = false;

void setup(void) {
  Serial.begin(115200);
  // TODO: connect to wifi

  // NOTE: taking out the setupIMU stuff breaks everything
  
  if (imu.setupIMU(1)) {
    Serial.println("IMU Connected!");
  } else {
    Serial.println("IMU Not Connected :/");
    Serial.println("Restarting");
    ESP.restart(); // restart the ESP (proper way)
  }
  
  

  game.load(song_name);
  game.start(song_name);
}

void loop() {
  // TODO: state machine of UI
  // for now, game just starts right away with hardcoded song name, and dies once the song has been played
  
  if (!complete) {
    complete = game.process(); // game.process will do things such as update display, detect motion, play music, etc; it returns whether or not game is over
  }
  
}
