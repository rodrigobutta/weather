#ifndef MQTT_CONNECTION_H
  #define MQTT_CONNECTION_H

  #include <Arduino.h>
  #include <Wire.h>
  #include <PubSubClient.h>
  #include "settings.h"
  #include "wifiConnection.h"

  WiFiClient espClient;
  PubSubClient client(espClient);

  void mqttCallback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;
    
    for (int i = 0; i < length; i++) {
      Serial.print((char)message[i]);
      messageTemp += (char)message[i];
    }
    Serial.println();

    if (String(topic) == "esp32/output") {
      Serial.print("Changing output to ");
      if(messageTemp == "on"){
        Serial.println("on");
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else if(messageTemp == "off"){
        Serial.println("off");
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
  }

  void mqttReconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("ESP8266Client",MQTT_USER,MQTT_PASSWORD)) {
        Serial.println("MQTT connected");
        // Subscribe
        client.subscribe("esp32/output");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(10000);
      }
    }
  }

  void mqttSend(char* topic, char* message) {
      Serial.print("MQTT SEND: ");
      Serial.println(message);
      client.publish(topic, message);
  }


  void mqttSetup() {
    
    client.setServer(MQTT_HOST, 1883);
    client.setCallback(mqttCallback);

  }


  void mqttLoop() {
    if (!client.connected()) {
      mqttReconnect();
    }
    client.loop();
  }

  #endif