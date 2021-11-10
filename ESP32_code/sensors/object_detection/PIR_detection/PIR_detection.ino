// PIR_detection.ino ESP32 kits PCB simple PIR object detection using the 
//   HC-SR501, HC-SR312 or HC-SR602 sensors
//   **** NB this uses the standard 'PIR' input pins by inserting the 3 pins which
//         powers the unit at 5V and the signal pin is #36
//

#include <WiFi.h>
#include <NTPClient.h>  // downloaded from https://github.com/taranais/NTPClient/archive/master.zip
#include <WiFiUdp.h>

// define all the variables here to make them global

int sense_pin = 36;  // this is the GPIO input pin that the usual PIR OUT pin is connected to, which is a 
                     //  simple way to connect the sensor to the PCB as the 3 main pins are aligned similarly

// initialise some control variables
int Current_State  = 0;   // simple variable to indicate the detection state
int trigger_interval = 2000; // minimum interval in milliseconds between detections to allow the 
                             // electronics to reset - the default on the electronics is 2 seconds

int counter = 0;

// network credentials
const char* ssid     = "PWR-WiFi";
const char* password = "digmaker99";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;


void setup() {

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

  pinMode(sense_pin, INPUT);

  Serial.println(" PIR detection program starting: ");
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address 
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  Serial.println("timestamps @GMT");
  // GMT 0 = 0

  // get current time/date
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  //Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");
  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);
  delay(1000);  

  // Initial mini loop to wait until until PIR output is 0 ie the electronics have settled

  while (digitalRead(sense_pin) == 1) {
    Current_State  = 0;
  }
  Serial.println ("  Ready - starting detection loop");
  Serial.println ("*********************************");

}

void loop() {

  //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // check for movement detection
  //+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  // Read SR501 state from the input GPIO pin
  Current_State = digitalRead(sense_pin);
  //Serial.print(" Current State = "); // debug output usually commented out
  //Serial.println(Current_State);     // debug output usually commented out

  if (Current_State == 1) {
    //PIR is triggered 
    counter = counter + 1;
    Serial.print (" SR501, SR312 or SR602 detected warm body: " );
    Serial.println(counter);
    Serial.println ("******************************************");
    delay(trigger_interval);
    Serial.print(" waited ");
    Serial.print(trigger_interval);
    Serial.println(" milliseconds to let the electronics reset");
    Serial.println(" .... now looking again for a warm body to be detected");
    Current_State  = 0;
  }

  // go round the loop again  
}
