#include "SD_control.h"

File myFile;

// Start the SD card
void SD_start(unsigned int CS){
  // Open serial communications - necessary for SPI
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

void file_open(char name[], uint8_t cs){
  myFile = SD.open(name);
}

// Reads a line in file and replaces values in array by given pointer
void file_read_line(File myFile, char *array){
  uint8_t count = 0; // Count for letter number in line
  while (myFile.available()){ // While there is stuff available in file
    *(array + count) = myFile.read(); // Set next read character to position in array inputted
    //Serial.print(*(array + count));
    if(*(array + count) == 13){ // If CARRIAGE RETURN character:
      return;
    }
    count++; // If not go on to next letter
  }
}

// Determines the size of a read line, but is dependent on a CARRIAGE RETURN character being preserved at the end of the line, otherwise it does not know that the line has ended and it will go outside the bounds of the array
uint8_t line_size(char *array){
  uint8_t size = 0;
  while(*(array + size) != 13){
    size++;
  }
  *(array + size) = ' '; // Replace CARRIAGE RETURN with ' '
  return size;
}