// buzzer_player.ino ESP32 maker kit version of buzzer_player.py

//  code developed and compiled using the Arduino IDE v1.8.13
//  ESP32 Dev Module set as the Board in the IDE - using board version 1.0.4
//   even though the board is probably the NodeMCU-32S i.e. 38 pin ESP32
//  Flash Size: 4MB
//  Partition Scheme set to: Default 4MB with spiffs (1.2 MB App / 1.5MB SPIFFS)
//   lots of other Board settings! but none experimented with

#include "SPIFFS.h"

String notes[110] = {};  // initialise the notes string array
int num_notes;
String str_notes;

String twinkle_melody[42] = {}; // initialise the twinkle_melody string array
String twinkle_tempo[42] = {};  // initialise the twinkle_tempo string array
int num_twinkle = 42;

String adventure_melody[44] = {}; // initialise the adventure_melody string array
String adventure_tempo[44] = {};  // initialise the adventure_tempo string array
int num_adventure = 44;

int buzzpin = 25;    // buzzer GPIO pin

void setup() {
  Serial.begin(115200);
  delay(1500); // short delay to set up the monitor

  Serial.print("program starting ..... ");

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

  pinMode(buzzpin, OUTPUT);

  // set up SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Serial.print("SPIFFS total available bytes: ");
  Serial.println(SPIFFS.totalBytes());
  Serial.print("SPIFFS used bytes: ");
  Serial.println(SPIFFS.usedBytes());

  // test SPIFFS with reference/test file
  File file = SPIFFS.open("/test.txt");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("Test/reference file content:");
  while(file.available()){
  Serial.write(file.read());
  }
  file.close();

  // **************************************************************
  // read all the default setting data from the SPIFFS text files   
  // **************************************************************
  str_notes = read_text("/numnotes.txt");


  // ********************************************************************
  // convert the integer and float strings back into integers and floats
  // ********************************************************************
  num_notes = str_notes.toInt();


  // read notes from file
  File notesfile = SPIFFS.open("/notes.txt");
  if(!notesfile){
    Serial.println("Failed to open notesfile for reading");
    return;
  }
  read_strings("/notes.txt", notes, num_notes);
  Serial.println("-------------------------------");
  Serial.println("reprint read notes");
  for (int i=0; i<=num_notes-1; i++){
    Serial.print(i);
    Serial.print(" - ");
    Serial.print(notes[i].length());
    Serial.print(" - ");
    Serial.print(notes[i]);
    Serial.print(" - ");
    Serial.print(notes[i].substring(0,3)); // 1st # is inclusive - 2nd # is exclusive
    Serial.print(":");
    Serial.println(notes[i].substring(5)); // omitting the 2nd # shows to the end of the string
  }

  // load song melodies and tempos from SPIFFS
  read_strings("/twinkle_twinkle_melody.txt", twinkle_melody, num_twinkle);
  read_strings("/twinkle_twinkle_tempo.txt", twinkle_tempo, num_twinkle);

  read_strings("/adventure_time_melody.txt", adventure_melody, num_adventure);
  read_strings("/adventure_time_tempo.txt", adventure_tempo, num_adventure);

  Serial.println ("Song melodies and tempos loaded from SPIFFS");

  Serial.println ("  ");
  Serial.println ("********************************************");
  Serial.println ("Playing Twinkle, Twinkle, Little Star Melody");
  Serial.println ("********************************************");
  //play(notes, num_twinkle, num_notes, twinkle_melody, twinkle_tempo, 0.50, 1.500);
  play(notes, num_twinkle, num_notes, twinkle_melody, twinkle_tempo, 0.50, 1.000);

  Serial.println ("  ");
  Serial.println ("********************************************");
  Serial.println ("Playing Adventure Time Melody");
  Serial.println ("********************************************");
  //play(notes, num_adventure, num_notes, adventure_melody, adventure_tempo, 1.3, 1.500);
  play(notes, num_adventure, num_notes, adventure_melody, adventure_tempo, 1.3, 1.500);

}


void loop() {

    // put your main code here, to run repeatedly:

}





// ********** Functions *******************

// **********************************************************
// Function to read a single string from a written text file
// **********************************************************
String read_text(String rfile) {
    int i;
    String str_read;
    //open the file for reading
    File f = SPIFFS.open(rfile, "r");
  
    if (!f) {
        str_read = "file open failed";
        Serial.println(str_read);
    }
    else
    {
        Serial.print("Reading Text Data from File: ");
        //read string from file
        str_read = f.readStringUntil('\n');
        if (rfile.substring(1,5) == "pass") {
            Serial.println("password not shown");
        } else {
            Serial.println(str_read);
        }
        f.close();  //Close file
        Serial.println("File Closed");
    }
    return str_read;

}


