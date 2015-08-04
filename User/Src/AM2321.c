#include "stm32f2xx_hal.h"


static GPIO_InitTypeDef  GPIO_InitStruct_AM2321;
static uint8_t Read_SensorData(void)	;	 //读单个字节
#define AM2321_DATA_IN  HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)
#define AM2321_OUT_HIGH HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_SET);
#define AM2321_OUT_LOW  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_RESET);


void  AM2321_Set_Input(void)
{
	__GPIOG_CLK_ENABLE();
	GPIO_InitStruct_AM2321.Pin = (GPIO_PIN_12);
    GPIO_InitStruct_AM2321.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct_AM2321.Pull = GPIO_PULLUP;
    GPIO_InitStruct_AM2321.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct_AM2321);
}
	
void  AM2321_Set_Output(void)
{
	__GPIOG_CLK_ENABLE();
	GPIO_InitStruct_AM2321.Pin = (GPIO_PIN_12);
    GPIO_InitStruct_AM2321.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct_AM2321.Pull = GPIO_PULLUP;
    GPIO_InitStruct_AM2321.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct_AM2321);
}
uint8_t Read_SensorData(void)		 //读单个字节																								  ubyte Read_SensorData(void)		 //读单个字节
{
	uint8_t i;
	uint8_t buffer,tmp;
	uint8_t retry=0;
	buffer = 0;
	for(i=0;i<8;i++)
	{
		//检测上次低电平是否结束
		while(!(AM2321_DATA_IN ))
		{
			retry++;
			if(retry >80)
		     break;
			HAL_Delay1us(1);
		}
		retry=0;
		//延时Min=26us Max50us 跳过数据"0" 的高电平
		HAL_Delay1us(20);
    HAL_Delay1us(30);
		//判断传感器发送数据位
		tmp =0;
		if(AM2321_DATA_IN)
		{
			tmp=1;
		}
		//等待高电平 结束
		while(AM2321_DATA_IN)
		{
			retry++;
			if(retry >80)
			  break;
			HAL_Delay1us(1);
		}
		buffer <<=1;
		buffer |= tmp;	
	}
	return buffer;
}

/********************************************\
|* 功能： 读传感器              	        *|
\********************************************/
uint8_t Read_Sensor(void)
{
  uint8_t i;
	uint16_t retry=0;
	//温湿度变量
  uint8_t Sensor_Data[5] = {0,0,0,0,1};
	//主机起始信号拉低(Min=800US typ=1ms Max=20Ms) 
	AM2321_Set_Output();
	AM2321_OUT_LOW;
	//延时10Ms 
	//HAL_Delay1us(1000);
	HAL_Delay(1);
	AM2321_OUT_HIGH;
	//释放总线 延时(Min=20us typ=30us Max=200us)
	HAL_Delay1us(30); 	
	//主机设为输入 判断传感器响应信号 	 
  //设置输入
	AM2321_Set_Input();
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	//if(AM2321_DATA_IN == GPIO_PIN_RESET)		   //收到传感器信号
	   //判断从机是否发出 80us 的低电平响应信号是否结束	 
	 HAL_Delay1us(50);
	if(AM2321_DATA_IN == GPIO_PIN_RESET)  		   //收到传感器信号
	{		
		 
	   while(AM2321_DATA_IN ==0)
	   {
		   retry++;
		   HAL_Delay1us(1);
		   if(retry >= 100 )
					break;
	   }
	   retry = 0;
	    //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	   while(AM2321_DATA_IN)
	   {
			 retry++;
			 HAL_Delay1us(1);
			 if(retry >= 100)
			 {
					break;
			 } 		
	   }
	    // 数据接收	传感器共发送40位数据 
	    // 即5个字节 高位先送  5个字节分别为湿度高位 湿度低位 温度高位 温度低位 校验和
	    // 校验和为：湿度高位+湿度低位+温度高位+温度低位
	  for(i=0;i<5;i++)
		{
		  Sensor_Data[i] = Read_SensorData();
		}
		//if((Sensor_Data[0]+Sensor_Data[1]+Sensor_Data[2]+Sensor_Data[3])!=Sensor_Data[4]) 
			if(0)
			return 0;
		else
		{
			if((Sensor_Data[2]&0x80) == 1)
			{
				Sensor_Data[2] = Sensor_Data[2]&0x7F;
				AM2321Temperature = Sensor_Data[2]*256 + Sensor_Data[3] ;
				AM2321Temperature = -AM2321Temperature + 400;
			}
			else
			{
				AM2321Temperature = Sensor_Data[2]*256 + Sensor_Data[3] + 400 ;
			}
		    Am2321Humidity = Sensor_Data[0]*256 + Sensor_Data[1];
			return 1;
		}	
	}	
	return 0;	
}    



