#include <stdio.h>
#include "esp_timer.h"
#include <esp_sleep.h>

#include <cloud_detector.h>
#include <bmp280_lib.h>
#include <wifi.h>
#include <DhtFunctions.h>
#include <SmokeLib.h>

#define SLEEP_TIME 30 *1000 * 1000

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

    smokeDetect();

    tsd.Field_Temperature = dataBmp280.temperature;
    tsd.Field_Altitude = dataBmp280.altitude;
    tsd.Field_Pressure = dataBmp280.pressure;
    tsd.Field_Humidity = dhd.humidity;
    tsd.Field_Sky = sspl.p_sky;
    tsd.Field_Cloud = sspl.p_cloud;
    tsd.Field_Error = sspl.p_error;
    tsd.Filed_Sun = sspl.p_sun;
    tsd.Field_Smoke = danger;

    while(send_to_thingspeak()!=ESP_OK && to<10 ){to++; vTaskDelay(1000 / portTICK_RATE_MS);}
    to=0;
}

void app_main(void)
{
    if(init_camera()!=ESP_OK) {return;}
    if(nvs_flash_init()!=ESP_OK){return;}
    if(esp_netif_init()!=ESP_OK){return;}
    if(esp_event_loop_create_default()!=ESP_OK){return;}

    esp_netif_create_default_wifi_sta();
    bmp280_setup();


    while (true)
    {
        esp_sleep_enable_timer_wakeup(SLEEP_TIME);
        esp_wifi_stop();
        esp_light_sleep_start();
        timer_callback();
    }
}