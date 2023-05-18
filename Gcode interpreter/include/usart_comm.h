#include <avr/io.h>

#ifndef USART_COMM_H_INCLUDED
#define USART_COMM_H_INCLUDED

void usart_init(unsigned int);
void usart_send_char(char);
char usart_receive_char();

#endif