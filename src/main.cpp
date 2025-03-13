/*************************************************** 
  This is a library for the CAP1188 I2C/SPI 8-chan Capacitive Sensor

  Designed specifically to work with the CAP1188 sensor from Adafruit
  ----> https://www.adafruit.com/products/1602

  These sensors use I2C/SPI to communicate, 2+ pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Adapted from CAP118 Library Example: https://registry.platformio.org/libraries/adafruit/Adafruit%20CAP1188%20Library
 
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>
#include <TFT_eSPI.h> // Include the TFT_eSPI library

#define PIN_BLUE 12

Adafruit_CAP1188 cap = Adafruit_CAP1188();

TFT_eSPI tft = TFT_eSPI(); // Create an instance of the TFT_eSPI library

void setup() {

  pinMode(PIN_BLUE, OUTPUT);
  Serial.begin(9600);
  Serial.println("CAP1188 test!");

  // Initialize the sensor, if using i2c you can pass in the i2c address
  // if (!cap.begin(0x28)) {
  if (!cap.begin()) {
    Serial.println("CAP1188 not found");
    while (1);
  }
  Serial.println("CAP1188 found!");

  // intializing TFT display for on-screen counter of pins
  tft.init(); // Initialize the TFT display
  tft.fillScreen(TFT_BLACK); // Fill the screen with black color
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set the text color to white
  tft.setTextSize(3);
}

void loop() {
  uint8_t touched = cap.touched();

  int touchCount = 0; // intializing touch count to 0

  if (touched == 0) {
    // No touch detected
    tft.drawNumber(0, 0, 0, 6); // reset number to zero if no touches are set
    return;
  }
  
    if (touched & (1 << 7)) { // scan bitmap touched for valid bits signifing contact with pin
      touchCount += 1;
      digitalWrite(PIN_BLUE, HIGH); // turn on blue LED if pin is touched
      delay(1000);
      digitalWrite(PIN_BLUE, LOW); // turn off blue LED after 1 second
      delay(1000);
    }
  tft.drawNumber(touchCount, 0, 0, 6); // drawing touch count on screen once the cap touch pins are scanned
  
  delay(100);
}