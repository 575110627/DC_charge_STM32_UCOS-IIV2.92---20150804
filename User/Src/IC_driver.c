/******************** (C) COPYRIGHT 2013 ********************
* 文件名          : PICC.c
* 作者            : 优灵电子开发团队。
* 版本            : V2.0.1.0
* 日期            : 2013-5-10
* 描述            : 所有对卡的操作，给模块发送的命令。
* 技术支持        : QQ: 526181679
********************************************************************************/

/* 包含文件 -------------------------------------------------------------------*/
//#include "PICC.h"
#include "main.h"
/* 宏定义 ---------------------------------------------------------------------*/
/* 本文件使用的变量 -----------------------------------------------------------*/
PICC Picc;
CMD Cmd;
extern UART_HandleTypeDef   UartHandle6;
/* 本文件使用的函数声明 -------------------------------------------------------*/
void SendCommand(void);
/* 本文件函数体 ---------------------------------------------------------------*/

/*******************************************************************************
* 函数名         : ReadCard
* 描述           : 读卡号
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void ReadCard(void)
{
	Cmd.SendBuffer[0] = 03;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x10;
	SendCommand();
}

/*******************************************************************************
* 函数名         : ReadBlock
* 描述           : 读块数据
* 输入           : block-块号
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void ReadBlock(unsigned char block)
{
	Cmd.SendBuffer[0] = 04;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x11;
	Cmd.SendBuffer[3] = block;
	SendCommand();
}

/*******************************************************************************
* 函数名         : WriteBlock
* 描述           : 写块数据
* 输入           : block-块号
									 data -块数据
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void WriteBlock(unsigned char block, unsigned char* data)
{
	unsigned char i;
	Cmd.SendBuffer[0] = 20;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x12;
	Cmd.SendBuffer[3] = block;
	for(i = 0; i < 16; i++)
	{
		Cmd.SendBuffer[i + 4] = *(data + i);
	}
	SendCommand();
}

/*******************************************************************************
* 函数名         : MakeCard
* 描述           : 办卡
* 输入           : block-块号
									 value-初始值
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void MakeCard(unsigned char block, unsigned char value)
{
	Cmd.SendBuffer[0] = 8;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x13;
	Cmd.SendBuffer[3] = block;
	Cmd.SendBuffer[4] = value >> 24;
	Cmd.SendBuffer[5] = value >> 16;
	Cmd.SendBuffer[6] = value >> 8;
	Cmd.SendBuffer[7] = value;
	SendCommand();
}

/*******************************************************************************
* 函数名         : SetDefault
* 描述           : 恢复卡
* 输入           : block-块号
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void SetDefault(unsigned char block)
{
	Cmd.SendBuffer[0] = 4;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x14;
	Cmd.SendBuffer[3] = block;
	SendCommand();
}

/*******************************************************************************
* 函数名         : Inc
* 描述           : 充值
* 输入           : block-块号
									 value-金额
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void Inc(unsigned char block, unsigned char value)
{
	Cmd.SendBuffer[0] = 8;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x15;
	Cmd.SendBuffer[3] = block;
	Cmd.SendBuffer[4] = value >> 24;
	Cmd.SendBuffer[5] = value >> 16;
	Cmd.SendBuffer[6] = value >> 8;
	Cmd.SendBuffer[7] = value;
	SendCommand();
}

/*******************************************************************************
* 函数名         : Dec
* 描述           : 扣款
* 输入           : block-块号
									 value-金额
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void Dec(unsigned char block, unsigned char value)
{
	Cmd.SendBuffer[0] = 8;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x16;
	Cmd.SendBuffer[3] = block;
	Cmd.SendBuffer[4] = value >> 24;
	Cmd.SendBuffer[5] = value >> 16;
	Cmd.SendBuffer[6] = value >> 8;
	Cmd.SendBuffer[7] = value;
	SendCommand();
}

/*******************************************************************************
* 函数名         : LodeKey
* 描述           : 装载密码
* 输入           : KeyA -密钥A
									 KeyB -密钥B
									 mode -加密模式
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LodeKey(unsigned char* KeyA, unsigned char* KeyB, unsigned char mode)
{
	unsigned char i;
	
	Cmd.SendBuffer[0] = 21;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x2B;
	for(i = 0; i < 6; i++)
	{
		Cmd.SendBuffer[3 + i] = *(KeyA + i);
		Cmd.SendBuffer[9 + i] = *(KeyB + i);
	}
	Cmd.SendBuffer[15] = mode;
	Cmd.SendBuffer[16] = 0x03;
	Cmd.SendBuffer[17] = 0x08;
	Cmd.SendBuffer[18] = 0x05;
	Cmd.SendBuffer[19] = 0x02;
	Cmd.SendBuffer[20] = 0x07;
	
	SendCommand();
}

/*******************************************************************************
* 函数名         : SetBaud
* 描述           : 设置波特率
* 输入           : baud -波特率
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void SetBaud(unsigned long baud)
{
	Cmd.SendBuffer[0] = 10;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x2C;
	Cmd.SendBuffer[3] = baud >> 24;
	Cmd.SendBuffer[4] = baud >> 16;
	Cmd.SendBuffer[5] = baud >> 8;
	Cmd.SendBuffer[6] = baud;
	Cmd.SendBuffer[7] = 0x98;
	Cmd.SendBuffer[8] = 0x24;
	Cmd.SendBuffer[9] = 0x31;
	
	SendCommand();
}

/*******************************************************************************
* 函数名         : SetAddress
* 描述           : 设置地址
* 输入           : address -地址
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void SetAddress(unsigned char address)
{
	Cmd.SendBuffer[0] = 10;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x2D;
	Cmd.SendBuffer[3] = address;
	Cmd.SendBuffer[4] = 0x37;
	Cmd.SendBuffer[5] = 0x21;
	Cmd.SendBuffer[6] = 0x56;
	SendCommand();
}

/*******************************************************************************
* 函数名         : SetMode
* 描述           : 设置地址
* 输入           : address -地址
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void SetMode(unsigned char mode, unsigned char block, unsigned long value)
{
	Cmd.SendBuffer[0] = 10;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x2E;
	Cmd.SendBuffer[3] = mode;
	Cmd.SendBuffer[4] = mode + 0x0A;
	Cmd.SendBuffer[5] = block;
	Cmd.SendBuffer[6] = value >> 24;
	Cmd.SendBuffer[7] = value >> 16;
	Cmd.SendBuffer[8] = value >> 8;
	Cmd.SendBuffer[9] = value;
	
	Cmd.SendBuffer[10] = 0x23;
	Cmd.SendBuffer[11] = 0x12;
	Cmd.SendBuffer[12] = 0x54;
	SendCommand();
}

/*******************************************************************************
* 函数名         : CommandProcess
* 描述           : 命令处理。
* 输入           : 无
* 输出           : 无
* 返回           : 执行结果。
                   -0   : 正确。
                   -0xFF: 无卡。
                   -0xFE: 卡操作错误。
                   -0xFD: 命令错误。
                   -0xFC: 没有收到命令。
*******************************************************************************/
unsigned char CommandProcess(void)
{
	unsigned char status = 0xFC, i;
	
	if(Cmd.ReceiveFlag)
	{
		if(Cmd.ReceiveBuffer[Cmd.ReceiveBuffer[0]] == CheckSum(Cmd.ReceiveBuffer, Cmd.ReceiveBuffer[0]))
		{
			//Cmd.ReceiveBuffer[0]数据长度  字节数?
			//Cmd.ReceiveBuffer[1]地址 默认0x00 
			//Cmd.ReceiveBuffer[2]命令字 发数命令字或0x80之后返回
			//Cmd.ReceiveBuffer[3]状态 0x00正确 0xFF 无卡 0xFE 错误  0xFD 保留 0xFC 扣款时余额不足 0xFB 检验错误
			//Cmd.ReceiveBuffer[4]卡类型
			//Cmd.ReceiveBuffer[5]卡类型
			//Cmd.ReceiveBuffer[6]Data
			//Cmd.ReceiveBuffer[7]Data
			if((Cmd.ReceiveBuffer[3] == 0x00)&&((Cmd.ReceiveBuffer[2]==0x91)||(Cmd.ReceiveBuffer[2]==0x92)))
			{
			
						switch(Cmd.ReceiveBuffer[2])
						{
							/* 读卡号 */
							case 0x90:
								Picc.UID = Cmd.ReceiveBuffer[9];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[8];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[7];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[6];
								break;
							/* 读块数据 */
							case 0x91:
								Picc.UID = Cmd.ReceiveBuffer[9];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[8];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[7];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[6];
								
								for(i = 0; i < 16; i++)
								{
									Picc.BlockData[i] = Cmd.ReceiveBuffer[10 + i];
								}
								ICReadFlag = 1;
								break;
							/* 写块数据 */
							case 0x92:
								Picc.UID = Cmd.ReceiveBuffer[9];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[8];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[7];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[6];
								ICWriteFlag = 1;
								break;
							/* 办卡 */
							case 0x93:
								Picc.UID = Cmd.ReceiveBuffer[9];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[8];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[7];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[6];
								break;
							/* 恢复卡 */
							case 0x94:
								Picc.UID = Cmd.ReceiveBuffer[9];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[8];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[7];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[6];
								break;
							/* 充值 */
							case 0x95:
								Picc.UID = Cmd.ReceiveBuffer[9];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[8];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[7];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[6];
								
								Picc.Value = Cmd.ReceiveBuffer[10];
								Picc.Value <<= 8;
								Picc.Value |= Cmd.ReceiveBuffer[11];
								Picc.Value <<= 8;
								Picc.Value |= Cmd.ReceiveBuffer[12];
								Picc.Value <<= 8;
								Picc.Value |= Cmd.ReceiveBuffer[13];
								break;
							/* 扣款 */
							case 0x96:
								Picc.UID = Cmd.ReceiveBuffer[9];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[8];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[7];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[6];
								
								Picc.Value = Cmd.ReceiveBuffer[10];
								Picc.Value <<= 8;
								Picc.Value |= Cmd.ReceiveBuffer[11];
								Picc.Value <<= 8;
								Picc.Value |= Cmd.ReceiveBuffer[12];
								Picc.Value <<= 8;
								Picc.Value |= Cmd.ReceiveBuffer[13];
								break;
							/* 保留 */
							case 0xAA:
								break;
							/* 装载密码 */
							case 0xAB:
								break;
							/* 设置波特率 */
							case 0xAC:
								break;
							/* 设置地址 */
							case 0xAD:
								break;
							/* 设置工作模式 */	
							case 0x2E:
								break;
							default:status = 0xFD; 
							  break;
						}
		   }
			
			else
			//	if(Cmd.ReceiveBuffer[3] == 0x00)
				{
					//FlagIfOperationCard = 1;
				}
			status = Cmd.ReceiveBuffer[3];
		}
		else
			{
				 status = 0xFD;
			}
			Cmd.ReceiveFlag = 0;
	}
 	if((1 == WriteBlockFlag)&&(1 == WriteBlockTimeFlag))//jie shou buchenggong  erqie manzu 100ms
 	{
 		WriteBlock(1,ICDataReadBuf);
 		WriteBlockTimeFlag =0;
 	}

	return status;
}


