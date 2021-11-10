/* ESP_servo_sweep.ino ESP32 maker kit version of Sweep by BARRAGAN <http://barraganstudio.com>
    modified by Enmore Green Ltd for use with the ESP32 Maker Kit Oct'21
    
 The original Sweep example code is in the public domain.
 modified 8 Nov 2013
 by Scott Fitzgerald
 modified for the ESP32 on March 2017
 by John Bennett
 see http://www.arduino.cc/en/Tutorial/Sweep for a description of the original code

 * Different servos require different pulse widths to vary servo angle, but the range is 
 * an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
 * sweep 180 degrees, so the lowest number in the published range for a particular servo
 * represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
 * of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
 * 1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 1800
 * degrees.
 * 
 * Circuit: (using a 38pin ESP32 module connected to the Maker Kit PCB)
 * Servo motors have three wires: power, ground, and signal. The power wire is typically red,
 * the ground wire is typically black or brown, and the signal wire is typically yellow,
 * orange or white. Since the ESP32 can supply limited current at only 3.3V, and servos draw
 * considerable power, the servo is connected to the PCB using the dedicated 3P male servo 
 * headers which use the 5V power line powered by the USB supply). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS. 
 *  
 * The GPIO pin is either #32 or #33 for the 2 available 3P servo headers
 * 
 * In this modified example, we assume a SG90 micro servo for which the published 
 * min and max for this servo is 1000us and 2000us, respectively, so the defaults 
 * should have been OK but have been adjusted for the performance seen
 */

#include <ESP32Servo.h>  // installed as the ESP32Servo library

Servo myservo;  // create servo object to control a servo
// up to 16 servo objects could be created on the ESP32 but would need 
//  an additional/separate power supply

int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPin = 32; // servo1 on the Maker Kit PCB but servo2 is equally accessible

void setup() {

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
  
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 Hz servo
	myservo.attach(servoPin, 600, 2500); // attaches the servo on pin 18 to the servo object
	// adjusted the default min/max of 1000us and 2000us to 600 and 2500, but different
	// servos may require different min/max settings for an accurate 0 to 180 sweep
}

void loop() {

	for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		myservo.write(pos);    // tell servo to go to position in variable 'pos'
		delay(15);             // waits 15ms for the servo to reach the position
	}

  // now go back in the other direction
	for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
		myservo.write(pos);    // tell servo to go to position in variable 'pos'
		delay(15);             // waits 15ms for the servo to reach the position
	}
}
