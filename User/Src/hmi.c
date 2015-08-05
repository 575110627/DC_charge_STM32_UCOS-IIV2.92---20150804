#include "stm32f2xx_hal.h"

extern I2C_HandleTypeDef    I2C1Handle;


/*  ���ģʽѡ�������Ӧ����*/
uint16_t mainselect=0;                         //������ѡ��1�����磬2���ˣ�3����Ա��4���Ʋ�����5�������
uint16_t automaticcharge=0;                    //���ģʽ���棬�Զ�����
uint16_t kwhcharge=0;                          //���ģʽ���棬��������
uint16_t moneycharge=0;                        //���ģʽ���棬������
uint16_t timecharge=0;                         //���ģʽ���棬��ʱ���
uint16_t setkwh=0;                             //���ģʽ���棬���õ���
uint16_t setmoney=0;                           //���ģʽ���棬���ý��
uint16_t sethour=0;                            //���ģʽ���棬����ʱ�䣨ʱ��
uint16_t setmin=0;                             //���ģʽ���棬����ʱ�䣨�֣�
uint16_t ButtonOKChargeMode=0;                 //���ģʽ���棬ȷ������
uint16_t ButtonEscChargeMode=0;                //���ģʽ���棬���ذ���
uint16_t ButtonEscInsertPile=0;                //��ʾ����ǹͷ���棬���ذ���


/* ����ʱ*/
uint16_t TimeCountDownFlag=0;                    //����ʱ��־λ=0;                   
uint16_t TimeCountDownChargeMode=100;            //���ģʽѡ�񵹼�ʱ100s
uint16_t TimeCountDownStartCard=60;              //ˢ����ʼ��絹��ʱ30s
uint16_t TimeCountDownInputKeyword=100;		       //����Ա���������룩����ʱ100s
uint16_t TimeCountDownControlParaSet=200;	       //���Ʋ������õ���ʱ200s
uint16_t TimeCountDownAdministrator=200;	       //����Ա���ò�������ʱ200s
uint16_t TimeCountDownInsertPile=60;	           //��ʾ��ǹ�絹��ʱ30s

/*��־λ*/
uint16_t StopChargeFlag=0;                        //���ֹͣ��簴����־λ
uint16_t flag1=0;                                 //���ģʽѡ���У�flag1=0��ʾ������ȷ������ˢ����ʼ���
uint16_t modelflag1=0;	                          //���ģʽѡ��0δѡ��1�Զ���2������3��4ʱ�� 
uint16_t cardstartflag1=0;	                      //��ʼˢ������־λ
uint16_t SendDataFlag=0;                          //������������ʪ�ȣ�����ѹ�����ȱ�־λ
uint16_t InsertPileFlag=0;                        //����ǹͷ��־λ



/*���˽�����ر���*/
uint32_t idcard=0;                               //����
uint32_t expense=0;                              //���ѽ��
uint32_t balance=0;                              //�������
uint16_t voltage=0;                              //����ѹ
uint16_t current=0;                              //������
uint16_t kwh=0;                                  //�ѳ����
uint16_t Uhour=0;                                //�ѳ�ʱ��ʱ
uint16_t Umin=0;                                 //�ѳ�ʱ���
uint16_t Rhour=0;                                //����ʱ��ʱ
uint16_t Rmin=0;                                 //����ʱ���
uint16_t soc=0;                                  //SOC
uint16_t ButtonEscCheckOut=0;                    //���˽����˳�����

 /*���Ʋ���������ر���*/
uint16_t CtrlParaBMS=0;                          //������BMS���
uint16_t CtrlParaManualOperation=0;              //�ֶ����
uint16_t CtrlParaCutOffCurrent=0;                //���ý�ֹ����
uint16_t CtrlParaVoltage=0;                      //���ó���ѹ
uint16_t CtrlParaCurrent=0;                      //���ó�����
uint16_t ButtonOkCtrlPara=0;                     //���Ʋ��������������  
uint16_t ButtonESCCtrlPara=0;                    //���Ʋ����˳�����

 /*����Աģʽ������ر���*/
