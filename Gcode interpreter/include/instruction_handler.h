#include <stdio.h>
#include <stdlib.h>
#include <SPI.h>

#ifndef INSTRUCTION_HANDLER_H_INCLUDED
#define INSTRUCTION_HANDLER_H_INCLUDED

uint8_t has_letter(char, char *, uint8_t);
double extract_number(uint8_t, char *, uint8_t);
void get_pos(char *, uint8_t);
void get_center();
void swap(uint8_t *, uint8_t *);

void g_codes_exec(char *, uint8_t);
void m_codes_exec(char *, uint8_t);
void ins_exec();

#endif