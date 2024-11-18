#include <Arduino.h>

class MAX9814 {
public:
    // Constructor
    MAX9814(uint8_t pin);

    // Functie om de microfoonwaarden te lezen
    int readMicValue();

    // Functie om de waarde om te zetten naar spanning
    float readMicVoltage();

    // Functie om de waarde om te naar decibel
    float readMicDecibels();

private:
    uint8_t micPin;
};
