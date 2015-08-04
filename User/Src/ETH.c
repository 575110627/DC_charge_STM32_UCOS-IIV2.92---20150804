#include "stm32f2xx_hal.h"
#include "main.h"
#include "ETH.h"
#include <string.h>
#include "tcp_echoclient.h"


extern I2C_HandleTypeDef    I2C1Handle;
extern strETHData_Typedef ETHData;
extern uint16_t RecvLen;

void AnalyzeData(void)
{
		uint16_t _crc;
		uint8_t a1,a2;
		_crc=CalcCrc(ETHData.Data,(ETHData.Length-2));   //����У��CRC           
		a1=(uint8_t)(_crc&0xff);                          //CRC���ֽ�
		a2=(uint8_t)((_crc>>8)&0xff);                  //CRC���ֽ�
		if((a1 == ETHData.Data[ETHData.Length-2])&&(a2 == ETHData.Data[ETHData.Length-1]))		 //У����ȷ 
		{	 			
// 				if(ETHData.Data[0] == 0xFF)  //�㲥֡
// 				{
// 						AnalyzeBroadcast();  
// 				}
// 				else 
// 				if(ETHData.Data[0] == 0x01)     //
// 				{ 
// 						AnalyzeReceive();
// 				}
			 switch(ETHData.Data[0])
			 {
					case 0xFF:  //�㲥֡
								AnalyzeBroadcast(); 
				     break;
				  case 0x01:  //�ӻ���ַ
								AnalyzeReceive(); 
				     break;
          default :
						break;
			 }
		}
}
void AnalyzeBroadcast(void)
{
	  if(ETHData.Data[1]==0x10)
			{
				Broadcast();                                  //�жϹ�����
			}			
}

void Broadcast(void)
{
			uint16_t startadd;                     //��ʼ��ַ
			uint8_t startaddH,startaddL;           //��ʼ��ַ�ߵ�λ 
			startaddH=ETHData.Data[2];
			startaddL=ETHData.Data[3];
			startadd=(startaddH<<8)+startaddL;
			switch(startadd)
			{
				case 0x1100:
										ReceiveBroadcastTime();          //������ʱ�㲥��ʱ
								break;
				case 0x1210:
										ReceiveBroadcastSystemData1();   //�����㲥���׮ϵͳ����֡1
								break;
				case 0x1220:
										ReceiveBroadcastSystemData2();   //�����㲥���׮ϵͳ����֡2 
								break;
				case 0x2010:
										ReceiveBroadcastBlacklist();
								break;
				default    :
					      break;
			}	
}

