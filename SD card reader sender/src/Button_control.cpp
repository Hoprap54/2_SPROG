#define ADC_P0 0
#define ADC_P1 1
#define ADC_P2 2

#include "Button_control.h"
#include <avr/io.h>
#include "usart_comm.h"

uint16_t adc_x;
uint16_t adc_y;
uint16_t adc_z;
uint16_t adc_e;
uint8_t x_dir;
uint8_t y_dir;
uint8_t z_dir;
uint8_t e_enable;
uint8_t e_prestate = 0;

void ADC_init(void)
{
    // Select Vref = AVcc
    ADMUX = (1 << REFS0);
    // set prescaler to 128 and turn on the ADC module
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
    DDRD = 0x0F; // Declare Outuput for the LED
}
uint16_t adc_read(uint8_t adc_channel)
{
    ADMUX &= 0xf0;        // clear any previously used channel, but keep internal reference
    ADMUX |= adc_channel; // set the desired channel
    // start a conversion
    ADCSRA |= (1 << ADEN) | (1 << ADSC);
    // Wait for the conversion to complete
    while ((ADCSRA & (1 << ADSC)))
        ;
    // We have the result, so we return it to the calling function as a 16 bit unsigned int
    return ADC;
}
void X_direction(void)
{
    adc_x = adc_read(ADC_P1);
    if (adc_x == 0)
    {
        x_dir = 1; // X_Right
        usart_send_char('X');
    }
    else if (adc_x < 685 && adc_x > 679)
    {
        x_dir = 2; // X_Left
        usart_send_char('x');
    }
    else if (adc_x == 1023)
    {
        x_dir = 0; // Button is not being pressed
    }
}
void Y_direction(void)
{
    adc_y = adc_read(ADC_P0);
    if (adc_y == 0)
    {
        y_dir = 1; // Y_down
        usart_send_char('y');
    }
    else if (adc_y < 515 && adc_y > 510)
    {
        y_dir = 2; // Y_up
        usart_send_char('Y');
    }
    else if (adc_y == 1023)
    {
        y_dir = 0; // Button is not being pressed
    }
}
void Z_direction(void)
{
    adc_z = adc_read(ADC_P2);
    if (adc_z == 0)
    {
        z_dir = 1; // Z_down
    }
    else if (adc_z < 513 && adc_z > 505)
    {
        z_dir = 2; // Z_up
    }
    else if (adc_z == 1023)
    {
        z_dir = 0; // Button is not being pressed
    }
}
bool E_enable(void)
{
    if (adc_e < 685 && adc_e > 679 && e_prestate == 0)
    {
        e_enable = 1; // Enable
        e_prestate = e_enable;
        return 0;
    }
    else if (adc_x == 1023 && e_prestate == 1)
    {
        e_enable = 0; // Button is not being pressed
        e_prestate = e_enable;
        return 1;
    }
    return 0;
}
