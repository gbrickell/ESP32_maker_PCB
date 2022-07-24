// SD_reader_OLED_SPI_test01.ino  SPI SD card reader for the ESP32 Maker Kit
//  - based upon the ESP32 sketch found at: Examples > SD(esp32) > SD_Test

//  code developed and compiled using the Arduino IDE v1.8.19
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with


// uses the ThingPulse OLED driver "ESP8266 and ESP32 Oled Driver for SSD1306 display"
#include "SSD1306Wire.h"        // used for the 128x64 OLED
#include "Open_Sans_Regular_12.h"
#include "Open_Sans_Regular_16.h"
#include "Open_Sans_Regular_18.h"
#include "Open_Sans_Regular_24.h"

#include "FS.h"
#include "SD.h"
#include "SPI.h"

SPIClass sdSPI(VSPI);

void setup(){

  Serial.begin(115200);
  delay(1500); // short delay to set up the monitor

  Serial.print("program starting ..... ");

  // pins 1, 3, 5, 14, and 15 are set HIGH on boot - so set them LOW unless/until they are needed
  //pinMode(1, OUTPUT);
  pinMode(3, OUTPUT);
  //pinMode(5, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  //digitalWrite(1, LOW);
  digitalWrite(3, LOW);
  //digitalWrite(5, LOW);
  digitalWrite(14, LOW);
  digitalWrite(15, LOW); 

  // set up the OLED and display start text
  SSD1306Wire display(0x3c, 21, 22);
  display.init();  
  Serial.println("\n128x64 OLED initialised");
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "test routine", "starting up", "");
  Serial.println("OLED start text displayed .... ");
  delay(4000);
  oledclear();

  // initialise the SD card reader 
  // - the standard SD.h library uses VSPI as its default and the default 
  //   VSPI pins exposed on the 7-pin (black) header on the Maker Kit PCB are:
  //   CS:   05 (labelled SS on the PCB)
  //   MOSI: 23 - pulled HIGH with 10kOhm resistor
  //   CLK:  18 (labelled SCK on the PCB)
  //   MISO: 19 - pulled HIGH with 10kOhm resistor
  
  // try for a maximum of 6 times to start the SD card reader
  for (int i=0; i<=5; i++) { 
    delay(500);
    if(!SD.begin()){
      Serial.print("Card Mount Failed: ");
      Serial.println(i);
    } else {
      Serial.println("Card Reader Mounted ....");
      oledclear();
      oledtext(3, 0, 0, 0, 0, 16, "SD reader", "mounted ", " ", "");
      delay(3000);
      oledclear();
      break;
    }
    if (i == 5) {
      oledclear();
      oledtext(3, 0, 0, 0, 0, 16, "SD reader", "failed ", "to mount", "");
      delay(3000);
      oledclear();
      return;
    }
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    oledclear();
    oledtext(3, 0, 0, 0, 0, 16, "SD reader", "NO card ", "inserted", "");
    delay(3000);
    oledclear();
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
    oledclear();
    oledtext(3, 0, 0, 0, 0, 16, "SD reader", "MMC card ", "type inserted", "");
    delay(3000);
    oledclear();
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
    oledclear();
    oledtext(3, 0, 0, 0, 0, 16, "SD reader", "SDSC card ", "type inserted", "");
    delay(3000);
    oledclear();
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
    oledclear();
    oledtext(3, 0, 0, 0, 0, 16, "SD reader", "SDHC card ", "type inserted", "");
    delay(3000);
    oledclear();
  } else {
    Serial.println("UNKNOWN");
    oledclear();
    oledtext(3, 0, 0, 0, 0, 16, "SD reader", "UNKNOWN card ", "type inserted", "");
    delay(3000);
    oledclear();
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "card size", "read", "");
  delay(3000);
  oledclear();
  
  listDir(SD, "/", 0);  
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "dir /", "listed", "");
  delay(3000);
  oledclear();
  
  listDir(SD, "/1", 0);
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "dir /1", "listed", "");
  delay(3000);
  oledclear();
  
  listDir(SD, "/2", 0);
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "dir /2", "listed", "");
  delay(3000);
  oledclear();
  
  createDir(SD, "/mydir");
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "dir /mydir", "created", "");
  delay(3000);
  oledclear();
  
  listDir(SD, "/", 0);
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "dir /", "listed", "");
  delay(3000);
  oledclear();
  
  removeDir(SD, "/mydir");
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "dir /mydir", "removed", "");
  delay(3000);
  oledclear();
  
  listDir(SD, "/", 0);
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "dir /", "listed", "");
  delay(3000);
  oledclear();
  
  writeFile(SD, "/hello.txt", "Hello ");  
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "hello.txt", "written", "");
  delay(3000);
  oledclear();
  
  appendFile(SD, "/hello.txt", "World!\n");
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "hello.txt", "appended", "");
  delay(3000);
  oledclear();
  
  readFile(SD, "/hello.txt");
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "hello.txt", "read", "");
  delay(3000);
  oledclear();
  
  deleteFile(SD, "/foo.txt");
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "foo.txt", "deleted", "");
  delay(3000);
  oledclear();
  
  renameFile(SD, "/hello.txt", "/foo.txt");
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "hello.txt", "renamed foo", "");
  delay(3000);
  oledclear();
  
  readFile(SD, "/foo.txt");
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "foo.txt", "read", "");
  delay(3000);
  oledclear();
  
  testFileIO(SD, "/test.txt");
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "test.txt", "read timed", "");
  delay(3000);
  oledclear();
  
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
  
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "SD space", "listed", "");
  delay(3000);
  oledclear();
  
  oledclear();
  oledtext(3, 0, 0, 0, 0, 16, "SD reader", "tests", "ended", "");
  delay(3000);
  oledclear();
  
}

