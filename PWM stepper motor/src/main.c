/*
*   D2 -> B.R.
*   D3 -> B.L.
*   ...
*   D8 -> In1
*   D9 -> In2
*   D10 -> In3
*   D11 -> In4
*/

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"


// Function prototypes
void move(int, unsigned int);
void delay_v(unsigned int);

void PWM0_on();
void PWM2_on();

// Global variables
char pat[4] = {0b0101, 0b0110, 0b1010, 0b1001};
int pos = 0;


int main(void) {

    uart_init();
    io_redirect();

    /* Declaration of I/O Pins */
    DDRD =  0xF0;   // Set Port D as input for the buttons 0b1111 0000
    PORTD = 0x3F;   // Activate internall pull
    DDRB =  0xFF;   // Set Port B as output for the LN298N 0b1111 0000
    
    // Main variables
    int steps = 0;
    unsigned int time = 10; // in ms

    printf("\n[+-steps] [time to try and do movement in (5-5000)]\n");

    while(1){
        scanf("%d %u", &steps, &time);

        move(steps, time);
    }
}

void move(int steps, unsigned int time){
    int dir = 1;
    if(steps < 0){
        dir = -1;
        steps *= -1;
    }

    if(time < 5) time = 5;
    if(time > 5000) time = 5000;

    unsigned int delay = time / steps;
    if(delay < 5) delay = 5;

    for(steps = steps; steps > 0; steps--){
        delay_v(delay);

        pos += dir;
        if(pos > 3) pos = 0;
        if(pos < 0) pos = 3;
        
        PORTB = pat[pos % 4];
    }
    printf("\nMovement finished\n");
}

void delay_v(unsigned int t){
    for(int i = 0; i < t; i++){
        _delay_ms(1);
    }
}


void PWM0_on(){
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); // Fast PWM mode - OC0A (PD6) clear at OC
    TCCR0B = (1 << CS02) | (1 << CS00); // Turn on clock - prescale 1024
}

void PWM2_on(){
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20); // Fast PWM mode - OC2B (PD3) clear at OC
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // Turn on clock - prescale 1024
}