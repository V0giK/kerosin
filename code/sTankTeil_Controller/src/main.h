#pragma once

#include <Arduino.h>
#include "variables.h"

// Nano resetten
void(* resetNano) (void) = 0; // Deklaration eines Funktionszeigers auf Adresse 0

void onButtonInClick();
void onButtonOutClick();
void onBothButtonsLongPress();

void handleFlowMeasurement();
void handlePressureMeasurement();

void handlePumpSeq();


// Callbacks
void onWrite(int16_t id, const String &value);
void onRead(int16_t id);