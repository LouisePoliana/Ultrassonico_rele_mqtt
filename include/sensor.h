#ifndef SENSOR_H
#define SENSOR_H
#include <Arduino.h>

const int trigPin = 5;
const int echoPin = 18;

long measureDistance() {
  // Enviar pulso de 10 microsegundos
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Ler o tempo de resposta do pino Echo
  long duration = pulseIn(echoPin, HIGH);

  // Converter o tempo em distância (cm)
  long distance = (duration * 0.0344) / 2; // Calcula a distância em cm

  return distance;
}
#endif