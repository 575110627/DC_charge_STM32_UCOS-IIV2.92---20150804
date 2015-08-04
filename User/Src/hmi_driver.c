#include "stm32f2xx_hal.h"


//���ڷ���һ�ֽ�����
#define TX_8(P1) SendChar((P1)&0xFF)
//���ڷ��������ֽ�����
#define TX_8X2(P1,P2) TX_8(P1);TX_8(P2); 
//���ڷ���һ��������
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1);
//��������������
#define TX_32(P1) TX_8((P1)>>24);TX_8((P1)>>16); TX_8((P1)>>8); TX_8(P1);  

/*����֡ͷ*/
void SendBeginCMD(void)
{
    TX_8X2(0x13,0x14);
}

/*�������ݳ��ȣ�03��04��05��07.�Լ��������ݳ���rd_len*/
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



/*д���ƼĴ���ָ����0x80,�����ƼĴ���ָ����0x81,д���ݴ洢��ָ����0x82,�����ݴ洢��ָ����0x83*/
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

/*���͵�ַ*/
void SendADR(uint16_t address)
{
		TX_16(address);
}


/*����16λ���ݣ�����32λ����*/
void SendData32(uint32_t data)
{
		TX_32(data);
}
void SendData16(uint16_t data)
{
		TX_16(data);
}


/*�л�ҳ��Ŀ��ƼĴ���0x03*/
void SendChangePageRes(void)
{
		TX_8(0x03);
}
/*��λ�������Ŀ��ƼĴ���0xEE*/
void SendResetHMIRes(void)
{
		TX_8(0xEE);
}

/*����RTC  */
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



/*����3���������ܣ�����Ӧ��ַ���ɳ������ݴ洢�����ݣ�д16λ���ݵ���Ӧ�洢����д32λ���ݵ���Ӧ�洢��*/
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
/*��ָ���������λ*/		
void Reset_HMI(void)
{
	SendBeginCMD();  		//����֡ͷ
	SendDataLen4();   	//�������ݳ���  4
	SendWrCtrlCMD();    //д���ƼĴ���   1 byte
	SendResetHMIRes();  //��λ�������Ŀ��ƼĴ���0xEE 1 byte
	TX_16(0x5AA5);
}
	
/*����2�������ܣ��л�ҳ�漰����ʱҳ��*/	
void ChangePage(uint16_t page)
{
		uint16_t  pages=page;
	  SendBeginCMD();  //����֡ͷ
		SendDataLen4();   //�������ݳ���  4
		SendWrCtrlCMD();   //д���ƼĴ���   1 byte
		SendChangePageRes();  //�л�ҳ��Ŀ��ƼĴ���0x03 1 byte
		SendADR(pages);       //���л�ҳ���ַ 2 byte
}
void RdCurrentPage(void)
{
		SendBeginCMD();	      
		SendDataLen3();
		SendRdCtrlCMD();        //�����ƼĴ���
		SendChangePageRes();       
		TX_8(0x02);            
}

/*�������ɺ�������:���¹���Ա����������ر���������ƽ����ۣ���ֵ��ۣ���ֵ��ۿ�ʼʱ�䣨ʱ���֣�����ֵ��۽���ʱ�䣨ʱ���֣�����ֵ��ۣ���ֵ��ۿ�ʼʱ�䣨ʱ���֣���ֵ��۽���ʱ�䣨ʱ���֣� ���룬IP��ַ1��2��3��4��MAC��ַ*/
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
	
			Updata_IPAddress1();                                                         //IP��ַ��һλ
			Updata_IPAddress2();                                                         //IP��ַ�ڶ�λ
			Updata_IPAddress3();                                                         //IP��ַ����λ
			Updata_IPAddress4();                                                         //IP��ַ����λ  
			//MAC��ַ
		  Updata_FinalEnergy();                                                        //��׮�ѳ����
			Updata_MoneyAverageElectricity();                                //ƽ�����
			Updata_MoneyPeakElectricity();                                     //��ֵ���
			Updata_SetTimeHourStartPeakElectricity();               //��ֵ��ۿ�ʼʱ�䣨ʱ��
			Updata_SetTimeMinStartPeakElectricity();                 //��ֵ��ۿ�ʼʱ�䣨�֣�
			Updata_SetTimeHourStopPeakElectricity();                 //��ֵ��۽���ʱ�䣨ʱ��
			Updata_SetTimeMinStopPeakElectricity();                   //��ֵ��۽���ʱ�䣨�֣�
			Updata_MoneyValleyElectricity();                                 //��ֵ���
			Updata_SetTimeHourStartValleyElectricity();           //��ֵ��ۿ�ʼʱ�䣨ʱ��
			Updata_SetTimeMinStartValleyElectricity();             //��ֵ��ۿ�ʼʱ�䣨�֣�
			Updata_SetTimeHourStopValleyElectricity();             //��ֵ��۽���ʱ�䣨ʱ��
			Updata_SetTimeMinStopValleyElectricity();               //��ֵ��۽���ʱ�䣨��?
	  //����
	    Updata_Keyword();
}

