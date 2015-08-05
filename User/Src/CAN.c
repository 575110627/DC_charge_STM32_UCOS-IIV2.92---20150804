/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

#define CANQUEUEMAXSIZE  200



typedef struct _CANQUEUE
{
	 uint8_t Head;
	 uint8_t Tail;
	 struct _StrCAN CANtoBMS[CANQUEUEMAXSIZE];
}CANQUEUE;

CANQUEUE CANQueue;
struct _StrCAN CANBMS1;

//入队
void CANqueue_push(struct _StrCAN StrCAN)
{
	 uint8_t i,pos=0;
	 pos = (CANQueue.Head+1)%CANQUEUEMAXSIZE;
	 if(pos != CANQueue.Head) //非空状态
	 {
		  CANQueue.CANtoBMS[CANQueue.Head].IDE = StrCAN.IDE;
		  CANQueue.CANtoBMS[CANQueue.Head].DLC = StrCAN.DLC;
		  for(i=0;i<StrCAN.DLC;i++)
		  {
				CANQueue.CANtoBMS[CANQueue.Head].Data[i] = StrCAN.Data[i];
			}				
		  CANQueue.Head = pos;
	 }
}
//出队
uint8_t CANQueue_pop(struct _StrCAN* StrCAN)
{ 
	uint8_t i;
	if(CANQueue.Head != CANQueue.Tail)//队非空
	{
		StrCAN->IDE = CANQueue.CANtoBMS[CANQueue.Tail].IDE;
		StrCAN->DLC = CANQueue.CANtoBMS[CANQueue.Tail].DLC;
		for(i=0;i<StrCAN->DLC;i++)
		{
			StrCAN->Data[i] = CANQueue.CANtoBMS[CANQueue.Tail].Data[i];
		}
		CANQueue.Tail = (CANQueue.Tail +1)%CANQUEUEMAXSIZE;
		return 1;
	}
	else 
		return 0;	
}
void CANQueue_Reset()
{
	uint8_t i=0;
	for(i=0;i<200;i++)
	{
		 CANQueue.CANtoBMS[i].IDE = 0;
	}
}
/*------------------------------------------------------
---------------------函数声明---------------------------
--------------------------------------------------------*/
void CAN_Config(void);
void CANtoBMS_Config(void);
void CANtoDC_Config(void);
void CAN_mInitData(void);//初始化CAn通信中需求的数据，由其他模块提供的数据则由其他模块初始化
static uint8_t CAN_mSend_CRM_ChargerIdentify(void);  //充电机发送充电机辨识报文
static uint8_t CAN_mSend_CTS_ChargerTime(void);  // 充电机发送时间同步信息报文
static uint8_t CAN_mSend_CML_ChargerMaxOutputCapactity(void);  //充电机发送最大输出能力报文
static uint8_t CAN_mSend_CRO_ChargerOutputReady(void);  //充电机发送充电机输出准备就绪状态报文
uint8_t CAN_mSend_CCS_ChargerChargeState(void);  //充电机发送充电状态报文
uint8_t CAN_mSend_CST_ChargerStop(void);  //充电机发送充电机中止充电报文
static uint8_t CAN_mSend_CSD_ChargerStatisticsData(void);  //  充电机发送充电机统计数据
static uint8_t CAN_mSend_CEM_ChargerError(void);  // 充电机发送错误报文
static uint8_t CAN_mSendDCModuleSetOutputParameter(uint8_t modueaddress);  //	充电机向直流充电模块发送控制报文
uint8_t CAN_mSendDCModuleStartOrStop(uint8_t modueaddress);   //	充电机向直流充电模块发送启动停止命令
static uint8_t CAN_mSendDCModuleReadStute(uint8_t modueaddress);  //	充电机向直流充电模块发送参数设置命令

static void CAN_mCANBufReset_BMS(void);//CAN接收BMS数据缓冲器数据复位
static void CAN_mCANBufReset_DC(void);//CAN接收DC数据缓冲器数据复位
void CAN_mEvent(void);       //CAN事件处理
void CAN_DataProcessing(void);    //CAN中断数据处理
static void CAN_mEventProc1(void);       //CAN事件处理1,处理 充电机辨识报文  报文代号CRM
static void CAN_mEventProc2(void);       //CAN事件处理2,处理 BMS和车辆辨识报文  报文代号BRM
static void CAN_mEventProc3(void);       //CAN事件处理3,处理 动力蓄电池充电参数报文  报文代号BCP
static void CAN_mEventProc4(void);       //CAN事件处理4,处理 充电机发送时间同步信息报文  报文代号CTS
static void CAN_mEventProc5(void);       //CAN事件处理5,处理 充电机最大输出能力报文  报文代号CML
static void CAN_mEventProc6(void);       //CAN事件处理6,处理 电池充电准备就绪状态报文  报文代号BRO
static void CAN_mEventProc7(void);       //CAN事件处理7,处理 充电机输出准备就绪状态报文  报文代号CRO
static void CAN_mEventProc8(void);       //CAN事件处理8,处理 电池充电需求报文  报文代号BCL
static void CAN_mEventProc9(void);       //CAN事件处理9,处理 电池充电总状态报文  报文代号BCS
static void CAN_mEventProc10(void);       //CAN事件处理10,处理 充电机充电状态报文 报文代号CCS
static void CAN_mEventProc11(void);       //CAN事件处理11,处理 动力蓄电池状态信息报文 报文代号BSM
static void CAN_mEventProc12(void);       //CAN事件处理12,处理 单体动力蓄电池电压报文 报文代号BMV
static void CAN_mEventProc13(void);       //CAN事件处理13,处理 动力蓄电池温度报文 报文代号BMT
static void CAN_mEventProc14(void);       //CAN事件处理14,处理 动力蓄电池预留报文 报文代号BSP
static void CAN_mEventProc15(void);       //CAN事件处理15,处理 BMS中止充电报文 报文代号BST
static void CAN_mEventProc16(void);       //CAN事件处理16,处理 充电机中止充电报文 报文代号CST
static void CAN_mEventProc17(void);       //CAN事件处理17,处理 BMS统计报文 报文代号BSD
static void CAN_mEventProc18(void);       //CAN事件处理18,处理 充电机统计报文 报文代号CSD
static void CAN_mEventProc19(void);       //CAN事件处理19,处理 BMS错误报文 报文代号BEM
static void CAN_mEventProc20(void);       //CAN事件处理20,处理 充电机错误报文 报文代号CEM
static void CAN_mEventProc21(void);       //CAN事件处理21,处理 主板给电源模块发送需求报文 报文代号CQM
static void CAN_mEventProc22(void);       //CAN事件处理22,处理 电源模块给主板发送状态信息 报文代号MSM

uint16_t CAN_mAccumulateChargeTime(void);     //计算充电时常，返回时间长度，分钟为单位
void ChargeOfManualOperation(void);     //手动充电

/*------------------------------------------------------
---------------------变量声明---------------------------
--------------------------------------------------------*/
extern CAN_HandleTypeDef    CanHandleToBMS;
extern CAN_HandleTypeDef    CanHandleToDC;
uint8_t CANCount=0;
/*-----------------------------------------------------------------------
---------------------函数定义--------------------------------------------
-------------------------------------------------------------------------*/
/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */

//CAN初始化
void CAN_Config(void)
{
		CANtoBMS_Config();
		CANtoDC_Config();
}

