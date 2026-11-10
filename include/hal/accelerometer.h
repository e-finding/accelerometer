// accelerometer.h header file
// retrieves x,y,z samples from the acceleromter

#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

#include <stdbool.h>

// holds accelerometer axis values for one sample
typedef struct {
    double x_g;
    double y_g;
    double z_g;
} accelerometer_sample_t;

// inialize the accelerometer
// calls the ADC module 
void accelerometer_init(void);

// returns a sample from the accelerometer in zero g volts
bool accelerometer_getSample(accelerometer_sample_t *sample);

#endif