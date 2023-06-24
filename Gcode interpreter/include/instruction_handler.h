#include <stdio.h>
#include <stdlib.h>
#include <SPI.h>

#ifndef INSTRUCTION_HANDLER_H_INCLUDED
#define INSTRUCTION_HANDLER_H_INCLUDED

uint8_t has_letter(char, char *, uint8_t);
double extract_number(uint8_t, char *, uint8_t);
void get_pos_delta(char *, uint8_t);
void set_current_pos();
void get_center(char *, uint8_t);

void g_code_exec(uint8_t, char *, uint8_t);
void m_code_exec(uint8_t, char *, uint8_t);
void ins_exec(char *, uint8_t);
void dPadSignalProcessing(char incomingChar);
#endif