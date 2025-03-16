#include <Wire.h>
#include <Adafruit_CAP1188.h>
#include <TFT_eSPI.h>

#define BLUE_PIN 12 // Define the blue pin
#define RED_PIN 25 // TO-DO - Define the red pin
#define GREEN_PIN 27 // TO-DO - Define the green pin
#define YELLOW_PIN 26 // TO-DO - Define the yellow pin
#define TOUCH_THRESHOLD 500


int touchThreshold = 0;
bool isSequenceActivated = false;


#define BUZZER_PIN 15 // TO-DO - Define the buzzer pin

Adafruit_CAP1188 cap = Adafruit_CAP1188();
TFT_eSPI tft = TFT_eSPI(); // Create an instance of the TFT_eSPI library

void setup() {
    // set pins as output
    pinMode(BLUE_PIN, OUTPUT); // Set the blue pin as an output
    pinMode(RED_PIN, OUTPUT);  //...
    pinMode(GREEN_PIN, OUTPUT); 
    pinMode(YELLOW_PIN, OUTPUT); 

    //pinMode(BUZZER_PIN, OUTPUT); // Set the buzzer pin as an output

    
    // create pulsing/beeping noise rather than constant sound
    /*
    tone(BUZZER_PIN, 1000); // Set TONE of buzzer???
    delay(2000);
    noTone(BUZZER_PIN); // Turn off buzzer
    */

    digitalWrite(BUZZER_PIN, HIGH); // turn off buzzer
    digitalWrite(RED_PIN, HIGH); 
    delay(1000);
    digitalWrite(YELLOW_PIN, HIGH); 
    delay(1000);
    digitalWrite(RED_PIN, LOW); 
    digitalWrite(YELLOW_PIN, LOW); 
    digitalWrite(GREEN_PIN, HIGH); // start as green
    delay(1000);

    Serial.begin(9600);
    Serial.println("CAP1188 test!");

    if (!cap.begin()) {
        Serial.println("CAP1188 not found");
        while (1);
    }
    Serial.println("CAP1188 found!");
}

void greenBuzzerPattern() {
    tone(BUZZER_PIN, 1000); // Beep at 1kHz
    delay(500);
    noTone(BUZZER_PIN);
    delay(1500);
}

void redBuzzerPattern() {
    tone(BUZZER_PIN, 1000); 
    delay(250);
    noTone(BUZZER_PIN);
    delay(250);
}

void stopLightSequence() {
  /**
   * Green light stays on for 5 seconds,
   * then green light turns off and the yellow light turns on for 2 seconds,
   * then yellow light turns off and the red light turns on for 10 seconds,
   * then the red and yellow light remain on for 2 seconds,
   * then the red and yellow lights turn off and the green light turns on.
   */
  // digitalWrite(BUZZER_PIN, HIGH); // turn on buzzer
unsigned long greenStartMilis = millis(); // Start of sample window
while (millis() - greenStartMilis < 5000) { // check that start
    greenBuzzerPattern();
}

  //tone(BUZZER_PIN, 500); // turn on buzzer 
  // delay(5000);
  digitalWrite(GREEN_PIN, LOW); // turn off green
  digitalWrite(YELLOW_PIN, HIGH); // turn on yellow
  delay(2000);
  digitalWrite(YELLOW_PIN, LOW); // turn off yellow

  digitalWrite(RED_PIN, HIGH); // turn on red
  //delay(10000);
  unsigned long redStartMilis = millis();
  while (millis() - redStartMilis < 10000) {
    redBuzzerPattern();
  }
  
  digitalWrite(YELLOW_PIN, HIGH);
  delay(2000);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(RED_PIN, LOW); // turn off red

  digitalWrite(GREEN_PIN, HIGH); 
  //digitalWrite(BUZZER_PIN, LOW); // turn off buzzer 
  //noTone(BUZZER_PIN); // Turn off buzzer

  isSequenceActivated = false;
}

void loop() {
    uint8_t touched = cap.touched();

    if (touched == 0) {
      greenBuzzerPattern();
        return; 
    }

    if (touched & (1 << 7)) {
      stopLightSequence();
      touched = cap.touched(); // resample touched to eliminate unwanted touch looping
    } else {
        digitalWrite(GREEN_PIN, HIGH); // when button is not pressed, green light is on
        
    }
}
