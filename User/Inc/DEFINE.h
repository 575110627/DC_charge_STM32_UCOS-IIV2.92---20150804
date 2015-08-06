#ifndef __DEFINE_H
#define __DEFINE_H

#include "stm32f2xx_hal.h"

/***********************************************
**�������ȼ�����
***********************************************/
#define  Falg_SUCCEED_PRIO_MEM                1 //��־�̳����ȼ�
#define  Falg_SUCCEED_PRIO                    2 //��־�̳����ȼ�
#define  SYSMonitorTask_PRIO                  3
#define  LED1Task_PRIO                        4
#define  LED2Task_PRIO                        5
#define  LED3Task_PRIO		                  6



/***********************************************
**�����ջ��������
***********************************************/
#define SYSMonitorTaskStackLengh              128
#define LED1TaskStackLengh                  128
#define LED2TaskStackLengh				          128
#define LED3TaskStackLengh    			128

extern 	uint8_t ReadBuffer[100];
//CANͨ�ű����¼���״̬��
extern uint8_t  ubCANEventStatus;
/* CANͨ������֡����  */
/*----------------------------------------------
-----------------���ֽ׶ζ���------------------
-----------------------------------------------*/
 //��ʶ�������������֮�󣬳�����BMS���ʹ˱��ģ����յ�BMS���͵ı�ʶ����֮ǰ����0x00�����յ�BMS��ʶ����֮����0xAA
extern uint8_t ubRecognitionResult;	
//������ţ�1/λ��1ƫ���������ݷ�Χ1-100	
extern uint8_t ubChargerNumber;

/*------------------------------------------------
------------PGN512,BRM���Ĳ���--------------------
-------------------------------------------------*/
//BMSͨ��Э��汾�ţ�����׼�涨��ǰ�汾ΪV1.0 ��ʾΪ��byte3 byte2 byte1  0x 00 01 00
extern uint32_t ulBmsProtocolVersion;
//������ͣ�01H��Ǧ���� 02H��������	03��������﮵�� 04H������﮵�� 05H������﮵�� 06H����Ԫ���ϵ�� 07H���ۺ�������ӵ�� 08H������﮵�� FFH���������
extern uint8_t ubBatteryType;
//������������ϵͳ�����/A*h,0.1A*h/λ��0.1A*hƫ���� ���ݷ�Χ :0-����1000A.h
extern uint16_t uwBmsRateCapacity;
//������������ϵͳ��ܵ�ѹ /V ��O.l V/λ ��0 V ƫ���������ݷ� Χ:0  750 V
extern uint16_t uwBatteryRateVoltage;
//��������������ƣ���׼ ASCII ��
extern uint8_t ubBatteryManufacturerName[4];
//�������ţ�Ԥ�����ɳ������ж���
extern uint16_t ulBatteryManufacturerName;
//������������ڣ��� 1985ƫ��λ
extern uint8_t ubBatteryProduceYear;
//������������ڣ���
extern uint8_t ubBatteryProduceMonth;
//������������ڣ���
extern uint8_t ubBatteryProduceDay;
//������������1��ÿλ  ,3���ֽ�
extern uint16_t ulBatteryChargeCycle;
//������Ȩ��־  0��ʾ����   1��ʾ ������
extern uint8_t ubPropertyMark;
//Ԥ��
extern uint8_t ubBMSReserved;
//����ʶ����VehicleIdentificationNumber
extern uint8_t ubVIN[17];

/*-----------------------------------------------
-------------------�������ý׶α���--------------
-------------------------------------------------*/


/*-----------------------------------------------
-------------------PGN1536 BCP���Ĳ���-------------
-------------------------------------------------*/
//���嶯����������������ѹ  ���ݷֱ��� :0.01 V/λ�� 0 V ƫ���� E ���ݷ�Χ :0 V~24V;   
extern uint16_t uwCellMaxAllowChargeVoltage;
//������������	 ���ݷֱ��� :0.lA/λ�� -400 A ƫ����;���ݷ�Χ: -400 A ~O A;
extern  uint16_t uwMaxAllowChargeCurrent;
//�������ر��������	  ���ݷֱ��� :0.1 kW  o h/ λ�� 0 kW. h ƫ����;���ݷ�Χ :0��.....��1 000  kW o h; 
extern uint16_t uwBatteryTotalEnergy;
//����������ܵ�ѹ   ���ݷֱ��� :0.1 V/ λ�� 0  V ƫ����;���ݷ�Χ :0 V ��......750  V;
extern uint16_t uwMaxAllowChargeVoltage;
//��������¶�		 ���ݷֱ��� :1 oC/λ��-50 OC ƫ���� F ���ݷ�Χ:һ 50 oC ��.....�� +200 oC;
extern uint8_t ubMaxAllowTemperature;
//�����������غɵ�״̬	���ݷֱ��� :0.1%/λ�� 0%ƫ����;���ݷ�Χ :0��.....��100% ; 
extern uint16_t uwBmsSOC;
//�������������ܵ�ѹ
extern uint16_t uwBmsTotalVoltage;
//---------PGN2048 CML����-------------
//DC��������ѹ
extern uint16_t uwDCMaxOutputVoltage;
//DC��������ѹ
extern uint16_t uwDCMinOutputVoltage;
//DC����������
extern uint16_t uwDCMaxOutputCurrent;
//BMS �Ƿ���׼����(< OxOO-) := BMS δ���ó��׼��; (OxAA):=BMS ��ɳ��׼��; (OxFF):=ԪЧ)
extern uint8_t ubBmsChargeReady;
//�����Ƿ���׼���� ((OxOO):=����δ��ɳ��׼��; (OxAA):=������ɳ��׼��; (OxFF):=��Ч)
extern  uint8_t ubChargerOutputReady;