void CANtoBMS_Config(void)
{
	
	CAN_FilterConfTypeDef  sFilterConfig;
  static CanTxMsgTypeDef        TxMessage1;
  static CanRxMsgTypeDef        RxMessage1;
	
  CanHandleToBMS.Instance = CAN1;
  CanHandleToBMS.pTxMsg = &TxMessage1;
  CanHandleToBMS.pRxMsg = &RxMessage1;
	//CAN 初始化 30MHz 250kbps
  CanHandleToBMS.Init.TTCM = DISABLE;
  CanHandleToBMS.Init.ABOM = ENABLE;
  CanHandleToBMS.Init.AWUM = DISABLE;
  CanHandleToBMS.Init.NART = DISABLE;
  CanHandleToBMS.Init.RFLM = ENABLE;
  CanHandleToBMS.Init.TXFP = DISABLE;
  CanHandleToBMS.Init.Mode = CAN_MODE_NORMAL;
  CanHandleToBMS.Init.SJW = CAN_SJW_1TQ;
  CanHandleToBMS.Init.BS1 = CAN_BS1_6TQ;
  CanHandleToBMS.Init.BS2 = CAN_BS2_8TQ;
  CanHandleToBMS.Init.Prescaler = 8;
  HAL_CAN_Init(&CanHandleToBMS);
 
	//CAN过滤器
  /*##-2- Configure the CAN Filter ###########################################*/
  sFilterConfig.FilterNumber = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.BankNumber = 14;

  HAL_CAN_ConfigFilter(&CanHandleToBMS, &sFilterConfig);

	//CAN发送设置
  /*##-3- Configure Transmission process #####################################*/
  CanHandleToBMS.pTxMsg->StdId = 0x0321;
  CanHandleToBMS.pTxMsg->ExtId = 0x181C56F4;
  CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToBMS.pTxMsg->DLC = 8;
	HAL_CAN_Receive_IT(&CanHandleToBMS, CAN_FIFO0);
}
void CANtoDC_Config(void)              //过滤器设置  1207c081 1207c082 1207c083
{
	CAN_FilterConfTypeDef  sFilterConfig;
  static CanTxMsgTypeDef        TxMessage;
  static CanRxMsgTypeDef        RxMessage;
  /*##-1- Configure the CAN peripheral #######################################*/
  CanHandleToDC.Instance = CAN2;
  CanHandleToDC.pTxMsg = &TxMessage;
  CanHandleToDC.pRxMsg = &RxMessage;
	//CAN 初始化 30MHz 125kbps
  CanHandleToDC.Init.TTCM = DISABLE;
  CanHandleToDC.Init.ABOM = DISABLE;
  CanHandleToDC.Init.AWUM = DISABLE;
  CanHandleToDC.Init.NART = DISABLE;
  CanHandleToDC.Init.RFLM = DISABLE;
  CanHandleToDC.Init.TXFP = DISABLE;
  CanHandleToDC.Init.Mode = CAN_MODE_NORMAL;
  CanHandleToDC.Init.SJW = CAN_SJW_1TQ;
  CanHandleToDC.Init.BS1 = CAN_BS1_6TQ;
  CanHandleToDC.Init.BS2 = CAN_BS2_8TQ;
  CanHandleToDC.Init.Prescaler = 16;
  HAL_CAN_Init(&CanHandleToDC);
  
	//CAN过滤器          //过滤器设置  1207c081 1207c082 1207c083
  /*##-2- Configure the CAN Filter ###########################################*/
  sFilterConfig.FilterNumber = 14;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = (((uint32_t)0x1207C080<<3)&0xFFFF0000)>>16;
  sFilterConfig.FilterIdLow =  (((uint32_t)0x1207C080<<3)|CAN_RTR_DATA|CAN_ID_EXT)&0xFFFF;
  sFilterConfig.FilterMaskIdHigh = 0xFFFF;
  sFilterConfig.FilterMaskIdLow = 0xFF80;
  sFilterConfig.FilterFIFOAssignment = CAN_FIFO1;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.BankNumber = 14;

  HAL_CAN_ConfigFilter(&CanHandleToDC, &sFilterConfig);
 
	//CAN发送设置
  /*##-3- Configure Transmission process #####################################*/
  CanHandleToDC.pTxMsg->StdId = 0x0320;
  CanHandleToDC.pTxMsg->ExtId = 0x1307C081;
  CanHandleToDC.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToDC.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToDC.pTxMsg->DLC = 8;
  /*##-1- Configure the CAN peripheral #######################################*/

	HAL_CAN_Receive_IT(&CanHandleToDC, CAN_FIFO1);
}
//初始化CAn通信中需求的数据，由其他模块提供的数据则由其他模块初始化
void CAN_mInitData(void)
{
		 uint8_t i;
		 CANQueue.Head = 0;
     CANQueue.Tail = 0;
		 uwCurrentRequirementLowbuf = 0;
		 uwCurrentRequirementHighbuf = 0;
	   FlagIfReceBmsStopChargeMeg = 0;
     ubCANTim3Count = 0;
     ubFlagIfReceBmsIdentifyMeg = 0;   //标志，是否收到BMS发送的电池充电参数报文 0x00未收到，0xAA收到
     FlagIfReceBmsReadyMeg = 0;   //标志，是否收到BMS发送的电池充电准备就绪状态报文 0未收到，1收到
     FlagIfReceBmsChargeRequirementMeg = 0;  //标志，是否收到BMS发送的电池充电需求报文 0x00未收到，0xAA收到
     FlagIfSendDCModuleMeg = 0;  //标志，是否收到DC直流充电机模块发送的报文 0没收到，1收到
     FlagIfSendDCMeg = 0;
		 FlagIfDCTimeout = 0;    //标志，DC电源模块是否超时5s 0x00不满足，0xAA满足
     FlagIfReceBmsBatteryStateMeg = 0;  //标志，是否收到BMS发送的动力蓄电池状态 0x00未收到，0xAA收到
     FlagIfReceBmsStopChargeMeg = 0;//标志，是否收到BMS发送的BMS中止充电报文 0x00未收到，0xAA收到
     FlagIfReceBmsStatisticsDataMeg = 0;  //标志，是否收到BMS发送的充电统计数据报文 0x00未收到，0xAA收到
     uwCANReceFlagState = 0x0000;//是否收到CAN数据帧标志位 0表示无效 1表示有效  
                                 //第0位 BMS和车辆辨识报文
                                 //第1位 动力蓄电池充电参数
                                 //第2位 电池充电准备就绪报文
                                 //第3位 电池充电需求
                                 //第4位 电池充电总状态
                                 //第5位 动力蓄电池状态信息
                                 //第6位 单体动力蓄电池电压
                                 //第7位 动力蓄电池温度
                                 //第8位 动力蓄电池预留报文
                                 //第9位 BMS中止充电
                                 //第10位 BMS统计报文
                                 //第11位 BMS错误报文
     //CAN数据接收缓冲器初始化
    // ubCANBufIndex_BMS = 0;
		 ubCANBufIndex_DC = 0;
     //CAN数据周期发送标志,判断是否超时
     FlagIfCycleSend10ms = 0;
     FlagIfCycleSend50ms = 0;
     FlagIfCycleSend250ms = 0; 
     FlagIfCycleSend500ms = 0;
     FlagIfCycleSend1000ms = 0;
     FlagIfTimeout = 0;
		 uwDCMaxOutputVoltage = 4200;
		 uwDCMinOutputVoltage = 2000;
		 uwDCMaxOutputCurrent = 4000-270;
		 ubTimeN100ms = 50;
     //CAN模块独立产生数据的初始化
     ubRecognitionResult = 0x00;
     ubChargerNumber = 0x10;
     for(i=0;i<4;i++)
     {
          ubBmsError[i] = 0x00;
          ubChargerError[i] = 0x00;  
     }
		 CAN_mCANBufReset_BMS();
		 CAN_mCANBufReset_DC();
		 ubCANEventStatus = 1; 
		 ubDCModueAddress = 0x01;//默认CAN模块是0x01
		 uwDCOutputCurrent[1] = 0;
		 uwDCOutputCurrent[2] = 0;
		 uwDCOutputCurrent[3] = 0;
		 uwDCOutputVoltage[1] = 0;
		 
}

//充电机发送充电机辨识报文
uint8_t CAN_mSend_CRM_ChargerIdentify(void)
{
  CanHandleToBMS.pTxMsg->ExtId = 0x1801F456;
  CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToBMS.pTxMsg->DLC = 8;
	ubChargerNumber = 0x10;         //充电机编号
	CanHandleToBMS.pTxMsg->Data[0] = ubRecognitionResult;
	CanHandleToBMS.pTxMsg->Data[1] = ubChargerNumber;
	CanHandleToBMS.pTxMsg->Data[2] = 0xFF;
	CanHandleToBMS.pTxMsg->Data[3] = 0xFF;
	CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
	CanHandleToBMS.pTxMsg->Data[5] = 0xFF;
	CanHandleToBMS.pTxMsg->Data[6] = 0xFF;
	CanHandleToBMS.pTxMsg->Data[7] = 0xFF;
	if(HAL_CAN_Transmit(&CanHandleToBMS, 0xff) == HAL_OK)
	    return  TRUE;
	else
		  return FALSE;	
}

// 充电机发送时间同步信息报文
uint8_t CAN_mSend_CTS_ChargerTime(void)
{
	CanHandleToBMS.pTxMsg->ExtId = 0x1807F456;
  CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToBMS.pTxMsg->DLC = 7;
	CanHandleToBMS.pTxMsg->Data[0] = RTCReadBCDTime.Second;//0x00;
	CanHandleToBMS.pTxMsg->Data[1] = RTCReadBCDTime.Minute;//0x00;
	CanHandleToBMS.pTxMsg->Data[2] = RTCReadBCDTime.Hour;//0x00;
	CanHandleToBMS.pTxMsg->Data[3] = RTCReadBCDTime.Day;  //0x00;
	CanHandleToBMS.pTxMsg->Data[4] = RTCReadBCDTime.Month;//0x00;
	CanHandleToBMS.pTxMsg->Data[5] = RTCReadBCDTime.Year; //0x00;
	CanHandleToBMS.pTxMsg->Data[6] = 0x20; //0x20;
	if(HAL_CAN_Transmit(&CanHandleToBMS, 0xff) == HAL_OK)
	    return  TRUE;
	else
		  return FALSE;
}

//充电机发送最大输出能力报文
uint8_t CAN_mSend_CML_ChargerMaxOutputCapactity(void)
{
	CanHandleToBMS.pTxMsg->ExtId = 0x1808F456;
  CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToBMS.pTxMsg->DLC = 6;
	CanHandleToBMS.pTxMsg->Data[0] = (uint8_t)(uwDCMaxOutputVoltage&0xFF);
	CanHandleToBMS.pTxMsg->Data[1] = (uint8_t)((uwDCMaxOutputVoltage>>8)&0xFF);
	CanHandleToBMS.pTxMsg->Data[2] = (uint8_t)(uwDCMinOutputVoltage&0xFF);
	CanHandleToBMS.pTxMsg->Data[3] = (uint8_t)((uwDCMinOutputVoltage>>8)&0xFF);
	CanHandleToBMS.pTxMsg->Data[4] = (uint8_t)(uwDCMaxOutputCurrent&0xFF);
	CanHandleToBMS.pTxMsg->Data[5] = (uint8_t)((uwDCMaxOutputCurrent>>8)&0xFF);
	if(HAL_CAN_Transmit(&CanHandleToBMS, 0xff) == HAL_OK)
	    return TRUE;
	else
		  return FALSE;
}

