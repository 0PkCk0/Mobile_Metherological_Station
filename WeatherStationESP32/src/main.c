#include <stdio.h>
#include "esp_timer.h"
#include <cloud_detector.h>
#include <bmp280_lib.h>
#include <wifi.h>
#include <DhtFunctions.h>
#include <esp_sleep.h>

camera_fb_t *pic;

void timer_callback()
{
    int to=0;

    while(wifi_connection()!=ESP_OK && to<10){to++; vTaskDelay(1000 / portTICK_RATE_MS);}
    to=0;

    printf("Taking picture...");
    pic = esp_camera_fb_get();
    pic_analysis(pic);
    esp_camera_fb_return(pic);

    while(bmp280_read()!=ESP_OK && to<10){to++; vTaskDelay(1000 / portTICK_RATE_MS);}
    to=0;

    while(dht_read()!=ESP_OK && to<10){to++; vTaskDelay(1000 / portTICK_RATE_MS);}
    to=0;

    FIELD1_VALUE = dataBmp280.temperature;
    FIELD2_VALUE = dataBmp280.pressure;
    while(send_to_thingspeak()!=ESP_OK && to<10 ){to++; vTaskDelay(1000 / portTICK_RATE_MS);}
    to=0;
}

void app_main(void)
{
    if(ESP_OK != init_camera()) {
        return;
    }

    bmp280_setup();
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    
    while (true)
    {
        esp_sleep_enable_timer_wakeup(30000000);
        esp_wifi_stop();
        esp_light_sleep_start();
        timer_callback();
    }
}