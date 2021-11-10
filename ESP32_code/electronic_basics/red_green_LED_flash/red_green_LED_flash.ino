/// red_green_LED_flash.ino ESP32 maker kit version of LED_red_green_flash.py

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with
 
int red_positive_pin = 14;         //red LED on ESP32 Maker Kit1
int green_positive_pin = 26;       //green LED on ESP32 Maker Kit1

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
  
  pinMode(red_positive_pin, OUTPUT);
  pinMode(green_positive_pin, OUTPUT);
  
}


void loop() {

  if (printon) {
    Serial.println("blink the red and green LEDs on/off with a 0.5 second delay");
    printon = false;
  }
  
  digitalWrite(red_positive_pin, HIGH);
  digitalWrite(green_positive_pin, LOW);
  delay(500);
  digitalWrite(red_positive_pin, LOW);
  digitalWrite(green_positive_pin, HIGH);
  delay(500);
}
//=======================================================================
