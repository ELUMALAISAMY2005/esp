#include <WiFi.h>
#include <WebServer.h>

// Replace with your Wi-Fi credentials
const char* ssid = "Airtel_Homies04";
const char* password = "password04";

WebServer server(80); // ESP32 Web Server
int pin = 2;          // Onboard LED pin
int blinkCount = 0;   // Tracks how many times the LED has blinked
int targetBlinks = 0; // Number of blinks requested
bool blinking = false;

void blinkLED() {
  if (targetBlinks > 0 && blinking) {
    digitalWrite(pin, HIGH);
    delay(500); // ON for 500ms
    digitalWrite(pin, LOW);
    delay(500); // OFF for 500ms
    blinkCount++;
    if (blinkCount >= targetBlinks) {
      blinking = false;
      targetBlinks = 0;
    }
  }
}

void handleBlink() {
  if (server.hasArg("count")) {
    targetBlinks = server.arg("count").toInt();
    blinkCount = 0;
    blinking = true;
    server.send(200, "text/plain", "Blinking started");
  } else {
    server.send(400, "text/plain", "Missing parameter: count");
  }
}

void handleStatus() {
  String status = "Blink Count: " + String(blinkCount) +
                  "\nTarget Blinks: " + String(targetBlinks) +
                  "\nBlinking: " + String(blinking ? "Yes" : "No");
  server.send(200, "text/plain", status);
}

void setup() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.println(WiFi.localIP());

  server.on("/blink", handleBlink);
  server.on("/status", handleStatus);

  server.begin();
}

void loop() {
  server.handleClient();
  if (blinking) {
    blinkLED();
  }
}
