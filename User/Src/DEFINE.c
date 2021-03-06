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
//CAN通信报文事件的状态机
uint8_t  ubCANEventStatus;
/* CAN通信数据帧定义  */
/*----------------------------------------------
-----------------握手阶段定义------------------
-----------------------------------------------*/
//----------------PGN256 CRM-------------
 //辨识结果，物理连接之后，充电机向BMS发送此报文，在收到BMS发送的辨识报文之前发送0x00，在收到BMS辨识报文之后发送0xAA
uint8_t ubRecognitionResult;	
//充电机编号，1/位，1偏移量，数据范围1-100	
uint8_t ubChargerNumber;
//------------PGN512,BRM报文参数--------------------
//BMS通信协议版本号，本标准规定当前版本为V1.0 表示为：byte3 byte2 byte1  0x 00 01 00
uint32_t ulBmsProtocolVersion;
//电池类型：01H：铅酸电池 02H：镍氢电池	03：磷酸铁锂电池 04H：锰酸锂电池 05H：钴酸锂电池 06H：三元材料电池 07H：聚合物锂离子电池 08H：钛酸锂电池 FFH：其他电池
uint8_t ubBatteryType;
//整车动力蓄电池系统额定容量/A*h,0.1A*h/位，0.1A*h偏移量 数据范围 :0-——1000A.h
uint16_t uwBmsRateCapacity;
//整车动力蓄电池系统额定总电压 /V ，O.l V/位 ，0 V 偏移量，数据范 围:0  750 V
uint16_t uwBatteryRateVoltage;
//电池生产厂商名称，标准 ASCII 码
uint8_t ubBatteryManufacturerName[4];
//电池组序号，预留，由厂商自行定义
uint16_t ulBatteryManufacturerName;
//电池组生产日期，年 1985偏移位
uint8_t ubBatteryProduceYear;
//电池组生产日期，月
uint8_t ubBatteryProduceMonth;
//电池组生产日期，日
uint8_t ubBatteryProduceDay;
//电池组充电次数，1次每位  ,3个字节
uint16_t ulBatteryChargeCycle;
//电池组产权标志  0表示租赁   1表示 车自有
uint8_t ubPropertyMark;
//预留
uint8_t ubBMSReserved;
//车辆识别码VehicleIdentificationNumber
uint8_t ubVIN[17];

/*-----------------------------------------------
-------------------参数配置阶段报文--------------
-------------------------------------------------*/

//-------------------PGN1536 BCP报文参数-------------
//单体动力蓄电池最高允许充电电压  数据分辨率 :0.01 V/位， 0 V 偏移量 E 数据范围 :0 V~24V;   
uint16_t uwCellMaxAllowChargeVoltage;
//最高允许充电电流	 数据分辨率 :0.lA/位， -400 A 偏移量;数据范围: -400 A ~O A;
uint16_t uwMaxAllowChargeCurrent;
//动力蓄电池标称总能量	  数据分辨率 :0.1 kW  o h/ 位， 0 kW. h 偏移量;数据范围 :0，.....，1 000  kW o h; 
uint16_t uwBatteryTotalEnergy;
//最高允许充电总电压   数据分辨率 :0.1 V/ 位， 0  V 偏移量;数据范围 :0 V ，......750  V;
uint16_t uwMaxAllowChargeVoltage;
//最高允许温度		 数据分辨率 :1 oC/位，-50 OC 偏移量 F 数据范围:一 50 oC ，.....， +200 oC;
uint8_t ubMaxAllowTemperature;
//整车动力蓄电池荷电状态	数据分辨率 :0.1%/位， 0%偏移量;数据范围 :0，.....，100% ; 
uint16_t uwBmsSOC;
//整车动力蓄电池总电压
uint16_t uwBmsTotalVoltage;
//---------PGN2048 CML报文-------------
//DC最高输出电压
uint16_t uwDCMaxOutputVoltage;
//DC最低输出电压
uint16_t uwDCMinOutputVoltage;
//DC最大输出电流
uint16_t uwDCMaxOutputCurrent;
//---------PGN2304 BRO报文-------------
//BMS 是否充电准备好(< OxOO-) := BMS 未做好充电准备; (OxAA):=BMS 完成充电准备; (OxFF):=元效)
uint8_t ubBmsChargeReady;
//------------PGN2560 CRO报文
//充电机是否充电准备好 ((OxOO):=充电机未完成充电准备; (OxAA):=充电机完成充电准备; (OxFF):=无效)
uint8_t ubChargerOutputReady;

/*------------------------------------------
----------------充电阶段报文---------------
-------------------------------------*/

//-----------PGN4096 BCL报文----------------
//电压需求(V)
uint16_t uwVoltageRequirement;
uint8_t uwVoltageRequirementLow;
uint8_t uwVoltageRequirementHigh;
//电流需求 (A)
int16_t uwCurrentRequirement;
int8_t uwCurrentRequirementLow;
int8_t uwCurrentRequirementHigh;
int8_t uwCurrentRequirementLowbuf;
int8_t uwCurrentRequirementHighbuf;
int16_t uwCurrentRequirementbuf;
uint16_t CURRENT;
//充电模式 (Ox01:恒压充电	;Ox02:恒流充电)
uint8_t ubChargeMode;
//-------------PGN4352 BCS报----------------
//充电电压测量值 (V)
uint16_t uwChargeVoltageMeasurement;
//充电电流测量值 (A)
uint16_t uwChargeCurrentMeasurement;
//最高单体动力蓄电池电压及其组号
uint16_t uwHighestCellVoltageAndGroupNumber;
//当前荷电状态 SOC(%)
uint8_t ubCurrentSOC;
//估算剩余充电时间 (min)
uint16_t uwRemainChargeTime;
//---------PGN4608 CCS报文-------------
//电压输出值 (V)
uint32_t uwOutputVoltageValue;
//电流输出值 (A)
uint32_t uwOutputCurrentValue;
//累计充电时间 (min)
uint16_t uwAccumulateChargeTime;
//--------------PGN4864 BSM--------
//最高单体动力蓄电池电压所在编号
uint8_t ubMaxCellVoltageNumber;
//最高动力蓄电池温度
uint8_t ubMaxTemperature;
//最高温度检测点编号
uint8_t ubMaxTemperatureNumber;
//最低动力蓄电池温度
uint8_t ubMinTemperature;
//最低动力蓄电池温度检测点编号
uint8_t ubMinTemperatureNumber;
//异常状态信息	 
//单体动力蓄电池电压过高/过低 ((00): =正常 ;{01):= 过高;(10):=过低)
//整车动力蓄电池荷电状态 soc 过高/过低 ((00): =正常 ;(01):= 过高;(10):= 过低〉
//动力蓄电池充电过电流 ((00):= 正常 ;(01):== 过流;(10): =不可信 状态〉
//动力蓄电池温度过高 ((00): =正常; (01): =:过高;(1 0):= 不可信 状态
//动力蓄电池绝缘状态 ((00): =正常 ;(01):=不正常;(10):= 不可信 状态〉
//动力蓄电池组输出连接器连接状态 ((00):= 正常; (01): =不正常;(10): =不可信状态〉
//充电允许 ((00):= 禁止 ;(01):=允许〉
uint16_t uwStatusAndAbnormal;
//----------------PGN5376 BMV--------------
//单体动力蓄电池电压
uint16_t uwCellVoltage[256];

