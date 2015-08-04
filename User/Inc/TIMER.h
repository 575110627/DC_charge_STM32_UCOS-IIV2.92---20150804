
#define FM24CL04READADDRESS   0xA0
#define FM24CL04WRITEADDRESS  0xA1
#define SD2405APIREADADDRESS   0x65
#define SD2405APIWRITEADDRESS  0x64 
void I2C_Config(void);
void Read_Config(void);
void Write_Config(void);
void Set_Time( RTC_SD2405_HandleTypeDef *);
 void Read_Time(RTC_SD2405_HandleTypeDef *);
