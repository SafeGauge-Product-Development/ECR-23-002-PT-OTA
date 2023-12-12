
void XReceiveBTMessage (uint8 BTdata);
void XBLE_state_manage(void);
void bleReset(unsigned char reset);
void mcuRtsHigh(void);
void mcuRtsLow(void);
unsigned char mcuCts(void);
void readInCalibrationPoints(void);
void shootUart1Data(unsigned char data);
void ledColour(unsigned char colour);
void readDataSensor(unsigned char data);
void setupADS1120(void);
void readAllADS1120configRegisters(void);
void WriteEEByte(uint16 address, uint8 data);
uint8 ReadEEByte(uint16 address);

