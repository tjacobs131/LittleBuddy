#ifndef LORASENDER_H
#define LORASENDER_H

#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>

class LoRaSender {
public:
    LoRaSender();
    void init();
    void send(uint8_t *data, uint8_t length);
    void loop();

    // Make onEvent public and static to allow global access
    static void onEvent(ev_t ev);

private:
    static void do_send(osjob_t* j);
    static osjob_t sendjob;
    static uint8_t payload[64];
    static uint8_t payloadLength;
};

#endif // LORASENDER_H
