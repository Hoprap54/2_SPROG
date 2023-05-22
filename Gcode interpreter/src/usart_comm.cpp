#include "usart_comm.h"

void usart_init(unsigned int baudrate){
    unsigned long baud_prescaler = (F_CPU / (baudrate * 16UL)) - 1; // Calculate baud prescaler
    UBRR0H = (uint8_t)(baud_prescaler >> 8);                        // High byte write of baud prescaler
    UBRR0L = (uint8_t)(baud_prescaler);                             // Low byte write of baud prescaler
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);   // Enable TX and RX for write and read
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // 8-bit data, 1 stop bit, no parity bit
}

void usart_send_char(char data){
    while(!(UCSR0A & (1 << UDRE0))); // Wait for transmit buffer to be empty
    UDR0 = data; // Input data to be sent
}

char usart_receive_char(){
    while(!(UCSR0A & (1 << RXC0))); // Wait for new data to arrive - BLOCKS CODE IF NONE ARRIVE
    return UDR0; // Return received data
}

void usart_send_string(char *str, uint8_t size){ // Send array str of size
    for(uint8_t i = 0; i < size; i++){ // Send every character
        usart_send_char(str[i]);
    }
}

void usart_receive_string(char *str, uint8_t size){ // Receive array str of size
    for(uint8_t i = 0; i < size; i++){ // Receive every character to array
        *(str + i) = usart_receive_char();
    }
}