//充电机发送充电机输出准备就绪状态报文
uint8_t CAN_mSend_CRO_ChargerOutputReady(void)
{
	CanHandleToBMS.pTxMsg->ExtId = 0x100AF456;
  CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToBMS.pTxMsg->DLC = 1;
	CanHandleToBMS.pTxMsg->Data[0] = ubChargerOutputReady;
	if(HAL_CAN_Transmit(&CanHandleToBMS, 0xff) == HAL_OK)
	    return  TRUE;
	else
		  return FALSE;
}

//充电机发送充电状态报文
uint8_t CAN_mSend_CCS_ChargerChargeState(void)
{
	uwAccumulateChargeTime = ChargeTime.Hour *60 +ChargeTime.Minute;
	CanHandleToBMS.pTxMsg->ExtId = 0x1812F456;
  CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToBMS.pTxMsg->DLC = 6;
	CanHandleToBMS.pTxMsg->Data[0] = (uint8_t)(uwOutputVoltageValue&0xFF);
	CanHandleToBMS.pTxMsg->Data[1] = (uint8_t)((uwOutputVoltageValue>>8)&0xFF);
	CanHandleToBMS.pTxMsg->Data[2] = (uint8_t)((4000-uwOutputCurrentValue)&0xFF);
	CanHandleToBMS.pTxMsg->Data[3] = (uint8_t)(((4000-uwOutputCurrentValue)>>8)&0xFF);
	CanHandleToBMS.pTxMsg->Data[4] = (uint8_t)(uwAccumulateChargeTime&0xFF);
	CanHandleToBMS.pTxMsg->Data[5] = (uint8_t)((uwAccumulateChargeTime>>8)&0xFF);
	if(HAL_CAN_Transmit(&CanHandleToBMS, 0xff) == HAL_OK)
	    return  TRUE;
	else
		  return FALSE;
}

//充电机发送充电机中止充电报文
uint8_t CAN_mSend_CST_ChargerStop(void)
{
		CanHandleToBMS.pTxMsg->ExtId = 0x101AF456;
		CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
		CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
		CanHandleToBMS.pTxMsg->DLC = 4;
		CanHandleToBMS.pTxMsg->Data[0] = ubChargerStopReason;
		CanHandleToBMS.pTxMsg->Data[1] = (uint8_t)(uwChargerStopFaultReason&0xFF);
		CanHandleToBMS.pTxMsg->Data[2] = (uint8_t)((uwChargerStopFaultReason>>8)&0xFF);
		CanHandleToBMS.pTxMsg->Data[3] = ubChargerStopErrorReason;
		if(HAL_CAN_Transmit(&CanHandleToBMS, 0xff) == HAL_OK)
				return  TRUE;
		else
				return FALSE;
}

// 充电机发送充电机统计数据
uint8_t CAN_mSend_CSD_ChargerStatisticsData(void)
{
	uwChargerEndChargeTime = ChargeTime.Hour *60 +ChargeTime.Minute;
	uwChargerEndChargePower = (DealEnergy[2]<<8) + DealEnergy[3];
	CanHandleToBMS.pTxMsg->ExtId = 0x181DF456;
  CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToBMS.pTxMsg->DLC = 5;
	CanHandleToBMS.pTxMsg->Data[0] = (uint8_t)(uwChargerEndChargeTime&0xFF);
	CanHandleToBMS.pTxMsg->Data[1] = ((uint8_t)((uwChargerEndChargeTime>>8)&0xFF));
	CanHandleToBMS.pTxMsg->Data[2] = (uint8_t)(uwChargerEndChargePower&0xFF);
	CanHandleToBMS.pTxMsg->Data[3] = ((uint8_t)((uwChargerEndChargePower>>8)&0xFF));
	CanHandleToBMS.pTxMsg->Data[4] = ubChargerNumber;
	if(HAL_CAN_Transmit(&CanHandleToBMS, 0xff) == HAL_OK)
	    return  TRUE;
	else
		  return FALSE;
}
	
// 充电机发送错误报文	
uint8_t CAN_mSend_CEM_ChargerError(void)
{
	CanHandleToBMS.pTxMsg->ExtId = 0x081F56F4;
  CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToBMS.pTxMsg->DLC = 4;
	CanHandleToBMS.pTxMsg->Data[0] = ubChargerError[0];
	CanHandleToBMS.pTxMsg->Data[1] = ubChargerError[1];
	CanHandleToBMS.pTxMsg->Data[2] = ubChargerError[2];
	CanHandleToBMS.pTxMsg->Data[3] = ubChargerError[3];
	if(HAL_CAN_Transmit(&CanHandleToBMS, 0xff) == HAL_OK)
	    return  TRUE;
	else
		  return FALSE;
}

 //	向直流充电模块发送设定输出控制报文
uint8_t CAN_mSendDCModuleSetOutputParameter(uint8_t modueaddress)
{
	if(0 == ChargingMethod)
	{
				uwCurrentRequirementbuf = uwCurrentRequirementLowbuf + (uwCurrentRequirementHighbuf<<8);
				uwVoltageRequirement = uwVoltageRequirementLow + (uwVoltageRequirementHigh<<8);
				uwCurrentRequirement = 4000- uwCurrentRequirementbuf;
				if(uwCurrentRequirement >= (90*DCMODUENUMBER))  //模块限流
				{
					uwCurrentRequirement = (90*DCMODUENUMBER);
				}
				if(uwCurrentRequirement >= uwMaxAllowChargeCurrent)  //模块限流
				{
					uwCurrentRequirement = uwMaxAllowChargeCurrent;
				}
				uwCurrentRequirementHigh = (uint8_t)(((uwCurrentRequirement*100/DCMODUENUMBER)>>8)&0xFF);
				uwCurrentRequirementLow = (uint8_t)((uwCurrentRequirement*100/DCMODUENUMBER)&0xFF);
				CanHandleToDC.pTxMsg->ExtId = 0x1307C080 + modueaddress;
				CanHandleToDC.pTxMsg->RTR = CAN_RTR_DATA;
				CanHandleToDC.pTxMsg->IDE = CAN_ID_EXT;
				CanHandleToDC.pTxMsg->DLC = 8;
				CanHandleToDC.pTxMsg->Data[0] = 0x00;
				CanHandleToDC.pTxMsg->Data[1] = 0x00;
				CanHandleToDC.pTxMsg->Data[2] = uwCurrentRequirementHigh;
				CanHandleToDC.pTxMsg->Data[3] = uwCurrentRequirementLow;
				CanHandleToDC.pTxMsg->Data[4] = (uint8_t)(((uwVoltageRequirement*100)>>24)&0xFF);
				CanHandleToDC.pTxMsg->Data[5] = (uint8_t)(((uwVoltageRequirement*100)>>16)&0xFF);
				CanHandleToDC.pTxMsg->Data[6] = (uint8_t)(((uwVoltageRequirement*100)>>8)&0xFF);
				CanHandleToDC.pTxMsg->Data[7] = (uint8_t)((uwVoltageRequirement*100)&0xFF);
				if(HAL_CAN_Transmit(&CanHandleToDC, 0xff) == HAL_OK)
						return  TRUE;
				else
						return FALSE;
		}
		else if(1 == ChargingMethod)   //手动充电
		{
				uwCurrentRequirementHigh = (uint8_t)((SetChargeCurrent*1000/DCMODUENUMBER)/256);
				uwCurrentRequirementLow = (uint8_t)((SetChargeCurrent*1000/DCMODUENUMBER)%256);
				CanHandleToDC.pTxMsg->ExtId = 0x1307C080 + modueaddress;
				CanHandleToDC.pTxMsg->RTR = CAN_RTR_DATA;
				CanHandleToDC.pTxMsg->IDE = CAN_ID_EXT;
				CanHandleToDC.pTxMsg->DLC = 8;
				CanHandleToDC.pTxMsg->Data[0] = 0x00;
				CanHandleToDC.pTxMsg->Data[1] = 0x00;
				CanHandleToDC.pTxMsg->Data[2] = uwCurrentRequirementHigh;
				CanHandleToDC.pTxMsg->Data[3] = uwCurrentRequirementLow;
				CanHandleToDC.pTxMsg->Data[4] = 0x00;
				CanHandleToDC.pTxMsg->Data[5] = (uint8_t)((SetChargeVoltage*1000)/256/256);
				CanHandleToDC.pTxMsg->Data[6] = (uint8_t)((SetChargeVoltage*1000)/256%256);
				CanHandleToDC.pTxMsg->Data[7] = (uint8_t)((SetChargeVoltage*1000)%256);
				if(HAL_CAN_Transmit(&CanHandleToDC, 0xff) == HAL_OK)
						return  TRUE;
				else
						return FALSE;
			
		}
		else
			return FALSE;
}

//	充电机向直流充电模块发送启动停止充电机命令
uint8_t CAN_mSendDCModuleStartOrStop(uint8_t modueaddress) 
{
  CanHandleToDC.pTxMsg->ExtId = 0x1307C080 +modueaddress;
  CanHandleToDC.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToDC.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToDC.pTxMsg->DLC = 8;
	
  CanHandleToDC.pTxMsg->Data[0] = 0x02;//协议中的CMD=2
	CanHandleToDC.pTxMsg->Data[1] = 0x00;
	CanHandleToDC.pTxMsg->Data[2] = 0x00;
	CanHandleToDC.pTxMsg->Data[3] = 0x00;
	CanHandleToDC.pTxMsg->Data[4] = 0x00;
	CanHandleToDC.pTxMsg->Data[5] = 0x00;
	CanHandleToDC.pTxMsg->Data[6] = 0x00;
	CanHandleToDC.pTxMsg->Data[7] = ubDCModue;
	if(HAL_CAN_Transmit(&CanHandleToDC, 0xff) == HAL_OK)
	    return  TRUE;
	else
		  return FALSE;
}

