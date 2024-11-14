#include "PN532.h"

// Constructor: Initializes the PN532 instance using IRQ and reset pins
PN532::PN532(uint8_t irq, uint8_t reset) : nfc(irq, reset) {}

bool PN532::begin() {
    Wire.begin(21, 22); // Initialize Wire library with specific SDA and SCL

    // Initialize the PN532 using I2C without specifying the bus or address
    if (!nfc.begin()) {
        Serial.println("Failed to initialize PN532!");
        return false;  // Initialization failed
    }

    // Get the firmware version for debugging purposes
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("Failed to detect PN532 firmware!");
        return false;
    }

    // Print firmware details
    Serial.print("Found chip PN5");
    Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware version: ");
    Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.');
    Serial.println((versiondata >> 8) & 0xFF, DEC);

    // Configure the PN532 to read RFID tags
    nfc.SAMConfig();
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
