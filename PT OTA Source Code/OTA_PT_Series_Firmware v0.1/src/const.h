
#define  PRODUCT_ID             1
#define  SOFTWARE_REVISION      1
 
#define  NO                 0
#define  YES                1
#define  FALSE              0
#define  TRUE               1
#define  NOT_CONNECTED      0
#define  CONNECTED          1
#define  DIGITAL            0
#define  ANALOG             1
#define  NEGATIVE           0
#define  POSITIVE           1
#define  NOT_READY          0
#define  READY              1
#define  FAIL               0
#define  SUCCESS            1

#define  BLE_NO_MATCH                      0
#define  BLE_RSP_ATTRIBUTES_WRITE          1
#define  BLE_RSP_GAP_SET_MODE              2
#define  BLE_EVT_SYSTEM_PROTOCOL_ERROR     3
#define  BLE_EVT_SYSTEM_BOOT               4
#define  BLE_EVT_ATTRIBUTES_VALUE          5
#define  BLE_EVT_ATTRIBUTES_STATUS         6
#define  BLE_EVT_CONNECTION_STATUS         7
#define  BLE_EVT_CONNECTION_DISCONNECTED   8

#define   BLUETIMER_0d125_SECOND    5
#define   BLUETIMER_0d25_SECOND     10
#define   BLUETIMER_0d33_SECOND     14
#define   BLUETIMER_0d5_SECOND      20
#define   BLUETIMER_1_SECOND        40
#define   BLUETIMER_2_SECOND        80
#define   BLUETIMER_3_SECOND        120
#define   BLUETIMER_4_SECOND        160
#define   BLUETIMER_5_SECOND        200
#define   BLUETIMER_10_SECOND       400
#define   BLUETIMER_100_SECOND      4000

#define  BUFFER_SIZE        256
#define  BUFFER_SIZE_DIV_2  128

#define BLACK               0b00000000
#define RED                 0b00000001
#define GREEN               0b00000010
#define YELLOW              0b00000011
#define BLUE                0b00000100
#define PURPLE              0b00000101
#define AQUA                0b00000110
#define WHITE               0b00000111

#define XDATA               8

// S_CS - up output - peripheral input
#define S_CS        LATAbits.LATA4

// S_SCLK - up output - peripheral input
#define S_SCLK      LATAbits.LATA3

// S_DOUT - uP input - peripheral output
//#define S_DOUT      LATAbits.LATA1
#define S_DOUT      PORTAbits.RA1

// S_DIN - uP input - peripheral output
//#define S_DIN       PORTAbits.RA2
#define S_DIN       LATAbits.LATA2

// S_DRDY - up input - peripheral output
#define S_DRDY      PORTAbits.RA5

#define ADC_RESET   0b00000110            // 0000 011x
#define STARTSYNC   0b00001000            // Start or restart conversions 0000 100x
#define POWERDOWN   0b00000010            // Enter power-down mode 0000 001x
#define RDATA       0b00010000            // Read data by command 0001 xxxx
#define RREG        0b00100000            // Read nn registers starting at address rr 0010 rrnn
#define ADC_WREG    0b01000000            // Write nn registers starting at address rr 0100 rrnn
#define ADC_WREG    0b01000000            // Write nn registers starting at address rr 0100 rrnn
#define BURNOUT     0b00001001            // Start or restart conversions 0000 100x with burnout currents

// Operands: rr = configuration register (00 to 11), nn = number of bytes ? 1 (00 to 11), and x = don't care.
//----------------------------------------
// ADS1120 Configuration Register 0
//----------------------------------------
// bits     7:4     MUX[3:0]
// bits     3:1     GAIN[2:0]
// bits     0       PGA_BYPASS
#define ADS1120_CONFIG_0    0b00111010
//#define ADS1120_CONFIG_0    0b00111110

//----------------------------------------
// ADS1120 Configuration Register 1
//----------------------------------------
// bits     7:5     DR[2:0]
// bits     4:3     MODE[1:0]
// bits     2       CM
// bits     1       TS
// bits     0       BCS
#define ADS1120_CONFIG_1    0b00000000

//----------------------------------------
// ADS1120 Configuration Register 2
//----------------------------------------
// bits     7:6     VREF[1:0]
// bits     5:4     50/60[1:0] 
// bits     3       PSW
// bits     2:0     IDAC[2:0]
#define ADS1120_CONFIG_2    0b10001000

//----------------------------------------
// ADS1120 Configuration Register 3
//----------------------------------------
// bits     7:5     I1MUX[2:0]
// bits     4:2     I2MUX[2:0]
// bits     1       DRDYM
// 0                Reserved  always write 0
#define ADS1120_CONFIG_3    0b00000000


#define CLOCK_SPEED_LOW     0
#define CLOCK_SPEED_HIGH    1

#define  BOOT               0
#define  NORMAL             1