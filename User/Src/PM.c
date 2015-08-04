#include "stm32f2xx_hal.h"
extern UART_HandleTypeDef UartHandle5;
extern uint8_t pData[1];

 void GetVoltage(uint8_t address);
static void GetCurrent(uint8_t address);
static void GetEnergy(uint8_t address);
static void GetFrequency(uint8_t address);//	 频率
static void GetRectivePower(uint8_t address);	 //总无功功率
static void GetApparentPower(uint8_t address);//总视在功率

static void CON485_Init(void);       //485控制线初始化
static void DCPM(uint8_t address);//直流电表
static void InsulationTZB(uint8_t address);//绝缘检测仪
uint16_t CalcCrc(uint8_t *chData,unsigned short uNo);



//uint8_t q1;
void PM_Init(void)
{
	//Tim3Flag=0x00;
	Tim4Flag=0x00;
	Flag485=0x00;
	RxFlag485=0x00;
	TxFlag485=0x01;
	PMCount485=0x00;
	Fun485Flag=0;
	MX_UART5_UART_Init();
	CON485_Init();
	
}
void GetVoltage(uint8_t address)					 //读电压AC
{
	uint8_t i;
	uint16_t crc;
	//uword crc;
	PMsendBuf[0]=address;
	PMsendBuf[1]=0x03;
	PMsendBuf[2]=0x00;
	//PMsendBuf[3]=0x32;
	PMsendBuf[3]=ACVoltageRegister;
	PMsendBuf[4]=0x00;
	PMsendBuf[5]=0x03;
	//PMsendBuf[6]=0xa4;
  // PMsendBuf[7]=0x04;
	crc=CalcCrc(PMsendBuf,6);
	PMsendBuf[6]=(uint8_t)(crc&0xff);	  //crc低字节
	PMsendBuf[7]=(uint8_t)((crc>>8)&0xff);  //crc高字节
	
	
	
	UART5RxBuffCount=0;
	for(i=0;i<24;i++) UART5RxBuff[i]=0x00;
	
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_UART_Transmit(&UartHandle5, PMsendBuf, 8, 0xFF);
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);

  //   HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
  // 	HAL_UART_Transmit(&huart1, PMsendBuf, 8, 0xFFFF);
  //   HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	
// 	HAL_Delay(14);
// 	

// 	for(i=0;i<20;i++)
// 	{
// 		if((UART3RxBuff[i] == address) && (UART3RxBuff[i+1] == 0x03))
// 		{
// 			VoltageA[0] = UART3RxBuff[i+3]; 
// 			VoltageA[1] = UART3RxBuff[i+4];
// 			VoltageB[0] = UART3RxBuff[i+5]; 
// 			VoltageB[1] = UART3RxBuff[i+6];
// 			VoltageC[0] = UART3RxBuff[i+7]; 
// 			VoltageC[1] = UART3RxBuff[i+8];
// 			
// 			//break;
// 		}
// 	}
}



 

//读相电流AC
void GetCurrent(uint8_t address)    
{
	
	uint8_t i;
	uint16_t crc;

	PMsendBuf[0]=address;
	PMsendBuf[1]=0x03;
	PMsendBuf[2]=0x00;
	//PMsendBuf[3]=0x38;
	PMsendBuf[3]=ACCurrentRegister;
	PMsendBuf[4]=0x00;
	PMsendBuf[5]=0x03;
	//PMsendBuf[6]=0x84;
	//PMsendBuf[7]=0x06;
	crc=CalcCrc(PMsendBuf,6);
	PMsendBuf[6]=(uint8_t)(crc&0xff);	  //crc低字节
	PMsendBuf[7]=(uint8_t)((crc>>8)&0xff);  //crc高字节
	
	
	UART5RxBuffCount=0;
	for(i=0;i<24;i++) UART5RxBuff[i]=0x00;
	
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_UART_Transmit(&UartHandle5, PMsendBuf, 8, 0xFF);
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	
// 	UART3RxBuffCount=0;
// 	for(i=0;i<24;i++) UART3RxBuff[i]=0x00;
// 	
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart3, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
//   HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart1, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	
// 	HAL_Delay(10);
// 	
//  
// 	
// 	for(i=0;i<20;i++)
// 	{
// 		if((UART3RxBuff[i] == address) && (UART3RxBuff[i+1] == 0x03))
// 		{
// 			CurrentA[0] = UART3RxBuff[i+3]; 
// 			CurrentA[1] = UART3RxBuff[i+4];
// 			CurrentB[0] = UART3RxBuff[i+5]; 
// 			CurrentB[1] = UART3RxBuff[i+6];
// 			CurrentC[0] = UART3RxBuff[i+7]; 
// 			CurrentC[1] = UART3RxBuff[i+8];
// 			for(i=0;i<20;i++) UART3RxBuff[i]=0x00;
// 			break;
// 		}
// 	}
}