uint16_t  MoneyAverageElectricity=0;             //ƽ�����
uint16_t  MoneyPeakElectricity=0;                //��ֵ���
uint16_t  MoneyValleyElectricity=0;              //��ֵ���
uint16_t  SetTimeHourStartPeakElectricity=0;     //��ֵ��ۿ�ʼʱ�䣨ʱ��
uint16_t  SetTimeMinStartPeakElectricity=0;      //��ֵ��ۿ�ʼʱ�䣨�֣�
uint16_t  SetTimeHourStopPeakElectricity=0;      //��ֵ��۽���ʱ�䣨ʱ��
uint16_t  SetTimeMinStopPeakElectricity=0;       //��ֵ��۽���ʱ�䣨�֣�
uint16_t  SetTimeHourStartValleyElectricity=0;   //��ֵ��ۿ�ʼʱ�䣨ʱ��
uint16_t  SetTimeMinStartValleyElectricity=0;    //��ֵ��ۿ�ʼʱ�䣨�֣�
uint16_t  SetTimeHourStopValleyElectricity=0;    //��ֵ��۽���ʱ�䣨ʱ��
uint16_t  SetTimeMinStopValleyElectricity=0;     //��ֵ��۽���ʱ�䣨�֣�
uint16_t  SaveAdministratorParameter=0;          //�������Ա���ò���
uint16_t  IPAddress1=0;                          //IP��ַ��һλ
uint16_t  IPAddress2=0;                          //IP��ַ�ڶ�λ
uint16_t  IPAddress3=0;                          //IP��ַ����λ
uint16_t  IPAddress4=0;                          //IP��ַ����λ
uint32_t  FinalKwh=0;                            //�ܵ���


 /*�������������ر���*/
uint16_t ButtonOKInputKeyword=0;                //�����������ȷ����
uint16_t ButtonEscInputKeyword=0;               //������������˳���
uint16_t ButtonEscAdministrator=0;	            //����Ա���ò��������˳���	
uint32_t KeywordSystem=123;                     //ϵͳĬ�����룬����������ʹ�����ƥ��                      
uint32_t KeywordInput=0;                        //������������棬���������
uint32_t KeywordSet=0;                          //����Ա������������������
uint32_t FinalKeywordSet=87654321;              //����Ա�����������ռ�����


/*���崥��������֡�ṹ���Ա����*/
PCTRL_MSG msg=NULL;                             //�ṹ��ָ�����msg
uint16_t  cmd_head=0;                           //֡ͷ13��14
uint8_t 	data_len=0;                           //֡���ȣ�����֡���ͣ���ַ�����ݳ��ȣ�����
uint8_t 	cmd_type=0;                           //֡���ͣ�80����д���ƼĴ�����81���ؼĴ�����82д���ݴ洢����83�����ݴ洢��
uint16_t  adr=0;                                //֡��ַ
uint8_t 	rd_len=0;                             //���ݳ��ȣ�����ǲ������ݴ洢��������Ϊ��λ������ǲ������ƼĴ��������ֽ�Ϊ��λ
uint16_t  hmidata[256]={0};                     //��������



uint16_t  size=0;
uint16_t  tem1=100;
uint16_t  tem2=60;
uint16_t  tem3=100;
uint16_t  tem4=200;
uint16_t  tem5=200;
uint16_t  tem6=60;


