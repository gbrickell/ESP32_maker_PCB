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

const int redled = 14;    // red LED on the Maker Kit PCB
const int greenled = 26;  // green LED on the Maker Kit PCB


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
  
  pinMode(redled, OUTPUT);
  digitalWrite(redled, 1);
  pinMode(greenled, OUTPUT);
  digitalWrite(greenled, 0);

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
  digitalWrite(greenled, 1);

  server.on("/", handleRoot);

  server.on("/inline", []() {
    digitalWrite(redled, 1);
    digitalWrite(greenled, 0);
    delay(1000);
    server.send(200, "text/plain", "this works as well");
    digitalWrite(redled, 0);
    digitalWrite(greenled, 1);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

void handleRoot() {
  digitalWrite(redled, 1);
  digitalWrite(greenled, 0);
  server.send(200, "text/plain", "hello from esp32!");
  delay(1000);
  digitalWrite(redled, 0);
  digitalWrite(greenled, 1);
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