//读电能
void GetEnergy(uint8_t address)
{
	uint8_t i;
	uint16_t crc;
  PMsendBuf[0]=address;
	PMsendBuf[1]=0x03;
	PMsendBuf[2]=0x00;
	//PMsendBuf[3]=0x0a;
	PMsendBuf[3]=ACEnergyRegister;
	PMsendBuf[4]=0x00;
	PMsendBuf[5]=0x02;
	//PMsendBuf[6]=0xe4;
	//PMsendBuf[7]=0x09;
	crc=CalcCrc(PMsendBuf,6);
	PMsendBuf[6]=(uint8_t)(crc&0xff);	  //crc低字节
	PMsendBuf[7]=(uint8_t)((crc>>8)&0xff);  //crc高字节

	UART5RxBuffCount=0;
	for(i=0;i<24;i++) UART5RxBuff[i]=0x00;
	
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_UART_Transmit(&UartHandle5, PMsendBuf, 8, 0xFFFF);
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	
//   UART3RxBuffCount=0;
// 	for(i=0;i<24;i++) UART3RxBuff[i]=0x00;
// 	
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart3, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
	
// 	HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart1, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	
// 	HAL_Delay(50);
// 	
// 	for(i=0;i<20;i++)
// 	{
// 		if((UART3RxBuff[i] == address) && (UART3RxBuff[i+1] == 0x03))
// 		{
// 			ACEnergy[0] = UART3RxBuff[i+3]; 
// 			ACEnergy[1] = UART3RxBuff[i+4];
// 			ACEnergy[2] = UART3RxBuff[i+5]; 
// 			ACEnergy[3] = UART3RxBuff[i+6];
// 			
// 			for(i=0;i<20;i++) UART3RxBuff[i]=0x00;
// 			break;
// 		}
// 	}
 }
 
 void GetFrequency(uint8_t address)//	 频率
 {
	uint8_t i;
	uint16_t crc;
  PMsendBuf[0]=address;
	PMsendBuf[1]=0x03;
	PMsendBuf[2]=0x00;
	//PMsendBuf[3]=0x4b;
	PMsendBuf[3]=ACFrequencyRegister;
	PMsendBuf[4]=0x00;
	PMsendBuf[5]=0x01;
	//PMsendBuf[6]=0xF4;
	//PMsendBuf[7]=0x1C;
	crc=CalcCrc(PMsendBuf,6);
	PMsendBuf[6]=(uint8_t)(crc&0xff);	  //crc低字节
	PMsendBuf[7]=(uint8_t)((crc>>8)&0xff);  //crc高字节

	UART5RxBuffCount=0;
	for(i=0;i<24;i++) UART5RxBuff[i]=0x00;
	
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_UART_Transmit(&UartHandle5, PMsendBuf, 8, 0xFF);
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	 
//   UART3RxBuffCount=0;
// 	for(i=0;i<24;i++) UART3RxBuff[i]=0x00;
// 	
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart3, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
	
// 	HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart1, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	
// 	HAL_Delay(50);
// 	
// 	for(i=0;i<20;i++)
// 	{
// 		if((UART3RxBuff[i] == address) && (UART3RxBuff[i+1] == 0x03))
// 		{
// 			Frequency[0] = UART3RxBuff[i+3]; 
// 			Frequency[1] = UART3RxBuff[i+4];
// 			
// 			
// 			for(i=0;i<20;i++) UART3RxBuff[i]=0x00;
// 			break;
// 		}
// 	}
	
}	
	
	
	