void Message_process(void)
{
			msg=(PCTRL_MSG)cmd_buffer;
		  cmd_type=msg->cmd_type;
		  data_len=msg->data_len;
		  cmd_head=PTRU16(&(msg->cmd_head));
		  adr=PTRU16(&(msg->adr));
		  hmidata[0]=PTRU16(&(msg->data[0]));
		  hmidata[1]=PTRU16(&(msg->data[1]));
		  rd_len=msg->rd_len;
	
			switch(adr)
		  {
				case 0x0030:
					 mainselect=hmidata[0]; 				                                  //������ѡ�񣺳��1������2������Ա3�����Ʋ���4
				   {
					   	if(1 == mainselect)
						     ChangePage(0x0007);
					    if(2 == mainselect)
						     ChangePage(0x0011);
					    if(3 == mainselect)
						     ChangePage(0x0037);
					    if(4 == mainselect)
						     ChangePage(0x0005);
					 }
					 break;	
				case 0x0050:
					 automaticcharge=hmidata[0]; 			                                //�Զ����
					 break;	
				case 0x0052:
					 kwhcharge=hmidata[0]; 				                                    //���������
					 break;	
				case 0x0054:
					 moneycharge=hmidata[0]; 				                                  //�������
					 break;	
				case 0x0056:
					 timecharge=hmidata[0]; 				                                  //��ʱ����
					 break;	
				case 0x005A:
					 setkwh=hmidata[0]; 				                                      //���õ���
					 break;	
				case 0x005C:
					 setmoney=hmidata[0]; 				                                    //���ý��
					 break;	
				case 0x005E:
					 sethour=hmidata[0]; 				                                      //����ʱ
					 break;	
				case 0x0060:
					 setmin=hmidata[0]; 				                                      //���÷�
					 break;	
				case 0x0058:
					 ButtonOKChargeMode=hmidata[0]; 				                          //���ģʽѡ��ȷ�ϰ���
					 break;	
				case 0x0062:
					 ButtonEscChargeMode=hmidata[0];                                  //���ģʽѡ���˳�����
					 break;
				case 0x0006:
				{
						ButtonEscInsertPile=hmidata[0];                                 //��ʾ����ǹͷ���棬���ذ���
						if(1==ButtonEscInsertPile)
						{
							SwipingPileFlag = 0;
							TimeCountDownInsertPile=60;
							ButtonEscInsertPile=0;
							InsertPileFlag=0;
							TimeCountDownFlag=0;
					  }                 			  
				}
					break;
				case 0x00C4:
					 KeywordInput=((hmidata[0])<<16)+hmidata[1];                     //�������������������
					 break;
				case 0x00C8:
					 ButtonOKInputKeyword=hmidata[0]; 				                       //�����������ȷ�ϰ���
					 break;
				case 0x00CA:
					 ButtonEscInputKeyword=hmidata[0];                               //������������˳�����
					 break;
				case 0x00D0:                       
				{ 
					KeywordSet=((hmidata[0])<<16)+hmidata[1];                        //�������ý�����������
					KeywordSystem=KeywordSet;
					break;
				}
				case 0x00C0:
						ButtonEscCheckOut=hmidata[0];                                  //��ѯ���淵�ؼ�
				    if(1 == ButtonEscCheckOut)
						{
								ButtonEscCheckOut = 0;
							  ChangePage(0x0001);
						}
						break;
				case 0x00D4:
						ButtonEscAdministrator=hmidata[0];                             //�������ý��淵�ؼ�
						break;
				case 0x00D6:
						MoneyAverageElectricity=hmidata[0];                            //ƽ�����
						break;
				case 0x00D8:
						MoneyPeakElectricity=hmidata[0];                               //��ֵ���
						break;
				case 0x00DA:
						MoneyValleyElectricity=hmidata[0];                             //��ֵ���
						break;
				case 0x00DC:
						SetTimeHourStartPeakElectricity=hmidata[0];                    //��ֵ��ۿ�ʼʱ�䣨ʱ��
						break;
				case 0x00DE:
						SetTimeMinStartPeakElectricity=hmidata[0];                     //��ֵ��ۿ�ʼʱ�䣨�֣�
						break;
				case 0x00F0:
						SetTimeHourStopPeakElectricity=hmidata[0];                     //��ֵ��۽���ʱ�䣨ʱ��
						break;
				case 0x00F2:
						SetTimeMinStopPeakElectricity=hmidata[0];                      //��ֵ��۽���ʱ�䣨�֣�
						break;
				case 0x00F4:
						SetTimeHourStartValleyElectricity=hmidata[0];                  //��ֵ��ۿ�ʼʱ�䣨ʱ��
						break;
				case 0x00F6:
						SetTimeMinStartValleyElectricity=hmidata[0];                   //��ֵ��ۿ�ʼʱ�䣨�֣�
						break;
				case 0x00F8:
						SetTimeHourStopValleyElectricity=hmidata[0];                   //��ֵ��۽���ʱ�䣨ʱ��
						break;
				case 0x00FA:
						SetTimeMinStopValleyElectricity=hmidata[0];                    //��ֵ��۽���ʱ�䣨�֣�
						break;
				case 0x00FC:
						SaveAdministratorParameter=hmidata[0];                         //�������Ա���ò���
						break;
				case 0x0070:
						CtrlParaBMS=hmidata[0];                                        //������BMS���
						break;
				case 0x0072:
						CtrlParaManualOperation=hmidata[0];                            //�ֶ����
						break;
				case 0x0074:
						CtrlParaCutOffCurrent=hmidata[0];                              //���ý�ֹ����
						break;
				case 0x0076:
						CtrlParaVoltage=hmidata[0];                                    //���ó���ѹ 
						break;
				case 0x0078:
						CtrlParaCurrent=hmidata[0];                                    //���ó�����
						break;
				case 0x007A:
						ButtonOkCtrlPara=hmidata[0];                                   //���Ʋ�������������� 
						break;
				case 0x007C:
						ButtonESCCtrlPara=hmidata[0];                                  //���Ʋ����˳�����
						break;
				case 0x0100:
						IPAddress1=hmidata[0];                                         //IP��ַ��һλ
						break;
				case 0x0102:
						IPAddress2=hmidata[0];                                         //IP��ַ�ڶ�λ
						break;
				case 0x0104:
						IPAddress3=hmidata[0];                                         //IP��ַ����λ
						break;
				case 0x0106:
						IPAddress4=hmidata[0];                                         //IP��ַ����λ
						break;
				
			}
			switch(mainselect)
			{
				case 0x0000:
							break;
				case 0x0001:
							ChargeModChoose();               //���ģʽѡ��
							break;
				case 0x0002:
							//CheckOut();                      //����
							break;
				case 0x0003:
							Input_keyword();                 //����Ա����������֤
							break;
				case 0x0004:
							CtrlPara();                      //���Ʋ���
							break;
				case 0x0005:
							Stop_Charge();                   //�������
							break;
				case 0x0006:
							Administrator();                 //����Ա���ò���
							break;
				
				}	
	
	}
