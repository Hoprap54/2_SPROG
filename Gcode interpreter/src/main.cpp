/*
Connections
// SD   -> Arduino Nano //
   CS   -> SS   (D10/PB2)
   MOSI -> MOSI (D11/PB3)
   MISO -> MISO (D12/PB4)
   SCK  -> SCK  (D13/PB5)
*/

// HELLOOOOOOOOOO CHRISTIAN

#include <SPI.h>

#include "SD_control.h"
#include "instruction_handler.h"

// chipSelect = SS digital pin number
#define chipSelect 10
#define FileName "lines.txt"

// Main function
void setup()
{
  char name[20] = FileName;
  // Start SD card
  SD_start(chipSelect);

  // Open file
  file_open(name);
  // If file is opened succesfully, start executing file instructions
  while (file_ready())
  {
    ins_exec();
  }
}

void loop()
{
  // Keep empty - arduino loop function thingy
}
