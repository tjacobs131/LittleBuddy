#include "PN532.h"

// Constructor: Initializes the PN532 instance using the custom SPI pins
PN532::PN532() : nfc(25, 2, 4, 12) {  // SS: GPIO 25, SCK: GPIO 2, MOSI: GPIO 4, MISO: GPIO 12
}

bool PN532::begin() {
    // Initialize SPI communication and delay for module to stabilize
    SPI.begin(2, 12, 4, 25);  // SCK: GPIO 2, MISO: GPIO 12, MOSI: GPIO 4, SS: GPIO 25
    delay(1000);  // Allow some time for the module to initialize

    // Initialize the PN532 using SPI
    if (!nfc.begin()) {
        Serial.println("Failed to initialize PN532 over SPI!");
        return false;  // Initialization failed
    }

    // Get the firmware version (for debugging purposes)
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("Failed to detect PN532 firmware!");
        return false;
    }

    // Print firmware details
    Serial.print("Found PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware version: "); Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

    // Configure the PN532 to read RFID tags (MIFARE/ISO 14443A)
    nfc.SAMConfig();  // Sets the module into a mode to detect passive RFID tags
    return true;
}

bool PN532::requestTag() {
    // Check if an RFID tag is in range
    uint8_t success = nfc.inListPassiveTarget();
    return success > 0;
}

bool PN532::readCardUID(uint8_t *uidBuffer, uint8_t &uidLength) {
    uint8_t uid[7];  // Buffer to store the UID
    uint8_t uidLengthLocal;

    // Read the RFID card UID
    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLengthLocal)) {
        uidLength = uidLengthLocal;  // Set the length of the UID
        for (uint8_t i = 0; i < uidLength; i++) {
            uidBuffer[i] = uid[i];  // Copy the UID to the provided buffer
        }
        return true;  // Successfully read the UID
    }
    return false;  // Failed to read a card
}