/*���׶α���*/
//��ѹ����(V)
extern uint16_t uwVoltageRequirement;
extern uint8_t uwVoltageRequirementLow;
extern uint8_t uwVoltageRequirementHigh;
//�������� (A)
extern int16_t uwCurrentRequirement;
extern int8_t uwCurrentRequirementLow;
extern int8_t uwCurrentRequirementHigh;
extern int8_t uwCurrentRequirementLowbuf;
extern int8_t uwCurrentRequirementHighbuf;
extern int16_t uwCurrentRequirementbuf;
extern uint16_t CURRENT;
//���ģʽ (Ox01:��ѹ���	;Ox02:�������)
extern uint8_t ubChargeMode;
//����ѹ����ֵ (V)
extern  uint16_t uwChargeVoltageMeasurement;
//����������ֵ (A)
extern uint16_t uwChargeCurrentMeasurement;
//��ߵ��嶯�����ص�ѹ�������
extern uint16_t uwHighestCellVoltageAndGroupNumber;
//��ǰ�ɵ�״̬ SOC(%)
extern  uint8_t ubCurrentSOC;
//����ʣ����ʱ�� (min)
extern  uint16_t uwRemainChargeTime;
//��ѹ���ֵ (V)
extern  uint32_t uwOutputVoltageValue;
//�������ֵ (A)
extern uint32_t uwOutputCurrentValue;
//�ۼƳ��ʱ�� (min)
extern uint16_t uwAccumulateChargeTime;
//��ߵ��嶯�����ص�ѹ���ڱ��
extern uint8_t ubMaxCellVoltageNumber;
//��߶��������¶�
extern uint8_t ubMaxTemperature;
//����¶ȼ�����
extern uint8_t ubMaxTemperatureNumber;
//��Ͷ��������¶�
extern uint8_t ubMinTemperature;
//��Ͷ��������¶ȼ�����
extern uint8_t ubMinTemperatureNumber;
//�쳣״̬��Ϣ	 
//���嶯�����ص�ѹ����/���� ((00): =���� ;{01):= ����;(10):=����)
//�����������غɵ�״̬ soc ����/���� ((00): =���� ;(01):= ����;(10):= ���͡�
//�������س������� ((00):= ���� ;(01):== ����;(10): =������ ״̬��
//���������¶ȹ��� ((00): =����; (01): =:����;(1 0):= ������ ״̬
//�������ؾ�Ե״̬ ((00): =���� ;(01):=������;(10):= ������ ״̬��
//�����������������������״̬ ((00):= ����; (01): =������;(10): =������״̬��
//������� ((00):= ��ֹ ;(01):=����
extern uint16_t uwStatusAndAbnormal;
//���嶯�����ص�ѹ
extern uint16_t uwCellVoltage[256];
//���嶯�������¶�
extern uint8_t ubCellTemperature[128];
//���嶯������Ԥ������
extern uint8_t ubCellReserved[16];
//BMS  ��ֹ���ԭ��
extern uint8_t ubBmsChargeStopReason;
//BMS  ��ֹ������ԭ��
extern uint16_t uwBmsChargeStopFaultReason;
//BMS  ��ֹ������ԭ��
extern uint8_t ubBmsChargeStopErrorReason;
//������ֹ���ԭ��
extern uint8_t ubChargerStopReason;
//�˵����ֹ������ԭ��
extern uint16_t uwChargerStopFaultReason;
//������ֹ������ԭ��
extern uint8_t ubChargerStopErrorReason;
//��ֹ�ɵ�״̬ SOC(%)
extern uint8_t ubBmsEndOfSOC;
//�������ص�����͵�ѹ (V)
extern  uint16_t uwBmsEndMinCellVoltage;
//�������ص�����ߵ�ѹ (V)
extern uint16_t uwBmsEndMaxCellVoltage;
//������������¶� CC)
extern uint8_t ubBmsEndMinTemperature;
//������������¶� CC)
extern uint8_t ubBmsEndMaxTemperature;
//�ۼƳ��ʱ�� (min)
extern uint16_t uwChargerEndChargeTime;
//������� (kW. h)
extern uint16_t uwChargerEndChargePower;
//BMS ������ (BEM)
extern  uint8_t ubBmsError[4];
//���������� (CEM)
extern  uint8_t ubChargerError[4];


//DCֱ�����ģ��״̬
//extern uint8_t ubDCModuleStatusHigh;
//extern uint8_t ubDCModuleStatusLow;

