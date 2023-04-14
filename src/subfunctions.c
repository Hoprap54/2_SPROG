#define F_CPU 16000000UL //needs to be defined for the delay functions to work.
#define BAUD 9600
#define NUMBER_STRING 1001
//#define CIRCUMFERENCE 0.02589182

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h> //here the delay functions are found
#include "usart.h"
#include "subfunctions.h"
#include <stdbool.h>

#define stpa_motor 1.8  // Step angle of a motor
#define d 7.89 // One turn moves 79.39mm
int cycle = (int)360/stpa_motor; // Amount of steps for one turn
char pos[4] = {0b0001, 0b0100, 0b1000, 0b0010}; // Motor configuration

void moveF(){
    for(int i = 0; i < 4 ; i++){
        PORTB = pos[i];
        _delay_ms(4);
    }
}
void moveB(){
    for(int i = 3; i >= 0 ; i--){
        PORTB = pos[i];
        _delay_ms(4);
    }
}
void move_step(int s){  // Move amount of steps
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