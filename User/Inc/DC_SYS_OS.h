/****************************************Copyright (c)****************************************************
**
**                                      卓越电气
**
**                                 直流充电桩项目组
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			target.h
** modified Date:  		2015-05-25
** Last Version:		V1.00
** Descriptions:		应用任务配置表
**
*********************************************************************************************************/
#ifndef 	_DC_SYS_OS_H_
#define 	_DC_SYS_OS_H_
/***********************************************
**任务优先级设置
***********************************************/
#define  Falg_SUCCEED_PRIO_MEM                1 //标志继承优先级
#define  Falg_SUCCEED_PRIO                    2 //标志继承优先级
#define  SYSMonitorTask_PRIO                  3
#define  SYSMEMTask_PRIO                      4
#define  GBTCanTask_PRIO                      5
#define  HVoltDCModeCanTask_PRIO		          6
#define  SYSNETTask_PRIO		                  7
#define  TFTTask_PRIO		                      9
#define  DTCTask_PRIO		                      10
#define  ModbusRxTask_PRIO 				            11
#define  WriteNVMTask_PRIO					          12



/***********************************************
**任务堆栈长度设置
***********************************************/
#define SYSMonitorTaskStackLengh              128
#define SYSMEMTaskStackLengh                  128
#define GBTCanTaskStackLengh				          128
#define HVoltDCModeCanTaskStackLengh    			128
#define SYSNETTaskStackLengh    			        128
#define DTCTaskStackLengh    			            128
#define ModbusRxTaskStackLengh				        128
#define WriteNVMTaskStackLengh			          128
#define TFTTaskStackLengh			                128


//
extern OS_EVENT    *EEPROM_KEY;                         /*EEPROM写临界信号量*/
extern OS_EVENT    *Adc1Reg0Sem;                        /*电流采集完成信号量*/
extern OS_EVENT    *Adc1Reg2Sem;                        /*24V电源电压采集信号量*/
extern OS_EVENT    *ADC1RegSem;	                     /*ADC1共享信号量*/
extern OS_EVENT    *ChargerInfoSem;                     /*充电信号量*/
extern OS_EVENT    *LEDInfoSem;                         /*led信号量*/
extern OS_EVENT    *CCSem ;                             /*CC信号量*/
extern OS_EVENT    *RTScanSem;                          /*实时时钟唤醒扫描信号量*/
extern OS_EVENT    *SPSaveSem ;						 /*系统参数保存信号*/
extern OS_EVENT    *SPSaveSuccSem;                      /*系统参数保存成功信号量*/
extern OS_EVENT    *DCCanRxMsgSem;						 /*dc信息报文信号量*/
extern OS_EVENT    *BMSCfgSem;                         /*bms信息CAN报文*/
extern OS_EVENT    *ModbusMsgSem_TFT ;                      /*MODBUS_TFT报文唤醒信号量*/
extern OS_EVENT    *ModbusMsgSem_PM ;                      /*MODBUS_PM报文唤醒信号量*/
extern OS_EVENT    *ModbusMsgSem_PR ;                      /*MODBUS_PR报文唤醒信号量*/
extern OS_EVENT    *icinfoSem ;                             /*ic访问信号量*/
extern OS_EVENT    *printinfoSem ;                             /*打印访问信号量*/
extern OS_EVENT    *rs485infoSem ;                             /*485访问信号量*/
extern OS_EVENT    *stopinfoSem ;                             /*急停信号量*/
extern OS_EVENT    *SpeechtimeSem ;       /*语音结束时间*/
extern OS_EVENT    *SYSstarupSem ;                             /*系统启动成功信号量*/
extern OS_EVENT    *DTCinfoSem ;                             /*诊断互斥信号量*/
extern OS_EVENT    *MEMinfoSem ;                             /*存储器互斥信号量*/
extern OS_EVENT    *Times_end ;                             /*同步资料信号量*/
extern OS_EVENT    *Times_end_NET;
///////////////////////////////////////////////////////////////////////////////////////////
extern OS_FLAG_GRP  *SYSMEMFLAG;                                  /*存储器逻辑控制标志组*/
extern OS_FLAG_GRP  *SYSMEMFLAG_NET;
//
extern OS_MEM      *MEMPt;             //内存分区指针
//

#endif


