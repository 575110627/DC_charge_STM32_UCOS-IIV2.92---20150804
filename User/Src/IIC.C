#include "stm32f2xx_hal.h"
#define true 1
#define false 0

/*********I2C初始化***********/

static GPIO_InitTypeDef  GPIO_InitStruct_iic;
#define IIC_DATA_IN  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)
#define IIC_SCLOUT_HIGH HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_SET);
#define IIC_SCLOUT_LOW  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_RESET);
#define IIC_SDAOUT_HIGH HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET);
#define IIC_SDAOUT_LOW  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET);


void IIC_SetOutput(void)
{
	__GPIOB_CLK_ENABLE();
	//GPIOB
	GPIO_InitStruct_iic.Pin = (GPIO_PIN_6|GPIO_PIN_7);
  GPIO_InitStruct_iic.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct_iic.Pull = GPIO_PULLUP;
  GPIO_InitStruct_iic.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_iic);
}

void IIC_SetInput()
{
	__GPIOB_CLK_ENABLE();
	GPIO_InitStruct_iic.Pin = (GPIO_PIN_7);
  GPIO_InitStruct_iic.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct_iic.Pull = GPIO_PULLUP;
  GPIO_InitStruct_iic.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_iic);
	
	GPIO_InitStruct_iic.Pin = (GPIO_PIN_6);
  GPIO_InitStruct_iic.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct_iic.Pull = GPIO_PULLUP;
  GPIO_InitStruct_iic.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_iic);
	
}
/*********I2C延时***********/

void I2CWait(void)     //延时   1us
{	
    uint8_t i=0;
    for(i=0;i<30;i++);    
}

/*********延时子程序*********/
void delayus(uint16_t nn)     //延时   0.1us
{
   uint8_t i = 0;
   while(nn)
   {
      for(i=0;i<2;i++);
      nn--;   
   }
}


/********开启SD2400的I2C总线********/

uint8_t I2CStart(void)
{
	//P0_IOCR01      =  0x00C0;//SDA设置为输出
  //  P2_IOCR08      =  0x00C0;//SCL设置为输出
	IIC_SetOutput();
  delayus(1);
  //SDA=1;
	//SCL=1;
	IIC_SCLOUT_HIGH;
	IIC_SDAOUT_HIGH;
	I2CWait();
  //P0_IOCR01      =  0x0000;//SDA设置为输入
	IIC_SetInput();
  //	I2CWait();
  if(!IIC_DATA_IN)
		  return false;	//SDA线为低电平则总线忙,退出
  //	P0_IOCR01      =  0x00C0;//SDA设置为输出
  // SDA=0;
	IIC_SDAOUT_LOW;
  I2CWait();
	while(IIC_DATA_IN)return false;	//SDA线为高电平则总线出错,退出
	//SCL=0;
	IIC_SCLOUT_LOW;
	I2CWait();
	return true;
}


/********关闭I2C总线*******/

void I2CStop(void)
{
  // P2_IOCR08      =  0x00C0;//SCL设置为输出
	//P0_IOCR01      =  0x00C0;//SDA设置为输出
	IIC_SetOutput();
  delayus(1);
  //SDA=0;       
	//SCL=0;
	IIC_SDAOUT_LOW;
	IIC_SCLOUT_LOW;
	I2CWait();
	//SCL=1;
	IIC_SCLOUT_HIGH;
	I2CWait();
	//SDA=1;
	IIC_SDAOUT_HIGH;
}


/*********IIC发送 ACK*********/

void I2CAck(void)
{	
  // P2_IOCR08      =  0x00C0;//SCL设置为输出
  //  P0_IOCR01      =  0x00C0;//SDA设置为输出
	IIC_SetOutput();
	delayus(1);
	IIC_SDAOUT_LOW;
	IIC_SCLOUT_LOW;
  //SDA=0;
	//SCL=0;
	I2CWait();
	//SCL=1;
	IIC_SCLOUT_HIGH;
	I2CWait();
	//SCL=0;
	IIC_SCLOUT_LOW;
	
} 


/*********IIC发送NO ACK*********/

void I2CNoAck(void)
{	
	//P0_IOCR01      =  0x00C0;//SDA设置为输出
	IIC_SetOutput();
  delayus(1);
  //SDA=1;
	//SCL=0;
  IIC_SDAOUT_HIGH;
	IIC_SCLOUT_LOW;
	I2CWait();
  //SCL=1;
	IIC_SCLOUT_HIGH;
	I2CWait();
	//SCL=0;
	IIC_SCLOUT_LOW;
  //P0_IOCR01      =  0x0000;//SDA设置为输入
	IIC_SetInput();
}


/*********IIC读取ACK信号*********/

uint8_t I2CWaitAck(void) 	 //返回为:1=有ACK,0=无ACK
{
	uint8_t errtime = 255;
    //P2_IOCR08      =  0x00C0;//SCL设置为输出
    //P0_IOCR01      =  0x0000;//SDA设置为输入
	IIC_SetInput();
    //SDA = 1;
	IIC_SDAOUT_HIGH;
  delayus(1);
  //SCL=0;
  IIC_SCLOUT_LOW;	
	I2CWait();
	//SCL=1;
	IIC_SCLOUT_HIGH;
  I2CWait();

	while(IIC_DATA_IN)
	{
		errtime--;
        delayus(1);
        if(!errtime)
        {
            I2CWait();
            I2CWait();
            //SCL=0;
					  IIC_SCLOUT_LOW;
    		return false;
        }
	}
	//SCL=0;
	IIC_SCLOUT_LOW;
  //P0_IOCR01      =  0x00C0;//SDA设置为输出
	IIC_SetOutput();
	return true;
} 


/************IIC发送一个字节*************/

void I2CSendByte(uint8_t demand) //数据从高位到低位//
{
	uint8_t i=8;                       
	//P0_IOCR01      =  0x00C0;//SDA设置为输出  
  //P2_IOCR08      =  0x00C0;//SCL设置为输出    
  IIC_SetOutput();	
	
	while(i--)
	{
		//SCL=0;
		IIC_SCLOUT_LOW;
    delayus(1);
    if(demand&0x80)
		{
		    //SDA = 1;
			IIC_SDAOUT_HIGH;
		}
		else
		{
		    //SDA = 0;
			IIC_SDAOUT_LOW;
		}
		demand<<=1;
		I2CWait();
		//SCL=1;
		IIC_SCLOUT_HIGH;
		I2CWait();
	}
	//SCL=0;
	IIC_SCLOUT_LOW;
} 


/*********IIC读入一字节*********/

uint8_t I2CReceiveByte(void)  //数据从高位到低位//
{
	uint8_t i=8;
	uint8_t ddata=0;
   // P2_IOCR08      =  0x00C0;//SCL设置为输出
   // P0_IOCR01      =  0x0000;//SDA设置为输入
  IIC_SetInput();
  //SDA = 1;
	IIC_SDAOUT_HIGH;
  delayus(1);
	
	while(i--)
	{
		ddata<<=1;      //数据从高位开始读取
        //SCL=0;
		IIC_SCLOUT_LOW;
		I2CWait();
		//SCL=1;
		IIC_SCLOUT_HIGH;
		I2CWait();	//从高位开始 ddata|=SDA;ddata<<=1
		if(IIC_DATA_IN)
		{
			ddata|=0x01;
		}
	}
	//SCL=0;
	IIC_SCLOUT_LOW;
  // P0_IOCR01      =  0x00C0;//SDA设置为输出
	IIC_SetOutput();	
	return ddata;
}

