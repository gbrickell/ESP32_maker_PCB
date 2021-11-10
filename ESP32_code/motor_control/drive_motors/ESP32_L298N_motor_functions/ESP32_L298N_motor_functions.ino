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
int in1 = 5;   // IN1
int in2 = 18;  // IN2
int enA = 32;  // using the servo1 connection

// Motor B
int in3 = 19;  // IN3
int in4 = 23;  // IN4
int enB = 33;  // using the servo2 connection

// Setting PWM properties
const int freq = 1000;
const int pwmChannelA = 0;
const int pwmChannelB = 1;
const int resolution = 8; // 8 bit resolution means duty cycle range is 0-254
int dutyA = 0; // PWM value from 0-255
int dutyB = 0; // PWM value from 0-255

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
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannelA, freq, resolution);
  ledcSetup(pwmChannelB, freq, resolution);
  // attach the PWM channels to the GPIOs to be controlled
  ledcAttachPin(enA, pwmChannelA);
  ledcAttachPin(enB, pwmChannelB);
 
  Serial.println("motor loop test running");
  Serial.println("press button 2 to run the test");

}

void loop() {

  if (digitalRead(btn_pin) == LOW) {  // check if button has been pressed

    // if here button has been pressed so do the motor test cycle
    // Move Motors A + B forward at maximum speed
    Serial.println("Moving Motors A + B Forward");
    forward(255, 255);
    delay(3000);
    // Stop the motors
    Serial.println("Motor stopped");
    stop(1);
    delay(1000);

    // Move Motors A + B backwards at maximum speed
    Serial.println("Moving Motors A + B  Backwards");
    backward(255, 255);
    delay(3000);
    // Stop the motors
    Serial.println("Motor stopped");
    stop(1);
    delay(1000);

    // turn left at maximum speed
    Serial.println("Turning left");
    turnLeft(1000, 255);
    delay(1000);

    // turn right at maximum speed
    Serial.println("Turning right");
    turnRight(1000, 255);
    delay(1000);

    // spin left at maximum speed
    Serial.println("Spinning left");
    spinLeft(1000, 255, 255);
    delay(1000);

    // spin right at maximum speed
    Serial.println("Spinning right");
    spinRight(1000, 255, 255);
    delay(1000);

    Serial.print("button 2 reset to ");
    Serial.println(digitalRead(btn_pin));
    Serial.println("press button 2 to re-run the test");
    
  }
    
}

void forward (int dutyCycleA, int dutyCycleB) {
    // separate duty cycles are set so that each motor can be fine tuned if they vary in performance

    // set enA with the PWM dutycycle
    ledcWrite(pwmChannelA, dutyCycleA); // Send PWM signal to motor A (left)
    // set in1 on and in2 off i.e. HIGH - LOW for forward motion
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // set enB with the PWM dutycycle   
    ledcWrite(pwmChannelB, dutyCycleB); // Send PWM signal to motor B (right)
    // set in3 on and in4 off i.e. HIGH - LOW for forward motion
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    Serial.print(" forward: dutyCycleA - ");
    Serial.println(dutyCycleA);
    Serial.print(" forward: dutyCycleB - ");
    Serial.println(dutyCycleB);
}
 
void backward(int dutyCycleA, int dutyCycleB) {
    // separate duty cycles are set so that each motor can be fine tuned if they vary in performance

    // set enA with the PWM dutycycle
    ledcWrite(pwmChannelA, dutyCycleA); // Send PWM signal to motor A (left)
    // set in1 off and in2 on i.e. LOW - HIGH for backward motion
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // set enB with the PWM dutycycle
    ledcWrite(pwmChannelB, dutyCycleB); // Send PWM signal to motor B (right)
    // set in3 off and in4 on i.e. LOW - HIGH for backward motion
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    Serial.print(" backward: dutyCycleA - ");
    Serial.println(dutyCycleA);
    Serial.print(" backward: dutyCycleB - ");
    Serial.println(dutyCycleB);
}