void GetRectivePower(uint8_t address) //总无功功率
{
		
	uint8_t i;
	uint16_t crc;
  PMsendBuf[0]=address;
	PMsendBuf[1]=0x03;
	PMsendBuf[2]=0x00;
	//PMsendBuf[3]=0x42;
	PMsendBuf[3]=ACRectivePowerRegister;
	PMsendBuf[4]=0x00;
	PMsendBuf[5]=0x01;
	//PMsendBuf[6]=0x24;
	//PMsendBuf[7]=0x1e;
	crc=CalcCrc(PMsendBuf,6);
	PMsendBuf[6]=(uint8_t)(crc&0xff);	  //crc低字节
	PMsendBuf[7]=(uint8_t)((crc>>8)&0xff);  //crc高字节

	UART5RxBuffCount=0;
	for(i=0;i<24;i++) UART5RxBuff[i]=0x00;
	
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_UART_Transmit(&UartHandle5, PMsendBuf, 8, 0xFF);
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	
//   UART3RxBuffCount=0;
// 	for(i=0;i<24;i++) UART3RxBuff[i]=0x00;
// 	
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart3, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
	
// 	HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart1, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	
// 	HAL_Delay(50);
// 	
// 	for(i=0;i<20;i++)
// 	{
// 		if((UART3RxBuff[i] == address) && (UART3RxBuff[i+1] == 0x03))
// 		{
// 			RectivePower[0] = UART3RxBuff[i+3]; 
// 			RectivePower[1] = UART3RxBuff[i+4];
// 		
// 			for(i=0;i<20;i++) UART3RxBuff[i]=0x00;
// 			break;
// 		}
// 	
// 	}
	}
	
	
	
	
	void GetApparentPower(uint8_t address)//总视在功率
	{
		
	uint8_t i;
	uint16_t crc;
  PMsendBuf[0]=address;
	PMsendBuf[1]=0x03;
	PMsendBuf[2]=0x00;
	//PMsendBuf[3]=0x46;
	PMsendBuf[3]=ACApparentPowerRegister;	
	PMsendBuf[4]=0x00;
	PMsendBuf[5]=0x01;
	//PMsendBuf[6]=0x65;
	//PMsendBuf[7]=0xDF;
	crc=CalcCrc(PMsendBuf,6);
	PMsendBuf[6]=(uint8_t)(crc&0xff);	  //crc低字节
	PMsendBuf[7]=(uint8_t)((crc>>8)&0xff);  //crc高字节

	UART5RxBuffCount=0;
	for(i=0;i<24;i++) UART5RxBuff[i]=0x00;
	
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_UART_Transmit(&UartHandle5, PMsendBuf, 8, 0xFF);
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		
//   UART3RxBuffCount=0;
// 	for(i=0;i<24;i++) UART3RxBuff[i]=0x00;
// 	
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart3, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
 	
// 	HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart1, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	
// 	HAL_Delay(50);
// 	
// 	for(i=0;i<20;i++)
// 	{
// 		if((UART3RxBuff[i] == address) && (UART3RxBuff[i+1] == 0x03))
// 		{
// 			ApparentPower[0] = UART3RxBuff[i+3]; 
// 			ApparentPower[1] = UART3RxBuff[i+4];
// 		
// 			for(i=0;i<20;i++) UART3RxBuff[i]=0x00;
// 			break;
// 		}
// 	
// 	}	
// 		
		
	}
	
	
	
	
	
 
 
 
	
	
	//直流电表
	void DCPM(uint8_t address)
	{
/*苏州兰创的直流电表*/	
// 	uint8_t i;
// 	uint16_t crc;
// 	//uword crc;
// 	PMsendBuf[0]=address;
// 	PMsendBuf[1]=0x03;
// 	PMsendBuf[2]=0x08;
// 	PMsendBuf[3]=0x00;
// 	PMsendBuf[4]=0x00;
// 	PMsendBuf[5]=0x07;
// 	//PMsendBuf[6]=0x06;
//  // PMsendBuf[7]=0x68;
// 	crc=CalcCrc(PMsendBuf,6);
// 	PMsendBuf[6]=(uint8_t)(crc&0xff);	  //crc低字节
// 	PMsendBuf[7]=(uint8_t)((crc>>8)&0xff);  //crc高字节
// 	
// 	
// 	UART5RxBuffCount=0;
// 	for(i=0;i<24;i++) UART5RxBuff[i]=0x00;
// 	
//   HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&UartHandle5, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
  
/*环岛的直流电表。。读电能*/
  uint8_t i;
	uint16_t crc;
  PMsendBuf[0]=address;
	PMsendBuf[1]=0x03;
	PMsendBuf[2]=0x00;
	PMsendBuf[3]=0x0e;
	PMsendBuf[4]=0x00;
	PMsendBuf[5]=0x02;
	//PMsendBuf[6]=0xe4;
	//PMsendBuf[7]=0x09;
	crc=CalcCrc(PMsendBuf,6);
	PMsendBuf[6]=(uint8_t)(crc&0xff);	  //crc低字节
	PMsendBuf[7]=(uint8_t)((crc>>8)&0xff);  //crc高字节

	UART5RxBuffCount=0;
	for(i=0;i<24;i++) UART5RxBuff[i]=0x00;
	
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_UART_Transmit(&UartHandle5, PMsendBuf, 8, 0xFFFF);
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			
	
}
	
	
	
	
	void InsulationTZB(uint8_t address)//绝缘检测仪
	{
		
	uint8_t i;
	uint16_t crc;
  PMsendBuf[0]=address;
	PMsendBuf[1]=0x01;
	PMsendBuf[2]=0x02;
	PMsendBuf[3]=0x03;
	PMsendBuf[4]=0x04;
	PMsendBuf[5]=0x05;
	//PMsendBuf[6]=0x0c;
	//PMsendBuf[7]=0x30;
	crc=CalcCrc(PMsendBuf,6);
	PMsendBuf[6]=(uint8_t)(crc&0xff);	  //crc低字节
	PMsendBuf[7]=(uint8_t)((crc>>8)&0xff);  //crc高字节

		
	UART5RxBuffCount=0;
	for(i=0;i<24;i++) UART5RxBuff[i]=0x00;
	
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_UART_Transmit(&UartHandle5, PMsendBuf, 8, 0xFF);
  HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		
		
	
//   UART3RxBuffCount=0;
// 	for(i=0;i<24;i++) UART3RxBuff[i]=0x00;
// 	
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart3, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
	
// 	HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
// 	HAL_UART_Transmit(&huart1, PMsendBuf, 8, 0xFFFF);
//   HAL_GPIO_WritePin( GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	
// 	HAL_Delay(50);
// 	
// 	for(i=0;i<20;i++)
// 	{
// 		if((UART3RxBuff[i] == address) )
// 		{
// 			InsulationRes[0] = UART3RxBuff[i+4]; 
// 			InsulationRes[1] = UART3RxBuff[i+5];
// 		  
// 			for(i=0;i<20;i++) UART3RxBuff[i]=0x00;
// 			break;
// 		}
// 	
// 	}	
				
	}
	
	
	
	
	
	
