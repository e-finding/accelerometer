#include <Arduino.h>
#include "hit_detector.h"
#include "hal/adc.h"
#include "hal/accelerometer.h"

// game logic
#define POINTS_PER_HIT 1
static int score = 0;

void setup() 
{
  Serial.begin(9600);
  hitDetector_init();
}

void loop() 
{
  static unsigned long lastSample = 0;
  unsigned long now = millis();

  // every 5ms check for hit
  if (now - lastSample >= 5) {
    lastSample = now;
    hitDetector_update();
  }

  int hits = hitDetector_hit();
  if (hits > 0) {
    score += hits * POINTS_PER_HIT;
    Serial.printf("Hit! Score: %d\n", score);
  }
}