void Count_Down(void)                            //����ʱ����
{
	if(mainselect==1)                              //���ģʽѡ��
			{
								 if(tem1!=TimeCountDownChargeMode)
								{
									WrDataMemory(0x00E2,TimeCountDownChargeMode); 
									tem1=TimeCountDownChargeMode;
								}
								if(TimeCountDownChargeMode<= 0)
								{
										ChangePage(0x0001);
										mainselect=0;
										TimeCountDownChargeMode=100;
									  WrDataMemory(0x00E2,TimeCountDownChargeMode);
										automaticcharge=0;
										kwhcharge=0;
										timecharge=0;
										moneycharge=0;
										setkwh=0;
										setmoney=0;
										sethour=0;
										setmin=0;
										WrDataMemory(0x005A,setkwh);
										WrDataMemory(0x005C,setmoney);
										WrDataMemory(0x005E,sethour);
										WrDataMemory(0x0060,setmin);
										ButtonOKChargeMode=0;
										ButtonEscChargeMode=0;
										flag1=0;
									  TimeCountDownFlag=0;
									}
		  }
	if(mainselect==3)                  //��������
			{
								 if(tem3!=TimeCountDownInputKeyword)
								{
									WrDataMemory(0x00E4,TimeCountDownInputKeyword);
									tem3=TimeCountDownInputKeyword;
								}
								if(TimeCountDownInputKeyword<=0)
								{
									ChangePage(0x0001);
									mainselect=0;
									TimeCountDownInputKeyword=100;
									WrDataMemory(0x00E4,TimeCountDownInputKeyword);
									KeywordInput=0;
									ButtonOKInputKeyword=0;
									WrDataMemory32(0x00C4,KeywordInput);	
									TimeCountDownFlag=0;
								}
			}
			if(mainselect==4)                  //���Ʋ���
			{
								 if(tem4!=TimeCountDownControlParaSet)
								{
									WrDataMemory(0x00E8,TimeCountDownControlParaSet);
									tem4=TimeCountDownControlParaSet;
								}
								if(TimeCountDownControlParaSet<=0)
								{
									ChangePage(0x0001);
									mainselect=0;
									TimeCountDownControlParaSet=200;
                  WrDataMemory(0x00E8,TimeCountDownControlParaSet);									
									TimeCountDownFlag=0;
									ButtonOkCtrlPara=0;
									ButtonESCCtrlPara=0;
								}
			}
	if(mainselect==6)                     //����Ա��������
			{
				 if(tem5!=TimeCountDownAdministrator)
								{
									WrDataMemory(0x00E6,TimeCountDownAdministrator);
									tem5=TimeCountDownAdministrator;
								}
								if(TimeCountDownAdministrator<=0)
								{
									ChangePage(0x0001);
									mainselect=0;
									TimeCountDownAdministrator=200;
									WrDataMemory(0x00E6,TimeCountDownAdministrator);
									TimeCountDownFlag=0;
								}
			}
	if(1==InsertPileFlag)                      //��ʾ����ǹͷ
	{
		if(tem6!=TimeCountDownInsertPile)
						{
							WrDataMemory(0x00EC,TimeCountDownInsertPile);
							tem6=TimeCountDownInsertPile;
						}
		if(TimeCountDownInsertPile<=0)
		{
			ChangePage(0x0001);
			InsertPileFlag=0;
			
			TimeCountDownInsertPile=60;
			WrDataMemory(0x00EC,TimeCountDownInsertPile);
			mainselect=0;
			TimeCountDownFlag=0;
			ButtonEscInsertPile=0;
		}
	}
	if(cardstartflag1==1)                         //ˢ����ʼ���
		{
			 if(tem2!=TimeCountDownStartCard)
						{
							WrDataMemory(0x00EA,TimeCountDownStartCard);
							tem2=TimeCountDownStartCard;
						}
			 if(TimeCountDownStartCard<=0)
						{
							if(ChargeFlag ==1)  //���
							{
								ChangePage(0x0011);
							}
							else     //����������
							{
								ChangePage(0x0001);
							}
							UpDataExpense(0);           //�������ѽ��
							UpDataVoltage(0);            //���µ�ѹ
							UpDataCurrent(0);           //���µ���
							UpDataKwh(0);               //�����ѳ����
							mainselect=0;
							SpeechFlag = 0;
							cardstartflag1=0;
							TimeCountDownChargeMode=100;
							TimeCountDownStartCard=60;
							WrDataMemory(0x00EA,TimeCountDownStartCard);
							automaticcharge=0;
							kwhcharge=0;
							timecharge=0;
							moneycharge=0;
							setkwh=0;
							setmoney=0;
							sethour=0;
							setmin=0;
							WrDataMemory(0x005A,setkwh);
							WrDataMemory(0x005C,setmoney);
							WrDataMemory(0x005E,sethour);
							WrDataMemory(0x0060,setmin);
							ButtonOKChargeMode=0;
							ButtonEscChargeMode=0;
							flag1=0;
							TimeCountDownFlag=0;
						}
			}
	}

    /*���ģʽѡ��*/