void AnalyzeReceive(void)
{
			 switch(ETHData.Data[1])                             //�жϹ�����
				{
					case 0x04:
								ReceiveFunction04();
							break;         
					case 0x10:
								ReceiveFunction10();
							break;
					default:
							break;           //��֧�ֵĹ�����	
				}
}
/*******�ӻ����չ㲥������*******/
void ReceiveBroadcastBlacklist(void)
{
				uint8_t recebuffer[20];
				recebuffer[0]  = ETHData.Data[7];                     //������1
				recebuffer[1]  = ETHData.Data[8];
				recebuffer[2]  = ETHData.Data[9];
				recebuffer[3]  = ETHData.Data[10];
				recebuffer[4]  = ETHData.Data[11];                    //������2
				recebuffer[5]  = ETHData.Data[12];
				recebuffer[6]  = ETHData.Data[13];
				recebuffer[7]  = ETHData.Data[14];
				recebuffer[8]  = ETHData.Data[15];                    //������3
				recebuffer[9]  = ETHData.Data[16];
				recebuffer[10] = ETHData.Data[17];
				recebuffer[11] = ETHData.Data[18];	
				recebuffer[12] = ETHData.Data[19];                   //������4
				recebuffer[13] = ETHData.Data[20];	
				recebuffer[14] = ETHData.Data[21];
				recebuffer[15] = ETHData.Data[22];
				recebuffer[16] = ETHData.Data[23];                   //������5
				recebuffer[17] = ETHData.Data[24];
				recebuffer[18] = ETHData.Data[25];
				recebuffer[19] = ETHData.Data[26];	
}
/*******�ӻ����չ㲥��ʱ*******/
void ReceiveBroadcastTime(void)
{
			RTCSetTime.Year = ETHData.Data[8];              //��15
			RTCSetTime.Month = ETHData.Data[10];            //��
			RTCSetTime.Day = ETHData.Data[12];              //��
			RTCSetTime.Week = ETHData.Data[14];             //����
			RTCSetTime.Hour = (ETHData.Data[16]|0x80);             //ʱ
			RTCSetTime.Minute = ETHData.Data[18];           //��
			RTCSetTime.Second = ETHData.Data[20];           //��	
			Set_Time(&RTCSetTime);
	    UpdateRTC();
}
/*******�ӻ��㲥���׮ϵͳ����֡1******/
void ReceiveBroadcastSystemData1(void)
{
				uint8_t i;
				uint16_t crc=0xffff;
				unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice = (ETHData.Data[7]<<8)+ETHData.Data[8];              //ƽ�����
				unSystemParameterConfigHandle.SystemParameterConfigHandle.HPrice=(ETHData.Data[9]<<8)+ETHData.Data[10];              //��ֵ���		
				unSystemParameterConfigHandle.SystemParameterConfigHandle.LPrice=(ETHData.Data[11]<<8)+ETHData.Data[12];             //��ֵ���
				unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStartH =(ETHData.Data[13]<<8)+ETHData.Data[14];             //��ֵ���ʱ�� ��ʼʱ           
				unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStartM=(ETHData.Data[15]<<8)+ETHData.Data[16];              //��ֵ���ʱ�� ��ʼ��            
				unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStopH=(ETHData.Data[17]<<8)+ETHData.Data[18];            //��ֵ���ʱ�� ����ʱ           
				unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStopM=(ETHData.Data[19]<<8)+ETHData.Data[20];            //��ֵ���ʱ�� ������
				crc=CalcCrc(unSystemParameterConfigHandle.unbuffer,60);
				unSystemParameterConfigHandle.SystemParameterConfigHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);            //CRC
				unSystemParameterConfigHandle.SystemParameterConfigHandle.CRCLow = (uint8_t)(crc&0xff) ;  			//CRC
				HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS+2,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
				for(i=0;i<ETHData.Length;i++)
				{
					ETHData.Data[i] = ETHData.Data[i]+1;
				}
				TCP_Server_SendData(ETHData.Data,ETHData.Length);
				HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS+2,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
				TCP_Server_SendData(unSystemParameterConfigHandle.unbuffer,62);
}
/*******�ӻ��㲥���׮ϵͳ����֡2******/
void ReceiveBroadcastSystemData2(void)
{
				uint8_t i;
				uint16_t crc=0xffff;
				unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStartH = (ETHData.Data[7]<<8)+ETHData.Data[8];         //��ֵ���ʱ�� ��ʼʱ
				unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStartM = (ETHData.Data[9]<<8)+ETHData.Data[10];         //��ֵ���ʱ�� ��ʼ��
				unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStopH  = (ETHData.Data[11]<<8)+ETHData.Data[12];          //��ֵ���ʱ�� ����ʱ
				unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStopM  = (ETHData.Data[13]<<8)+ETHData.Data[14];          //��ֵ���ʱ�� ������
				unSystemParameterConfigHandle.SystemParameterConfigHandle.StartMode    = (ETHData.Data[15]<<8)+ETHData.Data[16]; 	      //����ģʽ 
				unSystemParameterConfigHandle.SystemParameterConfigHandle.BillMode=(ETHData.Data[17]<<8)+ETHData.Data[18];         //�Ʒѷ�ʽ
				crc=CalcCrc(unSystemParameterConfigHandle.unbuffer,60);
				unSystemParameterConfigHandle.SystemParameterConfigHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);            //CRC
				unSystemParameterConfigHandle.SystemParameterConfigHandle.CRCLow = (uint8_t)(crc&0xff) ;  			//CRC
				HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS+2,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
				for(i=0;i<ETHData.Length;i++)
				{
					ETHData.Data[i] = ETHData.Data[i]+1;
				}
				TCP_Server_SendData(ETHData.Data,ETHData.Length);
				HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS+2,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
				TCP_Server_SendData(unSystemParameterConfigHandle.unbuffer,62);
}

