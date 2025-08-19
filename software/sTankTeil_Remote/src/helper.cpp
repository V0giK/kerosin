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

#include "helper.h"

char buffer[BUFFER_LENGTH];

// Tank type descriptions
const char* TankTypeDescription[] = {
    "Beutel",
    "1-Tank",
    "2-Tank"
};

// Convert integer to char* with optional postfix
const char* int2char(int iVal, LabelPostfixEnum postFix) {
    String s;
    
    if(postFix == LBL_POSTFIX_PROZENT) {
        s = String(iVal) + POSTFIX_PROZENT;
    } else if(postFix == LBL_POSTFIX_ML) {
        s = String(iVal) + POSTFIX_ML;
    } else if(postFix == LBL_POSTFIX_MBAR) {
        s = String(iVal) + POSTFIX_MBAR;
    } else {
        s = String(iVal);
    }
    
    s.toCharArray(buffer, BUFFER_LENGTH);
    return buffer;
}

// Get tank type description
const char* getTankTypeDescription(TankTypeEnum tankType) {
    return TankTypeDescription[tankType];
}

// Get value with bounds checking
int getValue(const char* val, int def, int min, int max) {
    int value = def;
    
    if(val != NULL && val[0] != '\0') {
        value = atoi(val);
    }
    
    if(min <= max) {
        if(value < min) value = min;
        if(value > max) value = max;
    }
    
    return value;
}

// Get model filename based on ID
String getModelFilename(int id) {
    return "/models/" + String(id) + ".json";
}

// Helper function to convert float to string with specified decimal places
float atoff(const char* str) {
    if(str == NULL || str[0] == '\0') {
        return 0.0f;
    }
    return atof(str);
}