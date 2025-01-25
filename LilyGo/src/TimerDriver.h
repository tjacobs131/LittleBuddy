#include <Arduino.h>

class TimerDriver {
public:
    TimerDriver();

    void start();          // Start de timer
    void stop();           // Stop de timer
    void reset();          // Reset de timer naar 0
    double elapsedMillis();   // Verkrijg verstreken tijd in milliseconden
    double elapsedSeconds();  // Verkrijg verstreken tijd in seconden

private:
    uint64_t start_time;
    uint64_t end_time;
    bool running;
};
