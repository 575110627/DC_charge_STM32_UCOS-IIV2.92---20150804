#ifndef __DEFINE_H
#define __DEFINE_H

#include "stm32f2xx_hal.h"

/***********************************************
**ÈÎÎñÓÅÏÈ¼¶ÉèÖÃ
***********************************************/
#define  Falg_SUCCEED_PRIO_MEM                1 //±êÖ¾¼Ì³ĞÓÅÏÈ¼¶
#define  Falg_SUCCEED_PRIO                    2 //±êÖ¾¼Ì³ĞÓÅÏÈ¼¶
#define  SYSMonitorTask_PRIO                  3
#define  LED1Task_PRIO                        4
#define  LED2Task_PRIO                        5
#define  LED3Task_PRIO		                  6



/***********************************************
**ÈÎÎñ¶ÑÕ»³¤¶ÈÉèÖÃ
***********************************************/
#define SYSMonitorTaskStackLengh              128
#define LED1TaskStackLengh                  128
#define LED2TaskStackLengh				          128
#define LED3TaskStackLengh    			128

extern 	uint8_t ReadBuffer[100];
//CANÍ¨ĞÅ±¨ÎÄÊÂ¼şµÄ×´Ì¬»ú
extern uint8_t  ubCANEventStatus;
/* CANÍ¨ĞÅÊı¾İÖ¡¶¨Òå  */
/*----------------------------------------------
-----------------ÎÕÊÖ½×¶Î¶¨Òå------------------
-----------------------------------------------*/
 //±æÊ¶½á¹û£¬ÎïÀíÁ¬½ÓÖ®ºó£¬³äµç»úÏòBMS·¢ËÍ´Ë±¨ÎÄ£¬ÔÚÊÕµ½BMS·¢ËÍµÄ±æÊ¶±¨ÎÄÖ®Ç°·¢ËÍ0x00£¬ÔÚÊÕµ½BMS±æÊ¶±¨ÎÄÖ®ºó·¢ËÍ0xAA
extern uint8_t ubRecognitionResult;	
//³äµç»ú±àºÅ£¬1/Î»£¬1Æ«ÒÆÁ¿£¬Êı¾İ·¶Î§1-100	
extern uint8_t ubChargerNumber;

/*------------------------------------------------
------------PGN512,BRM±¨ÎÄ²ÎÊı--------------------
-------------------------------------------------*/
//BMSÍ¨ĞÅĞ­Òé°æ±¾ºÅ£¬±¾±ê×¼¹æ¶¨µ±Ç°°æ±¾ÎªV1.0 ±íÊ¾Îª£ºbyte3 byte2 byte1  0x 00 01 00
extern uint32_t ulBmsProtocolVersion;
//µç³ØÀàĞÍ£º01H£ºÇ¦Ëáµç³Ø 02H£ºÄøÇâµç³Ø	03£ºÁ×ËáÌúï®µç³Ø 04H£ºÃÌËáï®µç³Ø 05H£ºîÜËáï®µç³Ø 06H£ºÈıÔª²ÄÁÏµç³Ø 07H£º¾ÛºÏÎïï®Àë×Óµç³Ø 08H£ºîÑËáï®µç³Ø FFH£ºÆäËûµç³Ø
extern uint8_t ubBatteryType;
//Õû³µ¶¯Á¦Ğîµç³ØÏµÍ³¶î¶¨ÈİÁ¿/A*h,0.1A*h/Î»£¬0.1A*hÆ«ÒÆÁ¿ Êı¾İ·¶Î§ :0-¡ª¡ª1000A.h
extern uint16_t uwBmsRateCapacity;
//Õû³µ¶¯Á¦Ğîµç³ØÏµÍ³¶î¶¨×ÜµçÑ¹ /V £¬O.l V/Î» £¬0 V Æ«ÒÆÁ¿£¬Êı¾İ·¶ Î§:0  750 V
extern uint16_t uwBatteryRateVoltage;
//µç³ØÉú²ú³§ÉÌÃû³Æ£¬±ê×¼ ASCII Âë
extern uint8_t ubBatteryManufacturerName[4];
//µç³Ø×éĞòºÅ£¬Ô¤Áô£¬ÓÉ³§ÉÌ×ÔĞĞ¶¨Òå
extern uint16_t ulBatteryManufacturerName;
//µç³Ø×éÉú²úÈÕÆÚ£¬Äê 1985Æ«ÒÆÎ»
extern uint8_t ubBatteryProduceYear;
//µç³Ø×éÉú²úÈÕÆÚ£¬ÔÂ
extern uint8_t ubBatteryProduceMonth;
//µç³Ø×éÉú²úÈÕÆÚ£¬ÈÕ
extern uint8_t ubBatteryProduceDay;
//µç³Ø×é³äµç´ÎÊı£¬1´ÎÃ¿Î»  ,3¸ö×Ö½Ú
extern uint16_t ulBatteryChargeCycle;
//µç³Ø×é²úÈ¨±êÖ¾  0±íÊ¾×âÁŞ   1±íÊ¾ ³µ×ÔÓĞ
extern uint8_t ubPropertyMark;
//Ô¤Áô
extern uint8_t ubBMSReserved;
//³µÁ¾Ê¶±ğÂëVehicleIdentificationNumber
extern uint8_t ubVIN[17];

/*-----------------------------------------------
-------------------²ÎÊıÅäÖÃ½×¶Î±¨ÎÄ--------------
-------------------------------------------------*/


/*-----------------------------------------------
-------------------PGN1536 BCP±¨ÎÄ²ÎÊı-------------
-------------------------------------------------*/
//µ¥Ìå¶¯Á¦Ğîµç³Ø×î¸ßÔÊĞí³äµçµçÑ¹  Êı¾İ·Ö±æÂÊ :0.01 V/Î»£¬ 0 V Æ«ÒÆÁ¿ E Êı¾İ·¶Î§ :0 V~24V;   
extern uint16_t uwCellMaxAllowChargeVoltage;
//×î¸ßÔÊĞí³äµçµçÁ÷	 Êı¾İ·Ö±æÂÊ :0.lA/Î»£¬ -400 A Æ«ÒÆÁ¿;Êı¾İ·¶Î§: -400 A ~O A;
extern  uint16_t uwMaxAllowChargeCurrent;
//¶¯Á¦Ğîµç³Ø±ê³Æ×ÜÄÜÁ¿	  Êı¾İ·Ö±æÂÊ :0.1 kW  o h/ Î»£¬ 0 kW. h Æ«ÒÆÁ¿;Êı¾İ·¶Î§ :0£¬.....£¬1 000  kW o h; 
extern uint16_t uwBatteryTotalEnergy;
//×î¸ßÔÊĞí³äµç×ÜµçÑ¹   Êı¾İ·Ö±æÂÊ :0.1 V/ Î»£¬ 0  V Æ«ÒÆÁ¿;Êı¾İ·¶Î§ :0 V £¬......750  V;
extern uint16_t uwMaxAllowChargeVoltage;
//×î¸ßÔÊĞíÎÂ¶È		 Êı¾İ·Ö±æÂÊ :1 oC/Î»£¬-50 OC Æ«ÒÆÁ¿ F Êı¾İ·¶Î§:Ò» 50 oC £¬.....£¬ +200 oC;
extern uint8_t ubMaxAllowTemperature;
//Õû³µ¶¯Á¦Ğîµç³ØºÉµç×´Ì¬	Êı¾İ·Ö±æÂÊ :0.1%/Î»£¬ 0%Æ«ÒÆÁ¿;Êı¾İ·¶Î§ :0£¬.....£¬100% ; 
extern uint16_t uwBmsSOC;
//Õû³µ¶¯Á¦Ğîµç³Ø×ÜµçÑ¹
extern uint16_t uwBmsTotalVoltage;
//---------PGN2048 CML±¨ÎÄ-------------
//DC×î¸ßÊä³öµçÑ¹
extern uint16_t uwDCMaxOutputVoltage;
//DC×îµÍÊä³öµçÑ¹
extern uint16_t uwDCMinOutputVoltage;
//DC×î´óÊä³öµçÁ÷
extern uint16_t uwDCMaxOutputCurrent;
//BMS ÊÇ·ñ³äµç×¼±¸ºÃ(< OxOO-) := BMS Î´×öºÃ³äµç×¼±¸; (OxAA):=BMS Íê³É³äµç×¼±¸; (OxFF):=ÔªĞ§)
extern uint8_t ubBmsChargeReady;
//³äµç»úÊÇ·ñ³äµç×¼±¸ºÃ ((OxOO):=³äµç»úÎ´Íê³É³äµç×¼±¸; (OxAA):=³äµç»úÍê³É³äµç×¼±¸; (OxFF):=ÎŞĞ§)
extern  uint8_t ubChargerOutputReady;

