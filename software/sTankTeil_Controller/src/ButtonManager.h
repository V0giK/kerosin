#include <Bounce2.h>

class ButtonManager {
public:
    // Typdefinition für die Callback-Funktionen
    typedef void (*Callback)();

    ButtonManager(int pin1, int pin2, unsigned long longPressDuration = 2000)
        : button1Pin(pin1), button2Pin(pin2), longPressDuration(longPressDuration), bothButtonsPressed(false), pressStartTime(0) {
        button1.attach(button1Pin);
        button1.interval(10);  // Entprellzeit für Taster 1

        button2.attach(button2Pin);
        button2.interval(10);  // Entprellzeit für Taster 2
    }

    void begin() {
        pinMode(button1Pin, INPUT); // es weird ein externer Pullup verwendet
        pinMode(button2Pin, INPUT); // es weird ein externer Pullup verwendet
        bActive = true;
    }

    // Setzen der Callback-Funktionen
    void onButtonInClick(Callback callback) {
        button1ClickCallback = callback;
    }

    void onButtonOutClick(Callback callback) {
        button2ClickCallback = callback;
    }

    void onBothButtonsLongPress(Callback callback) {
        bothButtonsLongPressCallback = callback;
    }

    void update() {
        if(!bActive) return;

        button1.update();
        button2.update();

        if (button1.read() == LOW && button2.read() == LOW) {
            if (!bothButtonsPressed) {
                pressStartTime = millis();
                bothButtonsPressed = true;
            }

            if (millis() - pressStartTime >= longPressDuration) {
                if (!longPressEventTriggered && bothButtonsLongPressCallback) {
                    bothButtonsLongPressCallback();
                    longPressEventTriggered = true;
                }
            }
        } else {
            if (bothButtonsPressed) {
                if(button1.read() == HIGH && button2.read() == HIGH) {
                    bothButtonsPressed = false;
                    longPressEventTriggered = false;
                }
            } else {
                if (!longPressEventTriggered) {
                    if (button1.rose() && button1ClickCallback) {
                        button1ClickCallback();
                    }
                    if (button2.rose() && button2ClickCallback) {
                        button2ClickCallback();
                    }
                }
            }
        }
    }

private:
    int button1Pin, button2Pin;
    unsigned long longPressDuration;
    unsigned long pressStartTime;
    bool bothButtonsPressed = false;
    bool longPressEventTriggered = false;
    bool bActive = false;
    Bounce button1 = Bounce();
    Bounce button2 = Bounce();

    // Funktionszeiger für die Callbacks
    Callback button1ClickCallback = nullptr;
    Callback button2ClickCallback = nullptr;
    Callback bothButtonsLongPressCallback = nullptr;
};
