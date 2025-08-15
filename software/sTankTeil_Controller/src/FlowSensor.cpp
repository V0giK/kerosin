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

#include "FlowSensor.h"

// Statische Instanz initialisieren
FlowSensor* FlowSensor::instance = nullptr;

/**
 * Konstruktor für den Durchflusssensor
 * @param sensorPin Der Pin, an dem der Sensor angeschlossen ist
 * @param calibrate Der Kalibrierungsfaktor für den Sensor
 */
FlowSensor::FlowSensor(uint8_t sensorPin, uint16_t calibrate) {
    pin = sensorPin;
    calibrationFactor = calibrate; // Standard 450 Pulse/Liter
    pulses = 0;
    totalMilliliters = 0;
    isFilling = true; // Standard: Tanken
    // tickCountMode = false;
}

// Interrupt-Wrapper-Funktion
void FlowSensor::pulseISR() {
    if (instance) {
        instance->pulses++; // Pulse inkrementieren
    }
}

/**
 * Initialisiert den Durchflusssensor
 */
void FlowSensor::begin() {
    instance = this; // Instanz speichern
    //pinMode(pin, INPUT_PULLUP); // Pin als Input setzen
    pinMode(pin, INPUT); // Pin als Input setzen (externen PullUp verwenden)
    attachInterrupt(digitalPinToInterrupt(pin), pulseISR, RISING); // Interrupt registrieren
}

/**
 * Setzt den PulseCount zurück
 */
void FlowSensor::resetPulses() {
    pulses = 0;
}

/**
 * Gibt die aktuelle Anzahl der Pulse zurück
 * @return Die Anzahl der Pulse
 */
unsigned long FlowSensor::getPulses() {
    return pulses;
}

/**
 * Setzt den Kalibrierungsfaktor für den Durchflusssensor
 * @param calibrate Der Kalibrierungsfaktor
 */
void FlowSensor::setCalibration(uint16_t calibrate) {
    calibrationFactor = calibrate;
}

/**
 * Setzt die Richtung des Durchflusssensors
 * @param filling true für Tanken, false für Enttanken
 */
void FlowSensor::setFilling(bool filling) {
    isFilling = filling;
}

/**
 * Berechnet die Durchflussrate und Menge (Milliliter)
 */
void FlowSensor::tick() {
    float milli = (pulses / (float)calibrationFactor) * 1000.0; // Pulse -> Liter -> Milliliter
    if (isFilling) {
        totalMilliliters += milli; // Addieren beim Tanken
    } else {
        totalMilliliters -= milli; // Subtrahieren beim Enttanken
    }
    pulses = 0; // Pulse für nächste Messung zurücksetzen

    unsigned long zeit = millis() - lastFlowTime;
    flowRate = (milli / (float)zeit) * 1000.0;
    lastFlowTime = millis();
}

/**
 * Gibt die aktuelle Durchflussrate zurück
 * @return Die Durchflussrate in Millilitern pro Sekunde
 */
float FlowSensor::getFlowRate() {
    return flowRate;
}

/**
 * Gibt die gesamte geförderte Menge in Millilitern zurück
 * @return Die gesamte geförderte Menge in Millilitern
 */
long FlowSensor::getTotalMilliliters() {
    return totalMilliliters;
}

/**
 * Setzt die gesamte geförderte Menge zurück
 */
void FlowSensor::resetTotalFlow() {
    totalMilliliters = 0;
}

// // Tickcount Mode aktivierien
// void FlowSensor::aktivateTickcountMode(bool aktiv){
//     tickCountMode = aktiv;
// }