/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/lwip_timers.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "app_ethernet.h"
#include "tcp_echoclient.h"
#include "ETH.h"


extern I2C_HandleTypeDef    I2C1Handle;
extern struct netif gnetif;
extern strETHData_Typedef ETHData;
uint8_t ReadBuffer[100];
//CANͨ�ű����¼���״̬��
uint8_t  ubCANEventStatus;
/* CANͨ������֡����  */
/*----------------------------------------------
-----------------���ֽ׶ζ���------------------
-----------------------------------------------*/
//----------------PGN256 CRM-------------
 //��ʶ�������������֮�󣬳�����BMS���ʹ˱��ģ����յ�BMS���͵ı�ʶ����֮ǰ����0x00�����յ�BMS��ʶ����֮����0xAA
uint8_t ubRecognitionResult;	
//������ţ�1/λ��1ƫ���������ݷ�Χ1-100	
uint8_t ubChargerNumber;
//------------PGN512,BRM���Ĳ���--------------------
//BMSͨ��Э��汾�ţ�����׼�涨��ǰ�汾ΪV1.0 ��ʾΪ��byte3 byte2 byte1  0x 00 01 00
uint32_t ulBmsProtocolVersion;
//������ͣ�01H��Ǧ���� 02H��������	03��������﮵�� 04H������﮵�� 05H������﮵�� 06H����Ԫ���ϵ�� 07H���ۺ�������ӵ�� 08H������﮵�� FFH���������
uint8_t ubBatteryType;
//������������ϵͳ�����/A*h,0.1A*h/λ��0.1A*hƫ���� ���ݷ�Χ :0-����1000A.h
uint16_t uwBmsRateCapacity;
//������������ϵͳ��ܵ�ѹ /V ��O.l V/λ ��0 V ƫ���������ݷ� Χ:0  750 V
uint16_t uwBatteryRateVoltage;
//��������������ƣ���׼ ASCII ��
uint8_t ubBatteryManufacturerName[4];
//�������ţ�Ԥ�����ɳ������ж���
uint16_t ulBatteryManufacturerName;
//������������ڣ��� 1985ƫ��λ
uint8_t ubBatteryProduceYear;
//������������ڣ���
uint8_t ubBatteryProduceMonth;
//������������ڣ���
uint8_t ubBatteryProduceDay;
//������������1��ÿλ  ,3���ֽ�
uint16_t ulBatteryChargeCycle;
//������Ȩ��־  0��ʾ����   1��ʾ ������
uint8_t ubPropertyMark;
//Ԥ��
uint8_t ubBMSReserved;
//����ʶ����VehicleIdentificationNumber
uint8_t ubVIN[17];

/*-----------------------------------------------
-------------------�������ý׶α���--------------
-------------------------------------------------*/

//-------------------PGN1536 BCP���Ĳ���-------------
//���嶯����������������ѹ  ���ݷֱ��� :0.01 V/λ�� 0 V ƫ���� E ���ݷ�Χ :0 V~24V;   
uint16_t uwCellMaxAllowChargeVoltage;
//������������	 ���ݷֱ��� :0.lA/λ�� -400 A ƫ����;���ݷ�Χ: -400 A ~O A;
uint16_t uwMaxAllowChargeCurrent;
//�������ر��������	  ���ݷֱ��� :0.1 kW  o h/ λ�� 0 kW. h ƫ����;���ݷ�Χ :0��.....��1 000  kW o h; 
uint16_t uwBatteryTotalEnergy;
//����������ܵ�ѹ   ���ݷֱ��� :0.1 V/ λ�� 0  V ƫ����;���ݷ�Χ :0 V ��......750  V;
uint16_t uwMaxAllowChargeVoltage;
//��������¶�		 ���ݷֱ��� :1 oC/λ��-50 OC ƫ���� F ���ݷ�Χ:һ 50 oC ��.....�� +200 oC;
uint8_t ubMaxAllowTemperature;
//�����������غɵ�״̬	���ݷֱ��� :0.1%/λ�� 0%ƫ����;���ݷ�Χ :0��.....��100% ; 
uint16_t uwBmsSOC;
//�������������ܵ�ѹ
uint16_t uwBmsTotalVoltage;
//---------PGN2048 CML����-------------
//DC��������ѹ
uint16_t uwDCMaxOutputVoltage;
//DC��������ѹ
uint16_t uwDCMinOutputVoltage;
//DC����������
uint16_t uwDCMaxOutputCurrent;
//---------PGN2304 BRO����-------------
//BMS �Ƿ���׼����(< OxOO-) := BMS δ���ó��׼��; (OxAA):=BMS ��ɳ��׼��; (OxFF):=ԪЧ)
uint8_t ubBmsChargeReady;
//------------PGN2560 CRO����
//�����Ƿ���׼���� ((OxOO):=����δ��ɳ��׼��; (OxAA):=������ɳ��׼��; (OxFF):=��Ч)
uint8_t ubChargerOutputReady;

/*------------------------------------------
----------------���׶α���---------------
-------------------------------------*/

//-----------PGN4096 BCL����----------------
//��ѹ����(V)
uint16_t uwVoltageRequirement;
uint8_t uwVoltageRequirementLow;
uint8_t uwVoltageRequirementHigh;
//�������� (A)
int16_t uwCurrentRequirement;
int8_t uwCurrentRequirementLow;
int8_t uwCurrentRequirementHigh;
int8_t uwCurrentRequirementLowbuf;
int8_t uwCurrentRequirementHighbuf;
int16_t uwCurrentRequirementbuf;
uint16_t CURRENT;
//���ģʽ (Ox01:��ѹ���	;Ox02:�������)
uint8_t ubChargeMode;
//-------------PGN4352 BCS��----------------
//����ѹ����ֵ (V)
uint16_t uwChargeVoltageMeasurement;
//����������ֵ (A)
uint16_t uwChargeCurrentMeasurement;
//��ߵ��嶯�����ص�ѹ�������
uint16_t uwHighestCellVoltageAndGroupNumber;
//��ǰ�ɵ�״̬ SOC(%)
uint8_t ubCurrentSOC;
//����ʣ����ʱ�� (min)
uint16_t uwRemainChargeTime;
//---------PGN4608 CCS����-------------
//��ѹ���ֵ (V)
uint32_t uwOutputVoltageValue;
//�������ֵ (A)
uint32_t uwOutputCurrentValue;
//�ۼƳ��ʱ�� (min)
uint16_t uwAccumulateChargeTime;
//--------------PGN4864 BSM--------
//��ߵ��嶯�����ص�ѹ���ڱ��
uint8_t ubMaxCellVoltageNumber;
//��߶��������¶�
uint8_t ubMaxTemperature;
//����¶ȼ�����
uint8_t ubMaxTemperatureNumber;
//��Ͷ��������¶�
uint8_t ubMinTemperature;
//��Ͷ��������¶ȼ�����
uint8_t ubMinTemperatureNumber;
//�쳣״̬��Ϣ	 
//���嶯�����ص�ѹ����/���� ((00): =���� ;{01):= ����;(10):=����)
//�����������غɵ�״̬ soc ����/���� ((00): =���� ;(01):= ����;(10):= ���͡�
//�������س������� ((00):= ���� ;(01):== ����;(10): =������ ״̬��
//���������¶ȹ��� ((00): =����; (01): =:����;(1 0):= ������ ״̬
//�������ؾ�Ե״̬ ((00): =���� ;(01):=������;(10):= ������ ״̬��
//�����������������������״̬ ((00):= ����; (01): =������;(10): =������״̬��
//������� ((00):= ��ֹ ;(01):=����
uint16_t uwStatusAndAbnormal;
//----------------PGN5376 BMV--------------
//���嶯�����ص�ѹ
uint16_t uwCellVoltage[256];

//----------------PGN5632 BMT--------------
//���嶯�������¶�
uint8_t ubCellTemperature[128];

//----------------PGN5888 BSP--------------
//���嶯������Ԥ������
uint8_t ubCellReserved[16];