//----------------PGN5632 BMT--------------
//单体动力蓄电池温度
uint8_t ubCellTemperature[128];

//----------------PGN5888 BSP--------------
//单体动力蓄电池预留报文
uint8_t ubCellReserved[16];

//--------------PGN6400 BTS----------
//BMS  中止充电原因
uint8_t ubBmsChargeStopReason;
//BMS  中止充电故障原因
uint16_t uwBmsChargeStopFaultReason;
//BMS  中止充电错误原因
uint8_t ubBmsChargeStopErrorReason;

//------------PGN6656 CST-------------
//充电机中止充电原因
uint8_t ubChargerStopReason;
//克电机中止充电故障原因
uint16_t uwChargerStopFaultReason;
//充电机中止充电错误原因
uint8_t ubChargerStopErrorReason;

//------------PGN7168 BSD--------------
//中止荷电状态 SOC(%)
uint8_t ubBmsEndOfSOC;
//动力蓄电池单体最低电压 (V)
uint16_t uwBmsEndMinCellVoltage;
//动力蓄电池单体最高电压 (V)
uint16_t uwBmsEndMaxCellVoltage;
//动力蓄电池最低温度 CC)
uint8_t ubBmsEndMinTemperature;
//动力蓄电池最高温度 CC)
uint8_t ubBmsEndMaxTemperature;

//-----------PGN7424 CSD-----------
//累计充电时间 (min)
uint16_t uwChargerEndChargeTime;
//输出能量 (kW. h)
uint16_t uwChargerEndChargePower;

//---------PGN7680 BEM------------
//BMS 错误报文 (BEM)
uint8_t ubBmsError[4];

//-----------PGN7936 CEM-----------
//充电机错误报文 (CEM)
uint8_t ubChargerError[4];

//DC直流充电模块状态
uint8_t ubDCModuleStatusHighAll;
uint8_t ubDCModuleStatusLowAll;

uint8_t ubDCModuleStatusHigh[4];
uint8_t ubDCModuleStatusLow[4];
// DC电压输出
uint16_t uwDCOutputVoltage[4];
//DC电流输出
uint16_t uwDCOutputCurrent[4];
// BMS需求充电方式缓存值 (Ox01:恒压充电	;Ox02:恒流充电)
uint8_t ubChargeModeBuf;

//DC直流充电模块状态
uint16_t uwDCModuleStatus;
//DC模块是否启动
uint8_t ubDCModue;  
//多个DC充电模块的地址，1-N个模块分别为0x01 0x02 0x03 ......，
uint8_t ubDCModueAddress;
#define CAN_STARTDCMODUE 0x55
#define CAN_STOPDCMODUE 0xAA


//标志，是否超时5s 0x00不满足，0xAA满足
uint8_t FlagIfTimeout;
//标志，DC电源模块是否超时5s 0x00不满足，0xAA满足
uint8_t FlagIfDCTimeout;
//超时时间设置，ubTimeNs表示多少个100ms
uint8_t ubTimeN100ms;


//标志，是否满足周期发送 发送周期10ms 0不满足，1满足
uint8_t FlagIfCycleSend10ms;
//标志，是否满足周期发送 发送周期50ms 0不满足，1满足
uint8_t FlagIfCycleSend50ms;
//标志，是否满足周期发送 发送周期250ms 0不满足，1满足
uint8_t FlagIfCycleSend250ms;
//标志，是否满足周期发送 发送周期500ms 0不满足，1满足
uint8_t FlagIfCycleSend500ms;
//标志，是否满足周期发送 发送周期1000ms 0不满足，1满足
uint8_t FlagIfCycleSend1000ms;

//定时器6计数，10ms计数一次   
uint8_t ubCANTim3Count;
//定时器6计数，10ms计数一次,CAN通信车辆通信超时计数器   
uint16_t uwCANTimeOutCount;
//定时器6计数，10ms计数一次,CAN通信DC电源模块超时计数器   
uint16_t uwCANDCTimeOutCount;


//标志，是否收到BMS发送的BMS和车辆辨识报文 0x00未收到，0xAA收到
uint8_t ubFlagIfReceBmsIdentifyMeg;
//标志，是否收到BMS发送的电池充电参数报文 0x00未收到，0xAA收到
uint8_t ubFlagIfReceBatteryParameterMeg;
//标志，是否收到BMS发送的电池充电准备就绪状态报文 0未收到，1收到
uint8_t FlagIfReceBmsReadyMeg;
//标志，是否收到BMS发送的电池充电总状态报文 0x00未收到，0xAA收到
uint8_t ubFlagIfReceBmsAllChargeStateMeg;
//标志，是否收到BMS发送的电池充电需求报文 0x00未收到，0xAA收到
uint8_t FlagIfReceBmsChargeRequirementMeg;
//标志，是否收到BMS发送的动力蓄电池状态 0x00未收到，0xAA收到
uint8_t FlagIfReceBmsBatteryStateMeg;
//标志，是否收到BMS发送结束充电或者BMS中止充电报文 0x00未收到，0xAA收到
uint8_t ubFlagIfReceBmsEndAndStopChargeMeg;
//标志，是否收到BMS发送的BMS中止充电报文 0x00未收到，0xAA收到
uint8_t FlagIfReceBmsStopChargeMeg;
//标志，是否收到BMS发送的充电统计数据报文 0x00未收到，0xAA收到
uint8_t FlagIfReceBmsStatisticsDataMeg;


//标志，是否发送报文给DC直流充电机模块 0x00不发送，0xAA发送
uint8_t FlagIfSendDCModuleMeg;
//标志，是否收到DC直流充电机模块发送的报文 0没收到，1收到
uint8_t FlagIfReceDCModuleMeg;

