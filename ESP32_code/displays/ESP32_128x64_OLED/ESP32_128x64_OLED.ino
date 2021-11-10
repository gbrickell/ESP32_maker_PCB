// ESP32_128x64_OLED.ino ESP32 maker kit version 

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

// uses the ThingPulse OLED driver "ESP8266 and ESP32 Oled Driver for SSD1306 display"
#include "SSD1306Wire.h"        // used for the 128x64 OLED
#include "Open_Sans_Regular_12.h"
#include "Open_Sans_Regular_16.h"
#include "Open_Sans_Regular_18.h"
#include "Open_Sans_Regular_24.h"



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

  // set up the OLED and display test text
  SSD1306Wire display(0x3c, 21, 22);
  display.init();  
  Serial.println("\n128x64 OLED initialised");
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "Hello, world!", "Hello, world!", "Hello, world!", "");
  Serial.print("OLED text displayed .... ");


}

void loop() {

}



void oledtext(int rows, int col1, int col2, int col3, int col4, int fontsize, String text1, String text2, String text3, String text4) {
    SSD1306Wire display(0x3c, 21, 22);
    display.init();
    display.flipScreenVertically();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    if (fontsize == 12) {
        display.setFont(Open_Sans_Regular_12);
    } else if (fontsize == 16) {
        display.setFont(Open_Sans_Regular_16);
    } else if (fontsize == 18) {
        display.setFont(Open_Sans_Regular_18);
    } else if (fontsize == 24) {
        display.setFont(Open_Sans_Regular_18);
    } else {
        display.setFont(Open_Sans_Regular_12);  // set a default if an unknown font size is set
        Serial.println("Default font size set to 12");
    }
    if (rows == 1) {
        display.drawString(col1, 24, text1);
    } else if (rows == 2) {
        display.drawString(col1, 0, text1);
        display.drawString(col2, 31, text2);
    } else if (rows == 3) {   // normally fontsize should =16
        display.drawString(col1, 0, text1);
        display.drawString(col2, 21, text2);
        display.drawString(col3, 42, text3);
    } else if (rows == 4) {
        display.drawString(col1, 0, text1);
        display.drawString(col2, 16, text2);
        display.drawString(col3, 32, text3);
        display.drawString(col3, 48, text4);
    }
    display.display();
}

void oledclear() {
    SSD1306Wire display(0x3c, 21, 22);
    display.init();
    display.clear();
    display.displayOff();

}
