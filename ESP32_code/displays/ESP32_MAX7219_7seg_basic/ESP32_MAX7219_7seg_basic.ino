// ESP32_MAX7219_7seg_basic.ino - 2x 4x 7 segment LED display for ESP32 maker kit version 

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 2.0.0
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

// powered by 5V and three GPIO's used for the CLK, LOAD and CS pins - uses the 7P header on the PCB

#include "LedControl.h"  // edited version of the Arduino library version
//  LedControl.h edited by replacing 
//  #include <avr\pgmspace.h> 
//    with
//  #if (defined(__AVR__))
//  #include <avr\pgmspace.h>
//  #else
//  #include <pgmspace.h>
//  #endif

// 
// 7P stepper/SPI header on PCB used 
//  IN4/MOSI(#23) -> DIN,  IN2/SCK(#18) -> CLK, IN1/SS(#5) -> CS, and no.of devices is 1
LedControl lc=LedControl(23,18,5,1);

void setup() {

  Serial.begin(115200);
  delay(1000); // short delay to set up the monitor

  Serial.println("program starting ..... ");

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

  
 // Initialize the MAX7219 device
  lc.shutdown(0,false);   // Enable display
  lc.setIntensity(0,10);  // Set brightness level (0 is min, 15 is max)
  lc.clearDisplay(0);     // Clear display register
  Serial.println("display initialised and cleared");

  Serial.println("program loop running");
  Serial.println("blink all segments 4 times, then display 0 to E on all elements");
  
}

void loop() {
  
 // Blink all segments 4 times
 for(int k=0; k<4; k++){
   for(int i=0; i<8; i++){
     lc.setDigit(0,i,8,true);
   }  // End i
   delay(300);
   lc.clearDisplay(0);
   delay(300);
 } // End k

 // Display 0 through E
 for(int k=0; k<15; k++){
   for(int i=0; i<8; i++){
    lc.setDigit(0,i,k,false);
  }// End i
  delay(1000);
 } // End k
  
}
