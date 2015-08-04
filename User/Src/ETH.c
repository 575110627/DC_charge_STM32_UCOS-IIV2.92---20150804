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
		_crc=CalcCrc(ETHData.Data,(ETHData.Length-2));   //¼ÆËãÐ£ÑéCRC           
		a1=(uint8_t)(_crc&0xff);                          //CRCµÍ×Ö½Ú
		a2=(uint8_t)((_crc>>8)&0xff);                  //CRC¸ß×Ö½Ú
		if((a1 == ETHData.Data[ETHData.Length-2])&&(a2 == ETHData.Data[ETHData.Length-1]))		 //Ð£ÑéÕýÈ· 
		{	 			
// 				if(ETHData.Data[0] == 0xFF)  //¹ã²¥Ö¡
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
					case 0xFF:  //¹ã²¥Ö¡
								AnalyzeBroadcast(); 
				     break;
				  case 0x01:  //´Ó»úµØÖ·
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
				Broadcast();                                  //ÅÐ¶Ï¹¦ÄÜÂë
			}			
}

void Broadcast(void)
{
			uint16_t startadd;                     //ÆðÊ¼µØÖ·
			uint8_t startaddH,startaddL;           //ÆðÊ¼µØÖ·¸ßµØÎ» 
			startaddH=ETHData.Data[2];
			startaddL=ETHData.Data[3];
			startadd=(startaddH<<8)+startaddL;
			switch(startadd)
			{
				case 0x1100:
										ReceiveBroadcastTime();          //Ö÷»ú¶¨Ê±¹ã²¥¶ÔÊ±
								break;
				case 0x1210:
										ReceiveBroadcastSystemData1();   //Ö÷»ú¹ã²¥³äµç×®ÏµÍ³²ÎÊýÖ¡1
								break;
				case 0x1220:
										ReceiveBroadcastSystemData2();   //Ö÷»ú¹ã²¥³äµç×®ÏµÍ³²ÎÊýÖ¡2 
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
			 switch(ETHData.Data[1])                             //ÅÐ¶Ï¹¦ÄÜÂë
				{
					case 0x04:
								ReceiveFunction04();
							break;         
					case 0x10:
								ReceiveFunction10();
							break;
					default:
							break;           //²»Ö§³ÖµÄ¹¦ÄÜÂë	
				}
}
/*******´Ó»ú½ÓÊÕ¹ã²¥ºÚÃûµ¥*******/
void ReceiveBroadcastBlacklist(void)
{
				uint8_t recebuffer[20];
				recebuffer[0]  = ETHData.Data[7];                     //ºÚÃûµ¥1
				recebuffer[1]  = ETHData.Data[8];
				recebuffer[2]  = ETHData.Data[9];
				recebuffer[3]  = ETHData.Data[10];
				recebuffer[4]  = ETHData.Data[11];                    //ºÚÃûµ¥2
				recebuffer[5]  = ETHData.Data[12];
				recebuffer[6]  = ETHData.Data[13];
				recebuffer[7]  = ETHData.Data[14];
				recebuffer[8]  = ETHData.Data[15];                    //ºÚÃûµ¥3
				recebuffer[9]  = ETHData.Data[16];
				recebuffer[10] = ETHData.Data[17];
				recebuffer[11] = ETHData.Data[18];	
				recebuffer[12] = ETHData.Data[19];                   //ºÚÃûµ¥4
				recebuffer[13] = ETHData.Data[20];	
				recebuffer[14] = ETHData.Data[21];
				recebuffer[15] = ETHData.Data[22];
				recebuffer[16] = ETHData.Data[23];                   //ºÚÃûµ¥5
				recebuffer[17] = ETHData.Data[24];
				recebuffer[18] = ETHData.Data[25];
				recebuffer[19] = ETHData.Data[26];	
}
/*******´Ó»ú½ÓÊÕ¹ã²¥¶¨Ê±*******/
void ReceiveBroadcastTime(void)
{
			RTCSetTime.Year = ETHData.Data[8];              //Äê15
			RTCSetTime.Month = ETHData.Data[10];            //ÔÂ
			RTCSetTime.Day = ETHData.Data[12];              //ÈÕ
			RTCSetTime.Week = ETHData.Data[14];             //ÐÇÆÚ
			RTCSetTime.Hour = (ETHData.Data[16]|0x80);             //Ê±
			RTCSetTime.Minute = ETHData.Data[18];           //·Ö
			RTCSetTime.Second = ETHData.Data[20];           //Ãë	
			Set_Time(&RTCSetTime);
	    UpdateRTC();
}
/*******´Ó»ú¹ã²¥³äµç×®ÏµÍ³²ÎÊýÖ¡1******/
void ReceiveBroadcastSystemData1(void)
{
				uint8_t i;
				uint16_t crc=0xffff;
				unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice = (ETHData.Data[7]<<8)+ETHData.Data[8];              //Æ½¾ùµç¼Û
				unSystemParameterConfigHandle.SystemParameterConfigHandle.HPrice=(ETHData.Data[9]<<8)+ETHData.Data[10];              //·åÖµµç¼Û		
				unSystemParameterConfigHandle.SystemParameterConfigHandle.LPrice=(ETHData.Data[11]<<8)+ETHData.Data[12];             //¹ÈÖµµç¼Û
				unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStartH =(ETHData.Data[13]<<8)+ETHData.Data[14];             //·åÖµµç¼ÛÊ±¼ä ÆðÊ¼Ê±           
				unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStartM=(ETHData.Data[15]<<8)+ETHData.Data[16];              //·åÖµµç¼ÛÊ±¼ä ÆðÊ¼·Ö            
				unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStopH=(ETHData.Data[17]<<8)+ETHData.Data[18];            //·åÖµµç¼ÛÊ±¼ä ½áÊøÊ±           
				unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStopM=(ETHData.Data[19]<<8)+ETHData.Data[20];            //·åÖµµç¼ÛÊ±¼ä ½áÊø·Ö
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
/*******´Ó»ú¹ã²¥³äµç×®ÏµÍ³²ÎÊýÖ¡2******/
void ReceiveBroadcastSystemData2(void)
{
				uint8_t i;
				uint16_t crc=0xffff;
				unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStartH = (ETHData.Data[7]<<8)+ETHData.Data[8];         //¹ÈÖµµç¼ÛÊ±¼ä ÆðÊ¼Ê±
				unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStartM = (ETHData.Data[9]<<8)+ETHData.Data[10];         //¹ÈÖµµç¼ÛÊ±¼ä ÆðÊ¼·Ö
				unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStopH  = (ETHData.Data[11]<<8)+ETHData.Data[12];          //¹ÈÖµµç¼ÛÊ±¼ä ½áÊøÊ±
				unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStopM  = (ETHData.Data[13]<<8)+ETHData.Data[14];          //¹ÈÖµµç¼ÛÊ±¼ä ½áÊø·Ö
				unSystemParameterConfigHandle.SystemParameterConfigHandle.StartMode    = (ETHData.Data[15]<<8)+ETHData.Data[16]; 	      //Æô¶¯Ä£Ê½ 
				unSystemParameterConfigHandle.SystemParameterConfigHandle.BillMode=(ETHData.Data[17]<<8)+ETHData.Data[18];         //¼Æ·Ñ·½Ê½
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
			uint16_t startadd;               //ÆðÊ¼µØÖ·
			uint8_t startaddH,startaddL;     //ÆðÊ¼µØÖ·¸ßµØÎ»
			startaddH=ETHData.Data[2];
			startaddL=ETHData.Data[3];
			startadd=(startaddH<<8)+startaddL;
			switch(startadd)
			{
				case 0x1000:
										ReplyToHostPoSlaveStaframe();
							break;             //´Ó»úÏìÓ¦Ö÷»úÂÖÑ¯´Ó»ú×´Ì¬Ö¡
				case 0x1020:
										ReplyToHostReadTotalTranData1();
							break;         //´Ó»úÏìÓ¦Ö÷»ú¶ÁÈ¡Ò»´ÎÍêÕû½»Ò×Êý¾ÝÖ¡
				default:
							break;
			}	 	 
}
void ReceiveFunction10(void)
{
			uint16_t startadd;               //ÆðÊ¼µØÖ·
			uint8_t startaddH,startaddL;     //ÆðÊ¼µØÖ·¸ßµØÎ
			startaddH=ETHData.Data[2];
			startaddL=ETHData.Data[3];
			startadd=(startaddH<<8)+startaddL;
			if(startadd == 0x2050)
			{
						SendTradingSerialNumber();
			}		
}
/*******´Ó»úÏìÓ¦Ö÷»úÂÖÑ¯´Ó»ú×´Ì¬Ö¡*******/
void ReplyToHostPoSlaveStaframe(void)
{
			uint8_t sendbuffer[19];
			uint16_t _crc;
			uint16_t register_num =0;           //¼Ä´æÆ÷ÊýÁ¿
			uint8_t a1,a2;
			register_num = 2*((ETHData.Data[4]<<8)+ETHData.Data[5]);
			sendbuffer[0] = ETHData.Data[0];  //´Ó»úµØÖ·
			sendbuffer[1] = ETHData.Data[1];  //¹¦ÄÜÂë
			sendbuffer[2] = (uint8_t)(register_num);//0x0E;         //×Ö½ÚÊý
			sendbuffer[3] = 0x00;         //³äµç×®ºÅ1
			sendbuffer[4] = 0x00;         //³äµç×®ºÅ1
			sendbuffer[5] = 0x00;         //³äµç×®ºÅ2
			sendbuffer[6] = 0x01;         //³äµç×®ºÅ2
			sendbuffer[7] = 0x00;         //³äµç×®×´Ì¬
			sendbuffer[8] = 0x00;         //³äµç×®×´Ì¬
			sendbuffer[9] = 0x00;         //³äµç×®¹ÊÕÏÀàÐÍ          
			sendbuffer[10] = 0x00;        //³äµç×®¹ÊÕÏÀàÐÍ 
			sendbuffer[11] = 0x00;        //ÊÇ·ñ¶ÁÈ¡½»Ò×¼ÇÂ¼
			sendbuffer[12] = 0x55;        //ÊÇ·ñ¶ÁÈ¡½»Ò×¼ÇÂ¼
			sendbuffer[13] = 0x00;        //ÊÇ·ñ²É¼¯ÀúÊ·Êý¾Ý
			sendbuffer[14] = 0x55;        //ÊÇ·ñ²É¼¯ÀúÊ·Êý¾Ý
			sendbuffer[15] = 0x00;        //ÊÇ·ñ¶ÁÈ¡ºÚÃûµ¥¿¨ºÅ
			sendbuffer[16] = 0x55;        //ÊÇ·ñ¶ÁÈ¡ºÚÃûµ¥¿¨ºÅ
			_crc=CalcCrc(sendbuffer,register_num + 3);
			a1=(uint8_t)(_crc&0xff);                          //CRCµÍ×Ö½Ú
			a2=(uint8_t)((_crc>>8)&0xff);                     //CRC¸ß×Ö½Ú
			sendbuffer[17]=a1;                 //CRC¸ß
			sendbuffer[18]=a2;                 //CRCµÍ
			TCP_Server_SendData(sendbuffer,19);
}
/*******´Ó»úÏìÓ¦Ö÷»ú¶ÁÈ¡Ò»´ÎÍêÕû½»Ò×Êý¾ÝÖ¡1******/
void ReplyToHostReadTotalTranData1(void)
{
			uint8_t sendbuffer[85];  
			uint16_t _crc;
      //µ±Ç°½»Ò×¼ÇÂ¼
	    if(1 == FlagIfReadPriDeal)
			{
						FlagIfReadPriDeal = 0;
				    ReadHisRec(2,unSaveRealTimeDataHandle.unbuffer,73);
				    HAL_Delay(100);
				    HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
				    HAL_Delay(100);
     				sendbuffer[0] = ETHData.Data[0];
						sendbuffer[1] = ETHData.Data[1];
						sendbuffer[2] = 2*((ETHData.Data[4]<<8)+ETHData.Data[5]); //×Ö½ÚÊý                             //×Ö½ÚÊý
						sendbuffer[3] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType>>8)&0xFF;                    //½»Ò×ÀàÐÍ
						sendbuffer[4] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType)&0xFF ;                   //½»Ò×ÀàÐÍ
						sendbuffer[5] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>24)&0xFF;                   //½»Ò×Á÷Ë®ºÅ1gao
						sendbuffer[6] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>16)&0xFF;                   //½»Ò×Á÷Ë®ºÅ1di
						sendbuffer[7] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>8)&0xFF;                   //½»Ò×Á÷Ë®ºÅ2
						sendbuffer[8] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo)&0xFF;                    //½»Ò×Á÷Ë®ºÅ2
						sendbuffer[9] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber>>8)&0xFF;//middlebuffer[7];                   //µØÇø´úÂë
						sendbuffer[10]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber)&0xFF;//middlebuffer[8];                  //µØÇø´úÂë
						sendbuffer[11]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>24)&0XFF;                  //¿¨ºÅ1
						sendbuffer[12]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>16)&0XFF;                   //¿¨ºÅ1
						sendbuffer[13]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>8)&0XFF;                  //¿¨ºÅ2
						sendbuffer[14]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo)&0XFF;                  //¿¨ºÅ2
						sendbuffer[15]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType>>8)&0xFF;                 //¿¨ÐÍ
						sendbuffer[16]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType)&0xFF ;                  //¿¨ÐÍ
						sendbuffer[17]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy>>8)&0xFF;;                 //½»Ò×µçÁ¿
						sendbuffer[18]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy)&0xFF;                 //½»Ò×µçÁ¿
						sendbuffer[19]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice>>8)&0xFF;                //½»Ò×µç¼Û               
						sendbuffer[20]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice)&0xFF ;                 //½»Ò×µç¼Û
						sendbuffer[21]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>24)&0xFF;                 //Í£³µ·Ñ1
						sendbuffer[22]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>26)&0xFF;                  //Í£³µ·Ñ1
						sendbuffer[23]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>8)&0xFF;                 //Í£³µ·Ñ2
						sendbuffer[24]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee)&0xFF;                  //Í£³µ·Ñ2
						sendbuffer[25]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>24)&0xFF;                 //½»Ò×Ç°Óà¶î1
						sendbuffer[26]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>16)&0xFF; ;                 //½»Ò×Ç°Óà¶î1
						sendbuffer[27]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>8)&0xFF; ;                 //½»Ò×Ç°Óà¶î2
						sendbuffer[28]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney)&0xFF; ;                 //½»Ò×Ç°Óà¶î2
						sendbuffer[29]= 20;                 //½»Ò×¿ªÊ¼Äê
						sendbuffer[30]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartYear;                 //½»Ò×¿ªÊ¼Äê
						sendbuffer[31]= 0x00;                 //½»Ò×¿ªÊ¼ÔÂ
						sendbuffer[32]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMonth;                 //½»Ò×¿ªÊ¼ÔÂ
						sendbuffer[33]= 0x00;                 //½»Ò×¿ªÊ¼ÈÕ
						sendbuffer[34]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartDay;                 //½»Ò×¿ªÊ¼ÈÕ
						sendbuffer[35]= 0x00;                 //½»Ò×¿ªÊ¼ÐÇÆÚ
						sendbuffer[36]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartWeek;                 //½»Ò×¿ªÊ¼ÐÇÆÚ
						sendbuffer[37]= 0x00;                 //½»Ò×¿ªÊ¼Ê±
						sendbuffer[38]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartHour;                 //½»Ò×¿ªÊ¼Ê±
						sendbuffer[39]= 0x00;                 //½»Ò×¿ªÊ¼·Ö
						sendbuffer[40]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMin;                 //½»Ò×¿ªÊ¼·Ö
						sendbuffer[41]= 0x00;                 //½»Ò×¿ªÊ¼Ãë
						sendbuffer[42]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartSec;                 //½»Ò×¿ªÊ¼Ãë
						sendbuffer[43]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>24)&0xFF;                 //½»Ò×ºóÓà¶î1            
						sendbuffer[44]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>16)&0xFF;                //½»Ò×ºóÓà¶î1
						sendbuffer[45]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>8)&0xFF;                 //½»Ò×ºóÓà¶î2
						sendbuffer[46]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney)&0xFF;                //½»Ò×ºóÓà¶î2
						sendbuffer[47]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>24)&0xFF;                 //½»Ò×¼ÆÊýÆ÷1
						sendbuffer[48]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>16)&0xFF; ;                 //½»Ò×¼ÆÊýÆ÷1
						sendbuffer[49]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>8)&0xFF; ;                 //½»Ò×¼ÆÊýÆ÷2
						sendbuffer[50]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter)&0xFF; ;                 //½»Ò×¼ÆÊýÆ÷2
						sendbuffer[51]= 0x00;                 //³äµç×®ºÅ1
						sendbuffer[52]= 0x00;                 //³äµç×®ºÅ1
						sendbuffer[53]= 0x00;                 //³äµç×®ºÅ2
						sendbuffer[54]= 0x00;                 //³äµç×®ºÅ2
						sendbuffer[55]= 0x00;                  //½»Ò×½áÊøÄê
						sendbuffer[56]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear;                  //½»Ò×½áÊøÄê
						sendbuffer[57]= 0x00;                  //½»Ò×½áÊøÔÂ
						sendbuffer[58]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth;                  //½»Ò×½áÊøÔÂ
						sendbuffer[59]= 0x00;                  //½»Ò×½áÊøÈÕ
						sendbuffer[60]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay;                  //½»Ò×½áÊøÈÕ
						sendbuffer[61]= 0x00;                  //½»Ò×½áÊøÐÇÆÚ
						sendbuffer[62]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek;                  //½»Ò×½áÊøÐÇÆÚ
						sendbuffer[63]= 0x00;                  //½»Ò×½áÊøÊ±
						sendbuffer[64]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour;                 //½»Ò×½áÊøÊ±
						sendbuffer[65]= 0x00;                 //½»Ò×½áÊø·Ö
						sendbuffer[66]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin;                 //½»Ò×½áÊø·Ö
						sendbuffer[67]= 0x00;                 //½»Ò×¿ªÊ¼Ãë
						sendbuffer[68]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec;                 //½»Ò×¿ªÊ¼Ãë
						sendbuffer[69]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>24)&0xFF;                 //¿¨°æ±¾ºÅ1
						sendbuffer[70]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>16)&0xFF;                 //¿¨°æ±¾ºÅ1
						sendbuffer[71]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>8)&0xFF;                 //¿¨°æ±¾ºÅ2
						sendbuffer[72]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer)&0xFF;                 //¿¨°æ±¾ºÅ2
						sendbuffer[73]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>24)&0xFF;                 //POS»úºÅ1
						sendbuffer[74]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>16)&0xFF;              //POS»úºÅ1
						sendbuffer[75]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>8)&0xFF ;               //POS»úºÅ2
						sendbuffer[76]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber)&0xFF;               //POS»úºÅ2
						sendbuffer[77]= 0x00;                 //¿¨×´Ì¬Âë
						sendbuffer[78]= 0x00;                 //¿¨×´Ì¬Âë
						sendbuffer[79]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod>>8)&0xFF;;                 //³äµç·½Ê½
						sendbuffer[80]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod)&0xFF;;                 //³äµç·½Ê½
						sendbuffer[81]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate>>8)&0xFF ;                  //µçÁ¿±¶ÂÊ
						sendbuffer[82]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate)&0xFF ;                  //µçÁ¿±¶ÂÊ
						_crc=CalcCrc(sendbuffer,83);
						sendbuffer[83]=(uint8_t)(_crc&0xff);                 //CRC¸ß
						sendbuffer[84]=(uint8_t)((_crc>>8)&0xff);                //CRCµÍ
						TCP_Server_SendData(sendbuffer,85);
			}
			else
			{
						sendbuffer[0] = ETHData.Data[0];
						sendbuffer[1] = ETHData.Data[1];
						sendbuffer[2] = 2*((ETHData.Data[4]<<8)+ETHData.Data[5]); //×Ö½ÚÊý                             //×Ö½ÚÊý
						sendbuffer[3] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType>>8)&0xFF;                    //½»Ò×ÀàÐÍ
						sendbuffer[4] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType)&0xFF ;                   //½»Ò×ÀàÐÍ
						sendbuffer[5] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>24)&0xFF;                   //½»Ò×Á÷Ë®ºÅ1gao
						sendbuffer[6] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>16)&0xFF;                   //½»Ò×Á÷Ë®ºÅ1di
						sendbuffer[7] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>8)&0xFF;                   //½»Ò×Á÷Ë®ºÅ2
						sendbuffer[8] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo)&0xFF;                    //½»Ò×Á÷Ë®ºÅ2
						sendbuffer[9] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber>>8)&0xFF;//middlebuffer[7];                   //µØÇø´úÂë
						sendbuffer[10]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber)&0xFF;//middlebuffer[8];                  //µØÇø´úÂë
						sendbuffer[11]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>24)&0XFF;                  //¿¨ºÅ1
						sendbuffer[12]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>16)&0XFF;                   //¿¨ºÅ1
						sendbuffer[13]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>8)&0XFF;                  //¿¨ºÅ2
						sendbuffer[14]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo)&0XFF;                  //¿¨ºÅ2
						sendbuffer[15]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType>>8)&0xFF;                 //¿¨ÐÍ
						sendbuffer[16]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType)&0xFF ;                  //¿¨ÐÍ
						sendbuffer[17]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy>>8)&0xFF;;                 //½»Ò×µçÁ¿
						sendbuffer[18]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy)&0xFF;                 //½»Ò×µçÁ¿
						sendbuffer[19]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice>>8)&0xFF;                //½»Ò×µç¼Û               
						sendbuffer[20]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice)&0xFF ;                 //½»Ò×µç¼Û
						sendbuffer[21]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>24)&0xFF;                 //Í£³µ·Ñ1
						sendbuffer[22]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>26)&0xFF;                  //Í£³µ·Ñ1
						sendbuffer[23]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>8)&0xFF;                 //Í£³µ·Ñ2
						sendbuffer[24]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee)&0xFF;                  //Í£³µ·Ñ2
						sendbuffer[25]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>24)&0xFF;                 //½»Ò×Ç°Óà¶î1
						sendbuffer[26]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>16)&0xFF; ;                 //½»Ò×Ç°Óà¶î1
						sendbuffer[27]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>8)&0xFF; ;                 //½»Ò×Ç°Óà¶î2
						sendbuffer[28]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney)&0xFF; ;                 //½»Ò×Ç°Óà¶î2
						sendbuffer[29]= 20;                 //½»Ò×¿ªÊ¼Äê
						sendbuffer[30]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartYear;                 //½»Ò×¿ªÊ¼Äê
						sendbuffer[31]= 0x00;                 //½»Ò×¿ªÊ¼ÔÂ
						sendbuffer[32]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMonth;                 //½»Ò×¿ªÊ¼ÔÂ
						sendbuffer[33]= 0x00;                 //½»Ò×¿ªÊ¼ÈÕ
						sendbuffer[34]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartDay;                 //½»Ò×¿ªÊ¼ÈÕ
						sendbuffer[35]= 0x00;                 //½»Ò×¿ªÊ¼ÐÇÆÚ
						sendbuffer[36]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartWeek;                 //½»Ò×¿ªÊ¼ÐÇÆÚ
						sendbuffer[37]= 0x00;                 //½»Ò×¿ªÊ¼Ê±
						sendbuffer[38]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartHour;                 //½»Ò×¿ªÊ¼Ê±
						sendbuffer[39]= 0x00;                 //½»Ò×¿ªÊ¼·Ö
						sendbuffer[40]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMin;                 //½»Ò×¿ªÊ¼·Ö
						sendbuffer[41]= 0x00;                 //½»Ò×¿ªÊ¼Ãë
						sendbuffer[42]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartSec;                 //½»Ò×¿ªÊ¼Ãë
						sendbuffer[43]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>24)&0xFF;                 //½»Ò×ºóÓà¶î1            
						sendbuffer[44]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>16)&0xFF;                //½»Ò×ºóÓà¶î1
						sendbuffer[45]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>8)&0xFF;                 //½»Ò×ºóÓà¶î2
						sendbuffer[46]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney)&0xFF;                //½»Ò×ºóÓà¶î2
						sendbuffer[47]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>24)&0xFF;                 //½»Ò×¼ÆÊýÆ÷1
						sendbuffer[48]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>16)&0xFF; ;                 //½»Ò×¼ÆÊýÆ÷1
						sendbuffer[49]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>8)&0xFF; ;                 //½»Ò×¼ÆÊýÆ÷2
						sendbuffer[50]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter)&0xFF; ;                 //½»Ò×¼ÆÊýÆ÷2
						sendbuffer[51]= 0x00;                 //³äµç×®ºÅ1
						sendbuffer[52]= 0x00;                 //³äµç×®ºÅ1
						sendbuffer[53]= 0x00;                 //³äµç×®ºÅ2
						sendbuffer[54]= 0x00;                 //³äµç×®ºÅ2
						sendbuffer[55]= 0x00;                  //½»Ò×½áÊøÄê
						sendbuffer[56]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear;                  //½»Ò×½áÊøÄê
						sendbuffer[57]= 0x00;                  //½»Ò×½áÊøÔÂ
						sendbuffer[58]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth;                  //½»Ò×½áÊøÔÂ
						sendbuffer[59]= 0x00;                  //½»Ò×½áÊøÈÕ
						sendbuffer[60]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay;                  //½»Ò×½áÊøÈÕ
						sendbuffer[61]= 0x00;                  //½»Ò×½áÊøÐÇÆÚ
						sendbuffer[62]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek;                  //½»Ò×½áÊøÐÇÆÚ
						sendbuffer[63]= 0x00;                  //½»Ò×½áÊøÊ±
						sendbuffer[64]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour;                 //½»Ò×½áÊøÊ±
						sendbuffer[65]= 0x00;                 //½»Ò×½áÊø·Ö
						sendbuffer[66]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin;                 //½»Ò×½áÊø·Ö
						sendbuffer[67]= 0x00;                 //½»Ò×¿ªÊ¼Ãë
						sendbuffer[68]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec;                 //½»Ò×¿ªÊ¼Ãë
						sendbuffer[69]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>24)&0xFF;                 //¿¨°æ±¾ºÅ1
						sendbuffer[70]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>16)&0xFF;                 //¿¨°æ±¾ºÅ1
						sendbuffer[71]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>8)&0xFF;                 //¿¨°æ±¾ºÅ2
						sendbuffer[72]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer)&0xFF;                 //¿¨°æ±¾ºÅ2
						sendbuffer[73]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>24)&0xFF;                 //POS»úºÅ1
						sendbuffer[74]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>16)&0xFF;              //POS»úºÅ1
						sendbuffer[75]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>8)&0xFF ;               //POS»úºÅ2
						sendbuffer[76]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber)&0xFF;               //POS»úºÅ2
						sendbuffer[77]= 0x00;                 //¿¨×´Ì¬Âë
						sendbuffer[78]= 0x00;                 //¿¨×´Ì¬Âë
						sendbuffer[79]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod>>8)&0xFF;;                 //³äµç·½Ê½
						sendbuffer[80]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod)&0xFF;;                 //³äµç·½Ê½
						sendbuffer[81]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate>>8)&0xFF ;                  //µçÁ¿±¶ÂÊ
						sendbuffer[82]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate)&0xFF ;                  //µçÁ¿±¶ÂÊ
						_crc=CalcCrc(sendbuffer,83);
						sendbuffer[83]=(uint8_t)(_crc&0xff);                 //CRC¸ß
						sendbuffer[84]=(uint8_t)((_crc>>8)&0xff);                //CRCµÍ
						TCP_Server_SendData(sendbuffer,85);
				}
}
void SendTradingSerialNumber(void)
{
			uint8_t sendbuffer[13];
			uint16_t _crc;
			sendbuffer[0]=ETHData.Data[0];  //´Ó»úµØÖ·;
			sendbuffer[1]=ETHData.Data[1];
			sendbuffer[2]=ETHData.Data[2];
			sendbuffer[3]=ETHData.Data[3];
			sendbuffer[4]=ETHData.Data[4];
			sendbuffer[5]=ETHData.Data[5]; 
			sendbuffer[6]=ETHData.Data[6];      
			sendbuffer[7]=ETHData.Data[7];         //¼Ä´æÆ÷Öµ1
			sendbuffer[8]=ETHData.Data[8];         //¼Ä´æÆ÷Öµ1
			sendbuffer[9]=ETHData.Data[9];         //¼Ä´æÆ÷Öµ2
			sendbuffer[10]=ETHData.Data[10];       //¼Ä´æÆ÷Öµ2
	    ReadPriDealNo = (ETHData.Data[7]<<24)+(ETHData.Data[8]<<16)+(ETHData.Data[9]<<8)+(ETHData.Data[10]);
		  FlagIfReadPriDeal = 1;
			_crc=CalcCrc(sendbuffer,11);
			sendbuffer[11]=(uint8_t)(_crc&0xff);                 //CRC¸ß
			sendbuffer[12]=(uint8_t)((_crc>>8)&0xff);;                 //CRCµÍ
			TCP_Server_SendData(sendbuffer,13);
// 	    sendbuffer[0]=ETHData.Data[0];  //´Ó»úµØÖ·;
// 			sendbuffer[1]=ETHData.Data[1];
// 			sendbuffer[2]=ETHData.Data[2];
// 			sendbuffer[3]=ETHData.Data[3];
// 			sendbuffer[4]=ETHData.Data[4];
// 			sendbuffer[5]=ETHData.Data[5]; 
// 			_crc=CalcCrc(sendbuffer,6);
// 			sendbuffer[11]=(uint8_t)(_crc&0xff);                 //CRC¸ß
// 			sendbuffer[12]=(uint8_t)((_crc>>8)&0xff);;                 //CRCµÍ
// 			TCP_Server_SendData(sendbuffer,8);
}

