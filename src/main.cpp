#include <Arduino.h>

#include "settings.h"
#include "wifiConnection.h"
#include "mqttConnection.h"

long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 11;


void setup() {
  Serial.begin(115200);

  wifiSetup();
  mqttSetup();

  pinMode(LED_BUILTIN, OUTPUT);
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