//标志，是否发送充电机辨识报文 0x00不发送，0xAA发送
uint8_t ubFlagIfSendChargerIdentifyMeg;
//标志，是否发送充电机时间同步信息报文 0x00不发送，0xAA发送
uint8_t ubFlagIfSendChargerTimeMeg;
//标志，是否发送最大输出能力报文 0x00不发送，0xAA发送
uint8_t ubFlagIfSendChargerMaxOutputCapactityMeg;
//标志，是否发送充电机输出就绪状态报文 0x00不发送，0xAA发送
uint8_t ubFlagIfSendChargerOutputReadyMeg;
//标志，是否发送充电机充电状态报文 0x00不发送，0xAA发送
uint8_t FlagIfSendChargerChargingStateMeg;
//标志，是否充电机中止充电报文 0x00不发送，0xAA发送
uint8_t ubFlagIfSendChargerStopChargeMeg;
//标志，是否发送充电机充电统计数据报文 0x00不发送，0xAA发送
uint8_t ubFlagIfSendChargerStatisticsDataMeg;



//CAN接收BMS数据缓冲器  通用排除以下几种情况
uint8_t ubCANBuf_BMS[8];
//CAN接收BMS数据缓冲器二位数组索引
uint8_t ubCANBufIndex_BMS;
//CAN收到BMS数据的ID
uint32_t uCANBufID_BMS;

//CAN接收DC数据缓冲器  通用排除以下几种情况
uint8_t ubCANBuf_DC[8];
//CAN接收DC数据缓冲器二位数组索引
uint8_t ubCANBufIndex_DC;
//CAN收到DC数据的ID
uint32_t uCANBufID_DC;

//多帧连接报文建立连接用
uint8_t ubCANConnect[8];
uint8_t ubCANMultiFrame[70][8];
uint8_t ubCANMultiFrameIndex;

uint16_t uwCANReceFlagState;//是否收到CAN数据帧标志位 0表示无效 1表示有效  
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
                                 //第12位 DC电源输出状态报�
uint16_t uwCANSendFlagState;//是否发送CAN数据帧标志位 0表示无效 1表示有效  
                                 //第0位 充电机辨识报文
                                 //第1位 充电机发送时间同步信息
                                 //第2位 充电机最大输出能力
                                 //第3位 充电机输出准备就绪报文
                                 //第4位 充电机充电状态
                                 //第5位 充电机中止充电
                                 //第6位 充电机统计数据
                                 //第7位 充电机错误报文
                                 //第8位 DC电源设置报文
                                 //第9位 DC电源启动停止报文           

uint8_t ubManyBytesStateBCS;//多帧数据的标志
uint8_t ubManyBytesStateBMV;//多帧数据的标志
uint8_t ubManyBytesStateBMT;//多帧数据的标志
uint8_t ubManyBytesStateBSP;//多帧数据的标志

//多数据传输协议连接管理缓存数据
uint8_t ubCANConnect[8];

uint8_t ubCellVoltageQuantity;     //单体动力蓄电池电池电压数量 
uint8_t ubCellTemperatureQuantity; //动力蓄电池温度 数量
uint8_t ubCellReservedQuantity;    //动力蓄电池预留报文  数量

uint8_t ubCellVoltageMegQuantity;     //单体动力蓄电池电池电压 报文帧数量 
uint8_t ubCellTemperatureMegQuantity; //动力蓄电池温度 报文帧数量
uint8_t ubCellReservedMegQuantity;    //动力蓄电池预留报文  报文帧数量

//物理连接正常标志
uint8_t ubPhysicalConnect;  //0x00表示未连接，0xAA表示物理连接正常，其他无效

uint16_t uwReadTimeCount;//CAN通信最小计时单位是分钟    这里定义30秒读取一次时间，后期根据要求修改
uint8_t FlagIfReadTime;//是否读取时间标志位
uint8_t FlagIfSendDCMeg;
uint8_t ubDCModue;      //DC模块是否启动
uint8_t FlagIfShowInformation5S = 0;  //刷卡结束后停留标志位
uint8_t FlagShowInformation = 0;

uint32_t Flag=0;

uint8_t ChargeFlag = 0;  //充电标志 1:正在充电 0:不在充电
uint8_t CANChargeFlag = 0; //充电阶段标志
//-----------------------------------------
//读卡器使用的变量
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
uint8_t WriteBlockFlag = 0;//写卡模块标志位，调用WriteBlock(1,ICDataReadBuf)函数时，此标志位置1,写卡成功之后此标志位复位0
uint8_t WriteBlockTimeFlag = 0;//写卡模块周期发送标志，100ms置位一次，用完即清0

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
unSaveRealTimeData_HandleTypedef  unSaveRealTimeDataHandle ;   //充电信息联合体
unSystemParameterConfig_HandleTypedef   unSystemParameterConfigHandle;   //存储数据联合体
unIAPSystemParameter_HandleTypeDef  unIAPSystemParameter;     //IAP联合体

uint8_t Time[8];

//电表变量
uint8_t UART5RxBuff[24];
uint8_t UART5RxBuffCount;
uint8_t UART5RxFlag;

uint8_t	 PMsendBuf[8];
uint8_t  VoltageA[2];
uint8_t  VoltageB[2];
uint8_t  VoltageC[2];
uint8_t  RectivePower[2];//无功功率
uint8_t  ApparentPower[2];//总视在功率
uint8_t  Frequency[2];//频率
uint8_t  ACEnergy [4]; //有功电能
uint8_t  CurrentA[2];//A相电流
uint8_t  CurrentB[2];//
uint8_t  CurrentC[2];//
uint8_t  DCVoltage[2];//直流表电压
uint8_t  DCEnergy [4]; //直流表电能
uint8_t  DCCurrent[2];//直流表电流
uint8_t  InsulationRes[4];//绝缘电阻
uint8_t  Crc485[2];

uint8_t  TxFlag485; 
uint8_t  RxFlag485=0;
uint8_t  PMCount485;
uint8_t  Tim4Flag;
uint8_t  Flag485;//代表发送不同的指令：
uint8_t  Fun485Flag;  // 1:三相电流  2:三相电压  3:交流电能 4:交流频率   5:总无功功率   6:总视在功率  7:绝缘检测电阻  8:直流电表 0:不发指令

uint8_t   StartEnergy[4];
uint32_t   StartEnergyAll;
uint8_t   DealEnergy[4] = {0,0,0,0};
uint32_t  DealEnergyAll;

uint8_t  cmd_buffer[MAX_CMD_BUFFER];



