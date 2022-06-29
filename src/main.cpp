#include <Arduino.h>
#include "time.h"

#include "settings.h"
#include "wifiConnection.h"
#include "mqttConnection.h"

long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 11;

const char* ntpServer = "south-america.pool.ntp.org";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


void setup() {
  Serial.begin(115200);

  wifiSetup();
  mqttSetup();

  pinMode(LED_BUILTIN, OUTPUT);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}


void handleMqttReceived (char* topic, String message) {

  if (String(topic) == "esp32/output") {
      Serial.print("Changing output to ");
      if(message == "on"){
        Serial.println("on");
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else if(message == "off"){
        Serial.println("off");
        digitalWrite(LED_BUILTIN, LOW);
      }
    }

}

void refreshLoop() {
    // Convert the value to a char array
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);

    mqttSend("esp32/temperature", tempString);
}

void loop() {
  mqttLoop();

  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    
    refreshLoop();
  }
}