extern uint8_t ubDCModuleStatusHigh[4];
extern uint8_t ubDCModuleStatusLow[4];
// DC��ѹ���
extern uint16_t uwDCOutputVoltage[4];
//DC�������
extern uint16_t uwDCOutputCurrent[4];
// BMS�����緽ʽ����ֵ (Ox01:��ѹ���	;Ox02:�������)
extern uint8_t ubChargeModeBuf;

//DCֱ�����ģ��״̬
extern uint8_t ubDCModuleStatusHighAll; 
                                 //��0λ 0��ʾģ������ 1��ʾ�ػ� 
                                 //��1λ 0��ʾģ������ 1��ʾ���� 
                                 //��2λ 0��ʾģ���ѹ 1��ʾ����
                                 //��3λ 0��ʾ�������� 1��ʾ����  
                                 //��4λ 0��ʾ�������� 1��ʾ��ѹ  
                                 //��5λ 0��ʾ�������� 1��ʾǷѹ  
                                 //��6λ 0��ʾ������� 1��ʾ��ѹ  
                                 //��7λ 0��ʾ������� 1��ʾǷѹ                                
extern uint8_t ubDCModuleStatusLowAll; 
                                 //��0λ 0��ʾ�������� 1��ʾ���� 
                                 //��1λ 0��ʾ���±��� 1��ʾ���� 
                                 //��2λ 0��ʾ���ùػ� 1��ʾ����
                              
//DCֱ�����ģ��״̬
extern uint16_t uwDCModuleStatus;



//��־���Ƿ�ʱ5s 0x00�����㣬0xAA����
extern  uint8_t FlagIfTimeout;
//��־��DC��Դģ���Ƿ�ʱ5s 0x00�����㣬0xAA����
extern uint8_t FlagIfDCTimeout;
//��ʱʱ�����ã�ubTimeNs��ʾ���ٸ�100ms
extern uint8_t ubTimeN100ms;


//��־���Ƿ��������ڷ��� ��������10ms 0�����㣬1����
extern uint8_t FlagIfCycleSend10ms;
//��־���Ƿ��������ڷ��� ��������50ms 0�����㣬1����
extern uint8_t FlagIfCycleSend50ms;
//��־���Ƿ��������ڷ��� ��������250ms 0�����㣬1����
extern uint8_t FlagIfCycleSend250ms;
//��־���Ƿ��������ڷ��� ��������500ms 0�����㣬1����
extern uint8_t FlagIfCycleSend500ms;
//��־���Ƿ��������ڷ��� ��������1000ms 0�����㣬1����
extern uint8_t FlagIfCycleSend1000ms;

//��־���Ƿ��յ�BMS���͵�BMS�ͳ�����ʶ���� 0x00δ�յ���0xAA�յ�
extern uint8_t ubFlagIfReceBmsIdentifyMeg;
//��־���Ƿ��յ�BMS���͵ĵ�س��������� 0x00δ�յ���0xAA�յ�
extern uint8_t ubFlagIfReceBatteryParameterMeg;
//��־���Ƿ��յ�BMS���͵ĵ�س��׼������״̬���� 0δ�յ���1�յ�
extern uint8_t FlagIfReceBmsReadyMeg;
//��־���Ƿ��յ�BMS���͵ĵ�س����״̬���� 0x00δ�յ���0xAA�յ�
extern uint8_t ubFlagIfReceBmsAllChargeStateMeg;
//��־���Ƿ��յ�BMS���͵ĵ�س�������� 0x00δ�յ���0xAA�յ�
extern uint8_t FlagIfReceBmsChargeRequirementMeg;
//��־���Ƿ��յ�BMS���͵Ķ�������״̬ 0x00δ�յ���0xAA�յ�
extern uint8_t FlagIfReceBmsBatteryStateMeg;
//��־���Ƿ��յ�BMS���ͽ���������BMS��ֹ��籨�� 0x00δ�յ���0xAA�յ�
extern uint8_t ubFlagIfReceBmsEndAndStopChargeMeg;
//��־���Ƿ��յ�BMS���͵�BMS��ֹ��籨�� 0x00δ�յ���0xAA�յ�
extern uint8_t FlagIfReceBmsStopChargeMeg;
//��־���Ƿ��յ�BMS���͵ĳ��ͳ�����ݱ��� 0x00δ�յ���0xAA�յ�
extern uint8_t FlagIfReceBmsStatisticsDataMeg;


//��־���Ƿ��ͱ��ĸ�DCֱ������ģ�� 0x00�����ͣ�0xAA����
extern uint8_t FlagIfSendDCModuleMeg;
//��־���Ƿ��յ�DCֱ������ģ�鷢�͵ı��� 0û�յ���1�յ�
extern uint8_t FlagIfReceDCModuleMeg;

