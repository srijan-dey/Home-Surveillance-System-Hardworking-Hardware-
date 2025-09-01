#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// ================= WiFi Credentials (Wokwi) =================
const char* ssid = "Wokwi-GUEST";   // Wokwi WiFi
const char* password = "";          // No password for Wokwi-GUEST

// ================= Telegram Bot Settings =================
String BOTtoken = "8294174398:AAFnaZYAedZdHqhhuCKnZXvHyBVPSYKb1Ko"; 
String CHAT_ID  = "8093434094"; 

// ================= Sensors & Actuators =================
#define PIR_PIN   13  // PIR Sensor OUT -> GPIO13
#define MIC_PIN   34   // Microphone OUT -> GPIO34 (ADC)
#define BUZZER    14   // Buzzer -> GPIO14
#define LED       12   // LED -> GPIO12

// ================= Sound Detection Threshold =================
int soundThreshold = 800; // Adjust (0-4095 for ESP32 ADC)

// ================= Cooldown (ms) =================
unsigned long motionCooldown = 10000;
unsigned long soundCooldown  = 10000;

unsigned long lastMotionTime = 0;
unsigned long lastSoundTime  = 0;

// ================= Function to Send Text Message =================
void sendTelegramMessage(String message) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected! Cannot send message.");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure(); // skip SSL cert check

  HTTPClient http;
  String url = "https://api.telegram.org/bot" + BOTtoken +
               "/sendMessage?chat_id=" + CHAT_ID + "&text=" + message;

  http.begin(client, url);
  http.setTimeout(10000); // 10 sec timeout

  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("Message sent! HTTP code: %d\n", httpCode);
  } else {
    Serial.printf("Error sending message. HTTP code: %d\n", httpCode);
  }
  http.end();
}

// ================= Alert Function (Blink + Beep) =================
void triggerAlarm() {
  for (int i = 0; i < 6; i++) { // Blink/beep 6 times
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(300);
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);
    delay(300);
  }
}

// ================= Setup =================
void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Connect to Wokwi WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password, 6);  // Channel 6 needed for Wokwi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected!");

  // Send startup message
  sendTelegramMessage("✅ Device online in Wokwi! Monitoring motion & sound...");
  Serial.println("System is monitoring motion & sound...");
}

// ================= Loop =================
void loop() {
  unsigned long now = millis();

  // ---- Motion Detection ----
  int pirState = digitalRead(PIR_PIN);
  if (pirState == HIGH && (now - lastMotionTime > motionCooldown)) {
    Serial.println("🚨 Motion detected! Sending alert...");
    sendTelegramMessage("⚠️ Motion detected in Wokwi!");
    triggerAlarm();
    lastMotionTime = now;
  }

  // ---- Loud Sound Detection ----
  int soundValue = analogRead(MIC_PIN);
  if (soundValue > soundThreshold && (now - lastSoundTime > soundCooldown)) {
    Serial.printf("🔊 Loud sound detected! Value: %d\n", soundValue);
    sendTelegramMessage("⚠️ Loud sound detected in Wokwi!");
    triggerAlarm();
    lastSoundTime = now;
  }

}
