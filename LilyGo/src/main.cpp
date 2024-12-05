#include <Arduino.h>
#include "LoRaSender.h"

LoRaSender loraSender;

void setup() {
    loraSender.init();

    // Example payload to send
    uint8_t data[5] = { 10, 20, 30, 40, 50 };
    loraSender.send(data, sizeof(data));
}

void loop() {
    // Continuously run LMIC loop
    loraSender.loop();
}
