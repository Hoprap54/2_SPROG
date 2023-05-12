#include "gcode_interpreter.h"

// gcode memory positions ranked in order of most used
double move_gcodes[] = {0, 1, 2, 3, 28, 30};
const uint8_t n_move_gcodes = 6;

const uint8_t n_gcode_info = 9;
char gcode_info_index[] = {'X', 'Y', 'Z', 'I', 'J', 'K', 'R', 'F', 'S'};
double gcode_info_value[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

double pos_last_value[] = {0, 0, 0};
double pos_delta_value[] = {0, 0, 0};

unsigned int last_gcode = 0;

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
  Serial.println(temp);
  return strtod(temp, NULL);
}

// Gather all information variables available in array of size
void gather_info(char *array, uint8_t size)
{
  for (uint8_t i = 0; i < n_gcode_info; i++)
  {
    if (has_letter(gcode_info_index[i], array, size))
    {
      gcode_info_value[i] = extract_number(i + 1, array, size);
      Serial.print(gcode_info_value[i]);
      Serial.print(" ");
    }
  }
  Serial.println("- Extracted info");
  // Calculate deltas for position and save current value as last value
  for (uint8_t i = 0; i < 3; i++)
  {
    pos_delta_value[i] = pos_last_value[i] - gcode_info_value[i];
    Serial.print(pos_last_value[i]);
    Serial.print(" ");
    pos_last_value[i] = gcode_info_value[i];
  }
  Serial.println("- Last pos");
  for (uint8_t i = 0; i < 3; i++)
  {
    Serial.print(pos_delta_value[i]);
    Serial.print(" ");
  }
  Serial.println("- Delta pos");
}

// Swapping function using pointers
void swap(uint8_t *a, uint8_t *b)
{
  uint8_t mem = *a; // Assigns 'mem' to the value at address 'a'
  *a = *b;          // Assigns the value at address 'a' to the value at address 'b'
  *b = mem;         // Assigns the value at address 'b' to the value of 'mem'
}

void g_codes_exec(char *array, uint8_t size)
{
  uint8_t gcodes[5]; // Array for storing multiple gcodes (this happens in NX cam, but always 1 operation code and then 1+ setup codes)
  uint8_t n = 0;     // Number of gcodes collected in array in function underneath

  // Check if G command is present
  if (has_letter('G', array, size))
  {
    // Gather all g-codes in instruction in array gcodes[]
    for (uint8_t i = 0; i < size; i++)
    {
      if (*(array + i) == 'G')
      {
        gcodes[n] = (int)extract_number(i + 1, array, size);
        n++;
      }
    }
    // Detect movement gcodes. If found swap to back - there will always only be 1 movement gcode in a line
    for (uint8_t i = 0; i < n; i++)
    {
      for (uint8_t j = 0; i < n_move_gcodes; j++)
      {
        if (gcodes[i] == move_gcodes[j])
        {
          swap(&gcodes[i], &gcodes[n]);
          break;
        }
      }
    }
    last_gcode = gcodes[n];
  }
  else
  { // If not present, load gcode from previous instruction into array
    gcodes[0] = last_gcode;
  }

  // Execute all commands in gcodes queue
  for (uint8_t i = n; i < n; i++)
  {
    switch (gcodes[i])
    {
    case 0: // Rapid repositioning

      break;
    case 1: // Linear interpolation
      Serial.println();
      break;

    default:
      Serial.print("Code is not implemented! (");
      Serial.println(gcodes[i]);
    }
  }
}

void m_codes_exec(char *ins, uint8_t size)
{
}
