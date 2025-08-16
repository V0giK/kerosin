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

#include "bufferFunctions.h"

// FIFO Buffer
CircularBuffer<recData, 15> fifo_input_buffer;

// Objekt für Recieved Data - von UART
recData getRecDataObj(int16_t id, String value) {
  recData dta;
  dta.id = id;
  dta.value = value;
  return dta;
}

bool buffer_tick() {
  bool hasActivity = !fifo_input_buffer.isEmpty();

  while(!fifo_input_buffer.isEmpty()) {
    recData dta = fifo_input_buffer.shift();
    int iVal = atoi(dta.value.c_str());

    switch(dta.id) {
      case COM_ID_FLOWCALIBRATE:
        if(iVal == 0) {
          flowSensor.resetPulses();
        }
        break;
      
      // Config
      case COM_ID_FLOW_TICKS:
        configManager.setImpulseProLiter(iVal);
        flowSensor.setCalibration(configManager.getImpulseProLiter());
        pressureSensor.setNumSamples(configManager.getAnzahlMesswerteMittelwert());
        break;
      case COM_ID_PRESSURE_AVG:
        configManager.setAnzahlMesswerteMittelwert(iVal);
        break;
      case COM_ID_AKKU_MIN:
        configManager.setMinimalspannungAkku(iVal);
        break;
      case COM_ID_AKKU_CALIB:
        //configManager.setBattKalibrierungsfaktor(iVal);
        break;
      case COM_ID_AKKU_VOLT: {
        int32_t cur = configManager.getBattKalibrierungsfaktor();
        int volt = voltReader.getLastVoltage() * 100;
        configManager.setBattKalibrierungsfaktor((iVal / volt) * cur);
        voltReader.setCalibrationFactor(configManager.getBattKalibrierungsfaktor() / 10000.0);
        break;
      }
      case COM_ID_BEEP:
        configManager.setSignaltonOn(iVal);
        buzzer.setActive(iVal);
        break;
      case COM_ID_PWR_MANUEL:
        configManager.setManuellePumpenleistung(iVal);
        break;
      case COM_ID_SYSTEM_OFF:
        configManager.setSystemabschaltungSekunden(iVal);
        break;
      case COM_ID_WRITE_EEPROM:
        if(iVal == 5) { configManager.saveConfig(); buzzer.playPositiveTone(); }
        break;
    
      // ModellDaten
      case COM_ID_TANKTYPE:
      case COM_ID_PUMP_PWR:
      case COM_ID_PRESSURE_DROP_HOSE_BREAK:
      case COM_ID_MAX_REFUEL_TIME:
      case COM_ID_MAX_DEFUEL_TIME:
      case COM_ID_BACK_FUEL_TIME:
      case COM_ID_AIR_REMOVAL_TIME:
      case COM_ID_PUMP_STOP_EMPTY_DELAY:
      case COM_ID_MESSUREMENT_DELAY:
      case COM_ID_MAX_REFUEL_ML:
      case COM_ID_MAX_DEFUEL_ML:
      case COM_ID_MAX_PRESSURE:
      case COM_ID_PUMP_STOP_PRESSURE_DIFF:
      case COM_ID_PUMP_STOP_PRESSURE_EMPTY:
      case COM_ID_HOPPER_PRESSURE:
      case COM_ID_PUMP_STOP_HOPPER_PRESSURE_DIFF:
      case COM_ID_SAVE_MODEL_EEPROM:
        handleModelCommand(dta.id, iVal, dta.value);
        break;

      // Pumpe
      case COM_ID_PUMP_MODE:
        pumpMode = iVal;

        // wenn keine Remote vorhanden
        if(!bRemoteConnected) {
          if(pumpMode == MODE_AUTO) {
            // TODO: prüfen ob Modeldaten vorhanden
            if(bHasModelSaved) {
              digitalWrite(PIN_LED_MODEL, HIGH);
              digitalWrite(PIN_LED_MANUEL, LOW);
            } else {
              pumpMode = MODE_MANUELL;
            }
          }
          if(pumpMode == MODE_MANUELL) {
            digitalWrite(PIN_LED_MODEL, LOW);
            digitalWrite(PIN_LED_MANUEL, HIGH);
          }
        }
        //initModelData = true;
        //remoteCom.sendData('R', COM_ID_TANKTYPE, "",  true);
        break;
      case COM_ID_BROADCAST:
        if(iVal == 0) {
          bSendBroadcast = false;
        } else {
          bSendBroadcast = true;
          pressureSensor.calibSensor();
        }
        break;
      case COM_ID_PUMP_CONTROL:
        handlePumpControl(iVal);
        break;
    }
  }

  return hasActivity;
}

