#include "stm32f2xx_hal.h"


//串口发送一字节数据
#define TX_8(P1) SendChar((P1)&0xFF)
//串口发送两个字节数据
#define TX_8X2(P1,P2) TX_8(P1);TX_8(P2); 
//串口发送一个字数据
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1);
//发送两个字数据
#define TX_32(P1) TX_8((P1)>>24);TX_8((P1)>>16); TX_8((P1)>>8); TX_8(P1);  

/*发送帧头*/
void SendBeginCMD(void)
{
    TX_8X2(0x13,0x14);
}

/*发送数据长度：03，04，05，07.以及接受数据长度rd_len*/
void SendDataLen3(void)
{
		TX_8(0x03);
}
void SendDataLen4(void)
{
		TX_8(0x04);
}
void SendDataLen5(void)
{
		TX_8(0x05);
}
void SendDataLen7(void)
{
		TX_8(0x07);
}
void SendRdLen(uint8_t	rd_len)
{
		TX_8(rd_len);
}



/*写控制寄存器指令码0x80,读控制寄存器指令码0x81,写数据存储器指令码0x82,读数据存储器指令码0x83*/
void SendWrDataCMD(void)
{
		TX_8(0x82);
}
void SendRdDataCMD(void)
{
		TX_8(0x83);
}
void SendWrCtrlCMD(void)
{
		TX_8(0x80);
}
void SendRdCtrlCMD(void)
{
		TX_8(0x81);
}

/*发送地址*/
void SendADR(uint16_t address)
{
		TX_16(address);
}


/*发送16位数据，发送32位数据*/
void SendData32(uint32_t data)
{
		TX_32(data);
}
void SendData16(uint16_t data)
{
		TX_16(data);
}


/*切换页面的控制寄存器0x03*/
void SendChangePageRes(void)
{
		TX_8(0x03);
}
/*复位触摸屏的控制寄存器0xEE*/
void SendResetHMIRes(void)
{
		TX_8(0xEE);
}

/*更新RTC  */
void UpdateRTC(void)
{
		SendBeginCMD();
		TX_8(0x000A);
	  TX_8(0x0080);
		TX_8(0x001F);
	  TX_8(0x005A);
		//TX_8(0x0015);
	  TX_8(BCDToChar(RTCSetTime.Year));
	  //TX_8(0x0006);
		TX_8(BCDToChar(RTCSetTime.Month));
		//TX_8(0x0019);
		TX_8(BCDToChar(RTCSetTime.Day));
	  //TX_8(0x0000);
		TX_8(BCDToChar(RTCSetTime.Week));
		//TX_8(0x0018);
		TX_8(BCDToChar(RTCSetTime.Hour));
	  //TX_8(0x0055);
		TX_8(BCDToChar(RTCSetTime.Minute));
		//TX_8(0x0000);
		TX_8(BCDToChar(RTCSetTime.Second));
}



/*下面3个函数功能：读相应地址若干长度数据存储器数据，写16位数据到相应存储器，写32位数据到相应存储器*/
void RdDataMemory(uint16_t address,uint8_t rd_len)
{
		uint16_t add=address;
		uint8_t	len=rd_len;
		SendBeginCMD();
		SendDataLen4();
		SendRdDataCMD();
		SendADR(add);
		SendRdLen(len);
}
void WrDataMemory(uint16_t address,uint16_t data)
{
		uint16_t  address1=address;
		uint16_t 	data1=data;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(address1);
		SendData16(data1);
	}
void WrDataMemory32(uint16_t address,uint32_t data)
{
		uint16_t  address1=address;
		uint32_t data1=data;
		SendBeginCMD();
		SendDataLen7();
		SendWrDataCMD();
		SendADR(address1);
		SendData32(data1);
}
/*发指令进行屏复位*/		
void Reset_HMI(void)
{
	SendBeginCMD();  		//发送帧头
	SendDataLen4();   	//发送数据长度  4
	SendWrCtrlCMD();    //写控制寄存器   1 byte
	SendResetHMIRes();  //复位触摸屏的控制寄存器0xEE 1 byte
	TX_16(0x5AA5);
}
	
/*下面2函数功能：切换页面及读此时页面*/	
void ChangePage(uint16_t page)
{
		uint16_t  pages=page;
	  SendBeginCMD();  //发送帧头
		SendDataLen4();   //发送数据长度  4
		SendWrCtrlCMD();   //写控制寄存器   1 byte
		SendChangePageRes();  //切换页面的控制寄存器0x03 1 byte
		SendADR(pages);       //待切换页面地址 2 byte
}
void RdCurrentPage(void)
{
		SendBeginCMD();	      
		SendDataLen3();
		SendRdCtrlCMD();        //读控制寄存器
		SendChangePageRes();       
		TX_8(0x02);            
}

