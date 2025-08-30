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

#include "BuzzerManager.h"

BuzzerManager::BuzzerManager(int pin, unsigned long interval, unsigned long repeatInterval)
    : buzzerPin(pin), reminderInterval(interval), reminderRepeatInterval(repeatInterval), lastActivityTime(0), lastReminderTime(0), bActive(true) {}

void BuzzerManager::begin() {
    pinMode(buzzerPin, OUTPUT);
    lastActivityTime = millis();
}

void BuzzerManager::setReminderSeconds(int inakivSec) {
    if(inakivSec != 0 && inakivSec <= 30) inakivSec = 30;
    reminderInterval = inakivSec * 1000L;
}

void BuzzerManager::setActive(bool active) {
    bActive = active;
}

void BuzzerManager::updateActivity() {
    lastActivityTime = millis();
    lastReminderTime = 0;
}

void BuzzerManager::checkReminder() {
    if(reminderInterval == 0 ) return;
    unsigned long currentTime = millis();
    if (currentTime - lastActivityTime > reminderInterval) {
        if (lastReminderTime == 0 || currentTime - lastReminderTime > reminderRepeatInterval) {
            playReminderTone();
            lastReminderTime = currentTime;
        }
    }
}

void BuzzerManager::playPositiveTone() {
    if(!bActive) return;
    int tones[] = {400, 600, 800};
    playToneSequence(tones, 3, 200, 250);
}

void BuzzerManager::playNegativeTone() {
    if(!bActive) return;
    int tones[] = {800, 600, 400};
    playToneSequence(tones, 3, 200, 250);
}

void BuzzerManager::playBatteryLowTone() {
    for (int i = 0; i < 3; i++) {
        tone(buzzerPin, 500, 300);
        delay(400);
    }
    noTone(buzzerPin);
}

void BuzzerManager::playBatteryEmptyTone() {
    for (int i = 0; i < 3; i++) {
        tone(buzzerPin, 1000, 150);
        delay(200);
    }
    tone(buzzerPin, 200, 1000);
    delay(1000);
    noTone(buzzerPin);
}

void BuzzerManager::playReminderTone() {
    if(!bActive) return;
    int tones[] = {500, 700, 500};
    playToneSequence(tones, 3, 200, 250);
}

void BuzzerManager::playAcknowledgmentTone() {
    if(!bActive) return;
    tone(buzzerPin, 1000, 100);
    delay(150);
    noTone(buzzerPin);
}

void BuzzerManager::playToneSequence(int tones[], int count, int duration, int pause) {
    for (int i = 0; i < count; i++) {
        tone(buzzerPin, tones[i], duration);
        delay(pause);
    }
    noTone(buzzerPin);
}
