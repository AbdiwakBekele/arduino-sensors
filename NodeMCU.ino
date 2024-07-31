#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <WebServer.h>

const char* ssid = "Omishtu-JTS";
const char* password = "omishtuJTS";



const int ledPin = 33; // LED pin to D33 (GPIO 33)
const int moistureSensorPin = 35; // Analog pin A0 (GPIO 36) for soil moisture sensor

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
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  Serial.println(WiFi.localIP());

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Start HTTP server
  server.on("/", handleMain);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  webSocket.loop();
  server.handleClient();

  // Read soil moisture sensor
  int sensorValue = analogRead(moistureSensorPin);
  Serial.println(sensorValue);
  int percentage = map(sensorValue, 4095, 0, 0, 100); 
  String percentage_string = String(percentage);
  //Serial.println(percentage);
  // Broadcast the moisture value
  webSocket.broadcastTXT(percentage_string);

  delay(500);  // Small delay to avoid overwhelming the WebSocket with messages
}