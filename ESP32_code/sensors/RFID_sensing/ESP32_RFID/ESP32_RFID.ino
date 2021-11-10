// ESP32_RFID.ino  RFID reader example for the ESP32 Maker Kit

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

//  simple 'if' check on the 'accepted' card UIDs in the 'get_UID' function
//   which can be edited to allow different cards

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   32  // uses the servo1 GPIO pin on the Maker Kit PCB
#define SS_PIN    5   // standard SS pin on the 7P SPI header on the PCB

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

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
    Serial.println("PCB pin defaults reset..... ");

    SPI.begin();        // Init SPI bus - assumes default pins for SPI
    mfrc522.PCD_Init(); // Init MFRC522 card reader

    // Prepare the key reader (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
        for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();
    Serial.println("---------------------------");
    Serial.println();
    
}

void loop() {

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));
   

    // Check for compatibility
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("No valid tag"));
        return;
    }
    
    MFRC522::StatusCode status;
   
    String content= "";
    get_UID(content);
    
    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();

}  

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

void get_UID(String content) {
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "E3 26 92 03" 
  or  content.substring(1) == "16 43 27 F8")   {
    Serial.println("This is tag is accepted");
    
  } else {
    Serial.println("Wrong tag");
  }
  Serial.println("---------------------------");
  Serial.println();
}
