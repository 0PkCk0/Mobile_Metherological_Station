#include "SmokeLib.h"

uint8_t danger;

void smokeDetect(){
        // Leggi il valore digitale dal pin DO
        int doValue = gpio_get_level(DO_PIN);
        danger = doValue;
        // Stampa i valori letti sulla console seriale
        printf("Valore digitale di output (DO): %d\n", doValue);
}