void ReceiveFunction04(void)
{
			uint16_t startadd;               //��ʼ��ַ
			uint8_t startaddH,startaddL;     //��ʼ��ַ�ߵ�λ
			startaddH=ETHData.Data[2];
			startaddL=ETHData.Data[3];
			startadd=(startaddH<<8)+startaddL;
			switch(startadd)
			{
				case 0x1000:
										ReplyToHostPoSlaveStaframe();
							break;             //�ӻ���Ӧ������ѯ�ӻ�״̬֡
				case 0x1020:
										ReplyToHostReadTotalTranData1();
							break;         //�ӻ���Ӧ������ȡһ��������������֡
				default:
							break;
			}	 	 
}
void ReceiveFunction10(void)
{
			uint16_t startadd;               //��ʼ��ַ
			uint8_t startaddH,startaddL;     //��ʼ��ַ�ߵ��
			startaddH=ETHData.Data[2];
			startaddL=ETHData.Data[3];
			startadd=(startaddH<<8)+startaddL;
			if(startadd == 0x2050)
			{
						SendTradingSerialNumber();
			}		
}
/*******�ӻ���Ӧ������ѯ�ӻ�״̬֡*******/
void ReplyToHostPoSlaveStaframe(void)
{
			uint8_t sendbuffer[19];
			uint16_t _crc;
			uint16_t register_num =0;           //�Ĵ�������
			uint8_t a1,a2;
			register_num = 2*((ETHData.Data[4]<<8)+ETHData.Data[5]);
			sendbuffer[0] = ETHData.Data[0];  //�ӻ���ַ
			sendbuffer[1] = ETHData.Data[1];  //������
			sendbuffer[2] = (uint8_t)(register_num);//0x0E;         //�ֽ���
			sendbuffer[3] = 0x00;         //���׮��1
			sendbuffer[4] = 0x00;         //���׮��1
			sendbuffer[5] = 0x00;         //���׮��2
			sendbuffer[6] = 0x01;         //���׮��2
			sendbuffer[7] = 0x00;         //���׮״̬
			sendbuffer[8] = 0x00;         //���׮״̬
			sendbuffer[9] = 0x00;         //���׮��������          
			sendbuffer[10] = 0x00;        //���׮�������� 
			sendbuffer[11] = 0x00;        //�Ƿ��ȡ���׼�¼
			sendbuffer[12] = 0x55;        //�Ƿ��ȡ���׼�¼
			sendbuffer[13] = 0x00;        //�Ƿ�ɼ���ʷ����
			sendbuffer[14] = 0x55;        //�Ƿ�ɼ���ʷ����
			sendbuffer[15] = 0x00;        //�Ƿ��ȡ����������
			sendbuffer[16] = 0x55;        //�Ƿ��ȡ����������
			_crc=CalcCrc(sendbuffer,register_num + 3);
			a1=(uint8_t)(_crc&0xff);                          //CRC���ֽ�
			a2=(uint8_t)((_crc>>8)&0xff);                     //CRC���ֽ�
			sendbuffer[17]=a1;                 //CRC��
			sendbuffer[18]=a2;                 //CRC��
			TCP_Server_SendData(sendbuffer,19);
}
/*******�ӻ���Ӧ������ȡһ��������������֡1******/
void ReplyToHostReadTotalTranData1(void)
{
			uint8_t sendbuffer[85];  
			uint16_t _crc;
      //��ǰ���׼�¼
	    if(1 == FlagIfReadPriDeal)
			{
						FlagIfReadPriDeal = 0;
				    ReadHisRec(2,unSaveRealTimeDataHandle.unbuffer,73);
				    HAL_Delay(100);
				    HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
				    HAL_Delay(100);
     				sendbuffer[0] = ETHData.Data[0];
						sendbuffer[1] = ETHData.Data[1];
						sendbuffer[2] = 2*((ETHData.Data[4]<<8)+ETHData.Data[5]); //�ֽ���                             //�ֽ���
						sendbuffer[3] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType>>8)&0xFF;                    //��������
						sendbuffer[4] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType)&0xFF ;                   //��������
						sendbuffer[5] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>24)&0xFF;                   //������ˮ��1gao
						sendbuffer[6] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>16)&0xFF;                   //������ˮ��1di
						sendbuffer[7] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>8)&0xFF;                   //������ˮ��2
						sendbuffer[8] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo)&0xFF;                    //������ˮ��2
						sendbuffer[9] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber>>8)&0xFF;//middlebuffer[7];                   //��������
						sendbuffer[10]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber)&0xFF;//middlebuffer[8];                  //��������
						sendbuffer[11]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>24)&0XFF;                  //����1
						sendbuffer[12]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>16)&0XFF;                   //����1
						sendbuffer[13]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>8)&0XFF;                  //����2
						sendbuffer[14]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo)&0XFF;                  //����2
						sendbuffer[15]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType>>8)&0xFF;                 //����
						sendbuffer[16]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType)&0xFF ;                  //����
						sendbuffer[17]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy>>8)&0xFF;;                 //���׵���
						sendbuffer[18]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy)&0xFF;                 //���׵���
						sendbuffer[19]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice>>8)&0xFF;                //���׵��               
						sendbuffer[20]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice)&0xFF ;                 //���׵��
						sendbuffer[21]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>24)&0xFF;                 //ͣ����1
						sendbuffer[22]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>26)&0xFF;                  //ͣ����1
						sendbuffer[23]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>8)&0xFF;                 //ͣ����2
						sendbuffer[24]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee)&0xFF;                  //ͣ����2
						sendbuffer[25]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>24)&0xFF;                 //����ǰ���1
						sendbuffer[26]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>16)&0xFF; ;                 //����ǰ���1
						sendbuffer[27]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>8)&0xFF; ;                 //����ǰ���2
						sendbuffer[28]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney)&0xFF; ;                 //����ǰ���2
						sendbuffer[29]= 20;                 //���׿�ʼ��
						sendbuffer[30]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartYear;                 //���׿�ʼ��
						sendbuffer[31]= 0x00;                 //���׿�ʼ��
						sendbuffer[32]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMonth;                 //���׿�ʼ��
						sendbuffer[33]= 0x00;                 //���׿�ʼ��
						sendbuffer[34]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartDay;                 //���׿�ʼ��
						sendbuffer[35]= 0x00;                 //���׿�ʼ����
						sendbuffer[36]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartWeek;                 //���׿�ʼ����
						sendbuffer[37]= 0x00;                 //���׿�ʼʱ
						sendbuffer[38]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartHour;                 //���׿�ʼʱ
						sendbuffer[39]= 0x00;                 //���׿�ʼ��
						sendbuffer[40]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMin;                 //���׿�ʼ��
						sendbuffer[41]= 0x00;                 //���׿�ʼ��
						sendbuffer[42]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartSec;                 //���׿�ʼ��
						sendbuffer[43]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>24)&0xFF;                 //���׺����1            
						sendbuffer[44]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>16)&0xFF;                //���׺����1
						sendbuffer[45]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>8)&0xFF;                 //���׺����2
						sendbuffer[46]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney)&0xFF;                //���׺����2
						sendbuffer[47]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>24)&0xFF;                 //���׼�����1
						sendbuffer[48]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>16)&0xFF; ;                 //���׼�����1
						sendbuffer[49]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>8)&0xFF; ;                 //���׼�����2
						sendbuffer[50]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter)&0xFF; ;                 //���׼�����2
						sendbuffer[51]= 0x00;                 //���׮��1
						sendbuffer[52]= 0x00;                 //���׮��1
						sendbuffer[53]= 0x00;                 //���׮��2
						sendbuffer[54]= 0x00;                 //���׮��2
						sendbuffer[55]= 0x00;                  //���׽�����
						sendbuffer[56]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear;                  //���׽�����
						sendbuffer[57]= 0x00;                  //���׽�����
						sendbuffer[58]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth;                  //���׽�����
						sendbuffer[59]= 0x00;                  //���׽�����
						sendbuffer[60]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay;                  //���׽�����
						sendbuffer[61]= 0x00;                  //���׽�������
						sendbuffer[62]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek;                  //���׽�������
						sendbuffer[63]= 0x00;                  //���׽���ʱ
						sendbuffer[64]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour;                 //���׽���ʱ
						sendbuffer[65]= 0x00;                 //���׽�����
						sendbuffer[66]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin;                 //���׽�����
						sendbuffer[67]= 0x00;                 //���׿�ʼ��
						sendbuffer[68]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec;                 //���׿�ʼ��
						sendbuffer[69]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>24)&0xFF;                 //���汾��1
						sendbuffer[70]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>16)&0xFF;                 //���汾��1
						sendbuffer[71]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>8)&0xFF;                 //���汾��2
						sendbuffer[72]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer)&0xFF;                 //���汾��2
						sendbuffer[73]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>24)&0xFF;                 //POS����1
						sendbuffer[74]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>16)&0xFF;              //POS����1
						sendbuffer[75]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>8)&0xFF ;               //POS����2
						sendbuffer[76]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber)&0xFF;               //POS����2
						sendbuffer[77]= 0x00;                 //��״̬��
						sendbuffer[78]= 0x00;                 //��״̬��
						sendbuffer[79]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod>>8)&0xFF;;                 //��緽ʽ
						sendbuffer[80]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod)&0xFF;;                 //��緽ʽ
						sendbuffer[81]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate>>8)&0xFF ;                  //��������
						sendbuffer[82]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate)&0xFF ;                  //��������
						_crc=CalcCrc(sendbuffer,83);
						sendbuffer[83]=(uint8_t)(_crc&0xff);                 //CRC��
						sendbuffer[84]=(uint8_t)((_crc>>8)&0xff);                //CRC��
						TCP_Server_SendData(sendbuffer,85);
			}
			else
			{
						sendbuffer[0] = ETHData.Data[0];
						sendbuffer[1] = ETHData.Data[1];
						sendbuffer[2] = 2*((ETHData.Data[4]<<8)+ETHData.Data[5]); //�ֽ���                             //�ֽ���
						sendbuffer[3] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType>>8)&0xFF;                    //��������
						sendbuffer[4] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType)&0xFF ;                   //��������
						sendbuffer[5] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>24)&0xFF;                   //������ˮ��1gao
						sendbuffer[6] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>16)&0xFF;                   //������ˮ��1di
						sendbuffer[7] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>8)&0xFF;                   //������ˮ��2
						sendbuffer[8] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo)&0xFF;                    //������ˮ��2
						sendbuffer[9] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber>>8)&0xFF;//middlebuffer[7];                   //��������
						sendbuffer[10]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber)&0xFF;//middlebuffer[8];                  //��������
						sendbuffer[11]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>24)&0XFF;                  //����1
						sendbuffer[12]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>16)&0XFF;                   //����1
						sendbuffer[13]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>8)&0XFF;                  //����2
						sendbuffer[14]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo)&0XFF;                  //����2
						sendbuffer[15]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType>>8)&0xFF;                 //����
						sendbuffer[16]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType)&0xFF ;                  //����
						sendbuffer[17]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy>>8)&0xFF;;                 //���׵���
						sendbuffer[18]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy)&0xFF;                 //���׵���
						sendbuffer[19]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice>>8)&0xFF;                //���׵��               
						sendbuffer[20]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice)&0xFF ;                 //���׵��
						sendbuffer[21]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>24)&0xFF;                 //ͣ����1
						sendbuffer[22]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>26)&0xFF;                  //ͣ����1
						sendbuffer[23]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>8)&0xFF;                 //ͣ����2
						sendbuffer[24]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee)&0xFF;                  //ͣ����2
						sendbuffer[25]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>24)&0xFF;                 //����ǰ���1
						sendbuffer[26]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>16)&0xFF; ;                 //����ǰ���1
						sendbuffer[27]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>8)&0xFF; ;                 //����ǰ���2
						sendbuffer[28]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney)&0xFF; ;                 //����ǰ���2
						sendbuffer[29]= 20;                 //���׿�ʼ��
						sendbuffer[30]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartYear;                 //���׿�ʼ��
						sendbuffer[31]= 0x00;                 //���׿�ʼ��
						sendbuffer[32]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMonth;                 //���׿�ʼ��
						sendbuffer[33]= 0x00;                 //���׿�ʼ��
						sendbuffer[34]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartDay;                 //���׿�ʼ��
						sendbuffer[35]= 0x00;                 //���׿�ʼ����
						sendbuffer[36]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartWeek;                 //���׿�ʼ����
						sendbuffer[37]= 0x00;                 //���׿�ʼʱ
						sendbuffer[38]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartHour;                 //���׿�ʼʱ
						sendbuffer[39]= 0x00;                 //���׿�ʼ��
						sendbuffer[40]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMin;                 //���׿�ʼ��
						sendbuffer[41]= 0x00;                 //���׿�ʼ��
						sendbuffer[42]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartSec;                 //���׿�ʼ��
						sendbuffer[43]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>24)&0xFF;                 //���׺����1            
						sendbuffer[44]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>16)&0xFF;                //���׺����1
						sendbuffer[45]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>8)&0xFF;                 //���׺����2
						sendbuffer[46]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney)&0xFF;                //���׺����2
						sendbuffer[47]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>24)&0xFF;                 //���׼�����1
						sendbuffer[48]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>16)&0xFF; ;                 //���׼�����1
						sendbuffer[49]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>8)&0xFF; ;                 //���׼�����2
						sendbuffer[50]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter)&0xFF; ;                 //���׼�����2
						sendbuffer[51]= 0x00;                 //���׮��1
						sendbuffer[52]= 0x00;                 //���׮��1
						sendbuffer[53]= 0x00;                 //���׮��2
						sendbuffer[54]= 0x00;                 //���׮��2
						sendbuffer[55]= 0x00;                  //���׽�����
						sendbuffer[56]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear;                  //���׽�����
						sendbuffer[57]= 0x00;                  //���׽�����
						sendbuffer[58]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth;                  //���׽�����
						sendbuffer[59]= 0x00;                  //���׽�����
						sendbuffer[60]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay;                  //���׽�����
						sendbuffer[61]= 0x00;                  //���׽�������
						sendbuffer[62]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek;                  //���׽�������
						sendbuffer[63]= 0x00;                  //���׽���ʱ
						sendbuffer[64]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour;                 //���׽���ʱ
						sendbuffer[65]= 0x00;                 //���׽�����
						sendbuffer[66]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin;                 //���׽�����
						sendbuffer[67]= 0x00;                 //���׿�ʼ��
						sendbuffer[68]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec;                 //���׿�ʼ��
						sendbuffer[69]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>24)&0xFF;                 //���汾��1
						sendbuffer[70]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>16)&0xFF;                 //���汾��1
						sendbuffer[71]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>8)&0xFF;                 //���汾��2
						sendbuffer[72]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer)&0xFF;                 //���汾��2
						sendbuffer[73]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>24)&0xFF;                 //POS����1
						sendbuffer[74]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>16)&0xFF;              //POS����1
						sendbuffer[75]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>8)&0xFF ;               //POS����2
						sendbuffer[76]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber)&0xFF;               //POS����2
						sendbuffer[77]= 0x00;                 //��״̬��
						sendbuffer[78]= 0x00;                 //��״̬��
						sendbuffer[79]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod>>8)&0xFF;;                 //��緽ʽ
						sendbuffer[80]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod)&0xFF;;                 //��緽ʽ
						sendbuffer[81]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate>>8)&0xFF ;                  //��������
						sendbuffer[82]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate)&0xFF ;                  //��������
						_crc=CalcCrc(sendbuffer,83);
						sendbuffer[83]=(uint8_t)(_crc&0xff);                 //CRC��
						sendbuffer[84]=(uint8_t)((_crc>>8)&0xff);                //CRC��
						TCP_Server_SendData(sendbuffer,85);
				}
}
void SendTradingSerialNumber(void)
{
			uint8_t sendbuffer[13];
			uint16_t _crc;
			sendbuffer[0]=ETHData.Data[0];  //�ӻ���ַ;
			sendbuffer[1]=ETHData.Data[1];
			sendbuffer[2]=ETHData.Data[2];
			sendbuffer[3]=ETHData.Data[3];
			sendbuffer[4]=ETHData.Data[4];
			sendbuffer[5]=ETHData.Data[5]; 
			sendbuffer[6]=ETHData.Data[6];      
			sendbuffer[7]=ETHData.Data[7];         //�Ĵ���ֵ1
			sendbuffer[8]=ETHData.Data[8];         //�Ĵ���ֵ1
			sendbuffer[9]=ETHData.Data[9];         //�Ĵ���ֵ2
			sendbuffer[10]=ETHData.Data[10];       //�Ĵ���ֵ2
	    ReadPriDealNo = (ETHData.Data[7]<<24)+(ETHData.Data[8]<<16)+(ETHData.Data[9]<<8)+(ETHData.Data[10]);
		  FlagIfReadPriDeal = 1;
			_crc=CalcCrc(sendbuffer,11);
			sendbuffer[11]=(uint8_t)(_crc&0xff);                 //CRC��
			sendbuffer[12]=(uint8_t)((_crc>>8)&0xff);;                 //CRC��
			TCP_Server_SendData(sendbuffer,13);
// 	    sendbuffer[0]=ETHData.Data[0];  //�ӻ���ַ;
// 			sendbuffer[1]=ETHData.Data[1];
// 			sendbuffer[2]=ETHData.Data[2];
// 			sendbuffer[3]=ETHData.Data[3];
// 			sendbuffer[4]=ETHData.Data[4];
// 			sendbuffer[5]=ETHData.Data[5]; 
// 			_crc=CalcCrc(sendbuffer,6);
// 			sendbuffer[11]=(uint8_t)(_crc&0xff);                 //CRC��
// 			sendbuffer[12]=(uint8_t)((_crc>>8)&0xff);;                 //CRC��
// 			TCP_Server_SendData(sendbuffer,8);
}

