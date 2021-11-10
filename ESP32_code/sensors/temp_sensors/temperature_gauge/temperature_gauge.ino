// temperature_gauge.ino ESP32 maker kit version of temperature_gauge.py that provides an approximate 
//   temperature using the thermistor when a button is pressed
// this is done by measuring the resistance of the thermistor using a so-called 'step response' method
//   and then the resistance is interpreted as a temperature according to the Steinhart-Hart equation
 
//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

#include <math.h>

int red_positive_pin = 14; // red LED on ESP32 Maker Kit1
int amber_positive_pin = 27;       //amber LED on ESP32 Maker Kit1
int green_positive_pin = 26;       //green LED on ESP32 Maker Kit1
int switch_pin = 34;       // this is the GPIO pin that one side of the bottom button (2) is connected to
int buzzpin = 25;          // buzzer GPIO pin
int pin_charge = 12; // GPIO pin connected to the non-capacitor side of the thermistor & when set HIGH charges the capacitor through a fixed 1k resistor
int pin_discharge = 13;  // GPIO pin connected to the thermistor/capacitor & when set LOW discharges the capacitor through a fixed 1k resistor

unsigned long StartTime;
unsigned long CurrentTime;
double ChargeTime = 0.0;

double GPIO_high = 2.14; // this is the voltage at which a GPIO pin will change from LOW to HIGH
// this is the most inaccurate 'guessed value' - and ideally it should be adjusted 
//  with some practical measurements to arrive at the measured thermistor resistance if known

double C = 0.324;   // this is the value of the capacitor in uF i.e. 330nF - adjust this value to accurate value if known
double R1 = 976.0;  // this is the value of the fixed resistors i.e. 1kohms - adjust this value to accurate value if known

// ref https://www.newport.com/medias/sys_master/images/images/hdb/hac/8797291479070/TN-STEIN-1-Thermistor-Constant-Conversions-Beta-to-Steinhart-Hart.pdf
double B = 3950.0;   // this is the B constant for use in the simple B or beta method
double R0 = 10000.0; // reference resistance at 25C in ohms for use in the simple Beta method

// Steinhart-Hart a, b, c constants for a 10kohm thermistor
// ref http://www.skyeinstruments.com/wp-content/uploads/Steinhart-Hart-Eqn-for-10k-Thermistors.pdf
double SH_A = 0.001125308852122;
double SH_B = 0.000234711863267;
double SH_C = 0.000000085663516;

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
    digitalWrite(green_positive_pin, LOW);  // red LED on and green off to signify the process is waiting
    Serial.println("red LED ON and amber OFF");
    Serial.println("press the bottom button (button 2) to collect data from the thermistor");
    digitalWrite(amber_positive_pin, LOW);  // amber LED off as well
    printon = false;
  }
  
  if (digitalRead(switch_pin) == LOW) {
    Serial.println("collecting the thermistor data");
    beep(1);   // beep to signify the start of the process
    digitalWrite(red_positive_pin, LOW);  
    digitalWrite(amber_positive_pin, HIGH);  // red LED off and amber on to signify the start of the process
    Serial.println("red LED OFF and amber ON");
    float av_R = calc_resistance();    // calculate the average resistance from 20 readings
    Serial.print("average resistance ohms: " );
    Serial.println(av_R, 18);
    Serial.print("simple temp calc: ");
    Serial.println(read_temp_c_simple(av_R), 18);
    Serial.print("Steinhart-Hart temp calc: ");
    Serial.println(read_temp_c_SH(av_R), 18);
    digitalWrite(green_positive_pin, HIGH);   // briefly show the green LED to signify the process is complete
    delay(2000);
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
    //Serial.println(duration, 18);
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
    //Serial.println(delayValue, 18);
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
    StartTime = micros();            // start the clock by recording the current time
    //Serial.print("StartTime: ");
    //Serial.println(StartTime, 19);
    while (digitalRead(pin_discharge) != HIGH) {  // loop while the GPIO pin_discharge is still not HIGH ie capacitor not charged
        delay(1);
    }
    CurrentTime = micros();          // stop the clock by recording the difference between the current and the start times
    //Serial.print("CurrentTime: ");
    //Serial.println(CurrentTime, 19);
    //  if here then the GPIO pin_discharge is HIGH ie the capacitor has charged so we can calculate the time in milliseconds
    ChargeTime = (CurrentTime - StartTime)/1000.0;    // ChargeTime is a global variable so no need to return it
    //Serial.print("capacitor recharged time (ms): ");
    //Serial.println(ChargeTime, 19);   //  show this debug by deleting the // in front of the print
}


