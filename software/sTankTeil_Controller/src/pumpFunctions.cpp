#include "pumpFunctions.h"

void setTankSequence(uint8_t pump_ctr, uint8_t tanktype) {
  switch(pump_ctr) {
    case CTR_ENTTANKEN:
      switch(tanktype) {
        case TANK_BEUTEL:
          pumpSeq.initializeFromMacro(DEFUELL_BEUTEL);
          break;
        case TANK_1TANK:
          pumpSeq.initializeFromMacro(DEFUELL_1TANK);
          break;
        case TANK_2TANK:
          pumpSeq.initializeFromMacro(DEFUELL_2TANKS);
          break;
      }
      break;
    case CTR_TANKEN:
      switch(tanktype) {
        case TANK_BEUTEL:
          pumpSeq.initializeFromMacro(REFUELL_BEUTEL);
          break;
        case TANK_1TANK:
          pumpSeq.initializeFromMacro(REFUELL_1TANK);
          break;
        case TANK_2TANK:
          pumpSeq.initializeFromMacro(REFUELL_2TANKS);
          break;
      }
      break;
    case CTR_STOP:
    default:
      break;
  }

}


void startTankSequence() {
  flowSensor.resetTotalFlow();
  pumpSeq.getFirst();
  pumpSeq.startProcessing();
}

void stopTankSequence() {
  pumpSeq.stopProcessing();
}



// Betanken
void refuellAction()
{
  if(pumpSeq.isProcessing())
  {
    Element* el = pumpSeq.getCurrent();
    if(el->nummer == SEQ_REFUELL)
    {
      if(DEBUG) { Serial.println("Tanken"); delay(10); }
      if(!el->status)
      {
        // starten
        el->status = true;
        //flowSensor.resetTotalFlow();        // getankte Menge nullen
        flowSensor.setFilling(true);
        pump.forwardRamp(model.getPumpPwr());   // Pumpe starten
        remoteCom.sendData('W', COM_ID_STATUS, "tanken");
        actionTime = millis();
      }
      else
      {
        actionSec = (millis() - actionTime) / 1000;
        if(DEBUG) { Serial.println(actionSec); delay(10); }

        /* Ende Betanken prüfen */
        // Pressure - MessureDelay vorbei
        if(model.getMessurementDelay() <= actionSec)
        {
          // Pressure prüfen
          if(model.getMaxPressure() != 0 && model.getMaxPressure() <= pressureSensor.getPressureMbar())
          {
            pump.stop();
            delay(100);
            if(model.getTankType() == TANK_BEUTEL)
            {
              int nxt = pumpSeq.getNext()->nummer;
              if(nxt == SEQ_NONE) {
                remoteCom.sendData('W', COM_ID_STATUS, "max. Druck erreicht");
                remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
                buzzer.playPositiveTone();
              }
            }
          }
          // Differenzdruck prüfen
          if(model.getPumpStopPressureDiff() != 0 && model.getPumpStopPressureDiff() <= pressureSensor.getDiffMbar())
          {
            pump.stop();
            delay(100);
            int nxt = pumpSeq.getNext()->nummer;
            if(nxt == SEQ_NONE) {
              remoteCom.sendData('W', COM_ID_STATUS, "fertig");
              remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
              buzzer.playPositiveTone();
            }
          }
        }

        // Fuell ml prüfen
        if(model.getMaxRefuelMl() != 0 && model.getMaxRefuelMl() <= flowSensor.getTotalMilliliters())
        {
          pump.stop();
          delay(100);
          int nxt = pumpSeq.getNext()->nummer;
          if(nxt == SEQ_NONE)
          {
            remoteCom.sendData('W', COM_ID_STATUS, "max. Menge erreicht");
            remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
            buzzer.playPositiveTone();
          }
        }

        // Fuell time prüfen
        if(model.getMaxRefuelTime() != 0 && model.getMaxRefuelTime() <= actionSec)
        {
          pump.stop();
          delay(100);
          int nxt = pumpSeq.getNext()->nummer;
          if(nxt == SEQ_NONE)
          {
            remoteCom.sendData('W', COM_ID_STATUS, "Zeit abgelaufen");
            remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
            buzzer.playPositiveTone();
          }
        }
      }
    }
  }
}

