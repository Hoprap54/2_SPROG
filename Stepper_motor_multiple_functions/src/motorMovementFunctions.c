#define BAUD 9600
#define NUMBER_STRING 1001

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h> //here the delay functions are found
#include "usart.h"
#include "motorMovementFunctions.h"
#include <stdbool.h>
#include <math.h>

#define d 1.25                                  // One turn moves 1.25mm
char pos[4] = {0b0001, 0b0100, 0b0010, 0b1000}; // Motor configuration
char lastPosX = 0b0000;
char lastPosY = 0b0000;

float stepHeightInv = 200 / 1.25;

void init_timer0(void)
{
        TCNT0 = 0;                           // Reset counter
        OCR0A = 249;                         // Set compare register A
        TCCR0A |= (1 << WGM01);              // Timer Mode to CTC
        TCCR0B |= (1 << CS01) | (1 << CS00); // Set prescaler to 64
}

void delay_ms(unsigned int t_ms)
{
        init_timer0();
        for (unsigned int i = 0; i < t_ms; i++)
        {
                // Compare count with delay value needed
                while ((TIFR0 & (1 << OCF0A)) == 0)
                {
                }                     // Wait for the overflow eventº
                TIFR0 = (1 << OCF0A); // Set timer to start counting again
        }
}

void move_F_PB()
{
        for (int i = 0; i < 4; i++)
        {
                PORTB = pos[i];
                delay_ms(3);
                lastPosX = i;
        }
}

void move_B_PB()
{
        for (int i = 3; i >= 0; i--)
        {
                PORTB = pos[i];
                delay_ms(3);
                lastPosX = i;
        }
}

void move_F_PD()
{
        for (int i = 3; i >= 0; i--)
        {
                PORTD = pos[i] << 4;
                delay_ms(3);
                lastPosY = i;
        }
}

void move_B_PD()
{
        for (int i = 0; i < 4; i++)
        {
                PORTD = pos[i] << 4;
                delay_ms(3);
                lastPosY = i;
        }
}

void move_same_time_B()
{
        int j = 0;
        for (int i = 3; i >= 0; i--, j++)
        {
                PORTD = pos[j] << 4;
                PORTB = pos[i];
                delay_ms(3);
                lastPosY = j;
                lastPosX = i;
        }
}

void move_same_time_F()
{
        int j = 3;
        for (int i = 0; i < 4; i++, j--)
        {
                PORTD = pos[j] << 4;
                PORTB = pos[i];
                delay_ms(3);
                lastPosY = j;
                lastPosX = i;
        }
}

void move_Center_From_X2()
{
        int stepsNeeded = 0; // here needs to be a formula to calculate how many steps are needed, it is not possible to do
        // by hand, it needs a diffrent program that will sum all the steps it has done from one limit switch to another
        for (int i = 0; i < stepsNeeded; i++)
                move_B_PB(); // this does 3 or 4 steps at a time, so we need to take that in consideration
}
// void move_t(int s){  // Move amount of turns
//     int turns = 50*s;
//     for(int j = 0; j < turns ; j++){
//         moveF();
//     }
// }
// void move_d(int dis){   // Move certain distance
//     int distance = dis*50/d;
//     for(int j = 0; j < distance ; j++){
//         moveB();
//     }
// }

void make_step_X(bool direction)
{
        // 1 = forward
        // 0 = back

        if (direction)
        {
                if (lastPosX == 0)
                {
                        lastPosX = 3;
                }
                else
                {
                        lastPosX--;
                }
        }
        else
        {
                if (lastPosX == 3)
                {
                        lastPosX = 0;
                }
                else
                {
                        lastPosX++;
                }
        }
        PORTD = pos[lastPosX] << 4;
        delay_ms(3);
}

void make_step_Y(bool direction)
{
        // 1 = forward
        // 0 = back
        if (direction)
        {
                if (lastPosY == 3)
                {
                        lastPosY = 0;
                }
                else
                {
                        lastPosY++;
                }
        }
        else
        {
                if (lastPosY == 0)
                {
                        lastPosY = 3;
                }
                else
                {
                        lastPosY--;
                }
        }
        PORTB = pos[lastPosY];
        delay_ms(3);
}

void move_full_circle(int radius)
{
        // uart_init();
        // io_redirect();
        // radius in mm
        uint16_t m = 0, n = 0, n_prev = 0, totalSteps = radius * stepHeightInv;
        bool direction = 1;
        for (m = 0; m < 4 * totalSteps; m++)
        {
                if (m < 2 * totalSteps)
                        n = (uint16_t)sqrt(2 * ((double)m) * radius * stepHeightInv - ((double)m) * m);
                else
                        n = (uint16_t)sqrt(2 * ((double)(4 * (double)totalSteps - 1 - m)) * radius * stepHeightInv - ((double)(4 * (double)totalSteps - 1 - m)) * (4 * (double)totalSteps - 1 - m));
                // // n = sqrt(-1000);
                // // printf("m = %u\nn = %u\nn_prev = %u\n\n", m, n, n_prev);
                if (((int32_t)n) - n_prev >= 1) // Because n and n_prev are of tyoe uint16_t the - operation between them will give a result in uint16_t which cannot be negative
                {
                        for (; n_prev < n; n_prev++)
                        {
                                make_step_Y(direction);
                        }
                }
                else if (((int32_t)n) - n_prev <= -1) // Because n and n_prev are of tyoe uint16_t the - operation between them will give a result in uint16_t which cannot be negative
                {
                        for (; n_prev > n; n_prev--)
                        {
                                make_step_Y(!direction);
                        }
                }
                make_step_X(direction);

                if (m == 2 * totalSteps - 1)
                {
                        direction = 0;
                }
        }
        // for (m = 2 * totalSteps; m >= 0; m--) HAHHAHAHA cannot be written like that, hahaha,
        // because m is gonna be equal to 0 after it gets decremented and starts again positive, bigger than 0, so it is infinite loop
        // for (m = 2 * totalSteps; m > 0; m--) // like that works
        // {
        //         n = (uint16_t)sqrt(2 * ((double)m) * radius * stepHeightInv - ((double)m) * m);
        //         if (((int32_t)n) - n_prev >= 1) // Because n and n_prev are of tyoe uint16_t the - operation between them will give a result in uint16_t which cannot be negative
        //         {
        //                 for (; n_prev < n; n_prev++)
        //                 {
        //                         make_step_Y(0);
        //                 }
        //         }
        //         else if (((int32_t)n) - n_prev <= -1) // Because n and n_prev are of tyoe uint16_t the - operation between them will give a result in uint16_t which cannot be negative
        //         {
        //                 for (; n_prev > n; n_prev--)
        //                 {
        //                         make_step_Y(1);
        //                 }
        //         }
        //         make_step_X(0);
        // }
}

void move_deltas(int dx, int dy)
{
        // works only for dy > dx and / is natural

        int r = dy / dx;

        for (int i = 0; i < dx * stepHeightInv; i++)
        {
                make_step_X(1);
                for (int j = 0; j < r; j++)
                        make_step_Y(1);
        }
}