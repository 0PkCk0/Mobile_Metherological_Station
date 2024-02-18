#ifndef __DHTFUNCTIONS__
#define __DHTFUNCTIONS__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <dht.h>

#if defined(CONFIG_EXAMPLE_TYPE_DHT11)
#define SENSOR_TYPE DHT_TYPE_DHT11
#endif
#if defined(CONFIG_EXAMPLE_TYPE_AM2301)
#define SENSOR_TYPE DHT_TYPE_AM2301
#endif
#if defined(CONFIG_EXAMPLE_TYPE_SI7021)
#define SENSOR_TYPE DHT_TYPE_SI7021
#endif

#define DHT_PIN 2

typedef struct{
    float temperature;
    float humidity;
}dhtData;

extern dhtData dhd;

esp_err_t dht_read();

#endif