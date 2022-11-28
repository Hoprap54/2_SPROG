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
#include <stdbool.h>

//variables for the interrupts
volatile unsigned long int timer = 0, counter = 0; //timer: variable for the time; counter: counter to count timeroverflows
volatile bool car_move_flag = false; //variable to indicate whether the car is moving
volatile int i, distancecounter = 0; //for for loop in interrupt
volatile char readBuffer[100];

//variables for the functions
char acceleration_index(double, double); //function for checking the state of acceleration
char acceleration_flag = 0; //variable for indicating the state of aceleration
char savereadBuffer[100];
double seconds, speed = 100, prev_speed = 0, eigthcircumference = 0.02589182, distance;

void initialize(void);//function for initializing the timer and interrupts
char displayreader(void);

ISR(USART_RX_vect){
    //printf("page page0%c%c%c",255,255,255);
    for(i=0;i<8;i++){

        scanf("%c",&readBuffer[i]);

    }

}

ISR(TIMER1_CAPT_vect){
    
    timer=ICR1+65535*counter;//updating timer value
    //printf("Input Capture EVENT!!!"); line used for debugging
    TCNT1=0; //reseting the timer to zero
    TIFR1|=1<<ICF1;//reseting the input capture flag
    counter=0;//reseting overflow counter
    car_move_flag = true;//car is being moved
    distancecounter++;
    
}
ISR(TIMER1_OVF_vect){
    counter++;//adding one to the overflow counter
    TCNT1=0;
    if(counter>2)//the car has not really moved for a long time
    car_move_flag = 0;//so the move-flag is reset
}
/* Declare function */
int main(void) {    

    uart_init(); // open the communication to the microcontroller
	io_redirect(); // redirect input and output to the communication

    initialize();

    //reseting all the values
    speed=0;
    prev_speed=0;
    acceleration_flag=0;
    car_move_flag = false;


    printf("%lf", speed); 
        while(1){
            
            //seconds calculation
            seconds = ((double)timer*1000)/15625000;
            printf("\n debug: %f",seconds);

            //distance calculation
            distance = (double)distancecounter*eigthcircumference;


            //speed calculation
            if (seconds)//speed is only recalculated when there is actually a timer-value (that is not zero)
            speed = eigthcircumference/seconds;//distance divided by time
            
            //check whether car is moving
            if (car_move_flag){
                printf("\nCar is moving");
            }


            //if it is not moving - set speed etc. to zero
            else if (car_move_flag==false){
                printf("\nCar is not moving.");
                speed=0;
                timer=0;
                seconds=0;
                prev_speed=0;
            }

        
            //checking for acceleration
            acceleration_flag= acceleration_index(speed, prev_speed);

            //output current data in console
            printf("\n This is the current state of the the timer:%lu and seconds:%lf - speed:%lf - accelerationflag:%d - distance:%lf",timer,seconds, speed, acceleration_flag, distance);
            _delay_ms(1000);
           

            //printf("\n prev-speed:%f",prev_speed); debugging
            prev_speed=speed;//setting a new value for previous speed
                                           
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



inline void initialize(void){
    
    sei();//enable global interrupts
    
    //usart interrupts
    SREG |= 1<<SREG_I;
    UCSR0B |=1<<RXCIE0;//enabeling interrupt for rx complete
    
    
    TIMSK1 |= (1<<ICIE1)|(1<<TOIE1);//timer interrupts must be enabled
    TCCR1A = 0x00;
    TCCR1B = (1<<ICNC1)|/*(1<<ICES1)|*/(1<<CS12)|(1<<CS10);//noise cancel-/*falling*/ raising edge - 1024 prescaling
    DDRB &= ~0x01;
    PORTB |= 0x01;
    TIFR1 |= 1<<ICF1;//reseting input capture flag
    
    
}


inline char displayreader(void){

for(i=0;i<100;i++){

    savereadBuffer[i] = readBuffer[i];

}


}


char acceleration_index(double current_speed, double previous_speed){

    char acceleration_flag;
    if(current_speed == 0 && previous_speed == 0)
    acceleration_flag=0;
    else
    if(current_speed < prev_speed)
    acceleration_flag=1;
    if(current_speed > prev_speed)
    acceleration_flag = 2;
    if(speed==0)
    acceleration_flag = 0;
    printf("debug");
    

    return acceleration_flag;

}


