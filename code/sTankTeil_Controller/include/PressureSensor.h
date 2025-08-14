#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

#include <Arduino.h>

class PressureSensor {
private:
    uint8_t pin;              // Analoger Pin des Sensors
    float vcc;                // Versorgungsspannung (z.B. 5V)
    float maxPressure_kPa;    // Maximaler Druck in kPa
    int numSamples;           // Anzahl der Messungen für Mittelwert
    float calibVoltage;       // Spannung am analog Pin bei 0 mbar
    float lastMbar;
    float curMbar;

    // Private Hilfsfunktion zur Spannungsberechnung
    float readAverageVoltage();

public:
    // Konstruktor mit Defaultwerten (maxPressure in PSI)
    PressureSensor(uint8_t sensorPin, float supplyVoltage = 5.0, float maxPressure_PSI = 30.0, int samples = 6);

    // Setter-Methoden für Parameter
    void setVCC(float supplyVoltage);
    void setMaxPressure_PSI(float maxPressure_PSI);
    void setNumSamples(int samples);

    void calibSensor();
    // Methode zur Druckberechnung (mbar)
    void tick();
    float getPressureMbar(bool sign = false);

    float getDiffMbar();
};

#endif
