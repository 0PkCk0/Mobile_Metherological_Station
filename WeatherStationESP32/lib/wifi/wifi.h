#ifndef MY_DATA
#define MY_DATA

#include <stdio.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "esp_http_client.h"


#define SSID "RebootYourPc"
#define PASS "fermifermipsg"
#define THINGSPEAK_API_KEY "CE1ZO2IYOX7MSZJ0"

typedef struct{
    float Field_Temperature;
    float Field_Pressure;
    float Field_Altitude;
    float Field_Humidity;
    float Field_Cloud;
    float Field_Error;
    float Filed_Sun;
    float Field_Sky;
    float Field_Smoke;
}ThingspeakData;

extern ThingspeakData tsd;

esp_err_t wifi_connection();
httpd_handle_t start_webserver(void);
void stop_webserver(httpd_handle_t server);
esp_err_t send_to_thingspeak();

#endif
