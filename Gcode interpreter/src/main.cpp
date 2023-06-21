#include "instruction_handler.h"
#include "usart_comm.h"

#include <util/delay.h>
#include <stdio.h>
#include <motorMovementFunctions.h>
#include <calibration.h>
#include <avr/interrupt.h>
#include "usart.h"

#define BAUD 9600

// Main function
void setup()
{
  usart_init(BAUD);

  DDRB |= (1 << PB5);

  // move_deltas(10, 6);

  // move_full_circle(30);

  // for (uint16_t i = 0; i < 32000; i++)
  // {
  //   // move_same_time_one_step(1, 1);
  //   make_step_X(1);
  // }
  limitSwitchSetUp();
  // calibrationX();
  // calibrationY();

  while (1)
  {
    char instruction[75] = "";
    char incomingChar = '1';

    incomingChar = usart_receive_char();
    if (incomingChar >= 'a' && incomingChar <= 'z')
    {
      dPadSignalProcessing(incomingChar);
      // for (int i = 0; i < 1000; i++)
      // {
      //   make_step_X(0);
      // }
    }
    else
    {
      uint8_t ins_size = incomingChar; // Receive instruction size
                                       // if(ins_size > 5){
      PORTB |= (1 << PB5);
      //}
      usart_receive_string(instruction, ins_size); // Get instruction
      // _delay_ms(1000);
      ins_exec(instruction, ins_size); // Execute instruction
      PORTB &= ~(1 << PB5);
      usart_send_char(1); // Send confirmation
    }
  }
}

void loop()
{
  // Keep empty - arduino loop function thingy
}