//--------------PGN6400 BTS----------
//BMS  ��ֹ���ԭ��
uint8_t ubBmsChargeStopReason;
//BMS  ��ֹ������ԭ��
uint16_t uwBmsChargeStopFaultReason;
//BMS  ��ֹ������ԭ��
uint8_t ubBmsChargeStopErrorReason;

//------------PGN6656 CST-------------
//������ֹ���ԭ��
uint8_t ubChargerStopReason;
//�˵����ֹ������ԭ��
uint16_t uwChargerStopFaultReason;
//������ֹ������ԭ��
uint8_t ubChargerStopErrorReason;

//------------PGN7168 BSD--------------
//��ֹ�ɵ�״̬ SOC(%)
uint8_t ubBmsEndOfSOC;
//�������ص�����͵�ѹ (V)
uint16_t uwBmsEndMinCellVoltage;
//�������ص�����ߵ�ѹ (V)
uint16_t uwBmsEndMaxCellVoltage;
//������������¶� CC)
uint8_t ubBmsEndMinTemperature;
//������������¶� CC)
uint8_t ubBmsEndMaxTemperature;

//-----------PGN7424 CSD-----------
//�ۼƳ��ʱ�� (min)
uint16_t uwChargerEndChargeTime;
//������� (kW. h)
uint16_t uwChargerEndChargePower;

//---------PGN7680 BEM------------
//BMS ������ (BEM)
uint8_t ubBmsError[4];

//-----------PGN7936 CEM-----------
//���������� (CEM)
uint8_t ubChargerError[4];

//DCֱ�����ģ��״̬
uint8_t ubDCModuleStatusHighAll;
uint8_t ubDCModuleStatusLowAll;

uint8_t ubDCModuleStatusHigh[4];
uint8_t ubDCModuleStatusLow[4];
// DC��ѹ���
uint16_t uwDCOutputVoltage[4];
//DC�������
uint16_t uwDCOutputCurrent[4];
// BMS�����緽ʽ����ֵ (Ox01:��ѹ���	;Ox02:�������)
uint8_t ubChargeModeBuf;

//DCֱ�����ģ��״̬
uint16_t uwDCModuleStatus;
//DCģ���Ƿ�����
uint8_t ubDCModue;  
//���DC���ģ��ĵ�ַ��1-N��ģ��ֱ�Ϊ0x01 0x02 0x03 ......��
uint8_t ubDCModueAddress;
#define CAN_STARTDCMODUE 0x55
#define CAN_STOPDCMODUE 0xAA


//��־���Ƿ�ʱ5s 0x00�����㣬0xAA����
uint8_t FlagIfTimeout;
//��־��DC��Դģ���Ƿ�ʱ5s 0x00�����㣬0xAA����
uint8_t FlagIfDCTimeout;
//��ʱʱ�����ã�ubTimeNs��ʾ���ٸ�100ms
uint8_t ubTimeN100ms;


//��־���Ƿ��������ڷ��� ��������10ms 0�����㣬1����
uint8_t FlagIfCycleSend10ms;
//��־���Ƿ��������ڷ��� ��������50ms 0�����㣬1����
uint8_t FlagIfCycleSend50ms;
//��־���Ƿ��������ڷ��� ��������250ms 0�����㣬1����
uint8_t FlagIfCycleSend250ms;
//��־���Ƿ��������ڷ��� ��������500ms 0�����㣬1����
uint8_t FlagIfCycleSend500ms;
//��־���Ƿ��������ڷ��� ��������1000ms 0�����㣬1����
uint8_t FlagIfCycleSend1000ms;

//��ʱ��6������10ms����һ��   
uint8_t ubCANTim3Count;
//��ʱ��6������10ms����һ��,CANͨ�ų���ͨ�ų�ʱ������   
uint16_t uwCANTimeOutCount;
//��ʱ��6������10ms����һ��,CANͨ��DC��Դģ�鳬ʱ������   
uint16_t uwCANDCTimeOutCount;


//��־���Ƿ��յ�BMS���͵�BMS�ͳ�����ʶ���� 0x00δ�յ���0xAA�յ�
uint8_t ubFlagIfReceBmsIdentifyMeg;
//��־���Ƿ��յ�BMS���͵ĵ�س��������� 0x00δ�յ���0xAA�յ�
uint8_t ubFlagIfReceBatteryParameterMeg;
//��־���Ƿ��յ�BMS���͵ĵ�س��׼������״̬���� 0δ�յ���1�յ�
uint8_t FlagIfReceBmsReadyMeg;
//��־���Ƿ��յ�BMS���͵ĵ�س����״̬���� 0x00δ�յ���0xAA�յ�
uint8_t ubFlagIfReceBmsAllChargeStateMeg;
//��־���Ƿ��յ�BMS���͵ĵ�س�������� 0x00δ�յ���0xAA�յ�
uint8_t FlagIfReceBmsChargeRequirementMeg;
//��־���Ƿ��յ�BMS���͵Ķ�������״̬ 0x00δ�յ���0xAA�յ�
uint8_t FlagIfReceBmsBatteryStateMeg;
//��־���Ƿ��յ�BMS���ͽ���������BMS��ֹ��籨�� 0x00δ�յ���0xAA�յ�
uint8_t ubFlagIfReceBmsEndAndStopChargeMeg;
//��־���Ƿ��յ�BMS���͵�BMS��ֹ��籨�� 0x00δ�յ���0xAA�յ�
uint8_t FlagIfReceBmsStopChargeMeg;
//��־���Ƿ��յ�BMS���͵ĳ��ͳ�����ݱ��� 0x00δ�յ���0xAA�յ�
uint8_t FlagIfReceBmsStatisticsDataMeg;


//��־���Ƿ��ͱ��ĸ�DCֱ������ģ�� 0x00�����ͣ�0xAA����
uint8_t FlagIfSendDCModuleMeg;
//��־���Ƿ��յ�DCֱ������ģ�鷢�͵ı��� 0û�յ���1�յ�
uint8_t FlagIfReceDCModuleMeg;

//��־���Ƿ��ͳ�����ʶ���� 0x00�����ͣ�0xAA����
uint8_t ubFlagIfSendChargerIdentifyMeg;
//��־���Ƿ��ͳ���ʱ��ͬ����Ϣ���� 0x00�����ͣ�0xAA����
uint8_t ubFlagIfSendChargerTimeMeg;
//��־���Ƿ����������������� 0x00�����ͣ�0xAA����
uint8_t ubFlagIfSendChargerMaxOutputCapactityMeg;
//��־���Ƿ��ͳ����������״̬���� 0x00�����ͣ�0xAA����
uint8_t ubFlagIfSendChargerOutputReadyMeg;
//��־���Ƿ��ͳ������״̬���� 0x00�����ͣ�0xAA����
uint8_t FlagIfSendChargerChargingStateMeg;
//��־���Ƿ������ֹ��籨�� 0x00�����ͣ�0xAA����
uint8_t ubFlagIfSendChargerStopChargeMeg;
//��־���Ƿ��ͳ������ͳ�����ݱ��� 0x00�����ͣ�0xAA����
uint8_t ubFlagIfSendChargerStatisticsDataMeg;



//CAN����BMS���ݻ�����  ͨ���ų����¼������
uint8_t ubCANBuf_BMS[8];
//CAN����BMS���ݻ�������λ��������
uint8_t ubCANBufIndex_BMS;
//CAN�յ�BMS���ݵ�ID
uint32_t uCANBufID_BMS;

//CAN����DC���ݻ�����  ͨ���ų����¼������
uint8_t ubCANBuf_DC[8];
//CAN����DC���ݻ�������λ��������
uint8_t ubCANBufIndex_DC;
//CAN�յ�DC���ݵ�ID
uint32_t uCANBufID_DC;

//��֡���ӱ��Ľ���������
uint8_t ubCANConnect[8];
uint8_t ubCANMultiFrame[70][8];
uint8_t ubCANMultiFrameIndex;

