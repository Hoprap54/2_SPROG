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

void moveF();
void moveB();

int main(void) {     
    /* Declaration of I/O Pins */
    DDRD = 0xF0;    // Set Port D as input for the buttons 0b1111 0000
    PORTD = 0x3F;   // Activate internall pull
    DDRB = 0xFF;    // Set Port B as output for the LN298N 0b1111 0000

    /* Declare variables */
    
    while (1){
        //button_state = PIND;
        switch (PIND)
        {
        case b0:
            moveF();            
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
    char pos[4] = {0b0101, 0b0110, 0b1010, 0b1001};
    for(int j = 0; j < 51 ; j++){
        for(i = 0; i < 4 ; i++){
            PORTB = pos[i];
            _delay_ms(5);
        }
    }
}
void moveB(){
    int i;
    char pos[4] = {0b0101, 0b0110, 0b1010, 0b1001};
    for(i = 3; i >= 0 ; i--){
        PORTB = pos[i];
        _delay_ms(5);
    }
}