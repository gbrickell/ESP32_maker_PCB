// 80x160_ST7735_Hello_World.ino  SPI display example for the ESP32 Maker Kit

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

// the custom file for this display must be referenced in User_Setup_Select.h
//  but only one file!!
#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

int loopcycle = 0;

void setup(void) {
  Serial.begin(115200);
  delay(1500); // short delay to set up the monitor

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

  Serial.println("display initialising and test text printing ..... ");
  tft.init();
  tft.setRotation(1);    // rotate screen so that it is 160 wide and 80 high
  tft.fillScreen(TFT_BLACK);
  // can't use Serial.println after this point
  // as it seems to conflict with the display "print" class
  
  // Set "cursor" at top left corner of display (0,0) and select font 2
  tft.setCursor(0, 0, 2);
  // Set the font colour to be white with a black background
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  // We can now plot text on screen using the "print" class
  tft.println("Test coloured text\n");
  tft.println("White text");
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.println("Red text");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Green text");
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.println("Blue text");

  delay(5000);
  tft.fillScreen(TFT_BLACK);  // blank screen

}

void loop() {
  // use the drawX function to generate a Hello World display
  //  and alter the colours at each loop
  if (loopcycle == 0) {
    drawX(0);
    loopcycle = 1;
  } else {
    drawX(1);
    loopcycle = 0;
  }
  delay(3000);

  tft.fillScreen(TFT_BLACK);  // blank screen and loop around again

}  


void drawX(int color) {
  tft.fillScreen(tft.color565(25,25,25)); // Grey

  // Draw circle
  if (color == 0) {
    tft.drawCircle(tft.width()/2, tft.height()/2, tft.width()/4, TFT_RED);
  } else {
    tft.drawCircle(tft.width()/2, tft.height()/2, tft.width()/4, TFT_BLUE);
  }

  // Draw diagonal lines
  if (color == 0) {
    tft.drawLine(0 ,              0, tft.width()-1, tft.height()-1, TFT_GREEN);
    tft.drawLine(0 , tft.height()-1, tft.width()-1,              0, TFT_BLUE);
  } else {
    tft.drawLine(0 ,              0, tft.width()-1, tft.height()-1, TFT_RED);
    tft.drawLine(0 , tft.height()-1, tft.width()-1,              0, TFT_GREEN);
    
  }
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, tft.color565(25,25,25));
  tft.drawString("Hello World!", tft.width()/2, tft.height()/2, 4); // Font 4
}
  
