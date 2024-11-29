#include <Arduino.h>
#include "LoRaSender.h"

// Create an instance of LoRaSender
LoRaSender loraSender;

void setup() {
    // Initialize the LoRaSender
    loraSender.init();

    uint8_t data[2]; // Replace with your actual values
    data[0] = 20;
    data[1] = 30;
    loraSender.send(data, sizeof(data));
}

void loop() {
    // Continuously run the LMIC loop
    loraSender.loop();
}