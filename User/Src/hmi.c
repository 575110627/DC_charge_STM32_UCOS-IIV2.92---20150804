#include "stm32f2xx_hal.h"

extern I2C_HandleTypeDef    I2C1Handle;


/*  充电模式选择界面相应变量*/
uint16_t mainselect=0;                         //主界面选择，1代表充电，2结账，3管理员，4控制参数，5结束充电
uint16_t automaticcharge=0;                    //充电模式界面，自动充满
uint16_t kwhcharge=0;                          //充电模式界面，按电量充
uint16_t moneycharge=0;                        //充电模式界面，按金额充
uint16_t timecharge=0;                         //充电模式界面，按时间充
uint16_t setkwh=0;                             //充电模式界面，设置电量
uint16_t setmoney=0;                           //充电模式界面，设置金额
uint16_t sethour=0;                            //充电模式界面，设置时间（时）
uint16_t setmin=0;                             //充电模式界面，设置时间（分）
uint16_t ButtonOKChargeMode=0;                 //充电模式界面，确定按键
uint16_t ButtonEscChargeMode=0;                //充电模式界面，返回按键
uint16_t ButtonEscInsertPile=0;                //提示插入枪头界面，返回按键


/* 倒计时*/
uint16_t TimeCountDownFlag=0;                    //倒计时标志位=0;                   
uint16_t TimeCountDownChargeMode=100;            //充电模式选择倒计时100s
uint16_t TimeCountDownStartCard=60;              //刷卡开始充电倒计时30s
uint16_t TimeCountDownInputKeyword=100;		       //管理员（密码输入）倒计时100s
uint16_t TimeCountDownControlParaSet=200;	       //控制参数设置倒计时200s
uint16_t TimeCountDownAdministrator=200;	       //管理员设置参数倒计时200s
uint16_t TimeCountDownInsertPile=60;	           //提示插枪界倒计时30s

/*标志位*/
uint16_t StopChargeFlag=0;                        //点击停止充电按键标志位
uint16_t flag1=0;                                 //充电模式选择中，flag1=0表示设置正确，进入刷卡开始充电
uint16_t modelflag1=0;	                          //充电模式选择，0未选择，1自动，2电量，3金额，4时间 
uint16_t cardstartflag1=0;	                      //开始刷卡充电标志位
uint16_t SendDataFlag=0;                          //更新数据如温湿度，余额，电压电流等标志位
uint16_t InsertPileFlag=0;                        //插入枪头标志位



/*结账界面相关变量*/
uint32_t idcard=0;                               //卡号
uint32_t expense=0;                              //消费金额
uint32_t balance=0;                              //卡内余额
uint16_t voltage=0;                              //充电电压
uint16_t current=0;                              //充电电流
uint16_t kwh=0;                                  //已充电量
uint16_t Uhour=0;                                //已充时间时
uint16_t Umin=0;                                 //已充时间分
uint16_t Rhour=0;                                //还需时间时
uint16_t Rmin=0;                                 //还需时间分
uint16_t soc=0;                                  //SOC
uint16_t ButtonEscCheckOut=0;                    //结账界面退出按键

 /*控制参数界面相关变量*/
uint16_t CtrlParaBMS=0;                          //按国标BMS充电
uint16_t CtrlParaManualOperation=0;              //手动充电
uint16_t CtrlParaCutOffCurrent=0;                //设置截止电流
uint16_t CtrlParaVoltage=0;                      //设置充电电压
uint16_t CtrlParaCurrent=0;                      //设置充电电流
uint16_t ButtonOkCtrlPara=0;                     //控制参数保存参数按键  
uint16_t ButtonESCCtrlPara=0;                    //控制参数退出按键

 /*管理员模式界面相关变量*/
uint16_t  MoneyAverageElectricity=0;             //平均电价
uint16_t  MoneyPeakElectricity=0;                //峰值电价
uint16_t  MoneyValleyElectricity=0;              //谷值电价
uint16_t  SetTimeHourStartPeakElectricity=0;     //峰值电价开始时间（时）
uint16_t  SetTimeMinStartPeakElectricity=0;      //峰值电价开始时间（分）
uint16_t  SetTimeHourStopPeakElectricity=0;      //峰值电价结束时间（时）
uint16_t  SetTimeMinStopPeakElectricity=0;       //峰值电价结束时间（分）
uint16_t  SetTimeHourStartValleyElectricity=0;   //谷值电价开始时间（时）
uint16_t  SetTimeMinStartValleyElectricity=0;    //谷值电价开始时间（分）
uint16_t  SetTimeHourStopValleyElectricity=0;    //谷值电价结束时间（时）
uint16_t  SetTimeMinStopValleyElectricity=0;     //谷值电价结束时间（分）
uint16_t  SaveAdministratorParameter=0;          //保存管理员设置参数
uint16_t  IPAddress1=0;                          //IP地址第一位
uint16_t  IPAddress2=0;                          //IP地址第二位
uint16_t  IPAddress3=0;                          //IP地址第三位
uint16_t  IPAddress4=0;                          //IP地址第四位
uint32_t  FinalKwh=0;                            //总电量


 /*输入密码界面相关变量*/
