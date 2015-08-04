/******************** (C) COPYRIGHT 2013 ********************
* 文件名          : PICC.h
* 作者            : 优灵电子开发团队
* 版本            : V2.0.1.0
* 日期            : 2013-5-10
* 描述            : 文件 PICC.c 的头文件
* 技术支持        : QQ: 526181679
*********************************************************************************/

#ifndef _PICC_H
#define _PICC_H

/* 包含文件 -------------------------------------------------------------------*/
/* 外部类型 -------------------------------------------------------------------*/
typedef struct _CMD
{
  unsigned char ReceiveBuffer[32];
	unsigned char SendBuffer[32];
  unsigned char ReceivePoint;
	unsigned char SendPoint;
	unsigned char SendFlag;
  unsigned char ReceiveFlag;
}CMD;

typedef struct _PICC
{
	unsigned int Type;
	unsigned long UID;
	unsigned long Value;
	unsigned char KeyA[6];
	unsigned char KeyB[6];
	unsigned char BlockData[16];
}PICC;

/* 外部变量 -------------------------------------------------------------------*/
extern PICC Picc;
extern CMD Cmd;
/* 外部函数 ------------------------------------------------------------------ */
void ReadCard(void);
void ReadBlock(unsigned char block);
void WriteBlock(unsigned char block, unsigned char* data);
void MakeCard(unsigned char block, unsigned char value);
void SetDefault(unsigned char block);
void Inc(unsigned char block, unsigned char value);
void Dec(unsigned char block, unsigned char value);
void LodeKey(unsigned char* KeyA, unsigned char* KeyB, unsigned char mode);
void SetBaud(unsigned long baud);
void SetAddress(unsigned char address);
void SetMode(unsigned char mode, unsigned char block, unsigned long value);
unsigned char CheckSum(unsigned char *dat, unsigned char num);
unsigned char CommandProcess(void);


void SendCommand(void);
unsigned char CheckSum(unsigned char *dat, unsigned char num);
void PiccTest1(void);
void PiccTest2(void);
void PiccTest3(void);
void PiccTest4(void);
void PiccTest5(void);
void PiccTest6(void);
void PiccTest7(void);
void PiccTest8(void);
void PiccTest9(void);
void PiccTest10(void);

unsigned char CardRead(void);
unsigned char CardLock(void);
unsigned char CardunLock(void);

void USART6_UART_Init(void);
#endif