// ****************************************************************************************************************
// this function converts the time taken to charge the capacitor into a value of resistance
// to make the answer as good as possible and reduce errors, a number of readings are taken and the average used.
// ****************************************************************************************************************
double calc_resistance() {
    int n = 20;             // this is the number of readings we will take
    float totaltime = 0.0;  // set the total of the time readings to zero at the start of the cycle
    for (int rloop=0; rloop+1 <=n; rloop++) {    // loop through the following lines of code n times
        discharge();     // discharge the capacitor
        charge_time();   // recalculate the new ChargeTime        
        totaltime = totaltime + ChargeTime;   // this is the running total of reading times taken so far
        //Serial.print("rloop ");
        //Serial.print(rloop+1);
        //Serial.print(" of ");
        //Serial.println(n);     // show these debugs by deleting the // in front of the print lines
        //Serial.print("current total time: ");
        //Serial.println(totaltime, 18);
    }

    double t_average = (double)totaltime / (double)n;   // on completion of the loop above calculate the average time reading
    Serial.print("t_average: "); 
    Serial.println(t_average, 18);
    // tau is the RC time constant ie tau = R*C and is defined as the time tau:seconds R:ohms and C:Farads
    // to reach 63.2% of the capacitor charging voltage - which is 3.3V from the GPIO pin
    // and from a normal charging curve at 0.7*tau the capacitor voltage will be 50% of the charging voltage
    // at this part of the charging curve we can approximate it to a straight line
    // so a simple approximation is that the ratio of the time to reach GPIO_high is the same as reaching 50% i.e
    double tau = 1000*(t_average * 0.5 * 3.3)/(GPIO_high * 0.7);   // t_average is in ms so convert to usecs
    Serial.print("tau: "); 
    Serial.println(tau, 18);

    double R_combined = tau/C;             // if we know tau then we can calculate the combined resistance from tau=R*C
    Serial.print("R combined (ohms): "); 
    Serial.println(R_combined, 18);
    double R_thermistor = R_combined - R1;  // therefore the thermistor resistance is calculated by subtracting the fixed resistor

    Serial.print("R thermistor (ohms): "); 
    Serial.println(R_thermistor, 18);       
    return R_thermistor;
}

// *******************************************************
// calculate the temperature using the simple beta method
// *******************************************************
double read_temp_c_simple(double resist) {

    double t0 = 273.15;     // 0 deg C in K
    double t25 = t0 + 25.0; // 25 deg C in K

    Serial.println("resist, t0, t25: "); 
    Serial.println(resist, 18);
    Serial.println(t0);
    Serial.println(t25);

    // simple beta method

    Serial.println("1/t25, 1/B log(resist/R0: "); 
    Serial.println(1.0/t25, 18);
    Serial.println(1.0/B, 18);
    Serial.println(log(resist/R0), 18);
    
    double beta_inv_T = 1.0/t25 + 1.0/B * log(resist/R0);  // NB natural log
    Serial.print("simple beta_inv_t: "); 
    Serial.println(beta_inv_T, 18);
    double beta_T = (1/beta_inv_T - t0);

    return beta_T;
}

// ************************************************************
// calculate the temperature using the Steinhart-Hart equation
// ************************************************************
double read_temp_c_SH(double resist) {

    double t0 = 273.15;     // 0 deg C in K
    double t25 = t0 + 25.0; // 25 deg C in K

    // full Steinhart-Hart equation
    double inv_T = SH_A + SH_B * log(resist) + SH_C * pow(log(resist),3);  // NB natural logs
    Serial.print("Steinhart-Hart inv_t: "); 
    Serial.println(inv_T, 18);
    double T = (1/inv_T - t0);
    return T;
}

//=======================================================================
