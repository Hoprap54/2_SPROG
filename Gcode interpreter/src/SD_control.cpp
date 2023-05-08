#include <stdio.h>
#include <SPI.h>
#include <SD.h>

// Start the SD card
void SD_start(unsigned int CS){
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.print("Initializing SD card...");
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  DDRB |= 0b00000100; // Set SS PIN as output mode
  
  // Initialize SD card
  if(!SD.begin(CS)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

// Reads a line in file and returns pointer to array
uint8_t file_read_line(File myFile, char *array_ptr){
  uint8_t count = 0; // Count for letter number in line
  while (myFile.available()){ // While there is stuff available in file
    *(array_ptr + count) = myFile.read(); // Set next read character to position in array inputted
    count++; // If not go on to next letter
    if(*(array_ptr + count) == '\n'){ // If next line character:
      *(array_ptr + count) = ' '; // Replace \n character with ' ' space for ease of use later
      return count; // Exit function
    }
  }
  *(array_ptr + count) = ' ';
  return count;
}