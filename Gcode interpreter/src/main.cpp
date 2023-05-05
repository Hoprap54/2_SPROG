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
#include <SD.h>

// chipSelect = SS digital pin number
#define chipSelect 10

// Global variables
// gcode memory positions ranked in order of most used
char   gcode_info_pos[]   = {'X','Y','Z','I','J','K','R','F','S','T'};
double gcode_info_value[] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
unsigned int last_gcode = 0;


// Function prototypes
void SD_start(unsigned int);
uint8_t file_read_line(File, char *);
void g_code_exec(char [], uint8_t);
void m_code_exec(char [], uint8_t);
uint8_t has_letter(char, char [], uint8_t);
double extract_number(uint8_t, char [], uint8_t);
void bubble_sort(int *, uint8_t);
void swap(int *, int *);


// Main function
void setup()
{
  char instruction[75] = "";
  uint8_t ins_size = 0;

  File myFile;
  const char name[20] = "lines.txt";
  

  // Start SD card
  SD_start(chipSelect);
  
  // Open file
  myFile = SD.open(name);
  // If file is opened succesfully, start executing file instructions
  if(myFile) {
    Serial.println(name);

    // Read from the file if there is gcode available
    while(myFile.available()){
      
      ins_size = file_read_line(myFile, instruction);
      Serial.println(instruction);

      // Gather all information variables available
      for(uint8_t i = 0; i < 10; i++){
        
      }
      m_code_exec(instruction, ins_size);
      g_code_exec(instruction, ins_size);
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


void g_code_exec(char ins[], uint8_t size){
  int gcodes[5] = {-1,-1,-1,-1,-1}; // Array for storing multiple gcodes (this happens in NX cam, but always 1 operation code and then 1+ setup codes)
  uint8_t number = 0;
  // Check if G command is present
  if(has_letter('G', ins, size)){
    // Gather all g-codes in instruction in array gcodes
    for(uint8_t i = 0; i < size; i++){
      if(ins[i] == 'G'){
        gcodes[number] = (int)extract_number(i, ins, size);
        number++;
      }
    }

    // Sort array from highest to lowest to order operations
    bubble_sort(gcodes, 10);
  }
  else{ // If not present, load gcode from previous instruction into array
    gcodes[0] = gcode_info_value[0];
  }

  // Execute all commands in gcodes queue
  for(uint8_t i = 0; i < 5; i++){
    if(gcodes[i] == -1){
      break;
    }

    switch(gcodes[i]){
      case 0: // Rapid repositioning
      case 1: // Linear interpolation

        break;

      default:
        Serial.print("Code is not implemented! (");
        Serial.println(gcodes[i]);
    }
  }
}

void m_code_exec(char ins[], uint8_t size){

}


uint8_t has_letter(char key, char instruction[], uint8_t size){
  for(uint8_t i = 0; i < size; i++){
    if(instruction[i] == key)
      return i;
  }
  return 0;
}

double extract_number(uint8_t pos, char array[], uint8_t size){
  uint8_t k = 0;
  char temp[10] = "";
  while(pos < size && k < 10){
    if(array[pos] != ' '){
      break;
    }
    temp[k] = array[pos];
    k++;
    pos++;
  }
  return strtod(temp, NULL);
}

void bubble_sort(int *array, uint8_t size){
  // Bubble sort loop
  // Variable 'swaps' to keep track of the number of swaps performed each loop
  uint8_t swaps = 0;

  do{
    swaps = 0; // Set swaps to 0 in the beginning of each swapping cycle of the sort
    // For loop which starts from i=1 and runs as long as i<size (10). i increments by 1 each loop
    for(int i = 1; i < size; i++){
      // If the number in array element i-1 is larger than the array element at i, then:
      if(*(array+i-1) < *(array+i)){
        swap((array+i-1), (array+i-1)); // Swap the numbers in the array
        swaps++; // Incremenet swaps by 1
      }
    }
  // If the number of swaps performed this cycle was not 0, then there might still be more to sort, so a new cycle starts
  }while(swaps != 0);
}

// Swapping function using pointers
void swap(int *a, int *b){
  int mem = *a; // Assigns 'mem' to the value at address 'a'
  *a = *b; // Assigns the value at address 'a' to the value at address 'b'
  *b = mem; // Assigns the value at address 'b' to the value of 'mem'
}