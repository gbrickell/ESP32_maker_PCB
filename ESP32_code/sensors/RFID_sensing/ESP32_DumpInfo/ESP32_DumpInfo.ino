/* ESP32_Dumpinfo.ino - reworking of a standard example from the MFRC522 library
 *  more info on the library can be found at https://github.com/miguelbalboa/rfid 
 *  
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 *  
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the ESP32 Maker Kit SPI interface.
 * 
 * When the ESP32 and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
 * So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
 * details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
 * keep the PICCs at reading distance until complete.
 *  
 * ESP32 Maker Kit pin layout used:
 * ---------------------------------
 *             MFRC522      ESP32
 *             Reader/PCD   
 * Signal      Pin          Pin    
 * ---------------------------------
 * RST/Reset   RST          32             
 * SPI SS      SDA(SS)       5            
 * SPI MOSI    MOSI         23 
 * SPI MISO    MISO         19 
 * SPI SCK     SCK          18 
 *
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         32   // uses the servo1 pin on the Maker Kit PCB
#define SS_PIN          5    // uses the standard SS pin on the 7P SPI header on the PCB

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  
	Serial.begin(115200);
  delay(1000); // short delay to set up the monitor
  Serial.println("---------------------------");
  Serial.println();
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
  Serial.println("---------------------------");
  Serial.println();
    
	SPI.begin();			    // Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  Serial.println  ("---------------------------------------------------");
  Serial.println();
 
}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
	mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  Serial.println  ("  -------------------------------------------------------------------------");
  Serial.println();
  
}
