#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
ThingspeakData tsd;
uint8_t WifiStatus = 0; 

static const char* TAG = "app_wifi";

wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = SSID,
            .password = PASS}};

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("Wi-Fi station started...\n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("Wi-Fi station connected to AP...\n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("Wi-Fi station disconnected from AP...\n");
        WifiStatus = 0;
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("Wi-Fi station got IP address...\n");
        WifiStatus = 1;
        break;
    default:
        break;
    }
}

esp_err_t wifi_connection()
{
    printf("Initializing Wi-Fi connection...\n");
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);
    
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_connect();
    ESP_LOGE(TAG, "Started Connection + some seconds delay");
    vTaskDelay(500);
    
    return ESP_OK;
}

esp_err_t send_to_thingspeak() {
    esp_http_client_config_t client_config = {
            .url = "https://api.thingspeak.com/update",
            .method = HTTP_METHOD_POST,
            .event_handler = NULL, // No event handler needed
            .transport_type = HTTP_TRANSPORT_OVER_SSL,
            .cert_pem = NULL, // No certificate verification
            .timeout_ms = 10000,
        };
        esp_http_client_handle_t client = esp_http_client_init(&client_config);
        ESP_LOGE(TAG, "client riuscito");
        if (client == NULL) {
            vTaskDelay(pdMS_TO_TICKS(10000)); // Delay before retrying
        }
        char post_data[200];

        snprintf(post_data, sizeof(post_data), "api_key=%s&field5=%d", THINGSPEAK_API_KEY_1, tsd.Filed_Classification);
        esp_http_client_set_post_field(client, post_data, strlen(post_data));
        ESP_LOGE(TAG, "Postato i dati");
        esp_err_t err1 = esp_http_client_perform(client);
        ESP_LOGE(TAG, "ESP HTTP CLIENT PERFOMR");
        vTaskDelay(500);
        if (err1 == ESP_OK) {
            ESP_LOGE(TAG,"Data sent to ThingSpeak successfully.\n");
            esp_http_client_cleanup(client);
            return ESP_OK;
        } else {
            ESP_LOGE(TAG,"Failed to send data to ThingSpeak. Error: %d\n", err1);
            esp_http_client_cleanup(client);
            return ESP_FAIL;
        }

        //stop_webserver(client);
}


// Function to deinitialize WiFi module
void deinitialize_wifi() {
    // Disconnect from WiFi network
    esp_wifi_disconnect();
    // Stop WiFi interface
    esp_wifi_stop();
    // Free memory allocated for WiFi configuration
    // (if any)
}