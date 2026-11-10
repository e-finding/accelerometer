#include "hit_detector.h"
#include "hal/accelerometer.h"
#include <Arduino.h>
#include <math.h>

// magic numbers
#define SAMPLING_PERIOD_MS          5
#define CALIBRATION_SAMPLES         50
#define HIT_THRESHOLD_G             0.3 // threshold for what is a 'hit'
#define QUIET_THRESHOLD             0.05 // threshold for what is considered baseline again
#define QUIET_SAMPLES               100 // how many samples it stays disarmed
#define MAX_DISARM_TIME             250
#define BASELINE_UPDATE_THRESHOLD   0.15 // tolerance from baseline it has to be to affect avg
#define BASELINE_ALPHA              0.001
#define COOLDOWN_MS                 100

// initialize local var
static double baseline = 1.0;
static bool armed = true;
static int quietCount = 0;
static unsigned long lastHitMs = 0;
static int hitCount = 0;


static double magnitude(const accelerometer_sample_t *s)
{
    return sqrt(s->x_g*s->x_g + s->y_g*s->y_g + s->z_g*s->z_g);
}


void hitDetector_init(void)
{
    accelerometer_init();
    
    accelerometer_sample_t s;
    double sum = 0;
    
    for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
        if (accelerometer_getSample(&s)) {
            sum += magnitude(&s);
        }
        delay(SAMPLING_PERIOD_MS);
    }

    baseline = sum / CALIBRATION_SAMPLES;
    lastHitMs = millis() - COOLDOWN_MS;
    hitCount = 0;
}

void hitDetector_update(void) 
{
    accelerometer_sample_t s;
    if (!accelerometer_getSample(&s)) return;

    double mag = magnitude(&s);
    double delta = fabs(mag - baseline);
    unsigned long now = millis();

    // check if change is small enough to update baseline
    if (delta < BASELINE_UPDATE_THRESHOLD)
        baseline += BASELINE_ALPHA * (mag - baseline);

    // check if its been long enough to record a hit
    if (armed) {
        if (delta > HIT_THRESHOLD_G && (now - lastHitMs) >= COOLDOWN_MS) {
            hitCount++;
            lastHitMs = now;
            armed = false;
            quietCount = 0;
        }
    } else if (now - lastHitMs >= MAX_DISARM_TIME) {
        armed = true;

    } else {
        if (delta < QUIET_THRESHOLD) {
            if (++quietCount >= QUIET_SAMPLES) armed = true;
        } else {
            quietCount = 0;
        }
    }
}

int hitDetector_hit(void)
{
    int n = hitCount;
    hitCount = 0;
    return n;
}
