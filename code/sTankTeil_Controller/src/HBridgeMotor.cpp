#include <Arduino.h>
#include "HBridgeMotor.h"

#define MIN_PWR 40

// Konstruktor
HBridgeMotor::HBridgeMotor(int pin1, int pin2) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->currentSpeed = 0;
    this->isForward = true;

    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
}

// Geschwindigkeit setzen (ohne Starten)
void HBridgeMotor::setSpeed(int percent) {
    percent = constrain(percent, 0, 100);  // Prozentwert auf 0-100 begrenzen
    currentSpeed = map(percent, 0, 100, 0, 255);  // Prozent in PWM-Wert umwandeln
}

// Motor vorwärts starten mit Sanftanlauf
void HBridgeMotor::startForward(int rampTime) {
    int initialSpeed = 0;
    for (int speed = initialSpeed; speed <= currentSpeed; speed++) {
        analogWrite(pin1, speed);  // HIGH/PWM
        digitalWrite(pin2, LOW);   // LOW
        delay(rampTime / currentSpeed);  // Zeitintervall für die Rampe
    }
    isForward = true;
    isRunning = true;
}

// Motor rückwärts starten mit Sanftanlauf
void HBridgeMotor::startBackward(int rampTime) {
    int initialSpeed = 0;
    for (int speed = initialSpeed; speed <= currentSpeed; speed++) {
        digitalWrite(pin1, LOW);  // LOW
        analogWrite(pin2, speed); // HIGH/PWM
        delay(rampTime / currentSpeed);  // Zeitintervall für die Rampe
    }
    isForward = false;
    isRunning = true;
}

// Sanftanlauf für Vorwärtsbewegung
void HBridgeMotor::forwardRamp(int targetPercent, int rampTime) {
    targetPercent = constrain(targetPercent, MIN_PWR, 100);  // Zielprozentwert auf 35-100 begrenzen
    int targetSpeed = map(targetPercent, 0, 100, 0, 255);  // Zielprozent in PWM-Wert umwandeln

    if(currentSpeed == 0) {
        currentSpeed = map(MIN_PWR, 0, 100, 0, 255);
    }
    for (int speed = currentSpeed; speed <= targetSpeed; speed++) {
        analogWrite(pin1, speed);  // HIGH/PWM
        digitalWrite(pin2, LOW);   // LOW
        delay(rampTime / (targetSpeed - currentSpeed));  // Zeitintervall für die Rampe
    }
    currentSpeed = targetSpeed;
    isForward = true;
    isRunning = true;
}

// Sanftanlauf für Rückwärtsbewegung
void HBridgeMotor::backwardRamp(int targetPercent, int rampTime) {
    targetPercent = constrain(targetPercent, MIN_PWR, 100);  // Zielprozentwert auf 35-100 begrenzen
    int targetSpeed = map(targetPercent, 0, 100, 0, 255);  // Zielprozent in PWM-Wert umwandeln

    if(currentSpeed == 0) {
        currentSpeed = map(MIN_PWR, 0, 100, 0, 255);
    }
    for (int speed = currentSpeed; speed <= targetSpeed; speed++) {
        digitalWrite(pin1, LOW);  // LOW
        analogWrite(pin2, speed); // HIGH/PWM
        delay(rampTime / (targetSpeed - currentSpeed));  // Zeitintervall für die Rampe
    }
    currentSpeed = targetSpeed;
    isForward = false;
    isRunning = true;
}

// Geschwindigkeit während der Bewegung anpassen
void HBridgeMotor::adjustSpeed(int percent) {
    percent = constrain(percent, MIN_PWR, 100);  // Prozentwert auf 0-100 begrenzen
    int newSpeed = map(percent, 0, 100, 0, 255);  // Prozent in PWM-Wert umwandeln

    if (isForward) {
        analogWrite(pin1, newSpeed);  // HIGH/PWM
        digitalWrite(pin2, LOW);     // LOW
    } else {
        digitalWrite(pin1, LOW);    // LOW
        analogWrite(pin2, newSpeed); // HIGH/PWM
    }

    currentSpeed = newSpeed;
}

// Motor vorwärts laufen lassen (Wert in % übergeben)
void HBridgeMotor::forward(int percent) {
    percent = constrain(percent, MIN_PWR, 100);  // Prozentwert auf 0-100 begrenzen
    int speed = map(percent, 0, 100, 0, 255);  // Prozent in PWM-Wert umwandeln
    analogWrite(pin1, speed);  // HIGH/PWM
    digitalWrite(pin2, LOW);   // LOW
    currentSpeed = speed;
    isForward = true;
    isRunning = true;
}

// Motor rückwärts laufen lassen (Wert in % übergeben)
void HBridgeMotor::backward(int percent) {
    percent = constrain(percent, MIN_PWR, 100);  // Prozentwert auf 0-100 begrenzen
    int speed = map(percent, 0, 100, 0, 255);  // Prozent in PWM-Wert umwandeln
    digitalWrite(pin1, LOW);  // LOW
    analogWrite(pin2, speed); // HIGH/PWM
    currentSpeed = speed;
    isForward = false;
    isRunning = true;
}

// Motor anhalten (freilaufend)
void HBridgeMotor::stop() {
    digitalWrite(pin1, LOW);  // LOW
    digitalWrite(pin2, LOW);  // LOW
    currentSpeed = 0;
    isRunning = false;
}

// Motor bremsen
void HBridgeMotor::brake() {
    digitalWrite(pin1, HIGH);  // HIGH
    digitalWrite(pin2, HIGH);  // HIGH
    currentSpeed = 0;
    isRunning = false;
}

bool HBridgeMotor::isOn() {
    return isRunning;
}