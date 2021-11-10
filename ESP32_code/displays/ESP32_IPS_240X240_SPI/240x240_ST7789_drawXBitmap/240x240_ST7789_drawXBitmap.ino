// 240x240_ST7789_drawXBitmap.ino  SPI display example for the ESP32 Maker Kit

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with


// Example sketch based upon an example from TFT_eSPI library
//  to demonstrate the drawing of X BitMap (XBM) format images onto the display.

// Information on the X BitMap (XBM) format can be found here:
// https://en.wikipedia.org/wiki/X_BitMap

// More on the TFT_eSPI library at https://github.com/Bodmer/TFT_eSPI



#include "espressif_logo.h"  // Sketch tab header for the logo image
#include "cat240x180.h"      // Sketch tab header for the cat image
#include "star240x240.h"     // Sketch tab header for the star image

// the custom file for this display must be referenced in User_Setup_Select.h
//  but only one file!!
#include <TFT_eSPI.h>        // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();   // Invoke library

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
  Serial.println("pin defaults reset..... ");
  Serial.println("display initialising and test images displaying ..... ");
  // can't use Serial.println after this point
  // as it seems to conflict with the display "print" class
  
  tft.begin();               // Initialise the display
  //tft.setRotation(1);        // rotate screen so that it is 160 wide and 80 high
  tft.fillScreen(TFT_BLACK); // Black screen fill
  
}

void loop() {

  // star example 1
  // ==============
  tft.fillScreen(TFT_WHITE); // White screen fill
  // Draw bitmap with top left corner at x,y with foreground only color
  // Bits set to 1 plot as the defined color, bits set to 0 are not plotted
  //               x  y  xbm   xbm width  xbm height  color
  tft.drawXBitmap(0, 0, star, starWidth, starHeight, TFT_BLACK);
  delay(2000);
  tft.fillScreen(TFT_BLACK); // Black screen fill

  // cat example 1
  // =============
  tft.fillScreen(TFT_WHITE); // White screen fill
  // Draw bitmap with top left corner at x,y with foreground only color
  // Bits set to 1 plot as the defined color, bits set to 0 are not plotted
  //               x  y  xbm  xbm width xbm height color
  tft.drawXBitmap(0, 30, cat, catWidth, catHeight, TFT_BLACK);
  delay(2000);
  tft.fillScreen(TFT_BLACK); // Black screen fill

  // Espressif logo example 1
  // ========================
  // Random x and y coordinates
  int x = random(tft.width()  - logoWidth);
  int y = random(tft.height() - logoHeight);

  // Draw bitmap with top left corner at x,y with foreground only color
  // Bits set to 1 plot as the defined color, bits set to 0 are not plotted
  //              x  y  xbm   xbm width  xbm height  color
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_WHITE);

  delay(1000);

  // Erase old one by drawing over with background colour
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_BLACK);


  // Espressif logo example 2
  // ========================
  // New random x and y coordinates
  x = random(tft.width()  - logoWidth);
  y = random(tft.height() - logoHeight);

  // Draw bitmap with top left corner at x,y with foreground and background colors
  // Bits set to 1 plot as the defined fg color, bits set to 0 are plotted as bg color
  //              x  y  xbm   xbm width  xbm height  fg color   bg color
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_WHITE, TFT_RED);

  delay(1000);

  // Erase old one by drawing over with background colour
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_BLACK, TFT_BLACK);

}
