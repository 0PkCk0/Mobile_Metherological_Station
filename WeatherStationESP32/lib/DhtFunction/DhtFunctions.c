#include "DhtFunctions.h"

dhtData dhd;

esp_err_t dht_read()
{
    gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);

    
    if (dht_read_float_data(DHT_TYPE_DHT11, 2, &dhd.humidity, &dhd.temperature) == ESP_OK){
        printf("Humidity: %.2f Temp: %.2fC\n", dhd.humidity, dhd.temperature);
        return ESP_OK;
    }else{
        printf("Could not read data from sensor\n");
        return ESP_FAIL;
    }
}