/*下面若干函数作用:更新管理员参数设置相关变量，包括平均电价，峰值电价，峰值电价开始时间（时，分），峰值电价结束时间（时，分），谷值电价，谷值电价开始时间（时，分）谷值电价结束时间（时，分） 密码，IP地址1，2，3，4，MAC地址*/
void UpdateSystemParameter(void)
{
	    IPAddress1 = (uint16_t)((unSystemParameterConfigHandle.SystemParameterConfigHandle.IPAddress>>24)&0xff);
	    IPAddress2 = (uint16_t)((unSystemParameterConfigHandle.SystemParameterConfigHandle.IPAddress>>16)&0xff);
	    IPAddress3 = (uint16_t)((unSystemParameterConfigHandle.SystemParameterConfigHandle.IPAddress>>8)&0xff);
	    IPAddress4 = (uint16_t)(unSystemParameterConfigHandle.SystemParameterConfigHandle.IPAddress&0xff);
	    MoneyAverageElectricity = unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice;
			MoneyPeakElectricity = unSystemParameterConfigHandle.SystemParameterConfigHandle.HPrice;
	    SetTimeHourStartPeakElectricity = unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStartH;
	    SetTimeMinStartPeakElectricity = unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStartM;
	    SetTimeHourStopPeakElectricity = unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStopH;
	    SetTimeMinStopPeakElectricity = unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStopM;
	    MoneyValleyElectricity = unSystemParameterConfigHandle.SystemParameterConfigHandle.LPrice;
	    SetTimeHourStartValleyElectricity = unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStartH;
	    SetTimeMinStartValleyElectricity = unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStartM;
	    SetTimeHourStopValleyElectricity = unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStopH;
	    SetTimeMinStopValleyElectricity = unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStopM;
	    KeywordSystem = unSystemParameterConfigHandle.SystemParameterConfigHandle.Password;
			FinalKwh = (ACEnergy[0]<<24) + (ACEnergy[1]<<16) + (ACEnergy[2]<<8) + ACEnergy[3] ;
	
			Updata_IPAddress1();                                                         //IP地址第一位
			Updata_IPAddress2();                                                         //IP地址第二位
			Updata_IPAddress3();                                                         //IP地址第三位
			Updata_IPAddress4();                                                         //IP地址第四位  
			//MAC地址
		  Updata_FinalEnergy();                                                        //本桩已充电量
			Updata_MoneyAverageElectricity();                                //平均电价
			Updata_MoneyPeakElectricity();                                     //峰值电价
			Updata_SetTimeHourStartPeakElectricity();               //峰值电价开始时间（时）
			Updata_SetTimeMinStartPeakElectricity();                 //峰值电价开始时间（分）
			Updata_SetTimeHourStopPeakElectricity();                 //峰值电价结束时间（时）
			Updata_SetTimeMinStopPeakElectricity();                   //峰值电价结束时间（分）
			Updata_MoneyValleyElectricity();                                 //谷值电价
			Updata_SetTimeHourStartValleyElectricity();           //谷值电价开始时间（时）
			Updata_SetTimeMinStartValleyElectricity();             //谷值电价开始时间（分）
			Updata_SetTimeHourStopValleyElectricity();             //谷值电价结束时间（时）
			Updata_SetTimeMinStopValleyElectricity();               //谷值电价结束时间（分?
	  //密码
	    Updata_Keyword();
}