//	充电机向直流充电模块发送读取信息报文
uint8_t CAN_mSendDCModuleReadStute(uint8_t modueaddress)
{
	CanHandleToDC.pTxMsg->ExtId = 0x1307C080 + modueaddress;
	CanHandleToDC.pTxMsg->RTR = CAN_RTR_DATA;
	CanHandleToDC.pTxMsg->IDE = CAN_ID_EXT;
	CanHandleToDC.pTxMsg->DLC = 8;
	CanHandleToDC.pTxMsg->Data[0] = 0x01;
	CanHandleToDC.pTxMsg->Data[1] = 0x00;
	CanHandleToDC.pTxMsg->Data[2] = 0x00;
	CanHandleToDC.pTxMsg->Data[3] = 0x00;
	CanHandleToDC.pTxMsg->Data[4] = 0x00;
	CanHandleToDC.pTxMsg->Data[5] = 0x00;
	CanHandleToDC.pTxMsg->Data[6] = 0x00;
	CanHandleToDC.pTxMsg->Data[7] = 0x00;
	if(HAL_CAN_Transmit(&CanHandleToDC, 0xff) == HAL_OK)
	    return  TRUE;
	else
		  return FALSE;
}

//CAN接收BMS数据缓冲器数据复位
void CAN_mCANBufReset_BMS(void)
{
	uint8_t i;
	for(i=0;i<70;i++)
	{
		ubCANMultiFrame[i][0]=0;
	}
}

//CAN接收DC数据缓冲器数据复位
void CAN_mCANBufReset_DC(void)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		ubCANBuf_DC[i] = 0;
	}
}
	
//CAN事件处理
void CAN_mEvent(void)
{

   uint8_t i=0;
   uint8_t j=0;
   FlagIfTimeout = 0;
   if(FlagIfSendDCModuleMeg == 1)               //是否发送DC电源模块，在充电机输出准备就绪状态发送之后启动输出 令标志位有效
   { 
		if(FlagIfSendDCMeg ==1)
		{
					FlagIfSendDCMeg = 0;				
					switch(CANCount)
						{
							case 0:  //	向直流充电模块发送设定输出控制报文,广播发送
									CAN_mSendDCModuleSetOutputParameter(0x00);
									break;
							case 1: 							
									CAN_mSendDCModuleReadStute(ubDCModueAddress);//分别读取多个模块
									ubDCModueAddress++;//读取模块地址加1，切换模块
									if(ubDCModueAddress == (DCMODUENUMBER+1))
									{
										ubDCModueAddress = 1;
									}
									break;
							case 2: //	充电机向直流充电模块发送启动停止充电机命令，广播发送
									CAN_mSendDCModuleStartOrStop(0x00); 
									break;
							default:  
									break;
						}	
					CANCount++;				
					if(CANCount == 3)
					{
						CANCount = 0;	
					}						
				}			
  }
   for(j=0;j<50;j++)
	 {
		switch(ubCANEventStatus)
		{
			case 1: CAN_mEventProc1( );
					break;
			case 2: CAN_mEventProc2( );
					break;
			case 3: CAN_mEventProc3( );
					break;
			case 4: CAN_mEventProc4( );
					break;
			case 5: CAN_mEventProc5( );
					break;
			case 6: CAN_mEventProc6( );
					break;
			case 7: CAN_mEventProc7( );
					break;
			case 8: CAN_mEventProc8( );
					break;
			case 9: CAN_mEventProc9( );
					break;
			case 10:CAN_mEventProc10( );
					break;                 
			case 11:CAN_mEventProc11( );
					break;
			case 12:CAN_mEventProc12( );
					break;
			case 13:CAN_mEventProc13( );
					break;
			case 14:CAN_mEventProc14( );
					break;
			case 15:CAN_mEventProc15( );
					break;
			case 16:CAN_mEventProc16( );
					break;
			case 17:CAN_mEventProc17( );
					break;
			case 18:CAN_mEventProc18( );
					break;
			case 19:CAN_mEventProc19( );
					break;
			case 20:CAN_mEventProc20( );
					break;
			case 21:CAN_mEventProc21( );
					break;
			case 22:CAN_mEventProc22( );
					break;
			default:
					break;
        }
	    if(!(CANQueue_pop(&CANBMS1)))
			{
			 break;
			}
		 else
		  {
			 switch(CANBMS1.IDE)
				{
					//多帧处理,连接处理
					case 0x1CEC56F4:
					{
						  ubCANMultiFrameIndex = 0;
						  CAN_mCANBufReset_BMS();
						  ubManyBytesStateBCS = 0;//多帧数据的标志
							ubManyBytesStateBMV = 0;//多帧数据的标志
							ubManyBytesStateBMT = 0;//多帧数据的标志
							ubManyBytesStateBSP = 0;//多帧数据的标志
							for(i=0;i<CANBMS1.DLC;i++)
							{
								ubCANConnect[i] = CANBMS1.Data[i];
								
							}
							switch(ubCANConnect[6])
							{
								//车辆辨识报文BRM  
								case 0x02:    //接收到BMS和车辆的辨识报文
										 ubFlagIfReceBmsIdentifyMeg = 1;
									break;
								//动力蓄电池充电参数BCP
								case 0x06:   //接收到充电参数报文
										 ubCANEventStatus = 3;
										 ubFlagIfReceBatteryParameterMeg = 1;
									break;
								//电池充电总状态 BCS
								case 0x11:   
										 ubManyBytesStateBCS = 1;
														 //接收到充电总状态
										 //FlagIfSendDCModuleMeg = 1;
							       ubCANEventStatus = 10;
										 ubFlagIfReceBmsAllChargeStateMeg = 1;  
									break;
								//单体动力蓄电池电压BMV
								case 0x15:   
									    // FlagIfSendDCModuleMeg = 1;
										 ubCANEventStatus = 10;
										 ubManyBytesStateBMV = 1;
									break;
								//蓄电池组温度BMT
								case 0x16:   
										 //FlagIfSendDCModuleMeg = 1;
										 ubCANEventStatus = 10;	
										 ubManyBytesStateBMT = 1;
									break;
								//动力蓄电池预留报文
								case 0x17:   
										// FlagIfSendDCModuleMeg = 1;
										 ubCANEventStatus = 10;	
										 ubManyBytesStateBSP = 1;
									break;
								//其他
								default:
									break;	
								}
					}									
						 break;
				 case 0x1CEB56F4:                //
				   {
							for(i=0;i<CANBMS1.DLC;i++)
							{
								ubCANMultiFrame[ubCANMultiFrameIndex][i] = CANBMS1.Data[i];										
							}
							ubCANMultiFrameIndex ++;
					}
						break;
					//BMS和车辆辨识报文BRM
				 case 0x180256F4:   
						break;
					//动力蓄电池充电参数BCP
				 case 0x180656F4:
						break;
					//充电机准备就绪报文 BRO
				 case 0x100956F4:       
							//ubCANBuf_BMS[0][0] = SW_MO5.ubData[0]; 
							ubBmsChargeReady = CANBMS1.Data[0]; 
							FlagIfReceBmsReadyMeg = 1;   //收到BMS准备就绪报文
							break;		
					//电池充电需求 BCL
				 case 0x181056F4: 
							//电压需
							uwVoltageRequirementLow	= (int8_t)(CANBMS1.Data[0]);
							uwVoltageRequirementHigh = (int8_t)(CANBMS1.Data[1]); 
							//电流需求	
							uwCurrentRequirementLowbuf =(int8_t) (CANBMS1.Data[2]);
							uwCurrentRequirementHighbuf =(int8_t) (CANBMS1.Data[3]);
							ubChargeMode = CANBMS1.Data[4];
							FlagIfReceBmsChargeRequirementMeg = 1;   //接受到BMS充电需求报文
							FlagIfSendDCModuleMeg = 1;
							ubCANEventStatus = 10;
							break;
					//BMS动力蓄电池状态信息 BSM
					case 0x181356F4:
							ubMaxCellVoltageNumber = CANBMS1.Data[0];
							ubMaxTemperature = CANBMS1.Data[1];
							ubMaxTemperatureNumber = CANBMS1.Data[2];
							ubMinTemperature = CANBMS1.Data[3];
							ubMinTemperatureNumber = CANBMS1.Data[4];
							uwStatusAndAbnormal = (CANBMS1.Data[6]<<8) + CANBMS1.Data[5];
							ubCANEventStatus = 10;
							FlagIfReceBmsBatteryStateMeg = 1;   //接受到BMS电池状态信息
						break;
					//单体动力蓄电池电压BMV 
					case 0x181556F4:   
						break;
					//蓄电池组温度BMT   
					case 0x181656F4:
						break;
					//动力蓄电池预留报文BSP  
					case 0x181756F4:
						break;
					//BMS中止充电 BST
					case 0x101956F4:
							 ubBmsChargeStopReason = CANBMS1.Data[0];
							 uwBmsChargeStopFaultReason = (CANBMS1.Data[2]<<8)+CANBMS1.Data[1];
							 ubBmsChargeStopErrorReason = CANBMS1.Data[3];
							 FlagIfReceBmsStopChargeMeg = 1;
						break;
					//BMS统计数据报文 BSD
					case 0x181C56F4:
							ubBmsEndOfSOC = CANBMS1.Data[0];
							uwBmsEndMinCellVoltage = (CANBMS1.Data[2]<<8)+CANBMS1.Data[1];
							uwBmsEndMaxCellVoltage = (CANBMS1.Data[4]<<8)+CANBMS1.Data[3];
							ubBmsEndMinTemperature = CANBMS1.Data[5];
							ubBmsEndMaxTemperature = CANBMS1.Data[6];
							FlagIfReceBmsStatisticsDataMeg = 1;			
							ubCANEventStatus = 18;
						break;
					//BMS错误报文 BEM
					case 0x081E56F4:
							ubBmsError[0] = CANBMS1.Data[0];
							ubBmsError[1] = CANBMS1.Data[1];
							ubBmsError[2] = CANBMS1.Data[2];
							ubBmsError[3] = CANBMS1.Data[3];
						break;
					//其他
					default:
						break;
				}			 			
		 }
   }	
	CAN_DataProcessing();
}

