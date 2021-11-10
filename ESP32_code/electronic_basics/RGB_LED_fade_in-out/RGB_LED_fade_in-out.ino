//based upon www.elegoo.com 2016.12.8
// using ESP32 maker Kit1
// replaced analogWrite with ledcWrite(pinChannel, dutycycle) 

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size set to: 4MB(1.2 MB App / 1.5MB SPIFFS)  - default setting
//   lots of other Board settings! but none experimented with

// Define Pins
#define BLUE 4
#define GREEN 16
#define RED 17

#define delayTime 5 // fading time between colors

// define variables
int redValue;
int greenValue;
int blueValue;

int brightness = 0;    // how bright the LED is - not used?
int fadeAmount = 5;    // how many points to fade the LED by - not used?

// setting PWM properties
const int freq = 5000;
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;
const int resolution = 10; // 10 bit resolution gives 0-1023 duty cycle control - possible resolutions are: 8, 10, 12, 15

void setup() {
  Serial.begin(115200);
  
  // define the 3 GPIO pins as outputs 
  pinMode(RED, OUTPUT);   // no longer strictly needed as we are now using ledcWrite
  pinMode(GREEN, OUTPUT); //  ditto
  pinMode(BLUE, OUTPUT);  //  ditto

  // configure LED PWM parameters
  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);

  // attach the channels to the GPIO pins to be controlled
  ledcAttachPin(RED, redChannel);
  ledcAttachPin(GREEN, greenChannel);
  ledcAttachPin(BLUE, blueChannel);

  Serial.println("Program starting");

  ledcWrite(redChannel, 0);
  ledcWrite(greenChannel, 0);
  ledcWrite(blueChannel, 0);
}


// main loop
void loop() {

  Serial.println("main loop cycle starting");
  
  redValue = 1023; // choose a value between 1 and 1023 to change the color.
  greenValue = 0;
  blueValue = 0;
  
  Serial.println("Red and Green LEDs");

  for(int i = 0; i < 1023; i += 1) {    // fades out red bring green full when i=1023
    redValue -= 1;      // red starts at 1023 so decrease it
    greenValue += 1;    // green starts at 0 so increase it
    // The following was reversed, counting in the wrong directions
    // ledcWrite(redChannel, 1023 - redValue);
    // ledcWrite(greenChannel, 1023 - greenValue);
    ledcWrite(redChannel, redValue);
    ledcWrite(greenChannel, greenValue);
    delay(delayTime);
  }

  delay(5000);  // keep the green on for 5 seconds

  redValue = 0;
  greenValue = 1023;
  blueValue = 0;
  
  Serial.println("Green and Blue LEDs");

  for(int i = 0; i < 1023; i += 1) { // fades out green bring blue full when i=1023
    greenValue -= 1;   // green starts at 1023 so decrease it
    blueValue += 1;    // blue starts at 0 so increase it
    // The following was reversed, counting in the wrong directions
    // ledcWrite(greenChannel, 1023 - greenValue);
    // ledcWrite(blueChannel, 1023 - blueValue);
    ledcWrite(greenChannel, greenValue);
    ledcWrite(blueChannel, blueValue);
    delay(delayTime);
  }

  delay(5000);  // keep the blue on for 5 seconds

  redValue = 0;
  greenValue = 0;
  blueValue = 1023;
  
  Serial.println("Blue and Red LEDs");

  for(int i = 0; i < 1023; i += 1)  { // fades out blue bring red full when i=1023
    // The following code has been rearranged to match the other two similar sections
    blueValue -= 1;  // blue starts at 1023 so decrease it
    redValue += 1;   // red starts at 0 so increase it
    // The following was reversed, counting in the wrong directions
    // ledcWrite(blueChannel, 1023 - blueValue);
    // ledcWrite(redChannel, 1023 - redValue);
    ledcWrite(blueChannel, blueValue);
    ledcWrite(redChannel, redValue);
    delay(delayTime);
  }

  delay(5000);  // keep the red on for 5 seconds

}   // continue the main loop
