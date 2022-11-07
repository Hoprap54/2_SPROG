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
		
        printf("Hello world and Beni!!! \n");
       
    }
        
    return 0;
}

/* Function description */

void run_Motor(){
    int count;
    DDRD = 0x60;       // Set Port D as output for the LEDs 0b0010 0000
    TCCR0A |= 0xA2;    // Fast PWM //mode with clear OC0A on compare match, set at bottom. Output B similar. //Page 84 and 86​
    TCCR0B |= 0x05;    // 1024 frequency scaling​
    TCNT0 = 0x0000;
    count += 1;
    if (count > OCR0A){
        PORTD = 0x60;
        OCR0A = 1;  // PWM TO 5v 
        count = 0;
    }else{
        PORTD = 0x00;
        OCR0A = 0;  // PWM TO 0v 
    }
}