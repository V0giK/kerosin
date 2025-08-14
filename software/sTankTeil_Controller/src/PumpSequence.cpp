#include "PumpSequence.h"

// Konstruktor
PumpSequence::PumpSequence(size_t size) : size(size), currentIndex(0) {
    elements = new Element[size];
    bProcessing = false;
    reset();
}

// Destruktor
PumpSequence::~PumpSequence() {
    delete[] elements;
}

// Initialwerte setzen
void PumpSequence::reset() {
    for (size_t i = 0; i < size; ++i) {
        elements[i].nummer = -1;
        elements[i].status = false;
    }
    currentIndex = 0;
    bProcessing = false;
}

// Erstes Element setzen
void PumpSequence::setFirst(int8_t nummer, bool status) {
    elements[0].nummer = nummer;
    elements[0].status = status;
    currentIndex = 0;
}

// Nächstes Element setzen
void PumpSequence::setNext(int8_t nummer, bool status) {
    if (currentIndex + 1 < size) {
        ++currentIndex;
        elements[currentIndex].nummer = nummer;
        elements[currentIndex].status = status;
    }
}

// Erstes Element abrufen
Element* PumpSequence::getFirst() {
    currentIndex = 0;
    return &elements[0];
}

// Nächstes Element abrufen
// Processing stoppen wenn das nächste Element SEQ_NONE ist
Element* PumpSequence::getNext(bool bStopProcessing) {

    Element* element;
    if (currentIndex + 1 < size) {
        ++currentIndex;
        element = &elements[currentIndex];
    }
    element = &elements[currentIndex]; // Letztes Element zurückgeben
    if(element->nummer == SEQ_NONE) { stopProcessing(); }

    return element;
}

// Aktuelless Element abrufen
Element* PumpSequence::getCurrent() {
    return &elements[currentIndex]; // Aktuelles Element zurückgeben
}

// ID des aktuellen Elements
size_t PumpSequence::getCurrentID() const {
    return currentIndex;
}
void PumpSequence::setCurrentID(size_t id) {
    currentIndex = id;
}

// Element ausgeben (Debug)
void PumpSequence::printElements() {
    for (size_t i = 0; i < size; ++i) {
        Serial.print("Element ");
        Serial.print(i);
        Serial.print(": Nummer = ");
        Serial.print(elements[i].nummer);
        Serial.print(", Status = ");
        Serial.println(elements[i].status ? "true" : "false");
    }
}

// Initialisierung mit Makro
void PumpSequence::initializeFromMacro(const char* macro) {
    reset(); // Array zurücksetzen
    size_t index = 0;
    const char* ptr = macro;

    while (*ptr != '\0' && index < size) {
        int value = atoi(ptr); // String in Zahl umwandeln
        elements[index].nummer = (uint8_t)value;
        elements[index].status = false;
        ++index;

        // Zum nächsten Wert springen
        while (*ptr != '\0' && *ptr != ',') ptr++;
        if (*ptr == ',') ptr++;
    }
}

// läuft die Verarbeitung
bool PumpSequence::isProcessing() {
    return bProcessing;
}

// Verarbeitung starten
void PumpSequence::startProcessing() {
    bProcessing = true;
}

// Verarbeitung stoppen
void PumpSequence::stopProcessing() {
    bProcessing = false;
}
