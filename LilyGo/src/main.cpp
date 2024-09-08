#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <sectrets.h>


// OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Open-Meteo API endpoint
String weather_api_url = "https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&current_weather=true";

// Function to fetch weather data
String fetchWeather() {
  HTTPClient http;
  http.begin(weather_api_url);
  int httpCode = http.GET();  // Make the request

  if (httpCode > 0) {
    String payload = http.getString();
    http.end();
    return payload;
  } else {
    Serial.println("Error fetching weather data");
    http.end();
    return "";
  }
}

// Function to display the weather temperature
void displayWeather(float temperature) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Temp:");
  display.setCursor(0, 30);
  display.print(temperature);
  display.print(" C");
  display.display();
}

// Parse the weather response and get the temperature
float parseWeatherTemperature(String json) {
  StaticJsonDocument<1024> doc;
  deserializeJson(doc, json);
  
  float temperature = doc["current_weather"]["temperature"];
  return temperature;
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Connecting to WiFi");
  display.display();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("Connected to WiFi");
  
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("WiFi Connected!");
  display.display();
  delay(2000);
}

void loop() {
  // Fetch weather data from Open-Meteo
  String weatherData = fetchWeather();

  if (weatherData != "") {
    // Parse weather temperature and display it
    float temperature = parseWeatherTemperature(weatherData);
    Serial.println("Current temperature: " + String(temperature) + "C");
    displayWeather(temperature);
  }

  delay(60000);  // Fetch weather data every 60 seconds
}
