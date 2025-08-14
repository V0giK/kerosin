// helper.h

#pragma once

#include <Arduino.h>

uint16_t calculateCRC(const char *data);
void enterSleepNano();