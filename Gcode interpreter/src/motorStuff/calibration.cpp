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

// all this variables are volatile because the ISR works with them
volatile bool isCalibratingX = 0;
volatile bool isEndX = 0;
volatile bool isCalibratingY = 0;
volatile bool isEndY = 0;
volatile bool bouncingProblem = false;

// this will increment till the max possible steps that can be done from one limit switch to another.
unsigned long totalPossibleStepsForX = 0;
unsigned long totalPossibleStepsForY = 0;

void limitSwitchSetUp()
{
    // using only one External interrupt for all 4 limit switches, we do not care which
    // limit switch was activated because if at least one of them is activated that means
    // that something went wrong and will be left to the machinest to solve.
    DDRD &= ~(1 << PD3) | (1 << PD2);
    PORTD |= (1 << PD3) | (1 << PD2);
    EIMSK |= (1 << INT1) | (1 << INT0);
    EICRA |= (1 << ISC11) | (1 << ISC01); // set INT0 to trigger on falling edge.

    sei(); // activate all interrupts.

    // P.S when working with this i could not cli(); because when eventually i would sei(); for no reason the ISR would start working.
}

void calibrationX()
{
    // reseting variables to their appropriate start value.
    totalPossibleStepsForX = 0;
    isCalibratingX = true;
    bouncingProblem = false;

    isEndX = 0;     // this variable will be changed in the ISR when the limit switch will be pressed.
    while (!isEndX) // if isEndX = true it means it has reached the end and the axis will not continue in that direction.
    {
        make_step_X(0); // moving backward on X axis.
    }
    bouncingProblem = true; // the limit switch has the problem of bouncing after it has been released and bouncingProblem
                            // set to true will bypass the ISR functions for a number of steps.
    delay_ms(2000);         // after it got to the first limit switch it will wait for 2 seconds because why not.

    isEndX = 0; // reset this to allow for the next part of the X axis calibration.
    while (!isEndX)
    {
        make_step_X(1);           // moving forward on X axis.
        totalPossibleStepsForX++; // for each step increment the total steps that have been done.

        if (totalPossibleStepsForX == (unsigned long)3200) // for 10 mm.
        {
            bouncingProblem = false; // after it has done 3200 steps (10 mm distance) it took more than enough time
                                     // in order for the bouncing to occur, which means that after this moment any
                                     // limit swtich presses will come from the actual machine.
        }
    }
    delay_ms(2000);
    isEndX = 0;

    isCalibratingX = false; // Tells the ISR that another limit switch press will not be considered as
    // calibration but as an actual terrible mistake

    bouncingProblem = true;                                        // when coming back it can also have a bouncing problem
    for (unsigned long i = 0; i < totalPossibleStepsForX / 2; i++) // reposition the block in the exact center.
    {
        make_step_X(0);
        if (i == (unsigned long)3200) // for 10 mm.
        {
            bouncingProblem = false;
        }
    }
    delay_ms(500);
}

// this is just as the X axis.
void calibrationY()
{
    totalPossibleStepsForY = 0;
    isCalibratingY = true;
    bouncingProblem = false;

    isEndY = 0;
    while (!isEndY)
    {
        make_step_Y(0);
    }
    bouncingProblem = true;
    delay_ms(2000);

    isEndY = 0;
    while (!isEndY)
    {
        make_step_Y(1);
        totalPossibleStepsForY++;

        if (totalPossibleStepsForY == (unsigned long)3200) // for 10 mm
        {
            bouncingProblem = false;
        }
    }
    delay_ms(2000);
    isEndY = 0;

    isCalibratingY = 0;
    bouncingProblem = true; // when coming back it can also have a bouncing problem

    for (unsigned long i = 0; i < totalPossibleStepsForY / 2; i++)
    {
        make_step_Y(0);
        if (i == (unsigned long)3200) // for 10 mm.
        {
            bouncingProblem = false;
        }
    }
    delay_ms(500);
}

void calibrationZ()
{
}

ISR(INT0_vect)
{
    // this does not work, even if you discconnect the pins with which the arduino talks to the motors (in theory breaking the comunication)
    // it did not work as inded, not at all, made stupid behavior maybe because flactuations of the voltage at the pins.
    //  PORTD &= ~((1 << PD4) || (1 << PD5) || (1 << PD6) || (1 << PD7));
    //  PORTB &= ~((1 << PB0) || (1 << PB1) || (1 << PB2) || (1 << PB3));
    //  PORTD = 0b00000000;
    //  PORTB = 0b00000000;
    if (!bouncingProblem) // bouncingProblem = true nothing will be executed.
    {
        if (isCalibratingX) // make true the apropriate axis ending
        {
            isEndX = true;
        }
        else if (isCalibratingY)
        {
            isEndY = true;
        }
        else // if none of them are in calibration mode that means that is freerunning either dpad
             // or gcode and any mistake will cancel the ability to move
        {
            PORTB = (1 << PB5);    // turn L LED PB5 on to show that it stoped everything
            changeMachineState(0); // this will put the value of the parameter in a state variable that does not allow the motors to step
        }
    }
}