#include "stm32f2xx_hal.h"

/**************************************
*  IO口 接线
*  CS     PG0
*  SCLK   PG6
*  SDO    PD11
*  SI     PG8
*  WP     PF15
*  RST    PB0
*  RDY    PB5
**************************************/

#define READY 1
#define BUSY  0

#define  CS_OUT_HIGH     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0,  GPIO_PIN_SET);
#define  CS_OUT_LOW      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_0,  GPIO_PIN_RESET);
#define  SCLK_OUT_HIGH   HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6,  GPIO_PIN_SET);
#define  SCLK_OUT_LOW    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6,  GPIO_PIN_RESET);
#define  SI_OUT_HIGH     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8,  GPIO_PIN_SET);
#define  SI_OUT_LOW      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8,  GPIO_PIN_RESET);
#define  RST_OUT_HIGH    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,  GPIO_PIN_SET);
#define  RST_OUT_LOW     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,  GPIO_PIN_RESET);
#define  WP_OUT_HIGH     HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15,  GPIO_PIN_SET);


#define  SO_IN    HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11)
#define  RDY_IN   HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)

extern GPIO_InitTypeDef  GPIO_InitStruct;   //GPIO结构体


uint8_t buffer[2],RecData;  
uint8_t x1;
uint8_t p[2]={0x66,0xAA};
/**************************************************
*  SPI的IO口参数配置
**************************************************/
void CS_SetOutput() //PG0
{
	__GPIOG_CLK_ENABLE();
	GPIO_InitStruct.Pin = (GPIO_PIN_0);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

void SCLK_SetOutput()     //PG6
{
  __GPIOG_CLK_ENABLE();
	GPIO_InitStruct.Pin = (GPIO_PIN_6);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}


void SO_SetOutput()   //PD11
{
	__GPIOD_CLK_ENABLE();
	GPIO_InitStruct.Pin = (GPIO_PIN_11);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void SO_SetInput()   //PD11
{
  __GPIOD_CLK_ENABLE();
	GPIO_InitStruct.Pin = (GPIO_PIN_11);
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
void SI_SetOutput()   //PG8
{
	__GPIOG_CLK_ENABLE();
	GPIO_InitStruct.Pin = (GPIO_PIN_8);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

void SI_SetInput()   //PG8
{
	__GPIOG_CLK_ENABLE();
	GPIO_InitStruct.Pin = (GPIO_PIN_8);
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);	
}

void RST_SetOutput()   //PB0
{
	__GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = (GPIO_PIN_0);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void RDY_SetInput()   //PB5
{
	__GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = (GPIO_PIN_5);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
}

void WP_SetOutput()    //PF15
{
	__GPIOF_CLK_ENABLE();
	GPIO_InitStruct.Pin = (GPIO_PIN_15);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}
/*************************************************
*  名称：SPI_Init(void)
*  功能：spi初始化
***************************************************/
void SPI_Config(void)
{
	CS_SetOutput();
    SCLK_SetOutput();
	SO_SetInput();
	SI_SetOutput();
	RST_SetOutput();
	RDY_SetInput();
	WP_SetOutput();
}

void SPI_Init(void)
{	
	WP_OUT_HIGH;
    //ERST=1;
	RST_OUT_HIGH;
	//CS =1;	  
	CS_OUT_HIGH;
	//SCK = 1;
	SCLK_OUT_HIGH;
	//SI  = 1;
	SI_OUT_HIGH;
	//SO  = 1;
	//SO_OUT_HIGH;
}  

/*************************************************
*功能:写数据
*输入参数:要发送的命令或数据
*返回参数:无
*************************************************/
void SPI_WriteByte(uint8_t SendData,uint8_t x)
{
	SCLK_OUT_LOW;
  HAL_Delay1us(1);
	while(x--)
	{	 
	 if(SendData&0x80)
	  {
			SI_OUT_HIGH;
	  }
	 else
	  {
			SI_OUT_LOW;
	  }
		HAL_Delay1us(1);
		SCLK_OUT_HIGH;
		HAL_Delay1us(1);
	 	SendData<<=1;	
		SCLK_OUT_LOW;
	 }
}

/*************************************************
*函数功能:spi读一个字节
*输入参数:无
*返回参数:读得的一字节数据
*************************************************/
uint8_t SPI_ReadByte(void)
{
	uint8_t y;
	uint8_t RecData = 0;  
	for(y=0;y<8;y++)
	{		
		
		  RecData<<=1;
		  HAL_Delay1us(1);
		  SCLK_OUT_HIGH;
		  HAL_Delay1us(1);
		  SCLK_OUT_LOW;
		  HAL_Delay1us(1);
		  if(SO_IN)
		  {
			RecData += 1;
		  }
	}
	return (RecData);
}
/***********************************************
* 函数功能：指定页擦除
* 输入参数：需要擦除的页
* 输出参数：无
************************************************/   
void SPI_PageErase(uint16_t Page)   
{   
    uint8_t i;
    if(Page>8191)
				return;   
    CS_OUT_LOW;
		HAL_Delay1us(10); 									  
    SPI_WriteByte(0x81,8); 					  //指令码
		SPI_WriteByte(0x00,1);					 //一个预留位
    SPI_WriteByte(Page>>6,7); 		        //写入13位页地址的高7位
    SPI_WriteByte((uint8_t)Page,6);		   //写入13位页地址的低6位
    for(i=0;i<10;i++)
		{	
			SCLK_OUT_LOW;
			HAL_Delay1us(100);
			SCLK_OUT_HIGH;
			HAL_Delay1us(100);
		}
    CS_OUT_HIGH;   
}   

/*******************************************************************
* 函数功能：写缓冲区1
* 输入参数：写入缓冲区起始地址，要写入的数据
* 输出参数：无
*******************************************************************/
 void SPI_WriteBuff1(uint16_t startaddr,uint8_t *p,uint16_t length)   
{ 
  uint16_t i;
  CS_OUT_LOW;
  HAL_Delay1us(10);
  SCLK_OUT_LOW;
  HAL_Delay1us(100);
  SPI_WriteByte(0x84,8);                 //写缓冲区1
  for(i=0;i<14;i++)
  {	  
	  SCLK_OUT_LOW;
	  HAL_Delay1us(20);
	  SCLK_OUT_HIGH;
	  HAL_Delay1us(20);
  }
  SPI_WriteByte(startaddr>>8,2);		 //写入10位缓冲区地址的高两位
  SPI_WriteByte((uint8_t)startaddr,8);   		 //写入10位缓冲区地址的低八位
  for(i=0;i<length;i++)
  {
   SPI_WriteByte(p[i],8);		//写数据到buff1
  }
  CS_OUT_HIGH;
  HAL_Delay1us(100);  
}


/*******************************************************************
* 函数功能：读缓冲区1
* 输入参数：要读缓冲区起始地址，要读的长度
* 输出参数：读到的数据
*******************************************************************/
 void SPI_ReadBuff1(uint16_t startaddr,uint8_t *p,uint16_t length)   
{ 
  uint16_t i;
  CS_OUT_LOW;
  HAL_Delay1us(10);
  SCLK_OUT_LOW;
  HAL_Delay1us(100);
  SPI_WriteByte(0xD4,8);                 //指令码 D4或者D1
  SPI_WriteByte(0x00,8);                 //8位无关位
  SPI_WriteByte(0x00,6);                 //6位无关位
  SPI_WriteByte(startaddr>>8,2);         //写入10位缓冲区地址的高两位
  SPI_WriteByte((uint8_t)startaddr,8);   //写入10位缓冲区地址的低八位
  SPI_WriteByte(0x00,8);                 //一个无关字节
  for(i=0;i<length;i++)
  {
   	  p[i] = SPI_ReadByte();		//写数据到buff1
  }
  CS_OUT_HIGH;
  HAL_Delay1us(100);  
}

/************************************************************
* 函数功能：通过buffer1写入flash
* 输入参数：需要写入的页地址，要写入的数据
* 输出参数：无
************************************************************/
void SPI_BuffertoMainMemoryPage(uint16_t Page,uint8_t *p,uint16_t length)   
{   
    uint16_t i = 0;  	
    if(Page>8191)
			return;   
    CS_OUT_LOW;  
	  HAL_Delay1us(1);
    SPI_WriteByte(0x88,8);     //指令码
	  SPI_WriteByte(0x00,1);		 //写入一个预留位
    SPI_WriteByte(Page>>6,7); 	//写入13位页地址的高7位
    SPI_WriteByte((uint8_t)Page,6);		//写入13位页地址的低6位
		for(i=0;i<10;i++)
		{	  
			SCLK_OUT_LOW;
			HAL_Delay1us(1);
			SCLK_OUT_HIGH;
			HAL_Delay1us(1);
		}
		for(i=0;i<length;i++)
		{ 
			 SPI_WriteByte(p[i],8); 
		}								//数据写入MainMemoryPage
    CS_OUT_HIGH; 
    HAL_Delay1us(1);   
}  

/**********************************************************
* 函数功能：从存储空间读到外部缓冲区
* 输入参数：数据所在的页地址Page，字节起始地址startaddr,读取数据的长度length
* 输出参数：读出的数据p
***********************************************************/
void SPI_MainMemoryPageRead(uint16_t Page,uint16_t startaddr,uint8_t *p ,uint16_t length)   
{   
    uint16_t i = 0;      
    if(Page<=8191) 
		{  
			CS_OUT_LOW;
		  HAL_Delay1us(1);
	    SPI_WriteByte(0xD2,8);          //主存储器页读
		 	SPI_WriteByte(0x00,1);					//一个预留位
			SPI_WriteByte(Page>>6,7); 	  //写入13位页地址的高7位
			SPI_WriteByte((uint8_t)Page,6);	  	  //写入13位页地址的低6位
			SPI_WriteByte(startaddr>>8,2); //写入10位首地址的高两位
			SPI_WriteByte((uint8_t)startaddr,8);    //写入10位首地址的低八位
			for(i=0;i<32;i++)		 //32个无关位
			{	
				SCLK_OUT_LOW;
				HAL_Delay1us(1);		   
				SCLK_OUT_HIGH;
				HAL_Delay1us(1);
			}   
			for(i=0;i<length;i++)
			{
					p[i] = SPI_ReadByte(); 				//数据读出放入buffer[c]
			}  
			CS_OUT_HIGH;
		  HAL_Delay1us(1);  
		}   
} 

/**********************************************************
* 函数功能: 外部缓冲区直接写到存储器
* 输入参数：数据所在的页地址，字节起始地址，数据长度length,数据缓冲区
* 输出参数：无
***********************************************************/
void SPI_MainMemoryPageWrite(uint16_t Page,uint16_t startaddr,uint8_t *p ,uint16_t length)   
{   
    uint16_t i = 0;   
       
    if(Page<=8191) 
    {  
			CS_OUT_LOW;
			HAL_Delay1us(1);
			SPI_WriteByte(0x82,8);
			SPI_WriteByte(0x00,2);					//一个预留位 2个无关位
			SPI_WriteByte(Page>>6,6); 	            //写入12位页地址的高6位
			SPI_WriteByte((uint8_t)Page,6);	  	    //写入12位页地址的低6位
			SPI_WriteByte(startaddr>>8,2);          //写入10位首地址的高两位
			SPI_WriteByte((uint8_t)startaddr,8);      //写入10位首地址的低八位
			for(i=0;i<length;i++)
			{ 
			 SPI_WriteByte(p[i],8); 
			}								//数据写入MainMemoryPage
			//CS=1;
			CS_OUT_HIGH;
			//Delay(3000);  
			HAL_Delay1us(3);
	  }   
} 
void ReadHisRec1(uint16_t recordno,uint8_t *p ,uint16_t length)   
{   
  //  uint16_t i = 0;
	  uint16_t page,byteadd ;
    if(1) 
	  {
		  page = (recordno%15000/5) +100;  
			byteadd =(uint8_t)((recordno%15000 % 5) * 100);
			SPI_MainMemoryPageRead(page,byteadd,p ,length);
//       CS_OUT_LOW;
// 	    HAL_Delay1us(3);
// 	    SPI_WriteByte(0xD2,8);
//       SPI_WriteByte((uint8_t)(page>>6),8); 	  //写入13位页地址的高7位
//       SPI_WriteByte((uint8_t)(page<<2),8);	  	  //写入13位页地址的低6位
//       SPI_WriteByte(0,8); //写入10位首地址的高两位
// 			for(i=0;i<32;i++)		 //32个无关位
// 			{	
// 				SCLK_OUT_LOW;
// 				HAL_Delay1us(1);		   
// 				SCLK_OUT_HIGH;
// 				HAL_Delay1us(1);
// 			}   
// 			for(i=0;i<length;i++)
// 			{
// 			 p[i] = SPI_ReadByte(); 				//数据读出放入buffer[c]
// 			}  
// 			CS_OUT_HIGH;
// 	    HAL_Delay1us(3);
	}   
}  

void ReadHisRec(uint16_t recordno,uint8_t *p,uint16_t length)
{ 
	uint8_t i;
  uint16_t page,byteadd;
	uint8_t TempBuffer[500];
	page = (recordno%15000/5) +100;  
	byteadd =(uint16_t)((recordno % 5) * 100);
	SPI_MainMemoryPageRead(page,0,TempBuffer ,500);
	for(i=0;i<length;i++)
	{
	   p[i] = TempBuffer[byteadd+i];
	}
}

void WriteHisRec(uint16_t recordno,uint8_t *p ,uint16_t length)   
{   
    uint8_t i = 0;   
		uint16_t page,byteadd ;
	  uint8_t TempBuffer[600];
		page = (uint16_t)(recordno%15000 /5) +100;
		byteadd =(uint16_t)((recordno%15000 % 5) * 100);
	  SPI_MainMemoryPageRead(page,0,TempBuffer ,500);
	  for(i=0;i<length;i++)
	  {
			TempBuffer[byteadd+i] = p[i] ;
	  }
		SPI_MainMemoryPageWrite(page,0,TempBuffer,500);    
}  
// void ReadRealRec(uint pageadd,ubyte *p)   
// {   
//     uint i = 0;
// //	uint page;//,byteadd ;
//   	if((pageadd>2999)&&(pageadd<3100))
// 	{
//         CS = 0;
// 	    Delay(10);
// 	    SPI_writebyte(0xD2,8);
//         SPI_writebyte((ubyte)(pageadd>>6),8); 	  //写入13位页地址的高7位
//         SPI_writebyte((ubyte)(pageadd<<2),8);	  	  //写入13位页地址的低6位
//         SPI_writebyte(0,8); //写入10位首地址的高两位
//     for(i=0;i<32;i++)		 //32个无关位
// 	{	
// 		SCK = 0;
// 		Delay(2);		   
// 		SCK = 1;
// 		Delay(2);
// 	}   
//     for(i=0;i<200;i++)
// 	{
// 	 p[i] = SPI_ReadByte(); 				//数据读出放入buffer[c]
// 	 }  
//      CS=1;
// 	 Delay(100);  
// 	}   
// }  
/*********************************************************
 *功能：将一个两位数数转换为相应的ASC码
 *入口参数：需要转换的数据
 *出口参数：转换以后的数
 ********************************************************/
uint16_t spichange( uint8_t ddata1)
{
 	uint8_t temp;
	uint16_t temp16;

	temp =  ddata1 >> 4;
	if(temp<=9)
	{
	 	temp = temp+0x30;
	}
	else
	{
		temp = temp+0x37;
	}
	temp16 = temp << 8;
	temp =  ddata1 & 0x0f;
	if(temp <= 9)
	{
	 	temp = temp+0x30;
	}
	else
	{
		temp = temp+0x37;
	}
	 
	temp16 = temp16 | temp;
	return(temp16);

}


 
// void WriteRealRec(uint pageadd,ubyte *p )
// {
// ubyte i;
//   	if((pageadd>2999)&&(pageadd<3100))
// 	{
// 	    CS = 0;
// 		Delay(10);
// 		SPI_writebyte(0x82,8);
// 	    SPI_writebyte((ubyte)(pageadd>>6),8); 	        //写入12位页地址的高7位
// 	    SPI_writebyte((uchar)(pageadd<<2),8);	  	    //写入12位页地址的低6位
// 	    SPI_writebyte(0,8);      //写入10位首地址的低八位
//     	for(i=0;i<200;i++)
// 		{ 
// 	   		SPI_writebyte(p[i],8); 
// 	 	}								//数据写入MainMemoryPage
//      	CS=1;
// 	 	Delay(9000);  
// ////////////////////////////////////////////////////////////////////
// 	    CS = 0;
// 		Delay(10);
// 		pageadd++;
// 		if(pageadd>=3100)
// 		{
// 		  pageadd = 3000;
// 		}
// 		SPI_writebyte(0x82,8);
// 	    SPI_writebyte((ubyte)(pageadd>>6),8); 	        //写入12位页地址的高7位
// 	    SPI_writebyte((uchar)(pageadd<<2),8);	  	    //写入12位页地址的低6位
// 	    SPI_writebyte(0,8);      //写入10位首地址的低八位
//     	for(i=0;i<200;i++)
// 		{ 
// 	   		SPI_writebyte(0xff,8); 
// 	 	}								//数据写入MainMemoryPage
//      	CS=1;
// 	 	Delay(9000);   	 
// 	}
// }

// void WriteStatuRec(ubyte *p )
// { 
//   ubyte i;
//   uword pageadd = 3200;
// 	    CS = 0;
// 		Delay(10);
// 		SPI_writebyte(0x82,8);
// 	    SPI_writebyte((ubyte)(pageadd>>6),8); 	        //写入12位页地址的高7位
// 	    SPI_writebyte((uchar)(pageadd<<2),8);	  	    //写入12位页地址的低6位
// 	    SPI_writebyte(0,8);      //写入10位首地址的低八位
//     	for(i=0;i<10;i++)
// 		{ 
// 	   		SPI_writebyte(p[i],8); 
// 	 	}								//数据写入MainMemoryPage
//      	CS=1;
// 	 	Delay(9000);  
// }

// void ReadStatuRec(ubyte *p)   
// {   
//     uint i = 0;
//     uword pageadd = 3200;
//         CS = 0;
// 	    Delay(10);
// 	    SPI_writebyte(0xD2,8);
//         SPI_writebyte((ubyte)(pageadd>>6),8); 	  //写入13位页地址的高7位
//         SPI_writebyte((ubyte)(pageadd<<2),8);	  	  //写入13位页地址的低6位
//         SPI_writebyte(0,8); //写入10位首地址的高两位
//     for(i=0;i<32;i++)		 //32个无关位
// 	{	
// 		SCK = 0;
// 		Delay(2);		   
// 		SCK = 1;
// 		Delay(2);
// 	}   
//     for(i=0;i<10;i++)
// 	{
// 	 p[i] = SPI_ReadByte(); 				//数据读出放入buffer[c]
// 	}  
//      CS=1;
// 	 Delay(100);  
// }  


// /*
// void WriteRealRec(uint pageadd,ubyte *p )
// {
// uint pageadd;
// 	pageadd = 3200;

//     CS = 0;
// 	Delay(10);
// 	SPI_writebyte(0x82,8);
//     SPI_writebyte((ubyte)(pageadd>>6),8); 	        //写入12位页地址的高7位
//     SPI_writebyte((uchar)(pageadd<<2),8);	  	    //写入12位页地址的低6位
//     SPI_writebyte(0,8);      //写入10位首地址的低八位
//    	for(i=0;i<100;i++)
// 	{ 
//    		SPI_writebyte(p[i],8); 
//  	}								//数据写入MainMemoryPage
//      	CS=1;
// 	 	Delay(9000);  

// }	*/

