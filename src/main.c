/*
*   D2 -> B.R.
*   D3 -> B.L.
*   ...
*   D8 -> In1
*   D9 -> In2
*   D10 -> In3
*   D11 -> In4
*
*   Step advance .. m
*/

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

#define b0 0x3b
#define b1 0x37 

void move_step(int s);    // Move amount of steps
void moveF();   // Move forward
void moveB();   // Move backwards
void move_d(int dis); // Move certain distance

#define stpa_motor 1.8  // Step angle of a motor
#define d 7.89 // One turn moves 79.39mm
int cycle = (int)360/stpa_motor; // Amount of steps for one turn
char pos[4] = {0b0001, 0b0100, 0b1000, 0b0010}; // Motor configuration

int main(void) {     
    uart_init();    // Open the communication to the micro controller
    io_redirect();  // Redirect input and output to the communication

    /* Declaration of I/O Pins */
    DDRD = 0xF0;    // Set Port D as input for the buttons 0b1111 0000
    PORTD = 0x3F;   // Activate internall pull
    DDRB = 0xFF;    // Set Port B as output for the LN298N 0b1111 1111
   
    /* Declare variables */
    int d_run;

    while (1){
        switch (PIND){
        case b0:
            printf("\nDistance: ");
            scanf("%d", &d_run);
            move_d(d_run);
            printf("\nDone");
            break;
        
        case b1:
            printf("\nTurns: ");
            scanf("%d", &d_run);
            move_step(d_run);
            printf("\nDone");
            break;
        default:
            PORTB = 0X00;   // Set all output to 0
            break;
        }
    }
}

void moveF(){
    for(int i = 0; i < 4 ; i++){
        PORTB = pos[i];
        _delay_ms(4);
    }
}
void moveB(){
    for(int i = 3; i >= 0 ; i--){
        PORTB = pos[i];
        _delay_ms(10);
    }
}
void move_step(int s){
    int turns = 50*s;
    for(int j = 0; j < turns ; j++){
        moveF();
    }
}

void move_d(int dis){
    int distance = dis*50/d;
    for(int j = 0; j < distance ; j++){
        moveF();
    }
}