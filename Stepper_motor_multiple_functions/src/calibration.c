#include "calibration.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "subfunctions.h"

#define PINX1 4 // what pin you may want
#define PINX2 5 // what pin you may want
#define PINY1 6 // what pin you may want
#define PINY2 7 // what pin you may want

bool calibrationX()
{
    while (PINC << PINX1 == 1)
        move_B_PB();

    while (PINC << PINX2 == 1)
        move_F_PB();

    move_Center_From_X2();
}

bool calibrationY()
{
}

bool calibrationZ()
{
    return true;
}
