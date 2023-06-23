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
char pos[8] = {0b0001, 0b0101, 0b0100, 0b0110, 0b0010, 0b1010, 0b1000, 0b1001}; // Motor configuration, this can be understood by
// looking in the report where it explains how this bits get transformed into specific coils being energised
char lastPosX = 0b0000;           // remembers the last motor configuration for X
char lastPosY = 0b0000;           // remembers the last motor configuration for Y
float stepHeightInv = 400 / 1.25; // this is the inverse of what distance it is gained from one step.
// it is used the inverse of it only because i wanted to write number * stepHeightInv but not number * stepHeight.

bool xAxisDirection = 1;        // remembers the direction of X axis, 1 = true = forward, 0 = false = backwards
bool yAxisDirection = 1;        // remembers the direction of Y axis, 1 = true = forward, 0 = false = backwards
volatile bool machineState = 1; // remembers the machine state.

void init_timer0(void) // set timer
{
    TCNT0 = 0;                           // Reset counter
    OCR0A = 249;                         // Set compare register A
    TCCR0A |= (1 << WGM01);              // Timer Mode to CTC
    TCCR0B |= (1 << CS01) | (1 << CS00); // Set prescaler to 64
}

void delay_ms(unsigned int t_ms) // make our own delay function with the timer0
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

void move_same_time_one_step(bool xDirection, bool yDirection)
{
    // 1 = forward
    // 0 = back
    if (machineState == 0)
        return; // if the machineState = 0 it means the limit switch got pressed when it was not
    // in calibration sequence which indicates that a big mistake has happened and that it should just live the
    // function everytime it wants to do a step

    // update the new directions for the current movement
    xAxisDirection = xDirection;
    yAxisDirection = yDirection;

    if (xDirection) // because it is used a list and not a ciclical list when it
    // gets to position 0 and 7 in the array it needs to restart from value 7 or 0 respectively
    // so the movement can continue in the apropriate order. This order can be seen in the report.
    // if the direction is forward = true it needs to go backwards through the array, I know, confussing xd.
    {
        if (lastPosX == 0) // if it needs to go backwards through the array and the last position was 0 it means that it the next one needs to be 7
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
        if (lastPosX == 7) // forward through the array if the last position was 7 the next one can be only 0 if not continue just incrementing the position.
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

    // after all that the last position variable lastPos(X/Y) got updated with the
    // new position and the PORTs will be appropriately changed to the new lastPos(X/Y) value

    PORTD &= 0x0F;
    PORTD |= pos[lastPosX] << 4; // because the motors on port B are set on the pins 4 5 6 and 7
    // any array set like 0b0000xxxx needs to be bitshifted in order to work on the motors 0bxxxx0000

    PORTB &= 0xF0;
    PORTB |= pos[lastPosY];

    delay_ms(1);

    PORTD &= 0x0F;
    PORTB &= 0xF0;
}

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
    delay_ms(1);
    PORTB &= 0xF0;
}

void move_full_circle(int radius)
{
    // radius in mm
    uint16_t m = 0, n = 0, n_prev = 0, totalSteps = radius * stepHeightInv; // totalSteps it is calculated only for a quarter of the circle
    bool direction = 1;                                                     // keeps track of the direction in order to move.
    for (m = 0; m < 4 * totalSteps; m++)
    {
        if (m < 2 * totalSteps)
            n = (uint16_t)sqrt(2 * ((double)m) * radius * stepHeightInv - ((double)m) * m);
        else
            n = (uint16_t)sqrt(2 * ((double)(4 * (double)totalSteps - 1 - m)) * radius * stepHeightInv - ((double)(4 * (double)totalSteps - 1 - m)) * (4 * (double)totalSteps - 1 - m));
        if (((int32_t)n) - n_prev >= 1) // Because n and n_prev are of tyoe uint16_t the - operation between them will give a result in uint16_t which cannot be negative
        {
            for (; n_prev < n; n_prev++)
            {
                make_step_Y(direction);
            }
        }
        else if (((int32_t)n) - n_prev <= -1) // Because n and n_prev are of type uint16_t the - operation between them will give a result in uint16_t which cannot be negative
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

void move_deltas(double dx, double dy) // used for linear movement in gcode mode
{

    // uart_init();
    // io_redirect();

    // int stepsDoneX = 0;
    // int stepsDoneY = 0;

    if (dx < 0) // checking in what direction it moves on x axis
    {
        xAxisDirection = 0; // if dx < 0 the direction will be set to 0
        dx = (-1) * dx;     // because we know that the direction is backward we can
        // just work with positive numbers in order to make life easier
    }
    else // same for y axis
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
    uint32_t precision = 100000; // because double calculations make aproximations mistakes it was decided
    // to use uint32_t by multypling the really small double number by precision and taking it as the workable value
    // after this any time of number and calculations needs to take in the account this change
    // the problem can be exemplified  lets say that 0.000040000 + 0.000040000 = 0.000080000
    // but c will make it as 0.000040000 + 0.000040000 = 0.000080001
    // this algorithm can make tens of thousands of calculations which means that this error adds up.

    if (dx >= dy) // taking for which one is bigger, because ratio>=1, the way the algorithm is made
    {
        if (dy != 0) // make sure dy is no 0, cause division by 0 xd
        {
            ratio = dx / dy;
            uint16_t intPartOfRatio = truncf(ratio);                                        // taking full number from the ratio
            uint32_t doublePartOfRatio = round((ratio - intPartOfRatio) * precision) + 1UL; // taking the double part of the number and multipling with the precision.
            uint32_t sumOfDoubles = 0;                                                      // this will take care of when the sum of the remainders till this point

            for (uint16_t i = 0; i < round(dy * stepHeightInv); i++) // the number of steps that needs to be
            // done by the axis with the smallest numbers of steps, which is Y axis in this case
            {
                move_same_time_one_step(xAxisDirection, yAxisDirection); // move together one step
                // because x axis has to do more steps lets see how many it needs to do
                for (int j = 1; j < intPartOfRatio; j++) // intPartOfRatio can have values like 1 2 3... and this would mean that dx is 1 times bigger than dy, 2 times bigger....
                {
                    make_step_X(xAxisDirection);
                }
                if (sumOfDoubles >= precision)
                {
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
        }
        else // if dy = 0 only one axis moves
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

                move_same_time_one_step(xAxisDirection, yAxisDirection);
                for (int j = 1; j < intPartOfRatio; j++)
                {
                    make_step_Y(yAxisDirection);
                }
                if (sumOfDoubles >= precision)
                {
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

void arc_move(double A[], double B[], double r) // this is a nice way of calculating to make arc movement but it was not implemented in the end.
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

bool giveXDirection() // gives the direction of the x axis either forward = true or backwards = false, it is not used in code right now.
{
    return xAxisDirection;
}
void changeMachineState(volatile bool newState) // it is used by the limit switch ISR function which either enables or disables movement of the motor.
{
    machineState = newState;
}