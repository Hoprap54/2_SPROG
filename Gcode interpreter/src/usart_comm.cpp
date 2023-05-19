#include "usart_comm.h"

void usart_init(unsigned int baudrate){
    unsigned long baud_prescaler = (F_CPU / (baudrate * 16UL)) - 1;
    UBRR0H = (uint8_t)(baud_prescaler >> 8);
    UBRR0L = (uint8_t)(baud_prescaler);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

void usart_send_char(char data){
    while(!(UCSR0A & (1 << UDRE0))); // Wait for transmit buffer
    UDR0 = data; // Data to be sent
}

char usart_receive_char(){
    while(!(UCSR0A & (1 << RXC0))); // Wait for new data
    return UDR0; // Received data
}

void usart_send_string(char str[], uint8_t size){
    for(uint8_t i = 0; i < size; i++){
        usart_send_char(str[i]);
    }
}

uint8_t usart_receive_string(char str[]){
    uint8_t size = usart_receive_char();
    for(uint8_t i = 0; i < size; i++){
        *(str + i) = usart_receive_char();
    }
}