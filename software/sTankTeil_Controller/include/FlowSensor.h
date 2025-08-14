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

#ifndef FLOWSENSOR_H
#define FLOWSENSOR_H

#include <Arduino.h>

class FlowSensor {
private:
    uint8_t pin;                             // Pin für den Durchflusssensor
    volatile unsigned long pulses;           // Pulse-Zähler (volatile wegen Interrupt)
    uint16_t calibrationFactor;              // Kalibrierungsfaktor (Pulse/L)
    long totalMilliliters;                   // Gesamtmenge in Millilitern (kann positiv/negativ sein)
    bool isFilling;                          // Richtung: true = Tanken, false = Enttanken
    static FlowSensor* instance;             // Statische Instanz für Interrupt-Handler
    // bool tickCountMode;                      // im Tickcount Mode werden die Pulse nicht zurückgesetzt (nur für die Durchflusskalibrierung!)
    float flowRate;
    // Flow per sekunde betimmen
    unsigned long lastFlowTime = 0;          // Zeitstempel der letzten Messung


    static void pulseISR();                  // Statische Interrupt-Funktion

public:
    FlowSensor(uint8_t sensorPin, uint16_t calibrate = 5312); // Konstruktor
    void begin();                            // Initialisierung des Sensors
    void resetPulses();                      // PulseCount zurücksetzen
    unsigned long getPulses();               // PulseCount abfragen
    void setCalibration(uint16_t calibrate); // Kalibrierungsfaktor setzen
    void setFilling(bool filling);           // Richtung setzen: Tanken (true) oder Enttanken (false)
    void tick();                             // Durchflussrate in Millilitern pro Sekunde berechnen
    float getFlowRate();
    long getTotalMilliliters();              // Gesamt geförderte Milliliter (positiv/negativ)
    void resetTotalFlow();                   // Reset der Gesamtmenge
};

#endif // FLOWSENSOR_H