void ChargeModChoose(void)
{
	//uint8_t i;
	if((1 == ChargeFlag))
	{
		//SpeechFlag = 0;
		mainselect = 0;
		Speech("���׮����ʹ����");
		ChangePage(0x0001);
	}
	else
	{
		if((automaticcharge&&kwhcharge)||(automaticcharge&&timecharge)||(automaticcharge&&moneycharge)||(kwhcharge&&timecharge)||(kwhcharge&&moneycharge)||(timecharge&&moneycharge))
		{
				automaticcharge=0;
				kwhcharge=0;
				timecharge=0;
				moneycharge=0;
			  ChangePage(0x0007);
		}
		if(automaticcharge)
		    ChangePage(0x0008); 
		if(kwhcharge)
		    ChangePage(0x0009);
    if(moneycharge)
		    ChangePage(0x000A); 		
		if(timecharge)
		    ChangePage(0x000B); 
		
		if(ButtonOKChargeMode==1)
		{
		  if((automaticcharge==0)&&(kwhcharge==0)&&(timecharge==0)&&(moneycharge==0))
			  {
				  ChangePage(0x000C);
				  flag1=1;
			  }
			if((kwhcharge==1)&&(setkwh==0))
			  {
				  ChangePage(0x000D);
			  	flag1=2;
				}
			if((moneycharge==1)&&(setmoney==0))
			  {
				  ChangePage(0x000E);
				  flag1=3;
				}
			if((timecharge==1)&&(sethour==0)&&(setmin==0))
			  {	
				 ChangePage(0x000F);
				 flag1=4;
				}

			if(flag1==0)
			{
				//CheckOut();
				PileState = CheckPileState();
				//PileState =1;
// 				for(i=0;i<4;i++)
// 				{
// 						ICCard1[i] = 0;
// 						ICMoney[i] = 0;
// 				}
// 				UpDataId();
// 				UpDataBalance();
				//ChargingMethod = 0;
				if(PileState)
				{
					ChangePage(0x001A);
					cardstartflag1=1;
					 ICReadFlag = 0;
				  SwipingCardFlag = 1; //����ɨ������
			  	SpeechFlag = 1;
				}
				else
				{
					ChangePage(0x0038);
					SwipingPileFlag = 1;
					SpeechFlag = 1;
					 ICReadFlag = 0;
					InsertPileFlag = 1;
				}
				//cardstartflag1=1;
				//SwipingCardFlag = 1; //����ɨ������
				//SpeechFlag = 1;
				mainselect=0;
				if(automaticcharge)
				{
					modelflag1=1;
					ChargingMode = 0;
				}
				if(kwhcharge)
				{
					modelflag1=2;
					ChargingMode = 1;
					SetKwh=100*setkwh;                             //���ģʽ���棬���õ���

				}
				if(moneycharge)
				{
					modelflag1=3;
					ChargingMode = 2;	
					SetMoney=100*setmoney;                           //���ģʽ���棬���ý��	
				}
				if(timecharge)
				{
					modelflag1=4;
					ChargingMode = 3;			
					SetHour=sethour;                            //���ģʽ���棬����ʱ�䣨ʱ��
					SetMin=setmin;                             //���ģʽ���棬����ʱ�䣨�֣�							
				}
				TimeCountDownChargeMode=100;
				WrDataMemory(0x00E2,TimeCountDownChargeMode);
				automaticcharge=0;
				kwhcharge=0;
				timecharge=0;
				moneycharge=0;
				setmoney=0;
				sethour=0;
				setmin=0;
				setkwh=0;
				//�������������
				UpDataExpense(0);           //�������ѽ��
				UpDataVoltage(0);            //���µ�ѹ
				UpDataCurrent(0);           //���µ���
				UpDataKwh(0);               //�����ѳ����
				WrDataMemory(0x005A,setkwh);
				WrDataMemory(0x005C,setmoney);
				WrDataMemory(0x005E,sethour);
				WrDataMemory(0x0060,setmin);
			}
			
			ButtonOKChargeMode=0;
			flag1=0;

			
		}
		if(ButtonEscChargeMode==1)
			{
				ChangePage(0x0001);
				mainselect=0;
				automaticcharge=0;
				kwhcharge=0;
				timecharge=0;
				moneycharge=0;
				setkwh=0;
				setmoney=0;
				sethour=0;
				setmin=0;
				WrDataMemory(0x005A,setkwh);
				WrDataMemory(0x005C,setmoney);
				WrDataMemory(0x005E,sethour);
				WrDataMemory(0x0060,setmin);
				ButtonOKChargeMode=0;
				ButtonEscChargeMode=0;
				flag1=0;
				TimeCountDownChargeMode=100;
				WrDataMemory(0x00E2,TimeCountDownChargeMode);

			}
		}
  }	

