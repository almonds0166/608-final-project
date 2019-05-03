/*
 * Wiring guide:
 * 
 * On the two IMUs, wire
 *    VCC/GND - power/ground
 *    SCL - 18
 *    SDA - 23
 *    AD0 - 19
 *    NCS - imu_cs_pin_left or imu_cs_pin_left
 * 
 * On the two displays, wire
 *    VCC/GND - power/ground
 *    CS - cs_pin_left or cs_pin_right
 *    RESET - 4
 *    A0 - 2
 *    SDA - 23
 *    SCK - 18
 *    LED - power
 *    
 * On the mp3 module, wire
 *    VCC(first left) - power
 *    RX(second left) - 33
 *    TX(third left) - 32
 *    SPK1(sixth left) - plus end on speaker 
 *    GND(seventh left) - ground
 *    SPK2(eighth left) - minus end on speaker
 */

#include "Interface.h"
#include <WiFi.h>

// WiFi
char NETWORK[] = "MIT"; // ssid
char PW[]      = "";    // p/w

//char NETWORK[] = "6s08"; 
//char PW[]      = "iesc6s08";

TFT_eSPI tft = TFT_eSPI(); 

const int imu_cs_pin_left = 17;
const int imu_cs_pin_right = 16;

MPU9250 imu_left(SPI, imu_cs_pin_left);
MPU9250 imu_right(SPI, imu_cs_pin_right);

HardwareSerial mySoftwareSerial(2); // handles communication with mp3
DFRobotDFPlayerMini mp3;

const int cs_pin_left = 12;
const int cs_pin_right = 0;

Saber saber_left(&imu_left, imu_cs_pin_left);
Saber saber_right(&imu_right, imu_cs_pin_right);

Display display_left(&tft, cs_pin_left);
Display display_right(&tft, cs_pin_right);

Saber*   sabers[2]   = {&saber_left, &saber_right};
Display* displays[2] = {&display_left, &display_right};

char song1[] = "Asterisk";
char song2[] = "Bad Apple";
char song3[] = "Navillera";
char song4[] = "The Blocks We Loved";
char* song_names[5] = {NULL, song1, song2, song3, song4};

Game game(sabers, displays, &mp3, song_names);

const int button1_pin = 5;
const int button2_pin = 13;
Button button1(button1_pin);
Button button2(button2_pin);

Interface interface(&game, &tft, cs_pin_left, cs_pin_right, &button1, &button2, song_names);

int song_index = 2;
boolean complete = false;

void setup(void) {
  Serial.begin(115200);
  while (!Serial); // wait until we can communicate

  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);

  pinMode(imu_cs_pin_left, OUTPUT);
  pinMode(imu_cs_pin_right, OUTPUT);
  pinMode(cs_pin_left, OUTPUT);
  pinMode(cs_pin_right, OUTPUT);

  digitalWrite(imu_cs_pin_left, HIGH);
  digitalWrite(imu_cs_pin_right, HIGH);

  digitalWrite(cs_pin_left, LOW);
  digitalWrite(cs_pin_right, LOW);
  tft.init();
  tft.setRotation(2);
  tft.setTextSize(1); // default font size
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  digitalWrite(cs_pin_left, HIGH);
  digitalWrite(cs_pin_right, HIGH);
  
  
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

  int status;

  Serial.println("Attempting to connect to IMU left");
  status = imu_left.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    while(true) {}
  }
  status = imu_left.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  if (status < 0) {
    Serial.println("IMU setting acce range unsuccessful");
    while(true) {}
  }
  status = imu_left.setSrd(3); // cap sample rate at 250Hz
  if (status < 0) {
    Serial.println("IMU setting sample rate unsuccessful");
    while(true) {}
  }
  Serial.println("IMU connected and set up!");
  digitalWrite(imu_cs_pin_left, HIGH);
  
  Serial.println("Attempting to connect to IMU right");
  status = imu_right.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    while(true) {}
  }
  status = imu_right.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  if (status < 0) {
    Serial.println("IMU setting acce range unsuccessful");
    while(true) {}
  }
  status = imu_right.setSrd(3); // cap sample rate at 250Hz
  if (status < 0) {
    Serial.println("IMU setting sample rate unsuccessful");
    while(true) {}
  }
  Serial.println("IMU connected and set up!");
  digitalWrite(imu_cs_pin_right, HIGH);
  
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

  interface.get_all_high_scores();
}

void loop() {
  interface.process();
}
