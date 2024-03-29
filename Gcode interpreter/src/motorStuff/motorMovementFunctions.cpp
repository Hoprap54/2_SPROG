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
#include <calibration.h>

#define d 1.25                                                                  // One turn moves 1.25mm
char pos[8] = {0b0001, 0b0101, 0b0100, 0b0110, 0b0010, 0b1010, 0b1000, 0b1001}; // Motor configuration
char lastPosX = 0b0000;
char lastPosY = 0b0000;
unsigned int v = 1;
float stepHeightInv = 400 / 1.25;

bool xAxisDirection = 1;
bool yAxisDirection = 1;
volatile bool machineState = 1;

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
    for (int i = 0; i < 8; i++)
    {
        PORTB = pos[i];
        delay_ms(v);
        lastPosX = i;
    }
}

void move_B_PB()
{
    for (int i = 7; i >= 0; i--)
    {
        PORTB = pos[i];
        delay_ms(v);
        lastPosX = i;
    }
}

void move_F_PD()
{
    for (int i = 7; i >= 0; i--)
    {
        PORTD = pos[i] << 4;
        _delay_us(900);
        lastPosY = i;
    }
}

void move_B_PD()
{
    for (int i = 0; i < 8; i++)
    {
        PORTD = pos[i] << 4;
        _delay_us(900);
        lastPosY = i;
    }
}

void move_step_L()
{
    int turns = 50;
    for (int j = 0; j < turns; j++)
    {
        move_F_PB();
    }
}

