// button_driver.h
#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include <Arduino.h>

enum ButtonPressType {
    NO_PRESS,
    SHORT_PRESS,
    LONG_PRESS
};

class ButtonDriver {
public:
    ButtonDriver(uint8_t pin, unsigned long longPressDuration = 2000);
    void begin();
    ButtonPressType getPressType();

private:
    uint8_t buttonPin;
    unsigned long longPressDuration;
    unsigned long pressStartTime;
    bool isPressed;
    ButtonPressType lastPressType;

    bool isButtonPressed();
};

#endif // BUTTON_DRIVER_H