/*******************************************************************************
* 函数名         : CheckSum
* 描述           : 命令校验。
* 输入           : 	dat : 要校验的数组。
										num : 校验的字节数。
* 输出           : 无
* 返回           : 校验值。
*******************************************************************************/
unsigned char CheckSum(unsigned char *dat, unsigned char num)
{
  unsigned char bTemp = 0, i;
  
  for(i = 0; i < num; i ++){bTemp ^= dat[i];}
  return bTemp;
}

/*******************************************************************************
* 函数名         : SendCommand
* 描述           : 命令开始发送函数。
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void SendCommand(void)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t PiccSendBuff[50];
	uint8_t PiccSendLength = 0;
	for(i=0;i<50;i++)
	{
		PiccSendBuff[i] = 0;
	}
	//while(Cmd.SendFlag != 0);
	Cmd.SendFlag = 1;
	Cmd.SendBuffer[Cmd.SendBuffer[0]] = CheckSum(Cmd.SendBuffer, Cmd.SendBuffer[0]);
	Cmd.SendPoint = Cmd.SendBuffer[0] + 1;
	PiccSendBuff[0] = 0x7F;
	for(i=1;i<(2*(Cmd.SendBuffer[0]+1));i++)
	{
		if(Cmd.SendBuffer[j]!=0x7F)
		{
			PiccSendBuff[i] = Cmd.SendBuffer[j];
		}
		else
		{
			PiccSendBuff[i] = 0x7F;
			PiccSendBuff[i+1] = 0x7F;
			i = i+1;
		}
		j=j+1;
		if(j==(Cmd.SendBuffer[0]+1))
		{
			PiccSendLength = i+1;
			break;
		}
	}	
	HAL_UART_Transmit(&UartHandle6, PiccSendBuff, PiccSendLength, 0xFFFF);
}







void USART6_UART_Init(void)
{
	
    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART6 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle6.Instance        = USART6;
  UartHandle6.Init.BaudRate   = 9600;
  UartHandle6.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle6.Init.StopBits   = UART_STOPBITS_1;
  UartHandle6.Init.Parity     = UART_PARITY_NONE;
  UartHandle6.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle6.Init.Mode       = UART_MODE_TX_RX;

  if (HAL_UART_Init(&UartHandle6) != HAL_OK)
  {
    /* Initialization Error */

  }
 //__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
  
  if(HAL_UART_Receive_IT(&UartHandle6, pData, 1) != HAL_OK)
  {
   // Error_Handler();
  }	
}

