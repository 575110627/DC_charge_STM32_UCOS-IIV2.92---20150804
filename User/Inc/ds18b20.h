

#define DS18B20_DATA_IN  HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5)
#define DS18B20_OUT_HIGH HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5,GPIO_PIN_SET);
#define DS18B20_OUT_LOW  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5,GPIO_PIN_RESET);

uint8_t DS18B20_Init(void);//初始化DS18B20
short DS18B20_Get_Temp(void);//获取温度
static void DS18B20_Write_Byte(uint8_t dat);//写入一个字节
static uint8_t DS18B20_Read_Byte(void);//读出一个字节
static uint8_t DS18B20_Read_Bit(void);//读出一个位
static uint8_t DS18B20_Check(void);//检测是否存在DS18B20
static void DS18B20_Reset(void);       //复位DS18B20    















