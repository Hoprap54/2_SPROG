#include "instruction_handler.h"
#include "usart_comm.h"

#define BAUD 9600

// Main function
void setup()
{
  //Serial.begin(BAUD);
  usart_init(BAUD);
  
  while(1){
    char instruction[75] = "";
    uint8_t ins_size = usart_receive_string(instruction); // Get instruction and get size
    //ins_exec(instruction, ins_size); // Execute instruction
    usart_send_char('n'); // Send confirmation
  }
}

void loop()
{
  // Keep empty - arduino loop function thingy
}
