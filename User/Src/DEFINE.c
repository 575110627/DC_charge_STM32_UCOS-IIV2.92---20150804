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
//CANÍ¨ĞÅ±¨ÎÄÊÂ¼şµÄ×´Ì¬»ú
uint8_t  ubCANEventStatus;
/* CANÍ¨ĞÅÊı¾İÖ¡¶¨Òå  */
/*----------------------------------------------
-----------------ÎÕÊÖ½×¶Î¶¨Òå------------------
-----------------------------------------------*/
//----------------PGN256 CRM-------------
 //±æÊ¶½á¹û£¬ÎïÀíÁ¬½ÓÖ®ºó£¬³äµç»úÏòBMS·¢ËÍ´Ë±¨ÎÄ£¬ÔÚÊÕµ½BMS·¢ËÍµÄ±æÊ¶±¨ÎÄÖ®Ç°·¢ËÍ0x00£¬ÔÚÊÕµ½BMS±æÊ¶±¨ÎÄÖ®ºó·¢ËÍ0xAA
uint8_t ubRecognitionResult;	
//³äµç»ú±àºÅ£¬1/Î»£¬1Æ«ÒÆÁ¿£¬Êı¾İ·¶Î§1-100	
uint8_t ubChargerNumber;
//------------PGN512,BRM±¨ÎÄ²ÎÊı--------------------
//BMSÍ¨ĞÅĞ­Òé°æ±¾ºÅ£¬±¾±ê×¼¹æ¶¨µ±Ç°°æ±¾ÎªV1.0 ±íÊ¾Îª£ºbyte3 byte2 byte1  0x 00 01 00
uint32_t ulBmsProtocolVersion;
//µç³ØÀàĞÍ£º01H£ºÇ¦Ëáµç³Ø 02H£ºÄøÇâµç³Ø	03£ºÁ×ËáÌúï®µç³Ø 04H£ºÃÌËáï®µç³Ø 05H£ºîÜËáï®µç³Ø 06H£ºÈıÔª²ÄÁÏµç³Ø 07H£º¾ÛºÏÎïï®Àë×Óµç³Ø 08H£ºîÑËáï®µç³Ø FFH£ºÆäËûµç³Ø
uint8_t ubBatteryType;
//Õû³µ¶¯Á¦Ğîµç³ØÏµÍ³¶î¶¨ÈİÁ¿/A*h,0.1A*h/Î»£¬0.1A*hÆ«ÒÆÁ¿ Êı¾İ·¶Î§ :0-¡ª¡ª1000A.h
uint16_t uwBmsRateCapacity;
//Õû³µ¶¯Á¦Ğîµç³ØÏµÍ³¶î¶¨×ÜµçÑ¹ /V £¬O.l V/Î» £¬0 V Æ«ÒÆÁ¿£¬Êı¾İ·¶ Î§:0  750 V
uint16_t uwBatteryRateVoltage;
//µç³ØÉú²ú³§ÉÌÃû³Æ£¬±ê×¼ ASCII Âë
uint8_t ubBatteryManufacturerName[4];
//µç³Ø×éĞòºÅ£¬Ô¤Áô£¬ÓÉ³§ÉÌ×ÔĞĞ¶¨Òå
uint16_t ulBatteryManufacturerName;
//µç³Ø×éÉú²úÈÕÆÚ£¬Äê 1985Æ«ÒÆÎ»
uint8_t ubBatteryProduceYear;
//µç³Ø×éÉú²úÈÕÆÚ£¬ÔÂ
uint8_t ubBatteryProduceMonth;
//µç³Ø×éÉú²úÈÕÆÚ£¬ÈÕ
uint8_t ubBatteryProduceDay;
//µç³Ø×é³äµç´ÎÊı£¬1´ÎÃ¿Î»  ,3¸ö×Ö½Ú
uint16_t ulBatteryChargeCycle;
//µç³Ø×é²úÈ¨±êÖ¾  0±íÊ¾×âÁŞ   1±íÊ¾ ³µ×ÔÓĞ
uint8_t ubPropertyMark;
//Ô¤Áô
uint8_t ubBMSReserved;
//³µÁ¾Ê¶±ğÂëVehicleIdentificationNumber
uint8_t ubVIN[17];

/*-----------------------------------------------
-------------------²ÎÊıÅäÖÃ½×¶Î±¨ÎÄ--------------
-------------------------------------------------*/

//-------------------PGN1536 BCP±¨ÎÄ²ÎÊı-------------
//µ¥Ìå¶¯Á¦Ğîµç³Ø×î¸ßÔÊĞí³äµçµçÑ¹  Êı¾İ·Ö±æÂÊ :0.01 V/Î»£¬ 0 V Æ«ÒÆÁ¿ E Êı¾İ·¶Î§ :0 V~24V;   
uint16_t uwCellMaxAllowChargeVoltage;
//×î¸ßÔÊĞí³äµçµçÁ÷	 Êı¾İ·Ö±æÂÊ :0.lA/Î»£¬ -400 A Æ«ÒÆÁ¿;Êı¾İ·¶Î§: -400 A ~O A;
uint16_t uwMaxAllowChargeCurrent;
//¶¯Á¦Ğîµç³Ø±ê³Æ×ÜÄÜÁ¿	  Êı¾İ·Ö±æÂÊ :0.1 kW  o h/ Î»£¬ 0 kW. h Æ«ÒÆÁ¿;Êı¾İ·¶Î§ :0£¬.....£¬1 000  kW o h; 
uint16_t uwBatteryTotalEnergy;
//×î¸ßÔÊĞí³äµç×ÜµçÑ¹   Êı¾İ·Ö±æÂÊ :0.1 V/ Î»£¬ 0  V Æ«ÒÆÁ¿;Êı¾İ·¶Î§ :0 V £¬......750  V;
uint16_t uwMaxAllowChargeVoltage;
//×î¸ßÔÊĞíÎÂ¶È		 Êı¾İ·Ö±æÂÊ :1 oC/Î»£¬-50 OC Æ«ÒÆÁ¿ F Êı¾İ·¶Î§:Ò» 50 oC £¬.....£¬ +200 oC;
uint8_t ubMaxAllowTemperature;
//Õû³µ¶¯Á¦Ğîµç³ØºÉµç×´Ì¬	Êı¾İ·Ö±æÂÊ :0.1%/Î»£¬ 0%Æ«ÒÆÁ¿;Êı¾İ·¶Î§ :0£¬.....£¬100% ; 
uint16_t uwBmsSOC;
//Õû³µ¶¯Á¦Ğîµç³Ø×ÜµçÑ¹
uint16_t uwBmsTotalVoltage;
//---------PGN2048 CML±¨ÎÄ-------------
//DC×î¸ßÊä³öµçÑ¹
uint16_t uwDCMaxOutputVoltage;
//DC×îµÍÊä³öµçÑ¹
uint16_t uwDCMinOutputVoltage;
//DC×î´óÊä³öµçÁ÷
uint16_t uwDCMaxOutputCurrent;
//---------PGN2304 BRO±¨ÎÄ-------------
//BMS ÊÇ·ñ³äµç×¼±¸ºÃ(< OxOO-) := BMS Î´×öºÃ³äµç×¼±¸; (OxAA):=BMS Íê³É³äµç×¼±¸; (OxFF):=ÔªĞ§)
uint8_t ubBmsChargeReady;
//------------PGN2560 CRO±¨ÎÄ
//³äµç»úÊÇ·ñ³äµç×¼±¸ºÃ ((OxOO):=³äµç»úÎ´Íê³É³äµç×¼±¸; (OxAA):=³äµç»úÍê³É³äµç×¼±¸; (OxFF):=ÎŞĞ§)
uint8_t ubChargerOutputReady;

/*------------------------------------------
----------------³äµç½×¶Î±¨ÎÄ---------------
-------------------------------------*/

//-----------PGN4096 BCL±¨ÎÄ----------------
//µçÑ¹ĞèÇó(V)
uint16_t uwVoltageRequirement;
uint8_t uwVoltageRequirementLow;
uint8_t uwVoltageRequirementHigh;
//µçÁ÷ĞèÇó (A)
int16_t uwCurrentRequirement;
int8_t uwCurrentRequirementLow;
int8_t uwCurrentRequirementHigh;
int8_t uwCurrentRequirementLowbuf;
int8_t uwCurrentRequirementHighbuf;
int16_t uwCurrentRequirementbuf;
uint16_t CURRENT;
//³äµçÄ£Ê½ (Ox01:ºãÑ¹³äµç	;Ox02:ºãÁ÷³äµç)
uint8_t ubChargeMode;
//-------------PGN4352 BCS±¨----------------
//³äµçµçÑ¹²âÁ¿Öµ (V)
uint16_t uwChargeVoltageMeasurement;
//³äµçµçÁ÷²âÁ¿Öµ (A)
uint16_t uwChargeCurrentMeasurement;
//×î¸ßµ¥Ìå¶¯Á¦Ğîµç³ØµçÑ¹¼°Æä×éºÅ
uint16_t uwHighestCellVoltageAndGroupNumber;
//µ±Ç°ºÉµç×´Ì¬ SOC(%)
uint8_t ubCurrentSOC;
//¹ÀËãÊ£Óà³äµçÊ±¼ä (min)
uint16_t uwRemainChargeTime;
//---------PGN4608 CCS±¨ÎÄ-------------
//µçÑ¹Êä³öÖµ (V)
uint32_t uwOutputVoltageValue;
//µçÁ÷Êä³öÖµ (A)
uint32_t uwOutputCurrentValue;
//ÀÛ¼Æ³äµçÊ±¼ä (min)
uint16_t uwAccumulateChargeTime;
//--------------PGN4864 BSM--------
//×î¸ßµ¥Ìå¶¯Á¦Ğîµç³ØµçÑ¹ËùÔÚ±àºÅ
uint8_t ubMaxCellVoltageNumber;
//×î¸ß¶¯Á¦Ğîµç³ØÎÂ¶È
uint8_t ubMaxTemperature;
//×î¸ßÎÂ¶È¼ì²âµã±àºÅ
uint8_t ubMaxTemperatureNumber;
//×îµÍ¶¯Á¦Ğîµç³ØÎÂ¶È
uint8_t ubMinTemperature;
//×îµÍ¶¯Á¦Ğîµç³ØÎÂ¶È¼ì²âµã±àºÅ
uint8_t ubMinTemperatureNumber;
//Òì³£×´Ì¬ĞÅÏ¢	 
//µ¥Ìå¶¯Á¦Ğîµç³ØµçÑ¹¹ı¸ß/¹ıµÍ ((00): =Õı³£ ;{01):= ¹ı¸ß;(10):=¹ıµÍ)
//Õû³µ¶¯Á¦Ğîµç³ØºÉµç×´Ì¬ soc ¹ı¸ß/¹ıµÍ ((00): =Õı³£ ;(01):= ¹ı¸ß;(10):= ¹ıµÍ¡µ
//¶¯Á¦Ğîµç³Ø³äµç¹ıµçÁ÷ ((00):= Õı³£ ;(01):== ¹ıÁ÷;(10): =²»¿ÉĞÅ ×´Ì¬¡µ
//¶¯Á¦Ğîµç³ØÎÂ¶È¹ı¸ß ((00): =Õı³£; (01): =:¹ı¸ß;(1 0):= ²»¿ÉĞÅ ×´Ì¬
//¶¯Á¦Ğîµç³Ø¾øÔµ×´Ì¬ ((00): =Õı³£ ;(01):=²»Õı³£;(10):= ²»¿ÉĞÅ ×´Ì¬¡µ
//¶¯Á¦Ğîµç³Ø×éÊä³öÁ¬½ÓÆ÷Á¬½Ó×´Ì¬ ((00):= Õı³£; (01): =²»Õı³£;(10): =²»¿ÉĞÅ×´Ì¬¡µ
//³äµçÔÊĞí ((00):= ½ûÖ¹ ;(01):=ÔÊĞí¡µ
uint16_t uwStatusAndAbnormal;
//----------------PGN5376 BMV--------------
//µ¥Ìå¶¯Á¦Ğîµç³ØµçÑ¹
uint16_t uwCellVoltage[256];