/*³äµç½×¶Î±¨ÎÄ*/
//µçÑ¹ĞèÇó(V)
extern uint16_t uwVoltageRequirement;
extern uint8_t uwVoltageRequirementLow;
extern uint8_t uwVoltageRequirementHigh;
//µçÁ÷ĞèÇó (A)
extern int16_t uwCurrentRequirement;
extern int8_t uwCurrentRequirementLow;
extern int8_t uwCurrentRequirementHigh;
extern int8_t uwCurrentRequirementLowbuf;
extern int8_t uwCurrentRequirementHighbuf;
extern int16_t uwCurrentRequirementbuf;
extern uint16_t CURRENT;
//³äµçÄ£Ê½ (Ox01:ºãÑ¹³äµç	;Ox02:ºãÁ÷³äµç)
extern uint8_t ubChargeMode;
//³äµçµçÑ¹²âÁ¿Öµ (V)
extern  uint16_t uwChargeVoltageMeasurement;
//³äµçµçÁ÷²âÁ¿Öµ (A)
extern uint16_t uwChargeCurrentMeasurement;
//×î¸ßµ¥Ìå¶¯Á¦Ğîµç³ØµçÑ¹¼°Æä×éºÅ
extern uint16_t uwHighestCellVoltageAndGroupNumber;
//µ±Ç°ºÉµç×´Ì¬ SOC(%)
extern  uint8_t ubCurrentSOC;
//¹ÀËãÊ£Óà³äµçÊ±¼ä (min)
extern  uint16_t uwRemainChargeTime;
//µçÑ¹Êä³öÖµ (V)
extern  uint32_t uwOutputVoltageValue;
//µçÁ÷Êä³öÖµ (A)
extern uint32_t uwOutputCurrentValue;
//ÀÛ¼Æ³äµçÊ±¼ä (min)
extern uint16_t uwAccumulateChargeTime;
//×î¸ßµ¥Ìå¶¯Á¦Ğîµç³ØµçÑ¹ËùÔÚ±àºÅ
extern uint8_t ubMaxCellVoltageNumber;
//×î¸ß¶¯Á¦Ğîµç³ØÎÂ¶È
extern uint8_t ubMaxTemperature;
//×î¸ßÎÂ¶È¼ì²âµã±àºÅ
extern uint8_t ubMaxTemperatureNumber;
//×îµÍ¶¯Á¦Ğîµç³ØÎÂ¶È
extern uint8_t ubMinTemperature;
//×îµÍ¶¯Á¦Ğîµç³ØÎÂ¶È¼ì²âµã±àºÅ
extern uint8_t ubMinTemperatureNumber;
//Òì³£×´Ì¬ĞÅÏ¢	 
//µ¥Ìå¶¯Á¦Ğîµç³ØµçÑ¹¹ı¸ß/¹ıµÍ ((00): =Õı³£ ;{01):= ¹ı¸ß;(10):=¹ıµÍ)
//Õû³µ¶¯Á¦Ğîµç³ØºÉµç×´Ì¬ soc ¹ı¸ß/¹ıµÍ ((00): =Õı³£ ;(01):= ¹ı¸ß;(10):= ¹ıµÍ¡µ
//¶¯Á¦Ğîµç³Ø³äµç¹ıµçÁ÷ ((00):= Õı³£ ;(01):== ¹ıÁ÷;(10): =²»¿ÉĞÅ ×´Ì¬¡µ
//¶¯Á¦Ğîµç³ØÎÂ¶È¹ı¸ß ((00): =Õı³£; (01): =:¹ı¸ß;(1 0):= ²»¿ÉĞÅ ×´Ì¬
//¶¯Á¦Ğîµç³Ø¾øÔµ×´Ì¬ ((00): =Õı³£ ;(01):=²»Õı³£;(10):= ²»¿ÉĞÅ ×´Ì¬¡µ
//¶¯Á¦Ğîµç³Ø×éÊä³öÁ¬½ÓÆ÷Á¬½Ó×´Ì¬ ((00):= Õı³£; (01): =²»Õı³£;(10): =²»¿ÉĞÅ×´Ì¬¡µ
//³äµçÔÊĞí ((00):= ½ûÖ¹ ;(01):=ÔÊĞí¡µ
extern uint16_t uwStatusAndAbnormal;
//µ¥Ìå¶¯Á¦Ğîµç³ØµçÑ¹
extern uint16_t uwCellVoltage[256];
//µ¥Ìå¶¯Á¦Ğîµç³ØÎÂ¶È
extern uint8_t ubCellTemperature[128];
//µ¥Ìå¶¯Á¦Ğîµç³ØÔ¤Áô±¨ÎÄ
extern uint8_t ubCellReserved[16];
//BMS  ÖĞÖ¹³äµçÔ­Òò
extern uint8_t ubBmsChargeStopReason;
//BMS  ÖĞÖ¹³äµç¹ÊÕÏÔ­Òò
extern uint16_t uwBmsChargeStopFaultReason;
//BMS  ÖĞÖ¹³äµç´íÎóÔ­Òò
extern uint8_t ubBmsChargeStopErrorReason;
//³äµç»úÖĞÖ¹³äµçÔ­Òò
extern uint8_t ubChargerStopReason;
//¿Ëµç»úÖĞÖ¹³äµç¹ÊÕÏÔ­Òò
extern uint16_t uwChargerStopFaultReason;
//³äµç»úÖĞÖ¹³äµç´íÎóÔ­Òò
extern uint8_t ubChargerStopErrorReason;
//ÖĞÖ¹ºÉµç×´Ì¬ SOC(%)
extern uint8_t ubBmsEndOfSOC;
//¶¯Á¦Ğîµç³Øµ¥Ìå×îµÍµçÑ¹ (V)
extern  uint16_t uwBmsEndMinCellVoltage;
//¶¯Á¦Ğîµç³Øµ¥Ìå×î¸ßµçÑ¹ (V)
extern uint16_t uwBmsEndMaxCellVoltage;
//¶¯Á¦Ğîµç³Ø×îµÍÎÂ¶È CC)
extern uint8_t ubBmsEndMinTemperature;
//¶¯Á¦Ğîµç³Ø×î¸ßÎÂ¶È CC)
extern uint8_t ubBmsEndMaxTemperature;
//ÀÛ¼Æ³äµçÊ±¼ä (min)
extern uint16_t uwChargerEndChargeTime;
//Êä³öÄÜÁ¿ (kW. h)
extern uint16_t uwChargerEndChargePower;
//BMS ´íÎó±¨ÎÄ (BEM)
extern  uint8_t ubBmsError[4];
//³äµç»ú´íÎó±¨ÎÄ (CEM)
extern  uint8_t ubChargerError[4];


