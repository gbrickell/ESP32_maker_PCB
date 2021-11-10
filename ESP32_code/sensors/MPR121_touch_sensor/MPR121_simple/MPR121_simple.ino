/*
 * simple data collection code that uses a MPR121 module with the ESP32 Maker Kit
 *  
 */

// MPR121_simple.ino - ESP32 maker kit test code that collects touch readings 
// from a MPR121 module when a button is pressed
// 
//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with


#include "Adafruit_MPR121.h" // installed as a library
#include <Wire.h>

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

int irqpin = 33;  // not currently used - but could use the servo2 pin on the Maker Kit PCB

// ****** initial set up ***********
void setup() {

    Serial.begin(115200);
    delay(1500); // short delay to set up the monitor
    Serial.println("program starting ..... ");

    // Default address for a MPR121 is 0x5A, but if the ADD pin istied to 3.3V it is 0x5B
    // If tied to SDA it is 0x5C and if SCL then 0x5D
    if (!cap.begin(0x5A)) {
        Serial.println("MPR121 not found, check wiring?");
        while (1);
    }
    Serial.println("MPR121 found!");

    // pins 1, 3, 5, 14, and 15 are set HIGH on boot - so set them LOW unless/until they are needed
    pinMode(1, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    digitalWrite(1, LOW);
    digitalWrite(3, LOW);
    digitalWrite(5, LOW);
    digitalWrite(14, LOW);
    digitalWrite(15, LOW); 

    pinMode(irqpin, INPUT_PULLUP);

}

void loop() {
  
  // Get the currently touched pads
  currtouched = cap.touched();
  
  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
    }
  }

  // reset our state
  lasttouched = currtouched;

  // comment out this line for detailed data from the sensor!
  return;
  
  // debugging info, what
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.baselineData(i)); Serial.print("\t");
  }
  Serial.println();
  
  // put a delay so it isn't overwhelming
  delay(100);

}
