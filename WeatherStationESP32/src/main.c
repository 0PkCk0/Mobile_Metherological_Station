#include <stdio.h>
#include "esp_timer.h"
#include <esp_sleep.h>

#include <cloud_detector.h>
#include <bmp280_lib.h>
#include <wifi.h>
#include <DhtFunctions.h>
#include <SmokeLib.h>
#include <LedLib.h>

#define RETRY_DELAY 1000

#define SLEEP_TIME 60 *1000 *1000

camera_fb_t *pic;

void timer_callback()
{
    //status Led
    turnBlue();

    //awake bmp from sleep
    bmp280_awake();
    int to=0;

    //init wifi connection and wait for the IP address (WiFiStatus=1)
    wifi_connection();
    while (WifiStatus==0 && to<10){to++; vTaskDelay(RETRY_DELAY / portTICK_RATE_MS);}
    to=0;

    //taking a picture 320x240 RGB565
    printf("Taking picture...");
    pic = esp_camera_fb_get();
    pic_analysis(pic);              //Pass the frame buffer to the cloud detection function
    esp_camera_fb_return(pic);

    //read from bmp and try mulitple times in case of errors
    while(bmp280_read()!=ESP_OK && to<10){to++; vTaskDelay(RETRY_DELAY / portTICK_RATE_MS);}
    to=0;

    //read from dht and try mulitple times in case of errors (really cheap sensor, sometimes it does not respond at first time)
    while(dht_read()!=ESP_OK && to<10){to++; vTaskDelay(RETRY_DELAY / portTICK_RATE_MS);}
    to=0;

    //check for dangerous substances (1=safe, 0=unsafe)
    smokeDetect();

    //collect data for thingspeak in a struct
    tsd.Field_Temperature = dataBmp280.temperature;
    tsd.Field_Altitude = dataBmp280.altitude;
    tsd.Field_Pressure = dataBmp280.pressure;
    tsd.Field_Humidity = dhd.humidity;
    tsd.Field_Sky = sspl.p_sky;
    tsd.Field_Cloud = sspl.p_cloud;
    tsd.Field_Error = sspl.p_error;
    tsd.Filed_Sun = sspl.p_sun;
    tsd.Field_Smoke = danger;

    //send data to thingspeak, since we use free API sometimes they doesn't work
    while(send_to_thingspeak()!=ESP_OK && to<10 ){to++; vTaskDelay(RETRY_DELAY / portTICK_RATE_MS);}
    to=0;

    shutdownLed();
}

void app_main(void)
{
    if(init_camera()!=ESP_OK) {return;}
    if(nvs_flash_init()!=ESP_OK){return;}
    if(esp_netif_init()!=ESP_OK){return;}
    if(esp_event_loop_create_default()!=ESP_OK){return;}
    esp_netif_create_default_wifi_sta();
    setupLed();
    bmp280_setup();

    while (true)
    {
        esp_sleep_enable_timer_wakeup(SLEEP_TIME);
        esp_wifi_stop();
        bmp280_sleep();
        esp_light_sleep_start();
        timer_callback();
    }
}