void handlePumpControl(int iVal) {
  pump.stop();
  delay(100);
  switch(iVal) {
    case CTR_TANKEN:
      pressureSensor.calibSensor();
      flowSensor.resetTotalFlow();
      if(pumpMode == MODE_AUTO) {
        setTankSequence((uint8_t) iVal, (uint8_t) model.getTankType());
        startTankSequence();
      } else {
        if(bRemoteConnected) {
          pump.forwardRamp(model.getPumpPwr());
          remoteCom.sendData('W', COM_ID_STATUS, "tanken");
        } else {
          pump.forwardRamp(configManager.getManuellePumpenleistung());
        }
      }
      break;
    case CTR_ENTTANKEN:
      pressureSensor.calibSensor();
      flowSensor.resetTotalFlow();
      if(pumpMode == MODE_AUTO) {
        setTankSequence((uint8_t) iVal, (uint8_t) model.getTankType());
        startTankSequence();
      } else {
        if(bRemoteConnected) {
          pump.backwardRamp(model.getPumpPwr());
          remoteCom.sendData('W', COM_ID_STATUS, "enttanken");
        } else {
          pump.backwardRamp(configManager.getManuellePumpenleistung());
        }
      }
      break;
    case CTR_STOP:
    default:
      stopTankSequence();
      pump.stop();
      remoteCom.sendData('W', COM_ID_STATUS, "stop");
      break;
  }
}

// ModellDaten
void handleModelCommand(int id, int iVal, const String& value){
  switch (id)
  {
    case COM_ID_TANKTYPE:
      model.setTankType(iVal);
      break;
    case COM_ID_PUMP_PWR:
      model.setPumpPwr(iVal);
      if(pump.isOn()) {
        pump.adjustSpeed(iVal);
      }
      break;
    case COM_ID_PRESSURE_DROP_HOSE_BREAK:
      model.setPressureDropHoseBreak(iVal);
      break;
    case COM_ID_MAX_REFUEL_TIME:
      model.setMaxRefuelTime(iVal);
      break;
    case COM_ID_MAX_DEFUEL_TIME:
      model.setMaxDefuelTime(iVal);
      break;
    case COM_ID_BACK_FUEL_TIME:
      model.setBackFuelTime(iVal);
      break;
    case COM_ID_AIR_REMOVAL_TIME:
      model.setAirRemovalTime(iVal);
      break;
    case COM_ID_PUMP_STOP_EMPTY_DELAY:
      model.setPumpStopEmptyDelay(iVal);
      break;
    case COM_ID_MESSUREMENT_DELAY:
      model.setMessurementDelay(iVal);
      break;
    case COM_ID_MAX_REFUEL_ML:
      model.setMaxRefuelMl(iVal);
      break;
    case COM_ID_MAX_DEFUEL_ML:
      model.setMaxDefuelMl(iVal);
      break;
    case COM_ID_MAX_PRESSURE:
      model.setMaxPressure(iVal);
      break;
    case COM_ID_PUMP_STOP_PRESSURE_DIFF:
      model.setPumpStopPressureDiff(iVal);
      break;
    case COM_ID_PUMP_STOP_PRESSURE_EMPTY:
      model.setPumpStopPressureEmpty(iVal);
      break;
    case COM_ID_HOPPER_PRESSURE:
      model.setHopperPressure(iVal);
      break;
    case COM_ID_PUMP_STOP_HOPPER_PRESSURE_DIFF:
      model.setPumpStopHopperPressureDiff(iVal);
      break;
    case COM_ID_SAVE_MODEL_EEPROM:
      if(iVal == 5) {
        if(pumpMode == MODE_MANUELL) {
          model.clearCRCeeprom(EEPROM_ADR_MODEL);
        } else {
          model.saveToEEPROM(EEPROM_ADR_MODEL);
        }
        buzzer.playPositiveTone();
      }
      break;
  }
}