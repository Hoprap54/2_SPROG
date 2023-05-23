/*
Connections
// SD   -> Arduino Nano //
   CS   -> SS   (D10/PB2)
   MOSI -> MOSI (D11/PB3)
   MISO -> MISO (D12/PB4)
   SCK  -> SCK  (D13/PB5)
*/

#include "SD_control.h"

File myFile;

// Start the SD card
void SD_start(unsigned int CS){
  // Open serial communications - necessary for SPI
  //Serial.begin(9600);
  //while(!Serial){}

  //Serial.print("Initializing SD card...");
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  DDRB |= 0b00000100; // Set SS PIN as output mode
  
  // Initialize SD card
  if(!SD.begin(CS)) {
    //Serial.println("initialization failed!");
    return;
  }
  //Serial.println("initialization done.");
}

uint8_t file_ready(){
  if(myFile.available()){
    return 1;
  }
  return 0;
}

uint8_t file_open(char *name){
  myFile = SD.open(name);
  if(myFile){
    //Serial.println(name);
    return 1;
  }
  // if the file didn't open, print an error:
  //Serial.print("Error opening ");
  //Serial.println(name);
  return 0;
}

void file_close(){
  // Close the file when finished
  myFile.close();
  //Serial.println("- Code completed");
}

// Reads a line in file and replaces values in array by given pointer
uint8_t file_read_ins(char *array){
  uint8_t i = 0; // Count for letter number in line
  char temp = 0; // Temporary read character holder
  while (myFile.available()){ // While there is stuff available in file
    temp = myFile.read();
    if(temp == 10){ // Skip LINE FEED
      continue;
    }
    else if(temp == '('){ // Exceptions: NX comment = starts with '('
      while(myFile.read() != '\r'){} // Set it to start at the next line by making the file handler run through every letter in line until CARRIAGE RETURN character
      return 0; // Failure - due to exception
    }
    else if(temp == '\r' || temp == 0){ // If CARRIAGE RETURN or NULL character
      *(array + i) = '\r';
      return 1; // Succesfully read full instruction
    }
    *(array + i) = temp; // Assign character to array after check
    i++; // If no stop conditions move to next character
  }
  *(array + i) = '\r';
  return 1; // Succes - file end, no CARRIAGE RETURN character
}

// Determines the size of a read line, but is dependent on a CARRIAGE RETURN character being preserved at the end of the line, otherwise it does not know that the line has ended and it will go outside the bounds of the array
uint8_t line_size(char *array){
  uint8_t size = 0;
  while(*(array + size) != '\r'){ // While not equal to CARRIAGE RETURN character
    size++;
  }
  *(array + size) = ' '; // Replace CARRIAGE RETURN with ' '
  size++;
  return size;
}