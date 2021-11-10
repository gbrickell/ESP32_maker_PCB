// ESP32_L298N_drive_1motor_test.ino - basic L298N drive motor test with the ESP32 maker kit
// just powers one motor and uses PWM for speed control
// 

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 2.0.0
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

// powered by a 5V battery bank or 4xAA battry holder connected to the micro UB/JST power bus 
// and the L298N also connected to the JST sockets
// four GPIO's (for 2 motors) used for the IN1-IN4 pins on the black 7P header on the PCB

// button 2 used to trigger the motor test cycle
//  which is pulled-up with a physical resistor
int btn_pin = 34; // GPIO pin on one side of the bottom button (2)


// Motor A
int motorAPin1 = 5;   // IN1
int motorAPin2 = 18;  // IN2
int enableAPin = 32;  // using the servo1 connection

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8; // 8 bit resolution means duty cycle range is 0-254
int dutyCycle = 200;

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

  pinMode(btn_pin, INPUT); // make the button connected GPIO pin an INPUT

  // sets the pins as outputs:
  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(enableAPin, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  // but don't assign the channel to GPIO yet since mixing 
  // simple ON/OFF and PWM seems to confuse the L298N  
  
  Serial.println("motor loop test running");
  Serial.println("press button 2 to run the test");

}

void loop() {

  if (digitalRead(btn_pin) == LOW) {  // check if button has been pressed

    // if here button has been pressed so do the motor test cycle
    // Move the DC motor forward at maximum speed
    Serial.println("Moving Forward");
    digitalWrite(motorAPin1, LOW);
    digitalWrite(motorAPin2, HIGH); 
    digitalWrite(enableAPin, HIGH);
    delay(2000);

    // Stop the DC motor
    Serial.println("Motor stopped");
    digitalWrite(motorAPin1, LOW);
    digitalWrite(motorAPin2, LOW);
    digitalWrite(enableAPin, LOW);
    delay(1000);

    // Move DC motor backwards at maximum speed
    Serial.println("Moving Backwards");
    digitalWrite(motorAPin1, HIGH);
    digitalWrite(motorAPin2, LOW); 
    digitalWrite(enableAPin, HIGH);
    delay(2000);

    // Stop the DC motor
    Serial.println("Motor stopped");
    digitalWrite(motorAPin1, LOW);
    digitalWrite(motorAPin2, LOW);
    digitalWrite(enableAPin, LOW);
    delay(1000);

    // now use PWM, so attach the channel to the GPIO to be controlled
    ledcAttachPin(enableAPin, pwmChannel);
    // Move DC motor forward with increasing speed
    digitalWrite(motorAPin1, LOW);
    digitalWrite(motorAPin2, HIGH);
    while (dutyCycle <= 255){
      ledcWrite(pwmChannel, dutyCycle);   
      Serial.print("Forward with duty cycle: ");
      Serial.println(dutyCycle);
      dutyCycle = dutyCycle + 5;
      delay(1500);
    }
    dutyCycle = 200;  // reset the dutycycle for the start of the next loop
    digitalWrite(motorAPin1, LOW);
    digitalWrite(motorAPin2, LOW); // stopped the motor
    ledcWrite(pwmChannel, 0); 
    
    ledcDetachPin(enableAPin);
    digitalWrite(enableAPin, LOW);
    
    Serial.print("button 2 reset to ");
    Serial.println(digitalRead(btn_pin));
    Serial.println("press button 2 to re-run the test");
    
  }
    
}
