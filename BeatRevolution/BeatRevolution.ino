#include "Game.h"
#include <WiFi.h>

// WiFi
char NETWORK[] = "MIT"; // ssid
char PW[]      = "";    // p/w

TFT_eSPI tft = TFT_eSPI(); 
MPU9255 imu;

const int cs_pin_left = 12;
const int cs_pin_right = 0;
float rate = 0.05;

Saber saber_left(&imu);
Saber saber_right(&imu);

Display display_left(&tft, rate, cs_pin_left);
Display display_right(&tft, rate, cs_pin_right);

Saber*   sabers[2]   = {&saber_left, &saber_right};
Display* displays[2] = {&display_left, &display_right};

Game game(sabers, displays);

char song_name[50] = "bad_apple"; // will eventually be an index
boolean complete = false;

void setup(void) {
  Serial.begin(115200);
  while (!Serial); // wait until we can communicate

  // Connect to WiFi
  WiFi.begin(NETWORK, PW);
  uint8_t connect_attempts = 0;
  Serial.printf("Attempting to connect to\n%s WiFi ", NETWORK);
  while (WiFi.status() != WL_CONNECTED && connect_attempts++ < 8) {
    delay(500);
    Serial.print('.');
  }
  delay(500);
  if(!WiFi.isConnected()) {
    Serial.println(" failed!\nRestarting in a bit...");
    delay(5000);
    ESP.restart();
  }
  Serial.println(" CONNECTED!");

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
