/*
 * Buttons
 *   PC0 -> B0
 *   PC1 -> B1
 *   PC2 -> B2
 * M1
 *   PD4 -> In1
 *   PD5 -> In2
 *   PD6 -> In3
 *   PD7 -> In4
 * M2
 *   PB0 -> In1
 *   PB1 -> In2
 *   PB2 -> In3
 *   PB3 -> In4
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"
#include "subfunctions.h"

#define b0 0b00111110
#define b1 0b00111101
#define b2 0b00111011
#define b3 0b00110111
#define b4 0b00111001
#define b5 0b00110110

int main(void)
{
    uart_init();   // Open the communication to the micro controller
    io_redirect(); // Redirect input and output to the communication

    /* Declaration of I/O Pins */
    // M1
    DDRD = 0xFF; // Output for M1

    // M2
    DDRB = 0xFF; // Output for M2

    // Buttons
    DDRC = 0x00;  // Inputs for buttons
    PORTC = 0x3F; // Activate pullups

    unsigned int mm_data = 0;

    while (1)
    {
        switch (PINC)
        { // Read buttons
        case b0:
            move_B_PD();
            break;
        case b1:
            move_F_PD();
            break;
        case b2:
            move_F_PB();
            break;
        case b3:
            move_B_PB();
            break;
        case b4:
            move_same_time_F();
            break;
        case b5:
            move_same_time_B();
            break;
        default:
            PORTD = 0X00; // Set all output to 0
            PORTB = 0X00; // Set all output to 0
            break;
        }
    }
}