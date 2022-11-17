#define F_CPU 16000000UL //needs to be defined for the delay functions to work.
#define BAUD 9600
#define NUMBER_STRING 1001

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h> //here the delay functions are found
#include "usart.h"

volatile unsigned long int timer=0, counter; 
unsigned long int microseconds;
float seconds;
void initialize(void);

ISR(TIMER1_CAPT_vect){
    timer=ICR1+65535*counter;
    //printf("Input Capture EVENT!!!");
    TCNT1=0;
    TIFR1|=1<<ICF1;//reseting the input capture flag
    counter=0;
    
}
ISR(TIMER1_OVF_vect){
    counter++;
    TCNT1=0;
}
/* Declare function */

int main(void) {    

    uart_init();   // open the communication to the microcontroller
	io_redirect(); // redirect input and output to the communication
    initialize();
/* Declare global variable */

    
    while(1) {
		
		_delay_ms(1000)	;
		
        printf("Hello \n");
        seconds=(timer*1000)/15625000;
            printf("\n This is the current state of the the timer:%lu and seconds:%f",timer,seconds);
            _delay_ms(2000);
            
       
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



void initialize(void){
    sei();//enable global interrupts
    
    TIMSK1|=(1<<ICIE1)|(1<<TOIE1);//timer interrupts must be enabled
    TCCR1A = 0x00;
    TCCR1B = (1<<ICNC1)|/*(1<<ICES1)|*/(1<<CS12)|(1<<CS10);//noise cancel-/*falling*/ raising edge - 1024 prescaling
    DDRB &= ~0x01;
    PORTB |= 0x01;
    TIFR1|=1<<ICF1;//reseting input capture flag

}


