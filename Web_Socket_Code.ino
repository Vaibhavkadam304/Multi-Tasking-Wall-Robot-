#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "Redmi";
const char* password = "12345678";

WiFiServer server(80);
WebSocketsServer webSocket(81);

int ENA1 = 14; // Motor 1 & 2 PWM Pin
int IN1_1 = 5; // Motor 1 Control Pin 1
int IN2_1 = 4; // Motor 1 Control Pin 2

int ENA2 = 12; // Motor 3 & 4 PWM Pin
int IN1_2 = 0;  // Motor 3 Control Pin 1
int IN2_2 = 2;  // Motor 3 Control Pin 2

void setup() {
  Serial.begin(115200);

  pinMode(ENA1, OUTPUT);
  pinMode(IN1_1, OUTPUT);
  pinMode(IN2_1, OUTPUT);
  
  pinMode(ENA2, OUTPUT);
  pinMode(IN1_2, OUTPUT);
  pinMode(IN2_2, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  // Wi-Fi connected, display message in Serial Monitor
  Serial.println("Connected to Wi-Fi");

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_TEXT:
      handleWebSocketMessage(payload);
      break;
  }
}

void handleWebSocketMessage(uint8_t * payload) {
  String message = (char*)payload;
  Serial.println("Received message: " + message);
  
  if (message == "forward") {
    moveForward();
  } else if (message == "backward") {
    moveBackward();
  } else if (message == "stop") {
    stopMotors();
  }
}

void moveForward() {
  digitalWrite(IN1_1, HIGH);
  digitalWrite(IN2_1, LOW);
  analogWrite(ENA1, 255); // Set maximum speed (PWM value 255) for left movement
  
  digitalWrite(IN1_2, LOW);
  digitalWrite(IN2_2, HIGH);
  analogWrite(ENA2, 255); // Set maximum speed (PWM value 255) for right movement
}

void moveBackward() {
  digitalWrite(IN1_1, LOW);
  digitalWrite(IN2_1, HIGH);
  analogWrite(ENA1, 255); // Set maximum speed (PWM value 255) for right movement
  
  digitalWrite(IN1_2, HIGH);
  digitalWrite(IN2_2, LOW);
  analogWrite(ENA2, 255); // Set maximum speed (PWM value 255) for left movement
}

void stopMotors() {
  analogWrite(ENA1, 0);
  analogWrite(ENA2, 0);
  digitalWrite(IN1_1, LOW);
  digitalWrite(IN2_1, LOW);
  digitalWrite(IN1_2, LOW);
  digitalWrite(IN2_2, LOW);
}

socket
<!DOCTYPE html>
<html>
<head>
  <title>Motor Control</title>
  <style>
    body {
      background-color: #f0f0f0;
      font-family: Arial, sans-serif;
      text-align: center;
      margin-top: 50px;
    }
    
    .button {
      appearance: button;
      background-color: #1899D6;
      border: solid transparent;
      border-radius: 16px;
      border-width: 0 0 4px;
      box-sizing: border-box;
      color: #FFFFFF;
      cursor: pointer;
      display: inline-block;
      font-family: din-round,sans-serif;
      font-size: 15px;
      font-weight: 700;
      letter-spacing: .8px;
      line-height: 20px;
      margin: 10px;
      outline: none;
      overflow: visible;
      padding: 13px 16px;
      text-align: center;
      text-transform: uppercase;
      touch-action: manipulation;
      transform: translateZ(0);
      transition: filter .2s;
      user-select: none;
      -webkit-user-select: none;
      vertical-align: middle;
      white-space: nowrap;
    }
    
    .button:after {
      background-clip: padding-box;
      background-color: #1CB0F6;
      border: solid transparent;
      border-radius: 16px;
      border-width: 0 0 4px;
      bottom: -4px;
      content: "";
      left: 0;
      position: absolute;
      right: 0;
      top: 0;
      z-index: -1;
    }
    
    .button:main,
    .button:focus {
      user-select: auto;
    }
    
    .button:hover:not(:disabled) {
      filter: brightness(1.1);
      -webkit-filter: brightness(1.1);
    }
    
    .button:disabled {
      cursor: auto;
    }
    
    .button:active {
      border-width: 4px 0 0;
      background: none;
    }
  </style>
</head>
<body>
  <h1>Motor Control</h1>
  <button class="button" onclick="sendMessage('forward')">Move Forward</button>
  <button class="button" onclick="sendMessage('backward')">Move Backward</button>
  <button class="button" onclick="sendMessage('stop')">Stop Motors</button>

  <script>
    var socket = new WebSocket("ws://192.168.43.114:81/");

    socket.onopen = function(event) {
      console.log("WebSocket connection opened.");
    };

    socket.onmessage = function(event) {
      console.log("Received message: " + event.data);
      // Handle messages from the server if needed
    };

    socket.onclose = function(event) {
      if (event.wasClean) {
        console.log("Closed cleanly, code=" + event.code + ", reason=" + event.reason);
      } else {
        console.error("Connection died");
      }
    };

    socket.onerror = function(error) {
      console.error("WebSocket Error: " + error);
    };

    function sendMessage(message) {
      socket.send(message);
    }
  </script>
</body>
</html>
 socket code
