#include "Display.h"

// Constructor
OLEDDriver::OLEDDriver()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

// Initialisatie van het display
void OLEDDriver::begin() {
    if (!display.begin(SSD1306_I2C_ADDRESS, SCREEN_ADDR)) {
        Serial.println(F("OLED niet gevonden!"));
        while (1);
    }
    display.display();  // Laat het initialisatiebeeld zien
    delay(2000);        // Wacht even
    display.clearDisplay();
}

// Scherm wissen
void OLEDDriver::clear() {
    display.clearDisplay();
}

// Tekst weergeven op de opgegeven x,y locatie
void OLEDDriver::displayText(const char* text, int x, int y) {
    display.setCursor(x, y);
    display.setTextSize(1);
    //display.setFont(&FreeMono9pt7b);
    display.setTextColor(SSD1306_WHITE);
    display.print(text);
}

// Afbeelding weergeven van een array van bytes
void OLEDDriver::displayImage(int x, int y, const uint8_t* image, int width, int height) {
    display.clearDisplay();
    display.drawBitmap(x, y, image, width, height, SSD1306_WHITE);
}

// Het scherm bijwerken
void OLEDDriver::update() {
    display.display();
}
