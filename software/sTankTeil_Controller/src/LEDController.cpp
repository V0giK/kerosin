#include "LEDController.h"

LEDController::LEDController(int ledPin) {
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

void LEDController::startLowBatteryBlinking() {
    isBlinkingLowBattery = true;
    isBlinkingError = false; // Stop error blinking if active
}

void LEDController::stopLowBatteryBlinking() {
    isBlinkingLowBattery = false;
    updateLEDState(); // Ensure LED is always on if no blinking is active
}

void LEDController::startErrorBlinking() {
    isBlinkingError = true;
    isBlinkingLowBattery = false; // Stop low battery blinking if active
}

void LEDController::stopErrorBlinking() {
    isBlinkingError = false;
    updateLEDState(); // Ensure LED is always on if no blinking is active
}

void LEDController::update() {
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

void LEDController::updateLEDState() {
    if (!isBlinkingLowBattery && !isBlinkingError) {
        digitalWrite(pin, HIGH); // Ensure the LED is ON when no blinking is active
    }
}
