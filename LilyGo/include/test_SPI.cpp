#include <SPI.h>
#include <Adafruit_PN532.h>

// Define custom pins for SPI
#define SS_PIN  25  // GPIO 25 for SS (Slave Select)
#define MOSI_PIN 4  // GPIO 4 for MOSI (Master Out Slave In)
#define MISO_PIN 0  // GPIO 0 for MISO (Master In Slave Out)
#define SCK_PIN 2   // GPIO 2 for SCK (Serial Clock)

// Create an instance of the PN532 class using software SPI
Adafruit_PN532 nfc(SS_PIN, SCK_PIN, MOSI_PIN, MISO_PIN);

void setup(void) {
    Serial.begin(115200);
    Serial.println("SPI Communication Test");

    // Initialize SPI communication
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
    delay(500);  // Wait for the module to initialize

    // Initialize the PN532 NFC module
    if (!nfc.begin()) {
        Serial.println("Failed to initialize PN532 over SPI!");
        while (1);  // Halt if initialization fails
    }

    Serial.println("PN532 initialized successfully.");
}

void loop(void) {
    // Test the SPI communication by reading the PN532 firmware version
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) {
        Serial.println("Didn't find PN532 module");
        delay(1000);
        return;
    }

    // Print firmware details
    Serial.print("Found PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware version: "); Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

    delay(1000);  // Repeat the check every second
}