uint16_t uwCANReceFlagState;//�Ƿ��յ�CAN����֡��־λ 0��ʾ��Ч 1��ʾ��Ч  
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
                                 //��12λ DC��Դ���״̬���
uint16_t uwCANSendFlagState;//�Ƿ���CAN����֡��־λ 0��ʾ��Ч 1��ʾ��Ч  
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

uint8_t ubManyBytesStateBCS;//��֡���ݵı�־
uint8_t ubManyBytesStateBMV;//��֡���ݵı�־
uint8_t ubManyBytesStateBMT;//��֡���ݵı�־
uint8_t ubManyBytesStateBSP;//��֡���ݵı�־

//�����ݴ���Э�����ӹ���������
uint8_t ubCANConnect[8];

uint8_t ubCellVoltageQuantity;     //���嶯�����ص�ص�ѹ���� 
uint8_t ubCellTemperatureQuantity; //���������¶� ����
uint8_t ubCellReservedQuantity;    //��������Ԥ������  ����

uint8_t ubCellVoltageMegQuantity;     //���嶯�����ص�ص�ѹ ����֡���� 
uint8_t ubCellTemperatureMegQuantity; //���������¶� ����֡����
uint8_t ubCellReservedMegQuantity;    //��������Ԥ������  ����֡����

//��������������־
uint8_t ubPhysicalConnect;  //0x00��ʾδ���ӣ�0xAA��ʾ��������������������Ч

uint16_t uwReadTimeCount;//CANͨ����С��ʱ��λ�Ƿ���    ���ﶨ��30���ȡһ��ʱ�䣬���ڸ���Ҫ���޸�
uint8_t FlagIfReadTime;//�Ƿ��ȡʱ���־λ
uint8_t FlagIfSendDCMeg;
uint8_t ubDCModue;      //DCģ���Ƿ�����
uint8_t FlagIfShowInformation5S = 0;  //ˢ��������ͣ����־λ
uint8_t FlagShowInformation = 0;

uint32_t Flag=0;

uint8_t ChargeFlag = 0;  //����־ 1:���ڳ�� 0:���ڳ��
uint8_t CANChargeFlag = 0; //���׶α�־
//-----------------------------------------
//������ʹ�õı���
//-----------------------------------------
uint8_t Tim3ICWaitCount;

uint8_t ICDataBfr[16];
uint8_t ICCard_Snr[4];
uint8_t ICKEYA_BUF[6]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t ICKEYB_BUF[6]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

uint8_t ICDataReadBuf[16];
uint8_t ICMoney[4];
uint32_t ulMoney;
uint8_t ICCharge;
uint8_t ICDataWriteBuf[16];
uint8_t ICCard1[4];
uint8_t ICMoney1[4];
uint8_t ICCard2[4];
uint8_t ICMoney2[4];
uint8_t ICReadFlag = 0;
uint8_t ICWriteFlag = 0;
uint8_t FlagIfOperationCard = 0;
uint8_t WriteBlockFlag = 0;//д��ģ���־λ������WriteBlock(1,ICDataReadBuf)����ʱ���˱�־λ��1,д���ɹ�֮��˱�־λ��λ0
uint8_t WriteBlockTimeFlag = 0;//д��ģ�����ڷ��ͱ�־��100ms��λһ�Σ����꼴��0

uint8_t   StartICMoney[4];
uint32_t  StartICMoneyAll;
uint8_t   DealMoney[4];
uint32_t  DealMoneyAll =0;
uint32_t  StopMoneyAll;

RTC_SD2405_HandleTypeDef RTCSetTime;
RTC_SD2405_HandleTypeDef RTCReadTime;
RTC_SD2405_HandleTypeDef RTCReadBCDTime;
RTC_SD2405_HandleTypeDef StartChargeBCDTime;
RTC_SD2405_HandleTypeDef StartChargeTime;
RTC_SD2405_HandleTypeDef ChargeTime;
unSaveRealTimeData_HandleTypedef  unSaveRealTimeDataHandle ;   //�����Ϣ������
unSystemParameterConfig_HandleTypedef   unSystemParameterConfigHandle;   //�洢����������
unIAPSystemParameter_HandleTypeDef  unIAPSystemParameter;     //IAP������

uint8_t Time[8];

//������
uint8_t UART5RxBuff[24];
uint8_t UART5RxBuffCount;
uint8_t UART5RxFlag;

uint8_t	 PMsendBuf[8];
uint8_t  VoltageA[2];
uint8_t  VoltageB[2];
uint8_t  VoltageC[2];
uint8_t  RectivePower[2];//�޹�����
uint8_t  ApparentPower[2];//�����ڹ���
uint8_t  Frequency[2];//Ƶ��
uint8_t  ACEnergy [4]; //�й�����
uint8_t  CurrentA[2];//A�����
uint8_t  CurrentB[2];//
uint8_t  CurrentC[2];//
uint8_t  DCVoltage[2];//ֱ�����ѹ
uint8_t  DCEnergy [4]; //ֱ�������
uint8_t  DCCurrent[2];//ֱ�������
uint8_t  InsulationRes[4];//��Ե����
uint8_t  Crc485[2];

uint8_t  TxFlag485; 
uint8_t  RxFlag485=0;
uint8_t  PMCount485;
uint8_t  Tim4Flag;
uint8_t  Flag485;//�����Ͳ�ͬ��ָ�
uint8_t  Fun485Flag;  // 1:�������  2:�����ѹ  3:�������� 4:����Ƶ��   5:���޹�����   6:�����ڹ���  7:��Ե������  8:ֱ����� 0:����ָ��

uint8_t   StartEnergy[4];
uint32_t   StartEnergyAll;
uint8_t   DealEnergy[4] = {0,0,0,0};
uint32_t  DealEnergyAll;

uint8_t  cmd_buffer[MAX_CMD_BUFFER];



/*         һ�㶨��               */
uint8_t FaultOperation = 0;
uint8_t SystemState;      //ϵͳ״̬��0������״̬��1�����״̬ ��2���쳣״̬��Ĭ��Ϊ0����ʼ��Ϊ0���������������л�ϵͳ״̬
uint8_t ChargingMethod = 0;  //��緽ʽ��0���Զ���磻1���ֶ���� ��Ĭ��Ϊ0����ʼ��Ϊ0����Ļѡ���ֶ�ģʽ��1
uint8_t ChargingMode = 0;     //���ģʽ��0������Ϊֹ��1����ʱ��� ��2.�������䣻3�������䣻Ĭ��Ϊ0����ʼ��Ϊ0����Ļѡ��
uint16_t SetKwh=0;                             //���ģʽ���棬���õ���
uint16_t SetMoney=0;                           //���ģʽ���棬���ý��
uint16_t SetHour=0;                            //���ģʽ���棬����ʱ�䣨ʱ��
uint16_t SetMin=0;                             //���ģʽ���棬����ʱ�䣨�֣�
uint16_t SetChargeCutOffCurrent=0;           //���ý�ֹ����
uint16_t SetChargeVoltage=0;                 //���ó���ѹ
uint16_t SetChargeCurrent=0;                 //���ó���?

uint8_t Timer1CountNumber; //ÿ500ms��1��10s��λ����ʱ��1��
uint8_t FlagCycle30s;     //10sѭ����ʱ��־λ��Ĭ��Ϊ0����ʱ��1��λ��ʹ��������
uint8_t FlagCycle10s;     //10sѭ����ʱ��־λ��Ĭ��Ϊ0����ʱ��1��λ��ʹ��������
uint8_t FlagCycle1s;      //1sѭ����ʱ��־λ��Ĭ��Ϊ0����ʱ��1��λ��ʹ��������
uint8_t FlagCycle500ms;   //500msѭ����ʱ��־λ��Ĭ��Ϊ0����ʱ��1��λ��ʹ��������

