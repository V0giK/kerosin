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

 #include <Arduino.h>

#include "variables.h"
#include "defines.h"
#include "main.h"
#include "bufferFunctions.h"
#include "pumpFunctions.h"


// Batteriespannung Warnungen - Callback-Funktion
void voltageAlert(const char* message, unsigned long duration) {
    // Serial.print(message);
    // Serial.print(" for ");
    // Serial.print(duration);
    // Serial.println(" seconds");

    if(strcmp(message, "warning") == 0) {
      if(duration >= 5) {
        buzzer.playBatteryLowTone();
        ledController.startLowBatteryBlinking();
      }

    } else {
      if(duration >= 5) {
        buzzer.playBatteryEmptyTone();
        ledController.startErrorBlinking();
        if(!(pump.isOn() || pumpSeq.isProcessing()) && bRemoteConnected) {
          // TODO: Spannung abschalten!!!!!
          //Serial.println("Power OFF.  empty battery");
          remoteCom.sendData('W', COM_ID_MC_RESET, "99", true);
          // delay(50);
          // resetFunc();
          // enterSleepNano();
        }
      } else {
        buzzer.playBatteryLowTone();
        ledController.stopLowBatteryBlinking();
      }
    }
}


void setup() {

  pinMode(PIN_LED_MODEL, OUTPUT);   // LED model
  pinMode(PIN_LED_MANUEL, OUTPUT);  // LED manuel
  // pinMode(PIN_LED_BATT, OUTPUT);    // LED batt. low
  pinMode(PIN_MOTE_TXRX, OUTPUT);
  digitalWrite(PIN_MOTE_TXRX, LOW);
  pinMode(PIN_MOTE_12V, OUTPUT);
  digitalWrite(PIN_MOTE_12V, LOW);

  // {
    // Serial.println("Lösche EEPROM...");
    // // EEPROM löschen
    // for (int i = 0; i < EEPROM.length(); i++) {
    //     EEPROM.write(i, 0xFF); // 0xFF entspricht einem leeren Wert
    // }
    // Serial.println("EEPROM wurde geleert.");
    // return;
  // }

  //configManager.resetToDefaults();


    Serial.begin(115200);        // Serielle Schnittstelle starten
    // Callbacks registrieren
    remoteCom.setWriteCallback(onWrite);
    remoteCom.setReadCallback(onRead);

    bRemoteConnected = !digitalRead(PIN_MOTE_CHK);
    Serial.print("Mote connected:" ); Serial.println(bRemoteConnected?"true":"false");
    
    if(bRemoteConnected) {
      digitalWrite(PIN_MOTE_TXRX, HIGH); // Versorgungsspannung für Kommunikation mit Remote aktivieren (RS422)
      digitalWrite(PIN_MOTE_12V, HIGH); // Versorgungsspannung für Remote aktivieren (12V)
      remoteCom.begin(9600);              // 19200 Serielle Schnittstelle mit der Remote initialisieren

    } else {
      buttonManager.begin(); // Initialisierung der Taster
      // Callbacks setzen
      buttonManager.onButtonInClick(onButtonInClick);
      buttonManager.onButtonOutClick(onButtonOutClick);
      buttonManager.onBothButtonsLongPress(onBothButtonsLongPress);

      // Modelldaten aus EEPROM lesen - falls vorhanden
      // Start immer im "Modell-Mode", ausser es sind keine Daten vorhanden ... dies auch beim Mode-Wechsel berücksichtigen!!!!!
      bHasModelSaved = model.loadFromEEPROM(EEPROM_ADR_MODEL);
      if(bHasModelSaved) {
        fifo_input_buffer.push(getRecDataObj(COM_ID_PUMP_MODE, String(MODE_AUTO)));
      } else {
        // model.setTankType(0); // TankType initialisieren da laufend darauf geprüft wird
        fifo_input_buffer.push(getRecDataObj(COM_ID_PUMP_MODE, String(MODE_MANUELL)));
      }
    }

    buzzer.begin(); // Buzzer initialisieren

    // Konfiguration laden
    configManager.loadConfig();
    buzzer.setReminderSeconds(configManager.getSystemabschaltungSekunden());
    buzzer.setActive(configManager.getSignaltonOn());

    // Durchflusssensor initialisieren
    flowSensor.begin();
    flowSensor.setCalibration(configManager.getImpulseProLiter());
    // Drucksensor initialisieren
    pressureSensor.setNumSamples(configManager.getAnzahlMesswerteMittelwert());
    pressureSensor.calibSensor();

    // Akkuspannung initialisieren
    // TODO: Wert fehlt noch im EEPROM !!!!!!!!!!!!!!!!!
    // voltReader.setCalibrationFactor(2.9183);
    voltReader.setCalibrationFactor(configManager.getBattKalibrierungsfaktor() / 10000.0);
    voltReader.setThresholds(configManager.getMinimalspannungAkku() / 10.0, 9.5, voltageAlert);

    if (DEBUG) Serial.println("System gestartet...");
    Serial.flush();


    // LEDs prüfen
    digitalWrite(PIN_LED_MODEL, HIGH);
    digitalWrite(PIN_LED_MANUEL, HIGH);
    digitalWrite(PIN_LED_BATT, HIGH);
    delay(1500);
    digitalWrite(PIN_LED_MODEL, LOW);
    digitalWrite(PIN_LED_MANUEL, LOW);
    digitalWrite(PIN_LED_BATT, LOW);

    pinMode(PIN_MOTE_CHK, INPUT);
    // pinMode(PIN_TASTER_IN, INPUT);
    // pinMode(PIN_TASTER_OUT, INPUT);


    // ESP32 (Remote) neu starten
    remoteCom.sendData('W', COM_ID_MC_RESET, "");

    buzzer.playAcknowledgmentTone();
}

