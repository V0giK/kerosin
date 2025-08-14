#pragma once

// 16 Bit
#define INT_BIT_LENGTH 15

// prüfen/ setzen des ACK Bit's für die Srrielle Kommunikation
#define isacki(value) (((value) >> (INT_BIT_LENGTH)) & 0x01)
#define acki(value) ((value) |= (1UL << (INT_BIT_LENGTH)))
#define nacki(value) ((value) &= ~(1UL << (INT_BIT_LENGTH)))
#define setacki(value, wack) ((wack)?(acki(value)):(nacki(value)))
#define clracki(value) ((nacki(value)))


#define TYPE_PUMP "1"

#define HARDWARE_TYPE TYPE_PUMP

#define PIN_MOTE_12V 2
#define PIN_MOTE_TXRX 13
#define PIN_SOFT_RX 11
#define PIN_SOFT_TX 10

#define PIN_PUMP_D0 5
#define PIN_PUMP_D1 6

#define PIN_FLOW 3
#define PIN_PRESSURE A6

#define PIN_BATT_MESURE A0
#define PIN_LED_MODEL   A1
#define PIN_LED_MANUEL  A2
#define PIN_LED_BATT    9
#define PIN_TASTER_IN   7
#define PIN_TASTER_OUT  8
#define PIN_MOTE_CHK    12
#define PIN_BUZZER 4


// UART Nachrichten IDs
#define COM_ID_TYPE                           1
#define COM_ID_FLOW_TICKS                     2
#define COM_ID_PRESSURE_AVG                   3
#define COM_ID_AKKU_MIN                       4
#define COM_ID_AKKU_CALIB                     5
#define COM_ID_BEEP                           6
#define COM_ID_SYSTEM_OFF                     7
#define COM_ID_PWR_MANUEL                     8
#define COM_ID_PUMP_PWR                       9
#define COM_ID_MAX_REFUEL_TIME                10
#define COM_ID_MAX_DEFUEL_TIME                11
#define COM_ID_BACK_FUEL_TIME                 12
#define COM_ID_AIR_REMOVAL_TIME               13
#define COM_ID_PUMP_STOP_EMPTY_DELAY          14
#define COM_ID_MESSUREMENT_DELAY              15
#define COM_ID_MAX_REFUEL_ML                  16
#define COM_ID_MAX_DEFUEL_ML                  17
#define COM_ID_MAX_PRESSURE                   18
#define COM_ID_PUMP_STOP_PRESSURE_DIFF        19
#define COM_ID_PUMP_STOP_PRESSURE_EMPTY       20
#define COM_ID_HOPPER_PRESSURE                21
#define COM_ID_PUMP_STOP_HOPPER_PRESSURE_DIFF 22
#define COM_ID_PRESSURE_DROP_HOSE_BREAK       23

#define COM_ID_SAVE_MODEL_EEPROM              29

#define COM_ID_STATUS                         30
#define COM_ID_CURRENT_ML                     31
#define COM_ID_CURRENT_MBAR                   32
#define COM_ID_CURRENT_DIFF_MBAR              33
#define COM_ID_AKKU_VOLT                      34

#define COM_ID_PUMP_CONTROL                   40
#define COM_ID_WRITE_EEPROM                   41
#define COM_ID_PUMP_MODE                      42
#define COM_ID_FLOWCALIBRATE                  43
#define COM_ID_TANKTYPE                       44
#define COM_ID_BROADCAST                      45

#define COM_ID_MC_RESET                       99

// Pumpmodes
#define MODE_MANUELL     0
#define MODE_AUTO        1
#define MODE_CALIB_FLOW  2
#define MODE_CALIB_MODEL 3

// Tanktype
#define TANK_BEUTEL 0
#define TANK_1TANK  1
#define TANK_2TANK  2

// Pumpaktionen
#define CTR_ENTTANKEN 0
#define CTR_TANKEN    1
#define CTR_STOP      2


//
#define REFUELL_BEUTEL     "1,2,1,4"
#define DEFUELL_BEUTEL     "3"
#define REFUELL_1TANK      "1,4"
#define DEFUELL_1TANK      "3"
#define REFUELL_2TANKS     "1,5,4"
#define DEFUELL_2TANKS     "3"
