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
#include "PressureSensor.h"

#define PSI_TO_KPA 6.89476 // Umrechnungsfaktor von PSI in kPa

// Konstruktor: MaxPressure wird in PSI angegeben und intern umgerechnet
PressureSensor::PressureSensor(uint8_t sensorPin, float supplyVoltage, float maxPressure_PSI, int samples) {
    pin = sensorPin;
    vcc = supplyVoltage;
    maxPressure_kPa = maxPressure_PSI * PSI_TO_KPA;
    numSamples = samples;
    lastMbar = 0;
    curMbar = 0;
    pinMode(pin, INPUT);
    // calibSensor() verursacht hier Probleme - Programm bleibt hier stehen
    //calibSensor();
}

// Private Methode: Mittelwert der Spannung berechnen
float PressureSensor::readAverageVoltage() {
    float totalVoltage = 0.0;

    for (int i = 0; i < numSamples; i++) {
        int rawValue = analogRead(pin);
        float voltage = (rawValue / 1023.0) * vcc;
        totalVoltage += voltage;
        delay(10); // Kurze Verzögerung
    }

    return (totalVoltage / numSamples) - calibVoltage; // Mittelwert zurückgeben
}

// Methode: Spannung bei 0 mbar ermitteln
void PressureSensor::calibSensor() {
    float totalVoltage = 0.0;
    for (int i = 0; i < numSamples; i++) {
        int rawValue = analogRead(pin);
        float voltage = (rawValue / 1023.0) * vcc;
        totalVoltage += voltage;
        delay(10); // Kurze Verzögerung
    }

    calibVoltage = (totalVoltage / numSamples) - 0.2; // Mittelwert zurückgeben
    curMbar  = lastMbar = 0.00;
}

// Setter-Methode für VCC
void PressureSensor::setVCC(float supplyVoltage) {
    vcc = supplyVoltage;
}

// Setter-Methode für MaxPressure (in PSI)
void PressureSensor::setMaxPressure_PSI(float maxPressure_PSI) {
    maxPressure_kPa = maxPressure_PSI * PSI_TO_KPA;
}

// Setter-Methode für numSamples
void PressureSensor::setNumSamples(int samples) {
    numSamples = samples;
}

// Öffentliche Methode: Druck in mbar (mit Vorzeichen?)
float PressureSensor::getPressureMbar(bool sign) {
    if(sign) return floor(curMbar);

    return abs(floor(curMbar));
}

// Öffentliche Methode: Druck in mbar berechnen
void PressureSensor::tick() {
    float voltage = readAverageVoltage();

    // Laut Datenblatt: V_out = 0.2V + (Druck / Max_Druck) * 4.5V
    // => Druck = ((V_out - 0.2) / 4.5) * maxPressure_kPa
    // Druckberechnung basierend auf Sensordatenblatt
    float pressure_kPa = ((voltage - 0.2) / 4.5) * maxPressure_kPa;

    lastMbar = curMbar;
    curMbar = pressure_kPa * 10; // kPa in mbar umrechnen
}

float PressureSensor::getDiffMbar() {
    return abs(floor(lastMbar - curMbar));

}