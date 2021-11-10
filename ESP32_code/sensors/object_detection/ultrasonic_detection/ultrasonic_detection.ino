// ultrasonic_detection.ino ESP32 kits PCB simple ultrasonic object detection using the HC-SR04P sensor
//   **** NB this must be the 3.3V powered 'P' version ***connected to the spare #3 (trig) and #35 (echo) pins
//

// define all the variables here to make them global

// ***************************
// ultrasonic sensor variables
// ***************************
int trigPin = 3;    // pin #3 used for the sensor Ping Pin (Trig)
int echoPin = 35;   // pin #35 used for the sensor Echo Pin (Echo)
unsigned long duration, inches;
int indec, cmdec;
int inchconv = 147; // ratio between pulse width and inches
int cmconv = 59;    // ratio between pulse width and cm
int avoid_distance = 20;   // set a defined distance to take avoidance action
int avoid_limit = 2;       // number of times distance limit is reached before action taken - avoids spurious readings
int avoid_count = 0;       // number of times that distance limit is reached
int cm, lcm, rcm;

void setup()
{
  Serial.begin(115200);


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

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println(" ultrasonic sensor test program start: ");
  
}


void loop()
{
    us_sense();  // run the ultrssonic sensor routine
    delay(500);
}



// ***************************
// ultrasonic sensor function
// ***************************
void us_sense()
{

  cm = getDistance();
  Serial.print(" distance: ");
  Serial.println(cm);

  if (cm < avoid_distance) {  // if an object is less than the avoid distance then 'count' the # of times this has happened
    avoid_count = avoid_count + 1;
    Serial.print(" distance: ");
    Serial.println(cm);
    Serial.print(" avoid count: ");
    Serial.println(avoid_count);
  }
  
  if(avoid_count >= avoid_limit) {  // if avoid count limit is reached then take avoidance action
    avoid_count = 0;  // reset the count limit first
    Serial.print(" avoidance action to be taken ");
    delay(2000);   // short wait so that print output can be seen
  }
  
}


// *********************************************
// function to sense the distance to an obstacle
// *********************************************
int getDistance()
{
  int rval;
  // send trigger pulse and time how long it takes to 'hear' it come back
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 38000L);  // Set timeout to 38mS
  if (duration == 0)
    duration = 1000;
  rval = microsecondsToCentimeters(duration);
//  Serial.println(rval);
  return rval;
}

// *******************
// conversion function
// *******************
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / cmconv;
}
