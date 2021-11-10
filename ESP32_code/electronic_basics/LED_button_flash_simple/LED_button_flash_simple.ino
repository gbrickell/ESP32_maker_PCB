/// LED_button_flash.ino ESP32 maker kit version of LED_button_flash.py

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

#include <ezButton.h>

ezButton button(39); // create ezButton object that attach to pin GIOP39
 
int red_positive_pin = 14;   // red LED on ESP32 Maker Kit1

bool printon = true;

void setup() {
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

  pinMode(red_positive_pin, OUTPUT);   // make the LED connected GPIO pin an OUTPUT

  button.setDebounceTime(50); // set debounce time to 50 milliseconds

}

void loop() {

  if (printon) {
    Serial.println("press the top button (button 1) to switch the LED on for 5 seconds");
    printon = false;
  }

  button.loop(); // MUST call the loop() function first

  if(button.isPressed()) {
    Serial.println("switching the red LED on for 5 seconds");
    digitalWrite(red_positive_pin, HIGH);  // switch the red LED on

    if(button.isReleased()) {
      Serial.println("The button is released");
    }
    
    delay(5000);  // wait 5 seconds
    digitalWrite(red_positive_pin, LOW);   // switch the red LED off
    printon = true;       // reset the print message 'flag'    
    }

}

//=======================================================================
