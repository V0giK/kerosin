#ifndef HBRIDGEMOTOR_H
#define HBRIDGEMOTOR_H

class HBridgeMotor {
private:
    int pin1;  // Pin 1 des Motors
    int pin2;  // Pin 2 des Motors
    int currentSpeed;  // Aktuelle Geschwindigkeit (0-255)
    bool isForward;  // Richtung des Motors
    bool isRunning;     // Motor dreht sich

public:
    
    HBridgeMotor(int, int);                                             // Konstruktor
    void setSpeed(int percent);                                         // Geschwindigkeit setzen (ohne Starten)
    void startForward(int rampTime = 1000);                             // Motor vorwärts starten mit Sanftanlauf
    void startBackward(int rampTime = 1000);                            // Motor rückwärts starten mit Sanftanlauf
    void forwardRamp(int targetPercent = 100, int rampTime = 1000);     // Sanftanlauf für Vorwärtsbewegung
    void backwardRamp(int targetPercent = 100, int rampTime = 1000);    // Sanftanlauf für Rückwärtsbewegung
    void adjustSpeed(int percent);                                      // Geschwindigkeit während der Bewegung anpassen
    void forward(int percent = 100);                                    // Motor vorwärts laufen lassen (Wert in % übergeben)
    void backward(int percent = 100);                                   // Motor rückwärts laufen lassen (Wert in % übergeben)
    void stop();                                                        // Motor anhalten (freilaufend)
    void brake();                                                       // Motor bremsen
    bool isOn();                                                        // Motor ist eingeschalten
};

#endif /* HBRIDGEMOTOR_H */