//��־���Ƿ��ͳ�����ʶ���� 0x00�����ͣ�0xAA����
extern uint8_t ubFlagIfSendChargerIdentifyMeg;
//��־���Ƿ��ͳ���ʱ��ͬ����Ϣ���� 0x00�����ͣ�0xAA����
extern uint8_t ubFlagIfSendChargerTimeMeg;
//��־���Ƿ����������������� 0x00�����ͣ�0xAA����
extern uint8_t ubFlagIfSendChargerMaxOutputCapactityMeg;
//��־���Ƿ��ͳ����������״̬���� 0x00�����ͣ�0xAA����
extern uint8_t ubFlagIfSendChargerOutputReadyMeg;
//��־���Ƿ��ͳ������״̬���� 0x00�����ͣ�0xAA����
extern uint8_t FlagIfSendChargerChargingStateMeg;
//��־���Ƿ������ֹ��籨�� 0x00�����ͣ�0xAA����
extern uint8_t ubFlagIfSendChargerStopChargeMeg;
//��־���Ƿ��ͳ������ͳ�����ݱ��� 0x00�����ͣ�0xAA����
extern uint8_t ubFlagIfSendChargerStatisticsDataMeg;

//��ʱ��6������10ms����һ��   
extern uint8_t ubCANTim3Count;
//��ʱ��6������10ms����һ��,CANͨ�ų���ͨ�ų�ʱ������   
extern uint16_t uwCANTimeOutCount;
//��ʱ��6������10ms����һ��,CANͨ��DC��Դģ�鳬ʱ������   
extern uint16_t uwCANDCTimeOutCount;

//CAN����BMS���ݻ�����  ͨ���ų����¼������
extern uint8_t ubCANBuf_BMS[8];
//CAN�������ݻ�������λ��������
extern uint8_t ubCANBufIndex_BMS;
//CAN�յ�BMS���ݵ�ID
extern uint32_t uCANBufID_BMS;

//CAN����DC���ݻ�����  ͨ���ų����¼������
extern uint8_t ubCANBuf_DC[8];
//CAN����DC���ݻ�������λ��������
extern uint8_t ubCANBufIndex_DC;
//CAN�յ�DC���ݵ�ID
extern uint32_t uCANBufID_DC;


//��֡���ӱ��Ľ���������
extern uint8_t ubCANConnect[8];
extern uint8_t ubCANMultiFrame[70][8];
extern uint8_t ubCANMultiFrameIndex;


extern uint16_t uwCANReceFlagState;//�Ƿ��յ�CAN����֡��־λ 0��ʾ��Ч 1��ʾ��Ч  
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
                                 //��12λ DC��Դ���״̬����
extern uint16_t uwCANSendFlagState;//�Ƿ���CAN����֡��־λ 0��ʾ��Ч 1��ʾ��Ч  
                                 //��0λ ������ʶ����
                                 //��1λ ��������ʱ��ͬ����Ϣ
                                 //��2λ ��������������
                                 //��3λ �������׼����������
                                 //��4λ �������״̬
                                 //��5λ ������ֹ���
                                 //��6λ ����ͳ������
                                 //��7λ ����������
                                 //��8λ DC��Դ���ñ���
                                 //��9λ DC��Դ����ֹͣ����           

extern uint8_t ubManyBytesStateBCS;//��֡���ݵı�־
extern uint8_t ubManyBytesStateBMV;//��֡���ݵı�־
extern uint8_t ubManyBytesStateBMT;//��֡���ݵı�־
extern uint8_t ubManyBytesStateBSP;//��֡���ݵı�־

//�����ݴ���Э�����ӹ���������
extern uint8_t ubCANConnect[8];

extern uint8_t ubCellVoltageQuantity;     //���嶯�����ص�ص�ѹ���� 
extern uint8_t ubCellTemperatureQuantity; //���������¶� ����
extern uint8_t ubCellReservedQuantity;    //��������Ԥ������  ����

extern uint8_t ubCellVoltageMegQuantity;     //���嶯�����ص�ص�ѹ ����֡���� 
extern uint8_t ubCellTemperatureMegQuantity; //���������¶� ����֡����
extern uint8_t ubCellReservedMegQuantity;    //��������Ԥ������  ����֡����

//��������������־
extern uint8_t ubPhysicalConnect;  //0x00��ʾδ���ӣ�0xAA��ʾ��������������������Ч

extern uint16_t uwReadTimeCount;//CANͨ����С��ʱ��λ�Ƿ���    ���ﶨ��30���ȡһ��ʱ�䣬���ڸ���Ҫ���޸�
extern uint8_t FlagIfReadTime;//�Ƿ��ȡʱ���־λ
extern uint8_t FlagIfSendDCMeg;
extern uint8_t FlagIfShowInformation5S ;  //ˢ��������ͣ����־λ
extern uint8_t FlagShowInformation ;

extern uint32_t Flag;

extern uint8_t ubDCModue;      //DCģ���Ƿ�����
//���DC���ģ��ĵ�ַ��1-N��ģ��ֱ�Ϊ0x01 0x02 0x03 ......��
extern uint8_t ubDCModueAddress;
#define CAN_STARTDCMODUE 0x55
#define CAN_STOPDCMODUE 0xAA


extern uint8_t Time[8];

extern uint8_t ChargeFlag ;  //����־ 1:���ڳ�� 0:���ڳ��
extern uint8_t CANChargeFlag ; //���׶α�־

