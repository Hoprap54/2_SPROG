#define BAUD 9600
#define NUMBER_STRING 1001

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h> //here the delay functions are found
#include "usart.h"
#include "subfunctions.h"
#include <stdbool.h>

#define d 1.25 // One turn moves 1.25mm
char pos[4] = {0b0001, 0b0100, 0b0010, 0b1000}; // Motor configuration

void init_timer0(void){
    TCNT0 = 0;      // Reset counter
    OCR0A = 249;    // Set compare register A
    TCCR0A |= (1 << WGM01);              // Timer Mode to CTC
    TCCR0B |= (1 << CS01) | (1 << CS00); // Set prescaler to 64
}
void delay_ms(unsigned int t_ms){
    init_timer0();
    for(unsigned int i = 0; i < t_ms; i++){
    // Compare count with delay value needed
        while ((TIFR0 & (1 << OCF0A)) == 0){} // Wait for the overflow eventº
        TIFR0 = (1 << OCF0A);                 // Set timer to start counting again
    }
}
void moveF(){
    for(int i = 0; i < 4 ; i++){
        PORTB = pos[i];
        delay_ms(3);
    }
}
void moveB(){
    for(int i = 3; i >= 0 ; i--){
        PORTB = pos[i];
        delay_ms(3);
    }
}
void move_t(int s){  // Move amount of turns
    int turns = 50*s;
    for(int j = 0; j < turns ; j++){
        moveF();
    }
}
void move_d(int dis){   // Move certain distance
    int distance = dis*50/d;
    for(int j = 0; j < distance ; j++){
        moveF();
    }
}