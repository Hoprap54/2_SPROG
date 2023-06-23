#include "instruction_handler.h"
#include "usart_comm.h"

#include <util/delay.h>
#include <stdio.h>
#include <motorMovementFunctions.h>
#include <calibration.h>
#include <avr/interrupt.h>
#include "usart.h"

#define BAUD 9600

volatile char incomingChar = 's';

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

  UCSR0B |= (1 << RXCIE0); // USART RX interrupt enable

  while (1)
  {

    while(incomingChar >= 'a' && incomingChar <= 'z')
    {
      dPadSignalProcessing(incomingChar);
      // for (int i = 0; i < 1000; i++)
      // {
      //   make_step_X(0);
      // }
    }

    calibrationX();
    calibrationY();
  
    while(uint8_t ins_size = usart_receive_char()){
      char instruction[75] = "";
      
      usart_receive_string(instruction, ins_size); // Get instruction
      ins_exec(instruction, ins_size); // Execute instruction
      usart_send_char(1); // Send confirmation
    }
  
  }
}

void loop()
{
  // Keep empty - arduino loop function thingy
}

ISR(USART_RX_vect){
  incomingChar = usart_receive_char();
  if(incomingChar < 'a' || incomingChar > 'z'){ // If not dpad control code
    UCSR0B &= ~(1 << RXCIE0); // USART RX interrupt disable - start reading gcode
  }
}