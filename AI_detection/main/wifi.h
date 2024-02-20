#ifndef __MY_DATA__
#define __MY_DATA__

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
#include "esp_event.h"

#define SSID "##########"
#define PASS "####à######"
#define THINGSPEAK_API_KEY_1 "29IJTIJ0F8UNHUQ5"

typedef struct{
    uint8_t Filed_Classification;
}ThingspeakData;

extern ThingspeakData tsd;
extern uint8_t WifiStatus; 

esp_err_t wifi_connection();
esp_err_t send_to_thingspeak();

void deinitialize_wifi();
#endif
