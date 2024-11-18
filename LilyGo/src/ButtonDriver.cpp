
#include "ButtonDriver.h"

ButtonDriver::ButtonDriver(uint8_t pin, unsigned long longPressDuration)
    : buttonPin(pin), longPressDuration(longPressDuration), pressStartTime(0),
      isPressed(false), lastPressType(NO_PRESS) {}

void ButtonDriver::begin() {
    pinMode(buttonPin, INPUT_PULLUP); // Gebruik interne pull-up weerstand
}

ButtonPressType ButtonDriver::getPressType() {
    bool currentlyPressed = isButtonPressed();

    // Controleer of de knop net ingedrukt is
    if (currentlyPressed && !isPressed) {
        pressStartTime = millis();
        isPressed = true;
    }

    // Controleer of de knop losgelaten is
    else if (!currentlyPressed && isPressed) {
        unsigned long pressDuration = millis() - pressStartTime;
        isPressed = false;

        // Bepaal het type druk (kort of lang)
        if (pressDuration >= longPressDuration) {
            lastPressType = LONG_PRESS;
        } else {
            lastPressType = SHORT_PRESS;
        }
        return lastPressType;
    }

    // Geen nieuwe drukdetectie
    return NO_PRESS;
}

bool ButtonDriver::isButtonPressed() {
    return digitalRead(buttonPin) == HIGH; // Aangenomen dat de knop actief laag is
    // NO = LOW/HIGH instelling
    // NC = LOW/HIGH instelling
}
