#include "hal/accelerometer.h"
#include "hal/adc.h"
#include <Arduino.h>

// magic numbers
#define X_CHANNEL           0
#define Y_CHANNEL           1
#define Z_CHANNEL           2
#define MAX_COUNT           4095.0
#define VREF                3.3 // Volts

// span is 1400 - 2200 from -1g to 1g, so 400/g
// sensitivity = 400 / 4095 * 3.3 = 0.322 V
// zero g = 1800 / 4095 * 3.3 = 1.451 V
#define ZERO_G_VOLTAGE_X    1.451
#define ZERO_G_VOLTAGE_Y    1.451
#define ZERO_G_VOLTAGE_Z    1.451
#define SENSITIVITY_PER_G   0.39 

// helper function
static double countsToG(int counts, double zeroGVolts)
{
    double voltage = (counts / MAX_COUNT) * VREF;
    return (voltage - zeroGVolts) / SENSITIVITY_PER_G;
}


void accelerometer_init(void)
{
    adc_init(); // set up ADC pins
}

bool accelerometer_getSample(accelerometer_sample_t *sample)
{
    if (sample == NULL) return false;

    // get sample accelerations
    int x = adc_readChannel(X_CHANNEL);
    int y = adc_readChannel(Y_CHANNEL);
    int z = adc_readChannel(Z_CHANNEL);

    // check adc read correctly
    if ((x == -1)|| (y == -1) || (z == -1)) { 
        Serial.println("Accelerometer - ADC failed to read"); 
        return false;
    }

    // convert digital counts to voltage
    sample->x_g = countsToG(x, ZERO_G_VOLTAGE_X);
    sample->y_g = countsToG(y, ZERO_G_VOLTAGE_Y);
    sample->z_g = countsToG(z, ZERO_G_VOLTAGE_Z);

    return true;
}
