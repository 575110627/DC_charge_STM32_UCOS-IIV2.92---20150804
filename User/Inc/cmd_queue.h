#ifndef __CMD_QUEUE_H
#define __CMD_QUEUE_H
#include "stm32f2xx_hal.h"

//缓冲区大小，在资源容许的情况下，尽量设置大一些，防止溢出
#define QUEUE_MAX_SIZE 500


#define CTRL_RES_WR  				0x80
#define CTRL_RES_RD  				0x81
#define VARIABLE_MEMORY_WR  0x82
#define VARIABLE_MEMORY_RD  0x83
#define CURVE_BUF_WR        0x84


#define ADR 0xFFA0 

/*
比如串口发送13 14帧头，存入16位变量cmd_head,先入低8位，后入高8位，而发送函数定义先发高8位，后发低8位，所以造成返回14，13.
所以写了函数，将PTR[0]左移8位到高8位，PTR[1]到低8位
*/

#define PTRU16(PTR) ((((uint8_t *)(PTR))[0]<<8)|((uint8_t *)(PTR))[1]) 
#define PTRU32(PTR) (((uint8_t *)(PTR))[0]<<24)|(((uint8_t *)(PTR))[1]<<16)|(((uint8_t *)(PTR))[2]<<8)|(((uint8_t *)(PTR))[3])

typedef unsigned char qdata;
typedef unsigned short qsize;

#pragma pack(push)
#pragma pack(1) //太重要，解决了我没加字节对齐问题

typedef struct
{   	 
		uint16_t    cmd_head;
		uint8_t     data_len;
		uint8_t		  cmd_type;
		uint16_t    adr;
		uint8_t     rd_len;
		uint16_t    data[256];		
}CTRL_MSG,*PCTRL_MSG;
#pragma pack(pop)

/****************************************************************************
* 名    称： queue_reset()
* 功    能： 复位指令接收缓冲区，清空数据
* 入口参数： 无
* 出口参数： 无
****************************************************************************/
extern void queue_reset(void);

/****************************************************************************
* 名    称： queue_push()
* 功    能： 添加一个数据到指令缓冲区
* 入口参数： _data-指令数据
* 出口参数： 无
****************************************************************************/
extern void queue_push(qdata _data);

/****************************************************************************
* 名    称： queue_find_cmd
* 功    能： 从指令缓冲区中查找一条完整的指令
* 入口参数： poffset-指令的开始位置，psize-指令的字节大小
* 出口参数： 无
****************************************************************************/
extern qsize queue_find_cmd(qdata *buffer,qsize buf_len);

#endif
