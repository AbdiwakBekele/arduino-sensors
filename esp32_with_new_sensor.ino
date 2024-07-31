#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <WebServer.h>

#define SENSOR_ADDRESS 0x40 // this is arbitrary



// WiFi credentials
const char* ssid = "Omishtu-JTS";
const char* password = "omishtuJTS";
const int Sensorpin = 35;
// Variables to store sensor data
float temperature, humidity, pressure, light, gas, altitude, soilMoisture;

WebSocketsServer webSocket = WebSocketsServer(81);  
WebServer server(80);  

String value;  

const char html_template[] PROGMEM = R"=====(
<html lang="en">
   <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>WebSocket Example</title>
      <script>
        var socket = new WebSocket("ws://" + location.hostname + ":81");
        socket.onopen = function(e) {  console.log("[socket] socket.onopen "); };
        socket.onerror = function(e) {  console.log("[socket] socket.onerror "); };
        socket.onmessage = function(e) {  
            console.log("[socket] " + e.data);
            document.getElementById("mrdiy_value").innerHTML = e.data;
        };
      </script>
   </head>
   <body style="max-width:400px;margin: auto;font-family:Arial, Helvetica, sans-serif;text-align:center">
      <div><h1><br /> ESP32 </h1></div>
      <div><p id="mrdiy_value" style="font-size:100px;margin:0"></p></div>
   </body>
</html>
)=====";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocket.sendTXT(num, "0");  // Send initial value or message
      }
      break;

    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      break;
     
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      // Optionally handle binary data
      break;
  }
}

// Function to handle the main page request
void handleMain() {
  server.send_P(200, "text/html", html_template);
}

// Function to handle 404 errors
void handleNotFound() {
  server.send(404, "text/html", "<html><body><p>404 Error</p></body></html>");
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // starting websocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // start HTTP server
  server.on("/", handleMain);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  // Read sensor data
  webSocket.loop();
  server.handleClient();

  int sensorValue = analogRead(Sensorpin);
  
  if (sensorValue) {
    // Print data to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    String temperature_string = String(temperature);
    webSocket.broadcastTXT(temperature_string);

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %"); 
    String humidity_string = String(humidity_string);
    webSocket.broadcastTXT(humidity_string);

    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");
    String pressure_string = String(pressure);
    webSocket.broadcastTXT(pressure_string);


    Serial.print("Light: ");
    Serial.print(light);
    Serial.println(" lux");
    String light_string = String(light);
    webSocket.broadcastTXT(light_string);

    Serial.print("Gas: ");
    Serial.print(gas);
    Serial.println(" ppm");
    String gas_string = String(gas);
    webSocket.broadcastTXT(gas_string);

    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.println(" m");
    String altitude_string = String(altitude);
    webSocket.broadcastTXT(altitude_string);

    Serial.print("Soil Moisture: ");
    Serial.print(soilMoisture);
    Serial.println(" %");
    int percentage = map(soilMoisture, 4095, 0, 0, 100); 
    String percentage_string = String(percentage);
    webSocket.broadcastTXT(percentage_string);

  delay(500); // Wait for 10 seconds before the next read
  }
}
