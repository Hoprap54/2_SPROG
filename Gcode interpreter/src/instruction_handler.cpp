#include "instruction_handler.h"
#include "motorMovementFunctions.h"

// gcode memory positions ranked in order of most used
const uint8_t move_gcodes[] = {0, 1, 2, 3, 28, 30}; // The different movement gcodes, necessary to find the singular movement gcode in any instruction
const uint8_t n_move_gcodes = 6;                    // Number of different movement gcodes in array above

uint8_t last_gcode = 254; // Needed to save the last movement gcode executed

const char pos_letters[] = {'X', 'Y', 'Z'}; // Letters to identify position
double pos_target[] = {0, 0, 0};            // Target XYZ position
double pos_current[] = {0, 0, 0};           // Current XYZ position
double pos_delta[] = {0, 0, 0};             // XYZ position delta

const char center_letters[] = {'I', 'J', 'K'}; // Letters to identify arc center position
double center_target[] = {0, 0, 0};            // Arc center XYZ position
double R = 0;                                  // Radius

// Other info
double F = 0; // Feedrate
double S = 0; // Spindle speed
double H = 0; // Tool height offset

// Functions //
// Does inputted array have letter '[key]'?
uint8_t has_letter(char key, char *array, uint8_t size)
{
  for (uint8_t i = 0; i < size; i++) // For each index in array check if index is letter
  {
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
  uint8_t k = 0;               // Position in temp array
  char temp[10] = "";          // Number extraction temp array
  while (pos < size && k < 10) // While remaining within the bounds of the temp array and the argument array
  {
    if (*(array + pos) == ' ') // When space occurs, break loop
    {
      break;
    }
    // Store number in temp array
    temp[k] = *(array + pos);
    k++;
    pos++;
  }
  return strtod(temp, NULL); // Return double converted version of array
}

// Get the new target position and calculate the deltas from current position to target position
void get_pos_delta(char *array, uint8_t size)
{
  uint8_t index = 0;
  for (uint8_t i = 0; i < 3; i++)
  {                                                  // Check through X Y Z
    index = has_letter(pos_letters[i], array, size); // If letter found, index = start pos of number after
    if (index)
    {                                                     // If X Y OR Z found
      pos_target[i] = extract_number(index, array, size); // Get number after letter from instruction
      pos_delta[i] = pos_target[i] - pos_current[i];      // Calculate delta
    }
  }
}

// Set the current position to the target position
void set_current_pos()
{
  for (uint8_t i = 0; i < 3; i++)
  {
    pos_current[i] = pos_target[i];
  }
}

// Get arc center coordinates
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

void m_code_exec(uint8_t code, char *array, uint8_t size)
{
}

void g_code_exec(uint8_t code, char *array, uint8_t size)
{
  uint8_t index = 0;

  switch (code) // Switch to do specific gcode function depending on number
  {
  case 0: // Rapid repositioning
    get_pos_delta(array, size);

    // Exec move
    move_deltas(pos_delta[0], pos_delta[1]);

    set_current_pos();
    break;

  case 1:                                 // Linear interpolation
    get_pos_delta(array, size);           // Get position information
    index = has_letter('F', array, size); // Check for possible feedrate
    if (index)                            // If presenet
    {
      F = extract_number(index, array, size); // Extract the number
    }

    // Exec move
    move_deltas(pos_delta[0], pos_delta[1]); // Execute move

    set_current_pos(); // Save target position as current position, as the move has been made
    break;

  case 2: // CW arc
    break;

  case 3: // CCW arc
    break;

  default:
    H++;
  }
}

// Detect all gcodes and execute them. Will execute setup gcodes first, then movement gcodes
void ins_exec(char *array, uint8_t size)
{
  uint8_t gcodes[] = {255, 255, 255, 255, 255}; // Array for storing multiple gcodes (this happens in NX cam, but always 1 movement code and then 1+ setup codes)
  uint8_t n = 0;                                // Number of gcodes collected in array in function underneath
  uint8_t move_found = 0;                       // When the 1 movement code has been found

  if (has_letter('G', array, size)) // If a gcode is even present
  {
    for (uint8_t i = 0; i < size; i++) // Search through instruction
    {
      if (*(array + i) == 'G') // If 'G' found
      {
        uint8_t temp = (uint8_t)extract_number(i + 1, array, size); // Extract gcode number
        if (move_found == 0)                                        // If the movement gcode has not been found yet, check if found code is movement code
        {
          for (uint8_t k = 0; k < n_move_gcodes; k++) // Check with all known movement gcodes
          {
            if (temp == move_gcodes[k]) // If found
            {
              last_gcode = temp; // Set last_gcode to the found movement code
              move_found = 1;    // Set variable to 1, so movement code is not checked for the rest of the gcodes available - there is only 1 movement gcode in a single instruction
              break;
            }
          }
          // After if statement check. If move has been found, within the for loop:
          if (move_found == 0)
          {
            gcodes[n] = temp; // then just assign the current gcode to the setup gcodes array
          }
        }
        // If the move was already found in a previous loop, then just assign the gcode of this loop to setting gcodes array
        else
        {
          gcodes[n] = temp;
        }
        n++;
      }
    }
    // Execute all setting gcodes, if there are any
    for (uint8_t i = 0; i < n; i++)
    {
      g_code_exec(gcodes[i], array, size);
    }
  }

  // Execute m code, if there are any
  /*
  uint8_t index = has_letter('M', array, size);
  if(index){
    uint8_t M = (uint8_t)extract_number(index, array, size);
    m_code_exec(M, array, size);
  }
  */

  // Execute movement gcode
  g_code_exec(last_gcode, array, size);
}

void dPadSignalProcessing(char incomingChar)
{
  switch (incomingChar) // each movement has a specific number 'letter'
  { // Move:
  case 'r': // Right
    make_step_X(1);
    break;
  case 'l': // Left
    make_step_X(0);
    break;
  case 'f': // Forward
    make_step_Y(1);
    break;
  case 'b': // Backward
    make_step_Y(0);
    break;
  case 'c':
    move_same_time_one_step(1, 1);
    break;
  case 'd':
    move_same_time_one_step(1, 0);
    break;
  case 'e':
    move_same_time_one_step(0, 0);
    break;
  case 'g':
    move_same_time_one_step(0, 1);
    break;
  }
}