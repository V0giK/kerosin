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
#include <CircularBuffer.hpp>
#include "defines.h"

#include "ButtonManager.h"
#include "BuzzerManager.h"
#include "SystemConfigManager.h"
#include "FlowSensor.h"
#include "PressureSensor.h"
#include "uartCommunication.h"
#include "VoltageReader.h"
#include "LEDController.h"
#include "HBridgeMotor.h"
#include "PumpSequence.h"
#include "ModelParameters.h"

// EEPROM-Startadresse (im Normalfall 0)
#define EEPROM_ADR_CONFIG 0 // Länge 12
#define EEPROM_ADR_MODEL 13 // Länge 57



// Debug-Ausgabe steuern
extern const bool DEBUG;





// Instanz der Klasse
extern BuzzerManager buzzer;

// SystemConfigManager-Instanz
extern SystemConfigManager configManager;

// Sensor-Konfiguration
extern FlowSensor flowSensor;
extern PressureSensor pressureSensor;

// UART-Kommunikation
extern UartCommunication remoteCom;

// Batteriespannung
extern VoltageReader voltReader;

// Taster LED
extern LEDController ledController;

// Zeitsteuerung
extern unsigned long lastMillis;
extern const unsigned long MEASUREMENT_INTERVAL;

extern unsigned long actionTime;
extern uint32_t actionSec;

extern unsigned long lastMillis5sec;
extern const unsigned long INTERVAL_5S;

// Leererkennung
extern unsigned long emptyTime;
extern uint32_t emptySec;

// Pumpen Steuerung
extern HBridgeMotor pump;

// aktueller Pumpmode
extern int pumpMode;
extern bool initModelData;
extern bool bSendBroadcast;

// Pumpsequenz
extern PumpSequence pumpSeq;

// Modell
extern ModelParameters model;
extern bool bHasModelSaved;

// Variablen die über den Code gestzt werden, hier Initialwerte
extern bool bHalfIntervall;
// Remote an/ab Stecken
extern bool bRemoteConnected;
extern bool bRemoteConnectedWasChanged;

// Taster
extern ButtonManager buttonManager;
