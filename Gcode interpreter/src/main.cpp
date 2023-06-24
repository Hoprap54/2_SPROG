#include "instruction_handler.h"
#include "usart_comm.h"

#include <util/delay.h>
#include <stdio.h>
#include <motorMovementFunctions.h>
#include <calibration.h>
#include <avr/interrupt.h>
#include "usart.h"

#define BAUD 9600

volatile char incomingChar = 's'; // Variable for storing interrupt usart inputs

// Main function
void setup()
{
  usart_init(BAUD); // Start usart

  DDRB |= (1 << PB5); // Enable output mode on pin PB5

  // Fast tests
  // move_deltas(10, 6);

  // move_full_circle(30);

  // for (uint16_t i = 0; i < 32000; i++)
  // {
  //   // move_same_time_one_step(1, 1);
  //   make_step_X(1);
  // }

  limitSwitchSetUp(); // Setup limit switches

  while (1)
  {
    UCSR0B |= (1 << RXCIE0); // USART RX interrupt enable
    incomingChar = 's'; // Reset incomingChar to 's', so it is inside while loop
    // Loop checks if interrupt USART variable 'incomingChar' is witihin manual control command range (alphabet)
    while (incomingChar >= 'a' && incomingChar <= 'z')
    {
      dPadSignalProcessing(incomingChar);
    }

    // Calibrate to middle of machine when command is outside command spectrum
    calibrationX();
    calibrationY();

    // Send confirmation to SD arduino that gcode can start being sent
    usart_send_char(1);

    // While the ins_size is not 0, execute gcode being recieved
    while (uint8_t ins_size = usart_receive_char())
    { // First receive length of instruction
      char instruction[75] = "";
      // Then
      usart_receive_string(instruction, ins_size); // Get instruction
      ins_exec(instruction, ins_size);             // Execute instruction
      usart_send_char(1);                          // Send confirmation to SD arduino
    }
  }
}

void loop()
{
  // Keep empty - arduino loop function thingy
}

// Interrupt service routine for USART RX input detected
ISR(USART_RX_vect)
{
  incomingChar = usart_receive_char(); // Receive the message
  if (incomingChar < 'a' || incomingChar > 'z')
  {                           // If not dpad control code
    UCSR0B &= ~(1 << RXCIE0); // USART RX interrupt disable - start reading gcode in main while(1) loop
  }
}