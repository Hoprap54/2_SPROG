#include "SD_control.h"
#include "usart_comm.h"

// Chipselect for SS pin(???) - I'm not sure, but I know the SD card library needs it
#define chip_select 10
#define BAUD 9600

void send_file(char[]);

// Main function
void setup()
{
    // Start SD card
    // Serial.begin(BAUD);
    SD_start(chip_select);
    usart_init(BAUD);

    char file_exp[] = "thecube.txt";
    char file1[] = "square.txt";
    char file2[] = "triangle.txt";

    send_file(file1);
    _delay_ms(5000);
    send_file(file2);
}

void loop()
{
    // Necessary loop for arduino
}

void send_file(char file[])
{
    file_open(file); // Open file specified

    while (file_ready())
    {                              // If file is available
        char instruction[75] = ""; // Prepare for 1 instruction

        if (file_read_ins(instruction))
        {                                              // read instruction and if read succesfully
            uint8_t ins_size = line_size(instruction); // Determine size of instruction
            usart_send_char(ins_size);                 // Send instruction size
            // printf("%s\n", instruction);
            usart_send_string(instruction, ins_size); // Send instruction
            usart_receive_char();                      // Wait for confirmation
        }
        else
        {
            break;
        }
    }
    file_close();
}