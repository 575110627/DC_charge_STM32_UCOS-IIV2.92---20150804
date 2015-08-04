#ifndef __CAN_H
#define __CAN_H

#include "stm32f2xx_hal.h"

struct _StrCAN
{
	 uint32_t  IDE;
	 uint8_t   DLC;
	 uint8_t   Data[8];
};

#define BRM_R_FLAG   0x0001  //0b0000000000000001   BMS和车辆辨识报文
#define BCP_R_FLAG   0x0002  //0b0000000000000010   动力蓄电池充电参数
#define BRO_R_FLAG   0x0004  //0b0000000000000100   电池充电准备就绪状态
#define BCL_R_FLAG   0x0008  //0b0000000000001000   电池充电需求
#define BCS_R_FLAG   0x0010  //0b0000000000010000   电池充电总状态
#define BSM_R_FLAG   0x0020  //0b0000000000100000   动力蓄电池状态信息
#define BMV_R_FLAG   0x0040  //0b0000000001000000   单体动力蓄电池电压
#define BMT_R_FLAG   0x0080  //0b0000000010000000   动力蓄电池温度
#define BSP_R_FLAG   0x0100  //0b0000000100000000   动力蓄电池预留报文
#define BST_R_FLAG   0x0200  //0b0000001000000000   BMS中止充电
#define BSD_R_FLAG   0x0400  //0b0000010000000000   BMS统计数据
#define BEM_R_FLAG   0x0800  //0b0000100000000000   BMS错误报文
#define DOS_R_FLAG   0x1000  //0b0001000000000000   DC电源状态报文

#define CRM_S_FLAG   0x0001  //0b0000000000000001   充电机辨识报文
#define CTS_S_FLAG   0x0002  //0b0000000000000010   充电机发送时间同步信息
#define CML_S_FLAG   0x0004  //0b0000000000000100   充电机最大输出能力
#define CRO_S_FLAG   0x0008  //0b0000000000001000   充电机输出准备就绪状态
#define CCS_S_FLAG   0x0010  //0b0000000000010000   充电机充电状态
#define CST_S_FLAG   0x0020  //0b0000000000100000   充电机中止充电
#define CSD_S_FLAG   0x0040  //0b0000000001000000   充电机统计数据
#define CEM_S_FLAG   0x0080  //0b0000000010000000   充电机错误报文
#define DPS_S_FLAG   0x0100  //0b0000000100000000   充电机参数设置
#define DSS_S_FLAG   0x0200  //0b0000001000000000   充电机启动停止

#define TRUE 1
#define FALSE 0

#define DCMODUENUMBER  3   //DC模块的数量
void CAN_Config(void);
void CAN_mEvent(void);       //CAN事件处理
void CAN_DataProcessing(void);    //CAN中断数据处理
void CANQueue_Reset(void);

void CAN_mInitData(void);//初始化CAn通信中需求的数据，由其他模块提供的数据则由其他模块初始化
uint8_t CAN_mSend_CCS_ChargerChargeState(void);  //充电机发送充电状态报文
uint8_t CAN_mSendDCModuleStartOrStop(uint8_t modueaddress);   //	充电机向直流充电模块发送启动停止命令
uint8_t CAN_mSend_CST_ChargerStop(void);  //充电机发送充电机中止充电报文

void CANtoBMS_Config(void);
void CANqueue_push(struct _StrCAN StrCAN);

uint16_t CAN_mAccumulateChargeTime(void);     //计算充电时常，返回时间长度，分钟为单位
void ChargeOfManualOperation(void);     //手动充电
uint8_t BCDToChar(uint8_t bcd);


#endif /* __CAN_H */