//DCÖ±Á÷³äµçÄ£¿é×´Ì¬
//extern uint8_t ubDCModuleStatusHigh;
//extern uint8_t ubDCModuleStatusLow;

extern uint8_t ubDCModuleStatusHigh[4];
extern uint8_t ubDCModuleStatusLow[4];
// DCµçÑ¹Êä³ö
extern uint16_t uwDCOutputVoltage[4];
//DCµçÁ÷Êä³ö
extern uint16_t uwDCOutputCurrent[4];
// BMSĞèÇó³äµç·½Ê½»º´æÖµ (Ox01:ºãÑ¹³äµç	;Ox02:ºãÁ÷³äµç)
extern uint8_t ubChargeModeBuf;

//DCÖ±Á÷³äµçÄ£¿é×´Ì¬
extern uint8_t ubDCModuleStatusHighAll; 
                                 //µÚ0Î» 0±íÊ¾Ä£¿éÔËĞĞ 1±íÊ¾¹Ø»ú 
                                 //µÚ1Î» 0±íÊ¾Ä£¿éÕı³£ 1±íÊ¾¹ÊÕÏ 
                                 //µÚ2Î» 0±íÊ¾Ä£¿éºãÑ¹ 1±íÊ¾ÏŞÁ÷
                                 //µÚ3Î» 0±íÊ¾·çÉÈÕı³£ 1±íÊ¾¹ÊÕÏ  
                                 //µÚ4Î» 0±íÊ¾ÊäÈëÕı³£ 1±íÊ¾¹ıÑ¹  
                                 //µÚ5Î» 0±íÊ¾ÊäÈëÕı³£ 1±íÊ¾Ç·Ñ¹  
                                 //µÚ6Î» 0±íÊ¾Êä³öÕı³£ 1±íÊ¾¹ıÑ¹  
                                 //µÚ7Î» 0±íÊ¾Êä³öÕı³£ 1±íÊ¾Ç·Ñ¹                                
extern uint8_t ubDCModuleStatusLowAll; 
                                 //µÚ0Î» 0±íÊ¾¹ıÁ÷±£»¤ 1±íÊ¾Õı³£ 
                                 //µÚ1Î» 0±íÊ¾¹ıÎÂ±£»¤ 1±íÊ¾Õı³£ 
                                 //µÚ2Î» 0±íÊ¾ÉèÖÃ¹Ø»ú 1±íÊ¾¿ª»ú
                              
//DCÖ±Á÷³äµçÄ£¿é×´Ì¬
extern uint16_t uwDCModuleStatus;



//±êÖ¾£¬ÊÇ·ñ³¬Ê±5s 0x00²»Âú×ã£¬0xAAÂú×ã
extern  uint8_t FlagIfTimeout;
//±êÖ¾£¬DCµçÔ´Ä£¿éÊÇ·ñ³¬Ê±5s 0x00²»Âú×ã£¬0xAAÂú×ã
extern uint8_t FlagIfDCTimeout;
//³¬Ê±Ê±¼äÉèÖÃ£¬ubTimeNs±íÊ¾¶àÉÙ¸ö100ms
extern uint8_t ubTimeN100ms;


//±êÖ¾£¬ÊÇ·ñÂú×ãÖÜÆÚ·¢ËÍ ·¢ËÍÖÜÆÚ10ms 0²»Âú×ã£¬1Âú×ã
extern uint8_t FlagIfCycleSend10ms;
//±êÖ¾£¬ÊÇ·ñÂú×ãÖÜÆÚ·¢ËÍ ·¢ËÍÖÜÆÚ50ms 0²»Âú×ã£¬1Âú×ã
extern uint8_t FlagIfCycleSend50ms;
//±êÖ¾£¬ÊÇ·ñÂú×ãÖÜÆÚ·¢ËÍ ·¢ËÍÖÜÆÚ250ms 0²»Âú×ã£¬1Âú×ã
extern uint8_t FlagIfCycleSend250ms;
//±êÖ¾£¬ÊÇ·ñÂú×ãÖÜÆÚ·¢ËÍ ·¢ËÍÖÜÆÚ500ms 0²»Âú×ã£¬1Âú×ã
extern uint8_t FlagIfCycleSend500ms;
//±êÖ¾£¬ÊÇ·ñÂú×ãÖÜÆÚ·¢ËÍ ·¢ËÍÖÜÆÚ1000ms 0²»Âú×ã£¬1Âú×ã
extern uint8_t FlagIfCycleSend1000ms;

//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄBMSºÍ³µÁ¾±æÊ¶±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
extern uint8_t ubFlagIfReceBmsIdentifyMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄµç³Ø³äµç²ÎÊı±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
extern uint8_t ubFlagIfReceBatteryParameterMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄµç³Ø³äµç×¼±¸¾ÍĞ÷×´Ì¬±¨ÎÄ 0Î´ÊÕµ½£¬1ÊÕµ½
extern uint8_t FlagIfReceBmsReadyMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄµç³Ø³äµç×Ü×´Ì¬±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
extern uint8_t ubFlagIfReceBmsAllChargeStateMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄµç³Ø³äµçĞèÇó±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
extern uint8_t FlagIfReceBmsChargeRequirementMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄ¶¯Á¦Ğîµç³Ø×´Ì¬ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
extern uint8_t FlagIfReceBmsBatteryStateMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍ½áÊø³äµç»òÕßBMSÖĞÖ¹³äµç±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
extern uint8_t ubFlagIfReceBmsEndAndStopChargeMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄBMSÖĞÖ¹³äµç±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
extern uint8_t FlagIfReceBmsStopChargeMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½BMS·¢ËÍµÄ³äµçÍ³¼ÆÊı¾İ±¨ÎÄ 0x00Î´ÊÕµ½£¬0xAAÊÕµ½
extern uint8_t FlagIfReceBmsStatisticsDataMeg;


