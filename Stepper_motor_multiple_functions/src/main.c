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
#include "motorMovementFunctions.h"
#include <stdbool.h>

#define b0 0b00111110
#define b1 0b00111101
#define b2 0b00111011
#define b3 0b00110111
#define b4 0b00111001
#define b5 0b00110110
#define b6 0b00110000

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
            // move_B_PD();
            // make_step_X(0);
            move_deltas(10, 30);

            break;
        case b1:
            // move_F_PD();
            make_step_X(1);
            // for (int i = 0; i < 200; i++)
            //     make_step_X(1);
            break;
        case b2:
            make_step_Y(1);
            // move_F_PB();
            break;
        case b3:
            make_step_Y(0);
            // move_B_PB();
            break;
        case b4:
            move_same_time_F();
            break;
        case b5:
            move_same_time_B();
            break;
        case b6:
            move_full_circle(30);
            break;
        default:
            PORTD = 0X00; // Set all output to 0
            PORTB = 0X00; // Set all output to 0
            break;
        }
    }
}