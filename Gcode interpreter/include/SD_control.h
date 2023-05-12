#include <avr/io.h>
#include <SPI.h>
#include <SD.h>

#ifndef SD_FILE_H_INCLUDED
#define SD_FILE_H_INCLUDED

void SD_start(unsigned int);
uint8_t file_ready();
uint8_t file_open(char *);
void file_close();
void file_read_line(char *);
unsigned char line_size(char *);

#endif