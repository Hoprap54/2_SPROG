#include "instruction_handler.h"
#include "SD_control.h"
#include "motorMovementFunctions.h"

// gcode memory positions ranked in order of most used
const uint8_t move_gcodes[] = {0, 1, 2, 3, 28, 30};
const uint8_t n_move_gcodes = 6;

uint8_t last_gcode = 254;

char pos_letters[] = {'X', 'Y', 'Z'};
double pos_target[] = {0, 0, 0};
double pos_current[] = {0, 0, 0};
double pos_delta[] = {0, 0, 0};

char center_letters[] = {'I', 'J', 'K'};
double center_target[] = {0, 0, 0};
double R = 0;

// Other info
double F = 0;
double S = 0;
double H = 0;

// Functions //
// Does inputted array have letter '[key]'?
uint8_t has_letter(char key, char *array, uint8_t size)
{
  for (uint8_t i = 0; i < size; i++)
  {
    // Serial.print(instruction[i]);
    if (*(array + i) == key)
    {
      return i + 1; // If yes return position number with counting logic (0 in array = 1)
    }
  }
  return 0; // If no return 0
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

void get_pos_delta(char *array, uint8_t size)
{
  uint8_t index = 0;
  for (uint8_t i = 0; i < 3; i++)
  { // Check through X Y Z
    index = has_letter(pos_letters[i], array, size);
    if (index)
    {                                                     // If X Y OR Z found
      pos_target[i] = extract_number(index, array, size); // Get number after letter from instruction
      pos_delta[i] = pos_target[i] - pos_current[i];      // Calculate delta
    }
  }
}

void set_current_pos()
{
  for (uint8_t i = 0; i < 3; i++)
  {
    pos_current[i] = pos_target[i];
  }
}

void get_center(char *array, uint8_t size)
{
  uint8_t index = 0;
  for (uint8_t i = 0; i < 3; i++)
  { // Check through I J K
    index = has_letter(pos_letters[i], array, size);
    if (index)
    {                                                        // If I J OR K found
      center_target[i] = extract_number(index, array, size); // Get number after letter from instruction
    }
  }
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
  uint8_t gcodes[] = {255, 255, 255, 255, 255}; // Array for storing multiple gcodes (this happens in NX cam, but always 1 operation code and then 1+ setup codes)
  uint8_t n = 0;                                // Number of gcodes collected in array in function underneath
  uint8_t move_found = 0;                       // When the 1 movement code has been found

  if (has_letter('G', array, size))
  {
    for (uint8_t i = 0; i < size; i++)
    {
      if (*(array + i) == 'G')
      {
        gcodes[n] = (uint8_t)extract_number(i + 1, array, size);
        if (move_found == 0)
        {
          for (uint8_t k = 0; k < n_move_gcodes; k++)
          {
            if (gcodes[n] == move_gcodes[k])
            {
              swap(&gcodes[n], &gcodes[0]);
              last_gcode = gcodes[0];
              move_found = 1;
            }
          }
        }
        n++;
      }
    }
  }
  else
  {
    gcodes[0] = last_gcode;
    n++;
  }

  // Execute all commands in gcodes queue
  for (int i = n - 1; i >= 0; i--)
  {
    uint8_t index = 0;

    Serial.print(gcodes[i]);
    switch (gcodes[i])
    {
    case 0: // Rapid repositioning
      Serial.println(" Rapid repositioning");
      get_pos_delta(array, size);

      

      set_current_pos();
      break;

    case 1: // Linear interpolation
      Serial.println(" Linear interpolation");
      get_pos_delta(array, size);
      index = has_letter('F', array, size);
      if (index)
      {
        F = extract_number(index, array, size);
      }

      // Exec move
      move_deltas((int)pos_delta[0], (int)pos_delta[1]);

      set_current_pos();
      break;

    case 2: // CW arc
      Serial.println(" CW arc");
      break;

    case 3: // CCW arc
      Serial.println(" CCW arc");
      break;

    default:
      Serial.println(" Code is not implemented!");
    }
  }
}

void m_codes_exec(char *array, uint8_t size)
{
}

void ins_exec()
{
  char instruction[75] = "";
  uint8_t succes = file_read_ins(instruction);
  uint8_t ins_size = line_size(instruction);

  if (succes)
  {
    Serial.print(instruction);
    Serial.print("- ");
    Serial.println(ins_size);

    g_codes_exec(instruction, ins_size);
  }

  Serial.println(" ");
}

void file_exec(char name[]){
  // Open file
  file_open(name);
  // If file is opened succesfully, start executing file instructions
  while (file_ready())
  {
    ins_exec();
  }
  file_close();
}