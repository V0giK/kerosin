#pragma once

#include <Arduino.h>

#include "defines.h"
#include "variables.h"

void setTankSequence(uint8_t, uint8_t);
void startTankSequence();
void stopTankSequence();

void refuellAction();
void defuellAction();
void backfuellAction();
void airremovalAction();