void Updata_MoneyAverageElectricity(void)                                //平均电价
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00D6); 
	  SendData16(MoneyAverageElectricity);
}
void Updata_MoneyPeakElectricity(void)                                     //峰值电价
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00D8); 
	  SendData16(MoneyPeakElectricity);
}
void Updata_MoneyValleyElectricity(void)                                 //谷值电价
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00DA); 
	  SendData16(MoneyValleyElectricity);
}
void Updata_SetTimeHourStartPeakElectricity(void)               //峰值电价开始时间（时）
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00DC); 
	  SendData16(SetTimeHourStartPeakElectricity);
}
void Updata_SetTimeMinStartPeakElectricity(void)                 //峰值电价开始时间（分）
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00DE); 
	  SendData16(SetTimeMinStartPeakElectricity);
}
void Updata_SetTimeHourStopPeakElectricity(void)                 //峰值电价结束时间（时）
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00F0); 
	  SendData16(SetTimeHourStopPeakElectricity);
}
void Updata_SetTimeMinStopPeakElectricity(void)                  //峰值电价结束时间（分）
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00F2); 
	  SendData16(SetTimeMinStopPeakElectricity);
}
void Updata_SetTimeHourStartValleyElectricity(void)           //谷值电价开始时间（时）
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00F4); 
	  SendData16(SetTimeHourStartValleyElectricity);
}
void Updata_SetTimeMinStartValleyElectricity(void)           //谷值电价开始时间（分）
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00F6); 
	  SendData16(SetTimeMinStartValleyElectricity);
}
void Updata_SetTimeHourStopValleyElectricity(void)           //谷值电价结束时间（时）
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00F8); 
	  SendData16(SetTimeHourStopValleyElectricity);
}
void Updata_SetTimeMinStopValleyElectricity(void)           //谷值电价结束时间（分）
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00FA); 
	  SendData16(SetTimeMinStopValleyElectricity);
}
void Updata_IPAddress1(void)                                                    //IP地址第一位
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x0100); 
	  SendData16(IPAddress1);
}
void Updata_IPAddress2(void)                                                    //IP地址第二位
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x0102); 
	  SendData16(IPAddress2);
}
void Updata_IPAddress3(void)                                                    //IP地址第三位
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x0104); 
	  SendData16(IPAddress3);
}
void Updata_IPAddress4(void)                                                    //IP地址第四位
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x0106); 
	  SendData16(IPAddress4);
}

void Updata_Keyword(void)
{
		SendBeginCMD();
		SendDataLen7();
		SendWrDataCMD();
		SendADR(0x00D0);
		SendData32(KeywordSystem);
}

void Updata_FinalEnergy(void)
{
		SendBeginCMD();
		SendDataLen7();
		SendWrDataCMD();
		SendADR(0x0110);
		SendData32(FinalKwh);
}


/*下面若干函数作用:更新充电状态相关变量，包括卡号，消费金额，余额，电压，电流，电量，已用时间，还需时间，SOC*/
void UpDataId()   //卡号
{
		uint32_t  idcard1=0;
	  idcard1 = (ICCard1[3]<<24)+(ICCard1[2]<<16)+(ICCard1[1]<<8)+ICCard1[0];
		SendBeginCMD();
		SendDataLen7();
		SendWrDataCMD();
		SendADR(0x00A0);
		SendData32(idcard1);      
} 
void UpDataExpense(uint32_t expense)    //消费金额
{
		uint32_t  expense1=expense;
		SendBeginCMD();
		SendDataLen7();
		SendWrDataCMD();
		SendADR(0x00A4);
		SendData32(expense1);   
}
void UpDataBalance()    //余额
{
		uint32_t balance1=0;
	  balance1 = (ICMoney[0]<<24)+(ICMoney[1]<<16)+(ICMoney[2]<<8)+ICMoney[3];
		SendBeginCMD();
		SendDataLen7();
		SendWrDataCMD();
		SendADR(0x00A8);
		SendData32(balance1); 
	
}
void UpDataVoltage(uint16_t voltage)    //电压
{
		uint16_t  voltage1=voltage;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00AC);
		SendData16(voltage1);
}
void UpDataCurrent(uint16_t current)    //电流
{
		uint16_t  current1=current;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00AE);
		SendData16(current1);
}
void UpDataKwh(uint16_t kwh)       //已充电量
{
		uint16_t  kwh1=kwh;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00B0);
		SendData16(kwh1);
}
void UpDataUsedHour(uint16_t Uhour)      //充电时
{
		uint16_t  Uhour1=Uhour;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00B2);
		SendData16(Uhour1);
}
void UpDataUsedMin(uint16_t Umin)      //充电分
{
		uint16_t  Umin1=Umin;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00B4);
		SendData16(Umin1);
}
void UpDataRemainHour(uint16_t Rhour)      //剩余时
{
		uint16_t  Rhour1=Rhour;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00B6);
		SendData16(Rhour1);
}
void UpDataRemainMin(uint16_t Rmin)       // 剩余分
{
		uint16_t  Rmin1=Rmin;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00B8);
		SendData16(Rmin1);
}
void UpDataSoc(uint8_t soc)              //SOC状态
{
		uint16_t  soc1;
	  soc1 = (uint16_t)(soc);
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00BA);
		SendData16(soc1);
}
/*下面2函数功能：更新温度，湿度*/	
void UpDateTemp(uint16_t te)                 //更新温度
{
		uint16_t te1=te;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x0000);
		SendData16(te1);
}
void UpDateHumi(uint16_t hu)             //  更新湿度
{
		uint16_t hu1=hu;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x0002);
		SendData16(hu1);
}