//   void MX_USART3_UART_Init(void)
// 	{
// 		
// 		 /*##-1- Configure the UART peripheral ######################################*/
//   /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
//   /* UART1 configured as follow:
//       - Word Length = 8 Bits
//       - Stop Bit    = One Stop bit
//       - Parity      = ODD parity
//       - BaudRate    = 9600 baud
//       - Hardware flow control disabled (RTS and CTS signals) */
//   huart3.Instance        = USART3;
//   huart3.Init.BaudRate   = 9600;
//   huart3.Init.WordLength = UART_WORDLENGTH_8B;
//   huart3.Init.StopBits   = UART_STOPBITS_1;
//   huart3.Init.Parity     = UART_PARITY_NONE;
//   huart3.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
//   huart3.Init.Mode       = UART_MODE_TX_RX;

//   if (HAL_UART_Init(&huart3) != HAL_OK)
//   {
//     /* Initialization Error */

//   }
//  //__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
//   
//   if(HAL_UART_Receive_IT(&huart3, (uint8_t *)&aRxBuffer, 1) != HAL_OK)
//   {
//    // Error_Handler();
//   }
//   UART3RxBuffCount = 0;
// 		
// 		
// 		
// 	}
	
	
	

	
	void MX_UART5_UART_Init(void)
	{
		
		 /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle5.Instance        = UART5;
  UartHandle5.Init.BaudRate   = 9600;
  UartHandle5.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle5.Init.StopBits   = UART_STOPBITS_1;
  UartHandle5.Init.Parity     = UART_PARITY_NONE;
  UartHandle5.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle5.Init.Mode       = UART_MODE_TX_RX;

  if (HAL_UART_Init(&UartHandle5) != HAL_OK)
  {
    /* Initialization Error */

  }
 //__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
  
  if(HAL_UART_Receive_IT(&UartHandle5, (uint8_t *)pData, 1) != HAL_OK)
  {
   // Error_Handler();
  }
  UART5RxBuffCount = 0;
		
		
		
	}
	
	
	
	
	
	//485控制线