//±êÖ¾£¬ÊÇ·ñ·¢ËÍ±¨ÎÄ¸øDCÖ±Á÷³äµç»úÄ£¿é 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
extern uint8_t FlagIfSendDCModuleMeg;
//±êÖ¾£¬ÊÇ·ñÊÕµ½DCÖ±Á÷³äµç»úÄ£¿é·¢ËÍµÄ±¨ÎÄ 0Ã»ÊÕµ½£¬1ÊÕµ½
extern uint8_t FlagIfReceDCModuleMeg;

//±êÖ¾£¬ÊÇ·ñ·¢ËÍ³äµç»ú±æÊ¶±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
extern uint8_t ubFlagIfSendChargerIdentifyMeg;
//±êÖ¾£¬ÊÇ·ñ·¢ËÍ³äµç»úÊ±¼äÍ¬²½ĞÅÏ¢±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
extern uint8_t ubFlagIfSendChargerTimeMeg;
//±êÖ¾£¬ÊÇ·ñ·¢ËÍ×î´óÊä³öÄÜÁ¦±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
extern uint8_t ubFlagIfSendChargerMaxOutputCapactityMeg;
//±êÖ¾£¬ÊÇ·ñ·¢ËÍ³äµç»úÊä³ö¾ÍĞ÷×´Ì¬±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
extern uint8_t ubFlagIfSendChargerOutputReadyMeg;
//±êÖ¾£¬ÊÇ·ñ·¢ËÍ³äµç»ú³äµç×´Ì¬±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
extern uint8_t FlagIfSendChargerChargingStateMeg;
//±êÖ¾£¬ÊÇ·ñ³äµç»úÖĞÖ¹³äµç±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
extern uint8_t ubFlagIfSendChargerStopChargeMeg;
//±êÖ¾£¬ÊÇ·ñ·¢ËÍ³äµç»ú³äµçÍ³¼ÆÊı¾İ±¨ÎÄ 0x00²»·¢ËÍ£¬0xAA·¢ËÍ
extern uint8_t ubFlagIfSendChargerStatisticsDataMeg;

//¶¨Ê±Æ÷6¼ÆÊı£¬10ms¼ÆÊıÒ»´Î   
extern uint8_t ubCANTim3Count;
//¶¨Ê±Æ÷6¼ÆÊı£¬10ms¼ÆÊıÒ»´Î,CANÍ¨ĞÅ³µÁ¾Í¨ĞÅ³¬Ê±¼ÆÊıÆ÷   
extern uint16_t uwCANTimeOutCount;
//¶¨Ê±Æ÷6¼ÆÊı£¬10ms¼ÆÊıÒ»´Î,CANÍ¨ĞÅDCµçÔ´Ä£¿é³¬Ê±¼ÆÊıÆ÷   
extern uint16_t uwCANDCTimeOutCount;

//CAN½ÓÊÕBMSÊı¾İ»º³åÆ÷  Í¨ÓÃÅÅ³ıÒÔÏÂ¼¸ÖÖÇé¿ö
extern uint8_t ubCANBuf_BMS[8];
//CAN½ÓÊÕÊı¾İ»º³åÆ÷¶şÎ»Êı×éË÷Òı
extern uint8_t ubCANBufIndex_BMS;
//CANÊÕµ½BMSÊı¾İµÄID
extern uint32_t uCANBufID_BMS;

//CAN½ÓÊÕDCÊı¾İ»º³åÆ÷  Í¨ÓÃÅÅ³ıÒÔÏÂ¼¸ÖÖÇé¿ö
extern uint8_t ubCANBuf_DC[8];
//CAN½ÓÊÕDCÊı¾İ»º³åÆ÷¶şÎ»Êı×éË÷Òı
extern uint8_t ubCANBufIndex_DC;
//CANÊÕµ½DCÊı¾İµÄID
extern uint32_t uCANBufID_DC;


//¶àÖ¡Á¬½Ó±¨ÎÄ½¨Á¢Á¬½ÓÓÃ
extern uint8_t ubCANConnect[8];
extern uint8_t ubCANMultiFrame[70][8];
extern uint8_t ubCANMultiFrameIndex;


extern uint16_t uwCANReceFlagState;//ÊÇ·ñÊÕµ½CANÊı¾İÖ¡±êÖ¾Î» 0±íÊ¾ÎŞĞ§ 1±íÊ¾ÓĞĞ§  
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
                                 //µÚ12Î» DCµçÔ´Êä³ö×´Ì¬±¨ÎÄ
extern uint16_t uwCANSendFlagState;//ÊÇ·ñ·¢ËÍCANÊı¾İÖ¡±êÖ¾Î» 0±íÊ¾ÎŞĞ§ 1±íÊ¾ÓĞĞ§  
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

extern uint8_t ubManyBytesStateBCS;//¶àÖ¡Êı¾İµÄ±êÖ¾
extern uint8_t ubManyBytesStateBMV;//¶àÖ¡Êı¾İµÄ±êÖ¾
extern uint8_t ubManyBytesStateBMT;//¶àÖ¡Êı¾İµÄ±êÖ¾
extern uint8_t ubManyBytesStateBSP;//¶àÖ¡Êı¾İµÄ±êÖ¾

//¶àÊı¾İ´«ÊäĞ­ÒéÁ¬½Ó¹ÜÀí»º´æÊı¾İ
extern uint8_t ubCANConnect[8];

extern uint8_t ubCellVoltageQuantity;     //µ¥Ìå¶¯Á¦Ğîµç³Øµç³ØµçÑ¹ÊıÁ¿ 
extern uint8_t ubCellTemperatureQuantity; //¶¯Á¦Ğîµç³ØÎÂ¶È ÊıÁ¿
extern uint8_t ubCellReservedQuantity;    //¶¯Á¦Ğîµç³ØÔ¤Áô±¨ÎÄ  ÊıÁ¿

extern uint8_t ubCellVoltageMegQuantity;     //µ¥Ìå¶¯Á¦Ğîµç³Øµç³ØµçÑ¹ ±¨ÎÄÖ¡ÊıÁ¿ 
extern uint8_t ubCellTemperatureMegQuantity; //¶¯Á¦Ğîµç³ØÎÂ¶È ±¨ÎÄÖ¡ÊıÁ¿
extern uint8_t ubCellReservedMegQuantity;    //¶¯Á¦Ğîµç³ØÔ¤Áô±¨ÎÄ  ±¨ÎÄÖ¡ÊıÁ¿

//ÎïÀíÁ¬½ÓÕı³£±êÖ¾
extern uint8_t ubPhysicalConnect;  //0x00±íÊ¾Î´Á¬½Ó£¬0xAA±íÊ¾ÎïÀíÁ¬½ÓÕı³££¬ÆäËûÎŞĞ§

extern uint16_t uwReadTimeCount;//CANÍ¨ĞÅ×îĞ¡¼ÆÊ±µ¥Î»ÊÇ·ÖÖÓ    ÕâÀï¶¨Òå30Ãë¶ÁÈ¡Ò»´ÎÊ±¼ä£¬ºóÆÚ¸ù¾İÒªÇóĞŞ¸Ä
extern uint8_t FlagIfReadTime;//ÊÇ·ñ¶ÁÈ¡Ê±¼ä±êÖ¾Î»
extern uint8_t FlagIfSendDCMeg;
extern uint8_t FlagIfShowInformation5S ;  //Ë¢¿¨½áÊøºóÍ£Áô±êÖ¾Î»
extern uint8_t FlagShowInformation ;

