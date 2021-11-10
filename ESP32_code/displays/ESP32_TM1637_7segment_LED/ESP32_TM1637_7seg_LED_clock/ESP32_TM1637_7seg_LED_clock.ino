// ESP32_TM1637_7seg_LED_clock.ino - 7 segment LED clock display for ESP32 maker kit version 

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 2.0.0
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

// powered by 5V and two GPIO's used for the CLK and DIO pins - uses the 7P header on the PCB
// followed the UNO instructions at https://www.electroniclinic.com/tm1637-a-4-digit-7-segment-display-with-arduino/
  
#include <TM1637.h>   // Grove 4-digit Display library
#include <WiFi.h>
#include <NTPClient.h>  // downloaded from https://github.com/taranais/NTPClient/archive/master.zip
#include <WiFiUdp.h>

// network credentials
const char* ssid     = "PWR-WiFi";
const char* password = "digmaker99";

// initialize a global TM1637 Display object
//  The constructor takes two arguments, the number of the clock pin and the digital output pin:
int clk = 19;  // IN3 on the stepper/SPI 7P female header on the PCB
int dio = 23;  // IN4 on the stepper/SPI 7P female header on the PCB
TM1637 tm (clk,dio);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;  
String LED0;
String LED1;
String LED2;
String LED3;
 
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);


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
  tm.set(3);  // set brightness 0-7
  delay(1000);                // wait 1000 ms
  Serial.print("display initialised");

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address 
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  Serial.println("timestamps @GMT");
  // GMT 0 = 0

}

void loop(){

  // get current time/date
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  //Serial.print("DATE: ");
  //Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);
  LED0 = timeStamp.substring(0,1);
  LED1 = timeStamp.substring(1,2);
  LED2 = timeStamp.substring(3,4);
  LED3 = timeStamp.substring(4,5);
  delay(100);  
  
  tm.display(0,LED0.toInt());
  tm.display(1,LED1.toInt());
  tm.point(1);
  tm.display(2,LED2.toInt());
  tm.display(3,LED3.toInt());

}
