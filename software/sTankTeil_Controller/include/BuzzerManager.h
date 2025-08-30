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
#pragma once

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
    BuzzerManager(int pin, unsigned long interval, unsigned long repeatInterval);

    // Initialisierung des Buzzers
    void begin();
    void setReminderSeconds(int inakivSec);
    void setActive(bool active);

    // Aktualisiert die Aktivität (z. B. bei einem Ereignis)
    void updateActivity();

    // Prüft, ob die Erinnerung abgespielt werden soll
    void checkReminder();

    // Positive Tonfolge
    void playPositiveTone();

    // Negative Tonfolge
    void playNegativeTone();

    // Tonfolge für Batteriestand niedrig - auch wenn Buzzer inaktiv ist
    void playBatteryLowTone();

    // Tonfolge für Batterie leer - auch wenn Buzzer inaktiv ist
    void playBatteryEmptyTone();

    // Erinnerungston
    void playReminderTone();

    // Quittungston (ein kurzer Bestätigungston)
    void playAcknowledgmentTone();

private:
    // Hilfsfunktion, um eine Sequenz von Tönen abzuspielen
    void playToneSequence(int tones[], int count, int duration, int pause);
};
