#include "instruction_handler.h"
#include "usart_comm.h"

#include <util/delay.h>
#include <stdio.h>

#define BAUD 9600

// Main function
void setup()
{
  usart_init(BAUD);

  DDRB |= (1 << PB5);

  while (1)
  {
    char instruction[75] = "";

    uint8_t ins_size = usart_receive_char(); // Receive instruction size
                                             // if(ins_size > 5){
    PORTB |= (1 << PB5);
    //}
    usart_receive_string(instruction, ins_size); // Get instruction
    // _delay_ms(1000);
    ins_exec(instruction, ins_size); // Execute instruction
    PORTB &= ~(1 << PB5);
    usart_send_char(1);                      // Send confirmation
  }
}

void loop()
{
  // Keep empty - arduino loop function thingy
}
