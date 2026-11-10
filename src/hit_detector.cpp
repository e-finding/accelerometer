#include "hit_detector.h"
#include "hal/accelerometer.h"
#include <Arduino.h>
#include <math.h>

// calibrate the baseline
#define CALIBRATION_PERIOD_MS       5 // time in ms between samples  
#define CALIBRATION_SAMPLES         50 // number of samples taken to form baseline
#define BASELINE_UPDATE_THRESHOLD   0.15 // tolerance from baseline it has to be to affect avg
#define BASELINE_ALPHA              0.001 // how much its weighted against the avg

// threshold for detecting hits
#define HIT_THRESHOLD_G             0.3 // threshold for what delta is a 'hit'
#define QUIET_THRESHOLD             0.05 // threshold for delta is considered within baseline
#define QUIET_SAMPLES               100 // how many samples it stays disarmed
#define COOLDOWN_MS                 150 // time in ms that has to pass between detected hits

// initialize local variables
static double baseline = 1.0;
static bool armed = true;
static int quietCount = 0;
static unsigned long lastHitMs = 0;
static int hitCount = 0;

// helper function that calculates the magnitude (abs position)
// of the accelerometer 
static double magnitude(const accelerometer_sample_t *s)
{
    return sqrt(s->x_g*s->x_g + s->y_g*s->y_g + s->z_g*s->z_g);
}

// initalize ADC, calibrate  baseline from samples
void hitDetector_init(void)
{
    accelerometer_init();
    
    accelerometer_sample_t s;
    double sum = 0;
    
    for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
        if (accelerometer_getSample(&s)) {
            sum += magnitude(&s);
        }
        delay(CALIBRATION_PERIOD_MS);
    }

    baseline = sum / CALIBRATION_SAMPLES;
    lastHitMs = millis() - COOLDOWN_MS; // do this so cooldown check passes immediately
    hitCount = 0;
}

// flow for detecting hits and updating baseline 
void hitDetector_update(void) 
{
    accelerometer_sample_t s;
    if (!accelerometer_getSample(&s)) return;

    double mag = magnitude(&s);
    double delta = fabs(mag - baseline); // change in mag from baseline
    unsigned long now = millis();

    // check if change is small enough to update baseline
    if (delta < BASELINE_UPDATE_THRESHOLD)
        baseline += BASELINE_ALPHA * (mag - baseline);

    // check if a hit can be detected
    if (armed) {
        // check if the disturbance is greater than our hit threshold
        // and if its been long enough since our last hit
        if (delta > HIT_THRESHOLD_G && (now - lastHitMs) >= COOLDOWN_MS) {
            hitCount++;
            lastHitMs = now;
            armed = false;
            quietCount = 0;
        }
    // if not then check if its more at rest
    // needs consequtive samples under the threshold
    } else {
        if (delta < QUIET_THRESHOLD) {
            if (++quietCount >= QUIET_SAMPLES) armed = true;
        } else {
            quietCount = 0;
        }
    }
}

// outputs hits
int hitDetector_hit(void)
{
    int n = hitCount;
    hitCount = 0;
    return n;
}
