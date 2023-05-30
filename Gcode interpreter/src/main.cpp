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
  // usart_init(BAUD);

  DDRB |= (1 << PB5);

  // move_deltas(10, 6);

  // move_full_circle(30);

  // limitSwitchSetUp();

  for (int i = 0; i < 20000; i++)
  {
    // make_step_Y(1);
    make_step_X(1);
  }

  // calibrationX();
  // calibrationY();

  // while (1)
  // {
  //   char instruction[75] = "";

  //   uint8_t ins_size = usart_receive_char(); // Receive instruction size
  //                                            // if(ins_size > 5){
  //   // PORTB |= (1 << PB5);
  //   //}
  //   usart_receive_string(instruction, ins_size); // Get instruction
  //   // _delay_ms(1000);
  //   ins_exec(instruction, ins_size); // Execute instruction
  //   // PORTB &= ~(1 << PB5);
  //   usart_send_char(1); // Send confirmation
  // }
}

void loop()
{
  // Keep empty - arduino loop function thingy
}