//---------------------------------
//������
//-----------------------------------------
extern uint8_t ICDataBfr[16];
extern uint8_t ICCard_Snr[4];
extern uint8_t ICKEYA_BUF[6];
extern uint8_t ICKEYB_BUF[6];
extern uint8_t ICDataReadBuf[16];
extern uint8_t ICMoney[4];
extern uint32_t ulMoney;
extern uint8_t ICCharge;
extern uint8_t ICDataWriteBuf[16];
extern uint8_t ICCard1[4];
extern uint8_t ICMoney1[4];
extern uint8_t ICCard2[4];
extern uint8_t ICMoney2[4];
extern uint8_t ICReadFlag;
extern uint8_t ICWriteFlag;
extern uint8_t FlagIfOperationCard;
extern uint8_t WriteBlockFlag;//д��ģ���־λ������WriteBlock(1,ICDataReadBuf)����ʱ���˱�־λ��1,д���ɹ�֮��˱�־λ��λ0
extern uint8_t WriteBlockTimeFlag;//д��ģ�����ڷ��ͱ�־��100ms��λһ�Σ����꼴��0

extern uint8_t Tim3ICWaitCount;
extern UART_HandleTypeDef huart6;
extern uint8_t pData[1];

extern uint8_t   StartICMoney[4];
extern uint32_t  StartICMoneyAll;
extern uint8_t   DealMoney[4];
extern uint32_t  DealMoneyAll;
extern uint32_t  StopMoneyAll;
//---------------------------------
//���
//-----------------------------------------
extern uint8_t UART5RxBuffCount;
extern uint8_t UART5RxBuff[24];
extern uint8_t UART5RxFlag;

extern uint8_t	PMsendBuf[8];
extern uint8_t  VoltageA[2];
extern uint8_t  VoltageB[2];
extern uint8_t  VoltageC[2];
extern uint8_t  RectivePower[2];//�޹�����
extern uint8_t  ApparentPower[2];//�����ڹ���
extern uint8_t  Frequency[2];//Ƶ��
extern uint8_t  ACEnergy [4]; //�й�����
extern uint8_t  CurrentA[2];//A�����
extern uint8_t  CurrentB[2];//
extern uint8_t  CurrentC[2];//
extern uint8_t  DCVoltage[2];//ֱ�����ѹ
extern uint8_t  DCEnergy [4]; //ֱ�������
extern uint8_t  DCCurrent[2];//ֱ�������
extern uint8_t  InsulationRes[4];//��Ե����
extern uint8_t  Crc485[2];

extern uint8_t  TxFlag485;  //Ϊ1ʱ���ͣ�
extern uint8_t  RxFlag485;  //
extern uint8_t  PMCount485;
extern uint8_t  Tim4Flag;
extern uint8_t  Flag485;//�����Ͳ�ͬ��ָ�
extern uint8_t  Fun485Flag;  // 1:�������  2:�����ѹ  3:�������� 4:����Ƶ��   5:���޹�����   6:�����ڹ���  7:��Ե������  8:ֱ����� 0:����ָ��

extern uint8_t   StartEnergy[4];
extern uint32_t   StartEnergyAll;
extern uint8_t   DealEnergy[4];
extern uint32_t  DealEnergyAll;



//----------------------------------
//������
//----------------------------------
#define MAX_CMD_BUFFER 500
extern uint8_t   cmd_buffer[MAX_CMD_BUFFER];
extern uint16_t  MoneyAverageElectricity;                     //ƽ�����
extern uint16_t  MoneyPeakElectricity;                        //��ֵ���
extern uint16_t  MoneyValleyElectricity;                      //��ֵ���
extern uint16_t  SetTimeHourStartPeakElectricity;             //��ֵ��ۿ�ʼʱ�䣨ʱ��
extern uint16_t  SetTimeMinStartPeakElectricity;              //��ֵ��ۿ�ʼʱ�䣨�֣�
extern uint16_t  SetTimeHourStopPeakElectricity;              //��ֵ��۽���ʱ�䣨ʱ��
extern uint16_t  SetTimeMinStopPeakElectricity;               //��ֵ��۽���ʱ�䣨�֣�
extern uint16_t  SetTimeHourStartValleyElectricity;           //��ֵ��ۿ�ʼʱ�䣨ʱ��
extern uint16_t  SetTimeMinStartValleyElectricity;            //��ֵ��ۿ�ʼʱ�䣨�֣�
extern uint16_t  SetTimeHourStopValleyElectricity;            //��ֵ��۽���ʱ�䣨ʱ��
extern uint16_t  SetTimeMinStopValleyElectricity;             //��ֵ��۽���ʱ�䣨�֣�

