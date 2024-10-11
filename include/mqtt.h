#ifndef MQTT_H
#define MQTT_H

#include <WiFi.h>
#include <PubSubClient.h>

const char* mqtt_server = "";
const int mqtt_port = 1883; // Porta padrão do MQTT
const char* mqtt_user = ""; // Se necessário
const char* mqtt_password = ""; // Se necessário
const int relayPin = 27;

WiFiClient espClient;
PubSubClient client(espClient);

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    
    if (client.connect("esp32Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado ao MQTT");
      
      // Inscreva-se em tópicos se necessário
      client.subscribe("relay/control");
      client.subscribe("comando");
    } else {
      Serial.print("Falha na conexão. Código de erro: ");
      Serial.print(client.state());
      delay(5000); // Tentar novamente após 5 segundos
    }
  }
}
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico ");
  Serial.print(topic);
  Serial.print(": ");

  String command = "";
  for (unsigned int i = 0; i < length; i++) {
    command += (char)payload[i];
  }
  Serial.println(command);

  // Controlar o relé com base no comando recebido
  if (command == "ligar") {
    digitalWrite(relayPin, LOW); // Ligar o relé
    client.publish("comando", "ON");
  } else if (command == "desligar") {
    digitalWrite(relayPin, HIGH); // Desligar o relé
    client.publish("comando", "OFF");
  }
}
#endif