#pragma once
#include <Arduino.h>

// Controller-Kommunikation und Validierung
bool searchAndLoadController();
bool requestAndValidateConfiguration(int16_t id, const String &description, unsigned long startTime, unsigned long globalTimeout);