//CAN事件处理1,处理 充电机辨识报文  报文代号CRM
void CAN_mEventProc1(void)
{
	  ubRecognitionResult = 0x00;        //辨识结果
    if(TRUE == CAN_mSend_CRM_ChargerIdentify())  //充电机发送充电机辨识报文
    {
        ubTimeN100ms = 50;              //设置5S超时   50*100ms=5000ms
        ubCANEventStatus = 2;          //直接转至下一个转态
    }
}      
	
//CAN事件处理2,处理 BMS和车辆辨识报文  报文代号BRM
void CAN_mEventProc2(void)
{
		uint8_t i;
	  //250ms未收到BMS返回的车辆辨识报文
    if(1 == FlagIfCycleSend250ms)     
    {
        if(TRUE == CAN_mSend_CRM_ChargerIdentify( ))  //充电机发送充电机辨识报文 
        {
            FlagIfCycleSend250ms = 0;
        }
    }
		//判断是否是多数据帧的通信连接 
    if(ubCANConnect[0] ==0x10)          
    {
         //多数据帧通信连接回复
				 CanHandleToBMS.pTxMsg->ExtId = 0x1CECF456;
				 CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
				 CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
				 CanHandleToBMS.pTxMsg->DLC = 8;
         CanHandleToBMS.pTxMsg->Data[0] = 0x11;      
         CanHandleToBMS.pTxMsg->Data[1] = ubCANConnect[3];
         CanHandleToBMS.pTxMsg->Data[2] = 0x01;
         CanHandleToBMS.pTxMsg->Data[3] = 0xFF;
         CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
         CanHandleToBMS.pTxMsg->Data[5] = ubCANConnect[5];
         CanHandleToBMS.pTxMsg->Data[6] = ubCANConnect[6];
         CanHandleToBMS.pTxMsg->Data[7] = ubCANConnect[7];
         ubCANBufIndex_BMS = 0;
         HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	充电机发送J1939多字节传输连接部分
         ubCANConnect[0] = 0x00;
    }
		//判断多数据帧数据是否接收完毕 
    if(ubCANMultiFrame[5][0] == 0x06)          
    {
         ulBmsProtocolVersion = (ubCANMultiFrame[0][3]<<16) + (ubCANMultiFrame[0][2]<<8)+ ubCANMultiFrame[0][1];
         ubBatteryType = ubCANMultiFrame[0][4];
		     uwBmsRateCapacity = (ubCANMultiFrame[0][6]<<8)+ ubCANMultiFrame[0][5];
         uwBatteryRateVoltage = (ubCANMultiFrame[1][1]<<8)+ ubCANMultiFrame[0][7];
         for(i=0;i<4;i++)
         {
             ubBatteryManufacturerName[i] = ubCANMultiFrame[1][2+i];
         }
         ulBatteryManufacturerName =  (ubCANMultiFrame[2][2]<<24) + (ubCANMultiFrame[2][1]<<16) + (ubCANMultiFrame[1][7]<<8) + ubCANMultiFrame[1][6];
         ubBatteryProduceYear = ubCANMultiFrame[2][3];
         ubBatteryProduceMonth = ubCANMultiFrame[2][4];
         ubBatteryProduceDay = ubCANMultiFrame[2][5];
         ulBatteryChargeCycle = (ubCANMultiFrame[3][1]<<16) + (ubCANMultiFrame[2][7]<<8)+ ubCANMultiFrame[2][6];
         ubPropertyMark = ubCANMultiFrame[3][2];
         ubBMSReserved = ubCANMultiFrame[3][3];
         for(i=0;i<4;i++)
         {
             ubVIN[i] =  ubCANMultiFrame[3][4+i];
         }
         for(i=4;i<11;i++)
         {
             ubVIN[i] =  ubCANMultiFrame[4][i-3];
         }
         for(i=11;i<17;i++)
         {
             ubVIN[i] =  ubCANMultiFrame[5][i-10];
         }
         //回复结束传输报文
         CanHandleToBMS.pTxMsg->Data[0] = 0x13;
         CanHandleToBMS.pTxMsg->Data[1] = 0x29;
         CanHandleToBMS.pTxMsg->Data[2] = 0x00;
         CanHandleToBMS.pTxMsg->Data[3] = 0x06;
         CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
         CanHandleToBMS.pTxMsg->Data[5] = 0x00;
         CanHandleToBMS.pTxMsg->Data[6] = 0x02;
         CanHandleToBMS.pTxMsg->Data[7] = 0x00;
         ubCANBufIndex_BMS = 0;
         HAL_CAN_Transmit(&CanHandleToBMS, 0xff);   //	充电机发送J1939多字节传输连接部分  数据接收结束，发送消息结束应答
         ubRecognitionResult = 0xAA;         
         CAN_mSend_CRM_ChargerIdentify( );  //充电机发送充电机辨识报文，辨识有效
         uwCANTimeOutCount = 0;  
         ubCANMultiFrameIndex = 0;
		     CAN_mCANBufReset_BMS();
         FlagIfTimeout = 0;
         ubCANEventStatus = 3 ;          
    }
		//判断是否超时5S
    if(FlagIfTimeout == 1)          
    {
        ubChargerError[0]|=0x01;     //接收BMS和车辆辨识报文超时
        CAN_mSend_CEM_ChargerError(); //充电机发送错误报文  接收BMS和车辆辨识报文超时
        ubCANEventStatus = 20;       //超时错误处理
        FlagIfTimeout = 0;           //5s超时标志位清零
    }
}     

 //CAN事件处理3,处理 动力蓄电池充电参数报文  报文代号BCP
void CAN_mEventProc3(void)
{
	 //判断是否是多字节数据帧连
    if(ubCANConnect[0] == 0x10)       
    {
         //多字节数据帧连接回复
		 CanHandleToBMS.pTxMsg->ExtId = 0x1CECF456;
		 CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
		 CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
		 CanHandleToBMS.pTxMsg->DLC = 8;
		 CanHandleToBMS.pTxMsg->Data[0] = 0x11;      
		 CanHandleToBMS.pTxMsg->Data[1] = ubCANConnect[3];
		 CanHandleToBMS.pTxMsg->Data[2] = 0x01;
		 CanHandleToBMS.pTxMsg->Data[3] = 0xFF;
		 CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
		 CanHandleToBMS.pTxMsg->Data[5] = ubCANConnect[5];
		 CanHandleToBMS.pTxMsg->Data[6] = ubCANConnect[6];
		 CanHandleToBMS.pTxMsg->Data[7] = ubCANConnect[7];
		 ubCANBufIndex_BMS = 0;
		 HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	充电机发送J1939多字节传输连接部分  //	充电机发送J1939多字节传输连接部分
		 ubCANConnect[0] = 0x00;
    }
		//判断多数据帧数据是否接收完毕 
    if(ubCANMultiFrame[1][0] == 0x02)        
    {         
		 uwCellMaxAllowChargeVoltage = (ubCANMultiFrame[0][2]<<8)+ ubCANMultiFrame[0][1];
		 uwMaxAllowChargeCurrent = (ubCANMultiFrame[0][4]<<8)+ ubCANMultiFrame[0][3];
		 uwBatteryTotalEnergy = (ubCANMultiFrame[0][6]<<8)+ ubCANMultiFrame[0][5];
		 uwMaxAllowChargeVoltage = (ubCANMultiFrame[1][1]<<8)+ ubCANMultiFrame[0][7];
		 ubMaxAllowTemperature = (ubCANMultiFrame[1][2]);
		 uwBmsSOC = (ubCANMultiFrame[1][4]<<8)+ ubCANMultiFrame[1][3];
		 uwBmsTotalVoltage = (ubCANMultiFrame[1][6]<<8)+ ubCANMultiFrame[1][5]; 
      
			CanHandleToBMS.pTxMsg->Data[0] = 0x13;
			CanHandleToBMS.pTxMsg->Data[1] = 0x0D;
			CanHandleToBMS.pTxMsg->Data[2] = 0x00;
			CanHandleToBMS.pTxMsg->Data[3] = 0x02;
			CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
			CanHandleToBMS.pTxMsg->Data[5] = 0x00;
			CanHandleToBMS.pTxMsg->Data[6] = 0x06;
			CanHandleToBMS.pTxMsg->Data[7] = 0x00;
			ubCANBufIndex_BMS = 0;
			HAL_CAN_Transmit(&CanHandleToBMS, 0xff);   //	充电机发送J1939多字节传输连接部分  数据接收结束，发送消息结束应答
			ubCANMultiFrameIndex =0;
			CAN_mCANBufReset_BMS( );//CAN接收数据缓冲器数据复位
			mDelayCAN10us(1);       
			CAN_mSend_CTS_ChargerTime();  // 充电机发送时间同步信息报文
			mDelayCAN10us(1); 
			CAN_mSend_CML_ChargerMaxOutputCapactity();  //充电机发送最大输出能力报文，数据直接在DAvE固定，此函数不需要装填数据，直接发送
			uwCANTimeOutCount = 0;                 //CAN通信车辆通信超时计数器清零
			FlagIfTimeout = 0;
			ubCANEventStatus = 6;
			ubFlagIfReceBatteryParameterMeg = 0;
    }
		//等待中  周期发送 充电机辨识报文有效指令 
    if(1 == FlagIfCycleSend250ms)     //等待中  周期发送 充电机辨识报文有效指令  
    {
         if(TRUE == CAN_mSend_CRM_ChargerIdentify( ))  //充电机发送充电机辨识报文 
         {
             FlagIfCycleSend250ms = 0;
         }
     }
	  //判断是否超时5S
    if(FlagIfTimeout == 1)             //判断是否超时5S
    {
        ubChargerError[1]|=0x01;       // 接收蓄电池充电参数报文超时
        if(TRUE == CAN_mSend_CEM_ChargerError())  // 充电机发送错误报文  接收蓄电池充电参数报文超时
        {
            ubCANEventStatus = 20;        //超时错误处理
            FlagIfTimeout = 0;         
        }
    }	
}    

