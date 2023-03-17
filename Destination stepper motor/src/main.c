/*
*   D2 -> B.R.
*   D3 -> B.L.
*   ...
*   D8 -> In1
*   D9 -> In2
*   D10 -> In3
*   D11 -> In4
*/

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define b0 0x3b
#define b1 0x37 

// Function prototypes

// Global variables
char pat[4] = {0b0101, 0b0110, 0b1010, 0b1001};
unsigned char step = 1;
char dir = 0;


int main(void) {     
    /* Declaration of I/O Pins */
    DDRD = 0xF0;    // Set Port D as input for the buttons 0b1111 0000
    PORTD = 0x3F;   // Activate internall pull
    DDRB = 0xFF;    // Set Port B as output for the LN298N 0b1111 0000
    

    while(1){
        switch(PIND){
            case b0:
                step++;
                break;
            
            case b1:
                step--;
                break;
        }

        if(step > 200){
            if(dir > 0) step = 1;
            if(dir < 0) step = 200;
        }

        PORTB = pat[step % 4];
        _delay_ms(5);
    }
}