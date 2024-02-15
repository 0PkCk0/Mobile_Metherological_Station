#ifndef __BMP280_LIB__
#define __BMP280_LIB__

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <string.h>
#include <math.h>
#include <bmp280.h>

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

#define I2C_MASTER_SDA 15
#define I2C_MASTER_SCL 14

typedef struct{
    float pressure;
    float altitude;
    float temperature;
}data_bmp280;

extern data_bmp280 dataBmp280;

void bmp280_setup();

void bmp280_sleep();

void bmp280_awake();

esp_err_t bmp280_read();

#endif