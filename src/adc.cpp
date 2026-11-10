#include "hal/adc.h"
#include <Arduino.h>

// use ADC1 pins only (ADC2 interfere with wifi)
static const int ADC_PINS[3] = {36, 39, 34}; // ch0=X, ch1=Y, ch2=Z

void adc_init(void)
{
    analogReadResolution(12); // 0-4095
    analogSetAttenuation(ADC_11db); // for 0-3.3 V range
}

int adc_readChannel(int ch) 
{
    if (ch < 0 || ch > 2) return -1;
    
    long sum = 0;

    // average for noise
    for (int i = 0; i < 16; i++) {
        sum += analogRead(ADC_PINS[ch]);
    } 
    
    return sum / 16;
}