uint8_t CardLock()
{
		uint8_t i;
		uint32_t Money=0;
		uint32_t ICCard=0;
		if(ICReadFlag == 1)    //读卡成功
		{
					ICReadFlag = 0;
					ICCard_Snr[0] =  	(Picc.UID >>24)&0xff;                 //卡号低位
					ICCard_Snr[1] =		(Picc.UID >>16)&0xff;
					ICCard_Snr[2] =		(Picc.UID >>8)&0xff;
					ICCard_Snr[3] =   (Picc.UID)&0xff;              //卡号高位
					for(i=0;i<16;i++)
					{
						ICDataReadBuf[i] = Picc.BlockData[i];
					}
					for(i=0;i<4;i++)
					{
						ICCard1[i] = ICCard_Snr[i];   //卡号
						ICMoney[i] = ICDataReadBuf[i];   //卡内金额
						StartICMoney[i] = ICMoney[i];	 //存储交易起始金额					
					}
					//UpDataId();     //上传卡号至屏幕
					//UpDataBalance();    //上传余额至屏幕
					//ICCard = (ICCard1[3]<<24)+(ICCard1[2]<<16)+(ICCard1[1]<<8)+ICCard1[0];  
					ICCard = Picc.UID;
					Money = (ICDataReadBuf[0]<<24) + (ICDataReadBuf[1]<<16) + (ICDataReadBuf[2]<<8) + ICDataReadBuf[3] ;
					if(Money <= 200)   //余额不足
					{
						    UpDataId();     //上传卡号至屏幕
								UpDataBalance();    //上传余额至屏幕
								SpeechFlagMoney = 1;
								return 3;
					}
					else
					{     
								if(unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargeFlag == 0xff)//上次交易不成功
								{
											 if(unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo == ICCard) // 卡号是否和上次一致
											 {
														//继续显示已充电量，已充金额
												    UpDataId();     //上传卡号至屏幕
														UpDataBalance();    //上传余额至屏幕
														FlagIfSaveData = 1;       //保存上次交易数据 默认为0 1表示有效
														ICWriteFlag = 1;     //直接充电
														return 2;            //返回无效数据
											 }
											 else
											 {
																			 //将上次数据写入主存储器
													FlagIfSaveToMemory = 1;
											 }
								}
								else        //上次交易完成
								{
																			 
								}
								if(0xff == ICDataReadBuf[4])    //上次交易未完成
								{
										//HAL_Delay(500);
									  UpDataId();     //上传卡号至屏幕
										UpDataBalance();    //上传余额至屏幕
										SpeechFlagDeal = 1;
										return 1;
								}
								else if(0x00 == ICDataReadBuf[4])     //成功读卡
								{
										ICDataReadBuf[4] = 0xff;
										WriteBlockFlag = 1;
										WriteBlock(1,ICDataReadBuf);
										return 2;
								}
					}
                    return 0;
		}
		else
		{
				// SpeechFlag = 1;
				 return 0;
		}
}