void CheckOut(void)
{
		 Rhour = uwRemainChargeTime /60;
	   Rmin = uwRemainChargeTime %60;
		 //UpDateTemp((AM2321Temperature-400)/10);                  //�����¶�
		 //UpDateHumi((Am2321Humidity)/10);                         //����ʪ��
		 UpDataExpense(DealMoneyAll);                             //�������ѽ��
		 UpDataVoltage(uwOutputVoltageValue);                     //���µ�ѹ
		 UpDataCurrent(uwOutputCurrentValue);                     //���µ���
		 UpDataKwh((DealEnergy[2]<<8) + DealEnergy[3]);           //���µ���
		 UpDataUsedHour((ChargeTime.Hour));                       //��������ʱ��Сʱ
		 UpDataUsedMin((ChargeTime.Minute));                      //��������ʱ���
		 UpDataRemainHour(uwRemainChargeTime /60);                //���»���ʱ��Сʱ
		 UpDataRemainMin(uwRemainChargeTime %60);                 //���»���ʱ���
		 UpDataSoc(uwBmsSOC);  	                                  //����SOC
}

void Input_keyword(void)    //�����ж�
{
		if(ButtonOKInputKeyword)     
		{
			KeywordSystem = unSystemParameterConfigHandle.SystemParameterConfigHandle.Password;
			if((KeywordSystem==KeywordInput)||(FinalKeywordSet==KeywordInput))            //����ϵͳ��������
			{
					WrDataMemory32(0x00D0,KeywordSystem);
				  ChangePage(0x0013);
					mainselect=6;
					KeywordInput=0;
					ButtonOKInputKeyword=0;
					WrDataMemory32(0x00C4,KeywordInput);
				  TimeCountDownInputKeyword=100;
				  WrDataMemory(0x00E4,TimeCountDownInputKeyword);
				  UpdateSystemParameter();              //�ϴ�ϵͳ����
			}
			else
			{
					ChangePage(0x001E);
					KeywordInput=0;
					ButtonOKInputKeyword=0;
					WrDataMemory32(0x00C4,KeywordInput);
				  TimeCountDownInputKeyword=100;
				  WrDataMemory(0x00E4,TimeCountDownInputKeyword);
			}
		}
		if(ButtonEscInputKeyword)  
		{
			  TimeCountDownInputKeyword	=100;
			  WrDataMemory(0x00E4,TimeCountDownInputKeyword);
			  ButtonEscInputKeyword=0;
				ButtonOKInputKeyword=0;
				KeywordInput=0;
				WrDataMemory32(0x00C4,KeywordInput);
			  mainselect=0;
		}
}

