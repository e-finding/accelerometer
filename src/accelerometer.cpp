#include "hal/accelerometer.h"
#include "hal/adc.h"
#include <Arduino.h>

// esp32 ADC channels 
#define X_CHANNEL           0
#define Y_CHANNEL           1
#define Z_CHANNEL           2

// voltage  
#define ZERO_G_VOLTAGE_X    1.65
#define ZERO_G_VOLTAGE_Y    1.65
#define ZERO_G_VOLTAGE_Z    1.65

// sensitivity 
#define SENSITIVITY_PER_G   0.35 

// helper function to convert millivolts to G
static double mvToZeroGVolts(int millivolts, double zeroGVolts)
{
    double voltage = millivolts / 1000.0;
    return (voltage - zeroGVolts) / SENSITIVITY_PER_G;
}

// initialze ADC
void accelerometer_init(void)
{
    adc_init();
}

// gets a sample from the accelerometer via ADC pins
// converts millivolts to voltage clalibrated to be zero g at 0
bool accelerometer_getSample(accelerometer_sample_t *sample)
{
    // check a sample is returned
    if (sample == NULL) return false;

    // get sample accelerations (in millivolts)
    int x = adc_readChannel(X_CHANNEL);
    int y = adc_readChannel(Y_CHANNEL);
    int z = adc_readChannel(Z_CHANNEL);

    // check ADC read correctly
    if ((x == -1)|| (y == -1) || (z == -1)) { 
        Serial.println("Accelerometer - ADC failed to read"); 
        return false;
    }

    // convert millivolts to corrected zero g voltage
    sample->x_g = mvToZeroGVolts(x, ZERO_G_VOLTAGE_X);
    sample->y_g = mvToZeroGVolts(y, ZERO_G_VOLTAGE_Y);
    sample->z_g = mvToZeroGVolts(z, ZERO_G_VOLTAGE_Z);

    return true;
}