//CAN事件处理4,处理 充电机发送时间同步信息报文  报文代号CTS
void CAN_mEventProc4(void)
{
	
}    

//CAN事件处理5,处理 充电机最大输出能力报文  报文代号CML
void CAN_mEventProc5(void)
{
	
}     

//CAN事件处理6,处理 电池充电准备就绪状态报文  报文代号BRO
void CAN_mEventProc6(void)
{
	if(1 == FlagIfCycleSend500ms)     //判断是否满足周期发送的条件
    {
         if(TRUE == CAN_mSend_CTS_ChargerTime( ))  // 充电机发送时间同步信息报文
         {
             mDelayCAN10us(1);
             if(TRUE == CAN_mSend_CML_ChargerMaxOutputCapactity( ))  //充电机发送最大输出能力报文，数据直接在DAvE固定，此函数不需要装填数据，直接发送 
             {
                 FlagIfCycleSend500ms = 0;
             }
         }
    }
   if(FlagIfReceBmsReadyMeg == 1)         //如果BMS准备就绪标志位置1则BMS已发送BRO报文
    {
         CAN_mCANBufReset_BMS();//CAN接收数据缓冲器数据复位
         ubCANBufIndex_BMS= 0; 
         if(0xAA == ubBmsChargeReady)     //电池充电准备就绪
         {  
             ubChargerOutputReady =0xAA;     
             CAN_mSend_CRO_ChargerOutputReady( );  //充电机发送充电机输出准备就绪状态报文
             uwCANTimeOutCount = 0;
             FlagIfTimeout = 0;
             //ubCANEventStatus = 7; 
             ubCANEventStatus = 10;					 
             FlagIfSendDCModuleMeg = 1;           //DC 电源报文发送有效
             ubDCModue = CAN_STARTDCMODUE;      //启动电源指令
             //CANChargeFlag = 1;
			       //ubDCModue = 0x55;      //启动电源指令
             FlagIfCycleSend1000ms = 1; //发送有效
             uwCANDCTimeOutCount =0;
         }
         FlagIfReceBmsReadyMeg =0; 
    }
   if(FlagIfTimeout == 1)        //判断是否超时5S
    {
        ubChargerError[1]|=0x04;  //接收BMS完成充电准备报文超时
        if(TRUE == CAN_mSend_CEM_ChargerError())  // 充电机发送错误报文  接收BMS完成充电准备报文超时
        {
            ubCANEventStatus = 20;     //超时错误处理
            FlagIfTimeout = 0;
        }
    }
}      

//CAN事件处理7,处理 充电机输出准备就绪状态报文  报文代号CRO
void CAN_mEventProc7(void)
{
	 if(1 == FlagIfCycleSend250ms)        //判断是否满足周期发送
    {
        if(TRUE == CAN_mSend_CRO_ChargerOutputReady( ))  //充电机发送充电机输出准备就绪状态报文
        {
			      ubCANEventStatus = 10;
            FlagIfCycleSend250ms = 0;
        }
    }
   if(1 == FlagIfTimeout)           //判断BMS发送电池充电总状态报文
    {
        ubChargerError[2]|=0x01;
        if(TRUE == CAN_mSend_CEM_ChargerError())  // 充电机发送错误报文  接收BMS和车辆辨识报文超时
        {
            ubCANEventStatus = 20;//超时错误处理
            FlagIfTimeout = 0;
        }
    }
}    

//CAN事件处理8,处理 电池充电需求报文  报文代号BCL
void CAN_mEventProc8(void)
{
	
}     

//CAN事件处理9,处理 电池充电总状态报文  报文代号BCS
void CAN_mEventProc9(void)
{
	
} 

