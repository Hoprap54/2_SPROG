#include <avr/io.h>

#ifndef USART_COMM_H_INCLUDED
#define USART_COMM_H_INCLUDED

void usart_init(unsigned int);
void usart_send_char(char);
char usart_receive_char();
void usart_send_string(char *, uint8_t);
void usart_receive_string(char *, uint8_t);

#endif