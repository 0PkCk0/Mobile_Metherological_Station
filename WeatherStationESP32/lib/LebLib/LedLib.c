#include "LedLib.h"
void setupLed(){
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << BLUE_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

void turnBlue(){
    gpio_set_level(BLUE_PIN, 1);
}

void shutdownLed(){
    gpio_set_level(BLUE_PIN, 0);
}