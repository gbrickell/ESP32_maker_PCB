/*
 * simple data collection code that uses a DHT11 module with the ESP32 Maker Kit
 *  
 */

// DHT11_simple.ino - ESP32 maker kit test code that collects temperature and humidity
// readings from a DHT11 module that has either a 5kOhm or 1kOhm pull-up resistor 
// built in to the module between the data line and the 3.3V power supply line
// Data is collected when a button is pressed
// 
//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

/**
 * include the various specialised libraries that are availble once
 *  the OneWireNg main library has been installed in the Arduino IDE
 */

 
#include "DHT.h"  // needs the Arduino "DHT sensor library" and associated "Adadruit Unified Sensor"
#define DHTPIN 33     // Digital pin connected to the DHT signal line - uses the PCB's servo2 pin

// Uncomment whatever type of sensor is being used
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

int switch_pin = 34;  // this is the GPIO pin connected to one side of the bottom button (2)
bool printon = true;  // print control flag

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);


// ****** initial set up ***********
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

    pinMode(switch_pin, INPUT);   // make the button connected GPIO pin an INPUT

    Serial.println(" press button 2 to take a reading .....");

    dht.begin();
}

void loop() {
  

  if (digitalRead(switch_pin) == LOW) {  // check if button has been pressed

    // Wait at least a few seconds between measurements.
    delay(2000);
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    // The 'heat index' is what the temperature feels like to the human body when
    //  relative humidity is combined with the air temperature
    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));

  }
    
}
