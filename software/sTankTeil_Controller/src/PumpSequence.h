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

// PumpSequence
#define SEQ_NONE          -1
#define SEQ_REQUEST_DATA   0
#define SEQ_REFUELL        1
#define SEQ_AIR_REMOVAL    2
#define SEQ_DEFUELL        3
#define SEQ_BACKFUELL      4
#define SEQ_HOPPER_REFUELL 5
#define SEQ_FINISH         6


// Strukturdefinition
struct Element {
    int8_t nummer;  // Wert von -100 bis +100 (int8_t: -128 bis 127)
    bool status;    // Boolean-Wert
};

class PumpSequence {
private:
    Element* elements;
    size_t size;
    size_t currentIndex;
    bool bProcessing;

public:
    // Konstruktor
    PumpSequence(size_t size);

    // Destruktor
    ~PumpSequence();

    // Initialwerte setzen
    void reset();

    // Erstes Element setzen
    void setFirst(int8_t nummer, bool status = false);

    // Nächstes Element setzen
    void setNext(int8_t nummer, bool status = false);

    // Erstes Element abrufen
    Element* getFirst();
    // Nächstes Element abrufen
    Element* getNext(bool bStopProcessing = true);
    // Akruelles Element abrufen
    Element* getCurrent();

    // ID des aktuellen Elements
    size_t getCurrentID() const;
    void setCurrentID(size_t);

    // Element ausgeben (Debug)
    void printElements();

    // Initialisierung mit Makro
    void initializeFromMacro(const char* macro);

    // läuft die Verarbeitung
    bool isProcessing();

    // Verarbeitung starten
    void startProcessing();

    // Verarbeitung stoppen
    void stopProcessing();
};