void CON485_Init(void)
	{
		
	GPIO_InitTypeDef  GPIO_InitStruct;
	__GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = (GPIO_PIN_8);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
//   GPIO_InitTypeDef  GPIO_InitStruct;
// 	__GPIOC_CLK_ENABLE();
//   GPIO_InitStruct.Pin = (GPIO_PIN_9);
//   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//   GPIO_InitStruct.Pull = GPIO_PULLUP;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
	}
	
	
	
	
	PM485 Send_485(uint8_t address,uint8_t addressAC)
	{
		if(TxFlag485==0x01)
		{
				if(address==0x01)                                           //发送交流表指令
				{
					if(addressAC==0x01)                                       //发送三相电压指令
					{
						GetVoltage( address);
						Flag485=0x01;
						return PM485_OK;	
					}
					
					else if(addressAC==0x02)                                       
					{
						GetCurrent( address); //发送三相电流指令
						Flag485=0x02;
						return PM485_OK;	
					}
					
					else if(addressAC==0x03)                                       //发送交流电能指令
					{
						GetEnergy( address);                                       
						Flag485=0x03;
						return PM485_OK;	
					}
					
					else if(addressAC==0x04)                                       //发送交流电表频率
					{
						
						GetFrequency( address);                                       
						Flag485=0x04;
						return PM485_OK;
						
					}
					
					else if(addressAC==0x05)                                       //发送交流电表总无功功率
					{
						
						GetRectivePower( address);                                       
						Flag485=0x05;
						return PM485_OK;
						
					}
					else if(addressAC==0x06)                                       //发送交流电表总视在功率
					{
						
						GetApparentPower( address);
						Flag485=0x06;	
						return PM485_OK;
						
					}
				}
				
				else if(address==0x10)
				{
				InsulationTZB(address);
				Flag485=0x07;
				return PM485_OK;
				}
				else if(address==2)
				{
				DCPM(address);
				Flag485=0x08;
				return PM485_OK;
				}
				
	}
	
	return PM485_ERROR;	
}
	
	
	
	
	

	

	PM485 Receive_485()
	{
		uint8_t i;
		uint16_t crc;
		if(Tim4Flag==0x01)
		{		
			Tim4Flag = 0;
      if(Flag485==0x01)
			{
				
			  for(i=0;i<24;i++)
	      {
							if((UART5RxBuff[i] == 0x01) )
							{
							
									crc=CalcCrc((UART5RxBuff+i),9);
									Crc485[0]=(uint8_t)(crc&0xff);	  //crc低字节
									Crc485[1]=(uint8_t)((crc>>8)&0xff);  //crc高字节
									
									if((Crc485[0]==UART5RxBuff[i+9])&&(Crc485[1]==UART5RxBuff[i+10]))
									{

											VoltageA[0] = UART5RxBuff[i+3]; 
											VoltageA[1] = UART5RxBuff[i+4];
											VoltageB[0] = UART5RxBuff[i+5]; 
											VoltageB[1] = UART5RxBuff[i+6];
											VoltageC[0] = UART5RxBuff[i+7]; 
											VoltageC[1] = UART5RxBuff[i+8];
											
											
											Tim4Flag=0x00;
											return PM485_OK;
									}
				      
		          }
				
		    }
		  }
			
			else if(Flag485==0x02)
			{
				
				 for(i=0;i<24;i++)
					{
							if((UART5RxBuff[i] == 0x01) )
							{
							
									crc=CalcCrc((UART5RxBuff+i),9);
									Crc485[0]=(uint8_t)(crc&0xff);	  //crc低字节
									Crc485[1]=(uint8_t)((crc>>8)&0xff);  //crc高字节
									
									if((Crc485[0]==UART5RxBuff[i+9])&&(Crc485[1]==UART5RxBuff[i+10]))
									{
										 CurrentA[0] = UART5RxBuff[i+3]; 
										 CurrentA[1] = UART5RxBuff[i+4];
										 CurrentB[0] = UART5RxBuff[i+5]; 
										 CurrentB[1] = UART5RxBuff[i+6];
										 CurrentC[0] = UART5RxBuff[i+7]; 
										 CurrentC[1] = UART5RxBuff[i+8];
										 Tim4Flag=0x00;
										 return PM485_OK;
									}
							}
					
					}
				}
			
			else if(Flag485==0x03)
			{
				
				 for(i=0;i<24;i++)
				 {
							if((UART5RxBuff[i] == 0x01) )
							{
								
								  crc=CalcCrc((UART5RxBuff+i),7);
									Crc485[0]=(uint8_t)(crc&0xff);	  //crc低字节
									Crc485[1]=(uint8_t)((crc>>8)&0xff);  //crc高字节
									
									if((Crc485[0]==UART5RxBuff[i+7])&&(Crc485[1]==UART5RxBuff[i+8]))
									{
											 ACEnergy[0] = UART5RxBuff[i+3]; 
											 ACEnergy[1] = UART5RxBuff[i+4];
											 ACEnergy[2] = UART5RxBuff[i+5]; 
											 ACEnergy[3] = UART5RxBuff[i+6];
											 Tim4Flag=0x00;
											 return PM485_OK;
									}
							
							}
					
					}
		  }
			
			
			else if(Flag485==0x04)
			{
				
				 for(i=0;i<24;i++)
				 {
							if((UART5RxBuff[i] == 0x01) )
							{
							
									crc=CalcCrc((UART5RxBuff+i),5);
									Crc485[0]=(uint8_t)(crc&0xff);	  //crc低字节
									Crc485[1]=(uint8_t)((crc>>8)&0xff);  //crc高字节
									
									if((Crc485[0]==UART5RxBuff[i+5])&&(Crc485[1]==UART5RxBuff[i+6]))
									{
											 Frequency[0] = UART5RxBuff[i+3]; 
											 Frequency[1] = UART5RxBuff[i+4];
											 
											 Tim4Flag=0x00;
											 return PM485_OK;
									}
							
							}
					
				 }
		  }
			
			
			else if(Flag485==0x05)
			{
				
			 for(i=0;i<24;i++)
	     {
						if((UART5RxBuff[i] == 0x01) )
						{
						
								crc=CalcCrc((UART5RxBuff+i),5);
								Crc485[0]=(uint8_t)(crc&0xff);	  //crc低字节
								Crc485[1]=(uint8_t)((crc>>8)&0xff);  //crc高字节
								
								if((Crc485[0]==UART5RxBuff[i+5])&&(Crc485[1]==UART5RxBuff[i+6]))
								{
										 RectivePower[0] = UART5RxBuff[i+3]; 
										 RectivePower[1] = UART5RxBuff[i+4];
										 
										
										 Tim4Flag=0x00;
										 return PM485_OK;
								}
						
						}
				
		    }
		  }
			
			
			else if(Flag485==0x06)
			{
				
			 for(i=0;i<24;i++)
	     {
						if((UART5RxBuff[i] == 0x01) )
						{
						
								crc=CalcCrc((UART5RxBuff+i),5);
								Crc485[0]=(uint8_t)(crc&0xff);	  //crc低字节
								Crc485[1]=(uint8_t)((crc>>8)&0xff);  //crc高字节
								
								if((Crc485[0]==UART5RxBuff[i+5])&&(Crc485[1]==UART5RxBuff[i+6]))
								{
									 ApparentPower[0] = UART5RxBuff[i+3]; 
									 ApparentPower[1] = UART5RxBuff[i+4];
									 
									 
									 Tim4Flag=0x00;
									 return PM485_OK;
								}
						
						}
				
		    }
		  }
			
			
			
			
		
		
		else if(Flag485==0x07)
		{
			for(i=0;i<24;i++)
	    {
					if((UART5RxBuff[i] == 0x10) )
					{
					
							crc=CalcCrc((UART5RxBuff+i),8);
							Crc485[0]=(uint8_t)(crc&0xff);	  //crc低字节
							Crc485[1]=(uint8_t)((crc>>8)&0xff);  //crc高字节
							
							if((Crc485[0]==UART5RxBuff[i+8])&&(Crc485[1]==UART5RxBuff[i+9]))
							{

							InsulationRes[0] = UART5RxBuff[i+4]; 
							InsulationRes[1] = UART5RxBuff[i+5];
							InsulationRes[2] = UART5RxBuff[i+6]; 
							InsulationRes[3] = UART5RxBuff[i+7];
							Tim4Flag=0x00;
							return PM485_OK;
							}
				 }		
		  }		
		}
		
		else if(Flag485==0x08)
		{
			for(i=0;i<24;i++)
	    {
					if((UART5RxBuff[i] == 2) )
					{
					
							crc=CalcCrc((UART5RxBuff+i),17);
							Crc485[0]=(uint8_t)(crc&0xff);	  //crc低字节
							Crc485[1]=(uint8_t)((crc>>8)&0xff);  //crc高字节
							
							if((Crc485[0]==UART5RxBuff[i+17])&&(Crc485[1]==UART5RxBuff[i+18]))
							{

// 									DCEnergy [0]=UART5RxBuff[i+3];//正相电度
// 									DCEnergy [1]=UART5RxBuff[i+4];
// 									DCEnergy[2]=UART5RxBuff[i+5];
// 									DCEnergy[3]=UART5RxBuff[i+6];
// 									
// 									
// 									DCVoltage[0] = UART5RxBuff[i+11]; //直流电压测量值
// 									DCVoltage[1] = UART5RxBuff[i+12];
// 									
// 									DCCurrent[0]=UART5RxBuff[i+13];//直流电流测量值
// 									DCCurrent[1]=UART5RxBuff[i+14];

// 									Tim4Flag=0x00;
// 									return PM485_OK;
								
								
								    /*环岛的电表读电能*/
								  crc=CalcCrc((UART5RxBuff+i),7);
									Crc485[0]=(uint8_t)(crc&0xff);	  //crc低字节
									Crc485[1]=(uint8_t)((crc>>8)&0xff);  //crc高字节
									
									if((Crc485[0]==UART5RxBuff[i+7])&&(Crc485[1]==UART5RxBuff[i+8]))
									{
											 DCEnergy[0] = UART5RxBuff[i+3]; 
											 DCEnergy[1] = UART5RxBuff[i+4];
											 DCEnergy[2] = UART5RxBuff[i+5]; 
											 DCEnergy[3] = UART5RxBuff[i+6];
											 Tim4Flag=0x00;
											 return PM485_OK;
									}
							}
				 }		
		  }		
		}
		
	Tim4Flag=0x00;
	}
	return PM485_ERROR;
}

