#include "DT22.h"

// Constructor die het DHT-object initialiseert met de opgegeven pin en het type DHT-sensor
c_DHT22::c_DHT22(uint8_t pin) : dht(pin, 22) {}

// Beginmethode om de DHT-sensor te initialiseren
void c_DHT22::begin() {
    dht.begin();
}

// Methode om de temperatuur te lezen
float c_DHT22::readTemperature() {
    return dht.readTemperature();
}

// Methode om de luchtvochtigheid te lezen
float c_DHT22::readHumidity() {
    return dht.readHumidity();
}