void loop(){

}

// ****************************************************
// individual functions to carry out SD reader actions
// ****************************************************

// ********* list the directories **************
// function arguments are:
// - the file system (SD)
// - the main directory name
// - levels to drill down within the directory
// *********************************************
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

// *********** create a directory **************
// function arguments are:
// - the file system (SD)
// - the new directory path
// *********************************************
void createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

// ********** remove a directory **************
// function arguments are:
// - the file system (SD)
// - the path of the directory to be removed
// *********************************************
void removeDir(fs::FS &fs, const char * path){
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

// ********** read file content **************
// function arguments are:
// - the file system (SD)
// - the file path 
// *******************************************
void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

// ********** write content to a file ********
//          overwriting previous content
// function arguments are:
// - the file system (SD)
// - the file path 
// - the file content as a char variable
// *******************************************
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// ********** append content to a file ********
//          keeping the existing content
// function arguments are:
// - the file system (SD)
// - the file path 
// - the file content as a char variable
// *******************************************
void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

// ********** rename a file **************
// function arguments are:
// - the file system (SD)
// - the original file name + path 
// - the new file name + path 
// ***************************************
void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

// ********** delete a file **************
// function arguments are:
// - the file system (SD)
// - the file name + path to be deleted
// ***************************************
void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

// *** show how long it takes to read a file ***
// function arguments are:
// - the file system (SD)
// - the file name + path to be read
// *********************************************
void testFileIO(fs::FS &fs, const char * path){
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}



void oledtext(int rows, int col1, int col2, int col3, int col4, int fontsize, String text1, String text2, String text3, String text4) {
    SSD1306Wire display(0x3c, 21, 22);
    display.init();
    display.flipScreenVertically();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    if (fontsize == 12) {
        display.setFont(Open_Sans_Regular_12);
    } else if (fontsize == 16) {
        display.setFont(Open_Sans_Regular_16);
    } else if (fontsize == 18) {
        display.setFont(Open_Sans_Regular_18);
    } else if (fontsize == 24) {
        display.setFont(Open_Sans_Regular_18);
    } else {
        display.setFont(Open_Sans_Regular_12);  // set a default if an unknown font size is set
        Serial.println("Default font size set to 12");
    }
    if (rows == 1) {
        display.drawString(col1, 24, text1);
    } else if (rows == 2) {
        display.drawString(col1, 0, text1);
        display.drawString(col2, 31, text2);
    } else if (rows == 3) {   // normally fontsize should =16
        display.drawString(col1, 0, text1);
        display.drawString(col2, 21, text2);
        display.drawString(col3, 42, text3);
    } else if (rows == 4) {
        display.drawString(col1, 0, text1);
        display.drawString(col2, 16, text2);
        display.drawString(col3, 32, text3);
        display.drawString(col3, 48, text4);
    }
    display.display();
}

void oledclear() {
    SSD1306Wire display(0x3c, 21, 22);
    display.init();
    display.clear();
    display.displayOff();

}