extern uint32_t Flag;

extern uint8_t ubDCModue;      //DCÄ£¿éÊÇ·ñÆô¶¯
//¶à¸öDC³äµçÄ£¿éµÄµØÖ·£¬1-N¸öÄ£¿é·Ö±ğÎª0x01 0x02 0x03 ......£¬
extern uint8_t ubDCModueAddress;
#define CAN_STARTDCMODUE 0x55
#define CAN_STOPDCMODUE 0xAA


extern uint8_t Time[8];

extern uint8_t ChargeFlag ;  //³äµç±êÖ¾ 1:ÕıÔÚ³äµç 0:²»ÔÚ³äµç
extern uint8_t CANChargeFlag ; //³äµç½×¶Î±êÖ¾

//---------------------------------
//¶Á¿¨Æ÷
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
extern uint8_t WriteBlockFlag;//Ğ´¿¨Ä£¿é±êÖ¾Î»£¬µ÷ÓÃWriteBlock(1,ICDataReadBuf)º¯ÊıÊ±£¬´Ë±êÖ¾Î»ÖÃ1,Ğ´¿¨³É¹¦Ö®ºó´Ë±êÖ¾Î»¸´Î»0
extern uint8_t WriteBlockTimeFlag;//Ğ´¿¨Ä£¿éÖÜÆÚ·¢ËÍ±êÖ¾£¬100msÖÃÎ»Ò»´Î£¬ÓÃÍê¼´Çå0

extern uint8_t Tim3ICWaitCount;
extern UART_HandleTypeDef huart6;
extern uint8_t pData[1];

extern uint8_t   StartICMoney[4];
extern uint32_t  StartICMoneyAll;
extern uint8_t   DealMoney[4];
extern uint32_t  DealMoneyAll;
extern uint32_t  StopMoneyAll;
//---------------------------------
//µç±í
//-----------------------------------------
extern uint8_t UART5RxBuffCount;
extern uint8_t UART5RxBuff[24];
extern uint8_t UART5RxFlag;

extern uint8_t	PMsendBuf[8];
extern uint8_t  VoltageA[2];
extern uint8_t  VoltageB[2];
extern uint8_t  VoltageC[2];
extern uint8_t  RectivePower[2];//ÎŞ¹¦¹¦ÂÊ
extern uint8_t  ApparentPower[2];//×ÜÊÓÔÚ¹¦ÂÊ
extern uint8_t  Frequency[2];//ÆµÂÊ
extern uint8_t  ACEnergy [4]; //ÓĞ¹¦µçÄÜ
extern uint8_t  CurrentA[2];//AÏàµçÁ÷
extern uint8_t  CurrentB[2];//
extern uint8_t  CurrentC[2];//
extern uint8_t  DCVoltage[2];//Ö±Á÷±íµçÑ¹
extern uint8_t  DCEnergy [4]; //Ö±Á÷±íµçÄÜ
extern uint8_t  DCCurrent[2];//Ö±Á÷±íµçÁ÷
extern uint8_t  InsulationRes[4];//¾øÔµµç×è
extern uint8_t  Crc485[2];

extern uint8_t  TxFlag485;  //Îª1Ê±·¢ËÍ£¬
extern uint8_t  RxFlag485;  //
extern uint8_t  PMCount485;
extern uint8_t  Tim4Flag;
extern uint8_t  Flag485;//´ú±í·¢ËÍ²»Í¬µÄÖ¸Áî£º
extern uint8_t  Fun485Flag;  // 1:ÈıÏàµçÁ÷  2:ÈıÏàµçÑ¹  3:½»Á÷µçÄÜ 4:½»Á÷ÆµÂÊ   5:×ÜÎŞ¹¦¹¦ÂÊ   6:×ÜÊÓÔÚ¹¦ÂÊ  7:¾øÔµ¼ì²âµç×è  8:Ö±Á÷µç±í 0:²»·¢Ö¸Áî

extern uint8_t   StartEnergy[4];
extern uint32_t   StartEnergyAll;
extern uint8_t   DealEnergy[4];
extern uint32_t  DealEnergyAll;



//----------------------------------
//´¥ÃşÆÁ
//----------------------------------
#define MAX_CMD_BUFFER 500
extern uint8_t   cmd_buffer[MAX_CMD_BUFFER];
extern uint16_t  MoneyAverageElectricity;                     //Æ½¾ùµç¼Û
extern uint16_t  MoneyPeakElectricity;                        //·åÖµµç¼Û
extern uint16_t  MoneyValleyElectricity;                      //¹ÈÖµµç¼Û
extern uint16_t  SetTimeHourStartPeakElectricity;             //·åÖµµç¼Û¿ªÊ¼Ê±¼ä£¨Ê±£©
extern uint16_t  SetTimeMinStartPeakElectricity;              //·åÖµµç¼Û¿ªÊ¼Ê±¼ä£¨·Ö£©
extern uint16_t  SetTimeHourStopPeakElectricity;              //·åÖµµç¼Û½áÊøÊ±¼ä£¨Ê±£©
extern uint16_t  SetTimeMinStopPeakElectricity;               //·åÖµµç¼Û½áÊøÊ±¼ä£¨·Ö£©
extern uint16_t  SetTimeHourStartValleyElectricity;           //¹ÈÖµµç¼Û¿ªÊ¼Ê±¼ä£¨Ê±£©
extern uint16_t  SetTimeMinStartValleyElectricity;            //¹ÈÖµµç¼Û¿ªÊ¼Ê±¼ä£¨·Ö£©
extern uint16_t  SetTimeHourStopValleyElectricity;            //¹ÈÖµµç¼Û½áÊøÊ±¼ä£¨Ê±£©
extern uint16_t  SetTimeMinStopValleyElectricity;             //¹ÈÖµµç¼Û½áÊøÊ±¼ä£¨·Ö£©