uint16_t SystemFaultState;   //ϵͳ����״̬�
                             //0000 0000 0000 0001 ��ͣ��ť����   //0000 0001 0000 0000   �洢���쳣
							 //0000 0000 0000 0010 ����δ�ر�     //0000 0010 0000 0000   ʱ���쳣
							 //0000 0000 0000 0100 ׮����б       //0000 0100 0000 0000   �������쳣
							 //0000 0000 0000 1000 ׮��ײ��       //0000 1000 0000 0000   �����ȱ��
                             //0000 0000 0001 0000 �����¶ȹ���   //0001 0000 0000 0000   ��Ե�����쳣
                             //0000 0000 0010 0000 �����¶ȹ���   //0010 0000 0000 0000   ��Ե������쳣 
                             //0000 0000 0100 0000 ����ʪ�ȹ���   //0100 0000 0000 0000   ��Դ���¶ȹ���
							 //0000 0000 1000 0000 ����ʪ�ȹ���   //1000 0000 0000 0000   ����쳣
uint8_t PileTiltState;              //���׮׮����б״̬��0��δ��б��1����б��Ĭ��Ϊ0��
uint8_t CabinDoorState;             //����״̬��0���رգ�1���򿪣�Ĭ��Ϊ0��
uint8_t EmergencyStopButtonState;   //��ͣ��ť״̬״̬��0��������1�����£�Ĭ��Ϊ0��
uint8_t LevelState;                 //ˮƽ��״̬��0��ʾ������1��ʾ��������Ĭ��Ϊ0�

uint8_t PileState;                  //ǹͷ״̬��0��ʾδ���ӣ�1��ʾ���ӣ�Ĭ��Ϊ0
uint16_t DS18B20Temperature;       //DS18b20�������¶�
uint16_t AM2321Temperature;        //AM2321��ʪ�ȴ������¶�
uint16_t Am2321Humidity;           //AM2321��ʪ�ȴ�����ʪ��

uint8_t SwipingCardFlag =0;   //ˢ����־��Ĭ��Ϊ0 ������ 1��ʾˢ��
uint8_t SwipingPileFlag = 0;
uint8_t SpeechFlag =0;        //������־��Ĭ��Ϊ0 1��ʾ������Ч
uint8_t SpeechFlagMoney =0;        //������־��Ĭ��Ϊ0 1��ʾ������Ч
uint8_t SpeechFlagDeal =0;        //������־��Ĭ��Ϊ0 1��ʾ������Ч
uint8_t FlagIfCloseDoor = 0;       // �رղ��ű�־
uint8_t FlagOpenDoor = 0;           //�򿪲���

uint8_t FlagIfSaveData = 0;       //�����ϴν������� Ĭ��Ϊ0 1��ʾ��Ч
uint8_t FlagIfSaveToMemory = 0;
uint8_t FlagIfReadPriDeal = 0;
uint16_t ReadPriDealNo = 0;
uint8_t  *Recvdata;
uint16_t RecvLen;
uint32_t RecvIPNumber;

void mDelayCAN10us(uint8_t n)
{
	uint8_t i = 200;
	while(n--)
	{
		while(i--);
	}
}

void HAL_Delay1us(uint16_t n)
{
	while(n--)
	{
		uint8_t i = 22;
		while(i--);
	}
}
void SPI_Delay(uint16_t n)
{
     while(n--);
}

void Clr_Data(void)
{
	uint8_t i;
	for(i=0;i<4;i++)
	{
		ICCard1[i] = 0;
		ICMoney[i] = 0;
	}
	CANQueue_Reset();
	uwOutputVoltageValue = 0;
	uwOutputCurrentValue = 0;
	StartChargeTime.Second = 0;
	StartChargeTime.Minute = 0;
	StartChargeTime.Hour = 0;
	StartChargeTime.Day = 0;
	StartChargeTime.Month = 0;
	StartChargeTime.Year = 0;	
	uwRemainChargeTime = 0; //ʣ��ʱ��
	DealMoneyAll = 0;    //�����
	uwBmsSOC = 0;   //BMS ��SOC
	ChargeTime.Hour = 0;  //���ʱ�� Сʱ
	ChargeTime.Minute = 0;    //���ʱ�� ����
	//FaultOperation = 0;       //���������־λ
	DealEnergy[2] = 0;    //���׵��ܸ߰�λ
	DealEnergy[3] = 0;     //���׵��ܵͰ�λ
	ubCANEventStatus = 1;
	FlagIfSaveData = 0;       //�����ϴν������� Ĭ��Ϊ0 1��ʾ��Ч
	UpDataId();
	UpDataBalance();
}


void SetACRelay(void) //���������Ӵ���
{
	HAL_GPIO_WritePin( GPIOC,GPIO_PIN_2, GPIO_PIN_RESET);
}

void ResetACRwlay(void)//ֹͣ�����̵���
{
	HAL_GPIO_WritePin( GPIOC,GPIO_PIN_2, GPIO_PIN_SET);
}

void SetDCRelay(void) //����ֱ���Ӵ���
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_15, GPIO_PIN_SET);
}

void ResetDCRwlay(void)//ֱֹͣ���Ӵ���
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_15, GPIO_PIN_RESET);
}

void SetBMSRelay(void) //����������Դ
{
	HAL_GPIO_WritePin( GPIOF,GPIO_PIN_10, GPIO_PIN_RESET);
}

void ResetBMSRwlay(void)//ֹͣ������Դ
{
	HAL_GPIO_WritePin( GPIOF,GPIO_PIN_10, GPIO_PIN_SET);
}

void StartFan(void)//��������
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_6, GPIO_PIN_SET);
}

void StopFan(void)//ֹͣ����
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_6, GPIO_PIN_RESET);
}

void OpenDoor(void)//����
{
	HAL_GPIO_WritePin( GPIOG,GPIO_PIN_9, GPIO_PIN_SET);
}
void CloseDoor(void)//����
{
	HAL_GPIO_WritePin( GPIOG,GPIO_PIN_9, GPIO_PIN_RESET);
}
uint8_t CheckPileState(void) //���ǹͷ״̬��ǹͷ����Ϊ����1 ��ǹͷδ���ӷ���0
{
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4) == 0)
		return 1;
	else
		return 0;	 
}

uint8_t CheckCabinDoorState(void) //������״̬�����Źر�Ϊ1������δ�ر�Ϊ0
{
	return HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_11);
}

uint8_t CheckLevelState(void)  //���ˮƽ��״̬��ˮƽ����������0��ˮƽ�ǲ���������1
{
	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4);
}
	
uint8_t CheckEmergencyStopButtonState(void)//��⼱ͣ��ť״̬����ͣ��ťδ����Ϊ0����ͣ��ť����Ϊ1
{
	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5);
}

void ReadSystemParameter(void)   //������ȡϵͳ����
{
	HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS+2,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
}

void ReadDealData(void)      //ˢ��ǰ��ȡ�ϴν�������
{
	HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,63,0xff);
}

void AccumulateChargeEnergy(void)
{
	uint32_t Energy;
	Energy = (ACEnergy[0]<<24) + (ACEnergy[1]<<16) + (ACEnergy[2]<<8) + ACEnergy[3] ;
  StartEnergyAll = (StartEnergy[0]<<24) + (StartEnergy[1]<<16) + (StartEnergy[2]<<8) + StartEnergy[3] ;
	if(Energy >= StartEnergyAll)
	{
			DealEnergyAll = Energy - StartEnergyAll;
			DealEnergy[0] =  (DealEnergyAll >>24)&0xff;
			DealEnergy[1] = (DealEnergyAll >>16)&0xff;
			DealEnergy[2] = (DealEnergyAll >>8)&0xff;
			DealEnergy[3] = DealEnergyAll &0xff;
	}
}

