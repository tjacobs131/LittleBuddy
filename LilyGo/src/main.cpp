#include <WiFi.h>
#include <PubSubClient.h>
#include "MAX9814.h"
#include "DT22.h"
#include "AGS02MA_Sensor.h"

// ** WiFi Configuration **
const char* ssid = "Your_SSID";           // Replace with your WiFi SSID
const char* password = "Your_PASSWORD";   // Replace with your WiFi password

// ** TTN MQTT Configuration **
const char* mqtt_server = "eu1.cloud.thethings.network";  // TTN MQTT server
const int mqtt_port = 1883;                              // Port 1883 (non-TLS) or 8883 (TLS)
const char* mqtt_user = "little-buddy@ttn";              // TTN Application username
const char* mqtt_password = "NNSXS.F6OOQAGNMJLAXJOIXEBZH4QSLLEP6YCOUAUO2RA.H7WYA7QLCTXAHDJ4SNVLIGRCF2DIMULZ7KJFRJFBCREW6HLXXP7A";          // TTN API Key for the application
const char* mqtt_topic_up = "v3/little-buddy@ttn/devices/device-id/up";      // Publish topic
const char* mqtt_topic_down = "v3/little-buddy@ttn/devices/device-id/down";  // Subscribe topic

WiFiClient espClient;
PubSubClient client(espClient);

// ** Sensor Configuration **
#define MAX9814PIN 13
#define DHTPIN 15
MAX9814 micSensor(MAX9814PIN);
c_DHT22 dhtSensor(DHTPIN);
AGS02MA_Sensor gasSensor;

void setupWiFi() {
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");
}

void reconnectMQTT() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
            Serial.println("connected");
            // Subscribe to the downlink topic
            client.subscribe(mqtt_topic_down);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

// MQTT Callback Function
void mqttCallback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.println(topic);

    Serial.print("Message: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
    }
    Serial.println();

    // You can handle messages here if needed
}

void setup() {
    Serial.begin(115200);

    // Initialize WiFi
    setupWiFi();

    // Initialize MQTT
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqttCallback);

    // Initialize Sensors
    micSensor.readMicDecibels();
    dhtSensor.begin();
    gasSensor.begin();
}

void loop() {
    // Ensure MQTT is connected
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();

    // Read sensor data
    float decibels = micSensor.readMicDecibels();
    float temperature = dhtSensor.readTemperature();
    float humidity = dhtSensor.readHumidity();
    float gas_ppb = gasSensor.readPPB();

    // Create a JSON payload
    String payload = "{";
    payload += "\"decibels\":" + String(decibels) + ",";
    payload += "\"temperature\":" + String(temperature) + ",";
    payload += "\"humidity\":" + String(humidity) + ",";
    payload += "\"gas_ppb\":" + String(gas_ppb);
    payload += "}";

    // Publish the payload to TTN MQTT
    Serial.print("Publishing to TTN MQTT: ");
    Serial.println(payload);
    client.publish(mqtt_topic_up, payload.c_str());

    // Wait for 60 seconds
    delay(60000);
}
