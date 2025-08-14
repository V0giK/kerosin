#pragma once

#include <Arduino.h>

// 16 Bit - muss mit Controller (Nano) übereinstimmen
#define INT_BIT_LENGTH 15
//#define INT_BIT_LENGTH ((sizeof(int) * CHAR_BIT)-1)

// prüfen/ setzen des ACK Bit's für die Srrielle Kommunikation
#define isacki(value) (((value) >> (INT_BIT_LENGTH)) & 0x01)
#define acki(value) ((value) |= (1UL << (INT_BIT_LENGTH)))
#define nacki(value) ((value) &= ~(1UL << (INT_BIT_LENGTH)))
#define setacki(value, wack) ((wack)?(acki(value)):(nacki(value)))
#define clracki(value) ((value) = (nacki(value)))


#define POSTFIX_PROZENT " %"
#define POSTFIX_ML      " ml"
#define POSTFIX_MBAR    " mbar"

#define BUFFER_LENGTH 90
extern char buffer[BUFFER_LENGTH];

enum LabelPostfixEnum {
    LBL_POSTFIX_NONE = 1,
    LBL_POSTFIX_PROZENT = 2,
    LBL_POSTFIX_ML = 3,
    LBL_POSTFIX_MBAR = 4
};

enum UartCodesEnum : int16_t {
    COM_ID_TYPE                           = 1,
    COM_ID_FLOW_TICKS                     = 2,
    COM_ID_PRESSURE_AVG                   = 3,
    COM_ID_AKKU_MIN                       = 4,
    COM_ID_AKKU_CALIB                     = 5,
    COM_ID_BEEP                           = 6,
    COM_ID_SYSTEM_OFF                     = 7,
    COM_ID_PWR_MANUEL                     = 8,
    COM_ID_PUMP_PWR                       = 9,
    COM_ID_MAX_REFUEL_TIME                = 10,
    COM_ID_MAX_DEFUEL_TIME                = 11,
    COM_ID_BACK_FUEL_TIME                 = 12,
    COM_ID_AIR_REMOVAL_TIME               = 13,
    COM_ID_PUMP_STOP_EMPTY_DELAY          = 14,
    COM_ID_MESSUREMENT_DELAY              = 15,
    COM_ID_MAX_REFUEL_ML                  = 16,
    COM_ID_MAX_DEFUEL_ML                  = 17,
    COM_ID_MAX_PRESSURE                   = 18,
    COM_ID_PUMP_STOP_PRESSURE_DIFF        = 19,
    COM_ID_PUMP_STOP_PRESSURE_EMPTY       = 20,
    COM_ID_HOPPER_PRESSURE                = 21,
    COM_ID_PUMP_STOP_HOPPER_PRESSURE_DIFF = 22,
    COM_ID_PRESSURE_DROP_HOSE_BREAK       = 23,

    COM_ID_SAVE_MODEL_EEPROM              = 29,
    
    COM_ID_STATUS                         = 30,
    COM_ID_CURRENT_ML                     = 31,
    COM_ID_CURRENT_MBAR                   = 32,
    COM_ID_CURRENT_DIFF_MBAR              = 33,
    COM_ID_AKKU_VOLT                      = 34,

    COM_ID_PUMP_CONTROL                   = 40,
    COM_ID_WRITE_EEPROM                   = 41,
    COM_ID_PUMP_MODE                      = 42,
    COM_ID_FLOWCALIBRATE                  = 43,
    COM_ID_TANKTYPE                       = 44,
    COM_ID_BROADCAST                      = 45,

    COM_ID_MC_RESET                       = 99
};

enum PumpMode {
    MODE_MANUELL     = 0,
    MODE_AUTO        = 1,
    MODE_CALIB_FLOW  = 2,
    MODE_CALIB_MODEL = 3
};

enum PumpAction {
    CTR_ENTTANKEN = 0,
    CTR_TANKEN    = 1,
    CTR_STOP      = 2
};

enum ButtonId {
    BTN_FLOW_RESET            =  1,
    BTN_FLOW_SET              =  2,
    BTN_MODEL_RESET_TANKCOUNT =  3,
    BTN_MODEL_RESET_TOTAL     =  4,
    BTN_MODEL_DELETE          =  5,
    BTN_MODEL_SAVE_YES        =  6,
    BTN_MODEL_SAVE_NO         =  7,
    BTN_SYSTEM_SAVE_YES       =  8,
    BTN_SYSTEM_SAVE_NO        =  9,
    BTN_MODEL_SAVE2CONTROLLER = 10,
    BTN_SNAKE_START           = 11,
    BTN_SNAKE_STOP            = 12,
    BTN_MODEL_TYPE_CHG        = 13
};

enum TankTypeEnum {
    TANK_BEUTEL = 0,
    TANK_1TANK  = 1,
    TANK_2TANK  = 2
};
extern const char* TankTypeDescription[];

const char* int2char(int iVal, LabelPostfixEnum postFix = LBL_POSTFIX_NONE);
const char* getTankTypeDescription(TankTypeEnum tankType);
int getValue(const char* val, int def = 0, int min = 0, int max = -1);
String getModelFilename(int id);

