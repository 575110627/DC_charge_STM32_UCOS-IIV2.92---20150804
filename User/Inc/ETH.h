#ifndef  _MODBUS_SLAVE_H_
#define  _MODBUS_SLAVE_H_

#include "main.h"
#include "stm32f2xx_hal.h"

void AnalyzeData(void);            //��������
void AnalyzeBroadcast(void);        //�����㲥֡
void AnalyzeReceive(void);
void Broadcast(void);   
void ReceiveBroadcastBlacklist(void);
void ReceiveBroadcastSystemData1(void);
void ReceiveBroadcastSystemData2(void);
void ReceiveFunction04(void);
void ReceiveFunction10(void);
void ReceiveBroadcastTime(void);
void ReplyToHostPoSlaveStaframe(void);
void ReplyToHostReadTotalTranData1(void);
void ReplyToHostReadTotalTranData2(void);
void ReplyToHostReadTotalTranData3(void);
void SendTradingSerialNumber(void);
#endif
