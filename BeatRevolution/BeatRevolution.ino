#include "Game.h"
#include <WiFi.h>

// WiFi
char NETWORK[] = "MIT"; // ssid
char PW[]      = "";    // p/w

//char NETWORK[] = "6s08"; // ssid
//char PW[]      = "iesc6s08";    // p/w

TFT_eSPI tft = TFT_eSPI(); 
MPU9255 imu;
HardwareSerial mySoftwareSerial(2); // handles communication with mp3
DFRobotDFPlayerMini mp3;

const int cs_pin_left = 12;
const int cs_pin_right = 0;
float rate = 0.07;

const int ad0_pin_left = 19;
const int ad0_pin_right = 17;

Saber saber_left(&imu, ad0_pin_left);
Saber saber_right(&imu, ad0_pin_right);

Display display_left(&tft, rate, cs_pin_left);
Display display_right(&tft, rate, cs_pin_right);

Saber*   sabers[2]   = {&saber_left, &saber_right};
Display* displays[2] = {&display_left, &display_right};

char song1[] = "Asterisk";
char song2[] = "Bad Apple";
char song3[] = "Navillera";
char song4[] = "The Blocks We Loved";
char* song_names[5] = {NULL, song1, song2, song3, song4};

Game game(sabers, displays, &mp3, song_names);

int song_index = 2;
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
  
  mySoftwareSerial.begin(9600, SERIAL_8N1, 32, 33);  // speed, type, RX, TX
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  delay(1000);
  if (!mp3.begin(mySoftwareSerial)) {  // Use softwareSerial to communicate with mp3.
    Serial.println(mp3.readType(), HEX);
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  mp3.setTimeOut(500); //Set serial communictaion time out 500ms
  mp3.volume(15); // set volume (0~30)
  mp3.EQ(DFPLAYER_EQ_NORMAL); // set EQ
  mp3.outputDevice(DFPLAYER_DEVICE_SD); // set to read from SD
  
  game.load(song_index);
  game.start(song_index);
}

void loop() {
  // TODO: state machine of UI
  // for now, game just starts right away with hardcoded song name, and dies once the song has been played
  
  if (!complete) {
    complete = game.process(); // game.process will do things such as update display, detect motion, play music, etc; it returns whether or not game is over
  }
}
