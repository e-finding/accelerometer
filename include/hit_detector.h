// hit_detector.h header file
// analyses accelerometer samples to detect hits

#ifndef _HIT_DETECTOR_H_
#define _HIT_DETECTOR_H_

// initializes accelerometer.h and gets calibration samples 
// to create the baseline (accelerometer when there is no hit)
void hitDetector_init(void);

// gets sample, converts to magnitude, subtracts the baseline to get the change
// if its 'quiet' or still it updates the baseline
// if it can detect a hit it updates the hitcount and initializes all varibles 
// for cooldown period
// if it cannot detect a hit (in cooldown) then it checks if its been at rest long 
// enough or if the cooldown time has expired
void hitDetector_update(void);

// returns hit count and resets it
int hitDetector_hit(void);

#endif