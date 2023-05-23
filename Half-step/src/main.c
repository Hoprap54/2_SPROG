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

#define b0 0b00001110
#define b1 0b00001101
#define b2 0b00001011
#define b3 0b00000111
#define b4 0b00001001
#define b5 0b00000110
#define b6 0b00000000

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

    // PORTD |= (1 << DD2) | (1 << DD3) | (1 << DD4) | (1 << DD5) | (1 << DD6) | (1 << DD7);

    // for (int i = 0; i < 200; i++)
    // {
    //     PORTD &= 0x0F;
    //     PORTD |= 0b00010000;
    //     // _delay_us(900);
    //     delay_ms(2);

    //     PORTD &= 0x0F;
    //     PORTD |= 0b01010000;
    //     // _delay_us(900);
    //     delay_ms(2);

    //     PORTD &= 0x0F;
    //     PORTD |= 0b01000000;
    //     // _delay_us(900);
    //     delay_ms(2);

    //     PORTD &= 0x0F;
    //     PORTD |= 0b01100000;
    //     // _delay_us(900);
    //     delay_ms(2);

    //     PORTD &= 0x0F;
    //     PORTD |= 0b00100000;
    //     // _delay_us(900);
    //     delay_ms(2);

    //     PORTD &= 0x0F;
    //     PORTD |= 0b10100000;
    //     // _delay_us(900);
    //     delay_ms(2);

    //     PORTD &= 0x0F;
    //     PORTD |= 0b10000000;
    //     // _delay_us(900);
    //     delay_ms(2);

    //     PORTD &= 0x0F;
    //     PORTD |= 0b10010000;
    //     // _delay_us(900);
    //     delay_ms(2);
    // }
    // make_step_X(0);
    // for (int i = 0; i < 2000; i++)
    // {
    //     make_step_Y(0);
    //     // delay_ms(1);
    //     // _delay_us(200);
    // }

    // for (int i = 0; i < 20000; i++)
    // {
    //     // move_same_time_one_step(1, 1);
    //     make_step_X(1);
    //     delay_ms(1);
    //     // _delay_us(400);
    // }

    // for (int i = 0; i < 1600; i++)
    // {
    //     make_step_X(1);
    // }
    // for (int i = 0; i < 3200; i++)
    // {
    //     make_step_Y(0);
    // }
    move_deltas(-10, -40);

    // while (1)
    // {
    //     switch (PIND >> 4)
    //     { // Read buttons
    //     case b0:
    //         make_step_X(0);
    //         break;
    //     case b1:
    //         make_step_X(1);
    //         break;
    //     case b2:
    //         make_step_Y(1);
    //         break;
    //     case b3:
    //         make_step_Y(0);
    //         break;
    //     case b4:
    //         move_same_time_one_step(1, 1);
    //         break;
    //     case b5:
    //         move_same_time_one_step(0, 0);
    //         break;
    //     case b6:
    //         move_deltas(67.994, 19.998);
    //         break;
    //     default:
    //         PORTC = 0X00; // Set all output to 0
    //         break;
    //     }
    // }
}