// Enttanken
void defuellAction()
{
  if(pumpSeq.isProcessing())
  {
    Element* el = pumpSeq.getCurrent();
    if(el->nummer == SEQ_DEFUELL)
    {
      if(DEBUG) { Serial.println("Enttanken"); delay(10); }
      if(!el->status)
      {
        // starten
        el->status = true;
        flowSensor.resetTotalFlow();        // getankte Menge nullen
        flowSensor.setFilling(false);
        pump.backwardRamp(model.getPumpPwr());   // Pumpe starten
        remoteCom.sendData('W', COM_ID_STATUS, "entanken");
        actionTime = millis();
        emptyTime = 0;
      }
      else
      {
        actionSec = (millis() - actionTime) / 1000;
        if(DEBUG) { Serial.println(actionSec); delay(10); }

        /* Ende Enttanken prüfen */

        // Pressure - MessureDelay vorbei
        if(model.getMessurementDelay() <= actionSec)
        {
          if(model.getTankType() == TANK_BEUTEL)
          {
            // Pressure prüfen - !!! ACHTUNG - Funktion nur bei bidirektionalem Drucksensor - ACHTUNG !!!
            if(model.getMaxPressure() != 0 && model.getMaxPressure() <= pressureSensor.getPressureMbar())
            {
              pump.stop();
              delay(100);
              int nxt = pumpSeq.getNext()->nummer;
              if(nxt == SEQ_NONE) {
                remoteCom.sendData('W', COM_ID_STATUS, "max. Druck erreicht");
                remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
                buzzer.playPositiveTone();
              }
            }

            // Druck wenn Modell leer prüfen - Beutel leer wenn Unterdruck > Leerlaufdruck && DiifDruck == 0 && FlowaRate <= 1 ml/s
            if(model.getPumpStopPressureEmpty() != 0 && model.getPumpStopPressureEmpty() <= pressureSensor.getPressureMbar() && (pump.isOn() && pressureSensor.getDiffMbar() == 0 && flowSensor.getFlowRate() <= 1))
            {
              if(emptyTime == 0)
              {
                emptyTime = millis();
              }
              else
              {
                emptySec = (millis() - emptyTime) / 1000;

                // erst nach der Zeit für Leererkennung stoppen
                if(model.getPumpStopEmptyDelay() <= emptySec) {
                  emptyTime = 0;
                  pump.stop();
                  delay(100);
                  int nxt = pumpSeq.getNext()->nummer;
                  if(nxt == SEQ_NONE) {
                    remoteCom.sendData('W', COM_ID_STATUS, "leer");
                    remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
                    buzzer.playPositiveTone();
                  }
                }
              }
            }
            else
            {
              emptyTime = 0;
            }
          }
          else  // kein Tankbeutel
          {  
            // Pressure prüfen - !!! ACHTUNG - Funktion nur bei bidirektionalem Drucksensor - ACHTUNG !!!
            if(model.getMaxPressure() != 0 && model.getMaxPressure() <= pressureSensor.getPressureMbar())
            {
              buzzer.playNegativeTone();
              pump.stop();
              delay(100);
              // TODO: ERROR für Überdruck senden
            }

            // Druck wenn Modell leer prüfen 
            if(model.getPumpStopPressureEmpty() != 0 && model.getPumpStopPressureEmpty() > pressureSensor.getPressureMbar())
            {
              if(emptyTime == 0)
              {
                emptyTime = millis();
              }
              else
              {
                emptySec = (millis() - emptyTime) / 1000;

                // erst nach der Zeit füe Leererkennung stoppen
                if(model.getPumpStopEmptyDelay() <= emptySec) {
                  emptyTime = 0;
                  pump.stop();
                  delay(100);
                  int nxt = pumpSeq.getNext()->nummer;
                  if(nxt == SEQ_NONE) {
                    remoteCom.sendData('W', COM_ID_STATUS, "leer");
                    remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
                    buzzer.playPositiveTone();
                  }
                }
              }
            }
            else
            {
              emptyTime = 0;
            }
          }
        }

        // für alle Tanktypen

        // Defuell ml prüfen
        if(model.getMaxDefuelMl() != 0 && model.getMaxDefuelMl() <= flowSensor.getTotalMilliliters())
        {
          pump.stop();
          delay(100);
          int nxt = pumpSeq.getNext()->nummer;
          if(nxt == SEQ_NONE)
          {
            remoteCom.sendData('W', COM_ID_STATUS, "max. Menge erreicht");
            remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
            buzzer.playPositiveTone();
          }
        }

        // Defuell time prüfen
        if(model.getMaxDefuelTime() != 0 && model.getMaxDefuelTime() <= actionSec)
        {
          pump.stop();
          delay(100);
          int nxt = pumpSeq.getNext()->nummer;
          if(nxt == SEQ_NONE)
          {
            remoteCom.sendData('W', COM_ID_STATUS, "Zeit abgelaufen");
            remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
            buzzer.playPositiveTone();
          }
        }
      }
    }
  }
}

