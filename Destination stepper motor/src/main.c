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

#define b0 0x3b
#define b1 0x37 

// Function prototypes
void move(int, unsigned int);
void delay_v(unsigned int);

// Global variables
char pat[4] = {0b0101, 0b0110, 0b1010, 0b1001};
int step = 1;


int main(void) {     

    uart_init();
    io_redirect();

    /* Declaration of I/O Pins */
    DDRD = 0xF0;    // Set Port D as input for the buttons 0b1111 0000
    PORTD = 0x3F;   // Activate internall pull
    DDRB = 0xFF;    // Set Port B as output for the LN298N 0b1111 0000
    
    printf("\n[destination step] [time to do it in]\n");

    int pos = 0;
    unsigned int time = 100;

    while(1){
        scanf("%d %u", &pos, &time);
        
        move(pos, time);
    }
}


void move(int pos, unsigned int time){
    // Make sure pos is within 1-200
    if(pos < 0) pos += 200*(1-(pos/200));
    if(pos > 200) pos -= 200*(pos/200);
    printf("%d ", pos);

    int steps = pos - step; // Calculate number of steps to go
    int dir = 1; // Direction choice
    printf("%d ", steps);

    if(steps > 0) dir = -1;

    int steps_a = steps + 200*dir;
    printf("%d ", steps_a);
    if(steps < 0) steps *= -1;
    if(steps_a < 0) steps_a *= -1;

    printf("%d ", steps);

    if(steps > steps_a){
        steps = steps_a;
    }else{
        dir *= -1;
    }

    printf("%d\n", steps);

    if(time < 5) time = 5;
    if(time > 5000) time = 5000;
    unsigned int delay = steps / time;
    
    for(steps = steps; steps > 0; steps--){
        delay_v(delay);
        printf("%d ", steps);
        step += dir;

        PORTB = pat[(step+200) % 4];
    }
    if(step > 200) step = 1;
    if(step < 1) step = 200;

    printf("\nDestination reached\n");
}

void delay_v(unsigned int t){
    for(int i = 0; i < t; i++){
        _delay_ms(1);
    }
}