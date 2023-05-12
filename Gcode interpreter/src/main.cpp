/*
Connections
// SD   -> Arduino Nano //
   CS   -> SS   (D10/PB2)
   MOSI -> MOSI (D11/PB3)
   MISO -> MISO (D12/PB4)
   SCK  -> SCK  (D13/PB5)
*/

#include <SPI.h>

#include "SD_control.h"
#include "gcode_interpreter.h"

// chipSelect = SS digital pin number
#define chipSelect 10
#define FileName "lines.txt"

// Global variables
// Function prototypes

// Main function
void setup()
{
  File myFile;
  
  char name[20] = FileName;

  // Open serial communications - necessary for SPI
  Serial.begin(9600);
  
  // Start SD card
  SD_start(chipSelect);

  // Open file
  myFile = SD.open(name);
  // If file is opened succesfully, start executing file instructions
  if (myFile)
  {
    Serial.println(name);

    // Read from the file if there is gcode available
    while (myFile.available())
    {
      char instruction[75] = "";
      uint8_t ins_size = 0;

      file_read_line(myFile, instruction);
      ins_size = line_size(instruction);
      Serial.print(instruction);
      Serial.print(" - ");
      Serial.println(ins_size);

      // Gather info
      gather_info(instruction, ins_size);
      // Execute m codes available
      // m_codes_exec(instruction, ins_size);
      // Execute g codes available
      // g_codes_exec(instruction, ins_size);

      _delay_ms(500);
    }
    // close the file when finished
    myFile.close();
    Serial.println("- Code completed");
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.print("Error opening ");
    Serial.println(name);
  }
}

void loop()
{
  // Keep empty - arduino loop function thingy
}
