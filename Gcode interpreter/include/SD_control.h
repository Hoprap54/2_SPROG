#include <avr/io.h>
#include <SPI.h>
#include <SD.h>

#ifndef SD_FILE_H_INCLUDED
#define SD_FILE_H_INCLUDED

void SD_start(unsigned int);
void file_read_line(File, char *);
unsigned char line_size(char *);

#endif