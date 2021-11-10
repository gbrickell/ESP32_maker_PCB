// ESP32_TM1637_7seg_LED.ino - simple 7 segment LED display for ESP32 maker kit version 

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 2.0.0
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

// powered by 5V and two GPIO's used for the CLK and DIO pins - uses the 7P header on the PCB
// followed the UNO instructions at https://www.electroniclinic.com/tm1637-a-4-digit-7-segment-display-with-arduino/
  
#include <TM1637.h>   // Grove 4-digit Display library

// initialize a global TM1637 Display object
//  The constructor takes two arguments, the number of the clock pin and the digital output pin:
int clk = 19;  // IN3 on the stepper/SPI 7P female header on the PCB
int dio = 23;  // IN4 on the stepper/SPI 7P female header on the PCB
TM1637 tm (clk,dio);




void setup(){
  Serial.begin(115200);
  delay(1500); // short delay to set up the monitor

  Serial.print("program starting ..... ");

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

  tm.init();
  tm.set(3);
  delay(1000);                // wait 1000 ms
  Serial.print("display initialised");

}

void loop(){
  
  tm.display(0,1);
  tm.display(1,2);
  tm.point(1);
  tm.display(2,10);
  tm.display(3,11);

}
