// RGB_LED_flash.ino ESP32 maker kit version of RGB_LED_flash.py
// devices (motors, servos as well as LEDs) essentially by switching them on and off very very fast
// PWM has 2 main parameters:
//  - Frequency: the number of time per second that a pulse is generated
//  - Duty Cycle: the % of time during a single cycle that the signal is high

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with
 
int red_positive_pin = 17;         //red RGB LED on ESP32 Maker Kit1
int green_positive_pin = 16;       //green RGB LED on ESP32 Maker Kit1
int blue_positive_pin = 4;         //blue RGB LED on ESP32 Maker Kit1

// setting PWM properties
const int freq = 5000;
// 10 bit resolution gives 0-1023 duty cycle control - possible resolutions are: 8, 10, 12, 15 
const int resolution = 10; 

const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;


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
  
  pinMode(red_positive_pin, OUTPUT);   // no longer strictly needed as we are now using ledcWrite
  pinMode(green_positive_pin, OUTPUT); //  ditto
  pinMode(blue_positive_pin, OUTPUT);  //  ditto
  
  // configure LED PWM functionalitites
  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);

  // attach the channels to the GPIOs to be controlled
  ledcAttachPin(red_positive_pin, redChannel);
  ledcAttachPin(green_positive_pin, greenChannel);
  ledcAttachPin(blue_positive_pin, blueChannel);

  ledcWrite(redChannel, 0);
  ledcWrite(greenChannel, 0);
  ledcWrite(blueChannel, 0);

}

void loop() {

  if (printon) {
    Serial.println("blink the red, green and blue RGB LEDs on/off @100% with a 1 second delay");
    printon = false;
  }
  
  ledcWrite(redChannel, 1023);
  ledcWrite(greenChannel, 0);
  ledcWrite(blueChannel, 0);
  delay(1000);
  ledcWrite(redChannel, 0);
  ledcWrite(greenChannel, 1023);
  ledcWrite(blueChannel, 0);
  delay(1000);
  ledcWrite(redChannel, 0);
  ledcWrite(greenChannel, 0);
  ledcWrite(blueChannel, 1023);
  delay(1000);
  
}
//=======================================================================