// Rücktanken
void backfuellAction()
{
  if(pumpSeq.isProcessing())
  {
    Element* el = pumpSeq.getCurrent();
    if(el->nummer == SEQ_BACKFUELL && model.getBackFuelTime() != 0)
    {
      
      if(!el->status)
      {
        // starten
        el->status = true;
        flowSensor.setFilling(false);
        pump.backwardRamp(model.getPumpPwr());   // Pumpe starten
        remoteCom.sendData('W', COM_ID_STATUS, "rueck tanken");
        actionTime = millis();

        if(DEBUG) { Serial.println("rueck Tanken"); delay(10); }
      }
      else
      {
        actionSec = (millis() - actionTime) / 1000;
        //if(DEBUG) { Serial.println(actionSec); delay(10); }

        // BackFuell time prüfen
        if(model.getBackFuelTime() <= actionSec)
        {
          pump.stop();
          delay(100);
          int nxt = pumpSeq.getNext()->nummer;
          if(nxt == SEQ_NONE)
          {
            remoteCom.sendData('W', COM_ID_STATUS, "ready");
            remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
            buzzer.playPositiveTone();
          }
        }
      }
    }
  }
}

// Luft entfernen - nur bei Beuteltank
void airremovalAction()
{
  if(pumpSeq.isProcessing() && model.getTankType() == TANK_BEUTEL)
  {
    Element* el = pumpSeq.getCurrent();
    if(el->nummer == SEQ_AIR_REMOVAL)
    {
      if(!el->status)
      {
        // starten
        el->status = true;
        flowSensor.setFilling(false);
        pump.backwardRamp(model.getPumpPwr());   // Pumpe starten
        remoteCom.sendData('W', COM_ID_STATUS, "Luft entfernen");
        actionTime = millis();
        emptyTime = 0;
  
        if(DEBUG) { Serial.println("Luft entfernen"); delay(10); }
      }
      else
      {
        actionSec = (millis() - actionTime) / 1000;

        if(DEBUG) { Serial.println(actionSec); delay(10); }

        /* Ende Entetanken prüfen */
        // Pressure - MessureDelay vorbei
        if(model.getMessurementDelay() <= actionSec)
        {
          // Pressure prüfen - !!! ACHTUNG - Funktion nur bei bidirektionalem Drucksensor - ACHTUNG !!!
          if(model.getMaxPressure() != 0 && model.getMaxPressure() <= pressureSensor.getPressureMbar())
          {
            pump.stop();
            delay(100);
            int nxt = pumpSeq.getNext()->nummer;
            if(nxt == SEQ_NONE) {
              remoteCom.sendData('W', COM_ID_STATUS, "max. Druck erreicht");
              remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
              buzzer.playPositiveTone();
            }
          }

          // Druck wenn Modell leer prüfen
          if(model.getPumpStopPressureEmpty() != 0 && model.getPumpStopPressureEmpty() < pressureSensor.getPressureMbar())
          {
            if(emptyTime == 0)
            {
              emptyTime = millis();
              remoteCom.sendData('W', COM_ID_STATUS, "Kerosin erkannt");
              if(DEBUG) { Serial.println("Kerosin kommt"); delay(10); }
            }
            else
            {
              emptySec = (millis() - emptyTime) / 1000;

              // erst nach der Zeit für 'keine Luft mehr'' stoppen
              if(model.getAirRemovalTime() <= emptySec) {
                emptyTime = 0;
                pump.stop();
                delay(100);
                int nxt = pumpSeq.getNext()->nummer;
                if(nxt == SEQ_NONE) {
                  remoteCom.sendData('W', COM_ID_STATUS, "leer");
                  remoteCom.sendData('W', COM_ID_PUMP_CONTROL, String(CTR_STOP).c_str(), true);
                  buzzer.playPositiveTone();
                }
              }
            }
          }
        }
      }
    }
  }
}