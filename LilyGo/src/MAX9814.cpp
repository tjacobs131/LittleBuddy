#include "MAX9814.h"

// Constructor: stel de pin in
MAX9814::MAX9814(uint8_t pin) : micPin(pin) {
    pinMode(micPin, INPUT);
}

// Functie om de microfoonwaarden te lezen
int MAX9814::readMicValue() {
    return analogRead(micPin);
}

// Functie om de waarde om te zetten naar spanning
float MAX9814::readMicVoltage() {
    int micValue = readMicValue();
    //float voltage = micValue * (5.0 / 1023.0);  // Voor een 10-bits ADC en 5V referentiespanning
    float voltage = micValue * (3.3 / 4095.0);  // Voor een 12-bits ADC en 3.3V referentiespanning
    return voltage;
}

// Functie om de waarde om te zetten naar decibels
float MAX9814::readMicDecibels() {
    // Lees de ruwe waarde van de microfoon
    float voltage = readMicVoltage();

    // Gebruik een berekening om van de spanning naar dB te gaan.
    // Dit is een schatting omdat de MAX9814 geen lineaire schaal biedt.
    float decibels = 20 * log10(voltage / 0.00631);  // 0.00631V als basiswaarde (Aangepast indien nodig)

    return decibels;
}