//CAN事件处理10,处理 充电机充电状态报文 报文代号CCS
void CAN_mEventProc10(void)
{
	  //uint16_t i,j;
      //uint8_t Buf[592];
    //如果BMS接受CCS报文
    if(FlagIfCycleSend50ms == 1)     //50ms周期发送
    { 
        if(TRUE == CAN_mSend_CCS_ChargerChargeState())  //充电机发送充电状态报文
        {
            FlagIfCycleSend50ms = 0;
        }
    }
		mDelayCAN10us(2);
    //如果接收到BMS状态报文，做异常判断
    if(1 == FlagIfReceBmsBatteryStateMeg)
    {
        if(!(uwStatusAndAbnormal & 0x1000))  //判断动力蓄电池状态信息中是否允许充电 7.6,7.5 00禁止 01 允许
        {
            ubChargerStopReason = 0x00;
            CAN_mSend_CST_ChargerStop();  //充电机发送充电机中止充电报文
            ubCANEventStatus = 16;//充电机中止充电
            CANChargeFlag = 0;
            FlagIfReceBmsStopChargeMeg = 0;
					  //FlagIfReceBmsBatteryStateMeg = 1;
            mDelayCAN10us(4);
            uwVoltageRequirement = 0;
            uwCurrentRequirement = 0;
            ubDCModue = CAN_STOPDCMODUE ;  //关闭充电机
            CAN_mSendDCModuleStartOrStop(0x00);  //	充电机向直流充电模块发送启动停止命令
        }
        FlagIfReceBmsBatteryStateMeg = 0;
    }
		//判断是否收到BMS中止充电报文
    if(FlagIfReceBmsStopChargeMeg == 1)   //一些条件：收到BMS发送的中止充电报文或者充电机到达中止充电条件，程序进入充电机中止充电，而且在枪没有被拔下来之前一直发送
    {
					ubChargerStopReason = 0x00;
					CAN_mSend_CST_ChargerStop();  //充电机发送充电机中止充电报文
					ubCANEventStatus = 16;//充电机中止充电
					FlagIfReceBmsStopChargeMeg = 0;
					mDelayCAN10us(2);
					uwVoltageRequirement = 0;
					uwCurrentRequirement = 0;  
					ubDCModue = CAN_STOPDCMODUE ;
					//ubDCModue = 0xAA;
					CAN_mSendDCModuleStartOrStop(0x00) ;  //	充电机向直流充电模块发送启动停止命令
    }
    //接受BMS发过来的电池充电总状态(BCS)
    if(ubManyBytesStateBCS == 1)
    {
        if(ubCANConnect[0] == 0x10)
        {            
						 CanHandleToBMS.pTxMsg->ExtId = 0x1CECF456;
						 CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
						 CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
						 CanHandleToBMS.pTxMsg->DLC = 8;
             CanHandleToBMS.pTxMsg->Data[0] = 0x11;
             CanHandleToBMS.pTxMsg->Data[1] = ubCANConnect[3];
             CanHandleToBMS.pTxMsg->Data[2] = 0x01;
             CanHandleToBMS.pTxMsg->Data[3] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[5] = ubCANConnect[5];
             CanHandleToBMS.pTxMsg->Data[6] = ubCANConnect[6];
             CanHandleToBMS.pTxMsg->Data[7] = ubCANConnect[7];
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	充电机发送J1939多字节传输连接部分
						 CAN_mCANBufReset_BMS();//CAN接收数据缓冲器数据复位
             ubCANConnect[0] = 0x00;
        }
        if(ubCANMultiFrame[1][0] == 0x02)
        {       
						 uwChargeVoltageMeasurement = (ubCANMultiFrame[0][2]<<8)+ ubCANMultiFrame[0][1];
						 uwChargeCurrentMeasurement = (ubCANMultiFrame[0][4]<<8)+ ubCANMultiFrame[0][3];
						 uwHighestCellVoltageAndGroupNumber = (ubCANMultiFrame[0][6]<<8)+ ubCANMultiFrame[0][5];
						 uwBmsSOC = ubCANMultiFrame[0][7];  //BMS
						 uwRemainChargeTime = (ubCANMultiFrame[1][2]<<8)+ ubCANMultiFrame[1][1];           
						 CanHandleToBMS.pTxMsg->ExtId = 0x1CECF456;
						 CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
						 CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
						 CanHandleToBMS.pTxMsg->DLC = 8;
             CanHandleToBMS.pTxMsg->Data[0] = 0x13;
             CanHandleToBMS.pTxMsg->Data[1] = 0x09;
             CanHandleToBMS.pTxMsg->Data[2] = 0x00;
             CanHandleToBMS.pTxMsg->Data[3] = 0x02;
             CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[5] = 0x00;
             CanHandleToBMS.pTxMsg->Data[6] = 0x11;
             CanHandleToBMS.pTxMsg->Data[7] = 0x00;
             ubCANMultiFrameIndex = 0;
						 ubManyBytesStateBCS = 0;
						 CAN_mCANBufReset_BMS();
						 ubCANBufIndex_BMS = 0;					
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	充电机发送J1939多字节传输连接部分  数据接收结束，发送消息结束应答            
             uwCANTimeOutCount = 0;
             FlagIfTimeout = 0;
             ubCANEventStatus = 10;
        }
    }
		//接收到BMS发过来的单体动力蓄电池电压(BMV)报文
    if(ubManyBytesStateBMV == 1)
    {
        if(ubCANConnect[0] == 0x10)
        {
             ubCellVoltageQuantity = (uint8_t)((ubCANConnect[1]+(ubCANConnect[2]<<8))/2);     //单体动力蓄电池电池电压数量 
             ubCellVoltageMegQuantity = ubCANConnect[3];     //单体动力蓄电池电池电压 报文帧数量 
             CanHandleToBMS.pTxMsg->ExtId = 0x1CECF456;
						 CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
						 CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
						 CanHandleToBMS.pTxMsg->DLC = 8;
             CanHandleToBMS.pTxMsg->Data[0] = 0x11;
             CanHandleToBMS.pTxMsg->Data[1] = ubCANConnect[3];
             CanHandleToBMS.pTxMsg->Data[2] = 0x01;
             CanHandleToBMS.pTxMsg->Data[3] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[5] = ubCANConnect[5];
             CanHandleToBMS.pTxMsg->Data[6] = ubCANConnect[6];
             CanHandleToBMS.pTxMsg->Data[7] = ubCANConnect[7];
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff); //	充电机发送J1939多字节传输连接部分
						 ubCANBufIndex_BMS = 0;
             ubCANConnect[0] = 0x00;
						 ubCANMultiFrameIndex = 0;
						 CAN_mCANBufReset_BMS();//CAN接收数据缓冲器数据复位
        }
        if(ubCANMultiFrame[ubCellVoltageMegQuantity -1][0] == ubCellVoltageMegQuantity )
		//if(ubCANMultiFrame[ubCellVoltageMegQuantity-1][0] == ubCellVoltageMegQuantity)
        {
//              for(i=0;i<(ubCellVoltageMegQuantity*8);i++)
//              {
//                  for(j=0;j<8;j++)
//                  {
//                      Buf[i] = ubCANMultiFrame[i/8][j];
//                  }
//              }
//              for(i=0,j=0;i<ubCellVoltageQuantity,j<(ubCellVoltageMegQuantity*8);i++,j=j+2)
//              {
//                  if(j%8 == 0)
//                  {
//                      j=j+1;
//                  }
//                  uwCellVoltage[i] = Buf[j];
//                  if((j+1)%8 == 0)
//                  {
//                      j=j+1;
//                  }
//                  uwCellVoltage[i] = uwCellVoltage[i]+ Buf[j+1]<<8;
//              }
             CanHandleToBMS.pTxMsg->Data[0] = 0x13;
						 CanHandleToBMS.pTxMsg->ExtId = 0x1CECF456;
						 CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
						 CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
						 CanHandleToBMS.pTxMsg->DLC = 8;
             CanHandleToBMS.pTxMsg->Data[1] = 0xFC;
             CanHandleToBMS.pTxMsg->Data[2] = 0x00;
             CanHandleToBMS.pTxMsg->Data[3] = ubCANMultiFrameIndex;//包的数量
             CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[5] = 0x00;
             CanHandleToBMS.pTxMsg->Data[6] = 0x15;
             CanHandleToBMS.pTxMsg->Data[7] = 0x00;
						 ubCANMultiFrameIndex = 0;
						 CAN_mCANBufReset_BMS();
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	充电机发送J1939多字节传输连接部分  数据接收结束，发送消息结束应答        
             uwCANTimeOutCount = 0;
             FlagIfTimeout = 0;
             ubCANEventStatus = 10;
             ubManyBytesStateBMV = 0;
        }
        ubCANEventStatus = 10;
    }
    //判断BMS发来的动力蓄电池温度(BMT)报文
    if(ubManyBytesStateBMT == 1)
    {
        if(ubCANConnect[0] == 0x10)
        {
             ubCellTemperatureQuantity = (uint8_t)((ubCANConnect[1]+(ubCANConnect[2]<<8))/2); ; //动力蓄电池温度 数量
             ubCellTemperatureMegQuantity = ubCANConnect[3];  //动力蓄电池温度 报文帧数量
             CanHandleToBMS.pTxMsg->ExtId = 0x1CECF456;
						 CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
						 CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
						 CanHandleToBMS.pTxMsg->DLC = 8;
						 CanHandleToBMS.pTxMsg->Data[0] = 0x11;
             CanHandleToBMS.pTxMsg->Data[1] = ubCANConnect[3];
             CanHandleToBMS.pTxMsg->Data[2] = 0x01;
             CanHandleToBMS.pTxMsg->Data[3] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[5] = ubCANConnect[5];
             CanHandleToBMS.pTxMsg->Data[6] = ubCANConnect[6];
             CanHandleToBMS.pTxMsg->Data[7] = ubCANConnect[7];
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);   //	充电机发送J1939多字节传输连接部分
             ubCANConnect[0] = 0x00;
						 ubCANMultiFrameIndex = 0;
						 CAN_mCANBufReset_BMS();//CAN接收数据缓冲器数据复位
        }
		//if(ubCANMultiFrame[ubCellTemperatureMegQuantity-1][0] == ubCellTemperatureMegQuantity)
        if(ubCANMultiFrame[1][0] == 2)
        {
//              for(i=0;i<(ubCellTemperatureMegQuantity*8);i++)
//              {
//                  for(j=0;j<8;j++)
//                  {
//                      Buf[i] = ubCANMultiFrame[i/8][j];
//                  }
//              }
//              for(i=0,j=0;i<ubCellTemperatureQuantity,j<(ubCellTemperatureMegQuantity*8);i++,j++)
//              {
//                  if(j%8 == 0)
//                  {
//                      j=j+1;
//                  }
//                  ubCellTemperature[i] = Buf[j];
//              }
						 CanHandleToBMS.pTxMsg->ExtId = 0x1CECF456;
						 CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
						 CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
						 CanHandleToBMS.pTxMsg->DLC = 8;
             CanHandleToBMS.pTxMsg->Data[0] = 0x13;
             CanHandleToBMS.pTxMsg->Data[1] = ubCellTemperatureQuantity;
            // CanHandleToBMS.pTxMsg->Data[1] = 0x0C;
             CanHandleToBMS.pTxMsg->Data[2] = 0x00;
             CanHandleToBMS.pTxMsg->Data[3] = ubCellTemperatureMegQuantity;//
             CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[5] = 0x00;
             CanHandleToBMS.pTxMsg->Data[6] = 0x16;
             CanHandleToBMS.pTxMsg->Data[7] = 0x00;
             ubCANMultiFrameIndex = 0 ;
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	充电机发送J1939多字节传输连接部分  数据接收结束，发送消息结束应答
             CAN_mCANBufReset_BMS();   
						 ubManyBytesStateBMT = 0;			 
             uwCANTimeOutCount = 0;
             FlagIfTimeout = 0;
             ubCANEventStatus = 10;
        }
    }
    //判断来自BMS的动力蓄电池预留报文(BSP)
    if(ubManyBytesStateBSP == 1)
    {        
         if(ubCANConnect[0] ==0x10)
         {
             ubCellReservedQuantity =(uint8_t)((ubCANConnect[1]+(ubCANConnect[2]<<8))/2); ;    //动力蓄电池预留报文  数量
             ubCellReservedMegQuantity = ubCANConnect[3];    //动力蓄电池预留报文  报文帧数量
						 CanHandleToBMS.pTxMsg->ExtId = 0x1CECF456;
						 CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
						 CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
						 CanHandleToBMS.pTxMsg->DLC = 8;				
             CanHandleToBMS.pTxMsg->Data[0] = 0x11;
             CanHandleToBMS.pTxMsg->Data[1] = ubCANConnect[3];
             CanHandleToBMS.pTxMsg->Data[2] = 0x01;
             CanHandleToBMS.pTxMsg->Data[3] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[5] = ubCANConnect[5];
             CanHandleToBMS.pTxMsg->Data[6] = ubCANConnect[6];
             CanHandleToBMS.pTxMsg->Data[7] = ubCANConnect[7];
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	充电机发送J1939多字节传输连接部分
             ubCANConnect[0] = 0x00;
        }
        //if(ubCANMultiFrame[18][0] == 0x19)
				if(ubCANMultiFrame[ubCellReservedMegQuantity-1][0] == ubCellReservedMegQuantity )
        {
//              for(i=0;i<24;i++)
//              {
//                  for(j=0;j<8;j++)
//                  {
//                      Buf[i] = ubCANMultiFrame[i/8][j];
//                  }
//              }
//              for(i=0,j=0;i<16,j<24;i++,j++)
//              {
//                  if(j%8 == 0)
//                  {
//                      j=j+1;
//                  }
//                  ubCellReserved[i] = Buf[j];
//              }
						 CanHandleToBMS.pTxMsg->ExtId = 0x1CECF456;
						 CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
						 CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
						 CanHandleToBMS.pTxMsg->DLC = 8;
             CanHandleToBMS.pTxMsg->Data[0] = 0x13;
             CanHandleToBMS.pTxMsg->Data[1] = ubCellReservedQuantity;
             CanHandleToBMS.pTxMsg->Data[2] = 0x00;
             CanHandleToBMS.pTxMsg->Data[3] = ubCellReservedMegQuantity;//3个包
             CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[5] = 0x00;
             CanHandleToBMS.pTxMsg->Data[6] = 0x17;
             CanHandleToBMS.pTxMsg->Data[7] = 0x00;
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	充电机发送J1939多字节传输连接部分  数据接收结束，发送消息结束应答
             ubCANMultiFrameIndex = 0;
						 CAN_mCANBufReset_BMS();//CAN接收数据缓冲器数据复位
             uwCANTimeOutCount = 0;
             FlagIfTimeout = 0;
             ubCANEventStatus = 10;
             ubManyBytesStateBSP = 0;
        } 
    }
}      

 //CAN事件处理11,处理 动力蓄电池状态信息报文 报文代号BSM
