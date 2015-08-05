/******************** (C) COPYRIGHT 2013 ********************
* �ļ���          : PICC.c
* ����            : ������ӿ����Ŷӡ�
* �汾            : V2.0.1.0
* ����            : 2013-5-10
* ����            : ���жԿ��Ĳ�������ģ�鷢�͵����
* ����֧��        : QQ: 526181679
********************************************************************************/

/* �����ļ� -------------------------------------------------------------------*/
//#include "PICC.h"
#include "main.h"
/* �궨�� ---------------------------------------------------------------------*/
/* ���ļ�ʹ�õı��� -----------------------------------------------------------*/
PICC Picc;
CMD Cmd;
extern UART_HandleTypeDef   UartHandle6;
/* ���ļ�ʹ�õĺ������� -------------------------------------------------------*/
void SendCommand(void);
/* ���ļ������� ---------------------------------------------------------------*/

/*******************************************************************************
* ������         : ReadCard
* ����           : ������
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void ReadCard(void)
{
	Cmd.SendBuffer[0] = 03;
	Cmd.SendBuffer[1] = 00;
	Cmd.SendBuffer[2] = 0x10;
	SendCommand();
}

/*******************************************************************************
* ������         : ReadBlock
* ����           : ��������
* ����           : block-���
* ���           : ��
* ����           : ��
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
* ������         : WriteBlock
* ����           : д������
* ����           : block-���
									 data -������
* ���           : ��
* ����           : ��
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
* ������         : MakeCard
* ����           : �쿨
* ����           : block-���
									 value-��ʼֵ
* ���           : ��
* ����           : ��
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
* ������         : SetDefault
* ����           : �ָ���
* ����           : block-���
* ���           : ��
* ����           : ��
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
* ������         : Inc
* ����           : ��ֵ
* ����           : block-���
									 value-���
* ���           : ��
* ����           : ��
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
* ������         : Dec
* ����           : �ۿ�
* ����           : block-���
									 value-���
* ���           : ��
* ����           : ��
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
* ������         : LodeKey
* ����           : װ������
* ����           : KeyA -��ԿA
									 KeyB -��ԿB
									 mode -����ģʽ
* ���           : ��
* ����           : ��
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
* ������         : SetBaud
* ����           : ���ò�����
* ����           : baud -������
* ���           : ��
* ����           : ��
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
* ������         : SetAddress
* ����           : ���õ�ַ
* ����           : address -��ַ
* ���           : ��
* ����           : ��
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
* ������         : SetMode
* ����           : ���õ�ַ
* ����           : address -��ַ
* ���           : ��
* ����           : ��
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
* ������         : CommandProcess
* ����           : �����
* ����           : ��
* ���           : ��
* ����           : ִ�н����
                   -0   : ��ȷ��
                   -0xFF: �޿���
                   -0xFE: ����������
                   -0xFD: �������
                   -0xFC: û���յ����
*******************************************************************************/
unsigned char CommandProcess(void)
{
	unsigned char status = 0xFC, i;
	
	if(Cmd.ReceiveFlag)
	{
		if(Cmd.ReceiveBuffer[Cmd.ReceiveBuffer[0]] == CheckSum(Cmd.ReceiveBuffer, Cmd.ReceiveBuffer[0]))
		{
			//Cmd.ReceiveBuffer[0]���ݳ���  �ֽ���?
			//Cmd.ReceiveBuffer[1]��ַ Ĭ��0x00 
			//Cmd.ReceiveBuffer[2]������ ���������ֻ�0x80֮�󷵻�
			//Cmd.ReceiveBuffer[3]״̬ 0x00��ȷ 0xFF �޿� 0xFE ����  0xFD ���� 0xFC �ۿ�ʱ���� 0xFB �������
			//Cmd.ReceiveBuffer[4]������
			//Cmd.ReceiveBuffer[5]������
			//Cmd.ReceiveBuffer[6]Data
			//Cmd.ReceiveBuffer[7]Data
			if((Cmd.ReceiveBuffer[3] == 0x00)&&((Cmd.ReceiveBuffer[2]==0x91)||(Cmd.ReceiveBuffer[2]==0x92)))
			{
			
						switch(Cmd.ReceiveBuffer[2])
						{
							/* ������ */
							case 0x90:
								Picc.UID = Cmd.ReceiveBuffer[9];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[8];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[7];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[6];
								break;
							/* �������� */
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
							/* д������ */
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
							/* �쿨 */
							case 0x93:
								Picc.UID = Cmd.ReceiveBuffer[9];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[8];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[7];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[6];
								break;
							/* �ָ��� */
							case 0x94:
								Picc.UID = Cmd.ReceiveBuffer[9];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[8];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[7];
								Picc.UID <<= 8;
								Picc.UID |= Cmd.ReceiveBuffer[6];
								break;
							/* ��ֵ */
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
							/* �ۿ� */
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
							/* ���� */
							case 0xAA:
								break;
							/* װ������ */
							case 0xAB:
								break;
							/* ���ò����� */
							case 0xAC:
								break;
							/* ���õ�ַ */
							case 0xAD:
								break;
							/* ���ù���ģʽ */	
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
* ������         : CheckSum
* ����           : ����У�顣
* ����           : 	dat : ҪУ������顣
										num : У����ֽ�����
* ���           : ��
* ����           : У��ֵ��
*******************************************************************************/
unsigned char CheckSum(unsigned char *dat, unsigned char num)
{
  unsigned char bTemp = 0, i;
  
  for(i = 0; i < num; i ++){bTemp ^= dat[i];}
  return bTemp;
}

/*******************************************************************************
* ������         : SendCommand
* ����           : ���ʼ���ͺ�����
* ����           : ��
* ���           : ��
* ����           : ��
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
		if(ICReadFlag == 1)    //�����ɹ�
		{
					ICReadFlag = 0;
					ICCard_Snr[0] =  	(Picc.UID >>24)&0xff;                 //���ŵ�λ
					ICCard_Snr[1] =		(Picc.UID >>16)&0xff;
					ICCard_Snr[2] =		(Picc.UID >>8)&0xff;
					ICCard_Snr[3] =   (Picc.UID)&0xff;              //���Ÿ�λ
					for(i=0;i<16;i++)
					{
						ICDataReadBuf[i] = Picc.BlockData[i];
					}
					for(i=0;i<4;i++)
					{
						ICCard1[i] = ICCard_Snr[i];   //����
						ICMoney[i] = ICDataReadBuf[i];   //���ڽ��
						StartICMoney[i] = ICMoney[i];	 //�洢������ʼ���					
					}
					//UpDataId();     //�ϴ���������Ļ
					//UpDataBalance();    //�ϴ��������Ļ
					//ICCard = (ICCard1[3]<<24)+(ICCard1[2]<<16)+(ICCard1[1]<<8)+ICCard1[0];  
					ICCard = Picc.UID;
					Money = (ICDataReadBuf[0]<<24) + (ICDataReadBuf[1]<<16) + (ICDataReadBuf[2]<<8) + ICDataReadBuf[3] ;
					if(Money <= 200)   //����
					{
						    UpDataId();     //�ϴ���������Ļ
								UpDataBalance();    //�ϴ��������Ļ
								SpeechFlagMoney = 1;
								return 3;
					}
					else
					{     
								if(unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargeFlag == 0xff)//�ϴν��ײ��ɹ�
								{
											 if(unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo == ICCard) // �����Ƿ���ϴ�һ��
											 {
														//������ʾ�ѳ�������ѳ���
												    UpDataId();     //�ϴ���������Ļ
														UpDataBalance();    //�ϴ��������Ļ
														FlagIfSaveData = 1;       //�����ϴν������� Ĭ��Ϊ0 1��ʾ��Ч
														ICWriteFlag = 1;     //ֱ�ӳ��
														return 2;            //������Ч����
											 }
											 else
											 {
																			 //���ϴ�����д�����洢��
													FlagIfSaveToMemory = 1;
											 }
								}
								else        //�ϴν������
								{
																			 
								}
								if(0xff == ICDataReadBuf[4])    //�ϴν���δ���
								{
										//HAL_Delay(500);
									  UpDataId();     //�ϴ���������Ļ
										UpDataBalance();    //�ϴ��������Ļ
										SpeechFlagDeal = 1;
										return 1;
								}
								else if(0x00 == ICDataReadBuf[4])     //�ɹ�����
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
	if(ICReadFlag == 1)    //�����ɹ�
	{
			ICReadFlag = 0;
			ICCard_Snr[0] =  	(Picc.UID >>24)&0xff;                 //���ŵ�λ
			ICCard_Snr[1] =		(Picc.UID >>16)&0xff;
			ICCard_Snr[2] =		(Picc.UID >>8)&0xff;
			ICCard_Snr[3] =   (Picc.UID)&0xff;              //���Ÿ�λ
			for(i=0;i<16;i++)
			{
					ICDataReadBuf[i] = Picc.BlockData[i];
			}
			StartICMoneyAll = (ICDataReadBuf[0]<<24) + (ICDataReadBuf[1]<<16) + (ICDataReadBuf[2]<<8) + ICDataReadBuf[3] ;
			if((ICCard_Snr[0]==ICCard1[0])&&(ICCard_Snr[1]==ICCard1[1])&&(ICCard_Snr[2]==ICCard1[2])&&(ICCard_Snr[3]==ICCard1[3]))  //������֤�ɹ�
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


