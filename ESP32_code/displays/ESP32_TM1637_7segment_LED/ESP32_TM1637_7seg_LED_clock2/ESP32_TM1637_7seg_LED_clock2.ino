// ESP32_TM1637_7seg_LED_clock.ino - 7 segment LED clock display for ESP32 maker kit version 

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 2.0.0
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

// powered by 5V and two GPIO's used for the CLK and DIO pins - uses the 7P header on the PCB
  
#include <TM1637.h>   // Grove 4-digit Display library
#include <WiFi.h>
#include "time.h"

// network credentials
const char* ssid     = "PWR-WiFi";
const char* password = "digmaker99";

// initialize a global TM1637 Display object
//  The constructor takes two arguments, the number of the clock pin and the digital output pin:
int clk = 19;  // IN3 on the stepper/SPI 7P female header on the PCB
int dio = 23;  // IN4 on the stepper/SPI 7P female header on the PCB
TM1637 tm (clk,dio);

// Variables to save date and time
String daytimeStamp;
String LED0;
String LED1;
String LED2;
String LED3;
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
const char* ntpServer = "pool.ntp.org";

String hour_time;
String mins_time;

void printLocalTime()  // get current time 
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  hour_time = String(timeinfo.tm_hour);
  mins_time = String(timeinfo.tm_min);
  //Serial.print("minutes: ");
  //Serial.println(mins_time);
 
}
 
void setup(){
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

  tm.init();  // initialise display
  tm.set(3);  // set display brightness 0-7
  delay(1000);                // wait 1000 ms
  Serial.println("display initialised");

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address 
  Serial.println("");
  Serial.println("WiFi connected to set internal clock to current time.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  Serial.println("WiFi disconnected.");
  Serial.println("updating display from internal clock ......");

}

void loop(){

  printLocalTime();  // function to get current time

  if (hour_time.length() == 2) {
    LED0 = hour_time.substring(0,1);
    LED1 = hour_time.substring(1);
  }
  else {
    LED0 = "0";
    LED1 = hour_time.substring(0,1);
  }

  if (mins_time.length() == 2) {
    LED2 = mins_time.substring(0,1);
    LED3 = mins_time.substring(1);
  }
  else {
    LED2 = "0";
    LED3 = mins_time.substring(0,1);
  }

  delay(100);  
  
  tm.display(0,LED0.toInt());
  tm.display(1,LED1.toInt());
  tm.point(1);
  tm.display(2,LED2.toInt());
  tm.display(3,LED3.toInt());

}