void AccumulateChargeMoney(void)
{
	DealMoneyAll = (DealEnergyAll * unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice) / 100;
  DealMoney[0] = DealMoneyAll /256/256/256;
	DealMoney[1] = (DealMoneyAll/256/256)%256;
	DealMoney[2] = (DealMoneyAll /256)%256;
	DealMoney[3] = DealMoneyAll %256;
}
void DataInit(void)//һ�������ʼ��
{
		uint8_t i;
		SystemState = 1;      //ϵͳ״̬��0������״̬��1�����״̬ ��2���쳣״̬��Ĭ��Ϊ0����ʼ��Ϊ0���������������л�ϵͳ״̬
    ChargingMethod  = 0;  //��緽ʽ��0���Զ���磻1���ֶ���� ��Ĭ��Ϊ0����ʼ��Ϊ0����Ļѡ���ֶ�ģʽ��1
    ChargingMode = 0;     //���ģʽ��0������Ϊֹ��1����ʱ��� ��2.�������䣻3�������䣻Ĭ��Ϊ0����ʼ��Ϊ0����Ļѡ��
    Timer1CountNumber = 0; //ÿ500ms��1��10s��λ����ʱ��1��
    FlagCycle10s = 0;     //10sѭ����ʱ��־λ��Ĭ��Ϊ0����ʱ��1��λ��ʹ��������
    FlagCycle1s = 0;      //1sѭ����ʱ��־λ��Ĭ��Ϊ0����ʱ��1��λ��ʹ��������
    FlagCycle500ms = 0;   //500msѭ����ʱ��־λ��Ĭ��Ϊ0����ʱ��1��λ��ʹ��������

    PileTiltState = 0;              //���׮׮����б״̬��0��δ��б��1����б��Ĭ��Ϊ0��
    CabinDoorState = 0;        //��Դ�ֲ���״̬��0���رգ�1���򿪣�Ĭ��Ϊ0��
    EmergencyStopButtonState = 0;   //��ͣ��ť״̬״̬��0��������1�����£�Ĭ��Ϊ0��
    PileState = 0;                  //ǹͷ״̬��0��ʾδ���ӣ�1��ʾ����     
    SystemFaultState = 0;	
		SwipingCardFlag = 0;
		SpeechFlag = 0;
	  ETHData.IPNumber = 0x0000;
	  ETHData.Length = 0;
	  FlagIfSaveData = 0;       //�����ϴν������� Ĭ��Ϊ0 1��ʾ��Ч
	  for(i=0;i<100;i++)
			{
					ETHData.Data[i] = 0;	
			}
}

void ShowPileFaultState(uint8_t w)//���׮���ϣ������
{
	if(w==1)
	{
		HAL_GPIO_WritePin( GPIOB,GPIO_PIN_1, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin( GPIOB,GPIO_PIN_1, GPIO_PIN_SET);
	}
}

void ShowPileChargeFullOrFree(uint8_t w)//���׮�������߿��� 0����1��
{
	if(w==1)
	{
		HAL_GPIO_WritePin( GPIOF,GPIO_PIN_14, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin( GPIOF,GPIO_PIN_14, GPIO_PIN_SET);
	}
}

void Enable_IRQ(void)
{
	 //	__enable_irq();
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
		HAL_NVIC_EnableIRQ(UART4_IRQn);
		HAL_NVIC_EnableIRQ(UART5_IRQn);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
		HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);	
		HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
		HAL_NVIC_EnableIRQ(TIM4_IRQn);
		HAL_NVIC_EnableIRQ(TIM5_IRQn);
}
	
void Disable_IRQ(void)
{
	  //	__disable_irq();
		HAL_NVIC_DisableIRQ(USART1_IRQn);
		HAL_NVIC_DisableIRQ(USART3_IRQn);
		HAL_NVIC_DisableIRQ(UART4_IRQn);
		HAL_NVIC_DisableIRQ(UART5_IRQn);
		HAL_NVIC_DisableIRQ(USART6_IRQn);
		HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);	
		HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
		HAL_NVIC_DisableIRQ(TIM2_IRQn);
		HAL_NVIC_DisableIRQ(TIM3_IRQn);
		HAL_NVIC_DisableIRQ(TIM4_IRQn);
		HAL_NVIC_DisableIRQ(TIM5_IRQn);
}

void ReadIAPSystemParatemer(void)
{
	//ֱ�Ӷ�������CRC
	
	
}

void SaveIAPSystemParatemer(void)
{
	  //��д��洢���ٶ�������У��CRC
	
	
}

uint8_t SystemSelfingCheck(void)//ϵͳ�Լ�
{
	uint8_t buf1 = 0xAA;
	uint8_t buf2 = 0;
	uint8_t i = 0;
	SystemState = 0;      //ϵͳ״̬��0������״̬��1�����״̬ ��2���쳣״̬��Ĭ��Ϊ0����ʼ��Ϊ0���������������л�ϵͳ״̬
	Speech("[v11]��ӭʹ��׿Խֱ�����׮");
	for(i=0;i<10;i++)
	{
		if(HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0xff,1,&buf2,1,0xff) == HAL_OK)
		{
			break;
		}
	}
	buf1 = buf2 + 1;
	for(i=0;i<10;i++)
	{
		if(HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0xff,1,&buf1,1,0xff) == HAL_OK)
		{
			break;
		}
  }
	for(i=0;i<10;i++)
	{
		if(HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0xff,1,&buf2,1,0xff) == HAL_OK)
		{
			break;
		}
	}
	if(buf1 != buf2)
	{
		SystemFaultState = SystemFaultState|0x0100;//�洢���쳣
	}
	CabinDoorState = CheckCabinDoorState( );
	if(1== CabinDoorState )
	{
		SystemFaultState = SystemFaultState|0x0002;//����δ�ر�
	}
	EmergencyStopButtonState = CheckEmergencyStopButtonState( );
	if(1 == EmergencyStopButtonState)
	{
		SystemFaultState = SystemFaultState|0x0001;//��ͣ��ť�������
	}
	LevelState = CheckLevelState( );
	if(1 == LevelState)
	{
		SystemFaultState = SystemFaultState|0x0008;//ϵͳ��ײ���
	}
	DS18B20Temperature = DS18B20_Get_Temp();
	if(MAXDS18B20TEMP < DS18B20Temperature)
	{
		SystemFaultState = SystemFaultState|0x4000;//��Դ���¶ȹ���
	}
	for(i=0;i<5;i++)
	{
		if(	Read_Sensor() == 0x01)
		{
			break;
		}
		HAL_Delay(1000);
  }
	if(MAXAM2321TEMP < AM2321Temperature)
	{
		SystemFaultState = SystemFaultState|0x0010;//�����¶ȹ���
	}
	if(MINAM2321TEMP > AM2321Temperature)
	{
		SystemFaultState = SystemFaultState|0x0020;//�����¶ȹ���
	}
	if(MAXAM2321HUMI < Am2321Humidity)
	{
		SystemFaultState = SystemFaultState|0x0040;//����ʪ�ȹ���
	}
	if(MINAM2321HUMI > Am2321Humidity)
	{
		SystemFaultState = SystemFaultState|0x0080;//����ʪ�ȹ���
	}
	//Read_Time( &RTCReadBCDTime);
	//UpdateRTC();
