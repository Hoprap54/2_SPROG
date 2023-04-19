/*
*   D2 -> B.R.
*   D3 -> B.L.
*   ...
*   D8 -> In1
*   D9 -> In2
*   D10 -> In3
*   D11 -> In4
*
*   Step advance .. m
*/

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

#define b0 0x3b
#define b1 0x37 

int main(void) {     
    uart_init();    // Open the communication to the micro controller
    io_redirect();  // Redirect input and output to the communication

    /* Declaration of I/O Pins */
    DDRD = 0xF0;    // Set Port D as input for the buttons 0b1111 0000
    PORTD = 0x3F;   // Activate internall pull
    DDRB = 0xFF;    // Set Port B as output for the LN298N 0b1111 1111
   
    /* Declare variables */

    while (1){
        
    }
}