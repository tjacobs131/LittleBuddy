#include <Arduino.h>
#include "LoRaSender.h"

// Create an instance of LoRaSender
LoRaSender loraSender;

void setup() {
    // Initialize the LoRaSender
    loraSender.init();

    // Prepare your payload with 5 values
    uint8_t data[1] = { 10}; // Replace with your actual values
    loraSender.send(data, sizeof(data));
}

void loop() {
    // Continuously run the LMIC loop
    loraSender.loop();
}
