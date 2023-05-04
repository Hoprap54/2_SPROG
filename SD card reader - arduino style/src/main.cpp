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

File myFile;
// chipSelect = SS digital pin number
const int chipSelect = 10;
const char name[20] = "THECUBE.txt";

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);


  Serial.print("Initializing SD card...");
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  DDRB |= 0b00000100;
  
  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  // Open file for reading
  myFile = SD.open(name);
  if (myFile) {
    Serial.println(name);
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
    	Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(name);
  }
}

void loop()
{
	// nothing happens after setup
}