//----------------PGN5632 BMT--------------
//µ¥Ìå¶¯Á¦Ğîµç³ØÎÂ¶È
uint8_t ubCellTemperature[128];

//----------------PGN5888 BSP--------------
//µ¥Ìå¶¯Á¦Ğîµç³ØÔ¤Áô±¨ÎÄ
uint8_t ubCellReserved[16];

//--------------PGN6400 BTS----------
//BMS  ÖĞÖ¹³äµçÔ­Òò
uint8_t ubBmsChargeStopReason;
//BMS  ÖĞÖ¹³äµç¹ÊÕÏÔ­Òò
uint16_t uwBmsChargeStopFaultReason;
//BMS  ÖĞÖ¹³äµç´íÎóÔ­Òò
uint8_t ubBmsChargeStopErrorReason;

//------------PGN6656 CST-------------
//³äµç»úÖĞÖ¹³äµçÔ­Òò
uint8_t ubChargerStopReason;
//¿Ëµç»úÖĞÖ¹³äµç¹ÊÕÏÔ­Òò
uint16_t uwChargerStopFaultReason;
//³äµç»úÖĞÖ¹³äµç´íÎóÔ­Òò
uint8_t ubChargerStopErrorReason;

//------------PGN7168 BSD--------------
//ÖĞÖ¹ºÉµç×´Ì¬ SOC(%)
uint8_t ubBmsEndOfSOC;
//¶¯Á¦Ğîµç³Øµ¥Ìå×îµÍµçÑ¹ (V)
uint16_t uwBmsEndMinCellVoltage;
//¶¯Á¦Ğîµç³Øµ¥Ìå×î¸ßµçÑ¹ (V)
uint16_t uwBmsEndMaxCellVoltage;
//¶¯Á¦Ğîµç³Ø×îµÍÎÂ¶È CC)
uint8_t ubBmsEndMinTemperature;
//¶¯Á¦Ğîµç³Ø×î¸ßÎÂ¶È CC)
uint8_t ubBmsEndMaxTemperature;

//-----------PGN7424 CSD-----------
//ÀÛ¼Æ³äµçÊ±¼ä (min)
uint16_t uwChargerEndChargeTime;
//Êä³öÄÜÁ¿ (kW. h)
uint16_t uwChargerEndChargePower;

//---------PGN7680 BEM------------
//BMS ´íÎó±¨ÎÄ (BEM)
uint8_t ubBmsError[4];

//-----------PGN7936 CEM-----------
//³äµç»ú´íÎó±¨ÎÄ (CEM)
uint8_t ubChargerError[4];

//DCÖ±Á÷³äµçÄ£¿é×´Ì¬
uint8_t ubDCModuleStatusHighAll;
uint8_t ubDCModuleStatusLowAll;

uint8_t ubDCModuleStatusHigh[4];
uint8_t ubDCModuleStatusLow[4];
// DCµçÑ¹Êä³ö
uint16_t uwDCOutputVoltage[4];
//DCµçÁ÷Êä³ö
uint16_t uwDCOutputCurrent[4];
// BMSĞèÇó³äµç·½Ê½»º´æÖµ (Ox01:ºãÑ¹³äµç	;Ox02:ºãÁ÷³äµç)
uint8_t ubChargeModeBuf;

//DCÖ±Á÷³äµçÄ£¿é×´Ì¬
uint16_t uwDCModuleStatus;
//DCÄ£¿éÊÇ·ñÆô¶¯
uint8_t ubDCModue;  
//¶à¸öDC³äµçÄ£¿éµÄµØÖ·£¬1-N¸öÄ£¿é·Ö±ğÎª0x01 0x02 0x03 ......£¬
uint8_t ubDCModueAddress;
#define CAN_STARTDCMODUE 0x55
#define CAN_STOPDCMODUE 0xAA


//±êÖ¾£¬ÊÇ·ñ³¬Ê±5s 0x00²»Âú×ã£¬0xAAÂú×ã
uint8_t FlagIfTimeout;
//±êÖ¾£¬DCµçÔ´Ä£¿éÊÇ·ñ³¬Ê±5s 0x00²»Âú×ã£¬0xAAÂú×ã
uint8_t FlagIfDCTimeout;
//³¬Ê±Ê±¼äÉèÖÃ£¬ubTimeNs±íÊ¾¶àÉÙ¸ö100ms
uint8_t ubTimeN100ms;


//±êÖ¾£¬ÊÇ·ñÂú×ãÖÜÆÚ·¢ËÍ ·¢ËÍÖÜÆÚ10ms 0²»Âú×ã£¬1Âú×ã
uint8_t FlagIfCycleSend10ms;
//±êÖ¾£¬ÊÇ·ñÂú×ãÖÜÆÚ·¢ËÍ ·¢ËÍÖÜÆÚ50ms 0²»Âú×ã£¬1Âú×ã
uint8_t FlagIfCycleSend50ms;
//±êÖ¾£¬ÊÇ·ñÂú×ãÖÜÆÚ·¢ËÍ ·¢ËÍÖÜÆÚ250ms 0²»Âú×ã£¬1Âú×ã
uint8_t FlagIfCycleSend250ms;
//±êÖ¾£¬ÊÇ·ñÂú×ãÖÜÆÚ·¢ËÍ ·¢ËÍÖÜÆÚ500ms 0²»Âú×ã£¬1Âú×ã
uint8_t FlagIfCycleSend500ms;
//±êÖ¾£¬ÊÇ·ñÂú×ãÖÜÆÚ·¢ËÍ ·¢ËÍÖÜÆÚ1000ms 0²»Âú×ã£¬1Âú×ã
uint8_t FlagIfCycleSend1000ms;

//¶¨Ê±Æ÷6¼ÆÊı£¬10ms¼ÆÊıÒ»´Î   
uint8_t ubCANTim3Count;
//¶¨Ê±Æ÷6¼ÆÊı£¬10ms¼ÆÊıÒ»´Î,CANÍ¨ĞÅ³µÁ¾Í¨ĞÅ³¬Ê±¼ÆÊıÆ÷   
uint16_t uwCANTimeOutCount;
//¶¨Ê±Æ÷6¼ÆÊı£¬10ms¼ÆÊıÒ»´Î,CANÍ¨ĞÅDCµçÔ´Ä£¿é³¬Ê±¼ÆÊıÆ÷   
uint16_t uwCANDCTimeOutCount;


//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄBMSºÍ³µÁ¾±æÊ¶±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
uint8_t ubFlagIfReceBmsIdentifyMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄµç³Ø³äµç²ÎÊı±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
uint8_t ubFlagIfReceBatteryParameterMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄµç³Ø³äµç×¼±¸¾ÍĞ÷×´Ì¬±¨ÎÄ 0Î´ÊÕµ½£¬1ÊÕµ½
uint8_t FlagIfReceBmsReadyMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄµç³Ø³äµç×Ü×´Ì¬±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
uint8_t ubFlagIfReceBmsAllChargeStateMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄµç³Ø³äµçĞèÇó±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
uint8_t FlagIfReceBmsChargeRequirementMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄ¶¯Á¦Ğîµç³Ø×´Ì¬ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
uint8_t FlagIfReceBmsBatteryStateMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍ½áÊø³äµç»òÕßBMSÖĞÖ¹³äµç±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
uint8_t ubFlagIfReceBmsEndAndStopChargeMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄBMSÖĞÖ¹³äµç±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
uint8_t FlagIfReceBmsStopChargeMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄ³äµçÍ³¼ÆÊı¾İ±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
uint8_t FlagIfReceBmsStatisticsDataMeg;


//±êÖ¾£¬ÊÇ·ñ·¢ËÍ±¨ÎÄ¸øDCÖ±Á÷³äµç»úÄ£¿é 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
uint8_t FlagIfSendDCModuleMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½DCÖ±Á÷³äµç»úÄ£¿é·¢ËÍµÄ±¨ÎÄ 0Ã»ÊÕµ½£¬1ÊÕµ½
uint8_t FlagIfReceDCModuleMeg;

