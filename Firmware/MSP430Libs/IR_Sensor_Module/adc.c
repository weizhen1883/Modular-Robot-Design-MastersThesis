#include <msp430.h>
#include <stdint.h>
#include "adc.h"

void init_adc(void) {
    uint16_t adc10ctl0 = 0x0000;
    adc10ctl0 |= SREF_0; /* VR+ = VCC & VR- = VSS */
    adc10ctl0 |= ADC10SHT_2; /* 16 clock sample-and-hold time */
    adc10ctl0 |= ADC10ON; /* enable ADC */
    ADC10CTL0 = adc10ctl0;
}

/* 10-bit ADC will return max of 0x3FF */
uint16_t adc_convert(uint8_t pin) {

    ADC10CTL1 = (pin << 12);
    ADC10AE0 = (1 << pin); /* enable analog input */

    ADC10CTL0 |= (ENC | ADC10SC); /* enable & start conversion */
    while(ADC10CTL1 & ADC10BUSY); /* wait for conversion to complete */
    ADC10CTL0 &= ~ENC; /* disable conversion */

    return(ADC10MEM); /* converted value */
}
