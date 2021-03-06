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
		_crc=CalcCrc(ETHData.Data,(ETHData.Length-2));   //计算校验CRC           
		a1=(uint8_t)(_crc&0xff);                          //CRC低字节
		a2=(uint8_t)((_crc>>8)&0xff);                  //CRC高字节
		if((a1 == ETHData.Data[ETHData.Length-2])&&(a2 == ETHData.Data[ETHData.Length-1]))		 //校验正确 
		{	 			
// 				if(ETHData.Data[0] == 0xFF)  //广播帧
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
					case 0xFF:  //广播帧
								AnalyzeBroadcast(); 
				     break;
				  case 0x01:  //从机地址
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
				Broadcast();                                  //判断功能码
			}			
}

void Broadcast(void)
{
			uint16_t startadd;                     //起始地址
			uint8_t startaddH,startaddL;           //起始地址高地位 
			startaddH=ETHData.Data[2];
			startaddL=ETHData.Data[3];
			startadd=(startaddH<<8)+startaddL;
			switch(startadd)
			{
				case 0x1100:
										ReceiveBroadcastTime();          //主机定时广播对时
								break;
				case 0x1210:
										ReceiveBroadcastSystemData1();   //主机广播充电桩系统参数帧1
								break;
				case 0x1220:
										ReceiveBroadcastSystemData2();   //主机广播充电桩系统参数帧2 
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
			 switch(ETHData.Data[1])                             //判断功能码
				{
					case 0x04:
								ReceiveFunction04();
							break;         
					case 0x10:
								ReceiveFunction10();
							break;
					default:
							break;           //不支持的功能码	
				}
}
/*******从机接收广播黑名单*******/
void ReceiveBroadcastBlacklist(void)
{
    /*
    uint8_t recebuffer[20];
    recebuffer[0]  = ETHData.Data[7];                     //黑名单1
    recebuffer[1]  = ETHData.Data[8];
    recebuffer[2]  = ETHData.Data[9];
    recebuffer[3]  = ETHData.Data[10];
    recebuffer[4]  = ETHData.Data[11];                    //黑名单2
    recebuffer[5]  = ETHData.Data[12];
    recebuffer[6]  = ETHData.Data[13];
    recebuffer[7]  = ETHData.Data[14];
    recebuffer[8]  = ETHData.Data[15];                    //黑名单3
    recebuffer[9]  = ETHData.Data[16];
    recebuffer[10] = ETHData.Data[17];
    recebuffer[11] = ETHData.Data[18];	
    recebuffer[12] = ETHData.Data[19];                   //黑名单4
    recebuffer[13] = ETHData.Data[20];	
    recebuffer[14] = ETHData.Data[21];
    recebuffer[15] = ETHData.Data[22];
    recebuffer[16] = ETHData.Data[23];                   //黑名单5
    recebuffer[17] = ETHData.Data[24];
    recebuffer[18] = ETHData.Data[25];
    recebuffer[19] = ETHData.Data[26];
*/	
}
/*******从机接收广播定时*******/
void ReceiveBroadcastTime(void)
{
    RTCSetTime.Year = ETHData.Data[8];              //年15
    RTCSetTime.Month = ETHData.Data[10];            //月
    RTCSetTime.Day = ETHData.Data[12];              //日
    RTCSetTime.Week = ETHData.Data[14];             //星期
    RTCSetTime.Hour = (ETHData.Data[16]|0x80);             //时
    RTCSetTime.Minute = ETHData.Data[18];           //分
    RTCSetTime.Second = ETHData.Data[20];           //秒	
    Set_Time(&RTCSetTime);
	UpdateRTC();
}
/*******从机广播充电桩系统参数帧1******/
void ReceiveBroadcastSystemData1(void)
{
    uint8_t i;
    uint16_t crc=0xffff;
    unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice = (ETHData.Data[7]<<8)+ETHData.Data[8];              //平均电价
    unSystemParameterConfigHandle.SystemParameterConfigHandle.HPrice=(ETHData.Data[9]<<8)+ETHData.Data[10];              //峰值电价		
    unSystemParameterConfigHandle.SystemParameterConfigHandle.LPrice=(ETHData.Data[11]<<8)+ETHData.Data[12];             //谷值电价
    unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStartH =(ETHData.Data[13]<<8)+ETHData.Data[14];             //峰值电价时间 起始时           
    unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStartM=(ETHData.Data[15]<<8)+ETHData.Data[16];              //峰值电价时间 起始分            
    unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStopH=(ETHData.Data[17]<<8)+ETHData.Data[18];            //峰值电价时间 结束时           
    unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStopM=(ETHData.Data[19]<<8)+ETHData.Data[20];            //峰值电价时间 结束分
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
/*******从机广播充电桩系统参数帧2******/
void ReceiveBroadcastSystemData2(void)
{
    uint8_t i;
    uint16_t crc=0xffff;
    unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStartH = (ETHData.Data[7]<<8)+ETHData.Data[8];         //谷值电价时间 起始时
    unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStartM = (ETHData.Data[9]<<8)+ETHData.Data[10];         //谷值电价时间 起始分
    unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStopH  = (ETHData.Data[11]<<8)+ETHData.Data[12];          //谷值电价时间 结束时
    unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStopM  = (ETHData.Data[13]<<8)+ETHData.Data[14];          //谷值电价时间 结束分
    unSystemParameterConfigHandle.SystemParameterConfigHandle.StartMode    = (ETHData.Data[15]<<8)+ETHData.Data[16]; 	      //启动模式 
    unSystemParameterConfigHandle.SystemParameterConfigHandle.BillMode=(ETHData.Data[17]<<8)+ETHData.Data[18];         //计费方式
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
    uint16_t startadd;               //起始地址
    uint8_t startaddH,startaddL;     //起始地址高地位
    startaddH=ETHData.Data[2];
    startaddL=ETHData.Data[3];
    startadd=(startaddH<<8)+startaddL;
    switch(startadd)
    {
        case 0x1000:
                                ReplyToHostPoSlaveStaframe();
                    break;             //从机响应主机轮询从机状态帧
        case 0x1020:
                                ReplyToHostReadTotalTranData1();
                    break;         //从机响应主机读取一次完整交易数据帧
        default:
                    break;
    }	 	 
}
void ReceiveFunction10(void)
{
    uint16_t startadd;               //起始地址
    uint8_t startaddH,startaddL;     //起始地址高地�
    startaddH=ETHData.Data[2];
    startaddL=ETHData.Data[3];
    startadd=(startaddH<<8)+startaddL;
    if(startadd == 0x2050)
    {
        SendTradingSerialNumber();
    }		
}
/*******从机响应主机轮询从机状态帧*******/
void ReplyToHostPoSlaveStaframe(void)
{
			uint8_t sendbuffer[19];
			uint16_t _crc;
			uint16_t register_num =0;           //寄存器数量
			uint8_t a1,a2;
			register_num = 2*((ETHData.Data[4]<<8)+ETHData.Data[5]);
			sendbuffer[0] = ETHData.Data[0];  //从机地址
			sendbuffer[1] = ETHData.Data[1];  //功能码
			sendbuffer[2] = (uint8_t)(register_num);//0x0E;         //字节数
			sendbuffer[3] = 0x00;         //充电桩号1
			sendbuffer[4] = 0x00;         //充电桩号1
			sendbuffer[5] = 0x00;         //充电桩号2
			sendbuffer[6] = 0x01;         //充电桩号2
			sendbuffer[7] = 0x00;         //充电桩状态
			sendbuffer[8] = 0x00;         //充电桩状态
			sendbuffer[9] = 0x00;         //充电桩故障类型          
			sendbuffer[10] = 0x00;        //充电桩故障类型 
			sendbuffer[11] = 0x00;        //是否读取交易记录
			sendbuffer[12] = 0x55;        //是否读取交易记录
			sendbuffer[13] = 0x00;        //是否采集历史数据
			sendbuffer[14] = 0x55;        //是否采集历史数据
			sendbuffer[15] = 0x00;        //是否读取黑名单卡号
			sendbuffer[16] = 0x55;        //是否读取黑名单卡号
			_crc=CalcCrc(sendbuffer,register_num + 3);
			a1=(uint8_t)(_crc&0xff);                          //CRC低字节
			a2=(uint8_t)((_crc>>8)&0xff);                     //CRC高字节
			sendbuffer[17]=a1;                 //CRC高
			sendbuffer[18]=a2;                 //CRC低
			TCP_Server_SendData(sendbuffer,19);
}
/*******从机响应主机读取一次完整交易数据帧1******/
void ReplyToHostReadTotalTranData1(void)
{
			uint8_t sendbuffer[85];  
			uint16_t _crc;
      //当前交易记录
	    if(1 == FlagIfReadPriDeal)
			{
						FlagIfReadPriDeal = 0;
				    ReadHisRec(2,unSaveRealTimeDataHandle.unbuffer,73);
				    HAL_Delay(100);
				    HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
				    HAL_Delay(100);
     				sendbuffer[0] = ETHData.Data[0];
						sendbuffer[1] = ETHData.Data[1];
						sendbuffer[2] = 2*((ETHData.Data[4]<<8)+ETHData.Data[5]); //字节数                             //字节数
						sendbuffer[3] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType>>8)&0xFF;                    //交易类型
						sendbuffer[4] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType)&0xFF ;                   //交易类型
						sendbuffer[5] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>24)&0xFF;                   //交易流水号1gao
						sendbuffer[6] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>16)&0xFF;                   //交易流水号1di
						sendbuffer[7] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>8)&0xFF;                   //交易流水号2
						sendbuffer[8] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo)&0xFF;                    //交易流水号2
						sendbuffer[9] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber>>8)&0xFF;//middlebuffer[7];                   //地区代码
						sendbuffer[10]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber)&0xFF;//middlebuffer[8];                  //地区代码
						sendbuffer[11]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>24)&0XFF;                  //卡号1
						sendbuffer[12]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>16)&0XFF;                   //卡号1
						sendbuffer[13]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>8)&0XFF;                  //卡号2
						sendbuffer[14]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo)&0XFF;                  //卡号2
						sendbuffer[15]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType>>8)&0xFF;                 //卡型
						sendbuffer[16]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType)&0xFF ;                  //卡型
						sendbuffer[17]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy>>8)&0xFF;;                 //交易电量
						sendbuffer[18]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy)&0xFF;                 //交易电量
						sendbuffer[19]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice>>8)&0xFF;                //交易电价               
						sendbuffer[20]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice)&0xFF ;                 //交易电价
						sendbuffer[21]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>24)&0xFF;                 //停车费1
						sendbuffer[22]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>26)&0xFF;                  //停车费1
						sendbuffer[23]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>8)&0xFF;                 //停车费2
						sendbuffer[24]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee)&0xFF;                  //停车费2
						sendbuffer[25]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>24)&0xFF;                 //交易前余额1
						sendbuffer[26]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>16)&0xFF; ;                 //交易前余额1
						sendbuffer[27]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>8)&0xFF; ;                 //交易前余额2
						sendbuffer[28]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney)&0xFF; ;                 //交易前余额2
						sendbuffer[29]= 20;                 //交易开始年
						sendbuffer[30]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartYear;                 //交易开始年
						sendbuffer[31]= 0x00;                 //交易开始月
						sendbuffer[32]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMonth;                 //交易开始月
						sendbuffer[33]= 0x00;                 //交易开始日
						sendbuffer[34]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartDay;                 //交易开始日
						sendbuffer[35]= 0x00;                 //交易开始星期
						sendbuffer[36]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartWeek;                 //交易开始星期
						sendbuffer[37]= 0x00;                 //交易开始时
						sendbuffer[38]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartHour;                 //交易开始时
						sendbuffer[39]= 0x00;                 //交易开始分
						sendbuffer[40]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMin;                 //交易开始分
						sendbuffer[41]= 0x00;                 //交易开始秒
						sendbuffer[42]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartSec;                 //交易开始秒
						sendbuffer[43]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>24)&0xFF;                 //交易后余额1            
						sendbuffer[44]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>16)&0xFF;                //交易后余额1
						sendbuffer[45]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>8)&0xFF;                 //交易后余额2
						sendbuffer[46]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney)&0xFF;                //交易后余额2
						sendbuffer[47]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>24)&0xFF;                 //交易计数器1
						sendbuffer[48]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>16)&0xFF; ;                 //交易计数器1
						sendbuffer[49]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>8)&0xFF; ;                 //交易计数器2
						sendbuffer[50]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter)&0xFF; ;                 //交易计数器2
						sendbuffer[51]= 0x00;                 //充电桩号1
						sendbuffer[52]= 0x00;                 //充电桩号1
						sendbuffer[53]= 0x00;                 //充电桩号2
						sendbuffer[54]= 0x00;                 //充电桩号2
						sendbuffer[55]= 0x00;                  //交易结束年
						sendbuffer[56]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear;                  //交易结束年
						sendbuffer[57]= 0x00;                  //交易结束月
						sendbuffer[58]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth;                  //交易结束月
						sendbuffer[59]= 0x00;                  //交易结束日
						sendbuffer[60]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay;                  //交易结束日
						sendbuffer[61]= 0x00;                  //交易结束星期
						sendbuffer[62]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek;                  //交易结束星期
						sendbuffer[63]= 0x00;                  //交易结束时
						sendbuffer[64]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour;                 //交易结束时
						sendbuffer[65]= 0x00;                 //交易结束分
						sendbuffer[66]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin;                 //交易结束分
						sendbuffer[67]= 0x00;                 //交易开始秒
						sendbuffer[68]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec;                 //交易开始秒
						sendbuffer[69]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>24)&0xFF;                 //卡版本号1
						sendbuffer[70]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>16)&0xFF;                 //卡版本号1
						sendbuffer[71]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>8)&0xFF;                 //卡版本号2
						sendbuffer[72]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer)&0xFF;                 //卡版本号2
						sendbuffer[73]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>24)&0xFF;                 //POS机号1
						sendbuffer[74]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>16)&0xFF;              //POS机号1
						sendbuffer[75]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>8)&0xFF ;               //POS机号2
						sendbuffer[76]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber)&0xFF;               //POS机号2
						sendbuffer[77]= 0x00;                 //卡状态码
						sendbuffer[78]= 0x00;                 //卡状态码
						sendbuffer[79]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod>>8)&0xFF;;                 //充电方式
						sendbuffer[80]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod)&0xFF;;                 //充电方式
						sendbuffer[81]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate>>8)&0xFF ;                  //电量倍率
						sendbuffer[82]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate)&0xFF ;                  //电量倍率
						_crc=CalcCrc(sendbuffer,83);
						sendbuffer[83]=(uint8_t)(_crc&0xff);                 //CRC高
						sendbuffer[84]=(uint8_t)((_crc>>8)&0xff);                //CRC低
						TCP_Server_SendData(sendbuffer,85);
			}
			else
			{
						sendbuffer[0] = ETHData.Data[0];
						sendbuffer[1] = ETHData.Data[1];
						sendbuffer[2] = 2*((ETHData.Data[4]<<8)+ETHData.Data[5]); //字节数                             //字节数
						sendbuffer[3] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType>>8)&0xFF;                    //交易类型
						sendbuffer[4] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType)&0xFF ;                   //交易类型
						sendbuffer[5] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>24)&0xFF;                   //交易流水号1gao
						sendbuffer[6] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>16)&0xFF;                   //交易流水号1di
						sendbuffer[7] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo>>8)&0xFF;                   //交易流水号2
						sendbuffer[8] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo)&0xFF;                    //交易流水号2
						sendbuffer[9] = (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber>>8)&0xFF;//middlebuffer[7];                   //地区代码
						sendbuffer[10]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber)&0xFF;//middlebuffer[8];                  //地区代码
						sendbuffer[11]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>24)&0XFF;                  //卡号1
						sendbuffer[12]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>16)&0XFF;                   //卡号1
						sendbuffer[13]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo>>8)&0XFF;                  //卡号2
						sendbuffer[14]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo)&0XFF;                  //卡号2
						sendbuffer[15]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType>>8)&0xFF;                 //卡型
						sendbuffer[16]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType)&0xFF ;                  //卡型
						sendbuffer[17]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy>>8)&0xFF;;                 //交易电量
						sendbuffer[18]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy)&0xFF;                 //交易电量
						sendbuffer[19]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice>>8)&0xFF;                //交易电价               
						sendbuffer[20]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice)&0xFF ;                 //交易电价
						sendbuffer[21]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>24)&0xFF;                 //停车费1
						sendbuffer[22]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>26)&0xFF;                  //停车费1
						sendbuffer[23]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee>>8)&0xFF;                 //停车费2
						sendbuffer[24]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee)&0xFF;                  //停车费2
						sendbuffer[25]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>24)&0xFF;                 //交易前余额1
						sendbuffer[26]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>16)&0xFF; ;                 //交易前余额1
						sendbuffer[27]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney>>8)&0xFF; ;                 //交易前余额2
						sendbuffer[28]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney)&0xFF; ;                 //交易前余额2
						sendbuffer[29]= 20;                 //交易开始年
						sendbuffer[30]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartYear;                 //交易开始年
						sendbuffer[31]= 0x00;                 //交易开始月
						sendbuffer[32]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMonth;                 //交易开始月
						sendbuffer[33]= 0x00;                 //交易开始日
						sendbuffer[34]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartDay;                 //交易开始日
						sendbuffer[35]= 0x00;                 //交易开始星期
						sendbuffer[36]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartWeek;                 //交易开始星期
						sendbuffer[37]= 0x00;                 //交易开始时
						sendbuffer[38]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartHour;                 //交易开始时
						sendbuffer[39]= 0x00;                 //交易开始分
						sendbuffer[40]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMin;                 //交易开始分
						sendbuffer[41]= 0x00;                 //交易开始秒
						sendbuffer[42]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartSec;                 //交易开始秒
						sendbuffer[43]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>24)&0xFF;                 //交易后余额1            
						sendbuffer[44]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>16)&0xFF;                //交易后余额1
						sendbuffer[45]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney>>8)&0xFF;                 //交易后余额2
						sendbuffer[46]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney)&0xFF;                //交易后余额2
						sendbuffer[47]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>24)&0xFF;                 //交易计数器1
						sendbuffer[48]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>16)&0xFF; ;                 //交易计数器1
						sendbuffer[49]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter>>8)&0xFF; ;                 //交易计数器2
						sendbuffer[50]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter)&0xFF; ;                 //交易计数器2
						sendbuffer[51]= 0x00;                 //充电桩号1
						sendbuffer[52]= 0x00;                 //充电桩号1
						sendbuffer[53]= 0x00;                 //充电桩号2
						sendbuffer[54]= 0x00;                 //充电桩号2
						sendbuffer[55]= 0x00;                  //交易结束年
						sendbuffer[56]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear;                  //交易结束年
						sendbuffer[57]= 0x00;                  //交易结束月
						sendbuffer[58]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth;                  //交易结束月
						sendbuffer[59]= 0x00;                  //交易结束日
						sendbuffer[60]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay;                  //交易结束日
						sendbuffer[61]= 0x00;                  //交易结束星期
						sendbuffer[62]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek;                  //交易结束星期
						sendbuffer[63]= 0x00;                  //交易结束时
						sendbuffer[64]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour;                 //交易结束时
						sendbuffer[65]= 0x00;                 //交易结束分
						sendbuffer[66]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin;                 //交易结束分
						sendbuffer[67]= 0x00;                 //交易开始秒
						sendbuffer[68]= unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec;                 //交易开始秒
						sendbuffer[69]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>24)&0xFF;                 //卡版本号1
						sendbuffer[70]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>16)&0xFF;                 //卡版本号1
						sendbuffer[71]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer>>8)&0xFF;                 //卡版本号2
						sendbuffer[72]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer)&0xFF;                 //卡版本号2
						sendbuffer[73]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>24)&0xFF;                 //POS机号1
						sendbuffer[74]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>16)&0xFF;              //POS机号1
						sendbuffer[75]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber>>8)&0xFF ;               //POS机号2
						sendbuffer[76]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber)&0xFF;               //POS机号2
						sendbuffer[77]= 0x00;                 //卡状态码
						sendbuffer[78]= 0x00;                 //卡状态码
						sendbuffer[79]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod>>8)&0xFF;;                 //充电方式
						sendbuffer[80]= (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod)&0xFF;;                 //充电方式
						sendbuffer[81]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate>>8)&0xFF ;                  //电量倍率
						sendbuffer[82]= (unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate)&0xFF ;                  //电量倍率
						_crc=CalcCrc(sendbuffer,83);
						sendbuffer[83]=(uint8_t)(_crc&0xff);                 //CRC高
						sendbuffer[84]=(uint8_t)((_crc>>8)&0xff);                //CRC低
						TCP_Server_SendData(sendbuffer,85);
				}
}
void SendTradingSerialNumber(void)
{
			uint8_t sendbuffer[13];
			uint16_t _crc;
			sendbuffer[0]=ETHData.Data[0];  //从机地址;
			sendbuffer[1]=ETHData.Data[1];
			sendbuffer[2]=ETHData.Data[2];
			sendbuffer[3]=ETHData.Data[3];
			sendbuffer[4]=ETHData.Data[4];
			sendbuffer[5]=ETHData.Data[5]; 
			sendbuffer[6]=ETHData.Data[6];      
			sendbuffer[7]=ETHData.Data[7];         //寄存器值1
			sendbuffer[8]=ETHData.Data[8];         //寄存器值1
			sendbuffer[9]=ETHData.Data[9];         //寄存器值2
			sendbuffer[10]=ETHData.Data[10];       //寄存器值2
	    ReadPriDealNo = (ETHData.Data[7]<<24)+(ETHData.Data[8]<<16)+(ETHData.Data[9]<<8)+(ETHData.Data[10]);
		  FlagIfReadPriDeal = 1;
			_crc=CalcCrc(sendbuffer,11);
			sendbuffer[11]=(uint8_t)(_crc&0xff);                 //CRC高
			sendbuffer[12]=(uint8_t)((_crc>>8)&0xff);;                 //CRC低
			TCP_Server_SendData(sendbuffer,13);
// 	    sendbuffer[0]=ETHData.Data[0];  //从机地址;
// 			sendbuffer[1]=ETHData.Data[1];
// 			sendbuffer[2]=ETHData.Data[2];
// 			sendbuffer[3]=ETHData.Data[3];
// 			sendbuffer[4]=ETHData.Data[4];
// 			sendbuffer[5]=ETHData.Data[5]; 
// 			_crc=CalcCrc(sendbuffer,6);
// 			sendbuffer[11]=(uint8_t)(_crc&0xff);                 //CRC高
// 			sendbuffer[12]=(uint8_t)((_crc>>8)&0xff);;                 //CRC低
// 			TCP_Server_SendData(sendbuffer,8);
}

