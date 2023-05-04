/*
Connections
Board -> Arduino Nano
CS    -> SS   (D10/PB2)
MOSI  -> MOSI (D11/PB3)
MISO  -> MISO (D12/PB4)
SCK   -> SCK  (D13/PB5)
*/

#include <stdio.h>
#include <avr/io.h>

#include <SPI.h>
#include <SD.h>

// Function prototypes
void read_line(File, char *);


// Main function
void setup()
{
  File myFile;
  // chipSelect = SS digital pin number
  const int chipSelect = 10;
  const char name[20] = "lines.txt";
  char instruction[75] = "";

  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.print("Initializing SD card...");
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  DDRB |= 0b00000100; // Set SS PIN as output mode
  
  // Initialize SD card
  if(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  // Open file for reading
  myFile = SD.open(name);
  if(myFile) {
    Serial.println(name);

    while(myFile.available()){
      // Read from the file if there is gcode available
      read_line(myFile, instruction);
      Serial.println(instruction);
      _delay_ms(1000);
    }
    // close the file when finished
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.print("Error opening ");
    Serial.println(name);
  }
}



void loop()
{
	// Keep empty - stupid arduino loop function thingy
}


// Reads a line in file and returns pointer to array
void read_line(File myFile, char *array_ptr){
  uint8_t count = 0; // Count for letter number in line
  while (myFile.available()) { // While there is stuff available in file
    *(array_ptr + count) = myFile.read(); // Set next read character to position in array inputted
    if(*(array_ptr + count) == '\n'){ // If next line character:
      *(array_ptr + count) = 0; // Delete \n character
      return; // Exit function
    }
    count++; // If not go on to next letter
  }
}

void op_type(char *){
  
}

void g_code(){

}

void m_code(){

}