uint16_t ButtonOKInputKeyword=0;                //输入密码界面确定键
uint16_t ButtonEscInputKeyword=0;               //输入密码界面退出键
uint16_t ButtonEscAdministrator=0;	            //管理员设置参数界面退出键	
uint32_t KeywordSystem=123;                     //系统默认密码，输入密码需和此密码匹配                      
uint32_t KeywordInput=0;                        //在输入密码界面，输入该密码
uint32_t KeywordSet=0;                          //管理员参数设置中设置密码
uint32_t FinalKeywordSet=87654321;              //管理员参数设置中终极密码


/*定义触摸屏返回帧结构体成员变量*/
PCTRL_MSG msg=NULL;                             //结构体指针变量msg
uint16_t  cmd_head=0;                           //帧头13，14
uint8_t 	data_len=0;                           //帧长度，包括帧类型，地址，数据长度，数据
uint8_t 	cmd_type=0;                           //帧类型：80代表写控制寄存器，81读控寄存器，82写数据存储器，83度数据存储器
uint16_t  adr=0;                                //帧地址
uint8_t 	rd_len=0;                             //数据长度，如果是操作数据存储器，以字为单位，如果是操作控制寄存器，以字节为单位
uint16_t  hmidata[256]={0};                     //具体数据



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
					 mainselect=hmidata[0]; 				                                  //主界面选择：充电1，结账2，管理员3，控制参数4
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
					 automaticcharge=hmidata[0]; 			                                //自动充电
					 break;	
				case 0x0052:
					 kwhcharge=hmidata[0]; 				                                    //按电量充电
					 break;	
				case 0x0054:
					 moneycharge=hmidata[0]; 				                                  //按金额充电
					 break;	
				case 0x0056:
					 timecharge=hmidata[0]; 				                                  //按时间充电
					 break;	
				case 0x005A:
					 setkwh=hmidata[0]; 				                                      //设置电量
					 break;	
				case 0x005C:
					 setmoney=hmidata[0]; 				                                    //设置金额
					 break;	
				case 0x005E:
					 sethour=hmidata[0]; 				                                      //设置时
					 break;	
				case 0x0060:
					 setmin=hmidata[0]; 				                                      //设置分
					 break;	
				case 0x0058:
					 ButtonOKChargeMode=hmidata[0]; 				                          //充电模式选择确认按键
					 break;	
				case 0x0062:
					 ButtonEscChargeMode=hmidata[0];                                  //充电模式选择退出按键
					 break;
				case 0x0006:
				{
						ButtonEscInsertPile=hmidata[0];                                 //提示插入枪头界面，返回按键
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
					 KeywordInput=((hmidata[0])<<16)+hmidata[1];                     //输入密码界面输入密码
					 break;
				case 0x00C8:
					 ButtonOKInputKeyword=hmidata[0]; 				                       //输入密码界面确认按键
					 break;
				case 0x00CA:
					 ButtonEscInputKeyword=hmidata[0];                               //输入密码界面退出按键
					 break;
				case 0x00D0:                       
				{ 
					KeywordSet=((hmidata[0])<<16)+hmidata[1];                        //参数设置界面设置密码
					KeywordSystem=KeywordSet;
					break;
				}
				case 0x00C0:
						ButtonEscCheckOut=hmidata[0];                                  //查询界面返回键
				    if(1 == ButtonEscCheckOut)
						{
								ButtonEscCheckOut = 0;
							  ChangePage(0x0001);
						}
						break;
				case 0x00D4:
						ButtonEscAdministrator=hmidata[0];                             //参数设置界面返回键
						break;
				case 0x00D6:
						MoneyAverageElectricity=hmidata[0];                            //平均电价
						break;
				case 0x00D8:
						MoneyPeakElectricity=hmidata[0];                               //峰值电价
						break;
				case 0x00DA:
						MoneyValleyElectricity=hmidata[0];                             //谷值电价
						break;
				case 0x00DC:
						SetTimeHourStartPeakElectricity=hmidata[0];                    //峰值电价开始时间（时）
						break;
				case 0x00DE:
						SetTimeMinStartPeakElectricity=hmidata[0];                     //峰值电价开始时间（分）
						break;
				case 0x00F0:
						SetTimeHourStopPeakElectricity=hmidata[0];                     //峰值电价结束时间（时）
						break;
				case 0x00F2:
						SetTimeMinStopPeakElectricity=hmidata[0];                      //峰值电价结束时间（分）
						break;
				case 0x00F4:
						SetTimeHourStartValleyElectricity=hmidata[0];                  //谷值电价开始时间（时）
						break;
				case 0x00F6:
						SetTimeMinStartValleyElectricity=hmidata[0];                   //谷值电价开始时间（分）
						break;
				case 0x00F8:
						SetTimeHourStopValleyElectricity=hmidata[0];                   //谷值电价结束时间（时）
						break;
				case 0x00FA:
						SetTimeMinStopValleyElectricity=hmidata[0];                    //谷值电价结束时间（分）
						break;
				case 0x00FC:
						SaveAdministratorParameter=hmidata[0];                         //保存管理员设置参数
						break;
				case 0x0070:
						CtrlParaBMS=hmidata[0];                                        //按国标BMS充电
						break;
				case 0x0072:
						CtrlParaManualOperation=hmidata[0];                            //手动充电
						break;
				case 0x0074:
						CtrlParaCutOffCurrent=hmidata[0];                              //设置截止电流
						break;
				case 0x0076:
						CtrlParaVoltage=hmidata[0];                                    //设置充电电压 
						break;
				case 0x0078:
						CtrlParaCurrent=hmidata[0];                                    //设置充电电流
						break;
				case 0x007A:
						ButtonOkCtrlPara=hmidata[0];                                   //控制参数保存参数按键 
						break;
				case 0x007C:
						ButtonESCCtrlPara=hmidata[0];                                  //控制参数退出按键
						break;
				case 0x0100:
						IPAddress1=hmidata[0];                                         //IP地址第一位
						break;
				case 0x0102:
						IPAddress2=hmidata[0];                                         //IP地址第二位
						break;
				case 0x0104:
						IPAddress3=hmidata[0];                                         //IP地址第三位
						break;
				case 0x0106:
						IPAddress4=hmidata[0];                                         //IP地址第四位
						break;
				
			}
			switch(mainselect)
			{
				case 0x0000:
							break;
				case 0x0001:
							ChargeModChoose();               //充电模式选择
							break;
				case 0x0002:
							//CheckOut();                      //结账
							break;
				case 0x0003:
							Input_keyword();                 //管理员密码输入验证
							break;
				case 0x0004:
							CtrlPara();                      //控制参数
							break;
				case 0x0005:
							Stop_Charge();                   //结束充电
							break;
				case 0x0006:
							Administrator();                 //管理员设置参数
							break;
				
				}	
	
	}
