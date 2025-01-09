#include <Arduino.h>
#include "AGS02MA_Sensor.h"
#include "Buzzer.h"
#include "PN532.h"
#include "LoRaSender.h"

// Sensor declarations
AGS02MA_Sensor agsSensor;      // AGS02MA sensor instance
Buzzer buzzer(26);             // Buzzer instance (connected to pin 26)

// NFC reader definitions
#define IRQ_PIN 2
#define RESET_PIN 3
PN532 nfc(IRQ_PIN, RESET_PIN);

// LoRa sender instance
LoRaSender loraSender;

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    Serial.println("Starting system setup...");

    // Initialize AGS02MA sensor
    if (agsSensor.begin()) {
        Serial.println("AGS02MA sensor initialized successfully.");
    } else {
        Serial.println("Failed to initialize AGS02MA sensor!");
        buzzer.playErrorTone();  // Play an error tone if sensor initialization fails
    }

    // Initialize NFC reader
    if (nfc.begin()) {
        Serial.println("NFC reader initialized successfully.");
    } else {
        Serial.println("Failed to initialize NFC reader!");
        buzzer.playErrorTone();  // Play an error tone if NFC initialization fails
    }

    // Initialize LoRa
    loraSender.init();
    Serial.println("LoRa module initialized successfully.");

    // Play a happy tone to indicate setup completion
    buzzer.playHappyTone();
    delay(1000);
}

void loop() {
    Serial.println("Reading data...");

    // Read gas concentrations from the AGS02MA sensor
    String gasData = agsSensor.getAllGasConcentrations();
    if (gasData.length() > 0) {
        Serial.println("AGS02MA gas data: " + gasData);

        // Send gas data over LoRa
        loraSender.send((uint8_t *)gasData.c_str(), gasData.length());
        Serial.println("Gas data sent over LoRa.");
    } else {
        Serial.println("No gas data read from AGS02MA sensor.");
        buzzer.playUnhappyTone();
    }

    // Check for NFC tag
    if (nfc.requestTag()) {
        Serial.println("RFID tag detected.");

        uint8_t uidBuffer[7];
        uint8_t uidLength;

        if (nfc.readCardUID(uidBuffer, uidLength)) {
            Serial.print("Read UID: ");
            for (uint8_t i = 0; i < uidLength; i++) {
                Serial.print(uidBuffer[i] < 16 ? " 0" : " ");
                Serial.print(uidBuffer[i], HEX);
            }
            Serial.println();

            // Send NFC UID data over LoRa
            loraSender.send(uidBuffer, uidLength);
            Serial.println("NFC UID data sent over LoRa.");
        } else {
            Serial.println("Failed to read NFC UID.");
        }
    } else {
        Serial.println("No RFID tag detected.");
    }

    // Wait for 60 seconds before repeating
    Serial.println("Sleeping for 60 seconds...");
    delay(60000);
}
