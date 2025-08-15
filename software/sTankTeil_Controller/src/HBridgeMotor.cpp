/*
 * Hauptprogramm zur Steuerung einer Arduino-basierten Tanklösung
 *
 * Copyright (C) 2025 V0giK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include "HBridgeMotor.h"

#define MIN_PWR 40

/**
 * Konstruktor für den HBridgeMotor
 * @param pin1 Pin 1 des Motors
 * @param pin2 Pin 2 des Motors
 */
HBridgeMotor::HBridgeMotor(int pin1, int pin2) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->currentSpeed = 0;
    this->isForward = true;

    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
}

/**
 * Setzt die Geschwindigkeit des Motors (ohne Starten)
 * @param percent Der gewünschte Geschwindigkeitswert in Prozent (0-100)
 */
void HBridgeMotor::setSpeed(int percent) {
    percent = constrain(percent, 0, 100);  // Prozentwert auf 0-100 begrenzen
    currentSpeed = map(percent, 0, 100, 0, 255);  // Prozent in PWM-Wert umwandeln
}

/**
 * Startet den Motor vorwärts mit Sanftanlauf
 * @param rampTime Die Zeit, die für den Sanftanlauf benötigt wird
 */
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

/**
 * Startet den Motor rückwärts mit Sanftanlauf
 * @param rampTime Die Zeit, die für den Sanftanlauf benötigt wird
 */
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

/**
 * Startet den Motor vorwärts mit Sanftanlauf
 * @param targetPercent Der Zielwert für die Geschwindigkeit in Prozent (0-100)
 * @param rampTime Die Zeit, die für den Sanftanlauf benötigt wird
 */
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

/**
 * Startet den Motor rückwärts mit Sanftanlauf
 * @param targetPercent Der Zielwert für die Geschwindigkeit in Prozent (0-100)
 * @param rampTime Die Zeit, die für den Sanftanlauf benötigt wird
 */
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

/**
 * Passt die Geschwindigkeit des Motors während der Bewegung an
 * @param percent Der gewünschte Geschwindigkeitswert in Prozent (0-100)
 */
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

/**
 * Startet den Motor vorwärts mit der angegebenen Geschwindigkeit
 * @param percent Der gewünschte Geschwindigkeitswert in Prozent (0-100)
 */
void HBridgeMotor::forward(int percent) {
    percent = constrain(percent, MIN_PWR, 100);  // Prozentwert auf 0-100 begrenzen
    int speed = map(percent, 0, 100, 0, 255);  // Prozent in PWM-Wert umwandeln
    analogWrite(pin1, speed);  // HIGH/PWM
    digitalWrite(pin2, LOW);   // LOW
    currentSpeed = speed;
    isForward = true;
    isRunning = true;
}

/**
 * Startet den Motor rückwärts mit der angegebenen Geschwindigkeit
 * @param percent Der gewünschte Geschwindigkeitswert in Prozent (0-100)
 */
void HBridgeMotor::backward(int percent) {
    percent = constrain(percent, MIN_PWR, 100);  // Prozentwert auf 0-100 begrenzen
    int speed = map(percent, 0, 100, 0, 255);  // Prozent in PWM-Wert umwandeln
    digitalWrite(pin1, LOW);  // LOW
    analogWrite(pin2, speed); // HIGH/PWM
    currentSpeed = speed;
    isForward = false;
    isRunning = true;
}

/**
 * Motor anhalten (freilaufend)
 */
void HBridgeMotor::stop() {
    digitalWrite(pin1, LOW);  // LOW
    digitalWrite(pin2, LOW);  // LOW
    currentSpeed = 0;
    isRunning = false;
}

/**
 *  Motor bremsen 
 */
void HBridgeMotor::brake() {
    digitalWrite(pin1, HIGH);  // HIGH
    digitalWrite(pin2, HIGH);  // HIGH
    currentSpeed = 0;
    isRunning = false;
}

/**
 * Überprüft, ob der Motor läuft
 * @return true, wenn der Motor läuft, andernfalls false
 */
bool HBridgeMotor::isOn() {
    return isRunning;
}