#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
public:
    Buzzer(uint8_t pin);  // Constructor to initialize the pin

    void begin();  // Set pin mode for the buzzer
    void playHappyTone();  // Play a happy sound
    void playUnhappyTone();  // Play an unhappy sound
    void playWarningTone();  // Play a warning sound
    void playErrorTone();  // Play an error sound

private:
    uint8_t _pin;  // Pin where the buzzer is connected
    void playTone(int frequency, int duration);  // Helper function to generate tone
};

#endif // BUZZER_H
