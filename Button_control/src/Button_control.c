/*
 * LED
 * PD2 -> G
 * PD3 -> R
 */
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include "usart.h"

#define ADC_P0 0 // what is the ADC channel we’ll use
#define ADC_P1 1 // what is the ADC channel we’ll use
#define ADC_P2 2 // what is the ADC channel we’ll use

void ADC_init(void);
uint16_t adc_read(uint8_t adc_channel);
uint16_t adc_x;
uint16_t adc_y;
uint16_t adc_z;
uint16_t adc_e;
uint8_t x_dir;
uint8_t y_dir;
uint8_t z_dir;
uint8_t e_enable;
uint8_t e_prestate = 0;

void X_direction(void);
void Y_direction(void);
void Z_direction(void);
void E_enable(void);

int main()
{
    uart_init();   // Open the communication to the micro controller
    io_redirect(); // Redirect input and output to the communication
    ADC_init();
    while (1)
    {
        // E_enable();
        // X_direction();
        // Y_direction();
        // Z_direction();
        adc_x = adc_read(ADC_P1);
        adc_y = adc_read(ADC_P0);
        adc_z = adc_read(ADC_P2);

        printf("adc_x = %d; adc_y = %d; adc_z = %d \n", adc_x, adc_y, adc_z);

        // if (adc_z > 670 && adc_z < 700 && adc_y > 950 && adc_x > 950)
        // {
        //     printf("Enable \n");
        // }
        // if (adc_x < 20 && adc_y > 950)
        // {
        //     printf("Right \n");
        // }
        // else if (adc_x > 660 && adc_x < 710 && adc_y > 950)
        // {
        //     printf("Left \n");
        // }
        // else if (adc_x > 950 && adc_y > 490 && adc_y < 530)
        // {
        //     printf("Forward \n");
        // }
        // else if (adc_x > 950 && adc_y < 50)
        // {
        //     printf("Backward \n");
        // }
        // else if (adc_x < 50 && adc_y > 480 && adc_y < 550)
        // {
        //     printf("Forward Right \n");
        // }
        // else if (adc_x < 30 && adc_y < 30)
        // {
        //     printf("Backward Right \n");
        // }
        // else if (adc_x > 660 && adc_x < 710 && adc_y < 30)
        // {
        //     printf("Backward Left \n");
        // }
        // else if (adc_x > 660 && adc_x < 720 && adc_y > 480 && adc_y < 550)
        // {
        //     printf("Forward Left \n");
        // }

        // if (adc_z > 480 && adc_z < 530 && adc_y > 700 && adc_x > 700)
        // {
        //     printf("Enable \n");
        // }
        // if (adc_x < 20 && adc_y > 470 && adc_y < 520)
        // {
        //     printf("Right \n");
        // }
        // else if (adc_x > 490 && adc_x < 520 && adc_y > 700)
        // {
        //     printf("Left \n");
        // }
        // else if (adc_x > 640 && adc_x < 680 && adc_y > 300 && adc_y < 360)
        // {
        //     printf("Forward \n");
        // }
        // else if (adc_x > 460 && adc_x < 520 && adc_y < 50)
        // {
        //     printf("Backward \n");
        // }
        // else if (adc_x < 50 && adc_y > 180 && adc_y < 230)
        // {
        //     printf("Forward Right \n");
        // }
        // else if (adc_x < 30 && adc_y < 30)
        // {
        //     printf("Backward Right \n");
        // }
        // else if (adc_x > 250 && adc_x < 310 && adc_y < 30)
        // {
        //     printf("Backward Left \n");
        // }
        // else if (adc_x > 340 && adc_x < 380 && adc_y > 240 && adc_y < 290)
        // {
        //     printf("Forward Left \n");
        // }

        // printf("\nE:%d  x:%d    y:%d    z:%d", e_enable, x_dir, y_dir, z_dir);
        _delay_ms(50);
    }
}

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
    // printf("%d ", adc_x);
    // printf("x %d; ", adc_x);

    // if (adc_x < 50u && adc_y > 480u && adc_y < 520u)
    // {
    //     printf("Right ");
    // }
    // if (adc_x > 700)
    // {
    //     // not being pressed
    // }
    // else if (adc_x < 5)
    // {
    //     // pressed
    //     printf("Right ");
    // }
    // else if (adc_x > 450 && adc_x < 550)
    // {
    //     printf("Left ");
    // }

    //     if (adc_x == 0)
    // {
    //     x_dir = 1; // X_Right
    //     printf("Right");
    // }
    // else if (adc_x < 685 && adc_x > 679)
    // {
    //     x_dir = 2; // X_Left
    //     printf("Left");
    // }
    // else if (adc_x == 1023)
    // {
    //     x_dir = 0; // Button is not being pressed
    // }
}
void Y_direction(void)
{
    adc_y = adc_read(ADC_P0);
    // printf("y %d; \n", adc_y);
    // if (adc_y > 700)
    // {
    //     // not being pressed
    // }
    // else if (adc_y < 5)
    // {
    //     // pressed
    //     printf("BackWard ");
    // }
    // else if (adc_y > 300 && adc_y < 350)
    // {
    //     printf("Forward ");
    // }

    // if (adc_y == 0)
    // {
    //     y_dir = 1; // Y_down
    //     printf("Backward");
    // }
    // else if (adc_y < 515 && adc_y > 510)
    // {
    //     y_dir = 2; // Y_up
    //     printf("Forward");
    // }
    // else if (adc_y == 1023)
    // {
    //     y_dir = 0; // Button is not being pressed
    // }
}
void Z_direction(void)
{
    adc_z = adc_read(ADC_P2);

    if (adc_z > 700)
    {
        // not being pressed
    }
    else if (adc_z < 5)
    {
        // pressed
        printf("Down ");
    }
    else if (adc_z > 300 && adc_z < 350)
    {
        printf("Up ");
    }
    //     if (adc_z == 0)
    //     {
    //         z_dir = 1; // Z_down
    //         printf("Down");
    //     }
    //     else if (adc_z < 513 && adc_z > 505)
    //     // else if (adc_z < 700 && adc_z > 100)
    //     {
    //         z_dir = 2; // Z_up
    //         printf("Up");
    //     }
    //     else if (adc_z == 1023)
    //     {
    //         z_dir = 0; // Button is not being pressed
    //     }
}
void E_enable(void)
{
    if (adc_e < 685 && adc_e > 679 && e_prestate == 0)
    {
        e_enable = 1; // Enable
        e_prestate = e_enable;
        printf("Enabled");
    }
    else if (adc_x == 1023 && e_prestate == 1)
    {
        e_enable = 0; // Button is not being pressed
        e_prestate = e_enable;
    }
}