//±êÖ¾£¬ÊÇ·ñ·¢ËÍ³äµç»ú±æÊ¶±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
uint8_t ubFlagIfSendChargerIdentifyMeg;
//±êÖ¾£¬ÊÇ·ñ·¢ËÍ³äµç»úÊ±¼äÍ¬²½ĞÅÏ¢±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
uint8_t ubFlagIfSendChargerTimeMeg;
//±êÖ¾£¬ÊÇ·ñ·¢ËÍ×î´óÊä³öÄÜÁ¦±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
uint8_t ubFlagIfSendChargerMaxOutputCapactityMeg;
//±êÖ¾£¬ÊÇ·ñ·¢ËÍ³äµç»úÊä³ö¾ÍĞ÷×´Ì¬±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
uint8_t ubFlagIfSendChargerOutputReadyMeg;
//±êÖ¾£¬ÊÇ·ñ·¢ËÍ³äµç»ú³äµç×´Ì¬±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
uint8_t FlagIfSendChargerChargingStateMeg;
//±êÖ¾£¬ÊÇ·ñ³äµç»úÖĞÖ¹³äµç±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
uint8_t ubFlagIfSendChargerStopChargeMeg;
//±êÖ¾£¬ÊÇ·ñ·¢ËÍ³äµç»ú³äµçÍ³¼ÆÊı¾İ±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
uint8_t ubFlagIfSendChargerStatisticsDataMeg;



//CAN½ÓÊÕBMSÊı¾İ»º³åÆ÷  Í¨ÓÃÅÅ³ıÒÔÏÂ¼¸ÖÖÇé¿ö
uint8_t ubCANBuf_BMS[8];
//CAN½ÓÊÕBMSÊı¾İ»º³åÆ÷¶şÎ»Êı×éË÷Òı
uint8_t ubCANBufIndex_BMS;
//CANÊÕµ½BMSÊı¾İµÄID
uint32_t uCANBufID_BMS;

//CAN½ÓÊÕDCÊı¾İ»º³åÆ÷  Í¨ÓÃÅÅ³ıÒÔÏÂ¼¸ÖÖÇé¿ö
uint8_t ubCANBuf_DC[8];
//CAN½ÓÊÕDCÊı¾İ»º³åÆ÷¶şÎ»Êı×éË÷Òı
uint8_t ubCANBufIndex_DC;
//CANÊÕµ½DCÊı¾İµÄID
uint32_t uCANBufID_DC;

//¶àÖ¡Á¬½Ó±¨ÎÄ½¨Á¢Á¬½ÓÓÃ
uint8_t ubCANConnect[8];
uint8_t ubCANMultiFrame[70][8];
uint8_t ubCANMultiFrameIndex;

uint16_t uwCANReceFlagState;//ÊÇ·ñÊÕµ½CANÊı¾İÖ¡±êÖ¾Î» 0±íÊ¾ÎŞĞ§ 1±íÊ¾ÓĞĞ§  
                                 //µÚ0Î» BMSºÍ³µÁ¾±æÊ¶±¨ÎÄ
                                 //µÚ1Î» ¶¯Á¦Ğîµç³Ø³äµç²ÎÊı
                                 //µÚ2Î» µç³Ø³äµç×¼±¸¾ÍĞ÷±¨ÎÄ
                                 //µÚ3Î» µç³Ø³äµçĞèÇó
                                 //µÚ4Î» µç³Ø³äµç×Ü×´Ì¬
                                 //µÚ5Î» ¶¯Á¦Ğîµç³Ø×´Ì¬ĞÅÏ¢
                                 //µÚ6Î» µ¥Ìå¶¯Á¦Ğîµç³ØµçÑ¹
                                 //µÚ7Î» ¶¯Á¦Ğîµç³ØÎÂ¶È
                                 //µÚ8Î» ¶¯Á¦Ğîµç³ØÔ¤Áô±¨ÎÄ
                                 //µÚ9Î» BMSÖĞÖ¹³äµç
                                 //µÚ10Î» BMSÍ³¼Æ±¨ÎÄ
                                 //µÚ11Î» BMS´íÎó±¨ÎÄ
                                 //µÚ12Î» DCµçÔ´Êä³ö×´Ì¬±¨Î
uint16_t uwCANSendFlagState;//ÊÇ·ñ·¢ËÍCANÊı¾İÖ¡±êÖ¾Î» 0±íÊ¾ÎŞĞ§ 1±íÊ¾ÓĞĞ§  
                                 //µÚ0Î» ³äµç»ú±æÊ¶±¨ÎÄ
                                 //µÚ1Î» ³äµç»ú·¢ËÍÊ±¼äÍ¬²½ĞÅÏ¢
                                 //µÚ2Î» ³äµç»ú×î´óÊä³öÄÜÁ¦
                                 //µÚ3Î» ³äµç»úÊä³ö×¼±¸¾ÍĞ÷±¨ÎÄ
                                 //µÚ4Î» ³äµç»ú³äµç×´Ì¬
                                 //µÚ5Î» ³äµç»úÖĞÖ¹³äµç
                                 //µÚ6Î» ³äµç»úÍ³¼ÆÊı¾İ
                                 //µÚ7Î» ³äµç»ú´íÎó±¨ÎÄ
                                 //µÚ8Î» DCµçÔ´ÉèÖÃ±¨ÎÄ
                                 //µÚ9Î» DCµçÔ´Æô¶¯Í£Ö¹±¨ÎÄ           

uint8_t ubManyBytesStateBCS;//¶àÖ¡Êı¾İµÄ±êÖ¾
uint8_t ubManyBytesStateBMV;//¶àÖ¡Êı¾İµÄ±êÖ¾
uint8_t ubManyBytesStateBMT;//¶àÖ¡Êı¾İµÄ±êÖ¾
uint8_t ubManyBytesStateBSP;//¶àÖ¡Êı¾İµÄ±êÖ¾

//¶àÊı¾İ´«ÊäĞ­ÒéÁ¬½Ó¹ÜÀí»º´æÊı¾İ
uint8_t ubCANConnect[8];

uint8_t ubCellVoltageQuantity;     //µ¥Ìå¶¯Á¦Ğîµç³Øµç³ØµçÑ¹ÊıÁ¿ 
uint8_t ubCellTemperatureQuantity; //¶¯Á¦Ğîµç³ØÎÂ¶È ÊıÁ¿
uint8_t ubCellReservedQuantity;    //¶¯Á¦Ğîµç³ØÔ¤Áô±¨ÎÄ  ÊıÁ¿

uint8_t ubCellVoltageMegQuantity;     //µ¥Ìå¶¯Á¦Ğîµç³Øµç³ØµçÑ¹ ±¨ÎÄÖ¡ÊıÁ¿ 
uint8_t ubCellTemperatureMegQuantity; //¶¯Á¦Ğîµç³ØÎÂ¶È ±¨ÎÄÖ¡ÊıÁ¿
uint8_t ubCellReservedMegQuantity;    //¶¯Á¦Ğîµç³ØÔ¤Áô±¨ÎÄ  ±¨ÎÄÖ¡ÊıÁ¿

//ÎïÀíÁ¬½ÓÕı³£±êÖ¾
uint8_t ubPhysicalConnect;  //0x00±íÊ¾Î´Á¬½Ó£¬0xAA±íÊ¾ÎïÀíÁ¬½ÓÕı³££¬ÆäËûÎŞĞ§

uint16_t uwReadTimeCount;//CANÍ¨ĞÅ×îĞ¡¼ÆÊ±µ¥Î»ÊÇ·ÖÖÓ    ÕâÀï¶¨Òå30Ãë¶ÁÈ¡Ò»´ÎÊ±¼ä£¬ºóÆÚ¸ù¾İÒªÇóĞŞ¸Ä
uint8_t FlagIfReadTime;//ÊÇ·ñ¶ÁÈ¡Ê±¼ä±êÖ¾Î»
uint8_t FlagIfSendDCMeg;
uint8_t ubDCModue;      //DCÄ£¿éÊÇ·ñÆô¶¯
uint8_t FlagIfShowInformation5S = 0;  //Ë¢¿¨½áÊøºóÍ£Áô±êÖ¾Î»
uint8_t FlagShowInformation = 0;

uint32_t Flag=0;

uint8_t ChargeFlag = 0;  //³äµç±êÖ¾ 1:ÕıÔÚ³äµç 0:²»ÔÚ³äµç
uint8_t CANChargeFlag = 0; //³äµç½×¶Î±êÖ¾
//-----------------------------------------
//¶Á¿¨Æ÷Ê¹ÓÃµÄ±äÁ¿
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
uint8_t WriteBlockFlag = 0;//Ğ´¿¨Ä£¿é±êÖ¾Î»£¬µ÷ÓÃWriteBlock(1,ICDataReadBuf)º¯ÊıÊ±£¬´Ë±êÖ¾Î»ÖÃ1,Ğ´¿¨³É¹¦Ö®ºó´Ë±êÖ¾Î»¸´Î»0
uint8_t WriteBlockTimeFlag = 0;//Ğ´¿¨Ä£¿éÖÜÆÚ·¢ËÍ±êÖ¾£¬100msÖÃÎ»Ò»´Î£¬ÓÃÍê¼´Çå0

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
unSaveRealTimeData_HandleTypedef  unSaveRealTimeDataHandle ;   //³äµçĞÅÏ¢ÁªºÏÌå
unSystemParameterConfig_HandleTypedef   unSystemParameterConfigHandle;   //´æ´¢Êı¾İÁªºÏÌå
unIAPSystemParameter_HandleTypeDef  unIAPSystemParameter;     //IAPÁªºÏÌå

uint8_t Time[8];

//µç±í±äÁ¿
uint8_t UART5RxBuff[24];
uint8_t UART5RxBuffCount;
uint8_t UART5RxFlag;

