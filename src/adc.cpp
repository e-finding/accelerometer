#include "hal/adc.h"
#include <Arduino.h>

// constant by which the channel reading is averaged by 
#define AVG_NOISE 8

// use ADC1 pins only (ADC2 interfere with wifi)
static const int ADC_PINS[3] = {36, 39, 34}; // ch0=X, ch1=Y, ch2=Z

void adc_init(void)
{
    analogSetAttenuation(ADC_11db); // for 0-3.3 V range
}

int adc_readChannel(int ch) 
{
    // check that channel is valid
    if (ch < 0 || ch > 2) return -1;
    
    // average reading to reduce noise
    long sum = 0;
    for (int i = 0; i < AVG_NOISE; i++) {
        sum += analogReadMilliVolts(ADC_PINS[ch]);
    } 
    
    return sum / AVG_NOISE;
}