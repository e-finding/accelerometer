// adc.h header file
// sets up ADC pins and averages the channel read to reduce noise

#ifndef _ADC_H_
#define _ADC_H_

// sets the attenuation (redundant)
void adc_init(void);

// reads the channel and outputs an average reading to reduce noise
int adc_readChannel(int ch);

#endif