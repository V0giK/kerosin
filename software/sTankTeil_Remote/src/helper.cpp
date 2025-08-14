#include "helper.h"

char buffer[BUFFER_LENGTH];

const char* int2char(int iVal, LabelPostfixEnum postFix) {
    memset(buffer, 0, BUFFER_LENGTH); // Buffer zurücksetzen

    // Integer zu char* umwandeln
    snprintf(buffer, BUFFER_LENGTH, "%d", iVal);
    if(buffer == NULL) {
        snprintf(buffer, BUFFER_LENGTH, "%d", 0);
    }

    // Postfix hinzufügen, falls vorhanden
    switch (postFix) {
        case LBL_POSTFIX_PROZENT:
            strncat(buffer, POSTFIX_PROZENT, BUFFER_LENGTH - strlen(buffer) - 1);
            break;
        case LBL_POSTFIX_ML:
            strncat(buffer, POSTFIX_ML, BUFFER_LENGTH - strlen(buffer) - 1);
            break;
        case LBL_POSTFIX_MBAR:
            strncat(buffer, POSTFIX_MBAR, BUFFER_LENGTH - strlen(buffer) - 1);
            break;
        case LBL_POSTFIX_NONE:
        default:
            break; // Kein Postfix hinzufügen
    }

    return buffer;
}

const char* TankTypeDescription[] = {
    "Beutel",
    "1 Tank",
    "2 Tanks"
};

const char* getTankTypeDescription(TankTypeEnum tankType) {
    return TankTypeDescription[tankType];
}

// für Serial.println(...); aus einer .c
extern "C" void printMessage(const char *message) {
    Serial.println(message);
}

// Werte die vom UI gelesen werden verifizieren
int getValue(const char* val, int def, int min, int max) {
    if(strlen(val) == 0) {
        return def;
    }
    int iVal = atoi(val);
    if(min != -1) {
        if(iVal < min) iVal = min;
    }
    if(max != -1) {
        if(iVal > max) iVal = max;
    }

    return iVal;
}

String getModelFilename(int id){
    return "/models/" + String(id) + ".json";
}