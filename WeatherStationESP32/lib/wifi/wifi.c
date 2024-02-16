#include "wifi.h"

ThingspeakData tsd;
uint8_t WifiStatus = 0; 

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

    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_connect();
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
        if (client == NULL) {
            vTaskDelay(pdMS_TO_TICKS(10000)); // Delay before retrying
        }
        char post_data[200];
        snprintf(post_data, sizeof(post_data), "api_key=%s&field1=%.2f&field2=%.2f&field3=%.2f&field4=%.2f&field6=%.2f", THINGSPEAK_API_KEY, tsd.Field_Altitude, tsd.Field_Pressure, tsd.Field_Temperature, tsd.Field_Humidity, tsd.Field_Smoke);
        esp_http_client_set_post_field(client, post_data, strlen(post_data));

        esp_err_t err = esp_http_client_perform(client);

        snprintf(post_data, sizeof(post_data), "api_key=%s&field1=%.2f&field2=%.2f&field3=%.2f&field4=%.2f", THINGSPEAK_API_KEY_1, tsd.Field_Sky, tsd.Filed_Sun, tsd.Field_Cloud, tsd.Field_Error);
        esp_http_client_set_post_field(client, post_data, strlen(post_data));

        esp_err_t err1 = esp_http_client_perform(client);

        if (err == ESP_OK && err1 == ESP_OK) {
            printf("Data sent to ThingSpeak successfully.\n");
            esp_http_client_cleanup(client);
            return ESP_OK;
        } else {
            printf("Failed to send data to ThingSpeak. Error: %d\n", err);
            esp_http_client_cleanup(client);
            return ESP_FAIL;
        }

        //stop_webserver(client);
}

httpd_handle_t start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    /*
    if (server != NULL) {
        xTaskCreate(send_to_thingspeak, "send_to_thingspeak", 4096, NULL, 5, NULL);
    }
    */
    return server;
}

void stop_webserver(httpd_handle_t server)
{
    if (server) {
        httpd_stop(server);
    }
}