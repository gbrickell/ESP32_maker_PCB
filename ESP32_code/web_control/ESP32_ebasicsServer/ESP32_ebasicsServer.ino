// ESP32_HelloServer.ino  basic web server test roution for the ESP32 Maker Kit

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

#include "WiFi.h"
#include "WebServer.h"
#include <WiFiClient.h>
#include <ESPmDNS.h>         // Include the mDNS library

const char* ssid = "xxxxxxxxx";    // use local SSID here
const char* password = "yyyyyyyy"; // use password for local SSID here

// hostname as a char variable
const char* namehost = "38pinESP32";

WebServer server(80);  //instantiate the web server at port 80 (http port#)

// *******************************************
// set the various LED and buzzer variables
// *******************************************
const int redled = 14;    // red LED on the Maker Kit PCB
const int amberled = 27; // amber LED on the Maker Kit PCB
const int greenled = 26;  // green LED on the Maker Kit PCB
const int buzzpin = 25;   // buzzer on the Maker Kit PCB
const int redPin = 17;     // RGB red
const int greenPin = 16;   // RGB green
const int bluePin = 4;     // RGB blue

// *********************
// web page variables
// *********************
String header_content;
String page_content = "";   // initialised global variable for web page content

// *****************************
// ** initial set up function **
// *****************************
void setup(void) {

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
  Serial.println("pin defaults reset..... ");

  // *************************************
  // prepare the RGB and RAG LEDs & buzzer
  // *************************************
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redled, OUTPUT);
  digitalWrite(redled, 1);  // set ON to signify start of program
  pinMode(amberled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(buzzpin, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection using the WiFi.waitForConnectResult() function
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  
  Serial.println("");
  Serial.print("IP address: ");
  delay(500);
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  delay(500);
  Serial.println(WiFi.macAddress());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  WiFi.setHostname(namehost);
  delay(1500);
  Serial.print("updated hostname: ");
  Serial.println(WiFi.getHostname());

  digitalWrite(redled, 0);
  digitalWrite(greenled, 1);  // signifies WiFi connected

  // ************************************************************************
  // interrupt 'handlers' triggered by various Web server requests:
  //  each specific URL that is detected by the web server invokes a 
  //  designated function that does something and then refreshes the web page
  // ************************************************************************
  // ** do this if web root i.e. / is requested
  server.on("/", handle_root);

  // ****** main selection actions ******
  // ** do this if /RAGleds is requested
  server.on("/RAGleds", handle_RAG);
  // ** do this if /RGBleds is requested
  server.on("/RGBleds", handle_RGB);
  // ** do this if /buzz is requested
  server.on("/buzz", handle_buzz);
  // ** do this if /sysinfo is requested
  server.on("/sysinfo", handle_sysinfo);

  // ****** sub selection parameter update actions ******
  // ******  do these if RAG ON/OFFs are selected  ******
  server.on("/RAGredON", handle_RAGredON);
  server.on("/RAGredOFF", handle_RAGredOFF);
  server.on("/RAGamberON", handle_RAGamberON);
  server.on("/RAGamberOFF", handle_RAGamberOFF);
  server.on("/RAGgreenON", handle_RAGgreenON);
  server.on("/RAGgreenOFF", handle_RAGgreenOFF);

  // ******  do these if RGB ON/OFFs are selected  ******
  server.on("/RGBredON", handle_RGBredON);
  server.on("/RGBredOFF", handle_RGBredOFF);
  server.on("/RGBgreenON", handle_RGBgreenON);
  server.on("/RGBgreenOFF", handle_RGBgreenOFF);
  server.on("/RGBblueON", handle_RGBblueON);
  server.on("/RGBblueOFF", handle_RGBblueOFF);

  // ******  do these if buzzer 'tunes' are selected  ******
  server.on("/buzz01", handle_buzz01);
  server.on("/buzz02", handle_buzz02);
  server.on("/buzz03", handle_buzz03);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  // ** do a one-time population of the common header HTML used in all web pages
  header_content = HTMLheader();
  
}

void loop(void) {
  server.handleClient();
}

// *****************************************
// *******  RGB LED colour set *************
// *****************************************
void RGB_color(String colour)
{
    if (colour == "red") {
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);
        digitalWrite(bluePin, LOW);
    } else if (colour == "green") {
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, HIGH);
        digitalWrite(bluePin, LOW);
    } else if (colour == "blue") {
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, LOW);
        digitalWrite(bluePin, HIGH);
    } else if (colour == "off") {
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, LOW);
        digitalWrite(bluePin, LOW);
    } else {
        Serial.println("unrecognised RGB setting");
    }

}

