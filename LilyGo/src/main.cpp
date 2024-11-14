#include "PN532.h"

// Define the IRQ and reset pins
#define IRQ_PIN 2
#define RESET_PIN 3

// Create an instance of the PN532 class
PN532 nfc(IRQ_PIN, RESET_PIN);

void setup() {
    Serial.begin(115200);  // Start the serial connection for debugging
    Serial.println("Starting NFC reader...");

    // Begin communication with the NFC module
    if (!nfc.begin()) {
        Serial.println("Failed to initialize the NFC reader!");
        while (1);  // Halt indefinitely if unable to start communication
    }
    Serial.println("NFC reader initialized successfully.");
}

void loop() {
    // Check for the presence of an RFID tag
    if (nfc.requestTag()) {
        Serial.println("RFID tag detected.");

        // Buffer to store the UID of the tag
        uint8_t uidBuffer[7];
        uint8_t uidLength;

        // Attempt to read the UID from the RFID tag
        if (nfc.readCardUID(uidBuffer, uidLength)) {
            Serial.print("Read UID with length ");
            Serial.print(uidLength);
            Serial.print(": ");

            // Print the UID in hex format
            for (uint8_t i = 0; i < uidLength; i++) {
                Serial.print(uidBuffer[i] < 16 ? " 0" : " ");
                Serial.print(uidBuffer[i], HEX);
            }
            Serial.println();
        } else {
            Serial.println("Failed to read UID.");
        }
    } else {
        Serial.println("No RFID tag in range.");
    }

    delay(1000);  // Wait a second before checking again
}
