#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include "sensor.h"
#include "mqtt.h"

// Configurações do Wi-Fi
const char* ssid = "";
const char* password = "";
///ddd

void setupWiFi() {
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Conectado ao Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Inicializa o relé desligado

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Conectar ao Wi-Fi
  setupWiFi();

  // Configurar o cliente MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);

  // Conectar ao broker MQTT
  reconnectMQTT();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  
  client.loop();

  // Medir a distância e publicar no tópico
  static unsigned long lastMeasurement = 0;
  unsigned long now = millis();
  if (now - lastMeasurement > 5000) { // Medir e publicar a cada 5 segundos
    lastMeasurement = now;

    long distance = measureDistance();
    String distanceStr = String(distance) + " cm";
    client.publish("sensor/distance", distanceStr.c_str());

    String statusMessage;
    if (distance <= 15) {
      statusMessage = "Objeto muito perto";
      digitalWrite(relayPin, LOW); // Ligar o relé
      client.publish("relay/status", "Relé ligado");
    } else if (distance >= 20) {
      statusMessage = "Objeto longe";
      digitalWrite(relayPin, HIGH); // Desligar o relé
      client.publish("relay/status", "Relé desligado");
    } else {
     statusMessage = "Distância média: " + distanceStr;
    }

    client.publish("sensor/status", statusMessage.c_str());

    Serial.print("Distância: ");
    Serial.print(distance);
    Serial.println(" cm");
    Serial.print("Status: ");
    Serial.println(statusMessage);
  }
}