uint8_t	 PMsendBuf[8];
uint8_t  VoltageA[2];
uint8_t  VoltageB[2];
uint8_t  VoltageC[2];
uint8_t  RectivePower[2];//ÎŞ¹¦¹¦ÂÊ
uint8_t  ApparentPower[2];//×ÜÊÓÔÚ¹¦ÂÊ
uint8_t  Frequency[2];//ÆµÂÊ
uint8_t  ACEnergy [4]; //ÓĞ¹¦µçÄÜ
uint8_t  CurrentA[2];//AÏàµçÁ÷
uint8_t  CurrentB[2];//
uint8_t  CurrentC[2];//
uint8_t  DCVoltage[2];//Ö±Á÷±íµçÑ¹
uint8_t  DCEnergy [4]; //Ö±Á÷±íµçÄÜ
uint8_t  DCCurrent[2];//Ö±Á÷±íµçÁ÷
uint8_t  InsulationRes[4];//¾øÔµµç×è
uint8_t  Crc485[2];

uint8_t  TxFlag485; 
uint8_t  RxFlag485=0;
uint8_t  PMCount485;
uint8_t  Tim4Flag;
uint8_t  Flag485;//´ú±í·¢ËÍ²»Í¬µÄÖ¸Áî£º
uint8_t  Fun485Flag;  // 1:ÈıÏàµçÁ÷  2:ÈıÏàµçÑ¹  3:½»Á÷µçÄÜ 4:½»Á÷ÆµÂÊ   5:×ÜÎŞ¹¦¹¦ÂÊ   6:×ÜÊÓÔÚ¹¦ÂÊ  7:¾øÔµ¼ì²âµç×è  8:Ö±Á÷µç±í 0:²»·¢Ö¸Áî

uint8_t   StartEnergy[4];
uint32_t   StartEnergyAll;
uint8_t   DealEnergy[4] = {0,0,0,0};
uint32_t  DealEnergyAll;

uint8_t  cmd_buffer[MAX_CMD_BUFFER];



/*         Ò»°ã¶¨Òå               */
uint8_t FaultOperation = 0;
uint8_t SystemState;      //ÏµÍ³×´Ì¬£º0£¬¿ÕÏĞ×´Ì¬£»1£¬³äµç×´Ì¬ £»2£¬Òì³£×´Ì¬£»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»¸ù¾İÉèÖÃÌõ¼şÇĞ»»ÏµÍ³×´Ì¬
uint8_t ChargingMethod = 0;  //³äµç·½Ê½£º0£¬×Ô¶¯³äµç£»1£¬ÊÖ¶¯³äµç £»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»ÆÁÄ»Ñ¡ÔñÊÖ¶¯Ä£Ê½ÖÃ1
uint8_t ChargingMode = 0;     //³äµçÄ£Ê½£º0£¬³äÂúÎªÖ¹£»1£¬°´Ê±¼ä³ä £»2.°´µçÁ¿³ä£»3£¬°´½ğ¶î³ä£»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»ÆÁÄ»Ñ¡Ôñ
uint16_t SetKwh=0;                             //³äµçÄ£Ê½½çÃæ£¬ÉèÖÃµçÁ¿
uint16_t SetMoney=0;                           //³äµçÄ£Ê½½çÃæ£¬ÉèÖÃ½ğ¶î
uint16_t SetHour=0;                            //³äµçÄ£Ê½½çÃæ£¬ÉèÖÃÊ±¼ä£¨Ê±£©
uint16_t SetMin=0;                             //³äµçÄ£Ê½½çÃæ£¬ÉèÖÃÊ±¼ä£¨·Ö£©
uint16_t SetChargeCutOffCurrent=0;           //ÉèÖÃ½ØÖ¹µçÁ÷
uint16_t SetChargeVoltage=0;                 //ÉèÖÃ³äµçµçÑ¹
uint16_t SetChargeCurrent=0;                 //ÉèÖÃ³äµçµç?

uint8_t Timer1CountNumber; //Ã¿500ms¼Ó1£¬10s¸´Î»£¬¶¨Ê±Æ÷1ÓÃ
uint8_t FlagCycle30s;     //10sÑ­»·¶¨Ê±±êÖ¾Î»£¬Ä¬ÈÏÎª0£»¶¨Ê±Æ÷1ÖÃÎ»£¬Ê¹ÓÃÍêÇåÁã
uint8_t FlagCycle10s;     //10sÑ­»·¶¨Ê±±êÖ¾Î»£¬Ä¬ÈÏÎª0£»¶¨Ê±Æ÷1ÖÃÎ»£¬Ê¹ÓÃÍêÇåÁã
uint8_t FlagCycle1s;      //1sÑ­»·¶¨Ê±±êÖ¾Î»£¬Ä¬ÈÏÎª0£»¶¨Ê±Æ÷1ÖÃÎ»£¬Ê¹ÓÃÍêÇåÁã
uint8_t FlagCycle500ms;   //500msÑ­»·¶¨Ê±±êÖ¾Î»£¬Ä¬ÈÏÎª0£»¶¨Ê±Æ÷1ÖÃÎ»£¬Ê¹ÓÃÍêÇåÁã

uint16_t SystemFaultState;   //ÏµÍ³¹ÊÕÏ×´Ì¬£
                             //0000 0000 0000 0001 ¼±Í£°´Å¥°´ÏÂ   //0000 0001 0000 0000   ´æ´¢Æ÷Òì³£
							 //0000 0000 0000 0010 ²ÖÃÅÎ´¹Ø±Õ     //0000 0010 0000 0000   Ê±ÖÓÒì³£
							 //0000 0000 0000 0100 ×®ÌåÇãĞ±       //0000 0100 0000 0000   ¶Á¿¨Æ÷Òì³£
							 //0000 0000 0000 1000 ×®Ìå×²»÷       //0000 1000 0000 0000   ÈıÏàµçÈ±Ïà
                             //0000 0000 0001 0000 »·¾³ÎÂ¶È¹ı¸ß   //0001 0000 0000 0000   ¾øÔµµç×èÒì³£
                             //0000 0000 0010 0000 »·¾³ÎÂ¶È¹ıµÍ   //0010 0000 0000 0000   ¾øÔµ¼ì²âÒÇÒì³£ 
                             //0000 0000 0100 0000 »·¾³Êª¶È¹ı¸ß   //0100 0000 0000 0000   µçÔ´²ÖÎÂ¶È¹ı¸ß
							 //0000 0000 1000 0000 »·¾³Êª¶È¹ıµÍ   //1000 0000 0000 0000   µç±íÒì³£
uint8_t PileTiltState;              //³äµç×®×®ÌåÇãĞ±×´Ì¬£¬0£¬Î´ÇãĞ±£»1£¬ÇãĞ±£»Ä¬ÈÏÎª0£»
uint8_t CabinDoorState;             //²ÖÃÅ×´Ì¬£¬0£¬¹Ø±Õ£»1£¬´ò¿ª£»Ä¬ÈÏÎª0£»
uint8_t EmergencyStopButtonState;   //¼±Í£°´Å¥×´Ì¬×´Ì¬£¬0£¬Õı³££»1£¬°´ÏÂ£»Ä¬ÈÏÎª0£»
uint8_t LevelState;                 //Ë®Æ½ÒÇ×´Ì¬£¬0±íÊ¾Õı³££¬1±íÊ¾²»Õı³££¬Ä¬ÈÏÎª0£

uint8_t PileState;                  //Ç¹Í·×´Ì¬£¬0±íÊ¾Î´Á¬½Ó£¬1±íÊ¾Á¬½Ó£¬Ä¬ÈÏÎª0
uint16_t DS18B20Temperature;       //DS18b20´«¸ĞÆ÷ÎÂ¶È
uint16_t AM2321Temperature;        //AM2321ÎÂÊª¶È´«¸ĞÆ÷ÎÂ¶È
uint16_t Am2321Humidity;           //AM2321ÎÂÊª¶È´«¸ĞÆ÷Êª¶È

uint8_t SwipingCardFlag =0;   //Ë¢¿¨±êÖ¾£¬Ä¬ÈÏÎª0 ÎŞÒâÒå 1±íÊ¾Ë¢¿¨
uint8_t SwipingPileFlag = 0;
uint8_t SpeechFlag =0;        //ÓïÒô±êÖ¾£¬Ä¬ÈÏÎª0 1±íÊ¾ÓïÒôÓĞĞ§
uint8_t SpeechFlagMoney =0;        //ÓïÒô±êÖ¾£¬Ä¬ÈÏÎª0 1±íÊ¾ÓïÒôÓĞĞ§
uint8_t SpeechFlagDeal =0;        //ÓïÒô±êÖ¾£¬Ä¬ÈÏÎª0 1±íÊ¾ÓïÒôÓĞĞ§
uint8_t FlagIfCloseDoor = 0;       // ¹Ø±Õ²ÖÃÅ±êÖ¾
uint8_t FlagOpenDoor = 0;           //´ò¿ª²ÖÃÅ

uint8_t FlagIfSaveData = 0;       //±£´æÉÏ´Î½»Ò×Êı¾İ Ä¬ÈÏÎª0 1±íÊ¾ÓĞĞ§
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
	uwRemainChargeTime = 0; //Ê£ÓàÊ±¼ä
	DealMoneyAll = 0;    //³äµç½ğ¶î
	uwBmsSOC = 0;   //BMS µÄSOC
	ChargeTime.Hour = 0;  //³äµçÊ±¼ä Ğ¡Ê±
	ChargeTime.Minute = 0;    //³äµçÊ±¼ä ·ÖÖÓ
	//FaultOperation = 0;       //´íÎó²Ù×÷±êÖ¾Î»
	DealEnergy[2] = 0;    //½»Ò×µçÄÜ¸ß°ËÎ»
	DealEnergy[3] = 0;     //½»Ò×µçÄÜµÍ°ËÎ»
	ubCANEventStatus = 1;
	FlagIfSaveData = 0;       //±£´æÉÏ´Î½»Ò×Êı¾İ Ä¬ÈÏÎª0 1±íÊ¾ÓĞĞ§
	UpDataId();
	UpDataBalance();
}


void SetACRelay(void) //Æô¶¯½»Á÷½Ó´¥Æ÷
{
	HAL_GPIO_WritePin( GPIOC,GPIO_PIN_2, GPIO_PIN_RESET);
}

void ResetACRwlay(void)//Í£Ö¹½»Á÷¼ÌµçÆ÷
{
	HAL_GPIO_WritePin( GPIOC,GPIO_PIN_2, GPIO_PIN_SET);
}

