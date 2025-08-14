#pragma once

#include <Arduino.h>
#include "defines.h"
#include "variables.h"
#include "pumpFunctions.h"

// Strukturdefinition
struct recData {
    int16_t id;
    String value;
};

// FIFO Buffer
extern CircularBuffer<recData, 15> fifo_input_buffer;

// Empfangspuffer verarbeiten
recData getRecDataObj(int16_t id, String value);
bool buffer_tick();