/*         һ�㶨��               */
extern  uint8_t FaultOperation;
extern	uint8_t SystemState;      //ϵͳ״̬��0������״̬��1�����״̬ ��2���쳣״̬��Ĭ��Ϊ0����ʼ��Ϊ0���������������л�ϵͳ״̬
extern	uint8_t ChargingMethod ;  //��緽ʽ��0���Զ���磻1���ֶ���� ��Ĭ��Ϊ0����ʼ��Ϊ0����Ļѡ���ֶ�ģʽ��1
extern	uint8_t ChargingMode;     //���ģʽ��0������Ϊֹ��1����ʱ��� ��2.�������䣻3�������䣻Ĭ��Ϊ0����ʼ��Ϊ0����Ļѡ��
extern uint8_t ChargingMode;     //���ģʽ��0������Ϊֹ��1����ʱ��� ��2.�������䣻3�������䣻Ĭ��Ϊ0����ʼ��Ϊ0����Ļѡ��
extern uint16_t SetKwh;                             //���ģʽ���棬���õ���
extern uint16_t SetMoney;                           //���ģʽ���棬���ý��
extern uint16_t SetHour;                            //���ģʽ���棬����ʱ�䣨ʱ��
extern uint16_t SetMin;                             //���ģʽ���棬����ʱ�䣨�֣�
extern uint16_t SetChargeCutOffCurrent;           //���ý�ֹ����
extern uint16_t SetChargeVoltage;                 //���ó���ѹ
extern uint16_t SetChargeCurrent;                 //���ó���?

extern	uint8_t Timer1CountNumber; //ÿ500ms��1��10s��λ����ʱ��1��
extern	uint8_t FlagCycle30s;     //10sѭ����ʱ��־λ��Ĭ��Ϊ0����ʱ��1��λ��ʹ��������
extern	uint8_t FlagCycle10s;     //10sѭ����ʱ��־λ��Ĭ��Ϊ0����ʱ��1��λ��ʹ��������
extern	uint8_t FlagCycle1s;      //1sѭ����ʱ��־λ��Ĭ��Ϊ0����ʱ��1��λ��ʹ��������
extern	uint8_t FlagCycle500ms;   //500msѭ����ʱ��־λ��Ĭ��Ϊ0����ʱ��1��λ��ʹ��������

extern uint16_t SystemFaultState;   //ϵͳ����״̬?
                             //0000 0000 0000 0001 ��ͣ��ť����   //0000 0001 0000 0000   �洢���쳣
							 //0000 0000 0000 0010 ����δ�ر�     //0000 0010 0000 0000   ʱ���쳣
							 //0000 0000 0000 0100 ׮����б       //0000 0100 0000 0000   �������쳣
							 //0000 0000 0000 1000 ׮��ײ��       //0000 1000 0000 0000   �����ȱ��
                             //0000 0000 0001 0000 �����¶ȹ���   //0001 0000 0000 0000   ��Ե�����쳣
                             //0000 0000 0010 0000 �����¶ȹ���   //0010 0000 0000 0000   ��Ե������쳣 
                             //0000 0000 0100 0000 ����ʪ�ȹ���   //0100 0000 0000 0000   ��Դ���¶ȹ���
							 //0000 0000 1000 0000 ����ʪ�ȹ���   //1000 0000 0000 0000   ����쳣
extern	uint8_t PileTiltState;              //���׮׮����б״̬��0��δ��б��1����б��Ĭ��Ϊ0��
extern	uint8_t CabinDoorState;             //����״̬��0���رգ�1���򿪣�Ĭ��Ϊ0��
extern	uint8_t EmergencyStopButtonState;   //��ͣ��ť״̬״̬��0��������1�����£�Ĭ��Ϊ0��
extern	uint8_t PileState;                  //ǹͷ״̬��0��ʾδ���ӣ�1��ʾ���ӣ�Ĭ��Ϊ0
extern  uint8_t LevelState;                 //ˮƽ��״̬��0��ʾ������1��ʾ��������Ĭ��Ϊ0


extern  uint16_t DS18B20Temperature;       //DS18b20�������¶�
extern  uint16_t AM2321Temperature;        //AM2321��ʪ�ȴ������¶�
extern  uint16_t Am2321Humidity;           //AM2321��ʪ�ȴ�����ʪ��

extern  uint8_t SwipingCardFlag;   //ɨ����־��Ĭ��Ϊ0 ������ 1��ʾˢ��
extern  uint8_t SwipingPileFlag;    //ɨǹ��־  Ĭ��Ϊ0 ������ 1��ʾɨǹ
extern  uint8_t SpeechFlag;        //������־��Ĭ��Ϊ0 1��ʾ������Ч
extern uint8_t SpeechFlagMoney ;        //������־��Ĭ��Ϊ0 1��ʾ������Ч
extern uint8_t SpeechFlagDeal ;        //������־��Ĭ��Ϊ0 1��ʾ������Ч
extern uint8_t FlagIfSaveData;       //�����ϴν������� Ĭ��Ϊ0 1��ʾ��Ч
extern uint8_t FlagIfSaveToMemory ;
extern uint8_t FlagIfReadPriDeal;
extern uint8_t FlagIfCloseDoor;       // �رղ��ű�־
extern uint8_t FlagOpenDoor;           //�򿪲���

extern uint16_t ReadPriDealNo ;

extern uint8_t *Recvdata;
extern uint16_t RecvLen;
extern uint32_t RecvIPNumber;





