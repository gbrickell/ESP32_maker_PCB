// ESP32_I2C_16x2_LCD.ino - simple I2C LCD display for ESP32 maker kit version 

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

// The 'usual' 5V powered 16x2 LCD must have its internal pull-up resistors removed because otherwise
//  the SDA and SCL lines will be at 5V - BUT then because the ESP32's internal pull-ups are generally
//  very weak 4.7k resistors should be used as pull-ups to the 3.3V source for reliable operation

#include <LiquidCrystal_I2C.h> // installed from https://github.com/marcoschwartz/LiquidCrystal_I2C/archive/master.zip
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, 16, 2);  

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

  // set up the LCD 
  Serial.println("\nI2C LCD static text");
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}

void loop(){
  // continuous loop displaying a series of text texts

  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  Serial.println("\nprinting text");
  lcd.print("Hello, World!");
  delay(1000);
  // clear the display to print new message
  Serial.println("\nclearing screen");
  lcd.clear();
  delay(1000);
  Serial.println("\nbacklight off");
  lcd.noBacklight();
  delay(1000);
  Serial.println("\nbacklight on");
  lcd.backlight();
  delay(1000);
  lcd.setCursor(0, 0);
  // print message
  Serial.println("\nprinting text");
  lcd.print("Goodbye, World!");
  delay(1000);
  lcd.setCursor(0, 1);
  // print message
  Serial.println("\nprinting text");
  lcd.print("2nd row text!");
  delay(1000);
  Serial.println("\nclearing screen");
  lcd.clear();
  delay(1000);

}
