// ESP32_stepper_basic.ino - basic stepper control with the ESP32 maker kit
// using the standard Arduino library. This standard library can control a 
// number of different types of stepper motor and for the 4 wire 28BYJ-48 stepper 
// motor, controlled by a ULN2003 module, it uses a full-step energising method
// that energises two phases at the same time, which produces the strongest torque
// 

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 2.0.0
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

// powered by 5V and four GPIO's used for the IN1-IN4 pins - uses the 7P header on the PCB

#include <Stepper.h>
 
const int stepsPerRevolution = 2048;  // change this to fit the number of steps 
       // per revolution which is 2048 for the 28BYJ-48 with a full-step method

// ULN2003 Motor Driver Pins 
#define IN1 5
#define IN2 18
#define IN3 19
#define IN4 23

// initialize the stepper library
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

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

  // set the stepper motor speed at 5 rpm
  myStepper.setSpeed(5);
  
  Serial.println("program loop running");

}

void loop() {

  // step one revolution in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(1000);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(1000);
    
}
