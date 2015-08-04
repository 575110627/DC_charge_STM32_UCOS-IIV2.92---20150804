#ifndef __HMI_H_
#define __HMI_H_
#include "cmd_queue.h"
#include "stm32f2xx_hal.h"

/*  充电模式选择界面相应变量*/
extern uint16_t mainselect;                    //主界面选择，1代表充电，2结账，3管理员，4控制参数,5代表结束充电
extern uint16_t automaticcharge;               //充电模式界面，自动充满
extern uint16_t kwhcharge;                     //充电模式界面，按电量充
extern uint16_t moneycharge;                   //充电模式界面，按金额充
extern uint16_t timecharge;                    //充电模式界面，按时间充
extern uint16_t setkwh;                        //充电模式界面，设置电量
extern uint16_t setmoney;                      //充电模式界面，设置金额
extern uint16_t sethour;                       //充电模式界面，设置时间（时）
extern uint16_t setmin;                        //充电模式界面，设置时间（分）
extern uint16_t ButtonOKChargeMode;            //充电模式界面，确定按键
extern uint16_t ButtonEscChargeMode;           //充电模式界面，返回按键
extern uint16_t ButtonEscInsertPile;           //提示插入枪头界面，返回按键

/* 倒计时*/
extern uint16_t TimeCountDownFlag;             //倒计时标志位
extern uint16_t TimeCountDownChargeMode;       //充电模式选择倒计时100s
extern uint16_t TimeCountDownStartCard;        //刷卡开始充电倒计时30s
extern uint16_t TimeCountDownInputKeyword;		 //管理员（密码输入）倒计时100s
extern uint16_t TimeCountDownControlParaSet;	 //控制参数设置倒计时200s
extern uint16_t TimeCountDownAdministrator;	   //管理员参数设置倒计时200s
extern uint16_t TimeCountDownInsertPile;	     //提示插枪界倒计时60s

/*标志位*/
extern uint16_t flag1;	                       //充电模式选择中，flag1=0表示设置正确，进入刷卡开始充电
extern uint16_t modelflag1;	                   //充电模式选择，0未选择，1自动，2电量，3金额，4时间
extern uint16_t cardstartflag1;	               //开始刷卡充电标志位
extern uint16_t SendDataFlag;                  //更新数据如温湿度，余额，电压电流等标志位
extern uint16_t StopChargeFlag;                //点击停止充电按键标志位
extern uint16_t InsertPileFlag;                //插入枪头标志位 
 
extern uint16_t  tem1;
extern uint16_t  tem2;
extern uint16_t  tem3;
extern uint16_t  tem4;
extern uint16_t  tem5;
extern uint16_t  tem6; 
 
 /*结账界面相关变量*/
extern uint32_t idcard;                         //卡号
extern uint32_t expense;                        //消费金额
extern uint32_t balance;                        //卡内余额
extern uint16_t voltage;                        //充电电压
extern uint16_t current;                        //充电电流
extern uint16_t kwh;                            //已充电量
extern uint16_t Uhour;                          //已充时间时
extern uint16_t Umin;                           //已充时间分
extern uint16_t Rhour;                          //还需时间时
extern uint16_t Rmin;                           //还需时间分
extern uint16_t soc;                            //SOC
extern uint16_t ButtonEscCheckOut;              //结账界面退出按键

 /*管理员模式界面相关变量*/
extern uint16_t  MoneyAverageElectricity;                     //平均电价
extern uint16_t  MoneyPeakElectricity;                        //峰值电价
extern uint16_t  MoneyValleyElectricity;                      //谷值电价
extern uint16_t  SetTimeHourStartPeakElectricity;             //峰值电价开始时间（时）
extern uint16_t  SetTimeMinStartPeakElectricity;              //峰值电价开始时间（分）
extern uint16_t  SetTimeHourStopPeakElectricity;              //峰值电价结束时间（时）
extern uint16_t  SetTimeMinStopPeakElectricity;               //峰值电价结束时间（分）
extern uint16_t  SetTimeHourStartValleyElectricity;           //谷值电价开始时间（时）
extern uint16_t  SetTimeMinStartValleyElectricity;            //谷值电价开始时间（分）
extern uint16_t  SetTimeHourStopValleyElectricity;            //谷值电价结束时间（时）
extern uint16_t  SetTimeMinStopValleyElectricity;             //谷值电价结束时间（分）
extern uint16_t  SaveAdministratorParameter;                  //保存管理员设置参数
extern uint16_t  IPAddress1;                                  //IP地址第一位
extern uint16_t  IPAddress2;                                  //IP地址第二位
extern uint16_t  IPAddress3;                                  //IP地址第三位
extern uint16_t  IPAddress4;                                  //IP地址第四位
extern uint32_t  FinalKwh;                                    //总电量

 /*输入密码界面相关变量*/
extern uint16_t ButtonOKInputKeyword;                //输入密码界面确定键
extern uint16_t ButtonEscInputKeyword;               //输入密码界面退出键
extern uint16_t ButtonEscAdministrator;	             //管理员设置参数界面退出键	
extern uint32_t KeywordSystem;                       //系统默认密码，输入密码需和此密码匹配                      
extern uint32_t KeywordInput;                        //在输入密码界面，输入该密码
extern uint32_t KeywordSet;                          //管理员参数设置中设置密码
extern uint32_t FinalKeywordSet;                     //管理员参数设置中终极密码

 /*控制参数界面相关变量*/
extern uint16_t CtrlParaBMS;                     //按国标BMS充电
extern uint16_t CtrlParaManualOperation;         //手动充电
extern uint16_t CtrlParaCutOffCurrent;           //设置截止电流
extern uint16_t CtrlParaVoltage;                 //设置充电电压
extern uint16_t CtrlParaCurrent;                 //设置充电电流
extern uint16_t ButtonOkCtrlPara;                //控制参数保存参数按键  
extern uint16_t ButtonESCCtrlPara;               //控制参数退出按键


extern uint16_t ButtonEscAdministrator;	         //管理员设置参数界面退出键		
extern uint32_t KeywordSystem;                   //系统默认密码，输入密码需和此密码匹配 
extern uint32_t KeywordInput;                    //在输入密码界面，输入该密码
extern uint32_t KeywordSet;                      //管理员参数设置中设置密码
extern uint16_t ButtonOKInputKeyword;            //输入密码界面确定键
extern uint16_t ButtonEscInputKeyword;           //输入密码界面退出键
extern uint16_t SaveAdministratorParameter;      //保存管理员设置参数

/*定义触摸屏返回帧结构体成员变量*/
extern	PCTRL_MSG msg;                          //结构体指针变量msg
extern	  uint16_t  cmd_head;                   //帧头13，14
extern		uint8_t 	data_len;                   //帧长度，包括帧类型，地址，数据长度，数据
extern		uint8_t 	cmd_type;                   //帧类型：80代表写控制寄存器，81读控寄存器，82写数据存储器，83度数据存储器
extern	  uint16_t  adr;                        //帧地址
extern		uint8_t 	rd_len;                     //数据长度，如果是操作数据存储器，以字为单位，如果是操作控制寄存器，以字节为单位
extern		uint16_t  hmidata[256];                  //具体数据

extern		uint16_t  size;
		


void SocShow(uint8_t soc);
void ChargeModChoose(void);
void CheckOut(void);
void Input_keyword(void);
void CtrlPara(void);
void Count_Down(void);
void Message_process(void);
void Stop_Charge(void);
void Administrator(void);

#endif