/*         Ò»°ã¶¨Òå               */
extern  uint8_t FaultOperation;
extern	uint8_t SystemState;      //ÏµÍ³×´Ì¬£º0£¬¿ÕÏĞ×´Ì¬£»1£¬³äµç×´Ì¬ £»2£¬Òì³£×´Ì¬£»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»¸ù¾İÉèÖÃÌõ¼şÇĞ»»ÏµÍ³×´Ì¬
extern	uint8_t ChargingMethod ;  //³äµç·½Ê½£º0£¬×Ô¶¯³äµç£»1£¬ÊÖ¶¯³äµç £»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»ÆÁÄ»Ñ¡ÔñÊÖ¶¯Ä£Ê½ÖÃ1
extern	uint8_t ChargingMode;     //³äµçÄ£Ê½£º0£¬³äÂúÎªÖ¹£»1£¬°´Ê±¼ä³ä £»2.°´µçÁ¿³ä£»3£¬°´½ğ¶î³ä£»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»ÆÁÄ»Ñ¡Ôñ
extern uint8_t ChargingMode;     //³äµçÄ£Ê½£º0£¬³äÂúÎªÖ¹£»1£¬°´Ê±¼ä³ä £»2.°´µçÁ¿³ä£»3£¬°´½ğ¶î³ä£»Ä¬ÈÏÎª0£»³õÊ¼»¯Îª0£»ÆÁÄ»Ñ¡Ôñ
extern uint16_t SetKwh;                             //³äµçÄ£Ê½½çÃæ£¬ÉèÖÃµçÁ¿
extern uint16_t SetMoney;                           //³äµçÄ£Ê½½çÃæ£¬ÉèÖÃ½ğ¶î
extern uint16_t SetHour;                            //³äµçÄ£Ê½½çÃæ£¬ÉèÖÃÊ±¼ä£¨Ê±£©
extern uint16_t SetMin;                             //³äµçÄ£Ê½½çÃæ£¬ÉèÖÃÊ±¼ä£¨·Ö£©
extern uint16_t SetChargeCutOffCurrent;           //ÉèÖÃ½ØÖ¹µçÁ÷
extern uint16_t SetChargeVoltage;                 //ÉèÖÃ³äµçµçÑ¹
extern uint16_t SetChargeCurrent;                 //ÉèÖÃ³äµçµç?

extern	uint8_t Timer1CountNumber; //Ã¿500ms¼Ó1£¬10s¸´Î»£¬¶¨Ê±Æ÷1ÓÃ
extern	uint8_t FlagCycle30s;     //10sÑ­»·¶¨Ê±±êÖ¾Î»£¬Ä¬ÈÏÎª0£»¶¨Ê±Æ÷1ÖÃÎ»£¬Ê¹ÓÃÍêÇåÁã
extern	uint8_t FlagCycle10s;     //10sÑ­»·¶¨Ê±±êÖ¾Î»£¬Ä¬ÈÏÎª0£»¶¨Ê±Æ÷1ÖÃÎ»£¬Ê¹ÓÃÍêÇåÁã
extern	uint8_t FlagCycle1s;      //1sÑ­»·¶¨Ê±±êÖ¾Î»£¬Ä¬ÈÏÎª0£»¶¨Ê±Æ÷1ÖÃÎ»£¬Ê¹ÓÃÍêÇåÁã
extern	uint8_t FlagCycle500ms;   //500msÑ­»·¶¨Ê±±êÖ¾Î»£¬Ä¬ÈÏÎª0£»¶¨Ê±Æ÷1ÖÃÎ»£¬Ê¹ÓÃÍêÇåÁã

extern uint16_t SystemFaultState;   //ÏµÍ³¹ÊÕÏ×´Ì¬?
                             //0000 0000 0000 0001 ¼±Í£°´Å¥°´ÏÂ   //0000 0001 0000 0000   ´æ´¢Æ÷Òì³£
							 //0000 0000 0000 0010 ²ÖÃÅÎ´¹Ø±Õ     //0000 0010 0000 0000   Ê±ÖÓÒì³£
							 //0000 0000 0000 0100 ×®ÌåÇãĞ±       //0000 0100 0000 0000   ¶Á¿¨Æ÷Òì³£
							 //0000 0000 0000 1000 ×®Ìå×²»÷       //0000 1000 0000 0000   ÈıÏàµçÈ±Ïà
                             //0000 0000 0001 0000 »·¾³ÎÂ¶È¹ı¸ß   //0001 0000 0000 0000   ¾øÔµµç×èÒì³£
                             //0000 0000 0010 0000 »·¾³ÎÂ¶È¹ıµÍ   //0010 0000 0000 0000   ¾øÔµ¼ì²âÒÇÒì³£ 
                             //0000 0000 0100 0000 »·¾³Êª¶È¹ı¸ß   //0100 0000 0000 0000   µçÔ´²ÖÎÂ¶È¹ı¸ß
							 //0000 0000 1000 0000 »·¾³Êª¶È¹ıµÍ   //1000 0000 0000 0000   µç±íÒì³£
extern	uint8_t PileTiltState;              //³äµç×®×®ÌåÇãĞ±×´Ì¬£¬0£¬Î´ÇãĞ±£»1£¬ÇãĞ±£»Ä¬ÈÏÎª0£»
extern	uint8_t CabinDoorState;             //²ÖÃÅ×´Ì¬£¬0£¬¹Ø±Õ£»1£¬´ò¿ª£»Ä¬ÈÏÎª0£»
extern	uint8_t EmergencyStopButtonState;   //¼±Í£°´Å¥×´Ì¬×´Ì¬£¬0£¬Õı³££»1£¬°´ÏÂ£»Ä¬ÈÏÎª0£»
extern	uint8_t PileState;                  //Ç¹Í·×´Ì¬£¬0±íÊ¾Î´Á¬½Ó£¬1±íÊ¾Á¬½Ó£¬Ä¬ÈÏÎª0
extern  uint8_t LevelState;                 //Ë®Æ½ÒÇ×´Ì¬£¬0±íÊ¾Õı³££¬1±íÊ¾²»Õı³££¬Ä¬ÈÏÎª0


extern  uint16_t DS18B20Temperature;       //DS18b20´«¸ĞÆ÷ÎÂ¶È
extern  uint16_t AM2321Temperature;        //AM2321ÎÂÊª¶È´«¸ĞÆ÷ÎÂ¶È
extern  uint16_t Am2321Humidity;           //AM2321ÎÂÊª¶È´«¸ĞÆ÷Êª¶È

extern  uint8_t SwipingCardFlag;   //É¨¿¨±êÖ¾£¬Ä¬ÈÏÎª0 ÎŞÒâÒå 1±íÊ¾Ë¢¿¨
extern  uint8_t SwipingPileFlag;    //É¨Ç¹±êÖ¾  Ä¬ÈÏÎª0 ÎŞÒâÒå 1±íÊ¾É¨Ç¹
extern  uint8_t SpeechFlag;        //ÓïÒô±êÖ¾£¬Ä¬ÈÏÎª0 1±íÊ¾ÓïÒôÓĞĞ§
extern uint8_t SpeechFlagMoney ;        //ÓïÒô±êÖ¾£¬Ä¬ÈÏÎª0 1±íÊ¾ÓïÒôÓĞĞ§
extern uint8_t SpeechFlagDeal ;        //ÓïÒô±êÖ¾£¬Ä¬ÈÏÎª0 1±íÊ¾ÓïÒôÓĞĞ§
extern uint8_t FlagIfSaveData;       //±£´æÉÏ´Î½»Ò×Êı¾İ Ä¬ÈÏÎª0 1±íÊ¾ÓĞĞ§
extern uint8_t FlagIfSaveToMemory ;
extern uint8_t FlagIfReadPriDeal;
extern uint8_t FlagIfCloseDoor;       // ¹Ø±Õ²ÖÃÅ±êÖ¾
extern uint8_t FlagOpenDoor;           //´ò¿ª²ÖÃÅ

extern uint16_t ReadPriDealNo ;

