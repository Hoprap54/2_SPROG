#include <avr/io.h>

void ADC_init(void);
void X_direction(void);
void Y_direction(void);
void Z_direction(void);
bool E_enable(void);
uint16_t adc_read(uint8_t adc_channel);
void ADC_init(void);