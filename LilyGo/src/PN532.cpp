#include "PN532.h"
#include <Adafruit_PN532.h>
#include <SPI.h>

// Constructor: Use software SPI with custom pin mapping
PN532::PN532() : nfc(25, 2, 4, 0) {  // SS: GPIO 25, SCK: GPIO 2, MOSI: GPIO 4, MISO: GPIO 0
}

bool PN532::begin() {
    nfc.begin();  // Initialize the Adafruit PN532 library
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        return false;  // No response from the PN532, initialization failed
    }
    nfc.SAMConfig();  // Configure the PN532 to read RFID cards
    return true;
}

bool PN532::requestTag() {
    uint8_t success = nfc.inListPassiveTarget();
    return success > 0;  // Return true if an RFID tag is detected
}

bool PN532::readCardUID(uint8_t *uidBuffer, uint8_t &uidLength) {
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the UID
    uint8_t uidLengthLocal;

    // Look for an RFID tag
    if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLengthLocal)) {
        uidLength = uidLengthLocal;  // Set the length of the UID
        for (uint8_t i = 0; i < uidLength; i++) {
            uidBuffer[i] = uid[i];  // Copy the UID into the provided buffer
        }
        return true;  // Successfully read the UID
    }
    return false;  // Failed to read a card
}