void SetDCRelay(void) //Æô¶¯Ö±Á÷½Ó´¥Æ÷
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_15, GPIO_PIN_SET);
}

void ResetDCRwlay(void)//Í£Ö¹Ö±Á÷½Ó´¥Æ÷
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_15, GPIO_PIN_RESET);
}

void SetBMSRelay(void) //Æô¶¯¸¨ÖúµçÔ´
{
	HAL_GPIO_WritePin( GPIOF,GPIO_PIN_10, GPIO_PIN_RESET);
}

void ResetBMSRwlay(void)//Í£Ö¹¸¨ÖúµçÔ´
{
	HAL_GPIO_WritePin( GPIOF,GPIO_PIN_10, GPIO_PIN_SET);
}

void StartFan(void)//Æô¶¯·çÉÈ
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_6, GPIO_PIN_SET);
}

void StopFan(void)//Í£Ö¹·çÉÈ
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_6, GPIO_PIN_RESET);
}

void OpenDoor(void)//¿ªÃÅ
{
	HAL_GPIO_WritePin( GPIOG,GPIO_PIN_9, GPIO_PIN_SET);
}
void CloseDoor(void)//¹ØÃÅ
{
	HAL_GPIO_WritePin( GPIOG,GPIO_PIN_9, GPIO_PIN_RESET);
}
uint8_t CheckPileState(void) //¼ì²âÇ¹Í·×´Ì¬£¬Ç¹Í·Á¬½ÓÎª·µ»Ø1 £¬Ç¹Í·Î´Á¬½Ó·µ»Ø0
{
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4) == 0)
		return 1;
	else
		return 0;	 
}

uint8_t CheckCabinDoorState(void) //¼ì²â²ÖÃÅ×´Ì¬£¬²ÖÃÅ¹Ø±ÕÎª1£¬²ÖÃÅÎ´¹Ø±ÕÎª0
{
	return HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_11);
}

uint8_t CheckLevelState(void)  //¼ì²âË®Æ½ÒÇ×´Ì¬£¬Ë®Æ½ÒÇÕı³£·µ»Ø0£¬Ë®Æ½ÒÇ²»Õı³£·µ»Ø1
{
	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4);
}
	
uint8_t CheckEmergencyStopButtonState(void)//¼ì²â¼±Í£°´Å¥×´Ì¬£¬¼±Í£°´Å¥Î´°´ÏÂÎª0£¬¼±Í£°´Å¥°´ÏÂÎª1
{
	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5);
}

void ReadSystemParameter(void)   //¿ª»ú¶ÁÈ¡ÏµÍ³²ÎÊı
{
	HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS+2,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
}

void ReadDealData(void)      //Ë¢¿¨Ç°¶ÁÈ¡ÉÏ´Î½»Ò×Êı¾İ
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
void DataInit(void)//Ò»°ã±äÁ¿³õÊ¼»¯
{
		uint8_t i;
		SystemState = 1;      //ÏµÍ³×´Ì¬£º0£¬¿ÕÏĞ×´Ì¬£»1£¬³äµç×´Ì¬ £»2£¬Òì³£×´Ì¬£»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»¸ù¾İÉèÖÃÌõ¼şÇĞ»»ÏµÍ³×´Ì¬
    ChargingMethod  = 0;  //³äµç·½Ê½£º0£¬×Ô¶¯³äµç£»1£¬ÊÖ¶¯³äµç £»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»ÆÁÄ»Ñ¡ÔñÊÖ¶¯Ä£Ê½ÖÃ1
    ChargingMode = 0;     //³äµçÄ£Ê½£º0£¬³äÂúÎªÖ¹£»1£¬°´Ê±¼ä³ä £»2.°´µçÁ¿³ä£»3£¬°´½ğ¶î³ä£»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»ÆÁÄ»Ñ¡Ôñ
    Timer1CountNumber = 0; //Ã¿500ms¼Ó1£¬10s¸´Î»£¬¶¨Ê±Æ÷1ÓÃ
    FlagCycle10s = 0;     //10sÑ­»·¶¨Ê±±êÖ¾Î»£¬Ä¬ÈÏÎª0£»¶¨Ê±Æ÷1ÖÃÎ»£¬Ê¹ÓÃÍêÇåÁã
    FlagCycle1s = 0;      //1sÑ­»·¶¨Ê±±êÖ¾Î»£¬Ä¬ÈÏÎª0£»¶¨Ê±Æ÷1ÖÃÎ»£¬Ê¹ÓÃÍêÇåÁã
    FlagCycle500ms = 0;   //500msÑ­»·¶¨Ê±±êÖ¾Î»£¬Ä¬ÈÏÎª0£»¶¨Ê±Æ÷1ÖÃÎ»£¬Ê¹ÓÃÍêÇåÁã

    PileTiltState = 0;              //³äµç×®×®ÌåÇãĞ±×´Ì¬£¬0£¬Î´ÇãĞ±£»1£¬ÇãĞ±£»Ä¬ÈÏÎª0£»
    CabinDoorState = 0;        //µçÔ´²Ö²ÖÃÅ×´Ì¬£¬0£¬¹Ø±Õ£»1£¬´ò¿ª£»Ä¬ÈÏÎª0£»
    EmergencyStopButtonState = 0;   //¼±Í£°´Å¥×´Ì¬×´Ì¬£¬0£¬Õı³££»1£¬°´ÏÂ£»Ä¬ÈÏÎª0£»
    PileState = 0;                  //Ç¹Í·×´Ì¬£¬0±íÊ¾Î´Á¬½Ó£¬1±íÊ¾Á¬½Ó     
    SystemFaultState = 0;	
		SwipingCardFlag = 0;
		SpeechFlag = 0;
	  ETHData.IPNumber = 0x0000;
	  ETHData.Length = 0;
	  FlagIfSaveData = 0;       //±£´æÉÏ´Î½»Ò×Êı¾İ Ä¬ÈÏÎª0 1±íÊ¾ÓĞĞ§
	  for(i=0;i<100;i++)
			{
					ETHData.Data[i] = 0;	
			}
}

void ShowPileFaultState(uint8_t w)//³äµç×®¹ÊÕÏ£¬ºìµÆÁÁ
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

void ShowPileChargeFullOrFree(uint8_t w)//³äµç×®³äÂú»òÕß¿ÕÏĞ 0²»ÁÁ1ÁÁ
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
	//Ö±½Ó¶Á£¬¼ÆËãCRC
	
	
}

void SaveIAPSystemParatemer(void)
{
	  //ÏÈĞ´Èë´æ´¢Æ÷ÔÙ¶Á³öÀ´£¬Ğ£ÑéCRC
	
	
}

uint8_t SystemSelfingCheck(void)//ÏµÍ³×Ô¼ì
{
	uint8_t buf1 = 0xAA;
	uint8_t buf2 = 0;
	uint8_t i = 0;
	SystemState = 0;      //ÏµÍ³×´Ì¬£º0£¬¿ÕÏĞ×´Ì¬£»1£¬³äµç×´Ì¬ £»2£¬Òì³£×´Ì¬£»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»¸ù¾İÉèÖÃÌõ¼şÇĞ»»ÏµÍ³×´Ì¬
	Speech("[v11]»¶Ó­Ê¹ÓÃ×¿Ô½Ö±Á÷³äµç×®");
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
		SystemFaultState = SystemFaultState|0x0100;//´æ´¢Æ÷Òì³£
	}
	CabinDoorState = CheckCabinDoorState( );
	if(1== CabinDoorState )
	{
		SystemFaultState = SystemFaultState|0x0002;//²ÖÃÅÎ´¹Ø±Õ
	}
	EmergencyStopButtonState = CheckEmergencyStopButtonState( );
	if(1 == EmergencyStopButtonState)
	{
		SystemFaultState = SystemFaultState|0x0001;//¼±Í£°´Å¥±»°´ÏÂÕ
	}
	LevelState = CheckLevelState( );
	if(1 == LevelState)
	{
		SystemFaultState = SystemFaultState|0x0008;//ÏµÍ³±»×²»÷Â
	}
	DS18B20Temperature = DS18B20_Get_Temp();
	if(MAXDS18B20TEMP < DS18B20Temperature)
	{
		SystemFaultState = SystemFaultState|0x4000;//µçÔ´²ÖÎÂ¶È¹ı¸ß
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
		SystemFaultState = SystemFaultState|0x0010;//»·¾³ÎÂ¶È¹ı¸ß
	}
	if(MINAM2321TEMP > AM2321Temperature)
	{
		SystemFaultState = SystemFaultState|0x0020;//»·¾³ÎÂ¶È¹ıµÍ
	}
	if(MAXAM2321HUMI < Am2321Humidity)
	{
		SystemFaultState = SystemFaultState|0x0040;//»·¾³Êª¶È¹ı¸ß
	}
	if(MINAM2321HUMI > Am2321Humidity)
	{
		SystemFaultState = SystemFaultState|0x0080;//»·¾³Êª¶È¹ıµÍ
	}
	//Read_Time( &RTCReadBCDTime);
	//UpdateRTC();