void loop() {
  bool hasActivity = false;
  hasActivity = remoteCom.tick();         // Remote prüfen
  
  buttonManager.update();   // Taster-Logik aktualisieren
  hasActivity |= buffer_tick();

  if(hasActivity) buzzer.updateActivity();
  buzzer.checkReminder();

  if(pump.isOn() || pumpSeq.isProcessing()) hasActivity = true;

  // Batteriespannung
  // Berechne die Spannung
  voltReader.readVoltage();


  ledController.update();


  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= MEASUREMENT_INTERVAL) {
      lastMillis = currentMillis;
      bHalfIntervall = false;

      handleFlowMeasurement();     // Durchflussmessung
      handlePressureMeasurement(); // Druckmessung
      handlePumpSeq();

  }
  if (!bHalfIntervall && currentMillis - lastMillis >= MEASUREMENT_INTERVAL / 2) {
    bHalfIntervall = true;

    // Durchflussrate und Gesamtmenge berechnen
    if(pumpMode != MODE_CALIB_FLOW){
      flowSensor.tick();
    }
    // Druck abfragen
    pressureSensor.tick();

    // Remote an/ausgesteckt
    if(!digitalRead(PIN_MOTE_CHK) != bRemoteConnected) {
      resetNano();
    }

  }

  // 5 Sekunden
  if (currentMillis - lastMillis5sec >= INTERVAL_5S) {
    lastMillis5sec = currentMillis;

    voltReader.evaluateVoltage(); // Bewertung der Spannung und ggf. Callback-Aufruf
    remoteCom.sendData('W', COM_ID_AKKU_VOLT, voltReader.getLastVoltageString().c_str());

  }

}



// Durchflussmessung durchführen
void handleFlowMeasurement() {
  if(pump.isOn() || bSendBroadcast) {
    switch(pumpMode) {
      case MODE_CALIB_FLOW:
        // TODO
        remoteCom.sendData('W', COM_ID_FLOWCALIBRATE, String(flowSensor.getPulses()).c_str());
        return;
        break;
      // case MODE_AUTO:
      // case MODE_CALIB_MODEL:
      // case MODE_MANUELL:
      default:
        break;
    }

    // UART-Daten senden
    remoteCom.sendData('W', COM_ID_CURRENT_ML, String(flowSensor.getTotalMilliliters()).c_str());

    // Debug-Ausgabe
    // if (DEBUG) {
    //   Serial.print("Durchflussrate: ");
    //   Serial.print(flowSensor.getFlowRate());
    //   Serial.print(" ml/s, Gesamtmenge: ");
    //   Serial.print(flowSensor.getTotalMilliliters());
    //   Serial.println(" ml");
    // }

  }
}

