#include "variables.h"

// Debug-Ausgabe steuern
const bool DEBUG = false;



// Instanz der Klasse
BuzzerManager buzzer(PIN_BUZZER, 300000, 5000); // 5 Minuten für erste Erinnerung, dann alle 5 Sekunden

// SystemConfigManager-Instanz
SystemConfigManager configManager(EEPROM_ADR_CONFIG);

// Sensor-Konfiguration
FlowSensor flowSensor(PIN_FLOW);                 // Flowsensor mit Defaultwerten (5312 Pulse/Liter)
PressureSensor pressureSensor(PIN_PRESSURE);     // Drucksensor mit Defaultwerten (30 PSI)

// UART-Kommunikation
UartCommunication remoteCom(PIN_SOFT_RX, PIN_SOFT_TX, true);

// Batteriespannung
VoltageReader voltReader(PIN_BATT_MESURE, 5.0, 10);

// Taster LED
LEDController ledController(PIN_LED_BATT);

// Zeitsteuerung
unsigned long lastMillis = 0;                    // Zeitstempel für Messintervalle
const unsigned long MEASUREMENT_INTERVAL = 500;  // 500 ms

unsigned long actionTime = 0;                    // Zeitstempel der action aus der Sequenz
uint32_t actionSec = 0;                          // vergangene Zeit in sekunden

unsigned long lastMillis5sec = 0;                // Zeitstempel für 5 Sekunden Timer
const unsigned long INTERVAL_5S = 5000;          // 5000 ms

// Leererkennung
unsigned long emptyTime = 0;                     // Zeitstempel der Leererkennung
uint32_t emptySec = 0;                           // vergangene Zeit in sekunden

// Pumpen Steuerung
HBridgeMotor pump(PIN_PUMP_D0, PIN_PUMP_D1);

// aktueller Pumpmode
int pumpMode = MODE_MANUELL;
bool initModelData = false;
bool bSendBroadcast = false;

// Pumpsequenz
PumpSequence pumpSeq(10);

// Modell
ModelParameters model;
bool bHasModelSaved = true; // Modelldaten in EEPROM vorhanden

// Variablen die über den Code gestzt werden, hier Initialwerte
bool bHalfIntervall = false;
// Remote an/ab Stecken
bool bRemoteConnected = false;
bool bRemoteConnectedWasChanged = false;

// Taster
ButtonManager buttonManager(PIN_TASTER_IN, PIN_TASTER_OUT);
