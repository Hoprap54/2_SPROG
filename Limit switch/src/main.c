#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char n;

int main(void){
    uart_init();   // Open the communication to the micro controller
    io_redirect(); // Redirect input and output to the communication

    DDRD = 0x00; // Clear the PD2 pin
    // PD2 (INT0 pin) is now an input
    PORTD |= 0x3F; // turn On the Pull-up
    // PD2 is now an input with pull-up enabled (MIGHT NOT BE NEEDED)
    EIMSK |= (1 << INT1) | (1 << INT0); // Turns on interrupt for INT0
    sei(); // turn on interrupts
    while (1){}
}

ISR (INT0_vect){ // External interrupt 0
    printf("L1");
}

ISR (INT1_vect){ // External interrupt 1
    printf("L2");
}