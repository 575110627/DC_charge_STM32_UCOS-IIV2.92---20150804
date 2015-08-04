/********SD2400º¯ÊýÃû********/
 void I2CInit(void);

 void I2CWait(void);
 uint8_t  I2CStart(void);
 void I2CStop(void);
 void I2CAck(void);
 void I2CNoAck(void);
 uint8_t  I2CWaitAck(void);
 void I2CSendByte(uint8_t demand);
 uint8_t I2CReceiveByte(void);

// void delayus(uword nn);
