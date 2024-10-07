#include "PN532.h"

PN532 rfid(2, 3);  // Define the IRQ and RESET pins

void setup() {
    Serial.begin(115200);
    if (!rfid.begin()) {
        Serial.println("PN532 initialization failed");
        while (1);
    }
    Serial.println("PN532 Initialized successfully");
}

void loop() {
    if (rfid.requestTag()) {
        uint8_t uid[7];  // UID buffer
        uint8_t uidLength;
        if (rfid.readCardUID(uid, uidLength)) {
            Serial.print("Card UID: ");
            for (uint8_t i = 0; i < uidLength; i++) {
                Serial.print(uid[i], HEX);
                Serial.print(" ");
            }
            Serial.println();
        }
    }
    delay(1000);  // Wait for a second before next request
}
