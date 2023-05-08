#include <stdio.h>
#include <SD.h>

#ifndef SD_FILE_H_INCLUDED
#define SD_FILE_H_INCLUDED

void SD_start(unsigned int);
unsigned char file_read_line(File, char *);

#endif