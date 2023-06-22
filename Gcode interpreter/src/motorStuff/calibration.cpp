#include "calibration.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "motorMovementFunctions.h"
#include <avr/interrupt.h>

#define PINX1 4 // what pin you may want
#define PINX2 5 // what pin you may want
#define PINY1 6 // what pin you may want
#define PINY2 7 // what pin you may want

volatile bool isCalibratingX = 0;
volatile bool isEndX = 0;
volatile bool isCalibratingY = 0;
volatile bool isEndY = 0;

unsigned long totalPossibleStepsForX = 0;
unsigned long totalPossibleStepsForY = 0;

void limitSwitchSetUp()
{
    DDRD &= ~(1 << PD2);
    PORTD |= 1 << PD2;
    EIMSK |= 1 << INT0;
    EICRA |= (1 << ISC01); // set INT0 to trigger on falling edge

    // DDRD &= ~(1 << PD3);
    // PORTD |= 1 << PD3;
    // EIMSK |= 1 << INT1;
    // EICRA |= (1 << ISC11); // set INT1 to trigger on falling edge

    sei();
}

void calibrationX()
{
    isCalibratingX = true;

    isEndX = 0;
    while (!isEndX)
    {
        make_step_X(0);
    }

    delay_ms(2000);

    isEndX = 0;
    while (!isEndX)
    {
        make_step_X(1);
        totalPossibleStepsForX++;
    }
    delay_ms(2000);
    isEndX = 0;

    isCalibratingX = false;

    for (unsigned long i = 0; i < totalPossibleStepsForX / 2; i++)
    {
        make_step_X(0);
    }
    delay_ms(500);
}

void calibrationY()
{
    isCalibratingY = 1;

    isEndY = 0;
    while (!isEndY)
    {
        make_step_Y(0);
    }

    delay_ms(2000);

    isEndY = 0;
    while (!isEndY)
    {
        make_step_Y(1);
        totalPossibleStepsForY++;
    }
    delay_ms(2000);
    isEndY = 0;

    isCalibratingY = 0;

    for (unsigned long i = 0; i < totalPossibleStepsForY / 2; i++)
    {
        make_step_Y(0);
    }
    delay_ms(2000);
}

void calibrationZ()
{
}

ISR(INT0_vect)
{
    // PORTD &= ~((1 << PD4) || (1 << PD5) || (1 << PD6) || (1 << PD7));
    // PORTB &= ~((1 << PB0) || (1 << PB1) || (1 << PB2) || (1 << PB3));
    // PORTD = 0b00000000;
    // PORTB = 0b00000000;
    if (isCalibratingX)
    {
        isEndX = true;
    }
    else if (isCalibratingY)
    {
        isEndY = true;
    }
    else
    {
        PORTB = (1 << PB5);
        changeMachineState(0);
    }
}

ISR(INT1_vect)
{
    // if (isCalibratingY)
    // {
    //     isEndY = true;
    // }
    // else
    // {
    //     PORTB = (1 << PB5);
    //     changeMachineState(0);
    // }
}