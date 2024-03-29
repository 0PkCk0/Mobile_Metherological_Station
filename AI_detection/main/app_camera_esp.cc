#include "app_camera_esp.h"
#include "sdkconfig.h"
#include "bsp/esp-bsp.h"

static const char *TAG = "app_camera";

int app_camera_init() {
  /*
  bsp_i2c_init();
  camera_config_t config = BSP_CAMERA_DEFAULT_CONFIG;
  */
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = CAMERA_PIN_D0;
  config.pin_d1 = CAMERA_PIN_D1;
  config.pin_d2 = CAMERA_PIN_D2;
  config.pin_d3 = CAMERA_PIN_D3;
  config.pin_d4 = CAMERA_PIN_D4;
  config.pin_d5 = CAMERA_PIN_D5;
  config.pin_d6 = CAMERA_PIN_D6;
  config.pin_d7 = CAMERA_PIN_D7;
  config.pin_xclk = CAMERA_PIN_XCLK;
  config.pin_pclk = CAMERA_PIN_PCLK;
  config.pin_vsync = CAMERA_PIN_VSYNC;
  config.pin_href = CAMERA_PIN_HREF;
  config.pin_sscb_sda = CAMERA_PIN_SIOD;
  config.pin_sscb_scl = CAMERA_PIN_SIOC;
  config.pin_pwdn = CAMERA_PIN_PWDN;
  config.pin_reset = CAMERA_PIN_RESET;
  config.xclk_freq_hz = XCLK_FREQ_HZ;
  config.jpeg_quality = 10;
  config.fb_count = 2;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  // Pixel format and frame size are specific configurations options for this application.
  // Frame size must be 96x96 pixels to match the trained model.
  // Pixel format defaults to grayscale to match the trained model.
  // With display support enabled, the pixel format is RGB565 to match the display. The frame is converted to grayscale before it is passed to the trained model.
  config.pixel_format = CAMERA_PIXEL_FORMAT;
  config.frame_size = CAMERA_FRAME_SIZE;

  // camera init and catch the error
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Camera init failed with error 0x%x", err);
    return -1;
  }

  
  sensor_t *s = esp_camera_sensor_get();
  s->set_vflip(s, 1); //flip it back
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID)
  {
      s->set_brightness(s, 5);  //up the blightness just a bit
      s->set_saturation(s, -5); //lower the saturation
  }
  
  return 0;
}

void app_camera_deinit(){
  esp_camera_deinit(); 
}