//-----------------------------------------
//ʱ��
//-----------------------------------------
typedef struct 				 //�����ʱ���趨�Ľṹ��
{
	uint8_t Second;	
	uint8_t Minute;
	uint8_t Hour;
	uint8_t Week;
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;      
}RTC_SD2405_HandleTypeDef;


typedef struct 				 //��¼�洢������Ϣ
{
	uint32_t  AcchAddress;       //���׮��
	uint32_t  PosNumber;         //pos����
	uint32_t  CardVer;           //���汾��
	uint32_t  IPAddress;         //IP��ַ
	uint32_t  MACAddressHigh;     //MAC��ַ��16λ ��48λ
	uint32_t  MACAddressLow;      //MAC��ַ��32λ ��48λ
	uint16_t  AreaNumber;        //��������  
	uint16_t  ConsRate;          //��������
	uint16_t  CardType;          //������
	uint16_t  TransType;         //��������
	uint16_t  APrice;			 //ƽ�����
	uint16_t  HPrice;			 //��ֵ���
	uint16_t  LPrice;			 //��ֵ���
	uint16_t  HPriceStartH;		 //��ֵ��ʼʱ��(ʱ)
	uint16_t  HPriceStartM;		 //��ֵ��ʼʱ�䣨�֣�
	uint16_t  HPriceStopH;		 //��ֵ����ʱ�䣨ʱ��
	uint16_t  HPriceStopM;		 //��ֵ����ʱ�䣨�֣�
	uint16_t  LPriceStartH;	 	 //��ֵ��ʼʱ��(ʱ)
	uint16_t  LPriceStartM;		 //��ֵ��ʼʱ�䣨�֣�
	uint16_t  LPriceStopH;		 //��ֵ����ʱ��(ʱ)
	uint16_t  LPriceStopM;		 //��ֵ����ʱ�䣨�֣�
	uint16_t  StartMode;		 //����ģʽ
	uint16_t  BillMode;			 //�Ʒ�ģʽ
	uint32_t  Password;	 		 //����
	uint8_t   CRCHigh;            //CRC
	uint8_t   CRCLow;             //CRC
}SystemParameterConfig_HandleTypeDef;//�洢������

typedef struct 				 //��¼�洢ʵʱ��Ϣ �ݴ���С�洢���У����׽����������������洢��
{
	uint32_t DealNo;       //������ˮ��    
	uint32_t DealCardNo;   //���׿���  ����
	uint32_t ParkFee;      //ͣ����
	uint32_t OriMoney;     //����ǰ��� ����
	uint32_t FinMoney;     //���׺���� ����
	uint32_t DealCounter;  //���׼�����
	uint32_t DealStartEnergy;
	uint16_t DealEnergy;   //���׵���   ����
	uint32_t DealStopEnergy;  
	uint16_t DealPrice;    //���׵��  ����
	uint16_t PileNum;      //���׮��   ��׮��
	uint16_t ChargMode;    //���ģʽ   ��Ϊ�Զ���������ʱ���������   
	uint16_t ChargMethod;    //��緽ʽ �ֶ����Զ�
	uint16_t StartYear;    //������ʼʱ�䣨�꣩ ����
	uint16_t StartMonth;   //������ʼʱ�䣨�£� ����
	uint16_t StartDay;     //������ʼʱ�䣨�գ� ����
	uint16_t StartWeek;    //������ʼʱ�䣨���ڣ� ����
	uint16_t StartHour;    //������ʼʱ�䣨ʱ�� ����
	uint16_t StartMin;     //������ʼʱ�䣨�֣    ����
	uint16_t StartSec;     //������ʼʱ�䣨�룩  ����
	uint16_t EndYear;      //���׽���ʱ�䣨�꣩  ����
	uint16_t EndMonth;     //���׽���ʱ�䣨�£�  ����
	uint16_t EndDay;       //���׽���ʱ�䣨�գ�  ����
	uint16_t EndWeek;      //���׽���ʱ�䣨���ڣ�  ����
	uint16_t EndHour;      //���׽���ʱ�䣨ʱ��   ����
	uint16_t EndMin;       //���׽���ʱ�䣨�֣�   ����
	uint16_t EndSec;       //���׽���ʱ�䣨�룩   ����
  uint8_t  ChargeFlag;   //�����Ƿ�ɹ���־0x00 ���׳ɹ� 0xff���ײ��ɹ�(ͻȻ�ϵ磬���ǹ)
	uint8_t  CRCHigh;      //CRC
	uint8_t  CRCLow;       //CRC
}SaveRealTimeData_HandleTypeDef;//�洢ʵʱ����

typedef struct
{
	uint8_t  IAPState;    //IAP����
	uint32_t IAPStartAddress;    //IAPִ����ʼ��ַ
	uint8_t  CRCHigh;      //CRC
	uint8_t  CRCLow;       //CRC
}IAPSystemParameter_HandleTypeDef;   //IAP�ṹ��

typedef union
{
		uint8_t unbuffer[62];  
    SystemParameterConfig_HandleTypeDef SystemParameterConfigHandle;                                 
                           
} unSystemParameterConfig_HandleTypedef;
typedef union
{
		uint8_t unbuffer[73];  
    SaveRealTimeData_HandleTypeDef SaveRealTimeDataHandle;                                 
                           
} unSaveRealTimeData_HandleTypedef;
	