void CAN_mEventProc11(void)
{
	
} 

//CAN事件处理12,处理 单体动力蓄电池电压报文 报文代号BMV
void CAN_mEventProc12(void)
{
	
}    

//CAN事件处理13,处理 动力蓄电池温度报文 报文代号BMT
void CAN_mEventProc13(void)
{
	
}      

//CAN事件处理14,处理 动力蓄电池预留报文 报文代号BSP
void CAN_mEventProc14(void)
{
	
}   

 //CAN事件处理15,处理 BMS中止充电报文 报文代号BST
void CAN_mEventProc15(void)
{

}     

//CAN事件处理16,处理 充电机中止充电报文 报文代号CST
void CAN_mEventProc16(void)
{
	if(FlagIfCycleSend10ms == 1)
    {
         if(TRUE == CAN_mSend_CST_ChargerStop())  //充电机发送充电机中止充电报文
         {
             FlagIfCycleSend10ms = 0;
         }
    }
    if(FlagIfReceBmsStopChargeMeg == 1)
    {
         ubCANEventStatus = 17;
         ubTimeN100ms = 50;  //设置5S超时   50*100ms=5000ms
         uwCANTimeOutCount = 0;
         FlagIfReceBmsStopChargeMeg = 0;
    }
}      

 //CAN事件处理17,处理 BMS统计报文 报文代号BSD
void CAN_mEventProc17(void)
{
	 if(FlagIfReceBmsStatisticsDataMeg == 1)
     {
         if(TRUE == CAN_mSend_CSD_ChargerStatisticsData( ))  //  充电机发送充电机统计数据 
         {
             FlagIfReceBmsStatisticsDataMeg = 0;
             ubCANEventStatus = 18;
         }
     }
}     

//CAN事件处理18,处理 充电机统计报文 报文代号CSD
void CAN_mEventProc18(void)
{
	if(FlagIfReceBmsStatisticsDataMeg == 1)
    {      
         if(TRUE == CAN_mSend_CSD_ChargerStatisticsData( ))  //  充电机发送充电机统计数据 
         {
             FlagIfReceBmsStatisticsDataMeg = 0;
             ubCANEventStatus = 24;  //无效处理程序
         }
         FlagIfSendDCModuleMeg = 0;
				 ResetACRwlay();      //停止交流继电器
				 ResetDCRwlay();      //停止直流接触器
				 ResetBMSRwlay();//停止辅助电源
    } 
// 	ResetACRwlay();      //停止交流继电器
// 	ResetDCRwlay();      //停止直流接触器
// 	ResetBMSRwlay();//停止辅助电源
}      

//CAN事件处理19,处理 BMS错误报文 报文代号BEM
void CAN_mEventProc19(void)
{
	
}    

//CAN事件处理20,处理 充电机错误报文 报文代号CEM

void CAN_mEventProc20(void)
{
	
}  

//CAN事件处理21,处理 主板给电源模块发送需求报文 报文代号CQM
void CAN_mEventProc21(void)
{
	
}      

//CAN事件处理22,处理 电源模块给主板发送状态信息 报文代号MSM
void CAN_mEventProc22(void)
{
	
}      

uint16_t CAN_mAccumulateChargeTime()
{
	StartChargeTime.Minute = BCDToChar(StartChargeBCDTime.Minute);
	StartChargeTime.Hour = BCDToChar(StartChargeBCDTime.Hour);
	RTCReadTime.Minute = BCDToChar(RTCReadBCDTime.Minute);
	RTCReadTime.Hour = BCDToChar(RTCReadBCDTime.Hour);
	if(StartChargeTime.Minute > RTCReadTime.Minute)
	{
		ChargeTime.Minute = RTCReadTime.Minute + 60 - StartChargeTime.Minute;
		RTCReadTime.Hour =RTCReadTime.Hour - 1;
	}
	else
	{
		ChargeTime.Minute = RTCReadTime.Minute - StartChargeTime.Minute;
	}
  if(StartChargeTime.Hour > RTCReadTime.Hour)
	{
		ChargeTime.Hour = RTCReadTime.Hour + 24 - StartChargeTime.Hour;
		//RTCReadTime.Hour =RTCReadTime.Hour - 1;
	}
	else
	{
		ChargeTime.Hour = RTCReadTime.Hour - StartChargeTime.Hour;
	}
	return 1;
}
uint8_t BCDToChar(uint8_t bcd)			 //二进制BCD码转换成整形
{
	   uint8_t ubToChar;
     uint8_t m,n;
     uint8_t ubBcd = bcd;
	//ubToChar = (ubyte)(((ubBcd>>4)&0x0F)*0x0A + ubBcd&0x0F);
    //test1 = (ubyte)((ubBcd&0xF0)/16 + ubBcd&0x0F);
    m =(uint8_t)(((ubBcd>>4)&0x0F)*0x0A);//+ubBcd&0x0F;
    n =ubBcd&0x0F;
    ubToChar = m+n;
	return ubToChar;
}

//CAN中断数据处理
void CAN_DataProcessing(void)
{
	//判断DC的数据
	switch(uCANBufID_DC)
	{
				//编号1充电机
				case 0x1207C081:   //监控设定输出时模块回复输出		
				{
						switch(ubCANBuf_DC[0])
							{
								case 0x00:   //监控设定输出时模块回复输出									
													break;
								case 0x01:   //监控读取信息时模块回复状态
														uwDCOutputCurrent[1] = (ubCANBuf_DC[3])+(ubCANBuf_DC[2]<<8);
														uwDCOutputVoltage[1] = (ubCANBuf_DC[5])+(ubCANBuf_DC[4]<<8);
														ubDCModuleStatusHigh[1] = ubCANBuf_DC[6];
														ubDCModuleStatusLow[1] = ubCANBuf_DC[7];
													break;
								case 0x02:   //监控设定开关机时模块回复开关机
									
													break;
													
								default:
													break;
							}
				}					
							break;
				//编号2充电机
				case 0x1207C082:   //监控读取信息时模块回复状态       
				{
					switch(ubCANBuf_DC[0])
							{
								case 0x00:   //监控设定输出时模块回复输出									
													break;
								case 0x01:   //监控读取信息时模块回复状态
														uwDCOutputCurrent[2] = (ubCANBuf_DC[3])+(ubCANBuf_DC[2]<<8);
														uwDCOutputVoltage[2] = (ubCANBuf_DC[5])+(ubCANBuf_DC[4]<<8);
														ubDCModuleStatusHigh[2] = ubCANBuf_DC[6];
														ubDCModuleStatusLow[2] = ubCANBuf_DC[7];
													break;
								case 0x02:   //监控设定开关机时模块回复开关机
									
													break;
													
								default:
													break;
							}
				 } 		
									break;
				//编号3充电机
				case 0x1207C083:   //监控设定开关机时模块回复开关机	
				{
					switch(ubCANBuf_DC[0])
							{
								case 0x00:   //监控设定输出时模块回复输出									
													break;
								case 0x01:   //监控读取信息时模块回复状态
														uwDCOutputCurrent[3] = (ubCANBuf_DC[3])+(ubCANBuf_DC[2]<<8);
														uwDCOutputVoltage[3] = (ubCANBuf_DC[5])+(ubCANBuf_DC[4]<<8);
														ubDCModuleStatusHigh[3] = ubCANBuf_DC[6];
														ubDCModuleStatusLow[3] = ubCANBuf_DC[7];
													break;
								case 0x02:   //监控设定开关机时模块回复开关机
									
													break;
													
								default:
													break;
							}
				}		
									break;							
				default:
									break;
	 }
	 uwOutputVoltageValue = uwDCOutputVoltage[1];
	 uwOutputCurrentValue = uwDCOutputCurrent[1] + uwDCOutputCurrent[2] + uwDCOutputCurrent[3];
	 //uCANBufID_DC = 0;
   uCANBufID_DC = 0x00000000;
	 CAN_mCANBufReset_DC();
}



void ChargeOfManualOperation(void)     //手动充电
{
		if(FlagIfSendDCMeg ==1)
		{
					FlagIfSendDCMeg = 0;				
					switch(CANCount)
						{
							case 0:  //	向直流充电模块发送设定输出控制报文,广播发送
									CAN_mSendDCModuleSetOutputParameter(0x00);
									break;
							case 1: 							
									CAN_mSendDCModuleReadStute(ubDCModueAddress);//分别读取多个模块
									ubDCModueAddress++;//读取模块地址加1，切换模块
									if(ubDCModueAddress == (DCMODUENUMBER+1))
									{
										ubDCModueAddress = 1;
									}
									break;
							case 2: //	充电机向直流充电模块发送启动停止充电机命令，广播发送
									CAN_mSendDCModuleStartOrStop(0x00); 
									break;
							default:  
									break;
						}	
					CANCount++;				
					if(CANCount == 3)
						CANCount = 0;				
		}			
	  CAN_DataProcessing();
}






