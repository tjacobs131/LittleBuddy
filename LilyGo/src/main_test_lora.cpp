#include "Buzzer.h"

Buzzer buzzer(26);  // Buzzer connected to pin 9

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

    buzzer.playHappyTone();  // Play a happy sound
    delay(1000);  // Wait for 1 second

    buzzer.playUnhappyTone();  // Play an unhappy sound
    delay(2000);  // Wait for 2 seconds
    
}
