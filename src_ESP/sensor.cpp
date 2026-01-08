#include "sensor.h"

#include <Arduino.h>
#include <esp_system.h>

void sensor_Init(void) {
  randomSeed(esp_random());
}

// generates random numbers for testing
int sensor_getTestData(void) {
  return random(0, 1000);
}
