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

class BuzzerManager {
private:
    const int buzzerPin;
    unsigned long reminderInterval;
    const unsigned long reminderRepeatInterval; // Intervall für wiederholten Erinnerungston
    unsigned long lastActivityTime;
    unsigned long lastReminderTime; // Zeit des letzten Erinnerungstons
    bool bActive;

public:
    // Konstruktor
    BuzzerManager(int pin, unsigned long interval, unsigned long repeatInterval)
        : buzzerPin(pin), reminderInterval(interval), reminderRepeatInterval(repeatInterval), lastActivityTime(0), lastReminderTime(0), bActive(true) {}

    // Initialisierung des Buzzers
    void begin() {
        pinMode(buzzerPin, OUTPUT);
        lastActivityTime = millis();
    }

    void setReminderSeconds(int inakivSec) {
        if(inakivSec != 0 && inakivSec <= 30) inakivSec = 30; // Inaktivität min. 30 Sekunden
        reminderInterval = inakivSec * 1000L;
    }

    void setActive(bool active) {
        bActive = active;
    }

    // Aktualisiert die Aktivität (z. B. bei einem Ereignis)
    void updateActivity() {
        lastActivityTime = millis();
        lastReminderTime = 0; // Zurücksetzen der letzten Erinnerung
    }

    // Prüft, ob die Erinnerung abgespielt werden soll
    void checkReminder() {
        if(reminderInterval == 0 ) return; // Abschalterinnerung deaktiviert

        unsigned long currentTime = millis();

        if (currentTime - lastActivityTime > reminderInterval) {
            if (lastReminderTime == 0 || currentTime - lastReminderTime > reminderRepeatInterval) {
                playReminderTone();
                lastReminderTime = currentTime;
            }
        }
    }

    // Positive Tonfolge
    void playPositiveTone() {
        if(!bActive) return;
        int tones[] = {400, 600, 800};
        playToneSequence(tones, 3, 200, 250);
    }

    // Negative Tonfolge
    void playNegativeTone() {
        if(!bActive) return;
        int tones[] = {800, 600, 400};
        playToneSequence(tones, 3, 200, 250);
    }

    // Tonfolge für Batteriestand niedrig - auch wenn Buzzer inaktiv ist
    void playBatteryLowTone() {
        for (int i = 0; i < 3; i++) {
            tone(buzzerPin, 500, 300);
            delay(400);
        }
        noTone(buzzerPin);
    }

    // Tonfolge für Batterie leer - auch wenn Buzzer inaktiv ist
    void playBatteryEmptyTone() {
        for (int i = 0; i < 3; i++) {
            tone(buzzerPin, 1000, 150);
            delay(200);
        }
        tone(buzzerPin, 200, 1000);
        delay(1000);
        noTone(buzzerPin);
    }

    // Erinnerungston
    void playReminderTone() {
        if(!bActive) return;
        int tones[] = {500, 700, 500};
        playToneSequence(tones, 3, 200, 250);
    }

    // Quittungston (ein kurzer Bestätigungston)
    void playAcknowledgmentTone() {
        if(!bActive) return;
        tone(buzzerPin, 1000, 100); // Kurzer Ton bei 1000 Hz, 100 ms
        delay(150); // Kleine Pause
        noTone(buzzerPin);
    }

private:
    // Hilfsfunktion, um eine Sequenz von Tönen abzuspielen
    void playToneSequence(int tones[], int count, int duration, int pause) {
        for (int i = 0; i < count; i++) {
            tone(buzzerPin, tones[i], duration);
            delay(pause);
        }
        noTone(buzzerPin);
    }
};
