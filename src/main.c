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

// Variables for the interrupts
volatile unsigned long int timer = 0, counter = 0; // Timer: variable for the time; counter: counter to count timeroverflows
volatile bool car_move_flag = false, stringbeginflag=false;               // Variable to indicate whether the car is moving
volatile int i, distancecounter = 0, test = 3, readBufferindex = 0;     // For for loop in interrupt
volatile char readBuffer[100]= {0}, rxexpect=0x71;

// Variables for the functions
char acceleration_index(double, double);    // Function for checking the state of acceleration
char acceleration_flag = 0;                 // Variable for indicating the state of aceleration
char savereadBuffer[100]= {0};
int currentpagenumber = 0;
double seconds, speed = 100, prev_speed = 0, eigthcircumference = 0.02589182, distance;
uint32_t setspeed=0;
bool displayreadsuccess = false;



/* Declare functions */
void initialize(void);      //Function for initializing the timer and interrupts
char displayreader(void);
void updatedata(void);
void PWM_Motor(int freq, int duty);
void PWM_on();
void PWM_off();

ISR(USART_RX_vect){

    scanf("%c", &readBuffer[readBufferindex]);

    if(stringbeginflag==false){//making sure that only first indicator gets detected
        if(readBuffer[readBufferindex]==rxexpect){
        
        readBufferindex=0;
        readBuffer[0]=rxexpect;
        stringbeginflag=true;

        }
    }

    readBufferindex++;
    if(readBufferindex==8){
    readBufferindex = 0;
    stringbeginflag = false;
    /*UCSR0B &= ~(1<<RXEN0);
    UCSR0B |= 1<<RXEN0;*/
    }
}

ISR(TIMER1_CAPT_vect){
    timer=ICR1+65535*counter;// Updating timer value
    //printf("Input Capture EVENT!!!"); line used for debugging
    TCNT1=0;                // Reseting the timer to zero
    TIFR1|=1<<ICF1;         // Reseting the input capture flag
    counter=0;              // Reseting overflow counter
    car_move_flag = true;   // Car is being moved
    distancecounter++;
    
}
ISR(TIMER1_OVF_vect){
    counter++;            // Adding one to the overflow counter
    TCNT1=0;
    if(counter>2){        // The car has not really moved for a long time
        car_move_flag = 0;// So the move-flag is reset
    }
}

