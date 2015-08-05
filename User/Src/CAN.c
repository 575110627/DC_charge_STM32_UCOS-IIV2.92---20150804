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

//���
void CANqueue_push(struct _StrCAN StrCAN)
{
	 uint8_t i,pos=0;
	 pos = (CANQueue.Head+1)%CANQUEUEMAXSIZE;
	 if(pos != CANQueue.Head) //�ǿ�״̬
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
//����
uint8_t CANQueue_pop(struct _StrCAN* StrCAN)
{ 
	uint8_t i;
	if(CANQueue.Head != CANQueue.Tail)//�ӷǿ�
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
---------------------��������---------------------------
--------------------------------------------------------*/
void CAN_Config(void);
void CANtoBMS_Config(void);
void CANtoDC_Config(void);
void CAN_mInitData(void);//��ʼ��CAnͨ������������ݣ�������ģ���ṩ��������������ģ���ʼ��
static uint8_t CAN_mSend_CRM_ChargerIdentify(void);  //�������ͳ�����ʶ����
static uint8_t CAN_mSend_CTS_ChargerTime(void);  // ��������ʱ��ͬ����Ϣ����
static uint8_t CAN_mSend_CML_ChargerMaxOutputCapactity(void);  //����������������������
static uint8_t CAN_mSend_CRO_ChargerOutputReady(void);  //�������ͳ������׼������״̬����
uint8_t CAN_mSend_CCS_ChargerChargeState(void);  //�������ͳ��״̬����
uint8_t CAN_mSend_CST_ChargerStop(void);  //�������ͳ�����ֹ��籨��
static uint8_t CAN_mSend_CSD_ChargerStatisticsData(void);  //  �������ͳ���ͳ������
static uint8_t CAN_mSend_CEM_ChargerError(void);  // �������ʹ�����
static uint8_t CAN_mSendDCModuleSetOutputParameter(uint8_t modueaddress);  //	������ֱ�����ģ�鷢�Ϳ��Ʊ���
uint8_t CAN_mSendDCModuleStartOrStop(uint8_t modueaddress);   //	������ֱ�����ģ�鷢������ֹͣ����
static uint8_t CAN_mSendDCModuleReadStute(uint8_t modueaddress);  //	������ֱ�����ģ�鷢�Ͳ�����������

static void CAN_mCANBufReset_BMS(void);//CAN����BMS���ݻ��������ݸ�λ
static void CAN_mCANBufReset_DC(void);//CAN����DC���ݻ��������ݸ�λ
void CAN_mEvent(void);       //CAN�¼�����
void CAN_DataProcessing(void);    //CAN�ж����ݴ���
static void CAN_mEventProc1(void);       //CAN�¼�����1,���� ������ʶ����  ���Ĵ���CRM
static void CAN_mEventProc2(void);       //CAN�¼�����2,���� BMS�ͳ�����ʶ����  ���Ĵ���BRM
static void CAN_mEventProc3(void);       //CAN�¼�����3,���� �������س���������  ���Ĵ���BCP
static void CAN_mEventProc4(void);       //CAN�¼�����4,���� ��������ʱ��ͬ����Ϣ����  ���Ĵ���CTS
static void CAN_mEventProc5(void);       //CAN�¼�����5,���� ������������������  ���Ĵ���CML
static void CAN_mEventProc6(void);       //CAN�¼�����6,���� ��س��׼������״̬����  ���Ĵ���BRO
static void CAN_mEventProc7(void);       //CAN�¼�����7,���� �������׼������״̬����  ���Ĵ���CRO
static void CAN_mEventProc8(void);       //CAN�¼�����8,���� ��س��������  ���Ĵ���BCL
static void CAN_mEventProc9(void);       //CAN�¼�����9,���� ��س����״̬����  ���Ĵ���BCS
static void CAN_mEventProc10(void);       //CAN�¼�����10,���� �������״̬���� ���Ĵ���CCS
static void CAN_mEventProc11(void);       //CAN�¼�����11,���� ��������״̬��Ϣ���� ���Ĵ���BSM
static void CAN_mEventProc12(void);       //CAN�¼�����12,���� ���嶯�����ص�ѹ���� ���Ĵ���BMV
static void CAN_mEventProc13(void);       //CAN�¼�����13,���� ���������¶ȱ��� ���Ĵ���BMT
static void CAN_mEventProc14(void);       //CAN�¼�����14,���� ��������Ԥ������ ���Ĵ���BSP
static void CAN_mEventProc15(void);       //CAN�¼�����15,���� BMS��ֹ��籨�� ���Ĵ���BST
static void CAN_mEventProc16(void);       //CAN�¼�����16,���� ������ֹ��籨�� ���Ĵ���CST
static void CAN_mEventProc17(void);       //CAN�¼�����17,���� BMSͳ�Ʊ��� ���Ĵ���BSD
static void CAN_mEventProc18(void);       //CAN�¼�����18,���� ����ͳ�Ʊ��� ���Ĵ���CSD
static void CAN_mEventProc19(void);       //CAN�¼�����19,���� BMS������ ���Ĵ���BEM
static void CAN_mEventProc20(void);       //CAN�¼�����20,���� ���������� ���Ĵ���CEM
static void CAN_mEventProc21(void);       //CAN�¼�����21,���� �������Դģ�鷢�������� ���Ĵ���CQM
static void CAN_mEventProc22(void);       //CAN�¼�����22,���� ��Դģ������巢��״̬��Ϣ ���Ĵ���MSM

uint16_t CAN_mAccumulateChargeTime(void);     //������ʱ��������ʱ�䳤�ȣ�����Ϊ��λ
void ChargeOfManualOperation(void);     //�ֶ����

/*------------------------------------------------------
---------------------��������---------------------------
--------------------------------------------------------*/
extern CAN_HandleTypeDef    CanHandleToBMS;
extern CAN_HandleTypeDef    CanHandleToDC;
uint8_t CANCount=0;
/*-----------------------------------------------------------------------
---------------------��������--------------------------------------------
-------------------------------------------------------------------------*/
/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */

//CAN��ʼ��
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
	//CAN ��ʼ�� 30MHz 250kbps
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
 
	//CAN������
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

	//CAN��������
  /*##-3- Configure Transmission process #####################################*/
  CanHandleToBMS.pTxMsg->StdId = 0x0321;
  CanHandleToBMS.pTxMsg->ExtId = 0x181C56F4;
  CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToBMS.pTxMsg->DLC = 8;
	HAL_CAN_Receive_IT(&CanHandleToBMS, CAN_FIFO0);
}
void CANtoDC_Config(void)              //����������  1207c081 1207c082 1207c083
{
	CAN_FilterConfTypeDef  sFilterConfig;
  static CanTxMsgTypeDef        TxMessage;
  static CanRxMsgTypeDef        RxMessage;
  /*##-1- Configure the CAN peripheral #######################################*/
  CanHandleToDC.Instance = CAN2;
  CanHandleToDC.pTxMsg = &TxMessage;
  CanHandleToDC.pRxMsg = &RxMessage;
	//CAN ��ʼ�� 30MHz 125kbps
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
  
	//CAN������          //����������  1207c081 1207c082 1207c083
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
 
	//CAN��������
  /*##-3- Configure Transmission process #####################################*/
  CanHandleToDC.pTxMsg->StdId = 0x0320;
  CanHandleToDC.pTxMsg->ExtId = 0x1307C081;
  CanHandleToDC.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToDC.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToDC.pTxMsg->DLC = 8;
  /*##-1- Configure the CAN peripheral #######################################*/

	HAL_CAN_Receive_IT(&CanHandleToDC, CAN_FIFO1);
}
//��ʼ��CAnͨ������������ݣ�������ģ���ṩ��������������ģ���ʼ��
void CAN_mInitData(void)
{
		 uint8_t i;
		 CANQueue.Head = 0;
     CANQueue.Tail = 0;
		 uwCurrentRequirementLowbuf = 0;
		 uwCurrentRequirementHighbuf = 0;
	   FlagIfReceBmsStopChargeMeg = 0;
     ubCANTim3Count = 0;
     ubFlagIfReceBmsIdentifyMeg = 0;   //��־���Ƿ��յ�BMS���͵ĵ�س��������� 0x00δ�յ���0xAA�յ�
     FlagIfReceBmsReadyMeg = 0;   //��־���Ƿ��յ�BMS���͵ĵ�س��׼������״̬���� 0δ�յ���1�յ�
     FlagIfReceBmsChargeRequirementMeg = 0;  //��־���Ƿ��յ�BMS���͵ĵ�س�������� 0x00δ�յ���0xAA�յ�
     FlagIfSendDCModuleMeg = 0;  //��־���Ƿ��յ�DCֱ������ģ�鷢�͵ı��� 0û�յ���1�յ�
     FlagIfSendDCMeg = 0;
		 FlagIfDCTimeout = 0;    //��־��DC��Դģ���Ƿ�ʱ5s 0x00�����㣬0xAA����
     FlagIfReceBmsBatteryStateMeg = 0;  //��־���Ƿ��յ�BMS���͵Ķ�������״̬ 0x00δ�յ���0xAA�յ�
     FlagIfReceBmsStopChargeMeg = 0;//��־���Ƿ��յ�BMS���͵�BMS��ֹ��籨�� 0x00δ�յ���0xAA�յ�
     FlagIfReceBmsStatisticsDataMeg = 0;  //��־���Ƿ��յ�BMS���͵ĳ��ͳ�����ݱ��� 0x00δ�յ���0xAA�յ�
     uwCANReceFlagState = 0x0000;//�Ƿ��յ�CAN����֡��־λ 0��ʾ��Ч 1��ʾ��Ч  
                                 //��0λ BMS�ͳ�����ʶ����
                                 //��1λ �������س�����
                                 //��2λ ��س��׼����������
                                 //��3λ ��س������
                                 //��4λ ��س����״̬
                                 //��5λ ��������״̬��Ϣ
                                 //��6λ ���嶯�����ص�ѹ
                                 //��7λ ���������¶�
                                 //��8λ ��������Ԥ������
                                 //��9λ BMS��ֹ���
                                 //��10λ BMSͳ�Ʊ���
                                 //��11λ BMS������
     //CAN���ݽ��ջ�������ʼ��
    // ubCANBufIndex_BMS = 0;
		 ubCANBufIndex_DC = 0;
     //CAN�������ڷ��ͱ�־,�ж��Ƿ�ʱ
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
     //CANģ������������ݵĳ�ʼ��
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
		 ubDCModueAddress = 0x01;//Ĭ��CANģ����0x01
		 uwDCOutputCurrent[1] = 0;
		 uwDCOutputCurrent[2] = 0;
		 uwDCOutputCurrent[3] = 0;
		 uwDCOutputVoltage[1] = 0;
		 
}

//�������ͳ�����ʶ����
uint8_t CAN_mSend_CRM_ChargerIdentify(void)
{
  CanHandleToBMS.pTxMsg->ExtId = 0x1801F456;
  CanHandleToBMS.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToBMS.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToBMS.pTxMsg->DLC = 8;
	ubChargerNumber = 0x10;         //�������
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

// ��������ʱ��ͬ����Ϣ����
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

//����������������������
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

//�������ͳ������׼������״̬����
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

//�������ͳ��״̬����
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

//�������ͳ�����ֹ��籨��
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

// �������ͳ���ͳ������
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
	
// �������ʹ�����	
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

 //	��ֱ�����ģ�鷢���趨������Ʊ���
uint8_t CAN_mSendDCModuleSetOutputParameter(uint8_t modueaddress)
{
	if(0 == ChargingMethod)
	{
				uwCurrentRequirementbuf = uwCurrentRequirementLowbuf + (uwCurrentRequirementHighbuf<<8);
				uwVoltageRequirement = uwVoltageRequirementLow + (uwVoltageRequirementHigh<<8);
				uwCurrentRequirement = 4000- uwCurrentRequirementbuf;
				if(uwCurrentRequirement >= (90*DCMODUENUMBER))  //ģ������
				{
					uwCurrentRequirement = (90*DCMODUENUMBER);
				}
				if(uwCurrentRequirement >= uwMaxAllowChargeCurrent)  //ģ������
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
		else if(1 == ChargingMethod)   //�ֶ����
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

//	������ֱ�����ģ�鷢������ֹͣ��������
uint8_t CAN_mSendDCModuleStartOrStop(uint8_t modueaddress) 
{
  CanHandleToDC.pTxMsg->ExtId = 0x1307C080 +modueaddress;
  CanHandleToDC.pTxMsg->RTR = CAN_RTR_DATA;
  CanHandleToDC.pTxMsg->IDE = CAN_ID_EXT;
  CanHandleToDC.pTxMsg->DLC = 8;
	
  CanHandleToDC.pTxMsg->Data[0] = 0x02;//Э���е�CMD=2
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

//	������ֱ�����ģ�鷢�Ͷ�ȡ��Ϣ����
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

//CAN����BMS���ݻ��������ݸ�λ
void CAN_mCANBufReset_BMS(void)
{
	uint8_t i;
	for(i=0;i<70;i++)
	{
		ubCANMultiFrame[i][0]=0;
	}
}

//CAN����DC���ݻ��������ݸ�λ
void CAN_mCANBufReset_DC(void)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		ubCANBuf_DC[i] = 0;
	}
}
	
//CAN�¼�����
void CAN_mEvent(void)
{

   uint8_t i=0;
   uint8_t j=0;
   FlagIfTimeout = 0;
   if(FlagIfSendDCModuleMeg == 1)               //�Ƿ���DC��Դģ�飬�ڳ������׼������״̬����֮��������� ���־λ��Ч
   { 
		if(FlagIfSendDCMeg ==1)
		{
					FlagIfSendDCMeg = 0;				
					switch(CANCount)
						{
							case 0:  //	��ֱ�����ģ�鷢���趨������Ʊ���,�㲥����
									CAN_mSendDCModuleSetOutputParameter(0x00);
									break;
							case 1: 							
									CAN_mSendDCModuleReadStute(ubDCModueAddress);//�ֱ��ȡ���ģ��
									ubDCModueAddress++;//��ȡģ���ַ��1���л�ģ��
									if(ubDCModueAddress == (DCMODUENUMBER+1))
									{
										ubDCModueAddress = 1;
									}
									break;
							case 2: //	������ֱ�����ģ�鷢������ֹͣ��������㲥����
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
					//��֡����,���Ӵ���
					case 0x1CEC56F4:
					{
						  ubCANMultiFrameIndex = 0;
						  CAN_mCANBufReset_BMS();
						  ubManyBytesStateBCS = 0;//��֡���ݵı�־
							ubManyBytesStateBMV = 0;//��֡���ݵı�־
							ubManyBytesStateBMT = 0;//��֡���ݵı�־
							ubManyBytesStateBSP = 0;//��֡���ݵı�־
							for(i=0;i<CANBMS1.DLC;i++)
							{
								ubCANConnect[i] = CANBMS1.Data[i];
								
							}
							switch(ubCANConnect[6])
							{
								//������ʶ����BRM  
								case 0x02:    //���յ�BMS�ͳ����ı�ʶ����
										 ubFlagIfReceBmsIdentifyMeg = 1;
									break;
								//�������س�����BCP
								case 0x06:   //���յ�����������
										 ubCANEventStatus = 3;
										 ubFlagIfReceBatteryParameterMeg = 1;
									break;
								//��س����״̬ BCS
								case 0x11:   
										 ubManyBytesStateBCS = 1;
														 //���յ������״̬
										 //FlagIfSendDCModuleMeg = 1;
							       ubCANEventStatus = 10;
										 ubFlagIfReceBmsAllChargeStateMeg = 1;  
									break;
								//���嶯�����ص�ѹBMV
								case 0x15:   
									    // FlagIfSendDCModuleMeg = 1;
										 ubCANEventStatus = 10;
										 ubManyBytesStateBMV = 1;
									break;
								//�������¶�BMT
								case 0x16:   
										 //FlagIfSendDCModuleMeg = 1;
										 ubCANEventStatus = 10;	
										 ubManyBytesStateBMT = 1;
									break;
								//��������Ԥ������
								case 0x17:   
										// FlagIfSendDCModuleMeg = 1;
										 ubCANEventStatus = 10;	
										 ubManyBytesStateBSP = 1;
									break;
								//����
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
					//BMS�ͳ�����ʶ����BRM
				 case 0x180256F4:   
						break;
					//�������س�����BCP
				 case 0x180656F4:
						break;
					//����׼���������� BRO
				 case 0x100956F4:       
							//ubCANBuf_BMS[0][0] = SW_MO5.ubData[0]; 
							ubBmsChargeReady = CANBMS1.Data[0]; 
							FlagIfReceBmsReadyMeg = 1;   //�յ�BMS׼����������
							break;		
					//��س������ BCL
				 case 0x181056F4: 
							//��ѹ��
							uwVoltageRequirementLow	= (int8_t)(CANBMS1.Data[0]);
							uwVoltageRequirementHigh = (int8_t)(CANBMS1.Data[1]); 
							//��������	
							uwCurrentRequirementLowbuf =(int8_t) (CANBMS1.Data[2]);
							uwCurrentRequirementHighbuf =(int8_t) (CANBMS1.Data[3]);
							ubChargeMode = CANBMS1.Data[4];
							FlagIfReceBmsChargeRequirementMeg = 1;   //���ܵ�BMS���������
							FlagIfSendDCModuleMeg = 1;
							ubCANEventStatus = 10;
							break;
					//BMS��������״̬��Ϣ BSM
					case 0x181356F4:
							ubMaxCellVoltageNumber = CANBMS1.Data[0];
							ubMaxTemperature = CANBMS1.Data[1];
							ubMaxTemperatureNumber = CANBMS1.Data[2];
							ubMinTemperature = CANBMS1.Data[3];
							ubMinTemperatureNumber = CANBMS1.Data[4];
							uwStatusAndAbnormal = (CANBMS1.Data[6]<<8) + CANBMS1.Data[5];
							ubCANEventStatus = 10;
							FlagIfReceBmsBatteryStateMeg = 1;   //���ܵ�BMS���״̬��Ϣ
						break;
					//���嶯�����ص�ѹBMV 
					case 0x181556F4:   
						break;
					//�������¶�BMT   
					case 0x181656F4:
						break;
					//��������Ԥ������BSP  
					case 0x181756F4:
						break;
					//BMS��ֹ��� BST
					case 0x101956F4:
							 ubBmsChargeStopReason = CANBMS1.Data[0];
							 uwBmsChargeStopFaultReason = (CANBMS1.Data[2]<<8)+CANBMS1.Data[1];
							 ubBmsChargeStopErrorReason = CANBMS1.Data[3];
							 FlagIfReceBmsStopChargeMeg = 1;
						break;
					//BMSͳ�����ݱ��� BSD
					case 0x181C56F4:
							ubBmsEndOfSOC = CANBMS1.Data[0];
							uwBmsEndMinCellVoltage = (CANBMS1.Data[2]<<8)+CANBMS1.Data[1];
							uwBmsEndMaxCellVoltage = (CANBMS1.Data[4]<<8)+CANBMS1.Data[3];
							ubBmsEndMinTemperature = CANBMS1.Data[5];
							ubBmsEndMaxTemperature = CANBMS1.Data[6];
							FlagIfReceBmsStatisticsDataMeg = 1;			
							ubCANEventStatus = 18;
						break;
					//BMS������ BEM
					case 0x081E56F4:
							ubBmsError[0] = CANBMS1.Data[0];
							ubBmsError[1] = CANBMS1.Data[1];
							ubBmsError[2] = CANBMS1.Data[2];
							ubBmsError[3] = CANBMS1.Data[3];
						break;
					//����
					default:
						break;
				}			 			
		 }
   }	
	CAN_DataProcessing();
}

//CAN�¼�����1,���� ������ʶ����  ���Ĵ���CRM
void CAN_mEventProc1(void)
{
	  ubRecognitionResult = 0x00;        //��ʶ���
    if(TRUE == CAN_mSend_CRM_ChargerIdentify())  //�������ͳ�����ʶ����
    {
        ubTimeN100ms = 50;              //����5S��ʱ   50*100ms=5000ms
        ubCANEventStatus = 2;          //ֱ��ת����һ��ת̬
    }
}      
	
//CAN�¼�����2,���� BMS�ͳ�����ʶ����  ���Ĵ���BRM
void CAN_mEventProc2(void)
{
		uint8_t i;
	  //250msδ�յ�BMS���صĳ�����ʶ����
    if(1 == FlagIfCycleSend250ms)     
    {
        if(TRUE == CAN_mSend_CRM_ChargerIdentify( ))  //�������ͳ�����ʶ���� 
        {
            FlagIfCycleSend250ms = 0;
        }
    }
		//�ж��Ƿ��Ƕ�����֡��ͨ������ 
    if(ubCANConnect[0] ==0x10)          
    {
         //������֡ͨ�����ӻظ�
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
         HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	��������J1939���ֽڴ������Ӳ���
         ubCANConnect[0] = 0x00;
    }
		//�ж϶�����֡�����Ƿ������� 
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
         //�ظ��������䱨��
         CanHandleToBMS.pTxMsg->Data[0] = 0x13;
         CanHandleToBMS.pTxMsg->Data[1] = 0x29;
         CanHandleToBMS.pTxMsg->Data[2] = 0x00;
         CanHandleToBMS.pTxMsg->Data[3] = 0x06;
         CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
         CanHandleToBMS.pTxMsg->Data[5] = 0x00;
         CanHandleToBMS.pTxMsg->Data[6] = 0x02;
         CanHandleToBMS.pTxMsg->Data[7] = 0x00;
         ubCANBufIndex_BMS = 0;
         HAL_CAN_Transmit(&CanHandleToBMS, 0xff);   //	��������J1939���ֽڴ������Ӳ���  ���ݽ��ս�����������Ϣ����Ӧ��
         ubRecognitionResult = 0xAA;         
         CAN_mSend_CRM_ChargerIdentify( );  //�������ͳ�����ʶ���ģ���ʶ��Ч
         uwCANTimeOutCount = 0;  
         ubCANMultiFrameIndex = 0;
		     CAN_mCANBufReset_BMS();
         FlagIfTimeout = 0;
         ubCANEventStatus = 3 ;          
    }
		//�ж��Ƿ�ʱ5S
    if(FlagIfTimeout == 1)          
    {
        ubChargerError[0]|=0x01;     //����BMS�ͳ�����ʶ���ĳ�ʱ
        CAN_mSend_CEM_ChargerError(); //�������ʹ�����  ����BMS�ͳ�����ʶ���ĳ�ʱ
        ubCANEventStatus = 20;       //��ʱ������
        FlagIfTimeout = 0;           //5s��ʱ��־λ����
    }
}     

 //CAN�¼�����3,���� �������س���������  ���Ĵ���BCP
void CAN_mEventProc3(void)
{
	 //�ж��Ƿ��Ƕ��ֽ�����֡��
    if(ubCANConnect[0] == 0x10)       
    {
         //���ֽ�����֡���ӻظ�
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
		 HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	��������J1939���ֽڴ������Ӳ���  //	��������J1939���ֽڴ������Ӳ���
		 ubCANConnect[0] = 0x00;
    }
		//�ж϶�����֡�����Ƿ������� 
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
			HAL_CAN_Transmit(&CanHandleToBMS, 0xff);   //	��������J1939���ֽڴ������Ӳ���  ���ݽ��ս�����������Ϣ����Ӧ��
			ubCANMultiFrameIndex =0;
			CAN_mCANBufReset_BMS( );//CAN�������ݻ��������ݸ�λ
			mDelayCAN10us(1);       
			CAN_mSend_CTS_ChargerTime();  // ��������ʱ��ͬ����Ϣ����
			mDelayCAN10us(1); 
			CAN_mSend_CML_ChargerMaxOutputCapactity();  //���������������������ģ�����ֱ����DAvE�̶����˺�������Ҫװ�����ݣ�ֱ�ӷ���
			uwCANTimeOutCount = 0;                 //CANͨ�ų���ͨ�ų�ʱ����������
			FlagIfTimeout = 0;
			ubCANEventStatus = 6;
			ubFlagIfReceBatteryParameterMeg = 0;
    }
		//�ȴ���  ���ڷ��� ������ʶ������Чָ�� 
    if(1 == FlagIfCycleSend250ms)     //�ȴ���  ���ڷ��� ������ʶ������Чָ��  
    {
         if(TRUE == CAN_mSend_CRM_ChargerIdentify( ))  //�������ͳ�����ʶ���� 
         {
             FlagIfCycleSend250ms = 0;
         }
     }
	  //�ж��Ƿ�ʱ5S
    if(FlagIfTimeout == 1)             //�ж��Ƿ�ʱ5S
    {
        ubChargerError[1]|=0x01;       // �������س��������ĳ�ʱ
        if(TRUE == CAN_mSend_CEM_ChargerError())  // �������ʹ�����  �������س��������ĳ�ʱ
        {
            ubCANEventStatus = 20;        //��ʱ������
            FlagIfTimeout = 0;         
        }
    }	
}    

//CAN�¼�����4,���� ��������ʱ��ͬ����Ϣ����  ���Ĵ���CTS
void CAN_mEventProc4(void)
{
	
}    

//CAN�¼�����5,���� ������������������  ���Ĵ���CML
void CAN_mEventProc5(void)
{
	
}     

//CAN�¼�����6,���� ��س��׼������״̬����  ���Ĵ���BRO
void CAN_mEventProc6(void)
{
	if(1 == FlagIfCycleSend500ms)     //�ж��Ƿ��������ڷ��͵�����
    {
         if(TRUE == CAN_mSend_CTS_ChargerTime( ))  // ��������ʱ��ͬ����Ϣ����
         {
             mDelayCAN10us(1);
             if(TRUE == CAN_mSend_CML_ChargerMaxOutputCapactity( ))  //���������������������ģ�����ֱ����DAvE�̶����˺�������Ҫװ�����ݣ�ֱ�ӷ��� 
             {
                 FlagIfCycleSend500ms = 0;
             }
         }
    }
   if(FlagIfReceBmsReadyMeg == 1)         //���BMS׼��������־λ��1��BMS�ѷ���BRO����
    {
         CAN_mCANBufReset_BMS();//CAN�������ݻ��������ݸ�λ
         ubCANBufIndex_BMS= 0; 
         if(0xAA == ubBmsChargeReady)     //��س��׼������
         {  
             ubChargerOutputReady =0xAA;     
             CAN_mSend_CRO_ChargerOutputReady( );  //�������ͳ������׼������״̬����
             uwCANTimeOutCount = 0;
             FlagIfTimeout = 0;
             //ubCANEventStatus = 7; 
             ubCANEventStatus = 10;					 
             FlagIfSendDCModuleMeg = 1;           //DC ��Դ���ķ�����Ч
             ubDCModue = CAN_STARTDCMODUE;      //������Դָ��
             //CANChargeFlag = 1;
			       //ubDCModue = 0x55;      //������Դָ��
             FlagIfCycleSend1000ms = 1; //������Ч
             uwCANDCTimeOutCount =0;
         }
         FlagIfReceBmsReadyMeg =0; 
    }
   if(FlagIfTimeout == 1)        //�ж��Ƿ�ʱ5S
    {
        ubChargerError[1]|=0x04;  //����BMS��ɳ��׼�����ĳ�ʱ
        if(TRUE == CAN_mSend_CEM_ChargerError())  // �������ʹ�����  ����BMS��ɳ��׼�����ĳ�ʱ
        {
            ubCANEventStatus = 20;     //��ʱ������
            FlagIfTimeout = 0;
        }
    }
}      

//CAN�¼�����7,���� �������׼������״̬����  ���Ĵ���CRO
void CAN_mEventProc7(void)
{
	 if(1 == FlagIfCycleSend250ms)        //�ж��Ƿ��������ڷ���
    {
        if(TRUE == CAN_mSend_CRO_ChargerOutputReady( ))  //�������ͳ������׼������״̬����
        {
			      ubCANEventStatus = 10;
            FlagIfCycleSend250ms = 0;
        }
    }
   if(1 == FlagIfTimeout)           //�ж�BMS���͵�س����״̬����
    {
        ubChargerError[2]|=0x01;
        if(TRUE == CAN_mSend_CEM_ChargerError())  // �������ʹ�����  ����BMS�ͳ�����ʶ���ĳ�ʱ
        {
            ubCANEventStatus = 20;//��ʱ������
            FlagIfTimeout = 0;
        }
    }
}    

//CAN�¼�����8,���� ��س��������  ���Ĵ���BCL
void CAN_mEventProc8(void)
{
	
}     

//CAN�¼�����9,���� ��س����״̬����  ���Ĵ���BCS
void CAN_mEventProc9(void)
{
	
} 

//CAN�¼�����10,���� �������״̬���� ���Ĵ���CCS
void CAN_mEventProc10(void)
{
	  //uint16_t i,j;
      //uint8_t Buf[592];
    //���BMS����CCS����
    if(FlagIfCycleSend50ms == 1)     //50ms���ڷ���
    { 
        if(TRUE == CAN_mSend_CCS_ChargerChargeState())  //�������ͳ��״̬����
        {
            FlagIfCycleSend50ms = 0;
        }
    }
		mDelayCAN10us(2);
    //������յ�BMS״̬���ģ����쳣�ж�
    if(1 == FlagIfReceBmsBatteryStateMeg)
    {
        if(!(uwStatusAndAbnormal & 0x1000))  //�ж϶�������״̬��Ϣ���Ƿ������� 7.6,7.5 00��ֹ 01 ����
        {
            ubChargerStopReason = 0x00;
            CAN_mSend_CST_ChargerStop();  //�������ͳ�����ֹ��籨��
            ubCANEventStatus = 16;//������ֹ���
            CANChargeFlag = 0;
            FlagIfReceBmsStopChargeMeg = 0;
					  //FlagIfReceBmsBatteryStateMeg = 1;
            mDelayCAN10us(4);
            uwVoltageRequirement = 0;
            uwCurrentRequirement = 0;
            ubDCModue = CAN_STOPDCMODUE ;  //�رճ���
            CAN_mSendDCModuleStartOrStop(0x00);  //	������ֱ�����ģ�鷢������ֹͣ����
        }
        FlagIfReceBmsBatteryStateMeg = 0;
    }
		//�ж��Ƿ��յ�BMS��ֹ��籨��
    if(FlagIfReceBmsStopChargeMeg == 1)   //һЩ�������յ�BMS���͵���ֹ��籨�Ļ��߳���������ֹ���������������������ֹ��磬������ǹû�б�������֮ǰһֱ����
    {
					ubChargerStopReason = 0x00;
					CAN_mSend_CST_ChargerStop();  //�������ͳ�����ֹ��籨��
					ubCANEventStatus = 16;//������ֹ���
					FlagIfReceBmsStopChargeMeg = 0;
					mDelayCAN10us(2);
					uwVoltageRequirement = 0;
					uwCurrentRequirement = 0;  
					ubDCModue = CAN_STOPDCMODUE ;
					//ubDCModue = 0xAA;
					CAN_mSendDCModuleStartOrStop(0x00) ;  //	������ֱ�����ģ�鷢������ֹͣ����
    }
    //����BMS�������ĵ�س����״̬(BCS)
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
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	��������J1939���ֽڴ������Ӳ���
						 CAN_mCANBufReset_BMS();//CAN�������ݻ��������ݸ�λ
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
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	��������J1939���ֽڴ������Ӳ���  ���ݽ��ս�����������Ϣ����Ӧ��            
             uwCANTimeOutCount = 0;
             FlagIfTimeout = 0;
             ubCANEventStatus = 10;
        }
    }
		//���յ�BMS�������ĵ��嶯�����ص�ѹ(BMV)����
    if(ubManyBytesStateBMV == 1)
    {
        if(ubCANConnect[0] == 0x10)
        {
             ubCellVoltageQuantity = (uint8_t)((ubCANConnect[1]+(ubCANConnect[2]<<8))/2);     //���嶯�����ص�ص�ѹ���� 
             ubCellVoltageMegQuantity = ubCANConnect[3];     //���嶯�����ص�ص�ѹ ����֡���� 
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
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff); //	��������J1939���ֽڴ������Ӳ���
						 ubCANBufIndex_BMS = 0;
             ubCANConnect[0] = 0x00;
						 ubCANMultiFrameIndex = 0;
						 CAN_mCANBufReset_BMS();//CAN�������ݻ��������ݸ�λ
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
             CanHandleToBMS.pTxMsg->Data[3] = ubCANMultiFrameIndex;//��������
             CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[5] = 0x00;
             CanHandleToBMS.pTxMsg->Data[6] = 0x15;
             CanHandleToBMS.pTxMsg->Data[7] = 0x00;
						 ubCANMultiFrameIndex = 0;
						 CAN_mCANBufReset_BMS();
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	��������J1939���ֽڴ������Ӳ���  ���ݽ��ս�����������Ϣ����Ӧ��        
             uwCANTimeOutCount = 0;
             FlagIfTimeout = 0;
             ubCANEventStatus = 10;
             ubManyBytesStateBMV = 0;
        }
        ubCANEventStatus = 10;
    }
    //�ж�BMS�����Ķ��������¶�(BMT)����
    if(ubManyBytesStateBMT == 1)
    {
        if(ubCANConnect[0] == 0x10)
        {
             ubCellTemperatureQuantity = (uint8_t)((ubCANConnect[1]+(ubCANConnect[2]<<8))/2); ; //���������¶� ����
             ubCellTemperatureMegQuantity = ubCANConnect[3];  //���������¶� ����֡����
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
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);   //	��������J1939���ֽڴ������Ӳ���
             ubCANConnect[0] = 0x00;
						 ubCANMultiFrameIndex = 0;
						 CAN_mCANBufReset_BMS();//CAN�������ݻ��������ݸ�λ
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
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	��������J1939���ֽڴ������Ӳ���  ���ݽ��ս�����������Ϣ����Ӧ��
             CAN_mCANBufReset_BMS();   
						 ubManyBytesStateBMT = 0;			 
             uwCANTimeOutCount = 0;
             FlagIfTimeout = 0;
             ubCANEventStatus = 10;
        }
    }
    //�ж�����BMS�Ķ�������Ԥ������(BSP)
    if(ubManyBytesStateBSP == 1)
    {        
         if(ubCANConnect[0] ==0x10)
         {
             ubCellReservedQuantity =(uint8_t)((ubCANConnect[1]+(ubCANConnect[2]<<8))/2); ;    //��������Ԥ������  ����
             ubCellReservedMegQuantity = ubCANConnect[3];    //��������Ԥ������  ����֡����
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
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	��������J1939���ֽڴ������Ӳ���
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
             CanHandleToBMS.pTxMsg->Data[3] = ubCellReservedMegQuantity;//3����
             CanHandleToBMS.pTxMsg->Data[4] = 0xFF;
             CanHandleToBMS.pTxMsg->Data[5] = 0x00;
             CanHandleToBMS.pTxMsg->Data[6] = 0x17;
             CanHandleToBMS.pTxMsg->Data[7] = 0x00;
             HAL_CAN_Transmit(&CanHandleToBMS, 0xff);  //	��������J1939���ֽڴ������Ӳ���  ���ݽ��ս�����������Ϣ����Ӧ��
             ubCANMultiFrameIndex = 0;
						 CAN_mCANBufReset_BMS();//CAN�������ݻ��������ݸ�λ
             uwCANTimeOutCount = 0;
             FlagIfTimeout = 0;
             ubCANEventStatus = 10;
             ubManyBytesStateBSP = 0;
        } 
    }
}      

 //CAN�¼�����11,���� ��������״̬��Ϣ���� ���Ĵ���BSM