// Druckmessung durchführen
void handlePressureMeasurement() {

  double pressure_mbar = floor(pressureSensor.getPressureMbar(true));
  double pressure_diff = floor(pressureSensor.getDiffMbar());
  if(pump.isOn() || bSendBroadcast) {
    remoteCom.sendData('W', COM_ID_CURRENT_MBAR, String(pressure_mbar).c_str());
    remoteCom.sendData('W', COM_ID_CURRENT_DIFF_MBAR, String(pressure_diff).c_str());

    // Debug-Ausgabe
    // if (DEBUG) {
    //     Serial.print("Druck: ");
    //     Serial.print(pressure_mbar, 2);
    //     Serial.print(" mbar, Diff: ");
    //     Serial.print(pressure_diff, 2);
    //     Serial.println(" mbar");
    // }
  }

}

// Callback für SW (Schreiben)
void onWrite(int16_t id, const String &value) {
  // if (DEBUG) Serial.println("SW erhalten: ID=" + String(id) + ", Wert=" + value);

  recData dta = getRecDataObj(id, value);
  // in Input-Puffer schreiben
  fifo_input_buffer.push(dta);
}

// Callback für SR (Lesen)
void onRead(int16_t id) {
  // if (DEBUG) Serial.println("SR erhalten: ID=" + String(id));

  bool hasActivity = true;
  switch (id) {
    case COM_ID_TYPE:
      remoteCom.sendData('W', id, HARDWARE_TYPE);
      break;
    case COM_ID_FLOW_TICKS:
      remoteCom.sendData('W', id, String(configManager.getImpulseProLiter()).c_str());
      break;
    case COM_ID_PRESSURE_AVG:
      remoteCom.sendData('W', id, String(configManager.getAnzahlMesswerteMittelwert()).c_str());
      break;
    case COM_ID_AKKU_MIN:
      remoteCom.sendData('W', id, String(configManager.getMinimalspannungAkku()).c_str());
      break;
    case COM_ID_AKKU_CALIB:
      remoteCom.sendData('W', id, String(configManager.getBattKalibrierungsfaktor()).c_str());
      break;
    case COM_ID_BEEP:
      remoteCom.sendData('W', id, String(configManager.getSignaltonOn()).c_str());
      break;
    case COM_ID_PWR_MANUEL:
      remoteCom.sendData('W', id, String(configManager.getManuellePumpenleistung()).c_str());
      break;
    case COM_ID_SYSTEM_OFF:
      remoteCom.sendData('W', id, String(configManager.getSystemabschaltungSekunden()).c_str());
      break;
    default:
      //
      hasActivity = false;
      break;
  }

  if(hasActivity) buzzer.updateActivity();
}




void handlePumpSeq() {
  refuellAction();
  backfuellAction();
  defuellAction();
  airremovalAction();
}






// Callback-Funktionen definieren
void onButtonInClick() {
  buzzer.playAcknowledgmentTone();
  if(pump.isOn()) {
    fifo_input_buffer.push(getRecDataObj(COM_ID_PUMP_CONTROL, String(CTR_STOP)));
  } else {
    fifo_input_buffer.push(getRecDataObj(COM_ID_PUMP_CONTROL, String(CTR_TANKEN)));
  }
}

void onButtonOutClick() {
  buzzer.playAcknowledgmentTone();
  if(pump.isOn()) {
    fifo_input_buffer.push(getRecDataObj(COM_ID_PUMP_CONTROL, String(CTR_STOP)));
  } else {
    fifo_input_buffer.push(getRecDataObj(COM_ID_PUMP_CONTROL, String(CTR_ENTTANKEN)));
  }
}

void onBothButtonsLongPress() {
  buzzer.playAcknowledgmentTone();
  if(pump.isOn()) {
    fifo_input_buffer.push(getRecDataObj(COM_ID_PUMP_CONTROL, String(CTR_STOP)));
  }
  if(bHasModelSaved && pumpMode == MODE_MANUELL) {
    fifo_input_buffer.push(getRecDataObj(COM_ID_PUMP_MODE, String(MODE_AUTO)));
  } else {
    if(!bHasModelSaved) buzzer.playNegativeTone();
    fifo_input_buffer.push(getRecDataObj(COM_ID_PUMP_MODE, String(MODE_MANUELL)));
  }
}

