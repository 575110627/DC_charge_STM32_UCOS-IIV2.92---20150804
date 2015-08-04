#ifndef __CMD_QUEUE_H
#define __CMD_QUEUE_H
#include "stm32f2xx_hal.h"

//��������С������Դ���������£��������ô�һЩ����ֹ���
#define QUEUE_MAX_SIZE 500


#define CTRL_RES_WR  				0x80
#define CTRL_RES_RD  				0x81
#define VARIABLE_MEMORY_WR  0x82
#define VARIABLE_MEMORY_RD  0x83
#define CURVE_BUF_WR        0x84


#define ADR 0xFFA0 

/*
���紮�ڷ���13 14֡ͷ������16λ����cmd_head,�����8λ�������8λ�������ͺ��������ȷ���8λ���󷢵�8λ��������ɷ���14��13.
����д�˺�������PTR[0]����8λ����8λ��PTR[1]����8λ
*/

#define PTRU16(PTR) ((((uint8_t *)(PTR))[0]<<8)|((uint8_t *)(PTR))[1]) 
#define PTRU32(PTR) (((uint8_t *)(PTR))[0]<<24)|(((uint8_t *)(PTR))[1]<<16)|(((uint8_t *)(PTR))[2]<<8)|(((uint8_t *)(PTR))[3])

typedef unsigned char qdata;
typedef unsigned short qsize;

#pragma pack(push)
#pragma pack(1) //̫��Ҫ���������û���ֽڶ�������

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
* ��    �ƣ� queue_reset()
* ��    �ܣ� ��λָ����ջ��������������
* ��ڲ����� ��
* ���ڲ����� ��
****************************************************************************/
extern void queue_reset(void);

/****************************************************************************
* ��    �ƣ� queue_push()
* ��    �ܣ� ���һ�����ݵ�ָ�����
* ��ڲ����� _data-ָ������
* ���ڲ����� ��
****************************************************************************/
extern void queue_push(qdata _data);

/****************************************************************************
* ��    �ƣ� queue_find_cmd
* ��    �ܣ� ��ָ������в���һ��������ָ��
* ��ڲ����� poffset-ָ��Ŀ�ʼλ�ã�psize-ָ����ֽڴ�С
* ���ڲ����� ��
****************************************************************************/
extern qsize queue_find_cmd(qdata *buffer,qsize buf_len);

#endif
