// HELLOOOOOOOOOO CHRISTIAN

#include <SPI.h>

#include "SD_control.h"
#include "instruction_handler.h"

// chipSelect = SS digital pin number
#define chipSelect 10

// Main function
void setup()
{
  // Start SD card
  SD_start(chipSelect);

  file_exec("square.txt");
  _delay_ms(1000);
  file_exec("triangle.txt");
}

void loop()
{
  // Keep empty - arduino loop function thingy
}
