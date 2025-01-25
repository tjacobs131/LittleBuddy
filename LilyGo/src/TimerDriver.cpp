
#include "TimerDriver.h"


// Constructor: initialiseert de timer zonder deze te starten
TimerDriver::TimerDriver() : start_time(0), end_time(0), running(false) {

}

// Start de timer
void TimerDriver::start() {
    start_time = millis();
    running = true;
}

// Stop de timer en leg de eindtijd vast
void TimerDriver::stop() {
    if (running) {
        end_time = millis();
        running = false;
    }
}

// Reset de timer naar 0
void TimerDriver::reset() {
    start_time = millis();  // Stel starttijd opnieuw in op huidige tijd
    end_time = start_time;              // Eindtijd resetten
    running = false;                    // Timer is nog niet gestart
}

// Verkrijg de verstreken tijd in milliseconden
double TimerDriver::elapsedMillis(){
    if (running) {
        return millis() - start_time;
    } else {
        return end_time - start_time;
    }
}

// Verkrijg de verstreken tijd in seconden
double TimerDriver::elapsedSeconds(){
    return static_cast<double>(elapsedMillis()) / 1000.0;
}