void Updata_MoneyAverageElectricity(void)                                //ƽ�����
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00D6); 
	  SendData16(MoneyAverageElectricity);
}
void Updata_MoneyPeakElectricity(void)                                     //��ֵ���
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00D8); 
	  SendData16(MoneyPeakElectricity);
}
void Updata_MoneyValleyElectricity(void)                                 //��ֵ���
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00DA); 
	  SendData16(MoneyValleyElectricity);
}
void Updata_SetTimeHourStartPeakElectricity(void)               //��ֵ��ۿ�ʼʱ�䣨ʱ��
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00DC); 
	  SendData16(SetTimeHourStartPeakElectricity);
}
void Updata_SetTimeMinStartPeakElectricity(void)                 //��ֵ��ۿ�ʼʱ�䣨�֣�
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00DE); 
	  SendData16(SetTimeMinStartPeakElectricity);
}
void Updata_SetTimeHourStopPeakElectricity(void)                 //��ֵ��۽���ʱ�䣨ʱ��
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00F0); 
	  SendData16(SetTimeHourStopPeakElectricity);
}
void Updata_SetTimeMinStopPeakElectricity(void)                  //��ֵ��۽���ʱ�䣨�֣�
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00F2); 
	  SendData16(SetTimeMinStopPeakElectricity);
}
void Updata_SetTimeHourStartValleyElectricity(void)           //��ֵ��ۿ�ʼʱ�䣨ʱ��
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00F4); 
	  SendData16(SetTimeHourStartValleyElectricity);
}
void Updata_SetTimeMinStartValleyElectricity(void)           //��ֵ��ۿ�ʼʱ�䣨�֣�
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00F6); 
	  SendData16(SetTimeMinStartValleyElectricity);
}
void Updata_SetTimeHourStopValleyElectricity(void)           //��ֵ��۽���ʱ�䣨ʱ��
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00F8); 
	  SendData16(SetTimeHourStopValleyElectricity);
}
void Updata_SetTimeMinStopValleyElectricity(void)           //��ֵ��۽���ʱ�䣨�֣�
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x00FA); 
	  SendData16(SetTimeMinStopValleyElectricity);
}
void Updata_IPAddress1(void)                                                    //IP��ַ��һλ
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x0100); 
	  SendData16(IPAddress1);
}
void Updata_IPAddress2(void)                                                    //IP��ַ�ڶ�λ
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x0102); 
	  SendData16(IPAddress2);
}
void Updata_IPAddress3(void)                                                    //IP��ַ����λ
{
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
	  SendADR(0x0104); 
	  SendData16(IPAddress3);
}
void Updata_IPAddress4(void)                                                    //IP��ַ����λ
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


/*�������ɺ�������:���³��״̬��ر������������ţ����ѽ�����ѹ������������������ʱ�䣬����ʱ�䣬SOC*/
void UpDataId()   //����
{
		uint32_t  idcard1=0;
	  idcard1 = (ICCard1[3]<<24)+(ICCard1[2]<<16)+(ICCard1[1]<<8)+ICCard1[0];
		SendBeginCMD();
		SendDataLen7();
		SendWrDataCMD();
		SendADR(0x00A0);
		SendData32(idcard1);      
} 
void UpDataExpense(uint32_t expense)    //���ѽ��
{
		uint32_t  expense1=expense;
		SendBeginCMD();
		SendDataLen7();
		SendWrDataCMD();
		SendADR(0x00A4);
		SendData32(expense1);   
}
void UpDataBalance()    //���
{
		uint32_t balance1=0;
	  balance1 = (ICMoney[0]<<24)+(ICMoney[1]<<16)+(ICMoney[2]<<8)+ICMoney[3];
		SendBeginCMD();
		SendDataLen7();
		SendWrDataCMD();
		SendADR(0x00A8);
		SendData32(balance1); 
	
}
void UpDataVoltage(uint16_t voltage)    //��ѹ
{
		uint16_t  voltage1=voltage;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00AC);
		SendData16(voltage1);
}
void UpDataCurrent(uint16_t current)    //����
{
		uint16_t  current1=current;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00AE);
		SendData16(current1);
}
void UpDataKwh(uint16_t kwh)       //�ѳ����
{
		uint16_t  kwh1=kwh;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00B0);
		SendData16(kwh1);
}
void UpDataUsedHour(uint16_t Uhour)      //���ʱ
{
		uint16_t  Uhour1=Uhour;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00B2);
		SendData16(Uhour1);
}
void UpDataUsedMin(uint16_t Umin)      //����
{
		uint16_t  Umin1=Umin;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00B4);
		SendData16(Umin1);
}
void UpDataRemainHour(uint16_t Rhour)      //ʣ��ʱ
{
		uint16_t  Rhour1=Rhour;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00B6);
		SendData16(Rhour1);
}
void UpDataRemainMin(uint16_t Rmin)       // ʣ���
{
		uint16_t  Rmin1=Rmin;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00B8);
		SendData16(Rmin1);
}
void UpDataSoc(uint8_t soc)              //SOC״̬
{
		uint16_t  soc1;
	  soc1 = (uint16_t)(soc);
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x00BA);
		SendData16(soc1);
}
/*����2�������ܣ������¶ȣ�ʪ��*/	
void UpDateTemp(uint16_t te)                 //�����¶�
{
		uint16_t te1=te;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x0000);
		SendData16(te1);
}
void UpDateHumi(uint16_t hu)             //  ����ʪ��
{
		uint16_t hu1=hu;
		SendBeginCMD();
		SendDataLen5();
		SendWrDataCMD();
		SendADR(0x0002);
		SendData16(hu1);
}