void SocShow(uint8_t soc)
{
		uint8_t soc1=soc;
		uint8_t a,b,sum;
		a=soc1/10;
		b=soc1%10;
		if(b>0&&b<5)
			b=0;
		else 
			b=5;
		sum=a*10+b;
		switch(sum)
		{		
			case 0:
				ChangePage(34);
				break;
			case 5:
				ChangePage(35);
				break;
			case 10:
				ChangePage(36);
				break;
			case 15:
				ChangePage(37);
				break;
			case 20:
				ChangePage(38);
				break;
			case 25:
				ChangePage(39);
				break;
			case 30:
				ChangePage(40);
				break;
			case 35:
				ChangePage(41);
				break;
			case 40:
				ChangePage(42);
				break;
			case 45:
				ChangePage(43);
				break;
			case 50:
				ChangePage(44);
				break;
			case 55:
				ChangePage(45);
				break;
			case 60:
				ChangePage(46);
				break;
			case 65:
				ChangePage(47);
				break;
			case 70:
				ChangePage(48);
				break;
			case 75:
				ChangePage(49);
				break;
			case 80:
				ChangePage(50);
				break;
			case 85:
				ChangePage(51);
				break;
			case 90:
				ChangePage(52);
				break;
			case 95:
				ChangePage(53);
				break;
			case 100:
				ChangePage(54);
				break;	
}

}
void CtrlPara(void)		
{
	if(1==CtrlParaBMS)	
			ChangePage(0x0004);
	if(1==CtrlParaManualOperation)	
			ChangePage(0x0003);	
	if((1==CtrlParaBMS)&&(1==CtrlParaManualOperation))
	{
			CtrlParaBMS=0;
			CtrlParaManualOperation=0;
			ChangePage(0x0005);
	}

	if(1==ButtonOkCtrlPara)
	{
		ButtonOkCtrlPara=0;
		if((0==CtrlParaManualOperation)&&(0==CtrlParaBMS))
		{
			Speech("��ѡ��һ�ֿ��Ʒ�ʽ");
		}
		if(1==CtrlParaManualOperation)
		{
			if((0==CtrlParaCutOffCurrent)&&(0==CtrlParaVoltage)&&(0==CtrlParaCurrent))
			{
				Speech("��������ز���");
			}
			else
			{
					ChargingMethod=1;
					SetChargeCutOffCurrent = CtrlParaCutOffCurrent;           //���ý�ֹ����
					SetChargeVoltage = CtrlParaVoltage;                       //���ó���ѹ
					SetChargeCurrent = CtrlParaCurrent;                       //���ó���?
					ChangePage(0x0001);
					mainselect=0;
					CtrlParaBMS=0;
					CtrlParaManualOperation=0;
					TimeCountDownControlParaSet=200;
				  WrDataMemory(0x00E8,TimeCountDownControlParaSet);
			}
		}
		if(1==CtrlParaBMS)
		{
			ChargingMethod=0;
			ChangePage(0x0001);
			mainselect=0;
			CtrlParaBMS=0;
			CtrlParaManualOperation=0;
			TimeCountDownControlParaSet=200;
			WrDataMemory(0x00E8,TimeCountDownControlParaSet);
		}
	}
	if(1==ButtonESCCtrlPara)
	{
		ButtonESCCtrlPara=0;
		TimeCountDownControlParaSet=200;
		WrDataMemory(0x00E8,TimeCountDownControlParaSet);
		mainselect=0;
		CtrlParaBMS=0;
		CtrlParaManualOperation=0;	
		ChangePage(0x0001);
	}
}
void Stop_Charge(void)
{
		mainselect=0;
		StopChargeFlag=1;
		SpeechFlag = 1;
}
void Administrator(void)	
{
	if(ButtonEscAdministrator)
		{
				ButtonEscAdministrator=0;
				mainselect=0;
			  TimeCountDownAdministrator=200;
			  WrDataMemory(0x00E6,TimeCountDownAdministrator);
	
		}
  if(1==SaveAdministratorParameter)                                //�������
	{
	    uint16_t crc=0xffff;
			TimeCountDownAdministrator=200;
		  WrDataMemory(0x00E6,TimeCountDownAdministrator);
			SaveAdministratorParameter=0;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.AcchAddress = 0;       //���׮��
			unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber = 0;         //pos����
			unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer = 0;           //���汾��
			unSystemParameterConfigHandle.SystemParameterConfigHandle.IPAddress = ((IPAddress1&0xff)<<24)+((IPAddress2&0xff)<<16)+((IPAddress3&0xff)<<8)+(IPAddress4&0xff);         //IP��ַ   
			unSystemParameterConfigHandle.SystemParameterConfigHandle.MACAddressHigh = 0;     //MAC��ַ��16λ ��48λ 
			unSystemParameterConfigHandle.SystemParameterConfigHandle.MACAddressLow = 0;      //MAC��ַ��32λ ��48λ
			unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber = 0;        //��������
			unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate = 0;          //��������
			unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType = 0;          //������
			unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType = 0;         //��������
			unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice = MoneyAverageElectricity;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.HPrice = MoneyPeakElectricity;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.LPrice = MoneyValleyElectricity;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStartH = SetTimeHourStartPeakElectricity;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStartM = SetTimeMinStartPeakElectricity;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStopH = SetTimeHourStopPeakElectricity;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.HPriceStopM = SetTimeMinStopPeakElectricity;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStartH = SetTimeHourStartValleyElectricity;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStartM = SetTimeMinStartValleyElectricity;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStopH = SetTimeHourStopValleyElectricity;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.LPriceStopM = SetTimeMinStopValleyElectricity;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.StartMode = 0;		 //����ģʽ
			unSystemParameterConfigHandle.SystemParameterConfigHandle.BillMode = 0;			 //�Ʒ�ģʽ
			unSystemParameterConfigHandle.SystemParameterConfigHandle.Password = KeywordSystem;	 		 //����
			crc=CalcCrc(unSystemParameterConfigHandle.unbuffer,60);
			unSystemParameterConfigHandle.SystemParameterConfigHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);            //CRC
			unSystemParameterConfigHandle.SystemParameterConfigHandle.CRCLow = (uint8_t)(crc&0xff) ;  			//CRC
			HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS+2,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
	}
}
	
	

