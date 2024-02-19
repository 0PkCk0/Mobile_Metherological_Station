#include "main_functions.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_sleep.h>
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "esp_heap_caps.h"

#define SLEEP_TIME 60*1000 * 1000

static const char* TAG = "app_main";

extern "C" {
  #include "wifi.h"
}

void tf_main(void) {
  static_model_setup_configuration();
  while(true){
    dinamic_model_configuration();
    inference();
    vTaskDelay(100);
    ESP_LOGI(TAG, "Finished Inference");
    free_dma_buffer();
    ESP_LOGI(TAG, "Buffer Free from AI Model");
    esp_task_wdt_delete(NULL);
    vTaskDelay(500);
    nvs_flash_init(); 
    wifi_connection();
    ESP_LOGI(TAG, "Wifi Connected");
    send_to_thingspeak();
    ESP_LOGI(TAG, "Message Sent");
    esp_task_wdt_add(NULL);
    deinitialize_wifi();
    ESP_LOGI(TAG, "Wifi Deinitialized");
    esp_task_wdt_delete(NULL);
    ESP_LOGI(TAG, "in 10 seconds the platform will restart");
    vTaskDelay(1000);
    esp_restart();
  }
}

extern "C" void app_main() {
  xTaskCreate((TaskFunction_t)&tf_main, "tf_main", 4 * 1024, NULL, 8, NULL);
  vTaskDelete(NULL);
}