uint8_t CardunLock()
{
	uint8_t i=0;
	if(ICReadFlag == 1)    //读卡成功
	{
			ICReadFlag = 0;
			ICCard_Snr[0] =  	(Picc.UID >>24)&0xff;                 //卡号低位
			ICCard_Snr[1] =		(Picc.UID >>16)&0xff;
			ICCard_Snr[2] =		(Picc.UID >>8)&0xff;
			ICCard_Snr[3] =   (Picc.UID)&0xff;              //卡号高位
			for(i=0;i<16;i++)
			{
					ICDataReadBuf[i] = Picc.BlockData[i];
			}
			StartICMoneyAll = (ICDataReadBuf[0]<<24) + (ICDataReadBuf[1]<<16) + (ICDataReadBuf[2]<<8) + ICDataReadBuf[3] ;
			if((ICCard_Snr[0]==ICCard1[0])&&(ICCard_Snr[1]==ICCard1[1])&&(ICCard_Snr[2]==ICCard1[2])&&(ICCard_Snr[3]==ICCard1[3]))  //卡号验证成功
			{
					StopMoneyAll      =  StartICMoneyAll - DealMoneyAll;
					ICDataReadBuf[0]  = (StopMoneyAll>>24)&0XFF;
					ICDataReadBuf[1]  = (StopMoneyAll>>16)&0XFF;
					ICDataReadBuf[2]  = (StopMoneyAll>>8)&0XFF;
					ICDataReadBuf[3]  = (StopMoneyAll)&0XFF;
					ICDataReadBuf[4]  = 0x00;	
					WriteBlock(1,ICDataReadBuf);
					WriteBlockFlag = 1;
					return 2;
			}
	}
			return 0;
}


