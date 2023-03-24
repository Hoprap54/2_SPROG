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

#define b0 0x3b
#define b1 0x37 

void moveF();
void moveB();

int main(void) {     
    /* Declaration of I/O Pins */
    DDRD = 0xF0;    // Set Port D as input for the buttons 0b1111 0000
    PORTD = 0x3F;   // Activate internall pull
    DDRB = 0xFF;    // Set Port B as output for the LN298N 0b1111 1111

    /* Declare variables */
    
    while (1){
        //button_state = PIND;
        switch (PIND){
        case b0:
            moveF();
            _delay_ms(10);
            break;
        
        case b1:
            moveB();            
            break;
        default:
            PORTB = 0X00;       // Set all output to 0
            break;
        }
    }
}

void moveF(){
    int i;
    char pos[4] = {0b0001, 0b0100, 0b1000, 0b0010};
    for(int j = 0; j < 50 ; j++){
        for(i = 0; i < 4 ; i++){
            PORTB = pos[i];
            _delay_ms(30);
        }
    }
}
void moveB(){
    int i;
    char pos[4] = {0b0001, 0b0100, 0b1000, 0b0010};
    for(i = 3; i >= 0 ; i--){
        PORTB = pos[i];
        _delay_ms(30);
    }
}