#include "instruction_handler.h"

// gcode memory positions ranked in order of most used
const uint8_t move_gcodes[] = {0, 1, 2, 3, 28, 30};
const uint8_t n_move_gcodes = 6;

const uint8_t n_gcode_info = 9;
const char gcode_info_index[] = {'X', 'Y', 'Z', 'I', 'J', 'K', 'R', 'F', 'S'};
double gcode_info_value[]     = { 0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 };

double pos_last_value[]       = { 0 ,  0 ,  0 };
double pos_delta_value[]      = { 0 ,  0 ,  0 };

uint8_t last_gcode = 0;

// Functions //
// Does inputted array have letter '[key]'?
uint8_t has_letter(char key, char *array, uint8_t size)
{
  for (uint8_t i = 0; i < size; i++)
  {
    if (*(array + i) == key)
    {
      return i;
    }
  }
  return 0;
}

// Extract number from string at start pos in array of size until space (' ')
double extract_number(uint8_t pos, char *array, uint8_t size)
{
  uint8_t k = 0;
  char temp[10] = "";
  while (pos < size && k < 10)
  {
    if (*(array + pos) == ' ')
    {
      break;
    }
    temp[k] = *(array + pos);
    k++;
    pos++;
  }
  return strtod(temp, NULL);
}

// Gather all information variables available in array of size
void gather_info(char *array, uint8_t size)
{
  uint8_t pos = 0;
  for (uint8_t i = 0; i < n_gcode_info; i++)
  {
    pos = has_letter(gcode_info_index[i], array, size);
    if (pos)
    {
      gcode_info_value[i] = extract_number(pos + 1, array, size);
      //Serial.print(gcode_info_value[i]);
      //Serial.print(" ");
    }
  }
  //Serial.println("- Extracted info");
  // Calculate deltas for position and save current value as last value
  for (uint8_t i = 0; i < 3; i++)
  {
    pos_delta_value[i] = gcode_info_value[i] - pos_last_value[i];
    //Serial.print(pos_delta_value[i]);
    //Serial.print(" ");
    pos_last_value[i] = gcode_info_value[i];
  }
  //Serial.println(" ");
}

// Swapping function using pointers
void swap(uint8_t *a, uint8_t *b)
{
  uint8_t mem = *a; // Assigns 'mem' to the value at address 'a'
  *a = *b;          // Assigns the value at address 'a' to the value at address 'b'
  *b = mem;         // Assigns the value at address 'b' to the value of 'mem'
}

// Detect all gcodes and detect them
void g_codes_exec(char *array, uint8_t size)
{
  uint8_t gcodes[] = {255,255,255,255,255}; // Array for storing multiple gcodes (this happens in NX cam, but always 1 operation code and then 1+ setup codes)
  uint8_t n = 0;     // Number of gcodes collected in array in function underneath

  

  /*
  for(uint8_t i = 0; i < 5; i++){
    Serial.print(gcodes[i]);
    Serial.print(" ");
  }
  */
  Serial.println(" ");

  // Execute all commands in gcodes queue
  for (uint8_t i = 0; i < n; i++)
  {
    Serial.print(gcodes[i]);
    switch (gcodes[i])
    {
    case 0: // Rapid repositioning
      Serial.println("Rapid repositioning");
      break;
    case 1: // Linear interpolation
      Serial.println("Linear interpolation");
      break;

    default:
      Serial.print("Code is not implemented! (");
      Serial.println(gcodes[i]);
    }
  }
}

void m_codes_exec(char *array, uint8_t size)
{
}
