/*
*   D10 -> CS
*   D11 -> MOSI
*   D12 -> MISO
*   D13 -> SCK
*/

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

int main(void) {     
    uart_init();    // Open the communication to the micro controller
    io_redirect();  // Redirect input and output to the communication
    
    void SPI_MasterInit(void){
        /* Set MOSI and SCK output, all others input */
        DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
        /* Enable SPI, Master, set clock rate fck/16 */
        SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
    }

    void SPI_MasterTransmit(char cData){
        /* Start transmission */
        SPDR = cData;
        /* Wait for transmission complete */
        while(!(SPSR & (1<<SPIF)));
    }


    while (1){
        
    }
}