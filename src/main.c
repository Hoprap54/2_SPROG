#define F_CPU 16000000UL //needs to be defined for the delay functions to work.
#define BAUD 9600
#define NUMBER_STRING 1001

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h> //here the delay functions are found
#include "usart.h"


/* Declare function */

int main(void) {    

    uart_init();   // open the communication to the microcontroller
	io_redirect(); // redirect input and output to the communication

/* Declare global variable */

    
    while(1) {
		
		_delay_ms(1000)	;
		
        printf("Hello Henrik !!! \n");
       
    }
        
    return 0;
}

/* Function description */