void move_step_R()
{
    int turns = 50;
    for (int j = 0; j < turns; j++)
    {
        move_F_PD();
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

void move_same_time_one_step(bool xDirection, bool yDirection)
{
    // 1 = forward
    // 0 = back
    if (machineState == 0)
        return;

    xAxisDirection = xDirection;
    yAxisDirection = yDirection;

    if (xDirection)
    {
        if (lastPosX == 0)
        {
            lastPosX = 7;
        }
        else
        {
            lastPosX--;
        }
    }
    else
    {
        if (lastPosX == 7)
        {
            lastPosX = 0;
        }
        else
        {
            lastPosX++;
        }
    }
    if (yDirection)
    {
        if (lastPosY == 7)
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
            lastPosY = 7;
        }
        else
        {
            lastPosY--;
        }
    }

    PORTD &= 0x0F;
    PORTD |= pos[lastPosX] << 4;

    PORTB &= 0xF0;
    PORTB |= pos[lastPosY];

    // _delay_us(900);
    // _delay_us(2000);
    delay_ms(1);

    PORTD &= 0x0F;
    PORTB &= 0xF0;
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
    if (machineState == 0)
        return;

    xAxisDirection = direction;

    if (direction)
    {
        if (lastPosX == 0)
        {
            lastPosX = 7;
        }
        else
        {
            lastPosX--;
        }
    }
    else
    {
        if (lastPosX == 7)
        {
            lastPosX = 0;
        }
        else
        {
            lastPosX++;
        }
    }

    PORTD &= 0x0F;
    PORTD |= pos[lastPosX] << 4;
    // _delay_us(2000);
    delay_ms(1);
    PORTD &= 0x0F;
}

void make_step_Y(bool direction)
{
    // 1 = forward
    // 0 = back
    if (machineState == 0)
        return;

    yAxisDirection = direction;

    if (direction)
    {
        if (lastPosY == 7)
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
            lastPosY = 7;
        }
        else
        {
            lastPosY--;
        }
    }
    PORTB &= 0xF0;
    PORTB |= pos[lastPosY];
    // _delay_us(2000);
    delay_ms(1);
    PORTB &= 0xF0;
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

void move_deltas(double dx, double dy)
{

    // uart_init();
    // io_redirect();

    // int stepsDoneX = 0;
    // int stepsDoneY = 0;

    if (dx < 0)
    {
        xAxisDirection = 0;
        dx = (-1) * dx;
    }
    else
    {
        xAxisDirection = 1;
    }
    if (dy < 0)
    {
        yAxisDirection = 0;
        dy = (-1) * dy;
    }
    else
    {
        yAxisDirection = 1;
    }

    double ratio;
    uint32_t precision = 100000;

    if (dx >= dy)
    {
        if (dy != 0)
        {
            ratio = dx / dy;
            uint16_t intPartOfRatio = truncf(ratio);
            // double doublePartOfRatio = ratio - intPartOfRatio;
            uint32_t doublePartOfRatio = round((ratio - intPartOfRatio) * precision) + 1UL;
            uint32_t sumOfDoubles = 0;
            printf("doublePartOfRatio = %lu\n", doublePartOfRatio);
            // printf("round(dy * stepHeightInv) = %f\n", round(dy * stepHeightInv));
            // printf("intPartOfRatio = %d\n", intPartOfRatio);

            for (uint16_t i = 0; i < round(dy * stepHeightInv); i++)
            {
                // stepsDoneY++;
                // make_step_Y(yDirection);
                // stepsDoneX++;
                move_same_time_one_step(xAxisDirection, yAxisDirection);
                for (int j = 1; j < intPartOfRatio; j++)
                {
                    // stepsDoneX++;
                    make_step_X(xAxisDirection);
                }
                if (sumOfDoubles >= precision)
                {
                    // stepsDoneX++;
                    make_step_X(xAxisDirection);
                    sumOfDoubles -= precision;
                    if (sumOfDoubles < 9UL)
                        sumOfDoubles = 0;
                }
                sumOfDoubles += doublePartOfRatio;
            }

            if (sumOfDoubles >= precision)
            {
                make_step_X(xAxisDirection);
                sumOfDoubles = 0;
            }

            // printf("xSteps = %d\n", stepsDoneX);
            // printf("ySteps = %d\n", stepsDoneY);
            // printf("ratio = %f\n", ratio);
            // printf("sumOfDoubles = %lu\n", sumOfDoubles);
            // printf("intPartOfRatio = %d\n", intPartOfRatio);
            // delay_ms(1000);
        }
        else
        {
            for (uint16_t i = 0; i < dx * stepHeightInv; i++)
                make_step_X(xAxisDirection);
        }
    }
    else
    {
        if (dx != 0)
        {
            ratio = dy / dx;
            uint16_t intPartOfRatio = truncf(ratio);
            uint32_t doublePartOfRatio = round((ratio - intPartOfRatio) * precision) + 1UL;
            uint32_t sumOfDoubles = 0;
            for (uint16_t i = 0; i < round(dx * stepHeightInv); i++)
            {
                // stepsDoneY++;
                // stepsDoneX++;
                move_same_time_one_step(xAxisDirection, yAxisDirection);
                for (int j = 1; j < intPartOfRatio; j++)
                {
                    // stepsDoneY++;
                    make_step_Y(yAxisDirection);
                }
                if (sumOfDoubles >= precision)
                {
                    // stepsDoneY++;
                    make_step_Y(yAxisDirection);
                    sumOfDoubles -= precision;
                    if (sumOfDoubles < 9UL)
                        sumOfDoubles = 0;
                }
                sumOfDoubles += doublePartOfRatio;
            }

            if (sumOfDoubles >= precision)
            {
                make_step_Y(yAxisDirection);
                sumOfDoubles = 0;
            }
        }
        else
        {
            for (uint16_t i = 0; i < dy * stepHeightInv; i++)
                make_step_Y(yAxisDirection);
        }
    }
}
void arc_move(double A[], double B[], double r)
{
    // These equations calculate from:
    // Start point, end point and radius
    // to:
    // An arc described by its origin (center) and span angle (turning CCW from start point to end point)

    // Middle point coordinate calculations
    double Mx = (B[0] - A[0]) / 2; // M x
    double My = (B[1] - A[1]) / 2; // M y

    // Delta value between point A and M
    double dx = Mx - A[0]; // dx from M to A
    double dy = My - A[1]; // dy from M to A

    double c = sqrt(dx * dx + dy * dy); // Half chord length

    double l = sqrt(r * r - c * c); // Perpendicular length from chord to center of arc

    // Perpendicular unit vector for delta vector
    double pe_dx = -dy / c;
    double pe_dy = dx / c;

    // Perpendicular vector with proper length
    double p_dx = pe_dx * l;
    double p_dy = pe_dy * l;

    // Vector AC from point A to center of arc C
    double ACx = dx + p_dx;
    double ACy = dy + p_dy;
}

bool giveXDirection()
{
    return xAxisDirection;
}
void changeMachineState(volatile bool newState)
{
    machineState = newState;
}