void Count_Down(void)                            //倒计时函数
{
	if(mainselect==1)                              //充电模式选择
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
	if(mainselect==3)                  //输入密码
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
			if(mainselect==4)                  //控制参数
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
	if(mainselect==6)                     //管理员参数设置
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
	if(1==InsertPileFlag)                      //提示插入枪头
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
	if(cardstartflag1==1)                         //刷卡开始充电
		{
			 if(tem2!=TimeCountDownStartCard)
						{
							WrDataMemory(0x00EA,TimeCountDownStartCard);
							tem2=TimeCountDownStartCard;
						}
			 if(TimeCountDownStartCard<=0)
						{
							if(ChargeFlag ==1)  //充电
							{
								ChangePage(0x0011);
							}
							else     //调回主界面
							{
								ChangePage(0x0001);
							}
							UpDataExpense(0);           //更新消费金额
							UpDataVoltage(0);            //更新电压
							UpDataCurrent(0);           //更新电流
							UpDataKwh(0);               //更新已充电能
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

    /*充电模式选择*/
void ChargeModChoose(void)
{
	//uint8_t i;
	if((1 == ChargeFlag))
	{
		//SpeechFlag = 0;
		mainselect = 0;
		Speech("充电桩正在使用中");
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
				  SwipingCardFlag = 1; //进行扫卡动作
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
				//SwipingCardFlag = 1; //进行扫卡动作
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
					SetKwh=100*setkwh;                             //充电模式界面，设置电量

				}
				if(moneycharge)
				{
					modelflag1=3;
					ChargingMode = 2;	
					SetMoney=100*setmoney;                           //充电模式界面，设置金额	
				}
				if(timecharge)
				{
					modelflag1=4;
					ChargingMode = 3;			
					SetHour=sethour;                            //充电模式界面，设置时间（时）
					SetMin=setmin;                             //充电模式界面，设置时间（分）							
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
				//清除缓冲区数据
				UpDataExpense(0);           //更新消费金额
				UpDataVoltage(0);            //更新电压
				UpDataCurrent(0);           //更新电流
				UpDataKwh(0);               //更新已充电能
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
		 //UpDateTemp((AM2321Temperature-400)/10);                  //更新温度
		 //UpDateHumi((Am2321Humidity)/10);                         //更新湿度
		 UpDataExpense(DealMoneyAll);                             //更新消费金额
		 UpDataVoltage(uwOutputVoltageValue);                     //更新电压
		 UpDataCurrent(uwOutputCurrentValue);                     //更新电流
		 UpDataKwh((DealEnergy[2]<<8) + DealEnergy[3]);           //更新电能
		 UpDataUsedHour((ChargeTime.Hour));                       //更新已用时间小时
		 UpDataUsedMin((ChargeTime.Minute));                      //更新已用时间分
		 UpDataRemainHour(uwRemainChargeTime /60);                //更新还需时间小时
		 UpDataRemainMin(uwRemainChargeTime %60);                 //更新还需时间分
		 UpDataSoc(uwBmsSOC);  	                                  //更新SOC
}

void Input_keyword(void)    //密码判断
{
		if(ButtonOKInputKeyword)     
		{
			KeywordSystem = unSystemParameterConfigHandle.SystemParameterConfigHandle.Password;
			if((KeywordSystem==KeywordInput)||(FinalKeywordSet==KeywordInput))            //进入系统参数界面
			{
					WrDataMemory32(0x00D0,KeywordSystem);
				  ChangePage(0x0013);
					mainselect=6;
					KeywordInput=0;
					ButtonOKInputKeyword=0;
					WrDataMemory32(0x00C4,KeywordInput);
				  TimeCountDownInputKeyword=100;
				  WrDataMemory(0x00E4,TimeCountDownInputKeyword);
				  UpdateSystemParameter();              //上传系统参数
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
			Speech("请选择一种控制方式");
		}
		if(1==CtrlParaManualOperation)
		{
			if((0==CtrlParaCutOffCurrent)&&(0==CtrlParaVoltage)&&(0==CtrlParaCurrent))
			{
				Speech("请设置相关参数");
			}
			else
			{
					ChargingMethod=1;
					SetChargeCutOffCurrent = CtrlParaCutOffCurrent;           //设置截止电流
					SetChargeVoltage = CtrlParaVoltage;                       //设置充电电压
					SetChargeCurrent = CtrlParaCurrent;                       //设置充电电?
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
  if(1==SaveAdministratorParameter)                                //保存参数
	{
	    uint16_t crc=0xffff;
			TimeCountDownAdministrator=200;
		  WrDataMemory(0x00E6,TimeCountDownAdministrator);
			SaveAdministratorParameter=0;
			unSystemParameterConfigHandle.SystemParameterConfigHandle.AcchAddress = 0;       //充电桩号
			unSystemParameterConfigHandle.SystemParameterConfigHandle.PosNumber = 0;         //pos机号
			unSystemParameterConfigHandle.SystemParameterConfigHandle.CardVer = 0;           //卡版本号
			unSystemParameterConfigHandle.SystemParameterConfigHandle.IPAddress = ((IPAddress1&0xff)<<24)+((IPAddress2&0xff)<<16)+((IPAddress3&0xff)<<8)+(IPAddress4&0xff);         //IP地址   
			unSystemParameterConfigHandle.SystemParameterConfigHandle.MACAddressHigh = 0;     //MAC地址高16位 总48位 
			unSystemParameterConfigHandle.SystemParameterConfigHandle.MACAddressLow = 0;      //MAC地址低32位 总48位
			unSystemParameterConfigHandle.SystemParameterConfigHandle.AreaNumber = 0;        //地区代码
			unSystemParameterConfigHandle.SystemParameterConfigHandle.ConsRate = 0;          //电量倍率
			unSystemParameterConfigHandle.SystemParameterConfigHandle.CardType = 0;          //卡类型
			unSystemParameterConfigHandle.SystemParameterConfigHandle.TransType = 0;         //交易类型
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
			unSystemParameterConfigHandle.SystemParameterConfigHandle.StartMode = 0;		 //启动模式
			unSystemParameterConfigHandle.SystemParameterConfigHandle.BillMode = 0;			 //计费模式
			unSystemParameterConfigHandle.SystemParameterConfigHandle.Password = KeywordSystem;	 		 //密码
			crc=CalcCrc(unSystemParameterConfigHandle.unbuffer,60);
			unSystemParameterConfigHandle.SystemParameterConfigHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);            //CRC
			unSystemParameterConfigHandle.SystemParameterConfigHandle.CRCLow = (uint8_t)(crc&0xff) ;  			//CRC
			HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS+2,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
	}
}
	
	

