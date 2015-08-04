#ifndef __CAN_H
#define __CAN_H

#include "stm32f2xx_hal.h"

struct _StrCAN
{
	 uint32_t  IDE;
	 uint8_t   DLC;
	 uint8_t   Data[8];
};

#define BRM_R_FLAG   0x0001  //0b0000000000000001   BMS�ͳ�����ʶ����
#define BCP_R_FLAG   0x0002  //0b0000000000000010   �������س�����
#define BRO_R_FLAG   0x0004  //0b0000000000000100   ��س��׼������״̬
#define BCL_R_FLAG   0x0008  //0b0000000000001000   ��س������
#define BCS_R_FLAG   0x0010  //0b0000000000010000   ��س����״̬
#define BSM_R_FLAG   0x0020  //0b0000000000100000   ��������״̬��Ϣ
#define BMV_R_FLAG   0x0040  //0b0000000001000000   ���嶯�����ص�ѹ
#define BMT_R_FLAG   0x0080  //0b0000000010000000   ���������¶�
#define BSP_R_FLAG   0x0100  //0b0000000100000000   ��������Ԥ������
#define BST_R_FLAG   0x0200  //0b0000001000000000   BMS��ֹ���
#define BSD_R_FLAG   0x0400  //0b0000010000000000   BMSͳ������
#define BEM_R_FLAG   0x0800  //0b0000100000000000   BMS������
#define DOS_R_FLAG   0x1000  //0b0001000000000000   DC��Դ״̬����

#define CRM_S_FLAG   0x0001  //0b0000000000000001   ������ʶ����
#define CTS_S_FLAG   0x0002  //0b0000000000000010   ��������ʱ��ͬ����Ϣ
#define CML_S_FLAG   0x0004  //0b0000000000000100   ��������������
#define CRO_S_FLAG   0x0008  //0b0000000000001000   �������׼������״̬
#define CCS_S_FLAG   0x0010  //0b0000000000010000   �������״̬
#define CST_S_FLAG   0x0020  //0b0000000000100000   ������ֹ���
#define CSD_S_FLAG   0x0040  //0b0000000001000000   ����ͳ������
#define CEM_S_FLAG   0x0080  //0b0000000010000000   ����������
#define DPS_S_FLAG   0x0100  //0b0000000100000000   ������������
#define DSS_S_FLAG   0x0200  //0b0000001000000000   ��������ֹͣ

#define TRUE 1
#define FALSE 0

#define DCMODUENUMBER  3   //DCģ�������
void CAN_Config(void);
void CAN_mEvent(void);       //CAN�¼�����
void CAN_DataProcessing(void);    //CAN�ж����ݴ���
void CANQueue_Reset(void);

void CAN_mInitData(void);//��ʼ��CAnͨ������������ݣ�������ģ���ṩ��������������ģ���ʼ��
uint8_t CAN_mSend_CCS_ChargerChargeState(void);  //�������ͳ��״̬����
uint8_t CAN_mSendDCModuleStartOrStop(uint8_t modueaddress);   //	������ֱ�����ģ�鷢������ֹͣ����
uint8_t CAN_mSend_CST_ChargerStop(void);  //�������ͳ�����ֹ��籨��

void CANtoBMS_Config(void);
void CANqueue_push(struct _StrCAN StrCAN);

uint16_t CAN_mAccumulateChargeTime(void);     //������ʱ��������ʱ�䳤�ȣ�����Ϊ��λ
void ChargeOfManualOperation(void);     //�ֶ����
uint8_t BCDToChar(uint8_t bcd);


#endif /* __CAN_H */

