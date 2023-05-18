#define BAUD 9600
#define NUMBER_STRING 1001

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h> // Here the delay functions are found
#include "usart.h"
#include "motorMovementFunctions.h"
#include <stdbool.h>
#include <math.h>

#define d 1.25  // One turn moves 1.25mm
char pos[8] = {0b0001, 0b0101, 0b0100, 0b0110, 0b0010, 0b1010, 0b1000, 0b1001}; // Motor configuration
char lastPosX = 0b0000;
char lastPosY = 0b0000;
float stepHeightInv = 200 / 1.25;

void enable_int(void){
    EIMSK |= (1 << INT1) | (1 << INT0); // Turns on interrupt for INT0 & INT1 
    sei(); // Turn on interrupts
}
void init_timer0(void)
{
        TCNT0 = 0;                           // Reset counter
        OCR0A = 249;                         // Set compare register A
        TCCR0A |= (1 << WGM01);              // Timer Mode to CTC
        TCCR0B |= (1 << CS01) | (1 << CS00); // Set prescaler to 64
}
void delay_ms(unsigned int t_ms){
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

void move_F_PB(){
        for (int i = 0; i < 8; i++)
        {
                PORTB = pos[i];
                _delay_us(900);
                lastPosX = i;
        }
}

void move_B_PB()
{
        for (int i = 7; i >= 0; i--)
        {
                PORTB = pos[i];
                _delay_us(900);
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

void move_step_L(){
    int turns = 50;
    for(int j = 0; j < turns ; j++){
        move_F_PB();
    }
}

void move_step_R(){
    int turns = 50;
    for(int j = 0; j < turns ; j++){
        move_F_PD();
    }
}