// 	Send_485(0x10,0x00);
// 	while()
//  Receive_485(0x10,0x00);
//  ��Ե�����⣬�ȴ����ԣ�timeout wait
// 	if(SystemFaultState != 0)
// 	{
// 		SystemState = 2;      //ϵͳ״̬��0������״̬��1�����״̬ ��2���쳣״̬��Ĭ��Ϊ0����ʼ��Ϊ0���������������л�ϵͳ״̬
// 		Speech("[v11]ϵͳ�쳣");
// 		}
	SystemState = 1;
	ChangePage(0x0001);  //ϵͳ�Լ��������Ļ����������
	CheckOut();
	return 0;
}
uint8_t Collect_System_State_Process(void)//�ɼ�ϵͳ״̬�ӳ���
{
	if(1 == FlagCycle500ms)
	{
		//�ɼ���Ե����������ȱ����
		//�жϾ�Ե�����Լ�������Ƿ�ϸ񣬲ɼ���RS485�н��У�500ms�ɼ�һ��
		FlagCycle500ms = 0;
	}
	//�ж�DC��Դ״̬,����DC��Դ��ʼ������־λ
	if(1 == FlagCycle1s)//1S���һ�� ���ż�ͣ��ײ��
	{
		CabinDoorState = CheckCabinDoorState( );
		if(1== CabinDoorState )
		{
			SystemFaultState = SystemFaultState|0x0002;//����δ�ر�
		}
		EmergencyStopButtonState = CheckEmergencyStopButtonState( );
		if(1 == EmergencyStopButtonState)
		{
			SystemFaultState = SystemFaultState|0x0001;//��ͣ��ť������
		}
		LevelState = CheckLevelState( );
		if(1 == LevelState)
		{
			SystemFaultState = SystemFaultState|0x0008;//ϵͳ��ײ��
		}
		FlagCycle1s = 0;
	}
	if(1 == FlagCycle10s)   //10S���һ��  ��Դ���¶� ������ʪ�� ��������� ����ʱ����
	{
		DS18B20Temperature = DS18B20_Get_Temp();
		if(MAXDS18B20TEMP < DS18B20Temperature)
		{
			SystemFaultState = SystemFaultState|0x4000;//��Դ���¶ȹ���
		}
		Read_Sensor( );
		if(MAXAM2321TEMP < AM2321Temperature)
		{
			SystemFaultState = SystemFaultState|0x0010;//�����¶ȹ���
		}
		if(MINAM2321TEMP > AM2321Temperature)
		{
			SystemFaultState = SystemFaultState|0x0020;//�����¶ȹ���
		}
		if(MAXAM2321HUMI < Am2321Humidity)
		{
			SystemFaultState = SystemFaultState|0x0040;//����ʪ�ȹ���
		}
		if(MINAM2321HUMI > Am2321Humidity)
		{
			SystemFaultState = SystemFaultState|0x0080;//����ʪ�ȹ���
		}
		ETH_Send_HeartbeatMeg();
		FlagCycle10s = 0;
		//Speech("�������ϴ�");
		//��������,FM�洢����������?������Щ����
	}
	return 0;
}
void HMI_Event_Process(void)//������Ļ�ӳ���
{
		TimeCountDownFlag=(uint16_t)((mainselect==1)||(mainselect==4)||(mainselect==3)||(mainselect==6)||(cardstartflag1==1)||(InsertPileFlag==1));	
		if(TimeCountDownFlag)                                   //����ʱ
	      Count_Down();
			
		size=queue_find_cmd(cmd_buffer,MAX_CMD_BUFFER);		      //�������ݴ���
	  if(size>0)	
	      Message_process();
	  
}
void ETH_Event_Process(void)//�������������ӳ���
{ 
    IfConnect();
	  ethernetif_input(&gnetif);//�����ݰ���������ֱ���˳�
	  ETHData = ETH_GetData();
    AnalyzeData();
}  
void ETH_Send_HeartbeatMeg(void)    //�ϴ�������
{
	uint8_t sendbuffer[15];
	sendbuffer[0]=0xAA;
	sendbuffer[1]=0xAA;
	sendbuffer[2]=0x01;              //�ӻ���ַ        
	sendbuffer[3]=0x00;             //׮��
	sendbuffer[4]=0x00;      
	sendbuffer[5]=0x00;      
	sendbuffer[6]=0x01;      
	sendbuffer[7]=RTCReadBCDTime.Year;
	sendbuffer[8]=RTCReadBCDTime.Month;         
	sendbuffer[9]=RTCReadBCDTime.Day;         
	//sendbuffer[7]=RTCReadBCDTime.Week;                 
	sendbuffer[10]=(RTCReadBCDTime.Hour)&0x7f;         
	sendbuffer[11]=RTCReadBCDTime.Minute;        
	sendbuffer[12]=RTCReadBCDTime.Second;        
	sendbuffer[13]=0x55;     
	sendbuffer[14]=0x55;             
	TCP_Server_SendData(sendbuffer,15);
}


strETHData_Typedef ETH_GetData(void)
{
	strETHData_Typedef ethdata;
	uint16_t i;
	for(i=0;i<RecvLen;i++)
    {
		 	ethdata.Data[i]=*(Recvdata+i);
    }
	ethdata.Length=RecvLen;
	ethdata.IPNumber=RecvIPNumber;
	for(i=0;i<RecvLen;i++)
    {
		 	*(Recvdata+i) = 0;
    }
	RecvLen = 0;
	RecvIPNumber = 0;	
	return ethdata;
}

void Free_State_Process(void)//����״̬�����ӳ���
{
		PileState = CheckPileState();//�ж�ǹ�Ƿ񱻰ε�
		if(PileState)
		{
			CAN_mEvent();       //CAN�¼�����
		}
		else
		{
			//ǹ���ε�������������
			//give flag 
		}
		//ֱ���ڽ�����簴ť���ֽ��д˹���
		if(1 == SpeechFlag)//��Ҫ����,������ˢ��,ֻ˵һ��
		{
			Speech("[v11]������ˢ��");
			SpeechFlag = 0;
		}
		if(1 == SwipingCardFlag)//������簴ť������,ҳ�泬ʱ����˱�־λ���
		{
					if(1 == CardLock())//��������ɹ�
					{
							//�ж��Ƿ�Ϊ��ǰ��翨
							if(1)//
							{
									//CANͨ�Ž��������紦���֣�ubCANEventStatus = ��
								//�ر���ؼ̵���
								ResetACRwlay( );//ֹͣ�����̵���
								ResetDCRwlay( );//ֱֹͣ���Ӵ���
								StopFan( );//ֹͣ����
								ShowPileChargeFullOrFree(0);//���׮�������߿���
								//��ؼ�¼���̣�	
								//���緢���������
								//��ʾ����ʾ��ǰ������Ϣ give flag to HMI
							}
							else
							{	
							//������ر�־λ   �����ɹ���־λ  ֪ͨ��Ļ������ʾ���źͿ������
							SwipingCardFlag = 0;
							SystemState = 1;//������״̬
							}
					}			
		}
}

