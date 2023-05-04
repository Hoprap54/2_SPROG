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
#include "subfunctions.h"
void SPI_init();
void SD_powerUpSeq();
uint8_t SD_goIdleState();

int main(void){ 
    // array to hold responses
    uint8_t res[5];

    // initialize UART
    UART_init(57600);

    // initialize SPI
    SPI_init();

    // start power up sequence
    SD_powerUpSeq();

    // command card to idle
    UART_puts("Sending CMD0...\r\n");
    res[0] = SD_goIdleState();
    UART_puts("Response:\r\n");
    SD_printR1(res[0]);

    // send if conditions
    UART_puts("Sending CMD8...\r\n");
    SD_sendIfCond(res);
    UART_puts("Response:\r\n");
    SD_printR7(res);

    while(1){}
}