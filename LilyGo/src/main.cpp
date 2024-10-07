#include "PN532.h"

PN532 rfid;

void setup() {
    Serial.begin(115200);
    
    if (!rfid.begin()) {
        Serial.println("PN532 initialization failed!");
        while (1);  // Halt if the PN532 initialization fails
    }
    Serial.println("PN532 initialized successfully.");
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
    delay(1000);  // Wait 1 second between each read attempt
}