extern uint8_t *Recvdata;
extern uint16_t RecvLen;
extern uint32_t RecvIPNumber;





//-----------------------------------------
//Ê±¼ä
//-----------------------------------------
typedef struct 				 //¶¨Òå³äÊ±¼äÉè¶¨µÄ½á¹¹Ìå
{
	uint8_t Second;	
	uint8_t Minute;
	uint8_t Hour;
	uint8_t Week;
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;      
}RTC_SD2405_HandleTypeDef;


typedef struct 				 //¼ÇÂ¼´æ´¢ÅäÖÃĞÅÏ¢
{
	uint32_t  AcchAddress;       //³äµç×®ºÅ
	uint32_t  PosNumber;         //pos»úºÅ
	uint32_t  CardVer;           //¿¨°æ±¾ºÅ
	uint32_t  IPAddress;         //IPµØÖ·
	uint32_t  MACAddressHigh;     //MACµØÖ·¸ß16Î» ×Ü48Î»
	uint32_t  MACAddressLow;      //MACµØÖ·µÍ32Î» ×Ü48Î»
	uint16_t  AreaNumber;        //µØÇø´úÂë  
	uint16_t  ConsRate;          //µçÁ¿±¶ÂÊ
	uint16_t  CardType;          //¿¨ÀàĞÍ
	uint16_t  TransType;         //½»Ò×ÀàĞÍ
	uint16_t  APrice;			 //Æ½¾ùµç¼Û
	uint16_t  HPrice;			 //·åÖµµç¼Û
	uint16_t  LPrice;			 //¹ÈÖµµç¼Û
	uint16_t  HPriceStartH;		 //·åÖµÆğÊ¼Ê±¼ä(Ê±)
	uint16_t  HPriceStartM;		 //·åÖµÆğÊ¼Ê±¼ä£¨·Ö£©
	uint16_t  HPriceStopH;		 //·åÖµ½áÊøÊ±¼ä£¨Ê±£©
	uint16_t  HPriceStopM;		 //·åÖµ½áÊøÊ±¼ä£¨·Ö£©
	uint16_t  LPriceStartH;	 	 //¹ÈÖµÆğÊ¼Ê±¼ä(Ê±)
	uint16_t  LPriceStartM;		 //¹ÈÖµÆğÊ¼Ê±¼ä£¨·Ö£©
	uint16_t  LPriceStopH;		 //¹ÈÖµ½áÊøÊ±¼ä(Ê±)
	uint16_t  LPriceStopM;		 //¹ÈÖµ½áÊøÊ±¼ä£¨·Ö£©
	uint16_t  StartMode;		 //Æô¶¯Ä£Ê½
	uint16_t  BillMode;			 //¼Æ·ÑÄ£Ê½
	uint32_t  Password;	 		 //ÃÜÂë
	uint8_t   CRCHigh;            //CRC
	uint8_t   CRCLow;             //CRC
}SystemParameterConfig_HandleTypeDef;//´æ´¢Æ÷Êı¾İ

typedef struct 				 //¼ÇÂ¼´æ´¢ÊµÊ±ĞÅÏ¢ Ôİ´æÔÚĞ¡´æ´¢Æ÷ÖĞ£¬½»Ò×½áÊø£¬ÕûÌå°áÒÆÖÁ´ó´æ´¢Æ÷
{
	uint32_t DealNo;       //½»Ò×Á÷Ë®ºÅ    
	uint32_t DealCardNo;   //½»Ò×¿¨ºÅ  ÒÑÓĞ
	uint32_t ParkFee;      //Í£³µ·Ñ
	uint32_t OriMoney;     //½»Ò×Ç°Óà¶î ÒÑÓĞ
	uint32_t FinMoney;     //½»Ò×ºóÓà¶î ÒÑÓĞ
	uint32_t DealCounter;  //½»Ò×¼ÆÊıÆ÷
	uint32_t DealStartEnergy;
	uint16_t DealEnergy;   //½»Ò×µçÁ¿   ÒÑÓĞ
	uint32_t DealStopEnergy;  
	uint16_t DealPrice;    //½»Ò×µç¼Û  ÒÑÓĞ
	uint16_t PileNum;      //³äµç×®ºÅ   ±¾×®ºÅ
	uint16_t ChargMode;    //³äµçÄ£Ê½   ·ÖÎª×Ô¶¯³äÂú£¬°´Ê±¼ä½ğ¶îµçÁ¿µÈ   
	uint16_t ChargMethod;    //³äµç·½Ê½ ÊÖ¶¯ºÍ×Ô¶¯
	uint16_t StartYear;    //½»Ò×ÆğÊ¼Ê±¼ä£¨Äê£© ÒÑÓĞ
	uint16_t StartMonth;   //½»Ò×ÆğÊ¼Ê±¼ä£¨ÔÂ£© ÒÑÓĞ
	uint16_t StartDay;     //½»Ò×ÆğÊ¼Ê±¼ä£¨ÈÕ£© ÒÑÓĞ
	uint16_t StartWeek;    //½»Ò×ÆğÊ¼Ê±¼ä£¨ĞÇÆÚ£© ÒÑÓĞ
	uint16_t StartHour;    //½»Ò×ÆğÊ¼Ê±¼ä£¨Ê±£© ÒÑÓĞ
	uint16_t StartMin;     //½»Ò×ÆğÊ¼Ê±¼ä£¨·Ö£    ÒÑÓĞ
	uint16_t StartSec;     //½»Ò×ÆğÊ¼Ê±¼ä£¨Ãë£©  ÒÑÓĞ
	uint16_t EndYear;      //½»Ò×½áÊøÊ±¼ä£¨Äê£©  ÒÑÓĞ
	uint16_t EndMonth;     //½»Ò×½áÊøÊ±¼ä£¨ÔÂ£©  ÒÑÓĞ
	uint16_t EndDay;       //½»Ò×½áÊøÊ±¼ä£¨ÈÕ£©  ÒÑÓĞ
	uint16_t EndWeek;      //½»Ò×½áÊøÊ±¼ä£¨ĞÇÆÚ£©  ÒÑÓĞ
	uint16_t EndHour;      //½»Ò×½áÊøÊ±¼ä£¨Ê±£©   ÒÑÓĞ
	uint16_t EndMin;       //½»Ò×½áÊøÊ±¼ä£¨·Ö£©   ÒÑÓĞ
	uint16_t EndSec;       //½»Ò×½áÊøÊ±¼ä£¨Ãë£©   ÒÑÓĞ
  uint8_t  ChargeFlag;   //½»Ò×ÊÇ·ñ³É¹¦±êÖ¾0x00 ½»Ò×³É¹¦ 0xff½»Ò×²»³É¹¦(Í»È»¶Ïµç£¬Îó°ÎÇ¹)
	uint8_t  CRCHigh;      //CRC
	uint8_t  CRCLow;       //CRC
}SaveRealTimeData_HandleTypeDef;//´æ´¢ÊµÊ±Êı¾İ

typedef struct
{
	uint8_t  IAPState;    //IAPË÷Òı
	uint32_t IAPStartAddress;    //IAPÖ´ĞĞÆğÊ¼µØÖ·
	uint8_t  CRCHigh;      //CRC
	uint8_t  CRCLow;       //CRC
}IAPSystemParameter_HandleTypeDef;   //IAP½á¹¹Ìå

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


