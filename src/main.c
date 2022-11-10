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
		
        printf("Hello \n");
       
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

void optocoupler(void){
unsigned int optotime;
float milisec;
int main(void) {    

    uart_init(); // open the communication to the microcontroller
	io_redirect(); // redirect input and output to the communication

//initializing the timers
    TCCR1A = 0x00;
    //TIMSK1|=(1<<5);
    // ICIE1=enable;
    TCCR1B = (1<<ICNC1)|/*(1<<ICES1)|*/(1<<CS12)|(1<<CS10);//noise cancel-raising edge, 1024 prescaling
    // TCCR1B = 0xC5;
    DDRB &= ~0x01;
    PORTB |= 0x01;
    TCNT1=0;
    printf("SPROG Get the time program\n");
    printf("Input Caputure Flag:%d\n",TIFR1>>5);
   TCNT1=0;
    while(1) {
		TCNT1=0;
        TIFR1|=1;
        ICR1=0;
        while((TIFR1|(1<<0))!=TIFR1){
        if((TIFR1|(1<<5))==TIFR1){
        //TCNT1;
		//printf("Timer: %u\n",TCNT1);
        TCNT1=0;
        TIFR1|=(1<<5);
        
        }

        printf("%d",TIFR1>>5);
        
        optotime=ICR1;
		milisec= ICR1*(10.24/160000);//conversion wrong
        
        printf("Optotime: %u/%flSeconds\n",optotime,milisec/1000);
        
        
        printf("%d",TIFR1>>5);

        printf("Debug");
        //printf("Reset input capture flag: %d",TIFR1>>5);
        _delay_ms(2000);
        
        }
    }
        
    return 0;
}
}