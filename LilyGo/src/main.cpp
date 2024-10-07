#include "Buzzer.h"

Buzzer buzzer(9);  // Buzzer connected to pin 9

void setup() {
    buzzer.begin();
    buzzer.playHappyTone();  // Play a happy tone on startup
}

void loop() {
    // Example of how to play different tones based on some condition
    buzzer.playWarningTone();  // Play a warning sound
    delay(2000);  // Wait for 2 seconds
    
    buzzer.playErrorTone();  // Play an error sound
    delay(3000);  // Wait for 3 seconds
}
