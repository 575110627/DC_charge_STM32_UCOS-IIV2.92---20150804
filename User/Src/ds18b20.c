#include "stm32f2xx_hal.h"

static GPIO_InitTypeDef  GPIO_InitStructure_DS18B20;
/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : Configure a SysTick Base time to 1 us.
* Input          : None
* Output         : None
* Return         : None
*****************************************************/

/*void delay1us(uint8_t delaytime)
{
  uint8_t i=20;
  GlobalTimingDelay = delaytime;
  while(GlobalTimingDelay !=0)
	{
	  while(i--);
	}
}*/

// 设置DS18B20的IO口DQ 为输入
//返回1:不存在
//返回0:存在    	 
void DS18B20_Set_Input(void)
{
  GPIO_InitStructure_DS18B20.Pin = (GPIO_PIN_5);
  GPIO_InitStructure_DS18B20.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure_DS18B20.Pull = GPIO_PULLUP;
  GPIO_InitStructure_DS18B20.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure_DS18B20);
}
// 设置DS18B20的IO口DQ 为输出
//返回1:不存在
//返回0:存在    	 
void DS18B20_Set_Output(void)
{
  /* Configure the GPIO pin */
  GPIO_InitStructure_DS18B20.Pin = (GPIO_PIN_5);
  GPIO_InitStructure_DS18B20.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure_DS18B20.Pull = GPIO_PULLUP;
  GPIO_InitStructure_DS18B20.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure_DS18B20);
}

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
uint8_t DS18B20_Init(void)
{
  //SysTick_Configuration();
 
  /* Enable the Clock */
  //RCC_AHB1PeriphClockCmd(DS18B20_GPIO_CLK, ENABLE);
  __GPIOE_CLK_ENABLE();
  /* Configure the GPIO pin */
  DS18B20_Set_Output();

  DS18B20_Reset();
  return DS18B20_Check();
}  

//复位DS18B20  复位脉冲（最短为480uS的低电平信号）
void DS18B20_Reset(void)	   
{        
	DS18B20_Set_Output(); //SET AS OUTPUT
	DS18B20_OUT_LOW;      //拉低DQ
  HAL_Delay1us(650);        //拉低650us
  DS18B20_OUT_HIGH;
	HAL_Delay1us(10);       // 10US
}


//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
    
	DS18B20_Set_Input();    // 总线主机便释放此线并进入接收方式（Rx）。
	// DSl820 等待15.60uS 并且接着发送存在脉冲（60-240uS的低电平信号。）
  while ((DS18B20_DATA_IN == 1) && (retry<60))
	{
		retry++;
		HAL_Delay1us(2);
	};	 
    
	if(retry>=60)
        return 1;
    
	retry=0;
  while ((DS18B20_DATA_IN == 0) && (retry<245))
	{
		retry++;
		HAL_Delay1us(2);
	};
	if(retry>=245)
        return 1;    
    
	return 0;
}



//从DS18B20读取一个位
//返回值：1/0
uint8_t DS18B20_Read_Bit(void) 			 // read one bit
{
  uint8_t data;
    
	DS18B20_Set_Output();   //SET AS OUTPUT
  DS18B20_OUT_LOW; 
	HAL_Delay1us(3);
  DS18B20_OUT_HIGH; 
	DS18B20_Set_Input();    //SET AS INPUT
	HAL_Delay1us(6);
	if(DS18B20_DATA_IN)
    {
        data=1;
    }
    else
    {
        data=0;  
    }
    HAL_Delay1us(50); 
    
    return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
uint8_t DS18B20_Read_Byte(void)    // read one byte
{        
    uint8_t i,j,dat;
    dat=0;
		for (i=1;i<=8;i++) 
		{
					j=DS18B20_Read_Bit();
					dat=dat>>1;
					dat+=(j<<7);
		}	

    return dat;
}

//写一个字节到DS18B20
//dat：要写入的字节
/******************************* 写DS18B20函数 ******************************************/
void DS18B20_Write_Byte(uint8_t dat)     
{             
  uint8_t j;
  uint8_t testb;
	DS18B20_Set_Output();//SET AS OUTPUT;
  for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;

        DS18B20_OUT_LOW;// Write 1
        HAL_Delay1us(2);  
        if (testb) 
        {
            DS18B20_OUT_HIGH;
        }
        HAL_Delay1us(55);
        DS18B20_OUT_HIGH;
        HAL_Delay1us(5);                          
  }
}

//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
short DS18B20_Get_Temp(void)
{
    uint8_t i, temp;
    uint8_t TL,TH;
	  short tem;
    uint8_t str[9];
    
    //开始温度转换
    DS18B20_Reset();	   
	  DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert

    //开始读取温度
    DS18B20_Reset();
	  DS18B20_Check();
 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	
    //TL=DS18B20_Read_Byte(); // LSB   
    //TH=DS18B20_Read_Byte(); // MSB  
    for (i=0;i<9;i++) 
	{    
	    str[i] = DS18B20_Read_Byte();
    }  
    TL = str[0]; // LSB   
    TH = str[1]; // MSB      
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }
    else
    {
        temp=1;//温度为正         
    }
    tem=TH<<8 | TL; //获得不带符号位的11位温度值
    //转换 *0.625
    tem = tem*10;
    tem = tem>>4;
		if(temp)
			return (tem + 400); //返回温度值
		else 
			return (-tem + 400);    
} 

