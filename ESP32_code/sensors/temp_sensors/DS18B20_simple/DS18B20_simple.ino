/*
 * simple data collection code that uses the core methods used in the 
 * DallasTemperature example from the OneWireNg library, 
 * for use with the ESP32 Maker Kit
 *  
 */

// DS18B20_simple.ino - ESP32 maker kit test code that collects a new temperature 
// reading from a preprepared DS18B20 sensor that has a 10kOhm pull-up resistor 
// between the data line and the 3.3V power supply line, when a button is pressed
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
#include "OneWireNg_CurrentPlatform.h"
#include "drivers/DSTherm.h"
#include "utils/Placeholder.h"

#define OW_PIN   33  // using the servo2 gpio pin on the Maker Kit PCB

int switch_pin = 34; // this is the GPIO pin connected to one side of the bottom button (2)

bool printon = true;  // print control flag

/*
 * Set to true for parasitically powered sensors - not used in this set up 
 */
#define PARASITE_POWER  false


static Placeholder<OneWireNg_CurrentPlatform> _ow;
static Placeholder<DSTherm> _dsth;

// ***********************************************
/* function that returns false if not supported */
// ***********************************************
static bool printId(const OneWireNg::Id& id) {
    const char *name = DSTherm::getFamilyName(id);

    Serial.print("  address code - ");
    Serial.print(id[0], HEX);
    for (size_t i=1; i < sizeof(OneWireNg::Id); i++) {
        Serial.print(':');
        Serial.print(id[i], HEX);
    }
    if (name) {
        Serial.print(" -> ");
        Serial.print(name);
        Serial.print(" device");
    }
    Serial.println();

    return (name != NULL);
}

// ********************************************************
// function to retrieve the readings from the 'scratchpad' 
//  and printing them out to the serial monitor
// ********************************************************
static void printScratchpad(const DSTherm::Scratchpad& scrpd) {
    const uint8_t *scrpd_raw = scrpd.getRaw();

    Serial.print("  scratchpad raw data:");  // print raw data from scratchpad
    for (size_t i = 0; i < DSTherm::Scratchpad::LENGTH; i++) {
        Serial.print(!i ? ' ' : ':');
        Serial.print(scrpd_raw[i], HEX);
    }
    Serial.println();
    
    Serial.print("  HIGH/LOW alarm settings - Th:");
    Serial.print(scrpd.getTh());

    Serial.print("; Tl:");
    Serial.println(scrpd.getTl());

    Serial.print("  sensor resolution: ");
    Serial.println(9 + (int)(scrpd.getResolution() - DSTherm::RES_9_BIT));

    long temp = scrpd.getTemp();
    Serial.print("  temperature reading: ");
    if (temp < 0) {
        temp = -temp;
        Serial.print('-');
    }
    Serial.print(temp / 1000);
    Serial.print('.');
    Serial.print(temp % 1000);
    Serial.print(" C");

    Serial.println();
}


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

    // 
    #ifdef PWR_CTRL_PIN
    # ifndef CONFIG_PWR_CTRL_ENABLED
    #  error "CONFIG_PWR_CTRL_ENABLED needs to be enabled"
    # endif
        new (&_ow) OneWireNg_CurrentPlatform(OW_PIN, PWR_CTRL_PIN, false);
    #else
        new (&_ow) OneWireNg_CurrentPlatform(OW_PIN, false);
    #endif
    DSTherm *dsth = new (&_dsth) DSTherm(_ow);


    #if (CONFIG_MAX_SRCH_FILTERS > 0)
        dsth->filterSupportedSlaves();
    #endif

    /*
     * Uncomment to set common configuration for all sensors connected to
     * the bus:
     * - Th = 0, Tl = 0 (high/low alarm triggers),
     * - Resolution: 12-bits.
     */
    //dsth->writeScratchpadAll(0, 0, DSTherm::RES_12_BIT);

    /*
     * The configuration above is stored in volatile sensors scratchpad
     * memory and will be lost after power unplug. Uncomment this line to
     * store the configuration permanently in sensors EEPROM.
     *
     * NOTE: It will affect all sensors connected to the bus!
     */
    //dsth->copyScratchpadAll(PARASITE_POWER);

    Serial.println(" press button 2 to take a reading .....");
    
}

void loop() {

  if (digitalRead(switch_pin) == LOW) {  // check if button has been pressed
    OneWireNg *ow = &_ow;
    DSTherm *dsth = &_dsth;
    Placeholder<DSTherm::Scratchpad> _scrpd;

    /* convert temperature on all sensors connected... 
       this command issues a Convert T [44h] command to the 
       DS18B20 sensor so that a temperature conversion is performed
       and the thermal data is stored in the scratchpad memory 
       in a 16-bit, sign-extended two’s complement format.   */
    dsth->convertTempAll(DSTherm::SCAN_BUS, PARASITE_POWER);

    /* ...and read them one-by-one by issuing a Read Scratchpad [BEh] command*/
    for (auto id: *ow) {
        if (printId(id)) {  // check a device is present and print details
            if (dsth->readScratchpad(id, &_scrpd) == OneWireNg::EC_SUCCESS)
                printScratchpad(_scrpd);  // use function to print the results to the monitor
            else
                Serial.println("  Invalid CRC!");
        }
    }

    Serial.println("  -------------------------------");
    delay(1000);

  }
    
}
