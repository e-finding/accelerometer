#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

#include <stdbool.h>

typedef struct {
    double x_g;
    double y_g;
    double z_g;
} accelerometer_sample_t;

void accelerometer_init(void);
bool accelerometer_getSample(accelerometer_sample_t *sample);

#endif