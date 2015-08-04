

#define DS18B20_DATA_IN  HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5)
#define DS18B20_OUT_HIGH HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5,GPIO_PIN_SET);
#define DS18B20_OUT_LOW  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5,GPIO_PIN_RESET);

uint8_t DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(void);//��ȡ�¶�
static void DS18B20_Write_Byte(uint8_t dat);//д��һ���ֽ�
static uint8_t DS18B20_Read_Byte(void);//����һ���ֽ�
static uint8_t DS18B20_Read_Bit(void);//����һ��λ
static uint8_t DS18B20_Check(void);//����Ƿ����DS18B20
static void DS18B20_Reset(void);       //��λDS18B20    















