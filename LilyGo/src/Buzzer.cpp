#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin) : _pin(pin) {}

void Buzzer::begin() {
    pinMode(_pin, OUTPUT);  // Set the buzzer pin to OUTPUT mode
}

void Buzzer::playTone(int frequency, int duration) {
    tone(_pin, frequency, duration);  // Use Arduino's tone function to play a tone
    delay(duration);  // Delay for the duration of the tone
    noTone(_pin);  // Stop the tone
}

void Buzzer::playHappyTone() {
    playTone(2000, 300);  // Louder, higher-pitched tone (2kHz) for 300ms
    delay(100);
    playTone(2500, 300);  // Even higher-pitched tone (2.5kHz) for 300ms
}

void Buzzer::playUnhappyTone() {
    playTone(400, 500);  // Low-pitched tone (400Hz) for 500ms
    delay(100);
    playTone(300, 500);  // Lower-pitched tone (300Hz) for 500ms
}

void Buzzer::playWarningTone() {
    for (int i = 0; i < 3; i++) {
        playTone(3000, 150);  // Quick high-pitched warning beep (3kHz) for 150ms
        delay(100);
    }
}

void Buzzer::playErrorTone() {
    playTone(200, 700);  // Very low-pitched tone (200Hz) for 700ms
    delay(100);
    playTone(200, 700);  // Repeat the low-pitched tone
}
