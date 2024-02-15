#ifndef __SMOKELIB__
#define __SMOKELIB__

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#define DO_PIN 13   // Pin DO

void smokeDetect();

extern uint8_t danger;

#endif