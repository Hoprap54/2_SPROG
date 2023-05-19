#include "SD_control.h"
#include "usart_comm.h"

#define chip_select 10

void send_file(char []);

void setup(){
    // Start SD card
    SD_start(chip_select);

    char file1[] = "square.txt";
    char file2[] = "triangle.txt";

    send_file(file1);
}

void loop(){
    // Necessary loop for arduino
}

void send_file(char file[]){
    file_open(file);

    while(file_ready()){
        char instruction[75] = "";

        if(file_read_ins(instruction)){
            uint8_t ins_size = line_size(instruction);
            usart_send_char(ins_size);
            for(uint8_t i = 0; i < ins_size; i++){
                usart_send_char(instruction[i]);
            }
        }
        else{
            break;
        }
    }
}