int main(void) {    
    
    uart_init();   // Open the communication to the microcontroller
	io_redirect(); // Redirect input and output to the communication
    initialize();

    // Reseting all the values
    speed=0;
    prev_speed=0;
    acceleration_flag=0;
    car_move_flag = false;

    //printf("%lf", speed); 
        
       
        while(!(readBuffer[0]==0x65 && readBuffer[1]==0x01 && readBuffer[2]==0x09 && readBuffer[3]==0x00));
        
        
      
        
        //setting the speed
            
            //ask for data of secpag.x0
            rxexpect=0x71;//setting what string to expect

            
            
            
           /*for(i=0;i<8;i++){//stringreader for debugging purpose
                //printf("%c",savereadBuffer[0]);
                printf("%c",savereadBuffer[i]);
                if(savereadBuffer[i]==0x71)
                printf("Elements %d",i);
            }*/
            
            rxexpect=0x71;
            printf("get %s.val%c%c%c","secpag.x0",255,255,255);	//sends "get secpag.n0.val"
            _delay_ms(51);
            displayreader(); //saving the readbuffer from being changed
            if(readBuffer[0] == 0x71 && readBuffer[5] == 0xFF && readBuffer[6] == 0xFF && readBuffer[7] == 0xFF){
                
                setspeed = readBuffer[1] | (readBuffer[2] << 8) | (readBuffer[3] << 16)| (readBuffer[4] << 24);
                printf("secpag.x1.val=%ld%c%c%c", setspeed, 255,255,255);
                
            }

            //reading the pageid
           
            rxexpect=0x66;
            printf("sendme%c%c%c",255,255,255);
                

                while(1){
                   // printf("page1.n0.val=%d%c%c%c", 7, 255,255,255);
                    _delay_ms(500);
                    rxexpect=0x66;
                  //  printf("sendme");
                    _delay_ms(750);
                    currentpagenumber=readBuffer[1];
                  //  printf("page1.n0.val=%d%c%c%c", currentpagenumber, 255,255,255);
                    displayreader(); //saving the readbuffer from being changed
                    updatedata();
                    // Reading data out of readbuffer (Display)
                    if(readBuffer[0]==0x65 && readBuffer[1]==0x01 && readBuffer[2]==0x09 && readBuffer[3]==0x00)
                    //printf("secpag.n0.val=%d%c%c%c ",(test+223), 255,255,255);
                    


                    // Reading data out of the optocoupler
                    seconds = ((double)timer*1000)/15625000;    // Time calculation (Seconds)
                    // printf("\n debug: %f",seconds);
                    distance = (double)distancecounter*eigthcircumference;  // Distance calculation

                    // Speed calculation (optocoupler)
                    if (seconds){ // Speed is only recalculated when there is actually a timer-value (that is not zero)
                        speed = eigthcircumference/seconds; // Distance divided by time
                        printf("secpag.x1.val=%ld%c%c%c", (long int)(speed*1000), 255,255,255);
                        //printf("page2.speed.val=%ld%c%c%c", (long int)(speed*1000), 255,255,255);
                    }
                    
                    
                    // Check whether car is moving
                    if (car_move_flag){
                        //printf("\nCar is moving");
                    }

                    // If it is not moving - set speed etc. to zero
                    else if (car_move_flag == false){
                    // printf("\nCar is not moving.");
                        speed=0;
                        //printf("page2.speed.val=%d%c%c%c", 0, 255,255,255);
                        timer=0;
                        seconds=0;
                        prev_speed=0;
                    }

                    
                    
                    // Checking for acceleration
                    acceleration_flag = acceleration_index(speed, prev_speed);
                    

                    // output current data in console
                    // printf("\n This is the current state of the the timer:%lu and seconds:%lf - speed:%lf - accelerationflag:%d - distance:%lf",timer,seconds, speed, acceleration_flag, distance);
                    //_delay_ms(1000);

                    //printf("\n prev-speed:%f",prev_speed); debugging
                    prev_speed=speed; // Setting a new value for previous speed                               
                }
            return 0;
        }

/* Function description */
inline void initialize(void){
    sei();  // Enable global interrupts
    test = 12;
    // Usart interrupts
    SREG |= 1<<SREG_I;
    UCSR0B |=1<<RXCIE0; // Enabeling interrupt for rx complete
    
    TIMSK1 |= (1<<ICIE1)|(1<<TOIE1);    // Timer interrupts must be enabled
    TCCR1A = 0x00;
    TCCR1B = (1<<ICNC1)|/*(1<<ICES1)|*/(1<<CS12)|(1<<CS10);//noise cancel-/*falling*/ raising edge - 1024 prescaling
    DDRB &= ~0x01;//opto
    PORTB |= 0x01;
    TIFR1 |= 1<<ICF1;                   // Reseting input capture flag
}

inline char displayreader(void){
    for(i=0;i<100;i++){
        savereadBuffer[i] = readBuffer[i];
    }
}

char acceleration_index(double current_speed, double previous_speed){
    char acceleration_flag;
    if(current_speed == 0 && previous_speed == 0){
        acceleration_flag=0;
    }else if(current_speed < prev_speed){
        acceleration_flag=1;
    }
        
    if(current_speed > prev_speed){
        acceleration_flag = 2;
    }
    if(speed==0){
        acceleration_flag = 0;
        //printf("debug");
    }
    return acceleration_flag;
}

inline void PWM_Motor(int freq, int duty){  
    DDRD = 0x40;    // Set Port D as output for the ENA (Motor) 0b0010 0000
    TCCR0A |= (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);  // Fast PWM

    // Compare value
    ICR1 = (F_CPU/1024) - 1;
    OCR0A = (((F_CPU/freq/1024) - 1 )*duty)/100;
    OCR0B = (((F_CPU/freq/1024) - 1 )*duty)/100;
}

void PWM_on(){
    TCNT0 = 0X00;
    TCCR0B |= (1<<CS00) | (1<<WGM02);    // 1024 Prescaler
}

void PWM_off(){
    OCR0A = 0;
    OCR0B = 0;    
}

inline void updatedata(void){

   // printf("secpag.n0.val=%d%c%c%c", 7, 255,255,255);
  //  printf("speed.val=%ld%c%c%c", (long int)(speed*1000), 255,255,255);

}