// 	Send_485(0x10,0x00);
// 	while()
//  Receive_485(0x10,0x00);
//  ¾øÔµµç×è¼ì²â£¬µÈ´ı²âÊÔ£¬timeout wait
// 	if(SystemFaultState != 0)
// 	{
// 		SystemState = 2;      //ÏµÍ³×´Ì¬£º0£¬¿ÕÏĞ×´Ì¬£»1£¬³äµç×´Ì¬ £»2£¬Òì³£×´Ì¬£»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»¸ù¾İÉèÖÃÌõ¼şÇĞ»»ÏµÍ³×´Ì¬
// 		Speech("[v11]ÏµÍ³Òì³£");
// 		}
	SystemState = 1;
	ChangePage(0x0001);  //ÏµÍ³×Ô¼ì½áÊø£¬ÆÁÄ»ÌøÈëÖ÷½çÃæ
	CheckOut();
	return 0;
}
uint8_t Collect_System_State_Process(void)//²É¼¯ÏµÍ³×´Ì¬×Ó³ÌĞò
{
	if(1 == FlagCycle500ms)
	{
		//²É¼¯¾øÔµµç×èºÍÈıÏàµçÈ±Ïà¼ì²â
		//ÅĞ¶Ï¾øÔµµç×èÒÔ¼°ÈıÏàµçÊÇ·ñºÏ¸ñ£¬²É¼¯ÔÚRS485ÖĞ½øĞĞ£¬500ms²É¼¯Ò»´Î
		FlagCycle500ms = 0;
	}
	//ÅĞ¶ÏDCµçÔ´×´Ì¬,ÉèÖÃDCµçÔ´¿ªÊ¼¹¤×÷±êÖ¾Î»
	if(1 == FlagCycle1s)//1S¼ì²âÒ»´Î ²ÖÃÅ¼±Í£·Å×²»÷
	{
		CabinDoorState = CheckCabinDoorState( );
		if(1== CabinDoorState )
		{
			SystemFaultState = SystemFaultState|0x0002;//²ÖÃÅÎ´¹Ø±Õ
		}
		EmergencyStopButtonState = CheckEmergencyStopButtonState( );
		if(1 == EmergencyStopButtonState)
		{
			SystemFaultState = SystemFaultState|0x0001;//¼±Í£°´Å¥±»°´ÏÂ
		}
		LevelState = CheckLevelState( );
		if(1 == LevelState)
		{
			SystemFaultState = SystemFaultState|0x0008;//ÏµÍ³±»×²»÷
		}
		FlagCycle1s = 0;
	}
	if(1 == FlagCycle10s)   //10S¼ì²âÒ»´Î  µçÔ´²ÖÎÂ¶È »·¾³ÎÂÊª¶È ±£´æ³äµç¹ı³Ì µÄÁÙÊ±Êı¾İ
	{
		DS18B20Temperature = DS18B20_Get_Temp();
		if(MAXDS18B20TEMP < DS18B20Temperature)
		{
			SystemFaultState = SystemFaultState|0x4000;//µçÔ´²ÖÎÂ¶È¹ı¸ß
		}
		Read_Sensor( );
		if(MAXAM2321TEMP < AM2321Temperature)
		{
			SystemFaultState = SystemFaultState|0x0010;//»·¾³ÎÂ¶È¹ı¸ß
		}
		if(MINAM2321TEMP > AM2321Temperature)
		{
			SystemFaultState = SystemFaultState|0x0020;//»·¾³ÎÂ¶È¹ıµÍ
		}
		if(MAXAM2321HUMI < Am2321Humidity)
		{
			SystemFaultState = SystemFaultState|0x0040;//»·¾³Êª¶È¹ı¸ß
		}
		if(MINAM2321HUMI > Am2321Humidity)
		{
			SystemFaultState = SystemFaultState|0x0080;//»·¾³Êª¶È¹ıµÍ
		}
		ETH_Send_HeartbeatMeg();
		FlagCycle10s = 0;
		//Speech("ĞÄÌø°üÉÏ´«");
		//±£´æÊı¾İ,FM´æ´¢Æ÷±£´æÊı¾İ?±£´æÄÄĞ©Êı¾İ
	}
	return 0;
}
void HMI_Event_Process(void)//´¦ÀíÆÁÄ»×Ó³ÌĞò
{
		TimeCountDownFlag=(uint16_t)((mainselect==1)||(mainselect==4)||(mainselect==3)||(mainselect==6)||(cardstartflag1==1)||(InsertPileFlag==1));	
		if(TimeCountDownFlag)                                   //µ¹¼ÆÊ±
	      Count_Down();
			
		size=queue_find_cmd(cmd_buffer,MAX_CMD_BUFFER);		      //½ÓÊÜÊı¾İ´¦Àí
	  if(size>0)	
	      Message_process();
	  
}
void ETH_Event_Process(void)//´¦ÀíÍøÂçÊı¾İ×Ó³ÌĞò
{ 
    IfConnect();
	  ethernetif_input(&gnetif);//ÓĞÊı¾İ°üÔò´¦Àí£¬·ñÔòÖ±½ÓÍË³ö
	  ETHData = ETH_GetData();
    AnalyzeData();
}  
void ETH_Send_HeartbeatMeg(void)    //ÉÏ´«ĞÄÌø°ü
{
	uint8_t sendbuffer[15];
	sendbuffer[0]=0xAA;
	sendbuffer[1]=0xAA;
	sendbuffer[2]=0x01;              //´Ó»úµØÖ·        
	sendbuffer[3]=0x00;             //×®ºÅ
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

void Free_State_Process(void)//¿ÕÏĞ×´Ì¬´¦Àí×Ó³ÌĞò
{
		PileState = CheckPileState();//ÅĞ¶ÏÇ¹ÊÇ·ñ±»°Îµô
		if(PileState)
		{
			CAN_mEvent();       //CANÊÂ¼ş´¦Àí
		}
		else
		{
			//Ç¹±»°Îµô£¬½øÈë½áËã½çÃæ
			//give flag 
		}
		//Ö±½ÓÔÚ½áÊø³äµç°´Å¥²¿·Ö½øĞĞ´Ë¹¤×÷
		if(1 == SpeechFlag)//ĞèÒªÓïÒô,½áËãÇëË¢¿¨,Ö»ËµÒ»´Î
		{
			Speech("[v11]½áÕËÇëË¢¿¨");
			SpeechFlag = 0;
		}
		if(1 == SwipingCardFlag)//½áÊø³äµç°´Å¥±»°´ÏÂ,Ò³Ãæ³¬Ê±£¬Ôò´Ë±êÖ¾Î»Çå³ı
		{
					if(1 == CardLock())//Èç¹û¶Á¿¨³É¹¦
					{
							//ÅĞ¶ÏÊÇ·ñÎªµ±Ç°³äµç¿¨
							if(1)//
							{
									//CANÍ¨ĞÅ½øÈë½áÊø³äµç´¦Àí²¿·Ö£¬ubCANEventStatus = £»
								//¹Ø±ÕÏà¹Ø¼ÌµçÆ÷
								ResetACRwlay( );//Í£Ö¹½»Á÷¼ÌµçÆ÷
								ResetDCRwlay( );//Í£Ö¹Ö±Á÷½Ó´¥Æ÷
								StopFan( );//Í£Ö¹·çÉÈ
								ShowPileChargeFullOrFree(0);//³äµç×®³äÂú»òÕß¿ÕÏĞ
								//Ïà¹Ø¼ÇÂ¼´æÅÌ£¬	
								//ÍøÂç·¢ËÍÏà¹ØÊı¾İ
								//ÏÔÊ¾ÆÁÏÔÊ¾µ±Ç°Ïû·ÑĞÅÏ¢ give flag to HMI
							}
							else
							{	
							//¸ø³öÏà¹Ø±êÖ¾Î»   ¶Á¿¨³É¹¦±êÖ¾Î»  Í¨ÖªÆÁÄ»³ÌĞòÏÔÊ¾¿¨ºÅºÍ¿¨ÄÚÓà¶î
							SwipingCardFlag = 0;
							SystemState = 1;//½øÈë³äµç×´Ì¬
							}
					}			
		}
}

void Charge_State_Process(void)//³äµç×´Ì¬´¦Àí×Ó³ÌĞò
{
	uint8_t i;
	uint8_t FlagICStatus = 0 ;
  /************************
  1.³äµç½çÃæ³äµçÄ£Ê½¼ÇÂ¼ 
	2.¸ù¾İ²åÇ¹ÅĞ¶ÏÊÇ·ñ½øÈëÌáÊ¾²åÇ¹Ò³Ãæ
	3.Ë¢¿¨½çÃæ
	4.³äµçĞÅÏ¢½çÃæ
******************************/
    //±äÁ¿
    uint32_t Energy;
    uint16_t DealEnergyLow;
	uint16_t crc=0xffff;
	//Á÷³Ì
	PileState = CheckPileState();
	//ÌáÊ¾²åÇ¹£¬ÓïÑÔ¸ù¾İSpeechFlagÖ»ËµÒ»´Î
	if((1 == SwipingPileFlag)&&(1 == SpeechFlag))
	{
		Speech("Çë²åÈë³äµç²å×ù");
		SpeechFlag = 0;
		OpenDoor();
    FlagOpenDoor = 1;           //´ò¿ª²ÖÃÅ
	}
  	//É¨Ç¹Í·
	if(1 == SwipingPileFlag)
	{	
			if(1==PileState)
				{
					ChangePage(0x001A);   //Ò³ÃæÇĞ»»ÖÁË¢¿¨½çÃæ
					SwipingCardFlag = 1; //½øĞĞÉ¨¿¨¶¯×÷
					SwipingPileFlag =0;    
					SpeechFlag = 1;			
					cardstartflag1=1;   
				}
	}
	//ÌáÊ¾Ë¢¿¨£¬ÓïÑÔ¸ù¾İSpeechFlagÖ»ËµÒ»´Î
	if((1 == SwipingCardFlag)&&(1 == SpeechFlag))
	{
		  Speech("ÇëË¢¿¨³äµç");
		  SpeechFlag = 0;
		  ICReadFlag = 0;
			ICWriteFlag = 0;
		  SetBMSRelay();//Æô¶¯¸¨ÖúµçÔ´
		  //¶ÁÉÏ´Î½»Ò×
		  HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSaveRealTimeDataHandle.unbuffer,73,0xff);
	}
	//É¨¿¨
	if(1 == SwipingCardFlag)
	{   
			if(0 == ICWriteFlag)
			{
					FlagICStatus = CardLock();	
			}		
	    if((1 == FlagICStatus)&&(1 == SpeechFlagDeal))       //ÉÏ´Î½»Ò×Î´Íê³É
			{
					SpeechFlagDeal = 0;
					Speech("ÉÏ´Î½»Ò×Î´Íê³É");
					UpDataId();     //ÉÏ´«¿¨ºÅÖÁÆÁÄ»
					UpDataBalance();    //ÉÏ´«Óà¶îÖÁÆÁÄ»
					TimeCountDownStartCard = 5;
					ChargeFlag = 0; 
			}
			else if((3 ==FlagICStatus)&&(1 == SpeechFlagMoney))           //¿¨ÄÚÓà¶î²»×ã
			{
					SpeechFlagMoney = 0;
					Speech("Óà¶î²»×ãÇëÁ¢¼´³äÖµ");
				  UpDataId();     //ÉÏ´«¿¨ºÅÖÁÆÁÄ»
		      UpDataBalance();    //ÉÏ´«Óà¶îÖÁÆÁÄ»
				  TimeCountDownStartCard = 5;
					ChargeFlag = 0; 
			}
			else if((0 ==FlagICStatus))//&&(1 == SpeechFlag))           //Ã»ÓĞ¶Áµ½¿¨ºÅ
			{
				   //Speech("Ë¢¿¨²»³É¹¦");
			}
			//else if((2 == FlagICStatus)&&(1 == ICWriteFlag))//Èç¹ûĞ´¿¨³É¹¦  //ÕıÈ·¶Á¿¨
			if(ICWriteFlag == 1)
				{
						Speech("¿ªÊ¼³äµç");
					  UpDataId();     //ÉÏ´«¿¨ºÅÖÁÆÁÄ»
		        UpDataBalance();    //ÉÏ´«Óà¶îÖÁÆÁÄ»
					  HAL_Delay(100);
				    WriteBlockFlag = 0;
					  ICWriteFlag = 0;
						ubDCModue = CAN_STARTDCMODUE;
						if(1 == FlagIfSaveToMemory)  //½«ÉÏ´ÎÎ´Íê³É½»Ò×Ğ´Èë´ó´æ´¢Æ÷
						{
								FlagIfSaveToMemory = 0;
								crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
							  WriteHisRec(unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo,unSaveRealTimeDataHandle.unbuffer,73);				
						}
						//±£´æÆğÊ¼³äµçÊ±¼ä
						StartChargeBCDTime.Second = RTCReadBCDTime.Second;    //Ãë
						StartChargeBCDTime.Minute = RTCReadBCDTime.Minute;   //·Ö
						StartChargeBCDTime.Hour = RTCReadBCDTime.Hour;       //Ê±
						StartChargeBCDTime.Day = RTCReadBCDTime.Day;         //ÈÕ
						StartChargeBCDTime.Month = RTCReadBCDTime.Month;     //ÔÂ
						StartChargeBCDTime.Year = RTCReadBCDTime.Year;       //Äê

						//¿ªÊ¼³äµç½×¶ÎĞè±£´æµÄÊµÊ±Êı¾İ 
						HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0xf0,I2C_MEMADD_SIZE_8BIT,StartEnergy,4,0xff);   //¶ÁÈ¡´æ´¢Æ÷µçÄÜ
						Energy = (ACEnergy[0]<<24) + (ACEnergy[1]<<16) + (ACEnergy[2]<<8) + ACEnergy[3] ;  //µç±í¶Á³öµ±Ç°Ê±¿ÌµçÄÜ
						StartEnergyAll = (StartEnergy[0]<<24) + (StartEnergy[1]<<16) + (StartEnergy[2]<<8) + StartEnergy[3] ;  //´æ´¢Æ÷´æ´¢ÉÏÒ»´Î½»Ò×½áÊøÊ±µÄµçÄÜ
						if(1 == FlagIfSaveData)   //ÒâÍâµ¼ÖÂÉÏ´Î½»Ò×Î´Íê³É
						{
							 FlagIfSaveData = 0;
							
							 StartEnergy[0] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy>>24)&0xff;
							 StartEnergy[1] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy>>16)&0xff;
							 StartEnergy[2] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy>>8)&0xff;
							 StartEnergy[3] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy)&0xff;
						}
						else              //ĞÂµÄÒ»´Î½»Ò×¼ÇÂ¼
						{   
							  StartICMoneyAll = (StartICMoney[0]<<24) + (StartICMoney[1]<<16) + (StartICMoney[2]<<8) + StartICMoney[3] ;
							  unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo = 1 + unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo; 
							  unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee = 0;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter = 0;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.PileNum = 0;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMode = ChargingMode ;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod = ChargingMethod;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo = (ICCard1[3]<<24)+(ICCard1[2]<<16)+(ICCard1[1]<<8)+ICCard1[0];     //µ±Ç°½»Ò×¿¨ºÅ
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney = StartICMoneyAll;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartYear = RTCReadBCDTime.Year;                                               //µ±Ç°½»Ò×ÆğÊ¼³äµçÊ±¼ä Äê
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMonth = RTCReadBCDTime.Month;                                             //µ±Ç°½»Ò×ÆğÊ¼³äµçÊ±¼ä  ÔÂ
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartDay = RTCReadBCDTime.Day;                                                  //µ±Ç°½»Ò×ÆğÊ¼³äµçÊ±¼ä  ÈÕ
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartWeek = RTCReadBCDTime.Week;                                               //µ±Ç°½»Ò×ÆğÊ¼³äµçÊ±¼ä  ĞÇÆÚ
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartHour = RTCReadBCDTime.Hour;                                                //µ±Ç°½»Ò×ÆğÊ¼³äµçÊ±¼ä  Ê±
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMin = RTCReadBCDTime.Minute;                                                //µ±Ç°½»Ò×ÆğÊ¼³äµçÊ±¼ä  ·Ö
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartSec = RTCReadBCDTime.Second;                                                //µ±Ç°½»Ò×ÆğÊ¼³äµçÊ±¼ä  Ãë
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargeFlag = 0xff; 
								if(Energy >= StartEnergyAll)    //µ±Ç°¶Á³öµçÄÜ´óÓÚµÈÓÚ´æ´¢Æ÷µçÄÜ °Ñµ±Ç°µçÄÜ×÷ÎªÆğÊ¼µçÄÜ
									{
											 for(i=0;i<4;i++)
											{
												StartEnergy[i] = ACEnergy[i];  //ğÊ¼µçÄÜ
												unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy = Energy;
											}
									}
									else            //µ±Ç°¶Áµ½µÄµçÄÜ´íÎó
									{
											for(i=0;i<4;i++)   //´æ´¢Æ÷Êı¾İ×÷ÎªÆğÊ¼µçÄÜ
												{
													 unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy = StartEnergyAll;
												}
									}
						}
						UpDataId();     //ÉÏ´«¿¨ºÅÖÁÆÁÄ»
						UpDataBalance();    //ÉÏ´«Óà¶îÖÁÆÁÄ»
						ChargeFlag = 1;    //ÕıÔÚ³äµç±êÖ¾
						SwipingCardFlag = 0;                      
						SetACRelay(); //Æô¶¯½»Á÷½Ó´¥Æ÷
						SetDCRelay(); //Æô¶¯Ö±Á÷½Ó´¥Æ÷
					  ShowPileChargeFullOrFree(0);  //ÂÌµÆ²»ÁÁ
						HAL_Delay(500);
						CAN_Config();
						//SetDCRelay(); //Æô¶¯Ö±Á÷½Ó´¥Æ÷
					  //ShowPileChargeFullOrFree(0);  //ÂÌµÆ²»ÁÁ
						//SetACRelay(); //Æô¶¯½»Á÷½Ó´¥Æ÷
						TimeCountDownStartCard = 5;
						if(1 == ChargingMethod)    //ÊÖ¶¯³äµç
						{
								//SetChargeCutOffCurrent  ;           //ÉèÖÃ½ØÖ¹µçÁ÷
								//SetChargeVoltage;                 //ÉèÖÃ³äµçµçÑ¹
							  //SetChargeCurrent;                 //ÉèÖÃ³äµçµç?	
						}
				}	
		}
		if(1 == ChargeFlag)
		{
					//if(DS18B20Temperature >=800)
			    if(1)
					{
							StartFan();//Æô¶¯·çÉÈ
					}
// 					else
// 					{
// 							StopFan();//Æô¶¯·çÉÈ
// 					}
					if(1 ==CheckPileState())
						{   		
										AccumulateChargeEnergy();    //¼ÆËãµçÄÜ
										AccumulateChargeMoney();     //¼ÆËã½ğ¶î
										CAN_mAccumulateChargeTime();   //¼ÆËã³äµçÊ±³¤
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney = StopMoneyAll ;            //½»Ò×ºóÓà¶î
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealPrice = unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice; //½»Ò×µç¼Û
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy = (DealEnergy[2]<<8) + DealEnergy[3];        //½»Ò×½áÊøÊ±¼ä£¨Ãë£©
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear = RTCReadBCDTime.Year;      //½»Ò×½áÊøÊ±¼ä£¨Äê£©
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth = RTCReadBCDTime.Month;     //½»Ò×½áÊøÊ±¼ä£¨ÔÂ£©
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay = RTCReadBCDTime.Day;         //½»Ò×½áÊøÊ±¼ä£¨ÈÕ£©
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek = RTCReadBCDTime.Week;         //½»Ò×½áÊøÊ±¼ä£¨ĞÇÆÚ£©
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour = RTCReadBCDTime.Hour;        //½»Ò×½áÊøÊ±¼ä£¨Ê±£©
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin = RTCReadBCDTime.Minute;       //½»Ò×½áÊøÊ±¼ä£¨·Ö£©
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec = RTCReadBCDTime.Second;        //½»Ò×½áÊøÊ±¼ä£¨Ãë£©
										crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
										if(FlagCycle30s == 1)  //30s´æ´¢Êı¾İÊµÊ±´æ´¢
										{
											HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSaveRealTimeDataHandle.unbuffer,73,0xff);
											//HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x63,I2C_MEMADD_SIZE_8BIT,"ff",1,0xff);
											//ETH_Send_HeartbeatMeg();
											FlagCycle30s = 0;
										}					
									    //¼ÆËã³äµç¹ı³ÌÖĞ¿¨ÄÚÓà¶î²»×ã
										if(DealMoneyAll >= (StartICMoneyAll + (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice/100)))
										{
											ResetACRwlay();//Í£Ö¹½»Á÷¼ÌµçÆ÷
											ResetDCRwlay();//Í£Ö¹Ö±Á÷½Ó´¥Æ÷ 
                      ShowPileChargeFullOrFree(1);  //ÂÌµÆÁÁ											
										}
										if(0 == ChargingMethod)     //³äµç·½Ê½ 0Îª×Ô¶¯³äµç£¬1ÎªÊÖ¶¯³äµç
										{	
													CAN_mEvent();       //CANÊÂ¼ş´¦Àí
													if(0 ==ChargingMode)   //×Ô¶¯³äÂú
														{
															
														}
													else  
														if(1 ==ChargingMode)    //°´µçÁ¿³ä
														{
																//³äµçµçÄÜ´ïµ½Éè¶¨µçÄÜ SetKwh<=DealEnergy
																DealEnergyLow = (DealEnergy[2]<<8) + DealEnergy[3];
																if(SetKwh <= DealEnergyLow)   //Éè¶¨µçÄÜĞ¡ÓÚ³äµçµçÄÜ ¹Ø±Õ¼ÌµçÆ÷
																{
																	 ubDCModue = CAN_STOPDCMODUE;
																	 CAN_mSendDCModuleStartOrStop(0x00);
																	 ResetACRwlay();//Í£Ö¹½»Á÷¼ÌµçÆ÷
																	 ResetDCRwlay();//Í£Ö¹Ö±Á÷½Ó´¥Æ÷
																	 ShowPileChargeFullOrFree(1);  //ÂÌµÆÁÁ
																	 //StopChargeFlag = 1;   //Í£Ö¹³äµç±êÖ¾Î»
																}
																else
																{
															
																}
														}
													else  
														if(2 ==ChargingMode)    //°´½ğ¶î³ä
														{
																//Éè¶¨½ğ¶î´ïµ½ÒÑ³äµç½ğ¶îSetMoney<=DealMoneyAll
																if(SetMoney <= (DealMoneyAll&0xffff))  //ÒÑ´ïµ½³äµç½ğ¶î  ¹Ø±Õ¼ÌµçÆ÷Í£Ö¹³äµç
																{
																	 ubDCModue = CAN_STOPDCMODUE;
																	 CAN_mSendDCModuleStartOrStop(0x00);
																	 ResetACRwlay();//Í£Ö¹½»Á÷¼ÌµçÆ÷
																	 ResetDCRwlay();//Í£Ö¹Ö±Á÷½Ó´¥Æ÷
																	 ShowPileChargeFullOrFree(1);  //ÂÌµÆÁÁ
																	 //StopChargeFlag = 1;   //Í£Ö¹³äµç±êÖ¾Î»
																}
																else    
																{
																
																}
														}
													else  
														if(3 ==ChargingMode)     //°´Ê±¼ä³ä
														{
															if((SetHour <= ChargeTime.Hour)&&(SetMin <= ChargeTime.Minute))  //³äµçÊ±³¤µ½´ïÉè¶¨Öµ
															{
															   ubDCModue = CAN_STOPDCMODUE;
															   CAN_mSendDCModuleStartOrStop(0x00);
															   ResetACRwlay();//Í£Ö¹½»Á÷¼ÌµçÆ÷
															   ResetDCRwlay();//Í£Ö¹Ö±Á÷½Ó´¥Æ÷
															   ShowPileChargeFullOrFree(1);  //ÂÌµÆÁÁ
															   //StopChargeFlag = 1;   //Í£Ö¹³äµç±êÖ¾Î»
															}
															else
															{
																
															}
														}
										 }	
										 else	
										 if(1 == ChargingMethod)        //ÊÖ¶¯³äµç£¬³äµçµçÑ¹³äµçµçÁ÷½ØÖ¹µçÁ÷
										 {
													//SetChargeCutOffCurrent;           //ÉèÖÃ½ØÖ¹µçÁ÷
													//SetChargeVoltage;                 //ÉèÖÃ³äµçµçÑ¹
													//SetChargeCurrent;                 //ÉèÖÃ³äµçµç?
													if((10*SetChargeCutOffCurrent) >= uwOutputCurrentValue)        //ÅĞ¶Ï³äµçµçÁ÷ºÍ½ØÖ¹µçÁ÷µÄ´óĞ¡¹ØÏµ
											    {
																 ubDCModue = CAN_STOPDCMODUE;
															   CAN_mSendDCModuleStartOrStop(0x00);
													}
										 } 
												
						}
						else    //·ÀÖ¹°ÎÇ¹
						{
										StopChargeFlag = 1;   //Í£Ö¹³äµç±êÖ¾Î»
										ResetACRwlay();//Í£Ö¹½»Á÷¼ÌµçÆ÷
										ResetDCRwlay();//Í£Ö¹Ö±Á÷½Ó´¥Æ÷
										ResetBMSRwlay();//Í£Ö¹¸¨ÖúµçÔ´
							 		  ShowPileChargeFullOrFree(1);  //ÂÌµÆÁÁ
										FaultOperation = 1;  //´íÎó²Ù×÷±êÖ¾Î»
						}
		}
		if((1 == StopChargeFlag)&&(1==SpeechFlag)&&(1 == ChargeFlag))   //Ë¢¿¨½áÊø³äµç
		{
						SpeechFlag = 0;
						ICReadFlag = 0;
						Speech("Ë¢¿¨½áÊø³äµç");
						ICWriteFlag = 0;
						//FlagIfOperationCard = 1;
		}
		if(((1 == StopChargeFlag)&&(1 == ChargeFlag))||(1 == FaultOperation))   //½áÊø³äµç
		{
					//if((1 == CardRead())||(1 == FaultOperation))
			    if(0 == ICWriteFlag)
					{
							FlagICStatus = CardunLock();	
					}
					//if((2 == FlagICStatus)||(1 == FaultOperation))    //Ë¢¿¨³É¹¦ £¬´íÎó²Ù×÷Ö±½Ó²»ĞèÒªË¢¿¨½áÊø³äµç
					if((1== ICWriteFlag)||(1 == FaultOperation))    //Ë¢¿¨³É¹¦ £¬´íÎó²Ù×÷Ö±½Ó²»ĞèÒªË¢¿¨½áÊø³äµç
					{
									FlagShowInformation = 1;
									ubDCModue = CAN_STOPDCMODUE;
					      	CAN_mSendDCModuleStartOrStop(0x00);
									HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0xf0,I2C_MEMADD_SIZE_8BIT,ACEnergy,4,0xff);  //½áÊø³äµçµçÄÜĞ´Èë´æ´¢Æ÷ 
									Speech("±¾´Î½»Ò×Íê³É£¬Ğ»Ğ»Ê¹ÓÃ");
						      StopFan();//Æô¶¯·çÉÈ
									//DataInit(); 
									ChargeFlag = 0;
									StopChargeFlag = 0;
									ChargingMode = 0;
									ChargingMethod = 0;
									ResetACRwlay();//Í£Ö¹½»Á÷¼ÌµçÆ÷
									ResetDCRwlay();//Í£Ö¹Ö±Á÷½Ó´¥Æ÷
						      ShowPileChargeFullOrFree(1);  //ÂÌµÆÁÁ
					      	ubCANEventStatus = 16;
									CAN_mSend_CST_ChargerStop();
									//ResetBMSRwlay();//Í£Ö¹¸¨ÖúµçÔ´
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney = StopMoneyAll ;            //½»Ò×ºóÓà¶î
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealPrice = unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice; //½»Ò×µç¼Û
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy = (DealEnergy[2]<<8) + DealEnergy[3];        //½»Ò×½áÊøÊ±¼ä£¨Ãë£©
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStopEnergy = (ACEnergy[0]<<24) + (ACEnergy[1]<<16) + (ACEnergy[2]<<8) + ACEnergy[3];
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear = RTCReadBCDTime.Year;      //½»Ò×½áÊøÊ±¼ä£¨Äê£©
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth = RTCReadBCDTime.Month;     //½»Ò×½áÊøÊ±¼ä£¨ÔÂ£©
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay = RTCReadBCDTime.Day;         //½»Ò×½áÊøÊ±¼ä£¨ÈÕ£©
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek = RTCReadBCDTime.Week;         //½»Ò×½áÊøÊ±¼ä£¨ĞÇÆÚ£©
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour = RTCReadBCDTime.Hour;        //½»Ò×½áÊøÊ±¼ä£¨Ê±£©
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin = RTCReadBCDTime.Minute;       //½»Ò×½áÊøÊ±¼ä£¨·Ö£©
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec = RTCReadBCDTime.Second;        //½»Ò×½áÊøÊ±¼ä£¨Ãë£©
									if(1 == ICWriteFlag)     //ÕıÈ·²Ù×÷0x00
									{
										  ICWriteFlag = 0;
										  unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargeFlag = 0x00; 
											//HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x63,I2C_MEMADD_SIZE_8BIT,0x00,1,0xff);	
                      //Ğ´ÖÁ´ó´æ´¢Æ÷
											crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
										  HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSaveRealTimeDataHandle.unbuffer,73,0xff);
                      WriteHisRec(unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo,unSaveRealTimeDataHandle.unbuffer,73);										
									}										
									else if(1 == FaultOperation)   //´íÎó²Ù×÷0xff
									{
											FaultOperation = 0;
										  //HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x63,I2C_MEMADD_SIZE_8BIT,"ff",1,0xff);
									//}
											crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
											//½áÊø³äµçÊ±Ğ´Êı¾İÖÁ´æ´¢Æ÷
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
						ResetBMSRwlay();//Í£Ö¹¸¨ÖúµçÔ´
						Clr_Data();   //³äµç¹ı³ÌÊı¾İÇåÁã Ö÷ÒªÊÇÆÁÄ»ÉÏÏÔÊ¾µÄÊı¾İ
						CAN_Config();  //CAN ³õÊ¼»¯
						CAN_mInitData();   //CANÊı¾İ³õÊ¼»¯
						ChangePage(0x0001);  //ÆÁÄ»ÌøÈëÖ÷½çÃæ
				}
		}
}
void Error_State_Process(void)//Òì³£´¦Àí×Ó³ÌĞò
{
	ShowPileFaultState(1);//Òì³£µÆÁÁ
	//ÆÁÄ»ÏÔÊ¾ÏµÍ³Òì³£flag
	//ÍøÂçÍ¨ĞÅ£¬Í¨Öª·şÎñÆ÷£¬ÏµÍ³Òì³£
}
