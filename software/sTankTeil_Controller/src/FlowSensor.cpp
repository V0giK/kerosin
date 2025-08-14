#include "FlowSensor.h"

// Statische Instanz initialisieren
FlowSensor* FlowSensor::instance = nullptr;

// Konstruktor
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

// Initialisierung
void FlowSensor::begin() {
    instance = this; // Instanz speichern
    //pinMode(pin, INPUT_PULLUP); // Pin als Input setzen
    pinMode(pin, INPUT); // Pin als Input setzen (externen PullUp verwenden)
    attachInterrupt(digitalPinToInterrupt(pin), pulseISR, RISING); // Interrupt registrieren
}

// PulseCount zurücksetzen
void FlowSensor::resetPulses() {
    pulses = 0;
}

// PulseCount abfragen
unsigned long FlowSensor::getPulses() {
    return pulses;
}

// Kalibrierungsfaktor setzen
void FlowSensor::setCalibration(uint16_t calibrate) {
    calibrationFactor = calibrate;
}

// Richtung setzen: Tanken oder Enttanken
void FlowSensor::setFilling(bool filling) {
    isFilling = filling;
}

// Durchflussrate und Menge berechnen (Milliliter)
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

    //return flowRate;
}

// Durchflussrate (Milliliter pro Sekunde)
float FlowSensor::getFlowRate() {
    return flowRate;
}

// Gesamt geförderte Milliliter abfragen
long FlowSensor::getTotalMilliliters() {
    return totalMilliliters;
}

// Gesamtmenge zurücksetzen
void FlowSensor::resetTotalFlow() {
    totalMilliliters = 0;
}

// // Tickcount Mode aktivierien
// void FlowSensor::aktivateTickcountMode(bool aktiv){
//     tickCountMode = aktiv;
// }