// *************************************************************************
// Function to 'sound' the passive buzzer with a specific frequency/duration
// *************************************************************************
int buzz(int frequency, int duration) {
    int buzzstat = 1;
    // create the function "buzz" and feed it the note (e.g. DS6=1245Hz) and duration (length in ms))
    //Serial.print("Buzzing: pin ");
    //Serial.println(buzzpin);
    //Serial.print("Buzzing: frequency ");   // pitch/frequency of the note
    //Serial.println(frequency);
    //Serial.print("Buzzing: length (ms) "); // length/duration of the note in ms
    //Serial.println(duration);
    if (frequency == 0) {
        delay(duration);
        int buzzstat = 0;
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
    return buzzstat;
    
}

// **********************
// simple beep function
// **********************
void beep(int beeptime) {
    // beeptime in seconds
    int buzzstatus;
    Serial.println("beeping buzzer at 900Hz for beeptime seconds");
    for (int j=0; j<=beeptime-1; j++) {
        // total duration of all the steps below to add up to 1 second
        buzzstatus = buzz(900, 300);
        delay(200);
        buzzstatus = buzz(900, 300);
        delay(200);
    }
}

// **********************************************************************************
// Function to play a specific tune/sound depending upon the passed reference number
// **********************************************************************************
void playtune(int tune) {
    int buzzstatus;
    // use the tune number to select what is played
    if (tune == 1) {
        Serial.println("beeping buzzer at 900Hz for 5 seconds");
        for (int j=0; j<=4; j++) {
            buzzstatus = buzz(900, 500);
            delay(500);
        }
     } else if (tune == 2) {
        Serial.println("beeping buzzer at 600Hz for 5 seconds");
        for (int j=0; j<=4; j++) {
            buzzstatus = buzz(600, 500);
            delay(500);
        }
     } else if (tune == 3) {
        Serial.println("beeping buzzer at 1200Hz for 5 seconds");
        for (int j=0; j<=4; j++) {
            buzzstatus = buzz(1200, 500);
            delay(500);
        }
     } else {
        Serial.println("tune number not valid");
     }
}

// *****************************************************************
// ***  this section is for all the browser response 'handlers'  ***
// *****************************************************************

void handle_root() {
    // ** do this if web root i.e. / is requested
    Serial.println("web root");
    server.send(200, "text/html", HTMLmain()); 
}

// **********************************
// *** main selection 'handlers' ****
// **********************************
// ***** main selection actions *****
void handle_RAG() {
    Serial.println("selecting RAG led control");
    server.send(200, "text/html", HTMLRAG()); 
}

void handle_RGB() {
    Serial.println("selecting RGB led control");
    server.send(200, "text/html", HTMLRGB()); 
}

void handle_buzz() {
    Serial.println("selecting buzzer control");
    server.send(200, "text/html", HTMLbuzz()); 
}

void handle_sysinfo() {
    // ** do this if /sysinfo is requested
    Serial.println("selecting system information display");
    server.send(200, "text/html", HTMLsysinfo()); 
}

// *******************************************
// *** sub selection  operation 'handlers' ***
// *******************************************
// ************ RAG leds ON/OFF **************
void handle_RAGredON() {
    Serial.println("red LED ON - other RAG LEDs OFF");
    digitalWrite(redled, 1);
    digitalWrite(amberled, 0);
    digitalWrite(greenled, 0);
    server.send(200, "text/html", HTMLRAG()); 
}
void handle_RAGredOFF() {
    Serial.println("red LED OFF - other RAG LEDs OFF");
    digitalWrite(redled, 0);
    digitalWrite(amberled, 0);
    digitalWrite(greenled, 0);
    server.send(200, "text/html", HTMLRAG()); 
}

void handle_RAGamberON() {
    Serial.println("amber LED ON - other RAG LEDs OFF");
    digitalWrite(redled, 0);
    digitalWrite(amberled, 1);
    digitalWrite(greenled, 0);
    server.send(200, "text/html", HTMLRAG()); 
}
void handle_RAGamberOFF() {
    Serial.println("amber LED OFF - other RAG LEDs OFF");
    digitalWrite(redled, 0);
    digitalWrite(amberled, 0);
    digitalWrite(greenled, 0);
    server.send(200, "text/html", HTMLRAG()); 
}

void handle_RAGgreenON() {
    Serial.println("green LED ON - other RAG LEDs OFF");
    digitalWrite(redled, 0);
    digitalWrite(amberled, 0);
    digitalWrite(greenled, 1);
    server.send(200, "text/html", HTMLRAG()); 
}
void handle_RAGgreenOFF() {
    Serial.println("green LED OFF - other RAG LEDs OFF");
    digitalWrite(redled, 0);
    digitalWrite(amberled, 0);
    digitalWrite(greenled, 0);
    server.send(200, "text/html", HTMLRAG());
}

// ************ RGB leds ON/OFF **************
void handle_RGBredON() {
    Serial.println("red RGB LED ON - other RGB LEDs OFF");
    digitalWrite(redPin, 1);
    digitalWrite(greenPin, 0);
    digitalWrite(bluePin, 0);    
    server.send(200, "text/html", HTMLRGB()); 
}
void handle_RGBredOFF() {
    Serial.println("red RGB LED OFF - other RGB LEDs OFF");
    digitalWrite(redPin, 0);
    digitalWrite(greenPin, 0);
    digitalWrite(bluePin, 0);    
    server.send(200, "text/html", HTMLRGB()); 
}

void handle_RGBgreenON() {
    Serial.println("green RGB LED ON - other RGB LEDs OFF");
    digitalWrite(redPin, 0);
    digitalWrite(greenPin, 1);
    digitalWrite(bluePin, 0);    
    server.send(200, "text/html", HTMLRGB()); 
}
void handle_RGBgreenOFF() {
    Serial.println("green RGB LED OFF - other RGB LEDs OFF");
    digitalWrite(redPin, 0);
    digitalWrite(greenPin, 0);
    digitalWrite(bluePin, 0);    
    server.send(200, "text/html", HTMLRGB()); 
}

void handle_RGBblueON() {
    Serial.println("blue RGB LED ON - other RGB LEDs OFF");
    digitalWrite(redPin, 0);
    digitalWrite(greenPin, 0);
    digitalWrite(bluePin, 1);    
    server.send(200, "text/html", HTMLRGB()); 
}
void handle_RGBblueOFF() {
    Serial.println("blue RGB LED OFF - other RGB LEDs OFF");
    digitalWrite(redPin, 0);
    digitalWrite(greenPin, 0);
    digitalWrite(bluePin, 0);    
    server.send(200, "text/html", HTMLRGB()); 
}

// ************ buzzer tunes **************
void handle_buzz01() {
    Serial.println("buzzer tune 01 playing");
    playtune(1);
    server.send(200, "text/html", HTMLbuzz()); 
}

void handle_buzz02() {
    Serial.println("buzzer tune 02 playing");
    playtune(2);
    server.send(200, "text/html", HTMLbuzz()); 
}

void handle_buzz03() {
    Serial.println("buzzer tune 03 playing");
    playtune(3);
    server.send(200, "text/html", HTMLbuzz()); 
}

void handleNotFound() {
  digitalWrite(redled, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(redled, 0);
}

// ****************************************************************
// ******  create the various web pages that are being used  ******
// ****************************************************************

// --------------------------------------------------------------------------------
// create the header area used in all the web pages - done once in the setup stage
// --------------------------------------------------------------------------------
String HTMLheader() {
    String h_content = "<!DOCTYPE html> <html>\n";
    h_content +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">\n";
    h_content +="<title>ESP32 Maker Kit</title>\n";
    // style content in the page header ( to be replaced by a css file eventually )
    h_content +="<style>html { font-family: Verdana; display: inline-block; margin: 0px auto; text-align: center; font-size: 15px;}\n";
    h_content +="body{margin-top: 50px;} h1 {color: #444444; margin: 10px auto 10px; font-size: 32px;} h3 {color: #444444; margin: 10px auto 10px; font-size: 24px;} h4 {color: #444444; margin: 10px auto 10px; font-size: 18px;}\n";
    h_content +=".button {display: block; width: 80px; background-color: #1abc9c;border: none;color: white; padding: 0px 20px 10px 20px; text-decoration: none; font-size: 32px; margin: 5px auto 5px; cursor: pointer; border-radius: 4px;}\n";
    h_content +=".btninline {display: inline-block; }\n";
    h_content +=".button-on {background-color: #1abc9c;}\n";
    h_content +=".button-on:active {background-color: #16a085;}\n";
    h_content +=".button-off {background-color: #34495e;}\n";
    h_content +=".button-off:active {background-color: #2c3e50;}\n";
    h_content +=".button-red {background-color: #f51031;}\n";
    h_content +=".button-red:active {background-color: #d20e2a;}\n";
    h_content +="p {font-size: 18px;color: #888; margin: 5px;}\n";
    h_content +="</style>\n";
    h_content +="</head>\n";
    return h_content;
}

// -----------------------------------------
// create the main selection web page
// -----------------------------------------
String HTMLmain(){
    String page_content = header_content;
    // start of main selection web page content
    page_content +="<body>\n";
    page_content +="<h1>";
    page_content += namehost;
    page_content +="</h1>\n";
    page_content +="<h1>ESP32 ebasics server</h1>\n";
    page_content +="<h3>main selections</h1>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"RAGleds\"><button>RAG led control</button></a></p>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"RGBleds\"><button>RGB led control</button></a></p>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"buzz\"><button>Buzzer tune control</button></a></p>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"sysinfo\"><button>System information</button></a></p>\n";
    page_content +="<p>&nbsp;</p>\n";
    page_content +="<p>&nbsp;</p>\n";

    page_content +="</body>\n";
    page_content +="</html>\n";
    return page_content;

}

// -----------------------------------------
// create the RAG led control web page
// -----------------------------------------
String HTMLRAG(){
    String page_content = header_content;
    // start of main selection web page content
    page_content +="<body>\n";
    page_content +="<h1>";
    page_content += namehost;
    page_content +="</h1>\n";
    page_content +="<h1>ESP32 ebasics server</h1>\n";
    page_content +="<h3>RAG led control</h1>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"RAGredON\"><button>RAG red led ON</button></a>&nbsp; &nbsp; &nbsp;";
    page_content +="<a class=\"button btninline button-off\" href=\"RAGredOFF\"><button>RAG red LED OFF</button></a></p>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"RAGamberON\"><button>RAG amber led ON</button></a>&nbsp; &nbsp; &nbsp;";
    page_content +="<a class=\"button btninline button-off\" href=\"RAGamberOFF\"><button>RAG amber led OFF</button></a></p>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"RAGgreenON\"><button>RAG green led ON</button></a>&nbsp; &nbsp; &nbsp;";
    page_content +="<a class=\"button btninline button-off\" href=\"RAGgreenOFF\"><button>RAG green led OFF</button></a></p>\n";

    page_content +="<p>&nbsp;</p>\n";
    page_content +="</html>\n";page_content +="<p><a class=\"button button-off\" href=\"/\"><button>back to main selection</button></a></p>\n";

    page_content +="</body>\n";
    page_content +="</html>\n";
    return page_content;

}

// -----------------------------------------
// create the RGB led control web page
// -----------------------------------------
String HTMLRGB(){
    String page_content = header_content;
    // start of main selection web page content
    page_content +="<body>\n";
    page_content +="<h1>";
    page_content += namehost;
    page_content +="</h1>\n";
    page_content +="<h1>ESP32 ebasics server</h1>\n";
    page_content +="<h3>RGB led control</h1>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"RGBredON\"><button>RGB red led ON</button></a>&nbsp; &nbsp; &nbsp;";
    page_content +="<a class=\"button btninline button-off\" href=\"RGBredOFF\"><button>RGB red LED OFF</button></a></p>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"RGBgreenON\"><button>RGB green led ON</button></a>&nbsp; &nbsp; &nbsp;";
    page_content +="<a class=\"button btninline button-off\" href=\"RGBgreenOFF\"><button>RGB green led OFF</button></a></p>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"RGBblueON\"><button>RGB blue led ON</button></a>&nbsp; &nbsp; &nbsp;";
    page_content +="<a class=\"button btninline button-off\" href=\"RGBblueOFF\"><button>RGB blue led OFF</button></a></p>\n";
    

    page_content +="<p>&nbsp;</p>\n";
    page_content +="</html>\n";page_content +="<p><a class=\"button button-off\" href=\"/\"><button>back to main selection</button></a></p>\n";

    page_content +="</body>\n";
    page_content +="</html>\n";
    return page_content;

}

// -----------------------------------------
// create the buzzer tune web page
// -----------------------------------------
String HTMLbuzz(){
    String page_content = header_content;
    // start of main selection web page content
    page_content +="<body>\n";
    page_content +="<h1>";
    page_content += namehost;
    page_content +="</h1>\n";
    page_content +="<h1>ESP32 ebasics server</h1>\n";
    page_content +="<h3>buzzer tunes</h1>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"buzz01\"><button>buzzer tune 1 900Hz</button></a></p>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"buzz02\"><button>buzzer tune 2 600Hz</button></a></p>\n";
    page_content +="<p><a class=\"button btninline button-off\" href=\"buzz03\"><button>buzzer tune 3 1200Hz</button></a></p>\n";
    
    page_content +="<p>&nbsp;</p>\n";
    page_content +="<p>&nbsp;</p>\n";

    page_content +="</body>\n";
    page_content +="</html>\n";page_content +="<p><a class=\"button button-off\" href=\"/\"><button>back to main selection</button></a></p>\n";
    return page_content;

}

// ---------------------------------------------
// create the system information display web page
// ---------------------------------------------
String HTMLsysinfo(){
    String page_content = header_content;

    // start of main selection web page content
    page_content +="<body>\n";
    page_content +="<h1>";
    page_content += namehost;
    page_content +="</h1>\n";
    page_content +="<h1>ESP32 ebasics server</h1>\n";
    page_content +="<h3>System Information</h3>\n";

    // **** networking information ****
    page_content +="<div style=\" font-size: 18px; margin-bottom: 5px; margin-top: 10px;\"><b>Networking:</b></div>\n";
    page_content +="<table border=\"1\" style=\"width: 450px; text-align: left;\" align=\"center\" cellpadding=\"3\" cellspacing=\"0\">\n";
    page_content +="<tr><td>host name:</td><td>" ;
    page_content +=WiFi.getHostname();
    page_content +="</td></tr>\n";
    page_content +="<tr><td>assigned IP address:</td><td>" ;
    page_content +=WiFi.localIP().toString();
    page_content +="</td></tr>\n";
    page_content +="<tr><td>WiFi MAC address:</td><td>" ;
    page_content +=WiFi.macAddress().c_str();
    page_content +="</td></tr>\n";
    page_content +="</table>\n";

    // **** memory information ****
    page_content +="<div style=\" font-size: 18px; margin-bottom: 5px; margin-top: 10px;\"><b>Memory information: Internal RAM</b></div>\n";
    page_content +="<table border=\"1\" style=\"width: 450px; text-align: left;\" align=\"center\" cellpadding=\"3\" cellspacing=\"0\">\n";

    //page_content +="<tr><td style=\"width: 225px; \">Total heap size:</td><td>" ;
    //page_content +=String(ESP.String(ESP.getHeapSize());
    //page_content +="</td></tr>\n";

    page_content +="<tr><td style=\"width: 225px; \">Available heap:</td><td>" ;
    page_content +=String(ESP.getFreeHeap());
    page_content +="</td></tr>\n";

    page_content +="<tr><td style=\"width: 225px; \">lowest level of free heap since boot:</td><td>" ;
    page_content +=String(ESP.getMinFreeHeap());
    page_content +="</td></tr>\n";

    page_content +="<tr><td style=\"width: 225px; \">largest block of heap that can be allocated at once:</td><td>" ;
    page_content +=String(ESP.getMaxAllocHeap());
    page_content +="</td></tr>\n";

    page_content +="</table>\n";

    page_content +="<div style=\" font-size: 18px; margin-bottom: 5px; margin-top: 10px;\"><b>Memory information: SPI RAM</b></div>\n";
    page_content +="<table border=\"1\" style=\"width: 450px; text-align: left;\" align=\"center\" cellpadding=\"3\" cellspacing=\"0\">\n";

    page_content +="<tr><td style=\"width: 225px; \">Total RAM size:</td><td>" ;
    page_content +=String(ESP.getPsramSize());
    page_content +="</td></tr>\n";

    page_content +="<tr><td style=\"width: 225px; \">Free RAM:</td><td>" ;
    page_content +=String(ESP.getFreePsram());
    page_content +="</td></tr>\n";

    page_content +="<tr><td style=\"width: 225px; \">Minimum free RAM:</td><td>" ;
    page_content +=String(ESP.getMinFreePsram());
    page_content +="</td></tr>\n";

    page_content +="<tr><td style=\"width: 225px; \">Maximum allocatable RAM:</td><td>" ;
    page_content +=String(ESP.getMaxAllocPsram());
    page_content +="</td></tr>\n";

    page_content +="</table>\n";

    // **** chip and firmware information ****
    page_content +="<div style=\" font-size: 18px; margin-bottom: 5px; margin-top: 10px;\"><b>Chip and Firmware information:</b></div>\n";
    page_content +="<table border=\"1\" style=\"width: 450px; text-align: left;\" align=\"center\" cellpadding=\"3\" cellspacing=\"0\">\n";

    page_content +="<tr><td style=\"width: 225px; \">chip revision::</td><td>" ;
    page_content +=String(ESP.getChipRevision());
    page_content +="</td></tr>\n";

    page_content +="<tr><td>Flash chip size:</td><td>" ;
    page_content +=ESP.getFlashChipSize();
    page_content +="</td></tr>\n";

    page_content +="<tr><td>SDK version::</td><td>" ;
    page_content +=String(ESP.getSdkVersion());
    page_content +="</td></tr>\n";

    page_content +="</table>\n";

    page_content +="<p>&nbsp;</p>\n";

    page_content +="<p><a class=\"button button-off\" href=\"/\"><button>back to main selection</button></a></p>\n";
    page_content +="</body>\n";
    page_content +="</html>\n";
    return page_content;
}
