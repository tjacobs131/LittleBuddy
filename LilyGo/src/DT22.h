#include <Arduino.h>
#include <DHT.h>

class c_DHT22 {
public:
    // Constructor die de pin en het type DHT-sensor accepteert
    c_DHT22(uint8_t pin);

    // Methode om de sensor te initialiseren
    void begin();

    // Methode om de temperatuur in Celsius te lezen
    float readTemperature();

    // Methode om de luchtvochtigheid te lezen
    float readHumidity();

private:
    DHT dht;  // DHT object
};
