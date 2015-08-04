#include "stm32f2xx_hal.h"

extern I2C_HandleTypeDef I2C1Handle;

void I2C_Config(void)
{
	I2C1Handle.Instance             = I2C1;
	//I2C_ADDRESSINGMODE_7BIT 
    //I2C1Handle.Init.AddressingMode  = I2C_MEMADD_SIZE_8BIT;
	I2C1Handle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2C1Handle.Init.ClockSpeed      = 100000;
    I2C1Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    I2C1Handle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
    I2C1Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    I2C1Handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLED;
    I2C1Handle.Init.OwnAddress1     = 0;//I2C_ADDRESS;
    I2C1Handle.Init.OwnAddress2     = 0;

	HAL_I2C_Init(&I2C1Handle);
}
void Read_Config(void)
{
	I2C1Handle.Instance             = I2C1;
    I2C1Handle.Init.AddressingMode  = I2C_MEMADD_SIZE_8BIT;
    I2C1Handle.Init.ClockSpeed      = 100000;
    I2C1Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    I2C1Handle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
    I2C1Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    I2C1Handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLED;
    I2C1Handle.Init.OwnAddress1     = 0xA1;//I2C_ADDRESS;
    I2C1Handle.Init.OwnAddress2     = 0;

	HAL_I2C_Init(&I2C1Handle);
}

void Write_Config(void)
{
	I2C1Handle.Instance             = I2C1;
    I2C1Handle.Init.AddressingMode  = I2C_MEMADD_SIZE_8BIT;
    I2C1Handle.Init.ClockSpeed      = 100000;
    I2C1Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    I2C1Handle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
    I2C1Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    I2C1Handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLED;
    I2C1Handle.Init.OwnAddress1     = 0xA0;//I2C_ADDRESS;
    I2C1Handle.Init.OwnAddress2     = 0;

	HAL_I2C_Init(&I2C1Handle);
}

void Set_Time( RTC_SD2405_HandleTypeDef *settime)
{
	uint8_t i = 0;
	uint8_t timebuf[7];
	uint8_t temp;
	temp = 0x80;
	for(i=0;i<10;i++)
 	{
	    if(HAL_I2C_Mem_Write(&I2C1Handle,SD2405APIWRITEADDRESS,0x10,I2C_MEMADD_SIZE_8BIT,&temp,1,0xff) == HAL_OK)
		{
			break;
		}
	}
	
	temp = 0x84;
	for(i=0;i<10;i++)
 	{
	    if(HAL_I2C_Mem_Write(&I2C1Handle,SD2405APIWRITEADDRESS,0x0F,I2C_MEMADD_SIZE_8BIT,&temp,1,0xff) ==HAL_OK)
		{
			break;
		}
	} 
	
	timebuf[0] = settime->Second;
	timebuf[1] = settime->Minute;
	timebuf[2] = settime->Hour;
	timebuf[3] = settime->Week;
	timebuf[4] = settime->Day;
	timebuf[5] = settime->Month;
	timebuf[6] = settime->Year;	

	for(i=0;i<10;i++)
	{
		if(HAL_I2C_Mem_Write(&I2C1Handle,SD2405APIWRITEADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,timebuf,7,0xff) == HAL_OK)
 		{
 			break;
 		}    
	}
	timebuf[0] = 0x00;
	timebuf[1] = 0x00;
	for(i=0;i<10;i++)
	{
	    if(HAL_I2C_Mem_Write(&I2C1Handle,SD2405APIWRITEADDRESS,0x0F,I2C_MEMADD_SIZE_8BIT,timebuf,2,0xff) == HAL_OK)
	    {
 			break;
 		} 
	}		
}


void Read_Time( RTC_SD2405_HandleTypeDef *readtime)
{
	uint8_t i = 0;
	uint8_t timebuf[8];	
	
	for(i=0;i<10;i++)
	{
		if(HAL_I2C_Mem_Read(&I2C1Handle,SD2405APIREADADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,timebuf,8,0xff) == HAL_OK)
		{
			break;
		}
    }
	readtime->Second = timebuf[0];	
	readtime->Minute = timebuf[1];
	readtime->Hour = timebuf[2];
	readtime->Week = timebuf[3];
	readtime->Day = timebuf[4];	
	readtime->Month = timebuf[5];
	readtime->Year = timebuf[6];
	
}


// void RTC_ReadData(RTC_SD2405_HandleTypeDef *readtime)
// {
//         uint16_t i;
// 	uint8_t temp;
// 	uint8_t timebuf[7];
// 		uint8_t MemAddress;
// 	    MemAddress = 0;
//         for(i=0;i<7;i++)
//         {
//                 MemAddress+=i;
//                 //HAL_I2C_Mem_Read(&hi2c1, 0xa0, MemAddress,I2C_MEMADD_SIZE_8BIT, &pData[i], 1, 200);
// 			    HAL_I2C_Mem_Read(&I2C1Handle,0x65,MemAddress,1,&temp,1,0xff);
// 			    timebuf[i]=temp;
//                 HAL_Delay(2);
//         }
// }

