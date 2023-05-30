/*
 * Limit switch
 *   PD2 -> LL1 & LR1
 *   PD3 -> LL2 & LR2
 * Buttons
 *   PD2 ->
 *   PD3 ->
 *   PD4 ->
 *   PD5 ->
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
#include <avr/interrupt.h>

#define b0 0b00111110
#define b1 0b00111101
#define b2 0b00111011
#define b3 0b00110111
#define b4 0b00111001
#define b5 0b00110110
#define b6 0b00110000

int main(void)
{
    /* Declaration of I/O Pins */
    // M1
    // M2
    // DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5) | (1 << DDC6) | (1 << 7);
    // DDRC = 0xFF;
    // PORTC = 0x00;
    DDRD |= (1 << DD4) | (1 << DD5) | (1 << DD6) | (1 << DD7);
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3);

    // Buttons
    DDRC = 0x00;  // Inputs for buttons
    PORTC = 0x3F; // Activate pullups

    // move_deltas(71.0, 21.0);
    move_deltas(80.0, 1.0);

    // while (1){
    //     switch (PINC){ // Read buttons
    //         case b0:
    //             move_B_PB();
    //             break;
    //         case b1:
    //             move_F_PB();
    //             break;
    //         case b2:
    //             move_F_PD();
    //             break;
    //         case b3:
    //             move_B_PD();
    //             break;
    //         case b4:
    //             move_step_L();
    //             break;
    //         case b5:
    //             move_step_R();
    //             break;
    //         case b6:
    //             break;
    //         default:
    //             PORTD &= ~((1<<DD7)|(1<<DD6)|(1<<DD5)|(1<<DD4)); // Set all output to 0
    //             PORTB = 0X00; // Set all output to 0
    //             break;
    //     }
    // }
}