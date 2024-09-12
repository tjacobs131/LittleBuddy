#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>

// OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Set up the OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

// Time setup
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;    // Adjust based on your timezone
const int   daylightOffset_sec = 3600;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Connecting to WiFi");
  display.display();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // WiFi connected, show IP address on serial monitor
  Serial.println("WiFi connected.");
  Serial.println(WiFi.localIP());
  
  // Show WiFi connected status on OLED
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print("WiFi Connected!");
  display.display();
  delay(2000);

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  display.clearDisplay();
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    display.setCursor(0, 0);
    display.clearDisplay();
    display.print("Time fetch error");
    display.display();
    return;
  }

  // Format and display time on OLED
  char timeStr[10];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Time:");
  display.setCursor(0, 30);
  display.print(timeStr);
  display.display();

  delay(1000);  // Update every second
}