void CAN_mEventProc11(void)
{
	
} 

//CAN�¼�����12,���� ���嶯�����ص�ѹ���� ���Ĵ���BMV
void CAN_mEventProc12(void)
{
	
}    

//CAN�¼�����13,���� ���������¶ȱ��� ���Ĵ���BMT
void CAN_mEventProc13(void)
{
	
}      

//CAN�¼�����14,���� ��������Ԥ������ ���Ĵ���BSP
void CAN_mEventProc14(void)
{
	
}   

 //CAN�¼�����15,���� BMS��ֹ��籨�� ���Ĵ���BST
void CAN_mEventProc15(void)
{

}     

//CAN�¼�����16,���� ������ֹ��籨�� ���Ĵ���CST
void CAN_mEventProc16(void)
{
	if(FlagIfCycleSend10ms == 1)
    {
         if(TRUE == CAN_mSend_CST_ChargerStop())  //�������ͳ�����ֹ��籨��
         {
             FlagIfCycleSend10ms = 0;
         }
    }
    if(FlagIfReceBmsStopChargeMeg == 1)
    {
         ubCANEventStatus = 17;
         ubTimeN100ms = 50;  //����5S��ʱ   50*100ms=5000ms
         uwCANTimeOutCount = 0;
         FlagIfReceBmsStopChargeMeg = 0;
    }
}      

 //CAN�¼�����17,���� BMSͳ�Ʊ��� ���Ĵ���BSD