typedef struct 				 //¼ÇÂ¼´æ´¢ÊµÊ±ĞÅÏ¢
{
	uint32_t IPNumber;       //½»Ò×Á÷Ë®ºÅ
	uint8_t Data[100];
	uint16_t Length;
}strETHData_Typedef;//´æ´¢ÊµÊ±Êı¾İ

extern RTC_SD2405_HandleTypeDef RTCSetTime;
extern RTC_SD2405_HandleTypeDef RTCReadTime;
extern RTC_SD2405_HandleTypeDef RTCReadBCDTime;
extern RTC_SD2405_HandleTypeDef StartChargeBCDTime;
extern RTC_SD2405_HandleTypeDef StartChargeTime;
extern RTC_SD2405_HandleTypeDef ChargeTime;
extern unSaveRealTimeData_HandleTypedef  unSaveRealTimeDataHandle ;
extern unSystemParameterConfig_HandleTypedef   unSystemParameterConfigHandle;
extern unIAPSystemParameter_HandleTypeDef  unIAPSystemParameter;



#define FAULTLIGHTSET      1   //¹ÊÕÏµÆÁÁ
#define FAULTLIGHTRESET    0   //¹ÊÕÏµÆÃğ
#define FULLFREELIGHTSET   1   //³äÂú»òÕß¿ÕÏĞµÆÁÁ
#define FULLFREELIGHTRESET 0   //³äÂú»òÕß¿ÕÏĞµÆÃğµ

#define MAXAM2321TEMP     900    //ÎÂÊª¶È´«¸ĞÆ÷×î¸ßÔÊĞíÎÂ¶È0.1/bit   Æ«ÒÆÁ¿40¡æ   
#define MINAM2321TEMP     200    //ÎÂÊª¶È´«¸ĞÆ÷×îµÍÔÊĞíÎÂ¶È0.1/bit   Æ«ÒÆÁ¿40¡æ
#define MAXAM2321HUMI     950    //ÎÂÊª¶È´«¸ĞÆ÷×î¸ßÔÊĞíÊª¶È0.1/bit
#define MINAM2321HUMI     50    //ÎÂÊª¶È´«¸ĞÆ÷×îµÍÔÊĞíÊª¶È0.1/bit
#define DS18B20TEMP      800    //µçÔ´²Ö·çÉÈ¿ªÆôÎÂ¶ÈãĞ0.1/bit   Æ«ÒÆÁ¿40¡æ
#define MAXDS18B20TEMP   1100    //µçÔ´²Ö×î¸ßÎÂ¶ÈãĞÖµ0.1/bit   Æ«ÒÆÁ¿40¡æ


void mDelayCAN10us(uint8_t n);
void HAL_Delay1us(uint16_t n);
void SPI_Delay(uint16_t n);

void AccumulateChargeEnergy(void);
void AccumulateChargeMoney(void);

void SetACRelay(void);//Æô¶¯½»Á÷½Ó´¥Æ÷
void ResetACRwlay(void);//Í£Ö¹½»Á÷¼ÌµçÆ÷
void SetDCRelay(void);//Æô¶¯Ö±Á÷½Ó´¥Æ÷
void ResetDCRwlay(void);//Í£Ö¹Ö±Á÷½Ó´¥Æ÷
void SetBMSRelay(void); //Æô¶¯¸¨ÖúµçÔ´
void ResetBMSRwlay(void);//Í£Ö¹¸¨ÖúµçÔ´


void StartFan(void);//Æô¶¯·çÉÈ
void StopFan(void);//Í£Ö¹·çÉÈ
void OpenDoor(void);//¿ªÃÅ
void CloseDoor(void);//¹ØÃÅ
uint8_t CheckPileState(void);//¼ì²âÇ¹Í·×´Ì¬£¬Ç¹Í·Á¬½ÓÎª·µ»Ø1 £¬Ç¹Í·Î´Á¬½Ó·µ»Ø0
uint8_t CheckCabinDoorState(void);//¼ì²â²ÖÃÅ×´Ì¬£¬²ÖÃÅ¹Ø±ÕÎª1£¬²ÖÃÅÎ´¹Ø±ÕÎª0
uint8_t CheckLevelState(void);  //¼ì²âË®Æ½ÒÇ×´Ì¬£¬Ë®Æ½ÒÇÕı³£·µ»Ø0£¬Ë®Æ½ÒÇ²»Õı³£·µ»Ø1
uint8_t CheckEmergencyStopButtonState(void);//¼ì²â¼±Í£°´Å¥×´Ì¬£¬¼±Í£°´Å¥Î´°´ÏÂÎª0£¬¼±Í£°´Å¥°´ÏÂÎª1
void ShowPileFaultState(uint8_t);//³äµç×®¹ÊÕÏ£¬ºìµÆÁÁ
void ShowPileChargeFullOrFree(uint8_t);//³äµç×®³äÂú»òÕß¿ÕÏĞ
void Enable_IRQ(void);                 //Ê¹ÄÜÖĞ¶Ï
void Disable_IRQ(void);                 //¹Ø±ÕÖĞ¶Ï
void ReadIAPSystemParatemer(void);      //¶ÁÈ¡ÏµÍ³²ÎÊı
void SaveIAPSystemParatemer(void);       //±£´æÏµÍ³²ÎÊı

uint8_t SystemSelfingCheck(void);//ÏµÍ³×Ô¼ì
uint8_t Collect_System_State_Process(void);//²É¼¯ÏµÍ³×´Ì¬×Ó³ÌĞò¬
void ReadSystemParameter(void);   //¿ª»úÊ±¶ÁÈ¡ÏµÍ³²ÎÊı
void ReadDealData(void);      //Ë¢¿¨Ç°¶ÁÈ¡ÉÏ´Î½»Ò×Êı¾İ

void SetACRelay(void) ; //Æô¶¯½»Á÷½Ó´¥Æ÷
void ResetACRwlay(void); //Í£Ö¹½»Á÷¼ÌµçÆ÷
void SetDCRelay(void); //Æô¶¯Ö±Á÷½Ó´¥Æ÷
void ResetDCRwlay(void);//Í£Ö¹Ö±Á÷½Ó´¥Æ÷

void HMI_Event_Process(void);//´¦ÀíÆÁÄ»×Ó³ÌĞò
void ETH_Event_Process(void);//´¦ÀíÍøÂçÊı¾İ×Ó³ÌĞò
strETHData_Typedef ETH_GetData(void);
void Charge_State_Process(void);//³äµç×´Ì¬´¦Àí×Ó³ÌĞò
void Free_State_Process(void);//¿ÕÏĞ×´Ì¬´¦Àí×Ó³ÌĞò
void Error_State_Process(void);//Òì³£´¦Àí×Ó³ÌĞò

void ETH_Send_HeartbeatMeg(void);  //ÉÏ´«ĞÄÌø°ü

void DataInit(void);//Ò»°ã±äÁ¿³õÊ¼»¯




#endif /* __DEFINE_H */