// *****************************************************************************************
// Function to read an existing multiple string file and extract the strings from it
// the local variables: 'rfile' is passed the file name, 'strarray' the string array
//  'pointed' to by reference and int num is the number of string array elements to be read
// *****************************************************************************************
void read_strings(String rfile, String strarray[], int num) {
    String tempstr;
    //r=Read Open file for reading
    File SPIfileStr = SPIFFS.open(rfile, "r");
  
    if (!SPIfileStr) {
        Serial.println("multiple string file open failed");
        strarray[0] = "multiple string file open failed";

    }
    else
    {
        //Read string data from file looking for the LF's that separate each string
        Serial.println("  ");
        Serial.println("Function read_strings: reading 'num' strings from file");
        for (int i=0; i<=num-1; i++){                     // loop thru the 'num' strings in the file
            tempstr=SPIfileStr.readStringUntil('\n'); // [i] string read upto the \n terminator
                               //  the terminator is discarded from the stream but adds a space
            // hmm looks like ESP32 SPIFFS doesn't need the last character removed so comment out for now
            //if (tempstr.length() > 2) {
            //    tempstr.remove(tempstr.length()-1,1);
            //}
            strarray[i] = tempstr;
            // debug only display of each read item
            //Serial.print(i);
            //Serial.print(": ");
            //Serial.println(strarray[i]);
        }
        SPIfileStr.close();  //Close file
        Serial.print("Function read_strings: string reading complete for file: ");
        Serial.println(rfile);

    }
}


// *************************************************************************
// Function to 'sound' the passive buzzer with a specific frequency/duration
// *************************************************************************
int buzz(int frequency, int duration) {   
    // create the function "buzz" and feed it the note (e.g. DS6=1245Hz) and duration (length in ms))
    //Serial.print("Buzzing: pin ");
    //Serial.println(buzzpin);
    Serial.print("Buzzing: frequency ");   // pitch/frequency of the note
    Serial.println(frequency);
    Serial.print("Buzzing: length (ms) "); // length/duration of the note in ms
    Serial.println(duration);
    if (frequency == 0) {
        delay(duration);
        int buzzstat = 0;
        return buzzstat;
    }
    // from the frequency calculate the time between buzzer pin HIGH-LOW setting in microseconds
    //float period = 1.0 / frequency;       // in physics, the period (sec/cyc) is the inverse of the frequency (cyc/sec)
    int delayValue = (1000*1000/frequency)/2;  // calculate the time in microseconds for half of the wave
    int numCycles = int((duration * frequency)/1000);   // the number of waves to produce is the duration times the frequency
    Serial.print("Number of cycles: ");
    Serial.println(numCycles);
    Serial.print("Hi-Low delay (microsecs): ");
    Serial.println(delayValue);
    for (int i=0; i<=numCycles-1; i++) {  // start a loop from 0 to the variable "cycles" calculated above
        digitalWrite(buzzpin, HIGH);      // set buzzer pin to high
        delayMicroseconds(delayValue);    // wait with buzzer pin high
        digitalWrite(buzzpin, LOW);       // set buzzer pin to low
        delayMicroseconds(delayValue);    // wait with buzzer pin low
    }

}



// ***********************************************************
// Function to 'play' a defined melody at its associated tempo
// ***********************************************************
void play(String allnotes[], int songnotes, int numnotes, String strmelody[], String strtempo[], float pause, float pace) { 
    // allnotes[]  - array of all the standard notes and their frequency
    // songnotes   - number of notes in the song
    // numnotes    - number of possible standard notes
    // strmelody[] - array of notes in the song
    // strtempo[]  - array of note lengths i.e. 1, 2, 4, 8 etc., where 8=quaver 4=semiquaver etc.
    // pause       - multiplier of the note duration as a pause between notes
    // pace        - used to calculate the note duration = pace/tempo
    float selfreq;
    Serial.print("Playing ");
    Serial.print(songnotes);
    Serial.println(" song notes");
    for (int i=0; i<=songnotes-1; i++) {        // Play strmelody[] song notes
      // loop thru all the possible notes to look for the associated frequency for the note to be played
      Serial.print("Playing song note: ");
      Serial.println(strmelody[i]);
      for (int j=0; j<=numnotes-1; j++) {
        String selectnote;
        if (strmelody[i].length() == 2) {
            selectnote = allnotes[j].substring(0,2);
        } else if (strmelody[i].length() == 3) {
            selectnote = allnotes[j].substring(0,3);
        } else {
            Serial.print(" funny number of characters in melody note"); 
        }
        //Serial.print("Is it this note: ");
        //Serial.println(selectnote);
        if (strmelody[i] == selectnote or strmelody[i] == "0") {
          // if here playnote found or freq = 0 - so use its frequency and play the note
          String tempo = strtempo[i];
          int noteDuration = 1000*pace/tempo.toInt();  // length or duration of the note in milliseconds
          if ( strmelody[i] == "0" ) {
            selfreq = 0.0;
            Serial.println("frequency set to zero");
          } else {
            String selfreqstr = allnotes[j].substring(5);
            Serial.print("associated frequency found: ");
            Serial.println(selfreqstr);
            selfreq = selfreqstr.toFloat();
            Serial.print("Playing frequency: ");
            Serial.println(selfreqstr);
          }
          int buzzstat = buzz(selfreq, noteDuration);      // Change the frequency along the song note  
          float pauseBetweenNotes = noteDuration * pause;
          Serial.print("pause betwen notes (ms): ");
          Serial.println(pauseBetweenNotes);
          delay(pauseBetweenNotes);          
          // now break out of the 'note finding' j for loop
          break;
        }
      }

      // now continue to loop through all the song notes
    }
}
