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

// SPI port available at port B
#define SPI_SCK  5  // 13
#define SPI_MISO 4  // 12
#define SPI_MOSI 3  // 11
#define SPI_CS   2  // 10

void SPI_MasterInit(void){
    /* Set MOSI and SCK output, all others input */
    PORTB = (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_CS);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPIE)|(1<<SPE)|(1<<MSTR)|(1<<SPR0);
    SPSR = 0x00;    
}

int main(void) {     
    uart_init();    // Open the communication to the micro controller
    io_redirect();  // Redirect input and output to the communication
        
    DDRB = (1<<SPI_CS); // Set CS High
    SPI_MasterInit();
    SPDR = 0x00;
    while (1){
        char d = SPDR;
        printf("\n Data: %s", d);
    }
}