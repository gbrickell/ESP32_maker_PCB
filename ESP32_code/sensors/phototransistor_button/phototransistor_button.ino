/// phototransistor_button.ino ESP32 maker kit version of phototransistor_button.py

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

int red_positive_pin = 14; // red LED on ESP32 Maker Kit1
int amber_positive_pin = 27;       //amber LED on ESP32 Maker Kit1
int green_positive_pin = 26;       //green LED on ESP32 Maker Kit1
int switch_pin = 34;       // this is the GPIO pin that one side of the bottom button (2) is connected to
int buzzpin = 25;          // buzzer GPIO pin
int pin_charge = 2;        // GPIO pin connected to the phototransistor collector & when set HIGH charges the capacitor through a fixed 1k resistor
int pin_discharge = 15;    // GPIO pin connectd to the emitter/capacitor & when set LOW discharges the capacitor through a fixed 1k resistor

unsigned long StartTime;
unsigned long CurrentTime;
unsigned long ChargeTime = 0;

bool printon = true;
bool btn_pressed = false;

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

  pinMode(red_positive_pin, OUTPUT);   // make the LED connected GPIO pin an OUTPUT
  pinMode(amber_positive_pin, OUTPUT);
  pinMode(green_positive_pin, OUTPUT);
  
  pinMode(switch_pin, INPUT);   // make the button connected GPIO pin an INPUT
  
  pinMode(buzzpin, OUTPUT);

}

void loop() {

  if (printon) {
    digitalWrite(red_positive_pin, HIGH);  
    digitalWrite(amber_positive_pin, LOW);  // red LED on and amber off to signify the process is waiting
    Serial.println("red LED ON and amber OFF");
    Serial.println("press the bottom button (button 2) to collect data from the phototransistor");
    printon = false;
  }

  if (digitalRead(switch_pin) == LOW) {
    Serial.println("button press detected");
    Serial.println("collecting the phototransistor data");
    beep(1);   // beep to signify the start of the process
    digitalWrite(red_positive_pin, LOW);  
    digitalWrite(amber_positive_pin, HIGH);     // red LED off and amber on to signify the start of the process
    Serial.println("red LED OFF and amber ON");
    float photo_time = phototransistor_time(5);    // calculate the average recharge time from 5 readings
    Serial.print("average phototransistor recharge time (ms): " );
    Serial.println(photo_time);
    btn_pressed = false;  // reset the button pressed 'flag'
    printon = true;       // reset the print message 'flag' 
    // now loop back to wait for the button to be pressed again
  }

}

// *************************************************************************
// Function to 'sound' the passive buzzer with a specific frequency/duration
// *************************************************************************
int buzz(int frequency, int duration) {   
    int buzzstat;
    // create the function "buzz" and feed it the note (e.g. DS6=1245Hz) and duration (length in ms))
    //Serial.print("Buzzing: pin ");
    //Serial.println(buzzpin);
    //Serial.print("Buzzing: frequency ");   // pitch/frequency of the note
    //Serial.println(frequency);
    //Serial.print("Buzzing: length (ms) "); // length/duration of the note in ms
    //Serial.println(duration);
    if (frequency == 0) {
        delay(duration);
        buzzstat = 0;
        return buzzstat;
    }
    // from the frequency calculate the time between buzzer pin HIGH-LOW setting in microseconds
    //float period = 1.0 / frequency;       // in physics, the period (sec/cyc) is the inverse of the frequency (cyc/sec)
    int delayValue = (1000*1000/frequency)/2;  // calculate the time in microseconds for half of the wave
    int numCycles = int((duration * frequency)/1000);   // the number of waves to produce is the duration times the frequency
    //Serial.print("Number of cycles: ");
    //Serial.println(numCycles);
    //Serial.print("Hi-Low delay (microsecs): ");
    //Serial.println(delayValue);
    for (int i=0; i<=numCycles-1; i++) {  // start a loop from 0 to the variable "cycles" calculated above
        digitalWrite(buzzpin, HIGH);      // set buzzer pin to high
        delayMicroseconds(delayValue);    // wait with buzzer pin high
        digitalWrite(buzzpin, LOW);       // set buzzer pin to low
        delayMicroseconds(delayValue);    // wait with buzzer pin low
    }

    buzzstat = 1;
    return buzzstat;
}

// **********************
// simple beep function
// **********************
void beep(int beeptime) {
    // beeptime in seconds
    int status;
    Serial.println("beeping buzzer at 900Hz for beeptime seconds");
    for (int j=0; j<=beeptime-1; j++) {
        // total duration of all the steps below to add up to 1 second
        status = buzz(900, 300);
        delay(200);
        status = buzz(900, 300);
        delay(200);
    }
}


// *************************************************************************************************
// this function discharges (empties) the 330nF capacitor so it is ready to be recharged (filled up)
// *************************************************************************************************
void discharge() {
    pinMode(pin_charge, INPUT);
    pinMode(pin_discharge, OUTPUT);
    digitalWrite(pin_discharge, LOW);
    delay(200);  // this wait time is set to a value large enough for the capacitor to fully discharge
    //Serial.println ("capacitor discharged");  // show this debug by deleting the // in front of the print
}

// ***********************************************************************************************************
// this function charges (fills up) the the 330nF capacitor and returns the time taken for the GPIO pin_charge 
// to go HIGH this means the voltage on the capacitor has reached approx. 1.65V
// ***********************************************************************************************************
void charge_time() {
    pinMode(pin_discharge, INPUT);    // configure the GPIO pin_discharge to be an input
    pinMode(pin_charge, OUTPUT);      // configure the GPIO pin_charge to be an output
    digitalWrite(pin_charge, HIGH);   // set the GPIO pin_charge HIGH so that it starts to charge the capacitor
    StartTime = millis();            // start the clock by recording the current time
    while (digitalRead(pin_discharge) != HIGH) {  // loop while the GPIO pin_discharge is still not HIGH ie capacitor not charged
        delay(1);
    }
    CurrentTime = millis();          // stop the clock by recording the difference between the current and the start times
    //  if here then the GPIO pin_discharge is HIGH ie the capacitor has charged so we can calculate the time in milliseconds
    ChargeTime = CurrentTime - StartTime;    // ChargeTime is a global variable so no need to return it
    // Serial.print("capacitor recharged time (ms): ");
    // Serial.println(ChargeTime);   //  show this debug by deleting the // in front of the print
}

// *************************************************************************************************
// this is a function to repetitively discharge and recharge the 330nF capacitor that is in series
// with the phototransistor and obtain an average time in milliseconds for the recharging
//  - the longer this time, the higher the phototransistor 'resistance' and the lower the light level
// ************************************************************************************************
float phototransistor_time(int repeats) {    // start the definition of the function
    int total = 0;
    for (int loop=0; loop<=repeats; loop++) {
        //Serial.print("loop counter: ");  // show this debug by deleting the // in front of the print
        //Serial.println(loop);               // show this debug by deleting the // in front of the print
        discharge();     // discharge the capacitor
        charge_time();   // recalculate the new ChargeTime
        total = total + ChargeTime;
        ChargeTime = 0;  // shouldn't be necessary but reset the ChargeTime to zero
        //Serial.print("current total time (ms): ");    // show this debug by deleting the // in front of the print
        //Serial.println(total);                        // show this debug by deleting the // in front of the print
    }
    float av_time = (float)total / float(repeats);
    Serial.print("average recharge time (ms): "); // show this debug by deleting the // in front of the print
    Serial.println(av_time);
    return av_time;

}

//=======================================================================
