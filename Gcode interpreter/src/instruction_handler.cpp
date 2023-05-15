#include "instruction_handler.h"
#include "SD_control.h"

// gcode memory positions ranked in order of most used
const uint8_t move_gcodes[] = {0, 1, 2, 3, 28, 30};
const uint8_t n_move_gcodes = 6;


uint8_t last_gcode = 254;

char pos_letters[]    = {'X','Y','Z'};
double pos_target[]   = { 0 , 0 , 0 };
double pos_current[]  = { 0 , 0 , 0 };
double pos_delta[]    = { 0 , 0 , 0 };

char center_letters[] = {'I','J','K'};
double center_info[]  = { 0 , 0 , 0 };
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
    //Serial.print(instruction[i]);
    if (*(array + i) == key)
    {
      return i+1;
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

void get_pos(char *array, uint8_t size){
  uint8_t index = 0;
  for(uint8_t i = 0; i < 3; i++){ // Check through X Y Z
    index = has_letter(pos_letters[i], array, size);
    if(index){ // If X Y OR Z found
      index -= 1;
      pos_target[i] = extract_number(index + 1, array, size); // Get number after letter from instruction
      pos_delta[i] = pos_target[i] - pos_current[i]; // Calculate delta
      pos_current[i] = pos_target[i]; // Set the current position to the target position
    }
  }
}

void get_center(){

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
  uint8_t move_found = 0; // When the 1 movement code has been found

  if(has_letter('G', array, size)){
    for(uint8_t i = 0; i < size; i++){
      if(*(array + i) == 'G'){
        gcodes[n] = (uint8_t)extract_number(i + 1, array, size);
        if(move_found == 0){
          for(uint8_t k = 0; k < n_move_gcodes; k++){
            if(gcodes[n] == move_gcodes[k]){
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
  else{
    gcodes[0] = last_gcode;
    n++;
  }

  get_pos(array, size);

  for(uint8_t i = 0; i < 3; i++){
    Serial.print(pos_target[i]);
    Serial.print(" ");
  }
  Serial.println("- target");

  for(uint8_t i = 0; i < 3; i++){
    Serial.print(pos_current[i]);
    Serial.print(" ");
  }
  Serial.println("- current");

  for(uint8_t i = 0; i < 3; i++){
    Serial.print(pos_delta[i]);
    Serial.print(" ");
  }
  Serial.println("- delta");

  // Execute all commands in gcodes queue
  for (int i = n-1; i >= 0; i--)
  {
    Serial.print(gcodes[i]);
    switch (gcodes[i])
    {
    case 0: // Rapid repositioning
      Serial.println(" Rapid repositioning");
      break;
    case 1: // Linear interpolation
      Serial.println(" Linear interpolation");
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

void m_codes_exec()
{
}

void ins_exec(){
  char instruction[75] = "";
  uint8_t succes = file_read_ins(instruction);
  uint8_t ins_size = line_size(instruction);

  if(succes){
    Serial.print(instruction);
    Serial.print("- ");
    Serial.println(ins_size);

    g_codes_exec(instruction, ins_size);
  }

  Serial.println(" ");
  _delay_ms(500);
}