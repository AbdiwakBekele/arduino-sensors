#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal.h>

// WiFi credentials
const char* ssid = "Omishtu-JTS";
const char* password = "omishtuJTS";

// Server endpoint
const char* serverName = "http://192.168.1.8:3000/soil-data"; // Update with your server's IP and endpoint

const int rs = 14; // RS pin to D14 (GPIO 14)
const int en = 12; // Enable pin to D12 (GPIO 12)
const int d4 = 25; // D4 pin to D25 (GPIO 25)
const int d5 = 26; // D5 pin to D26 (GPIO 26)
const int d6 = 27; // D6 pin to D27 (GPIO 27)
const int d7 = 32; // D7 pin to D32 (GPIO 32)

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int ledPin = 33; // LED pin to D33 (GPIO 33)
const int moistureSensorPin = 35; // Analog pin A0 (GPIO 36) for soil moisture sensor

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize GPIO
  pinMode(ledPin, OUTPUT);

  // Initialize LCD
  lcd.begin(16, 1);
  lcd.print("Initializing...");
  delay(10000);
  lcd.clear();
 
  // Connect to WiFi
  WiFi.begin(ssid, password);
  lcd.begin(25, 1);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    lcd.setCursor(0, 1);
    lcd.print("Connecting...");
  }
 
  Serial.println("Connected to WiFi");
  lcd.clear();
  lcd.print("WiFi Connected");
  delay(4000);
}

void loop() {
  // Read soil moisture sensor
  int sensorValue = analogRead(moistureSensorPin);
  int percentage = map(sensorValue, 0, 4095, 0, 100); // Adjust the range based on your sensor's characteristics
  Serial.println("Moisture: ");
  Serial.println(percentage);
  Serial.println("%");

  // Control LED (water pump) based on moisture level
  if (percentage <= 50) {
    digitalWrite(ledPin, HIGH);  // Turn water pump on
  } else {
    digitalWrite(ledPin, LOW);  // Turn water pump off
  }

  // Send data to server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String httpRequestData = "{\"sensor\":\"soil_moisture\", \"value\":" + String(percentage) + "}";
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
      lcd.clear();
      lcd.print("Data Sent");
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      lcd.clear();
      lcd.print("POST Failed");
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  // Wait before next measurement
  delay(10000); 
}
