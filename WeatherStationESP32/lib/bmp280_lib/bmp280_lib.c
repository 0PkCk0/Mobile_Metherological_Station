#include "bmp280_lib.h"


data_bmp280 dataBmp280;

bmp280_t dev;

void bmp280_setup(){
    ESP_ERROR_CHECK(i2cdev_init());
    bmp280_params_t params;
    bmp280_init_default_params(&params);
    memset(&dev, 0, sizeof(bmp280_t));

    ESP_ERROR_CHECK(bmp280_init_desc(&dev, BMP280_I2C_ADDRESS_0, 0, I2C_MASTER_SDA, I2C_MASTER_SCL));
    ESP_ERROR_CHECK(bmp280_init(&dev, &params));

    printf("BMP280: found BMP280\n");
}

void bmp280_sleep(){
    bmp280_params_t params;
    bmp280_init_default_params(&params);
    params.mode = BMP280_MODE_SLEEP;
    ESP_ERROR_CHECK(bmp280_init(&dev, &params));
}

void bmp280_awake(){
    bmp280_params_t params;
    bmp280_init_default_params(&params);
    ESP_ERROR_CHECK(bmp280_init(&dev, &params));
}

esp_err_t bmp280_read()
{
    vTaskDelay(pdMS_TO_TICKS(1000));
    if (bmp280_read_float(&dev, &dataBmp280.temperature, &dataBmp280.pressure, NULL) != ESP_OK)
    {
        printf("Temperature/pressure reading failed\n");
        return ESP_FAIL;
    }
    printf("Pressure: %.2f Pa, Temperature: %.2f C, ", dataBmp280.pressure, dataBmp280.temperature);
    dataBmp280.altitude = 44330 * (1.0 - pow(dataBmp280.pressure / 101300, 0.1903));
    printf("Altitude: %.2f m", dataBmp280.altitude);
    printf("\n");
    return ESP_OK;
}
