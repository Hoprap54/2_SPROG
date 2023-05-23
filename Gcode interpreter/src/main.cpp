#include "instruction_handler.h"
#include "usart_comm.h"

#include <util/delay.h>
#include <stdio.h>

#define BAUD 9600

// Main function
void setup()
{
  usart_init(BAUD);

  while (1)
  {
    char instruction[75] = "";

    uint8_t ins_size = usart_receive_char();     // Receive instruction size
    usart_receive_string(instruction, ins_size); // Get instruction
    if (ins_size > 0)
    {
      DDRB |= 1 << PB5;
      PORTB |= 1 << PB5;
      ins_exec(instruction, ins_size); // Execute instruction
      usart_send_char('n');            // Send confirmation
    }
  }
}

void loop()
{
  // Keep empty - arduino loop function thingy
}
