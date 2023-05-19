#include <SPI.h>

#include "SD_control.h"
#include "instruction_handler.h"

// chipSelect = SS digital pin number
#define chipSelect 10

// Main function
void setup()
{
  char file1[20] = "square.txt";
  char file2[20] = "triangle.txt";
  // Start SD card
  SD_start(chipSelect);

  file_exec(file1);
  _delay_ms(1000);
  file_exec(file2);
}

void loop()
{
  // Keep empty - arduino loop function thingy
}
