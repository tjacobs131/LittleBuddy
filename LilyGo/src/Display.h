#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Fonts/FreeMono9pt7b.h"

// Definieer de I2C-adres en schermdimensies
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDR   0x3C  // Het I2C-adres van het OLED-scherm (kan variëren)
#define SSD1306_I2C_ADDRESS 0x3C

class OLEDDriver {
public:
    OLEDDriver();
    void begin();
    void clear();
    void displayText(const char* text, int x, int y);
    void displayImage(int x, int y, const uint8_t* image, int width, int height);
    void update();

private:
    Adafruit_SSD1306 display;
};

#endif // OLED_DRIVER_H