void Charge_State_Process(void)//���״̬�����ӳ���
{
	uint8_t i;
	uint8_t FlagICStatus = 0 ;
  /************************
  1.��������ģʽ��¼ 
	2.���ݲ�ǹ�ж��Ƿ������ʾ��ǹҳ��
	3.ˢ������
	4.�����Ϣ����
******************************/
    //����
    uint32_t Energy;
    uint16_t DealEnergyLow;
	uint16_t crc=0xffff;
	//����
	PileState = CheckPileState();
	//��ʾ��ǹ�����Ը���SpeechFlagֻ˵һ��
	if((1 == SwipingPileFlag)&&(1 == SpeechFlag))
	{
		Speech("����������");
		SpeechFlag = 0;
		OpenDoor();
    FlagOpenDoor = 1;           //�򿪲���
	}
  	//ɨǹͷ
	if(1 == SwipingPileFlag)
	{	
			if(1==PileState)
				{
					ChangePage(0x001A);   //ҳ���л���ˢ������
					SwipingCardFlag = 1; //����ɨ������
					SwipingPileFlag =0;    
					SpeechFlag = 1;			
					cardstartflag1=1;   
				}
	}
	//��ʾˢ�������Ը���SpeechFlagֻ˵һ��
	if((1 == SwipingCardFlag)&&(1 == SpeechFlag))
	{
		  Speech("��ˢ�����");
		  SpeechFlag = 0;
		  ICReadFlag = 0;
			ICWriteFlag = 0;
		  SetBMSRelay();//����������Դ
		  //���ϴν���
		  HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSaveRealTimeDataHandle.unbuffer,73,0xff);
	}
	//ɨ��
	if(1 == SwipingCardFlag)
	{   
			if(0 == ICWriteFlag)
			{
					FlagICStatus = CardLock();	
			}		
	    if((1 == FlagICStatus)&&(1 == SpeechFlagDeal))       //�ϴν���δ���
			{
					SpeechFlagDeal = 0;
					Speech("�ϴν���δ���");
					UpDataId();     //�ϴ���������Ļ
					UpDataBalance();    //�ϴ��������Ļ
					TimeCountDownStartCard = 5;
					ChargeFlag = 0; 
			}
			else if((3 ==FlagICStatus)&&(1 == SpeechFlagMoney))           //��������
			{
					SpeechFlagMoney = 0;
					Speech("������������ֵ");
				  UpDataId();     //�ϴ���������Ļ
		      UpDataBalance();    //�ϴ��������Ļ
				  TimeCountDownStartCard = 5;
					ChargeFlag = 0; 
			}
			else if((0 ==FlagICStatus))//&&(1 == SpeechFlag))           //û�ж�������
			{
				   //Speech("ˢ�����ɹ�");
			}
			//else if((2 == FlagICStatus)&&(1 == ICWriteFlag))//���д���ɹ�  //��ȷ����
			if(ICWriteFlag == 1)
				{
						Speech("��ʼ���");
					  UpDataId();     //�ϴ���������Ļ
		        UpDataBalance();    //�ϴ��������Ļ
					  HAL_Delay(100);
				    WriteBlockFlag = 0;
					  ICWriteFlag = 0;
						ubDCModue = CAN_STARTDCMODUE;
						if(1 == FlagIfSaveToMemory)  //���ϴ�δ��ɽ���д���洢��
						{
								FlagIfSaveToMemory = 0;
								crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
							  WriteHisRec(unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo,unSaveRealTimeDataHandle.unbuffer,73);				
						}
						//������ʼ���ʱ��
						StartChargeBCDTime.Second = RTCReadBCDTime.Second;    //��
						StartChargeBCDTime.Minute = RTCReadBCDTime.Minute;   //��
						StartChargeBCDTime.Hour = RTCReadBCDTime.Hour;       //ʱ
						StartChargeBCDTime.Day = RTCReadBCDTime.Day;         //��
						StartChargeBCDTime.Month = RTCReadBCDTime.Month;     //��
						StartChargeBCDTime.Year = RTCReadBCDTime.Year;       //��

						//��ʼ���׶��豣���ʵʱ���� 
						HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0xf0,I2C_MEMADD_SIZE_8BIT,StartEnergy,4,0xff);   //��ȡ�洢������
						Energy = (ACEnergy[0]<<24) + (ACEnergy[1]<<16) + (ACEnergy[2]<<8) + ACEnergy[3] ;  //��������ǰʱ�̵���
						StartEnergyAll = (StartEnergy[0]<<24) + (StartEnergy[1]<<16) + (StartEnergy[2]<<8) + StartEnergy[3] ;  //�洢���洢��һ�ν��׽���ʱ�ĵ���
						if(1 == FlagIfSaveData)   //���⵼���ϴν���δ���
						{
							 FlagIfSaveData = 0;
							
							 StartEnergy[0] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy>>24)&0xff;
							 StartEnergy[1] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy>>16)&0xff;
							 StartEnergy[2] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy>>8)&0xff;
							 StartEnergy[3] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy)&0xff;
						}
						else              //�µ�һ�ν��׼�¼
						{   
							  StartICMoneyAll = (StartICMoney[0]<<24) + (StartICMoney[1]<<16) + (StartICMoney[2]<<8) + StartICMoney[3] ;
							  unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo = 1 + unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo; 
							  unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee = 0;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter = 0;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.PileNum = 0;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMode = ChargingMode ;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod = ChargingMethod;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo = (ICCard1[3]<<24)+(ICCard1[2]<<16)+(ICCard1[1]<<8)+ICCard1[0];     //��ǰ���׿���
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney = StartICMoneyAll;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartYear = RTCReadBCDTime.Year;                                               //��ǰ������ʼ���ʱ�� ��
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMonth = RTCReadBCDTime.Month;                                             //��ǰ������ʼ���ʱ��  ��
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartDay = RTCReadBCDTime.Day;                                                  //��ǰ������ʼ���ʱ��  ��
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartWeek = RTCReadBCDTime.Week;                                               //��ǰ������ʼ���ʱ��  ����
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartHour = RTCReadBCDTime.Hour;                                                //��ǰ������ʼ���ʱ��  ʱ
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMin = RTCReadBCDTime.Minute;                                                //��ǰ������ʼ���ʱ��  ��
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartSec = RTCReadBCDTime.Second;                                                //��ǰ������ʼ���ʱ��  ��
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargeFlag = 0xff; 
								if(Energy >= StartEnergyAll)    //��ǰ�������ܴ��ڵ��ڴ洢������ �ѵ�ǰ������Ϊ��ʼ����
									{
											 for(i=0;i<4;i++)
											{
												StartEnergy[i] = ACEnergy[i];  //�ʼ����
												unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy = Energy;
											}
									}
									else            //��ǰ�����ĵ��ܴ���
									{
											for(i=0;i<4;i++)   //�洢��������Ϊ��ʼ����
												{
													 unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy = StartEnergyAll;
												}
									}
						}
						UpDataId();     //�ϴ���������Ļ
						UpDataBalance();    //�ϴ��������Ļ
						ChargeFlag = 1;    //���ڳ���־
						SwipingCardFlag = 0;                      
						SetACRelay(); //���������Ӵ���
						SetDCRelay(); //����ֱ���Ӵ���
					  ShowPileChargeFullOrFree(0);  //�̵Ʋ���
						HAL_Delay(500);
						CAN_Config();
						//SetDCRelay(); //����ֱ���Ӵ���
					  //ShowPileChargeFullOrFree(0);  //�̵Ʋ���
						//SetACRelay(); //���������Ӵ���
						TimeCountDownStartCard = 5;
						if(1 == ChargingMethod)    //�ֶ����
						{
								//SetChargeCutOffCurrent  ;           //���ý�ֹ����
								//SetChargeVoltage;                 //���ó���ѹ
							  //SetChargeCurrent;                 //���ó���?	
						}
				}	
		}
		if(1 == ChargeFlag)
		{
					//if(DS18B20Temperature >=800)
			    if(1)
					{
							StartFan();//��������
					}
// 					else
// 					{
// 							StopFan();//��������
// 					}
					if(1 ==CheckPileState())
						{   		
										AccumulateChargeEnergy();    //�������
										AccumulateChargeMoney();     //������
										CAN_mAccumulateChargeTime();   //������ʱ��
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney = StopMoneyAll ;            //���׺����
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealPrice = unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice; //���׵��
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy = (DealEnergy[2]<<8) + DealEnergy[3];        //���׽���ʱ�䣨�룩
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear = RTCReadBCDTime.Year;      //���׽���ʱ�䣨�꣩
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth = RTCReadBCDTime.Month;     //���׽���ʱ�䣨�£�
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay = RTCReadBCDTime.Day;         //���׽���ʱ�䣨�գ�
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek = RTCReadBCDTime.Week;         //���׽���ʱ�䣨���ڣ�
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour = RTCReadBCDTime.Hour;        //���׽���ʱ�䣨ʱ��
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin = RTCReadBCDTime.Minute;       //���׽���ʱ�䣨�֣�
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec = RTCReadBCDTime.Second;        //���׽���ʱ�䣨�룩
										crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
										if(FlagCycle30s == 1)  //30s�洢����ʵʱ�洢
										{
											HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSaveRealTimeDataHandle.unbuffer,73,0xff);
											//HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x63,I2C_MEMADD_SIZE_8BIT,"ff",1,0xff);
											//ETH_Send_HeartbeatMeg();
											FlagCycle30s = 0;
										}					
									    //����������п�������
										if(DealMoneyAll >= (StartICMoneyAll + (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice/100)))
										{
											ResetACRwlay();//ֹͣ�����̵���
											ResetDCRwlay();//ֱֹͣ���Ӵ��� 
                      ShowPileChargeFullOrFree(1);  //�̵���											
										}
										if(0 == ChargingMethod)     //��緽ʽ 0Ϊ�Զ���磬1Ϊ�ֶ����
										{	
													CAN_mEvent();       //CAN�¼�����
													if(0 ==ChargingMode)   //�Զ�����
														{
															
														}
													else  
														if(1 ==ChargingMode)    //��������
														{
																//�����ܴﵽ�趨���� SetKwh<=DealEnergy
																DealEnergyLow = (DealEnergy[2]<<8) + DealEnergy[3];
																if(SetKwh <= DealEnergyLow)   //�趨����С�ڳ����� �رռ̵���
																{
																	 ubDCModue = CAN_STOPDCMODUE;
																	 CAN_mSendDCModuleStartOrStop(0x00);
																	 ResetACRwlay();//ֹͣ�����̵���
																	 ResetDCRwlay();//ֱֹͣ���Ӵ���
																	 ShowPileChargeFullOrFree(1);  //�̵���
																	 //StopChargeFlag = 1;   //ֹͣ����־λ
																}
																else
																{
															
																}
														}
													else  
														if(2 ==ChargingMode)    //������
														{
																//�趨���ﵽ�ѳ����SetMoney<=DealMoneyAll
																if(SetMoney <= (DealMoneyAll&0xffff))  //�Ѵﵽ�����  �رռ̵���ֹͣ���
																{
																	 ubDCModue = CAN_STOPDCMODUE;
																	 CAN_mSendDCModuleStartOrStop(0x00);
																	 ResetACRwlay();//ֹͣ�����̵���
																	 ResetDCRwlay();//ֱֹͣ���Ӵ���
																	 ShowPileChargeFullOrFree(1);  //�̵���
																	 //StopChargeFlag = 1;   //ֹͣ����־λ
																}
																else    
																{
																
																}
														}
													else  
														if(3 ==ChargingMode)     //��ʱ���
														{
															if((SetHour <= ChargeTime.Hour)&&(SetMin <= ChargeTime.Minute))  //���ʱ�������趨ֵ
															{
															   ubDCModue = CAN_STOPDCMODUE;
															   CAN_mSendDCModuleStartOrStop(0x00);
															   ResetACRwlay();//ֹͣ�����̵���
															   ResetDCRwlay();//ֱֹͣ���Ӵ���
															   ShowPileChargeFullOrFree(1);  //�̵���
															   //StopChargeFlag = 1;   //ֹͣ����־λ
															}
															else
															{
																
															}
														}
										 }	
										 else	
										 if(1 == ChargingMethod)        //�ֶ���磬����ѹ��������ֹ����
										 {
													//SetChargeCutOffCurrent;           //���ý�ֹ����
													//SetChargeVoltage;                 //���ó���ѹ
													//SetChargeCurrent;                 //���ó���?
													if((10*SetChargeCutOffCurrent) >= uwOutputCurrentValue)        //�жϳ������ͽ�ֹ�����Ĵ�С��ϵ
											    {
																 ubDCModue = CAN_STOPDCMODUE;
															   CAN_mSendDCModuleStartOrStop(0x00);
													}
										 } 
												
						}
						else    //��ֹ��ǹ
						{
										StopChargeFlag = 1;   //ֹͣ����־λ
										ResetACRwlay();//ֹͣ�����̵���
										ResetDCRwlay();//ֱֹͣ���Ӵ���
										ResetBMSRwlay();//ֹͣ������Դ
							 		  ShowPileChargeFullOrFree(1);  //�̵���
										FaultOperation = 1;  //���������־λ
						}
		}
		if((1 == StopChargeFlag)&&(1==SpeechFlag)&&(1 == ChargeFlag))   //ˢ���������
		{
						SpeechFlag = 0;
						ICReadFlag = 0;
						Speech("ˢ���������");
						ICWriteFlag = 0;
						//FlagIfOperationCard = 1;
		}
		if(((1 == StopChargeFlag)&&(1 == ChargeFlag))||(1 == FaultOperation))   //�������
		{
					//if((1 == CardRead())||(1 == FaultOperation))
			    if(0 == ICWriteFlag)
					{
							FlagICStatus = CardunLock();	
					}
					//if((2 == FlagICStatus)||(1 == FaultOperation))    //ˢ���ɹ� ���������ֱ�Ӳ���Ҫˢ���������
					if((1== ICWriteFlag)||(1 == FaultOperation))    //ˢ���ɹ� ���������ֱ�Ӳ���Ҫˢ���������
					{
									FlagShowInformation = 1;
									ubDCModue = CAN_STOPDCMODUE;
					      	CAN_mSendDCModuleStartOrStop(0x00);
									HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0xf0,I2C_MEMADD_SIZE_8BIT,ACEnergy,4,0xff);  //����������д��洢�� 
									Speech("���ν�����ɣ�ллʹ��");
						      StopFan();//��������
									//DataInit(); 
									ChargeFlag = 0;
									StopChargeFlag = 0;
									ChargingMode = 0;
									ChargingMethod = 0;
									ResetACRwlay();//ֹͣ�����̵���
									ResetDCRwlay();//ֱֹͣ���Ӵ���
						      ShowPileChargeFullOrFree(1);  //�̵���
					      	ubCANEventStatus = 16;
									CAN_mSend_CST_ChargerStop();
									//ResetBMSRwlay();//ֹͣ������Դ
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney = StopMoneyAll ;            //���׺����
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealPrice = unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice; //���׵��
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy = (DealEnergy[2]<<8) + DealEnergy[3];        //���׽���ʱ�䣨�룩
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStopEnergy = (ACEnergy[0]<<24) + (ACEnergy[1]<<16) + (ACEnergy[2]<<8) + ACEnergy[3];
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear = RTCReadBCDTime.Year;      //���׽���ʱ�䣨�꣩
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth = RTCReadBCDTime.Month;     //���׽���ʱ�䣨�£�
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay = RTCReadBCDTime.Day;         //���׽���ʱ�䣨�գ�
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek = RTCReadBCDTime.Week;         //���׽���ʱ�䣨���ڣ�
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour = RTCReadBCDTime.Hour;        //���׽���ʱ�䣨ʱ��
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin = RTCReadBCDTime.Minute;       //���׽���ʱ�䣨�֣�
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec = RTCReadBCDTime.Second;        //���׽���ʱ�䣨�룩
									if(1 == ICWriteFlag)     //��ȷ����0x00
									{
										  ICWriteFlag = 0;
										  unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargeFlag = 0x00; 
											//HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x63,I2C_MEMADD_SIZE_8BIT,0x00,1,0xff);	
                      //д����洢��
											crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
										  HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSaveRealTimeDataHandle.unbuffer,73,0xff);
                      WriteHisRec(unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo,unSaveRealTimeDataHandle.unbuffer,73);										
									}										
									else if(1 == FaultOperation)   //�������0xff
									{
											FaultOperation = 0;
										  //HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x63,I2C_MEMADD_SIZE_8BIT,"ff",1,0xff);
									//}
											crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
											//�������ʱд�������洢��
											HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSaveRealTimeDataHandle.unbuffer,73,0xff);
									}
					}
			else
					{
				
				
					}
		}
		if(1 == FlagShowInformation)
		{
				if(FlagIfShowInformation5S >= 5)
				{
						FlagShowInformation = 0;
						FlagIfShowInformation5S = 0;
						ResetBMSRwlay();//ֹͣ������Դ
						Clr_Data();   //�������������� ��Ҫ����Ļ����ʾ������
						CAN_Config();  //CAN ��ʼ��
						CAN_mInitData();   //CAN���ݳ�ʼ��
						ChangePage(0x0001);  //��Ļ����������
				}
		}
}
void Error_State_Process(void)//�쳣�����ӳ���
{
	ShowPileFaultState(1);//�쳣����
	//��Ļ��ʾϵͳ�쳣flag
	//����ͨ�ţ�֪ͨ��������ϵͳ�쳣
}