/*         一般定义               */
uint8_t FaultOperation = 0;
uint8_t SystemState;      //系统状态：0，空闲状态；1，充电状态 ；2，异常状态；默认为0；初始化为0；根据设置条件切换系统状态
uint8_t ChargingMethod = 0;  //充电方式：0，自动充电；1，手动充电 ；默认为0；初始化为0；屏幕选择手动模式置1
uint8_t ChargingMode = 0;     //充电模式：0，充满为止；1，按时间充 ；2.按电量充；3，按金额充；默认为0；初始化为0；屏幕选择
uint16_t SetKwh=0;                             //充电模式界面，设置电量
uint16_t SetMoney=0;                           //充电模式界面，设置金额
uint16_t SetHour=0;                            //充电模式界面，设置时间（时）
uint16_t SetMin=0;                             //充电模式界面，设置时间（分）
uint16_t SetChargeCutOffCurrent=0;           //设置截止电流
uint16_t SetChargeVoltage=0;                 //设置充电电压
uint16_t SetChargeCurrent=0;                 //设置充电电?

uint8_t Timer1CountNumber; //每500ms加1，10s复位，定时器1用
uint8_t FlagCycle30s;     //10s循环定时标志位，默认为0；定时器1置位，使用完清零
uint8_t FlagCycle10s;     //10s循环定时标志位，默认为0；定时器1置位，使用完清零
uint8_t FlagCycle1s;      //1s循环定时标志位，默认为0；定时器1置位，使用完清零
uint8_t FlagCycle500ms;   //500ms循环定时标志位，默认为0；定时器1置位，使用完清零

uint16_t SystemFaultState;   //系统故障状态�
                             //0000 0000 0000 0001 急停按钮按下   //0000 0001 0000 0000   存储器异常
							 //0000 0000 0000 0010 仓门未关闭     //0000 0010 0000 0000   时钟异常
							 //0000 0000 0000 0100 桩体倾斜       //0000 0100 0000 0000   读卡器异常
							 //0000 0000 0000 1000 桩体撞击       //0000 1000 0000 0000   三相电缺相
                             //0000 0000 0001 0000 环境温度过高   //0001 0000 0000 0000   绝缘电阻异常
                             //0000 0000 0010 0000 环境温度过低   //0010 0000 0000 0000   绝缘检测仪异常 
                             //0000 0000 0100 0000 环境湿度过高   //0100 0000 0000 0000   电源仓温度过高
							 //0000 0000 1000 0000 环境湿度过低   //1000 0000 0000 0000   电表异常
uint8_t PileTiltState;              //充电桩桩体倾斜状态，0，未倾斜；1，倾斜；默认为0；
uint8_t CabinDoorState;             //仓门状态，0，关闭；1，打开；默认为0；
uint8_t EmergencyStopButtonState;   //急停按钮状态状态，0，正常；1，按下；默认为0；
uint8_t LevelState;                 //水平仪状态，0表示正常，1表示不正常，默认为0�

uint8_t PileState;                  //枪头状态，0表示未连接，1表示连接，默认为0
uint16_t DS18B20Temperature;       //DS18b20传感器温度
uint16_t AM2321Temperature;        //AM2321温湿度传感器温度
uint16_t Am2321Humidity;           //AM2321温湿度传感器湿度

uint8_t SwipingCardFlag =0;   //刷卡标志，默认为0 无意义 1表示刷卡
uint8_t SwipingPileFlag = 0;
uint8_t SpeechFlag =0;        //语音标志，默认为0 1表示语音有效
uint8_t SpeechFlagMoney =0;        //语音标志，默认为0 1表示语音有效
uint8_t SpeechFlagDeal =0;        //语音标志，默认为0 1表示语音有效
uint8_t FlagIfCloseDoor = 0;       // 关闭仓门标志
uint8_t FlagOpenDoor = 0;           //打开仓门

uint8_t FlagIfSaveData = 0;       //保存上次交易数据 默认为0 1表示有效
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
	uwRemainChargeTime = 0; //剩余时间
	DealMoneyAll = 0;    //充电金额
	uwBmsSOC = 0;   //BMS 的SOC
	ChargeTime.Hour = 0;  //充电时间 小时
	ChargeTime.Minute = 0;    //充电时间 分钟
	//FaultOperation = 0;       //错误操作标志位
	DealEnergy[2] = 0;    //交易电能高八位
	DealEnergy[3] = 0;     //交易电能低八位
	ubCANEventStatus = 1;
	FlagIfSaveData = 0;       //保存上次交易数据 默认为0 1表示有效
	UpDataId();
	UpDataBalance();
}


void SetACRelay(void) //启动交流接触器
{
	HAL_GPIO_WritePin( GPIOC,GPIO_PIN_2, GPIO_PIN_RESET);
}

void ResetACRwlay(void)//停止交流继电器
{
	HAL_GPIO_WritePin( GPIOC,GPIO_PIN_2, GPIO_PIN_SET);
}

void SetDCRelay(void) //启动直流接触器
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_15, GPIO_PIN_SET);
}

void ResetDCRwlay(void)//停止直流接触器
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_15, GPIO_PIN_RESET);
}

void SetBMSRelay(void) //启动辅助电源
{
	HAL_GPIO_WritePin( GPIOF,GPIO_PIN_10, GPIO_PIN_RESET);
}

void ResetBMSRwlay(void)//停止辅助电源
{
	HAL_GPIO_WritePin( GPIOF,GPIO_PIN_10, GPIO_PIN_SET);
}

void StartFan(void)//启动风扇
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_6, GPIO_PIN_SET);
}

void StopFan(void)//停止风扇
{
	HAL_GPIO_WritePin( GPIOA,GPIO_PIN_6, GPIO_PIN_RESET);
}

void OpenDoor(void)//开门
{
	HAL_GPIO_WritePin( GPIOG,GPIO_PIN_9, GPIO_PIN_SET);
}
void CloseDoor(void)//关门
{
	HAL_GPIO_WritePin( GPIOG,GPIO_PIN_9, GPIO_PIN_RESET);
}
uint8_t CheckPileState(void) //检测枪头状态，枪头连接为返回1 ，枪头未连接返回0
{
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4) == 0)
		return 1;
	else
		return 0;	 
}

uint8_t CheckCabinDoorState(void) //检测仓门状态，仓门关闭为1，仓门未关闭为0
{
	return HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_11);
}

uint8_t CheckLevelState(void)  //检测水平仪状态，水平仪正常返回0，水平仪不正常返回1
{
	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4);
}
	
uint8_t CheckEmergencyStopButtonState(void)//检测急停按钮状态，急停按钮未按下为0，急停按钮按下为1
{
	return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5);
}

void ReadSystemParameter(void)   //开机读取系统参数
{
	HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS+2,0x00,I2C_MEMADD_SIZE_8BIT,unSystemParameterConfigHandle.unbuffer,62,0xff);
}

