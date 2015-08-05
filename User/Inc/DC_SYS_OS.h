/****************************************Copyright (c)****************************************************
**
**                                      ׿Խ����
**
**                                 ֱ�����׮��Ŀ��
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			target.h
** modified Date:  		2015-05-25
** Last Version:		V1.00
** Descriptions:		Ӧ���������ñ�
**
*********************************************************************************************************/
#ifndef 	_DC_SYS_OS_H_
#define 	_DC_SYS_OS_H_
/***********************************************
**�������ȼ�����
***********************************************/
#define  Falg_SUCCEED_PRIO_MEM                1 //��־�̳����ȼ�
#define  Falg_SUCCEED_PRIO                    2 //��־�̳����ȼ�
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
**�����ջ��������
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
extern OS_EVENT    *EEPROM_KEY;                         /*EEPROMд�ٽ��ź���*/
extern OS_EVENT    *Adc1Reg0Sem;                        /*�����ɼ�����ź���*/
extern OS_EVENT    *Adc1Reg2Sem;                        /*24V��Դ��ѹ�ɼ��ź���*/
extern OS_EVENT    *ADC1RegSem;	                     /*ADC1�����ź���*/
extern OS_EVENT    *ChargerInfoSem;                     /*����ź���*/
extern OS_EVENT    *LEDInfoSem;                         /*led�ź���*/
extern OS_EVENT    *CCSem ;                             /*CC�ź���*/
extern OS_EVENT    *RTScanSem;                          /*ʵʱʱ�ӻ���ɨ���ź���*/
extern OS_EVENT    *SPSaveSem ;						 /*ϵͳ���������ź�*/
extern OS_EVENT    *SPSaveSuccSem;                      /*ϵͳ��������ɹ��ź���*/
extern OS_EVENT    *DCCanRxMsgSem;						 /*dc��Ϣ�����ź���*/
extern OS_EVENT    *BMSCfgSem;                         /*bms��ϢCAN����*/
extern OS_EVENT    *ModbusMsgSem_TFT ;                      /*MODBUS_TFT���Ļ����ź���*/
extern OS_EVENT    *ModbusMsgSem_PM ;                      /*MODBUS_PM���Ļ����ź���*/
extern OS_EVENT    *ModbusMsgSem_PR ;                      /*MODBUS_PR���Ļ����ź���*/
extern OS_EVENT    *icinfoSem ;                             /*ic�����ź���*/
extern OS_EVENT    *printinfoSem ;                             /*��ӡ�����ź���*/
extern OS_EVENT    *rs485infoSem ;                             /*485�����ź���*/
extern OS_EVENT    *stopinfoSem ;                             /*��ͣ�ź���*/
extern OS_EVENT    *SpeechtimeSem ;       /*��������ʱ��*/
extern OS_EVENT    *SYSstarupSem ;                             /*ϵͳ�����ɹ��ź���*/
extern OS_EVENT    *DTCinfoSem ;                             /*��ϻ����ź���*/
extern OS_EVENT    *MEMinfoSem ;                             /*�洢�������ź���*/
extern OS_EVENT    *Times_end ;                             /*ͬ�������ź���*/
extern OS_EVENT    *Times_end_NET;
///////////////////////////////////////////////////////////////////////////////////////////
extern OS_FLAG_GRP  *SYSMEMFLAG;                                  /*�洢���߼����Ʊ�־��*/
extern OS_FLAG_GRP  *SYSMEMFLAG_NET;
//
extern OS_MEM      *MEMPt;             //�ڴ����ָ��
//

#endif


