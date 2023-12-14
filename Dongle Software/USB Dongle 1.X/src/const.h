#define  LED_OFF                _LATA0 = 0; _LATA1 = 0; _LATB2 = 0
#define  LED_GREEN              _LATA0 = 0; _LATA1 = 0; _LATB2 = 1
#define  LED_RED                _LATA0 = 0; _LATA1 = 1; _LATB2 = 0
#define  LED_YELLOW             _LATA0 = 0; _LATA1 = 1; _LATB2 = 1
#define  LED_BLUE               _LATA0 = 1; _LATA1 = 0; _LATB2 = 0
#define  LED_AQUA               _LATA0 = 1; _LATA1 = 0; _LATB2 = 1
#define  LED_PURPLE             _LATA0 = 1; _LATA1 = 1; _LATB2 = 0
#define  LED_WHITE              _LATA0 = 1; _LATA1 = 1; _LATB2 = 1
   
#define  NO                 0
#define  YES                1
#define  YES_RECEIVED       2
#define  RESET              0
#define  SET                1
#define  FALSE              0
#define  TRUE               1
#define  NONE               255
#define  NOT_CONNECTED      0
#define  CONNECTED          1

#define  BOUNCE         1
#define  NO_MATCH       255
#define  NO_PACKET      255

#define   DISCOVER                  0
#define   CONNECT_DIRECT            1
#define   END_PROCEDURE             2
#define   BOOT                      3
#define   PROTOCOL_ERROR            4
#define   STATUS                    5
#define   DISCONNECTED              6
#define   PROCEDURE_COMPLETED       7
#define   GROUP_FOUND               8
#define   FIND_INFORMATION_FOUND    9
#define   ATTRIBUTE_VALUE           10
#define   SCAN_RESPONSE             11

#define   TEST      0
#define   DEMO      1

#define   BUFFER_SIZE           512
#define   BUFFER_SIZE_LARGE     10000
#define   BUFFER_TRIP_HIGH      4
#define   BUFFER_TRIP_LOW       1

//#define   HALF_BUFFER_SIZE BUFFER_SIZE / 2

#define   BLUETIMER_0d125_SECOND    2
#define   BLUETIMER_0d25_SECOND     5
#define   BLUETIMER_0d33_SECOND     7
#define   BLUETIMER_0d5_SECOND      10
#define   BLUETIMER_1_SECOND        20
#define   BLUETIMER_2_SECOND        40
#define   BLUETIMER_3_SECOND        60
#define   BLUETIMER_4_SECOND        80
#define   BLUETIMER_5_SECOND        100
#define   BLUETIMER_10_SECOND       200
#define   BLUETIMER_100_SECOND      1000


#define   CONN_INTERVAL_MIN     16          // 16 x 1.25ms = 20ms  // Craig was 16 // I have made it 40mSec //32 SC 16
#define   CONN_INTERVAL_MAX     40          // 40 x 1.25ms = 50ms  // Craig was 40 // 40
#define   TIME_OUT              800         // changed to 8 sec // 1000  // 100 x 10ms = 1000ms (must be greater than (1 + LATENCY) * CONN_INTERVAL_MAX * 2) Craig was 100
#define   LATENCY               4           // (1 + 4) x 50ms = 250ms   (1 + LATENCY) * CONN_INTERVAL_MAX) Craig was 4

#define   FIRST_HANDLE          0x0001
#define   LAST_HANDLE           0xFFFF

#define   NO_SECOND_CHAR        0xFFFF

#define   EEPROM_NAME_ADDRS_1   0
#define   EEPROM_NAME_ADDRS_2   16
#define   EEPROM_NAME_ADDRS_3   32
#define   EEPROM_NAME_ADDRS_4   48

#define   EEPROM_BT_ADDRS_1     64
#define   EEPROM_BT_ADDRS_2     72
#define   EEPROM_BT_ADDRS_3     80
#define   EEPROM_BT_ADDRS_4     88

#define   EEPROM_BATTERY        100

#define   EEPROM_MEM_ADDRS_1    254
#define   EEPROM_MEM_ADDRS_2    255

#define   BATTERY_THRESHOLD     120
#define   DATA_TIMEOUT          40     // 2 seconds

#define   ON_BLACK              0
#define   ON_WHITE              1

#define   START_HEADER                       0x80
#define   STOP_HEADER                        0x81
#define   TEST_CONNECTION_HEADER             0x82
#define   NO_CONNECTION_HEADER               0x83
#define   CONNECTION_ESTABLISHED_HEADER      0x84
#define   REBOOT_DONGLE                      0x85
#define   GATT_DATA_HEADER                   0x87
#define   STOP_BLUETOOTH                     0x89
#define   BLE_STATES_HEADER                  0x8A
#define   TRACE_HEADER                       0x8B
#define   GETGATT_HEADER                     0x8C
#define   DISCONNECT_BLUETOOTH               0x8F
#define   DONGLE_WRITE_BACK_HEADER           0x92
#define   ENABLE_BT_SEQ                      0x93
#define   BT_HAS_DISCONNECTED                0x94
#define   DISCONNECTION_ERROR                0x95

#define   MULTITOOL                          0
#define   SG_SENSOR                          1