void CAN_mEventProc17(void)
{
	 if(FlagIfReceBmsStatisticsDataMeg == 1)
     {
         if(TRUE == CAN_mSend_CSD_ChargerStatisticsData( ))  //  �������ͳ���ͳ������ 
         {
             FlagIfReceBmsStatisticsDataMeg = 0;
             ubCANEventStatus = 18;
         }
     }
}     

//CAN�¼�����18,���� ����ͳ�Ʊ��� ���Ĵ���CSD
void CAN_mEventProc18(void)
{
	if(FlagIfReceBmsStatisticsDataMeg == 1)
    {      
         if(TRUE == CAN_mSend_CSD_ChargerStatisticsData( ))  //  �������ͳ���ͳ������ 
         {
             FlagIfReceBmsStatisticsDataMeg = 0;
             ubCANEventStatus = 24;  //��Ч�������
         }
         FlagIfSendDCModuleMeg = 0;
				 ResetACRwlay();      //ֹͣ�����̵���
				 ResetDCRwlay();      //ֱֹͣ���Ӵ���
				 ResetBMSRwlay();//ֹͣ������Դ
    } 
// 	ResetACRwlay();      //ֹͣ�����̵���
// 	ResetDCRwlay();      //ֱֹͣ���Ӵ���
// 	ResetBMSRwlay();//ֹͣ������Դ
}      