void ReadDealData(void)      //刷卡前读取上次交易数据
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
void DataInit(void)//一般变量初始化
{
		uint8_t i;
		SystemState = 1;      //系统状态：0，空闲状态；1，充电状态 ；2，异常状态；默认为0；初始化为0；根据设置条件切换系统状态
    ChargingMethod  = 0;  //充电方式：0，自动充电；1，手动充电 ；默认为0；初始化为0；屏幕选择手动模式置1
    ChargingMode = 0;     //充电模式：0，充满为止；1，按时间充 ；2.按电量充；3，按金额充；默认为0；初始化为0；屏幕选择
    Timer1CountNumber = 0; //每500ms加1，10s复位，定时器1用
    FlagCycle10s = 0;     //10s循环定时标志位，默认为0；定时器1置位，使用完清零
    FlagCycle1s = 0;      //1s循环定时标志位，默认为0；定时器1置位，使用完清零
    FlagCycle500ms = 0;   //500ms循环定时标志位，默认为0；定时器1置位，使用完清零

    PileTiltState = 0;              //充电桩桩体倾斜状态，0，未倾斜；1，倾斜；默认为0；
    CabinDoorState = 0;        //电源仓仓门状态，0，关闭；1，打开；默认为0；
    EmergencyStopButtonState = 0;   //急停按钮状态状态，0，正常；1，按下；默认为0；
    PileState = 0;                  //枪头状态，0表示未连接，1表示连接     
    SystemFaultState = 0;	
		SwipingCardFlag = 0;
		SpeechFlag = 0;
	  ETHData.IPNumber = 0x0000;
	  ETHData.Length = 0;
	  FlagIfSaveData = 0;       //保存上次交易数据 默认为0 1表示有效
	  for(i=0;i<100;i++)
			{
					ETHData.Data[i] = 0;	
			}
}

void ShowPileFaultState(uint8_t w)//充电桩故障，红灯亮
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

void ShowPileChargeFullOrFree(uint8_t w)//充电桩充满或者空闲 0不亮1亮
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
	//直接读，计算CRC
	
	
}

void SaveIAPSystemParatemer(void)
{
	  //先写入存储器再读出来，校验CRC
	
	
}

uint8_t SystemSelfingCheck(void)//系统自检
{
	uint8_t buf1 = 0xAA;
	uint8_t buf2 = 0;
	uint8_t i = 0;
	SystemState = 0;      //系统状态：0，空闲状态；1，充电状态 ；2，异常状态；默认为0；初始化为0；根据设置条件切换系统状态
	Speech("[v11]欢迎使用卓越直流充电桩");
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
		SystemFaultState = SystemFaultState|0x0100;//存储器异常
	}
	CabinDoorState = CheckCabinDoorState( );
	if(1== CabinDoorState )
	{
		SystemFaultState = SystemFaultState|0x0002;//仓门未关闭
	}
	EmergencyStopButtonState = CheckEmergencyStopButtonState( );
	if(1 == EmergencyStopButtonState)
	{
		SystemFaultState = SystemFaultState|0x0001;//急停按钮被按下�
	}
	LevelState = CheckLevelState( );
	if(1 == LevelState)
	{
		SystemFaultState = SystemFaultState|0x0008;//系统被撞击�
	}
	DS18B20Temperature = DS18B20_Get_Temp();
	if(MAXDS18B20TEMP < DS18B20Temperature)
	{
		SystemFaultState = SystemFaultState|0x4000;//电源仓温度过高
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
		SystemFaultState = SystemFaultState|0x0010;//环境温度过高
	}
	if(MINAM2321TEMP > AM2321Temperature)
	{
		SystemFaultState = SystemFaultState|0x0020;//环境温度过低
	}
	if(MAXAM2321HUMI < Am2321Humidity)
	{
		SystemFaultState = SystemFaultState|0x0040;//环境湿度过高
	}
	if(MINAM2321HUMI > Am2321Humidity)
	{
		SystemFaultState = SystemFaultState|0x0080;//环境湿度过低
	}
	//Read_Time( &RTCReadBCDTime);
	//UpdateRTC();
