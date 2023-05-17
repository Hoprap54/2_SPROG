#include <avr/io.h>
#include <SPI.h>
#include <SD.h>

#ifndef SD_FILE_H_INCLUDED
#define SD_FILE_H_INCLUDED

void SD_start(unsigned int);
uint8_t file_ready();
uint8_t file_open(char *);
void file_close();
uint8_t file_read_ins(char *);
unsigned char line_size(char *);

#endif