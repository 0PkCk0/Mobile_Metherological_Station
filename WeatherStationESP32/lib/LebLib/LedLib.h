#ifndef __LEDLIB__
#define __LEDLIB__


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BLUE_PIN    GPIO_NUM_12

void setupLed();
void turnBlue();
void shutdownLed();

#endif