// 	Send_485(0x10,0x00);
// 	while()
//  Receive_485(0x10,0x00);
//  绝缘电阻检测，等待测试，timeout wait
// 	if(SystemFaultState != 0)
// 	{
// 		SystemState = 2;      //系统状态：0，空闲状态；1，充电状态 ；2，异常状态；默认为0；初始化为0；根据设置条件切换系统状态
// 		Speech("[v11]系统异常");
// 		}
	SystemState = 1;
	ChangePage(0x0001);  //系统自检结束，屏幕跳入主界面
	CheckOut();
	return 0;
}
uint8_t Collect_System_State_Process(void)//采集系统状态子程序
{
	if(1 == FlagCycle500ms)
	{
		//采集绝缘电阻和三相电缺相检测
		//判断绝缘电阻以及三相电是否合格，采集在RS485中进行，500ms采集一次
		FlagCycle500ms = 0;
	}
	//判断DC电源状态,设置DC电源开始工作标志位
	if(1 == FlagCycle1s)//1S检测一次 仓门急停放撞击
	{
		CabinDoorState = CheckCabinDoorState( );
		if(1== CabinDoorState )
		{
			SystemFaultState = SystemFaultState|0x0002;//仓门未关闭
		}
		EmergencyStopButtonState = CheckEmergencyStopButtonState( );
		if(1 == EmergencyStopButtonState)
		{
			SystemFaultState = SystemFaultState|0x0001;//急停按钮被按下
		}
		LevelState = CheckLevelState( );
		if(1 == LevelState)
		{
			SystemFaultState = SystemFaultState|0x0008;//系统被撞击
		}
		FlagCycle1s = 0;
	}
	if(1 == FlagCycle10s)   //10S检测一次  电源仓温度 环境温湿度 保存充电过程 的临时数据
	{
		DS18B20Temperature = DS18B20_Get_Temp();
		if(MAXDS18B20TEMP < DS18B20Temperature)
		{
			SystemFaultState = SystemFaultState|0x4000;//电源仓温度过高
		}
		Read_Sensor( );
		if(MAXAM2321TEMP < AM2321Temperature)
		{
			SystemFaultState = SystemFaultState|0x0010;//环境温度过高
		}
		if(MINAM2321TEMP > AM2321Temperature)
		{
			SystemFaultState = SystemFaultState|0x0020;//环境温度过低
		}
		if(MAXAM2321HUMI < Am2321Humidity)
		{
			SystemFaultState = SystemFaultState|0x0040;//环境湿度过高
		}
		if(MINAM2321HUMI > Am2321Humidity)
		{
			SystemFaultState = SystemFaultState|0x0080;//环境湿度过低
		}
		ETH_Send_HeartbeatMeg();
		FlagCycle10s = 0;
		//Speech("心跳包上传");
		//保存数据,FM存储器保存数据?保存哪些数据
	}
	return 0;
}
void HMI_Event_Process(void)//处理屏幕子程序
{
		TimeCountDownFlag=(uint16_t)((mainselect==1)||(mainselect==4)||(mainselect==3)||(mainselect==6)||(cardstartflag1==1)||(InsertPileFlag==1));	
		if(TimeCountDownFlag)                                   //倒计时
	      Count_Down();
			
		size=queue_find_cmd(cmd_buffer,MAX_CMD_BUFFER);		      //接受数据处理
	  if(size>0)	
	      Message_process();
	  
}
void ETH_Event_Process(void)//处理网络数据子程序
{ 
    IfConnect();
	  ethernetif_input(&gnetif);//有数据包则处理，否则直接退出
	  ETHData = ETH_GetData();
    AnalyzeData();
}  
void ETH_Send_HeartbeatMeg(void)    //上传心跳包
{
	uint8_t sendbuffer[15];
	sendbuffer[0]=0xAA;
	sendbuffer[1]=0xAA;
	sendbuffer[2]=0x01;              //从机地址        
	sendbuffer[3]=0x00;             //桩号
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

void Free_State_Process(void)//空闲状态处理子程序
{
		PileState = CheckPileState();//判断枪是否被拔掉
		if(PileState)
		{
			CAN_mEvent();       //CAN事件处理
		}
		else
		{
			//枪被拔掉，进入结算界面
			//give flag 
		}
		//直接在结束充电按钮部分进行此工作
		if(1 == SpeechFlag)//需要语音,结算请刷卡,只说一次
		{
			Speech("[v11]结账请刷卡");
			SpeechFlag = 0;
		}
		if(1 == SwipingCardFlag)//结束充电按钮被按下,页面超时，则此标志位清除
		{
					if(1 == CardLock())//如果读卡成功
					{
							//判断是否为当前充电卡
							if(1)//
							{
									//CAN通信进入结束充电处理部分，ubCANEventStatus = ；
								//关闭相关继电器
								ResetACRwlay( );//停止交流继电器
								ResetDCRwlay( );//停止直流接触器
								StopFan( );//停止风扇
								ShowPileChargeFullOrFree(0);//充电桩充满或者空闲
								//相关记录存盘，	
								//网络发送相关数据
								//显示屏显示当前消费信息 give flag to HMI
							}
							else
							{	
							//给出相关标志位   读卡成功标志位  通知屏幕程序显示卡号和卡内余额
							SwipingCardFlag = 0;
							SystemState = 1;//进入充电状态
							}
					}			
		}
}

void Charge_State_Process(void)//充电状态处理子程序
{
	uint8_t i;
	uint8_t FlagICStatus = 0 ;
  /************************
  1.充电界面充电模式记录 
	2.根据插枪判断是否进入提示插枪页面
	3.刷卡界面
	4.充电信息界面
******************************/
    //变量
    uint32_t Energy;
    uint16_t DealEnergyLow;
	uint16_t crc=0xffff;
	//流程
	PileState = CheckPileState();
	//提示插枪，语言根据SpeechFlag只说一次
	if((1 == SwipingPileFlag)&&(1 == SpeechFlag))
	{
		Speech("请插入充电插座");
		SpeechFlag = 0;
		OpenDoor();
    FlagOpenDoor = 1;           //打开仓门
	}
  	//扫枪头
	if(1 == SwipingPileFlag)
	{	
			if(1==PileState)
				{
					ChangePage(0x001A);   //页面切换至刷卡界面
					SwipingCardFlag = 1; //进行扫卡动作
					SwipingPileFlag =0;    
					SpeechFlag = 1;			
					cardstartflag1=1;   
				}
	}
	//提示刷卡，语言根据SpeechFlag只说一次
	if((1 == SwipingCardFlag)&&(1 == SpeechFlag))
	{
		  Speech("请刷卡充电");
		  SpeechFlag = 0;
		  ICReadFlag = 0;
			ICWriteFlag = 0;
		  SetBMSRelay();//启动辅助电源
		  //读上次交易
		  HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSaveRealTimeDataHandle.unbuffer,73,0xff);
	}
	//扫卡
	if(1 == SwipingCardFlag)
	{   
			if(0 == ICWriteFlag)
			{
					FlagICStatus = CardLock();	
			}		
	    if((1 == FlagICStatus)&&(1 == SpeechFlagDeal))       //上次交易未完成
			{
					SpeechFlagDeal = 0;
					Speech("上次交易未完成");
					UpDataId();     //上传卡号至屏幕
					UpDataBalance();    //上传余额至屏幕
					TimeCountDownStartCard = 5;
					ChargeFlag = 0; 
			}
			else if((3 ==FlagICStatus)&&(1 == SpeechFlagMoney))           //卡内余额不足
			{
					SpeechFlagMoney = 0;
					Speech("余额不足请立即充值");
				  UpDataId();     //上传卡号至屏幕
		      UpDataBalance();    //上传余额至屏幕
				  TimeCountDownStartCard = 5;
					ChargeFlag = 0; 
			}
			else if((0 ==FlagICStatus))//&&(1 == SpeechFlag))           //没有读到卡号
			{
				   //Speech("刷卡不成功");
			}
			//else if((2 == FlagICStatus)&&(1 == ICWriteFlag))//如果写卡成功  //正确读卡
			if(ICWriteFlag == 1)
				{
						Speech("开始充电");
					  UpDataId();     //上传卡号至屏幕
		        UpDataBalance();    //上传余额至屏幕
					  HAL_Delay(100);
				    WriteBlockFlag = 0;
					  ICWriteFlag = 0;
						ubDCModue = CAN_STARTDCMODUE;
						if(1 == FlagIfSaveToMemory)  //将上次未完成交易写入大存储器
						{
								FlagIfSaveToMemory = 0;
								crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
							  WriteHisRec(unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo,unSaveRealTimeDataHandle.unbuffer,73);				
						}
						//保存起始充电时间
						StartChargeBCDTime.Second = RTCReadBCDTime.Second;    //秒
						StartChargeBCDTime.Minute = RTCReadBCDTime.Minute;   //分
						StartChargeBCDTime.Hour = RTCReadBCDTime.Hour;       //时
						StartChargeBCDTime.Day = RTCReadBCDTime.Day;         //日
						StartChargeBCDTime.Month = RTCReadBCDTime.Month;     //月
						StartChargeBCDTime.Year = RTCReadBCDTime.Year;       //年

						//开始充电阶段需保存的实时数据 
						HAL_I2C_Mem_Read(&I2C1Handle,FM24CL04READADDRESS,0xf0,I2C_MEMADD_SIZE_8BIT,StartEnergy,4,0xff);   //读取存储器电能
						Energy = (ACEnergy[0]<<24) + (ACEnergy[1]<<16) + (ACEnergy[2]<<8) + ACEnergy[3] ;  //电表读出当前时刻电能
						StartEnergyAll = (StartEnergy[0]<<24) + (StartEnergy[1]<<16) + (StartEnergy[2]<<8) + StartEnergy[3] ;  //存储器存储上一次交易结束时的电能
						if(1 == FlagIfSaveData)   //意外导致上次交易未完成
						{
							 FlagIfSaveData = 0;
							
							 StartEnergy[0] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy>>24)&0xff;
							 StartEnergy[1] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy>>16)&0xff;
							 StartEnergy[2] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy>>8)&0xff;
							 StartEnergy[3] = (unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy)&0xff;
						}
						else              //新的一次交易记录
						{   
							  StartICMoneyAll = (StartICMoney[0]<<24) + (StartICMoney[1]<<16) + (StartICMoney[2]<<8) + StartICMoney[3] ;
							  unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo = 1 + unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo; 
							  unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ParkFee = 0;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCounter = 0;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.PileNum = 0;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMode = ChargingMode ;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargMethod = ChargingMethod;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealCardNo = (ICCard1[3]<<24)+(ICCard1[2]<<16)+(ICCard1[1]<<8)+ICCard1[0];     //当前交易卡号
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.OriMoney = StartICMoneyAll;
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartYear = RTCReadBCDTime.Year;                                               //当前交易起始充电时间 年
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMonth = RTCReadBCDTime.Month;                                             //当前交易起始充电时间  月
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartDay = RTCReadBCDTime.Day;                                                  //当前交易起始充电时间  日
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartWeek = RTCReadBCDTime.Week;                                               //当前交易起始充电时间  星期
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartHour = RTCReadBCDTime.Hour;                                                //当前交易起始充电时间  时
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartMin = RTCReadBCDTime.Minute;                                                //当前交易起始充电时间  分
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.StartSec = RTCReadBCDTime.Second;                                                //当前交易起始充电时间  秒
								unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargeFlag = 0xff; 
								if(Energy >= StartEnergyAll)    //当前读出电能大于等于存储器电能 把当前电能作为起始电能
									{
											 for(i=0;i<4;i++)
											{
												StartEnergy[i] = ACEnergy[i];  //鹗嫉缒�
												unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy = Energy;
											}
									}
									else            //当前读到的电能错误
									{
											for(i=0;i<4;i++)   //存储器数据作为起始电能
												{
													 unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStartEnergy = StartEnergyAll;
												}
									}
						}
						UpDataId();     //上传卡号至屏幕
						UpDataBalance();    //上传余额至屏幕
						ChargeFlag = 1;    //正在充电标志
						SwipingCardFlag = 0;                      
						SetACRelay(); //启动交流接触器
						SetDCRelay(); //启动直流接触器
					  ShowPileChargeFullOrFree(0);  //绿灯不亮
						HAL_Delay(500);
						CAN_Config();
						//SetDCRelay(); //启动直流接触器
					  //ShowPileChargeFullOrFree(0);  //绿灯不亮
						//SetACRelay(); //启动交流接触器
						TimeCountDownStartCard = 5;
						if(1 == ChargingMethod)    //手动充电
						{
								//SetChargeCutOffCurrent  ;           //设置截止电流
								//SetChargeVoltage;                 //设置充电电压
							  //SetChargeCurrent;                 //设置充电电?	
						}
				}	
		}
		if(1 == ChargeFlag)
		{
					//if(DS18B20Temperature >=800)
			    if(1)
					{
							StartFan();//启动风扇
					}
// 					else
// 					{
// 							StopFan();//启动风扇
// 					}
					if(1 ==CheckPileState())
						{   		
										AccumulateChargeEnergy();    //计算电能
										AccumulateChargeMoney();     //计算金额
										CAN_mAccumulateChargeTime();   //计算充电时长
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney = StopMoneyAll ;            //交易后余额
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealPrice = unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice; //交易电价
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy = (DealEnergy[2]<<8) + DealEnergy[3];        //交易结束时间（秒）
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear = RTCReadBCDTime.Year;      //交易结束时间（年）
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth = RTCReadBCDTime.Month;     //交易结束时间（月）
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay = RTCReadBCDTime.Day;         //交易结束时间（日）
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek = RTCReadBCDTime.Week;         //交易结束时间（星期）
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour = RTCReadBCDTime.Hour;        //交易结束时间（时）
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin = RTCReadBCDTime.Minute;       //交易结束时间（分）
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec = RTCReadBCDTime.Second;        //交易结束时间（秒）
										crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
										unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
										if(FlagCycle30s == 1)  //30s存储数据实时存储
										{
											HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSaveRealTimeDataHandle.unbuffer,73,0xff);
											//HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x63,I2C_MEMADD_SIZE_8BIT,"ff",1,0xff);
											//ETH_Send_HeartbeatMeg();
											FlagCycle30s = 0;
										}					
									    //计算充电过程中卡内余额不足
										if(DealMoneyAll >= (StartICMoneyAll + (unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice/100)))
										{
											ResetACRwlay();//停止交流继电器
											ResetDCRwlay();//停止直流接触器 
                      ShowPileChargeFullOrFree(1);  //绿灯亮											
										}
										if(0 == ChargingMethod)     //充电方式 0为自动充电，1为手动充电
										{	
													CAN_mEvent();       //CAN事件处理
													if(0 ==ChargingMode)   //自动充满
														{
															
														}
													else  
														if(1 ==ChargingMode)    //按电量充
														{
																//充电电能达到设定电能 SetKwh<=DealEnergy
																DealEnergyLow = (DealEnergy[2]<<8) + DealEnergy[3];
																if(SetKwh <= DealEnergyLow)   //设定电能小于充电电能 关闭继电器
																{
																	 ubDCModue = CAN_STOPDCMODUE;
																	 CAN_mSendDCModuleStartOrStop(0x00);
																	 ResetACRwlay();//停止交流继电器
																	 ResetDCRwlay();//停止直流接触器
																	 ShowPileChargeFullOrFree(1);  //绿灯亮
																	 //StopChargeFlag = 1;   //停止充电标志位
																}
																else
																{
															
																}
														}
													else  
														if(2 ==ChargingMode)    //按金额充
														{
																//设定金额达到已充电金额SetMoney<=DealMoneyAll
																if(SetMoney <= (DealMoneyAll&0xffff))  //已达到充电金额  关闭继电器停止充电
																{
																	 ubDCModue = CAN_STOPDCMODUE;
																	 CAN_mSendDCModuleStartOrStop(0x00);
																	 ResetACRwlay();//停止交流继电器
																	 ResetDCRwlay();//停止直流接触器
																	 ShowPileChargeFullOrFree(1);  //绿灯亮
																	 //StopChargeFlag = 1;   //停止充电标志位
																}
																else    
																{
																
																}
														}
													else  
														if(3 ==ChargingMode)     //按时间充
														{
															if((SetHour <= ChargeTime.Hour)&&(SetMin <= ChargeTime.Minute))  //充电时长到达设定值
															{
															   ubDCModue = CAN_STOPDCMODUE;
															   CAN_mSendDCModuleStartOrStop(0x00);
															   ResetACRwlay();//停止交流继电器
															   ResetDCRwlay();//停止直流接触器
															   ShowPileChargeFullOrFree(1);  //绿灯亮
															   //StopChargeFlag = 1;   //停止充电标志位
															}
															else
															{
																
															}
														}
										 }	
										 else	
										 if(1 == ChargingMethod)        //手动充电，充电电压充电电流截止电流
										 {
													//SetChargeCutOffCurrent;           //设置截止电流
													//SetChargeVoltage;                 //设置充电电压
													//SetChargeCurrent;                 //设置充电电?
													if((10*SetChargeCutOffCurrent) >= uwOutputCurrentValue)        //判断充电电流和截止电流的大小关系
											    {
																 ubDCModue = CAN_STOPDCMODUE;
															   CAN_mSendDCModuleStartOrStop(0x00);
													}
										 } 
												
						}
						else    //防止拔枪
						{
										StopChargeFlag = 1;   //停止充电标志位
										ResetACRwlay();//停止交流继电器
										ResetDCRwlay();//停止直流接触器
										ResetBMSRwlay();//停止辅助电源
							 		  ShowPileChargeFullOrFree(1);  //绿灯亮
										FaultOperation = 1;  //错误操作标志位
						}
		}
		if((1 == StopChargeFlag)&&(1==SpeechFlag)&&(1 == ChargeFlag))   //刷卡结束充电
		{
						SpeechFlag = 0;
						ICReadFlag = 0;
						Speech("刷卡结束充电");
						ICWriteFlag = 0;
						//FlagIfOperationCard = 1;
		}
		if(((1 == StopChargeFlag)&&(1 == ChargeFlag))||(1 == FaultOperation))   //结束充电
		{
					//if((1 == CardRead())||(1 == FaultOperation))
			    if(0 == ICWriteFlag)
					{
							FlagICStatus = CardunLock();	
					}
					//if((2 == FlagICStatus)||(1 == FaultOperation))    //刷卡成功 ，错误操作直接不需要刷卡结束充电
					if((1== ICWriteFlag)||(1 == FaultOperation))    //刷卡成功 ，错误操作直接不需要刷卡结束充电
					{
									FlagShowInformation = 1;
									ubDCModue = CAN_STOPDCMODUE;
					      	CAN_mSendDCModuleStartOrStop(0x00);
									HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0xf0,I2C_MEMADD_SIZE_8BIT,ACEnergy,4,0xff);  //结束充电电能写入存储器 
									Speech("本次交易完成，谢谢使用");
						      StopFan();//启动风扇
									//DataInit(); 
									ChargeFlag = 0;
									StopChargeFlag = 0;
									ChargingMode = 0;
									ChargingMethod = 0;
									ResetACRwlay();//停止交流继电器
									ResetDCRwlay();//停止直流接触器
						      ShowPileChargeFullOrFree(1);  //绿灯亮
					      	ubCANEventStatus = 16;
									CAN_mSend_CST_ChargerStop();
									//ResetBMSRwlay();//停止辅助电源
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.FinMoney = StopMoneyAll ;            //交易后余额
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealPrice = unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice; //交易电价
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealEnergy = (DealEnergy[2]<<8) + DealEnergy[3];        //交易结束时间（秒）
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealStopEnergy = (ACEnergy[0]<<24) + (ACEnergy[1]<<16) + (ACEnergy[2]<<8) + ACEnergy[3];
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndYear = RTCReadBCDTime.Year;      //交易结束时间（年）
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMonth = RTCReadBCDTime.Month;     //交易结束时间（月）
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndDay = RTCReadBCDTime.Day;         //交易结束时间（日）
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndWeek = RTCReadBCDTime.Week;         //交易结束时间（星期）
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndHour = RTCReadBCDTime.Hour;        //交易结束时间（时）
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndMin = RTCReadBCDTime.Minute;       //交易结束时间（分）
									unSaveRealTimeDataHandle.SaveRealTimeDataHandle.EndSec = RTCReadBCDTime.Second;        //交易结束时间（秒）
									if(1 == ICWriteFlag)     //正确操作0x00
									{
										  ICWriteFlag = 0;
										  unSaveRealTimeDataHandle.SaveRealTimeDataHandle.ChargeFlag = 0x00; 
											//HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x63,I2C_MEMADD_SIZE_8BIT,0x00,1,0xff);	
                      //写至大存储器
											crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
										  HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,unSaveRealTimeDataHandle.unbuffer,73,0xff);
                      WriteHisRec(unSaveRealTimeDataHandle.SaveRealTimeDataHandle.DealNo,unSaveRealTimeDataHandle.unbuffer,73);										
									}										
									else if(1 == FaultOperation)   //错误操作0xff
									{
											FaultOperation = 0;
										  //HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS,0x63,I2C_MEMADD_SIZE_8BIT,"ff",1,0xff);
									//}
											crc = CalcCrc(unSaveRealTimeDataHandle.unbuffer,71);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCHigh = (uint8_t)((crc>>8)&0xff);
											unSaveRealTimeDataHandle.SaveRealTimeDataHandle.CRCLow = (uint8_t)(crc&0xff);
											//结束充电时写数据至存储器
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
						ResetBMSRwlay();//停止辅助电源
						Clr_Data();   //充电过程数据清零 主要是屏幕上显示的数据
						CAN_Config();  //CAN 初始化
						CAN_mInitData();   //CAN数据初始化
						ChangePage(0x0001);  //屏幕跳入主界面
				}
		}
}
void Error_State_Process(void)//异常处理子程序
{
	ShowPileFaultState(1);//异常灯亮
	//屏幕显示系统异常flag
	//网络通信，通知服务器，系统异常
}
