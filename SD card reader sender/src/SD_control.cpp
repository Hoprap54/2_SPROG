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

// Initialize the SD card
void SD_start(unsigned int CS)
{
  DDRB |= 0b00000100; // Set SS PIN as output mode - required for SD card operation

  // Initialize SD card
  if (!SD.begin(CS))
  {
    // Initialization failed
    return;
  }
  // Initialization succesful
}

// Function that returns true (1) if the a file has more data available
uint8_t file_ready()
{
  if (myFile.available())
  {
    return 1;
  }
  return 0;
}

// Function that opens a file named "[name]" from the SD card and returns true (1) if the file is opened succesfully
uint8_t file_open(char *name)
{
  myFile = SD.open(name);
  if (myFile)
  {
    // If file succesfully opened
    return 1;
  }
  // If file did not open succesfully
  return 0;
}

// Closes the currently opened file
void file_close()
{
  // Close the file when finished
  myFile.close();
}

// Reads a line in file and replaces characters in array (string) by instruction character
uint8_t file_read_ins(char *array)
{
  uint8_t i = 0; // Count for letter number in line
  char temp = 0; // Temporary read character holder
  while (myFile.available())
  { // While there is stuff available in file
    temp = myFile.read();
    if (temp == 10)
    { // Skip LINE FEED
      continue;
    }
    else if (temp == '(')
    { // Exceptions: NX comment = starts with '('
      while (myFile.read() != '\r')
      {
      }         // Set it to start at the next line by making the file handler run through every letter in line until CARRIAGE RETURN character
      return 0; // Failure - due to exception
    }
    else if (temp == '\r' || temp == 0)
    { // If CARRIAGE RETURN or NULL character
      *(array + i) = '\r';
      return 1; // Succesfully read full instruction
    }
    *(array + i) = temp; // Assign character to array after check
    i++;                 // If no stop conditions move to next character
  }
  *(array + i + 1) = '\r';
  return 1; // Succes - file end, no CARRIAGE RETURN character
}

// Determines the size of a read line, but is dependent on a CARRIAGE RETURN character being preserved at the end of the line,
// otherwise it does not know that the line has ended and it will go outside the bounds of the array
uint8_t line_size(char *array)
{
  uint8_t size = 0;
  while (*(array + size) != '\r')
  { // While not equal to CARRIAGE RETURN character
    size++; // Increase size
  }
  *(array + size) = ' '; // Replace CARRIAGE RETURN with ' '
  size++;
  return size;
}