typedef union
{
	uint8_t unbuffer[7];
	IAPSystemParameter_HandleTypeDef IAPSystemParameter; 
}unIAPSystemParameter_HandleTypeDef;


typedef struct 				 //��¼�洢ʵʱ��Ϣ
{
	uint32_t IPNumber;       //������ˮ��
	uint8_t Data[100];
	uint16_t Length;
}strETHData_Typedef;//�洢ʵʱ����

extern RTC_SD2405_HandleTypeDef RTCSetTime;
extern RTC_SD2405_HandleTypeDef RTCReadTime;
extern RTC_SD2405_HandleTypeDef RTCReadBCDTime;
extern RTC_SD2405_HandleTypeDef StartChargeBCDTime;
extern RTC_SD2405_HandleTypeDef StartChargeTime;
extern RTC_SD2405_HandleTypeDef ChargeTime;
extern unSaveRealTimeData_HandleTypedef  unSaveRealTimeDataHandle ;
extern unSystemParameterConfig_HandleTypedef   unSystemParameterConfigHandle;
extern unIAPSystemParameter_HandleTypeDef  unIAPSystemParameter;



#define FAULTLIGHTSET      1   //���ϵ���
#define FAULTLIGHTRESET    0   //���ϵ���
#define FULLFREELIGHTSET   1   //�������߿��е���
#define FULLFREELIGHTRESET 0   //�������߿��е���

#define MAXAM2321TEMP     900    //��ʪ�ȴ�������������¶�0.1/bit   ƫ����40��   
#define MINAM2321TEMP     200    //��ʪ�ȴ�������������¶�0.1/bit   ƫ����40��
#define MAXAM2321HUMI     950    //��ʪ�ȴ������������ʪ��0.1/bit
#define MINAM2321HUMI     50    //��ʪ�ȴ������������ʪ��0.1/bit
#define DS18B20TEMP      800    //��Դ�ַ��ȿ����¶���0.1/bit   ƫ����40��
#define MAXDS18B20TEMP   1100    //��Դ������¶���ֵ0.1/bit   ƫ����40��


void mDelayCAN10us(uint8_t n);
void HAL_Delay1us(uint16_t n);
void SPI_Delay(uint16_t n);

void AccumulateChargeEnergy(void);
void AccumulateChargeMoney(void);

void SetACRelay(void);//���������Ӵ���
void ResetACRwlay(void);//ֹͣ�����̵���
void SetDCRelay(void);//����ֱ���Ӵ���
void ResetDCRwlay(void);//ֱֹͣ���Ӵ���
void SetBMSRelay(void); //����������Դ
void ResetBMSRwlay(void);//ֹͣ������Դ


void StartFan(void);//��������
void StopFan(void);//ֹͣ����
void OpenDoor(void);//����
void CloseDoor(void);//����
uint8_t CheckPileState(void);//���ǹͷ״̬��ǹͷ����Ϊ����1 ��ǹͷδ���ӷ���0
uint8_t CheckCabinDoorState(void);//������״̬�����Źر�Ϊ1������δ�ر�Ϊ0
uint8_t CheckLevelState(void);  //���ˮƽ��״̬��ˮƽ����������0��ˮƽ�ǲ���������1
uint8_t CheckEmergencyStopButtonState(void);//��⼱ͣ��ť״̬����ͣ��ťδ����Ϊ0����ͣ��ť����Ϊ1
void ShowPileFaultState(uint8_t);//���׮���ϣ������
void ShowPileChargeFullOrFree(uint8_t);//���׮�������߿���
void Enable_IRQ(void);                 //ʹ���ж�
void Disable_IRQ(void);                 //�ر��ж�
void ReadIAPSystemParatemer(void);      //��ȡϵͳ����
void SaveIAPSystemParatemer(void);       //����ϵͳ����

uint8_t SystemSelfingCheck(void);//ϵͳ�Լ�
uint8_t Collect_System_State_Process(void);//�ɼ�ϵͳ״̬�ӳ���
void ReadSystemParameter(void);   //����ʱ��ȡϵͳ����
void ReadDealData(void);      //ˢ��ǰ��ȡ�ϴν�������

void SetACRelay(void) ; //���������Ӵ���
void ResetACRwlay(void); //ֹͣ�����̵���
void SetDCRelay(void); //����ֱ���Ӵ���
void ResetDCRwlay(void);//ֱֹͣ���Ӵ���

void HMI_Event_Process(void);//������Ļ�ӳ���
void ETH_Event_Process(void);//�������������ӳ���
strETHData_Typedef ETH_GetData(void);
void Charge_State_Process(void);//���״̬�����ӳ���
void Free_State_Process(void);//����״̬�����ӳ���
void Error_State_Process(void);//�쳣�����ӳ���

void ETH_Send_HeartbeatMeg(void);  //�ϴ�������

void DataInit(void);//һ�������ʼ��




#endif /* __DEFINE_H */
