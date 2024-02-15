#include "DhtFunctions.h"

esp_err_t dht_read()
{
    float temperature, humidity;

    gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);

    
    if (dht_read_float_data(DHT_TYPE_DHT11, 2, &humidity, &temperature) == ESP_OK){
        printf("Humidity: %.1f Temp: %.1fC\n", humidity, temperature);
        return ESP_OK;
    }else{
        printf("Could not read data from sensor\n");
        return ESP_FAIL;
    }
}