#include <WiFi.h>
#include <HTTPClient.h>

// --- SETTINGS ---
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

String SCRIPT_URL = "YOUR_SCRIPT_URL"; 

#define DEVICE_NAME "ESP32_Workshop"

const long SEND_INTERVAL_MS = 30000; 
unsigned long lastSendMillis = 0;

#define IR1_PIN 36
#define ledGreen1 32
#define ledRed1 33

#define IR2_PIN 4 
#define ledGreen2 23
#define ledRed2 5

void setup() {
  pinMode(IR1_PIN, INPUT);
  pinMode(ledGreen1, OUTPUT);
  pinMode(ledRed1, OUTPUT);
  pinMode(IR2_PIN, INPUT);
  pinMode(ledGreen2, OUTPUT);
  pinMode(ledRed2, OUTPUT);
  
  Serial.begin(115200);
  connectWiFi(); 
}

void loop() {
  if (millis() - lastSendMillis >= SEND_INTERVAL_MS) {
    lastSendMillis = millis();

    int valIR1 = digitalRead(IR1_PIN);
    int valIR2 = digitalRead(IR2_PIN);

    Serial.print("Read Sensor IR1: "); Serial.println(valIR1);
    Serial.print("Read Sensor IR2: "); Serial.println(valIR2);

    sendDataToSheet(valIR1, valIR2);
  }
  
  int currentValIR1 = digitalRead(IR1_PIN);
  int currentValIR2 = digitalRead(IR2_PIN);
  digitalWrite(ledRed1, currentValIR1 == 0 ? HIGH : LOW);
  digitalWrite(ledGreen1, currentValIR1 != 0 ? HIGH : LOW);
  digitalWrite(ledRed2, currentValIR2 == 1 ? HIGH : LOW);
  digitalWrite(ledGreen2, currentValIR2 != 1 ? HIGH : LOW);
  delay(200);
  digitalWrite(ledRed1, LOW);
  digitalWrite(ledGreen1, LOW);
  digitalWrite(ledRed2, LOW);
  digitalWrite(ledGreen2, LOW);
}

void connectWiFi() {
  Serial.print("Connecting to "); Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println(WiFi.localIP());
}

void sendDataToSheet(int ir1_val, int ir2_val) {
  HTTPClient http;

  String urlParams = "?device=" + String(DEVICE_NAME) +
                     "&IR1=" + String(ir1_val) +
                     "&IR2=" + String(ir2_val) +
                     "&reason=PeriodicReading";

  String fullUrl = SCRIPT_URL + urlParams;

  Serial.print("Sending data: ");
  Serial.println(fullUrl);

  http.begin(fullUrl.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("HTTP Response: " + String(httpCode) + " | Payload: " + payload);
  } else {
    Serial.println("Error sending: " + String(httpCode));
  }
  http.end();
}