//CAN�¼�����19,���� BMS������ ���Ĵ���BEM
void CAN_mEventProc19(void)
{
	
}    

//CAN�¼�����20,���� ���������� ���Ĵ���CEM

void CAN_mEventProc20(void)
{
	
}  

//CAN�¼�����21,���� �������Դģ�鷢�������� ���Ĵ���CQM
void CAN_mEventProc21(void)
{
	
}      

//CAN�¼�����22,���� ��Դģ������巢��״̬��Ϣ ���Ĵ���MSM
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
uint8_t BCDToChar(uint8_t bcd)			 //������BCD��ת��������
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

//CAN�ж����ݴ���
void CAN_DataProcessing(void)
{
	//�ж�DC������
	switch(uCANBufID_DC)
	{
				//���1����
				case 0x1207C081:   //����趨���ʱģ��ظ����		
				{
						switch(ubCANBuf_DC[0])
							{
								case 0x00:   //����趨���ʱģ��ظ����									
													break;
								case 0x01:   //��ض�ȡ��Ϣʱģ��ظ�״̬
														uwDCOutputCurrent[1] = (ubCANBuf_DC[3])+(ubCANBuf_DC[2]<<8);
														uwDCOutputVoltage[1] = (ubCANBuf_DC[5])+(ubCANBuf_DC[4]<<8);
														ubDCModuleStatusHigh[1] = ubCANBuf_DC[6];
														ubDCModuleStatusLow[1] = ubCANBuf_DC[7];
													break;
								case 0x02:   //����趨���ػ�ʱģ��ظ����ػ�
									
													break;
													
								default:
													break;
							}
				}					
							break;
				//���2����
				case 0x1207C082:   //��ض�ȡ��Ϣʱģ��ظ�״̬       
				{
					switch(ubCANBuf_DC[0])
							{
								case 0x00:   //����趨���ʱģ��ظ����									
													break;
								case 0x01:   //��ض�ȡ��Ϣʱģ��ظ�״̬
														uwDCOutputCurrent[2] = (ubCANBuf_DC[3])+(ubCANBuf_DC[2]<<8);
														uwDCOutputVoltage[2] = (ubCANBuf_DC[5])+(ubCANBuf_DC[4]<<8);
														ubDCModuleStatusHigh[2] = ubCANBuf_DC[6];
														ubDCModuleStatusLow[2] = ubCANBuf_DC[7];
													break;
								case 0x02:   //����趨���ػ�ʱģ��ظ����ػ�
									
													break;
													
								default:
													break;
							}
				 } 		
									break;
				//���3����
				case 0x1207C083:   //����趨���ػ�ʱģ��ظ����ػ�	
				{
					switch(ubCANBuf_DC[0])
							{
								case 0x00:   //����趨���ʱģ��ظ����									
													break;
								case 0x01:   //��ض�ȡ��Ϣʱģ��ظ�״̬
														uwDCOutputCurrent[3] = (ubCANBuf_DC[3])+(ubCANBuf_DC[2]<<8);
														uwDCOutputVoltage[3] = (ubCANBuf_DC[5])+(ubCANBuf_DC[4]<<8);
														ubDCModuleStatusHigh[3] = ubCANBuf_DC[6];
														ubDCModuleStatusLow[3] = ubCANBuf_DC[7];
													break;
								case 0x02:   //����趨���ػ�ʱģ��ظ����ػ�
									
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



void ChargeOfManualOperation(void)     //�ֶ����
{
		if(FlagIfSendDCMeg ==1)
		{
					FlagIfSendDCMeg = 0;				
					switch(CANCount)
						{
							case 0:  //	��ֱ�����ģ�鷢���趨������Ʊ���,�㲥����
									CAN_mSendDCModuleSetOutputParameter(0x00);
									break;
							case 1: 							
									CAN_mSendDCModuleReadStute(ubDCModueAddress);//�ֱ��ȡ���ģ��
									ubDCModueAddress++;//��ȡģ���ַ��1���л�ģ��
									if(ubDCModueAddress == (DCMODUENUMBER+1))
									{
										ubDCModueAddress = 1;
									}
									break;
							case 2: //	������ֱ�����ģ�鷢������ֹͣ��������㲥����
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






