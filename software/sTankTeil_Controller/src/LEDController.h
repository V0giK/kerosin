#pragma once

#include <Arduino.h>

class LEDController {
private:
    int pin;
    unsigned long lastBlinkTime;
    unsigned long blinkIntervalLowBattery;
    unsigned long blinkIntervalError;
    bool isBlinkingLowBattery;
    bool isBlinkingError;
    bool ledState;
    
public:
    LEDController(int ledPin) {
        pin = ledPin;
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH); // LED initially on
        lastBlinkTime = 0;
        blinkIntervalLowBattery = 1000;  // 1 second for low battery blink
        blinkIntervalError = 200;       // 0.2 second for error blink
        isBlinkingLowBattery = false;
        isBlinkingError = false;
        ledState = true; // LED initially on
    }

    // Start blinking for low battery
    void startLowBatteryBlinking() {
        isBlinkingLowBattery = true;
        isBlinkingError = false; // Stop error blinking if active
    }

    // Stop blinking for low battery
    void stopLowBatteryBlinking() {
        isBlinkingLowBattery = false;
        updateLEDState(); // Ensure LED is always on if no blinking is active
    }

    // Start blinking for error
    void startErrorBlinking() {
        isBlinkingError = true;
        isBlinkingLowBattery = false; // Stop low battery blinking if active
    }

    // Stop blinking for error
    void stopErrorBlinking() {
        isBlinkingError = false;
        updateLEDState(); // Ensure LED is always on if no blinking is active
    }

    // Main update function to handle blinking and normal LED state
    void update() {
        unsigned long currentMillis = millis();

        // Handle blinking for low battery
        if (isBlinkingLowBattery && currentMillis - lastBlinkTime >= blinkIntervalLowBattery) {
            lastBlinkTime = currentMillis;
            ledState = !ledState; // Toggle LED state
            digitalWrite(pin, ledState ? HIGH : LOW);
        }

        // Handle blinking for error
        else if (isBlinkingError && currentMillis - lastBlinkTime >= blinkIntervalError) {
            lastBlinkTime = currentMillis;
            ledState = !ledState; // Toggle LED state
            digitalWrite(pin, ledState ? HIGH : LOW);
        }
        // Handle case when no blinking is active (LED should stay ON)
        else if (!isBlinkingLowBattery && !isBlinkingError) {
            digitalWrite(pin, HIGH); // LED stays ON
        }
    }

private:
    // Ensure LED is ON when no blinking is active
    void updateLEDState() {
        if (!isBlinkingLowBattery && !isBlinkingError) {
            digitalWrite(pin, HIGH); // Ensure the LED is ON when no blinking is active
        }
    }
};

// // Usage example
// LEDController ledController(13);  // Assuming LED is connected to pin 13

// void setup() {
//     Serial.begin(9600);
// }

// void loop() {
//     ledController.update();

//     // Simulate low battery condition for 5 seconds
//     ledController.startLowBatteryBlinking();
//     delay(5000);  // Wait 5 seconds
//     ledController.stopLowBatteryBlinking();
    
//     // Simulate error condition for 5 seconds
//     ledController.startErrorBlinking();
//     delay(5000);  // Wait 5 seconds
//     ledController.stopErrorBlinking();
// }
