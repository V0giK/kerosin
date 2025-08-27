#include "controllerUtils.h"
#include "helper.h"
#include "uartCommunication.h"
#include "ui/vars.h"
#include "main.h"
#include "configManager.h" // <--- Hinzufügen für Config Definition
#include <lvgl.h>      // Für lv_timer_handler()
#include "ui/ui.h"     // Für ui_tick()

// Extern globales Objekt für UART-Kommunikation
extern UartCommunication uartCom;
extern Config config;
extern const bool DEBUG;

// Implementierung aus main.cpp ausgelagert
bool searchAndLoadController() {
    bool bConfOk = true;
    const unsigned long globalTimeout = 10000;
    unsigned long startTime = millis();

    set_var_s_status("search controller...");
    if(DEBUG) Serial.println("search controller...");

    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_TYPE, "Geräte Typ", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_FLOW_TICKS, "Flow Ticks", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_PRESSURE_AVG, "Pressure Average", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_AKKU_MIN, "Minimalspannung", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_AKKU_CALIB, "Akku Kalibrierungsfaktor", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_BEEP, "Signalton", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_PWR_MANUEL, "Pumpleistung manuell", startTime, globalTimeout);
    if(bConfOk)
      bConfOk = requestAndValidateConfiguration(COM_ID_SYSTEM_OFF, "System idle Abschaltzeit", startTime, globalTimeout);

    if(!bConfOk) {
      if(DEBUG) Serial.println("!!!! lesen vom Controller nicht möglich !!!!");
    }

    return bConfOk;
}

bool requestAndValidateConfiguration(int16_t id, const String &description, unsigned long startTime, unsigned long globalTimeout) {
    String response;
    if(DEBUG) Serial.println("Sende Anfrage für: " + description);
    bool ack = uartCom.sendData('R', id, "", true, 3);
    if(!ack){
      uartCom.reset();
      delay(100);
      ack = uartCom.sendData('R', id, "", true, 3);
      if(!ack) return false;
    }

    while (millis() - startTime < globalTimeout) {
        lv_timer_handler();
        ui_tick();
        uartCom.tick();

        if (uartCom.getRequestResponse(id, response)) {
            if (!response.isEmpty()) {
                if(DEBUG) Serial.println(description + " empfangen: " + response);

                switch(id) {
                  case COM_ID_TYPE:
                    break;
                  case COM_ID_AKKU_MIN:
                    config.akkuMinV = response.toInt();
                    break;
                  case COM_ID_AKKU_CALIB:
                    // TODO
                    break;
                  case COM_ID_BEEP:
                    config.beep = response.toInt();
                    break;
                  case COM_ID_PWR_MANUEL:
                    config.pumpPwrManu = response.toInt();
                    break;
                  case COM_ID_FLOW_TICKS:
                    config.flowTicks = response.toInt();
                    break;
                  case COM_ID_PRESSURE_AVG:
                    config.pressureAvg = response.toInt();
                    break;
                  case COM_ID_SYSTEM_OFF:
                    config.sysPowerOffTime = response.toInt();
                    break;
                  default:
                    break;
                }
                return true;
            }
        }
    }
    if(DEBUG) Serial.println("Fehler: Keine Antwort für " + description + " erhalten.");
    return false;
}