void Fun_Receive_Send(uint8_t address,uint8_t addressAC)
{
		if(Send_485(address,addressAC)==PM485_OK)
		{	
			 PMCount485=0x00;
			 RxFlag485=0x00;
			 TxFlag485=0x00;			
		} 
		if(Receive_485( )==PM485_OK)
		{
			 TxFlag485=0x01;
			 RxFlag485= 0x01;
       Fun485Flag=0;			
		}
		if ((PMCount485>=0x05)&&(RxFlag485==0x00))
		{
			PMCount485=0x00;
			TxFlag485=0x01;
		}
}


void Fun_485(void)
{
	  if(Fun485Flag==1)                 
		{
				Fun_Receive_Send(0x01,0x01);
   	}
		else if(Fun485Flag==2)
		{
				Fun_Receive_Send(0x01,0x02);
			  
   	}
	  else if(Fun485Flag==3)
		{
				Fun_Receive_Send(0x01,0x03);
			  
   	}
		else if(Fun485Flag==4)
		{
				Fun_Receive_Send(0x01,0x04);
			  
   	}
		else if(Fun485Flag==5)
		{
				Fun_Receive_Send(0x01,0x05);
			  
   	}
		else if(Fun485Flag==6)
		{
				Fun_Receive_Send(0x01,0x06);
   	}
		else if(Fun485Flag==7)
		{
				Fun_Receive_Send(0x10,0x00);
   	}
		else if(Fun485Flag==8)
		{
				Fun_Receive_Send(0x02,0x01);
   	}
	
}

	
// 	crc16校验码计算函数,计算算法：
// 1、设置crc校验寄存器的初值为0xffff;
// 2、将报文的第一个字节与crc寄存器的低字节异或，结果存入crc寄存器
// 3、判断crc的第一位是否为1，如果是1,crc右移1位后和0xa001异或，如果为0，则再移1位；
// 4、重复步骤3，直到完成8个位；
// 5、重复步骤2、3、4直到完成所有字节
// 6、返回计算结果
	uint16_t CalcCrc(uint8_t *chData,unsigned short uNo)
 {
	uint16_t crc=0xffff;
	uint16_t i,j;
	for(i=0;i<uNo;i++)
	{
	  crc^=chData[i];                   //异或
	  for(j=0;j<8;j++)
	  {
	    if(crc&1)
		{
	     crc>>=1;
		 crc^=0xA001;
		}
	    else
		 crc>>=1;
	  }
	}
	return (crc);
 }


 
 void GetACEnergy(void)
 {
	   //uint8_t i;
	   //uint16_t crc;
     PMsendBuf[0]=0x01;
	   PMsendBuf[1]=0x03;
	   PMsendBuf[2]=0x00;
	   PMsendBuf[3]=0x0a;
	   PMsendBuf[4]=0x00;
	   PMsendBuf[5]=0x02;
	   PMsendBuf[6]=0xe4;
	   PMsendBuf[7]=0x09;
     HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	   HAL_UART_Transmit(&UartHandle5, PMsendBuf, 8, 0xFFFF);
	   HAL_Delay(1);	
     HAL_GPIO_WritePin( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);	 
	 Flag485 = 0x03;
	 }



