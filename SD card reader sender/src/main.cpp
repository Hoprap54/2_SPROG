#include "SD_control.h"
#include "usart_comm.h"
#include "Button_control.h"

// Chipselect for SS pin(???) - I'm not sure, but I know the SD card library needs it
#define chip_select 10
#define BAUD 9600
#define ADC_P0 0
#define ADC_P1 1
#define ADC_P2 2

bool mode = 1; // 1 = D-pad; 0 = gcode.
uint16_t x = 0;
uint16_t y = 0;
uint16_t z = 0;

void send_file(char[]);

// Main function
void setup()
{
    // Start SD card
    // Serial.begin(BAUD);
    SD_start(chip_select);
    usart_init(BAUD);
    ADC_init();

    DDRC |= (1 << PC3) | (1 << PC4);
    PORTC |= 1 << PC3;
    // DDRC = 0xFF;
    // PORTC = 0xFF;
    // _delay_ms(10000);
    char now_char = 's';
    char last_char = 's';
    while (mode)
    {
        x = adc_read(ADC_P1);
        y = adc_read(ADC_P0);
        z = adc_read(ADC_P2);

        // printf("adc_x = %d; adc_y = %d; adc_z = %d \n", adc_x, adc_y, adc_z);
        if (z > 670 && z < 700 && y > 950 && x > 950)
        {
            // printf("Enable \n");
            mode = 0;
            PORTC &= ~(1 << PC3);
            PORTC |= 1 << PC4;
        }
        else if (x < 20 && y > 950)
        {
            // printf("Right \n");
            // usart_send_char('r');
            now_char = 'r';
        }
        else if (x > 660 && x < 710 && y > 950)
        {
            // printf("Left \n");
            // usart_send_char('l');
            now_char = 'l';
        }
        else if (x > 950 && y > 490 && y < 530)
        {
            // printf("Forward \n");
            // usart_send_char('f');
            now_char = 'f';
        }
        else if (x > 950 && y < 50)
        {
            // printf("Backward \n");
            // usart_send_char('b');
            now_char = 'b';
        }
        else if (x < 50 && y > 480 && y < 550)
        {
            // printf("Forward Right \n");
            // usart_send_char('c');
            now_char = 'c';
        }
        else if (x < 30 && y < 30)
        {
            // printf("Backward Right \n");
            // usart_send_char('d');
            now_char = 'd';
        }
        else if (x > 660 && x < 710 && y < 30)
        {
            // printf("Backward Left \n");
            // usart_send_char('e');
            now_char = 'e';
        }
        else if (x > 660 && x < 720 && y > 480 && y < 550)
        {
            // printf("Forward Left \n");
            // usart_send_char('g');
            now_char = 'g';
        }
        else{ // If no buttons are active, send 's' for stop
            now_char = 's';
        }

        if(now_char != last_char){ // If command has changed, send change
            usart_send_char(now_char);
            last_char = now_char;
        }
    }

    usart_send_char(1);

    char file_exp[] = "thecube.txt";
    char file1[] = "square.txt";
    char file2[] = "triangle.txt";

    send_file(file2);
    _delay_ms(5000);
    send_file(file1);

    usart_send_char(0);
}

void loop()
{
    // Necessary loop for arduino
}

void send_file(char file[])
{
    if (file_open(file))
    { // Open file specified

        while (file_ready())
        {                              // If file is available
            char instruction[75] = ""; // Prepare for 1 instruction

            if (file_read_ins(instruction))
            {                                              // read instruction and if read succesfully
                uint8_t ins_size = line_size(instruction); // Determine size of instruction
                usart_send_char(ins_size);                 // Send instruction size
                usart_send_string(instruction, ins_size);  // Send instruction
                usart_receive_char();                      // Wait for confirmation
            }
            else
            {
                break;
            }
        }
        file_close();
    }
}