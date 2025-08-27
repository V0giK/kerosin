#pragma once

#include <lvgl.h>
#include "ModelParameters.h"
#include "ui/screens.h" // Add this include for ScreensEnum
#include "displayConfig.h"

// Deklaration der Funktion zum Screen-Handling
void handleScreenFlags();
void go2screen(ScreensEnum newScreen);
void go2screenPrev();

// Neue Methoden für UI-Initialisierung und Fehleranzeige
void showErrorAndSleep(const char* message);
void setInitialValues();
void lvglSetup();
void updateDisplay(int iterations);
