#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin) : _pin(pin) {}

void Buzzer::begin() {
    pinMode(_pin, OUTPUT);  // Set the buzzer pin to OUTPUT mode
}

void Buzzer::playTone(int frequency, int duration) {
    int period = 1000000 / frequency;  // Period of the tone in microseconds
    int halfPeriod = period / 2;
    
    for (long i = 0; i < duration * 1000L; i += period) {
        digitalWrite(_pin, HIGH);  // Turn the buzzer ON
        delayMicroseconds(halfPeriod);
        digitalWrite(_pin, LOW);  // Turn the buzzer OFF
        delayMicroseconds(halfPeriod);
    }
}

void Buzzer::playHappyTone() {
    playTone(1000, 200);  // High-pitched tone (1kHz) for 200ms
    delay(100);
    playTone(1500, 200);  // Higher-pitched tone (1.5kHz) for 200ms
}

void Buzzer::playUnhappyTone() {
    playTone(500, 300);  // Low-pitched tone (500Hz) for 300ms
    delay(100);
    playTone(300, 300);  // Lower-pitched tone (300Hz) for 300ms
}

void Buzzer::playWarningTone() {
    for (int i = 0; i < 3; i++) {
        playTone(2000, 100);  // Quick high-pitched warning beep (2kHz)
        delay(100);
    }
}

void Buzzer::playErrorTone() {
    playTone(100, 500);  // Very low-pitched tone (100Hz) for 500ms
    delay(100);
    playTone(100, 500);  // Repeat the same low-pitched tone
}
