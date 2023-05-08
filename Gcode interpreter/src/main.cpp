/*
Connections
// SD   -> Arduino Nano //
   CS   -> SS   (D10/PB2)
   MOSI -> MOSI (D11/PB3)
   MISO -> MISO (D12/PB4)
   SCK  -> SCK  (D13/PB5)
*/

#include <stdio.h>
#include <avr/io.h>

#include <SPI.h>

#include "SD_control.h"

// chipSelect = SS digital pin number
#define chipSelect 10

// Global variables
// gcode memory positions ranked in order of most used
double move_gcodes[]      = { 0 , 1 , 2 , 3 , 28 , 30 };
const uint8_t n_move_gcodes = 6;

const uint8_t n_gcode_info = 9;
char   gcode_info_pos[]    = {'X','Y','Z','I','J','K','R','F','S'};
double gcode_info_value[]  = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };

double pos_last_value[]    = { 0 , 0 , 0 };
double pos_delta_value[]   = { 0 , 0 , 0 };

unsigned int last_gcode = 0;


// Function prototypes
void gather_info(char *, uint8_t);
void g_codes_exec(char *, uint8_t);
void m_codes_exec(char *, uint8_t);
uint8_t has_letter(char, char *, uint8_t);
double extract_number(uint8_t, char *, uint8_t);
// void bubble_sort(int *, uint8_t);
void swap(uint8_t *, uint8_t *);


// Main function
void setup()
{
  char instruction[75] = "";
  uint8_t ins_size = 0;

  File myFile;
  char name[20] = "lines.txt";
  

  // Start SD card
  SD_start(chipSelect);
  
  // Open file
  myFile = SD.open(name);
  // If file is opened succesfully, start executing file instructions
  if(myFile){
    Serial.println(name);

    // Read from the file if there is gcode available
    while(myFile.available()){
      
      ins_size = file_read_line(myFile, instruction);
      Serial.println(instruction);

      // Gather all information variables available
      for(uint8_t i = 0; i < n_gcode_info; i++){
        if(has_letter(gcode_info_pos[i], instruction, ins_size)){
          gcode_info_value[i] = extract_number(i+1, instruction, ins_size);
        }
      }
      // Calculate deltas and remember last position
      for(uint8_t i = 0; i < 3; i++){
        pos_delta_value[i] = pos_last_value[i] - gcode_info_value[i];
      }
      // Execute m codes available
      m_codes_exec(instruction, ins_size);
      // Execute g codes available
      g_codes_exec(instruction, ins_size);
    }
    // close the file when finished
    myFile.close();
    Serial.println("Code completed");
  } else {
  	// if the file didn't open, print an error:
    Serial.print("Error opening ");
    Serial.println(name);
  }
}


void loop()
{
	// Keep empty - arduino loop function thingy
}


void g_codes_exec(char *ins, uint8_t size){
  uint8_t gcodes[5]; // Array for storing multiple gcodes (this happens in NX cam, but always 1 operation code and then 1+ setup codes)
  uint8_t n = 0; // Number of gcodes collected in array in function underneath
  
  // Check if G command is present
  if(has_letter('G', ins, size)){
    // Gather all g-codes in instruction in array gcodes
    for(uint8_t i = 0; i < size; i++){
      if(*(ins + i) == 'G'){
        gcodes[n] = (int)extract_number(i+1, ins, size);
        n++;
      }
    }
    // Detect movement gcodes. If found swap to back - there will always only be 1 movement gcode in a line
    for(uint8_t i = 0; i < n; i++){
      for(uint8_t j = 0; i < n_move_gcodes; j++){
        if(gcodes[i] == move_gcodes[j]){
          swap(&gcodes[i], &gcodes[n]);
          break;
        }
      }
    }
    last_gcode = gcodes[n];
  }
  else{ // If not present, load gcode from previous instruction into array
    gcodes[0] = last_gcode;
  }

  // Execute all commands in gcodes queue
  for(uint8_t i = n; i < n; i++){
    switch(gcodes[i]){
      case 0: // Rapid repositioning

        break;
      case 1: // Linear interpolation
        
        break;

      default:
        Serial.print("Code is not implemented! (");
        Serial.println(gcodes[i]);
    }
  }
}

void m_codes_exec(char *ins, uint8_t size){

}

uint8_t has_letter(char key, char *array, uint8_t size){
  for(uint8_t i = 0; i < size; i++){
    if(*(array + i) == key){
      return i;
    }
  }
  return 0;
}

// Extract 
double extract_number(uint8_t pos, char *array, uint8_t size){
  uint8_t k = 0;
  char temp[10] = "";
  while(pos < size && k < 10){
    if(*(array + pos) == ' '){
      break;
    }
    temp[k] = array[pos];
    k++;
    pos++;
  }
  return strtod(temp, NULL);
}

/*
// Bubble sort lowest to highest
void bubble_sort(int *array, uint8_t size){
  // Bubble sort loop
  // Variable 'swaps' to keep track of the number of swaps performed each loop
  uint8_t swaps = 0;

  do{
    swaps = 0; // Set swaps to 0 in the beginning of each swapping cycle of the sort
    // For loop which starts from i=1 and runs as long as i<size (10). i increments by 1 each loop
    for(int i = 1; i < size; i++){
      // If the number in array element i-1 is larger than the array element at i, then:
      if(*(array+i-1) > *(array+i)){
        swap((array+i-1), (array+i-1)); // Swap the numbers in the array
        swaps++; // Incremenet swaps by 1
      }
    }
  // If the number of swaps performed this cycle was not 0, then there might still be more to sort, so a new cycle starts
  }while(swaps != 0);
}
*/

// Swapping function using pointers
void swap(uint8_t *a, uint8_t *b){
  uint8_t mem = *a; // Assigns 'mem' to the value at address 'a'
  *a = *b; // Assigns the value at address 'a' to the value at address 'b'
  *b = mem; // Assigns the value at address 'b' to the value of 'mem'
}