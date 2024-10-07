#include <Arduino.h>
#include "PN532.h"

// Create an instance of the PN532 class
PN532 rfid;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting PN532 NFC Module...");

    // Initialize the PN532 NFC module
    if (!rfid.begin()) {
        Serial.println("PN532 initialization failed!");
        while (1);  // Stop if initialization fails
    }

    Serial.println("PN532 initialized successfully.");
}

void loop() {
    Serial.println("Waiting for an RFID tag...");

    // Check if an RFID tag is present
    if (rfid.requestTag()) {
        uint8_t uid[7];  // Buffer to store the UID
        uint8_t uidLength;

        // Try to read the RFID card's UID
        if (rfid.readCardUID(uid, uidLength)) {
            Serial.print("Card UID: ");
            for (uint8_t i = 0; i < uidLength; i++) {
                Serial.print(uid[i], HEX);
                Serial.print(" ");
            }
            Serial.println();
        } else {
            Serial.println("Failed to read card UID!");
        }
    } else {
        Serial.println("No RFID tag detected.");
    }

    delay(1000);  // Wait for 1 second before checking again
}