void turnLeft(int turn_time, int dutyCycleB) {
    // set enB with the PWM dutycycle
    ledcWrite(pwmChannelB, dutyCycleB); // Send PWM signal to motor B (right)
    // set in3 on and in4 off - i.e. HIGH - LOW for right motor forward motion
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    // wait for turn_time ms
    delay(turn_time);
    stop(1);

    Serial.print(" turn left: dutyCycleB - ");
    Serial.println(dutyCycleB);
    Serial.print(" turn left: turn time - ");
    Serial.println(turn_time);
}
 
void turnRight(int turn_time, int dutyCycleA) {
    // set enA with the PWM dutycycle
    ledcWrite(pwmChannelA, dutyCycleA); // Send PWM signal to motor A (left)
    // set in1 on and in2 off i.e. HIGH - LOW for left motor forward motion
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // wait for turn_time ms
    delay(turn_time);
    stop(1);

    Serial.print(" turn right: dutyCycleA - ");
    Serial.println(dutyCycleA);
    Serial.print(" turn left: turn time - ");
    Serial.println(turn_time);
}

 
void spinLeft(int spin_time, int dutyCycleA, int dutyCycleB) {
    // set enA with the PWM dutycycle
    ledcWrite(pwmChannelA, dutyCycleA); // Send PWM signal to motor A (left)
    // set in1 off and in2 on i.e. LOW - HIGH for left motor backward motion
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // set enB with the PWM dutycycle
    ledcWrite(pwmChannelB, dutyCycleB); // Send PWM signal to motor B (right)
    // set in3 on and in4 off i.e. HIGH - LOW for right motor forward motion
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    // wait for spin_time ms
    delay(spin_time);
    stop(1);

    Serial.print(" spin left: dutyCycleA - ");
    Serial.println(dutyCycleA);
    Serial.print(" spin left: dutyCycleB - ");
    Serial.println(dutyCycleB);
    Serial.print(" spin left: spin time - ");
    Serial.println(spin_time);
}
 
void spinRight(int spin_time, int dutyCycleA, int dutyCycleB) {
    // set enA with the PWM dutycycle
    ledcWrite(pwmChannelA, dutyCycleA); // Send PWM signal to motor A (left)
    // set in1 on and in2 off i.e. HIGH - LOW for left motor forward motion
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // set enB with the PWM dutycycle
    ledcWrite(pwmChannelB, dutyCycleB); // Send PWM signal to motor B (right)
    // set in3 off and in4 on i.e. LOW - HIGH for right motor backward motion
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // wait for spin_time ms
    delay(spin_time);
    stop(1);

    Serial.print(" spin right: dutyCycleA - ");
    Serial.println(dutyCycleA);
    Serial.print(" spin right: dutyCycleB - ");
    Serial.println(dutyCycleB);
    Serial.print(" spin right: spin time - ");
    Serial.println(spin_time);
}
 
void stop(int stop_mode) {

    if (stop_mode == 1) {
        // full braking mode
        // set enA with 100% PWM dutycycle
        ledcWrite(pwmChannelA, 255); // Send PWM signal to motor A (left)
        // set in1 off and in2 off i.e. LOW- LOW for no motion
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);

        // set enB with 100% PWM dutycycle
        ledcWrite(pwmChannelB, 255); // Send PWM signal to motor B (right)
        // set in3 off and in4 off i.e. LOW- LOW for no motion
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);

    } 
    
    else if (stop_mode == 0) {
        // coasting stop mode
        // set enA with 0% PWM dutycycle
        ledcWrite(pwmChannelA, 0); // Send PWM signal to motor A (left)
        // set in1 off and in2 off i.e. LOW- LOW for no motion
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);

        // set enB with 0% PWM dutycycle
        ledcWrite(pwmChannelB, 0); // Send PWM signal to motor B (right)
        // set in3 off and in4 off i.e. LOW- LOW for no motion
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
    }
    delay(10); // short pause for the stop to come into effect

    Serial.print(" stop: mode - ");
    Serial.println(stop_mode);
}
