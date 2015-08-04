#include "stm32f2xx_hal.h"
typedef struct _QUEUE
{
    qsize _head;
    qsize _tail;
    qdata _data[QUEUE_MAX_SIZE];
} QUEUE;//串口数据接收队列结构体

static QUEUE que = {0,0,0};
static qdata cmd_state = 0;
static qsize cmd_pos = 0;
/**********************************************
*名    称： queue_reset
*功    能   对队列缓存区进行复位，清空数据
**********************************************/

void queue_reset()
{
    que._head = que._tail = 0;
    cmd_pos = cmd_state = 0;
}

/**********************************************
*名    称： queue_push
*功    能   存入一个数据到串口接收队列（FIFO）中
**********************************************/
void queue_push(qdata _data)
{   
    qsize pos = (que._head+1)%QUEUE_MAX_SIZE;
    if(pos!=que._tail)//非满状态
    {
        que._data[que._head] = _data;
        que._head = pos;
    }
}
/**********************************************
*名    称： queue_pop
*功    能   从串口接收队列(FIFO)中取出一个字节数据
**********************************************/
void queue_pop(qdata* _data)
{
    if(que._tail!=que._head)//非空状态
    {
        *_data = que._data[que._tail];
        que._tail = (que._tail+1)%QUEUE_MAX_SIZE;
    }
}
/**********************************************
*名    称： queue_size
*功    能   获取串口接收队列(FIFO)的长度
**********************************************/
qsize queue_size()
{ 
	return ((que._head+QUEUE_MAX_SIZE-que._tail)%QUEUE_MAX_SIZE);
}

/******************************************************************
*名    称： queue_find_cmd
*功    能   获取串口接收队列(FIFO)中一个完整的帧
            一个完整的帧是以13,14开头，然后后面有cmd_len个数据的一个数据帧
********************************************************************/
qsize queue_find_cmd(qdata *buffer,qsize buf_len)
{
		
    qsize cmd_size = 0;
    qdata _data = 0;
    while((queue_size())>0)
	{
		//取一个数据
		queue_pop(&_data);
		if(cmd_pos==0&&_data!=0x13)//帧头出错，跳过
			continue;
		if(cmd_pos==0&&_data==0x13)
			cmd_state=1;
		if(cmd_pos==1&&_data==0x14)
			cmd_state=2;
		else	
		{
			if(cmd_pos==1)
			{	
			  queue_reset();
				break;
			}
		}
	    if(cmd_pos<buf_len)//防止缓冲区溢出
		{ 
			buffer[cmd_pos++] = _data;
			if(buffer[2]==6)
			{
				if(cmd_pos==9)
			{
						cmd_size = cmd_pos;
	          cmd_state = 0;
	          cmd_pos = 0;
						queue_reset();
	          return cmd_size;
		   }
		 }
		 if(buffer[2]==8)
		 {
			 if(cmd_pos==11)
			{
						cmd_size = cmd_pos;
	          cmd_state = 0;
	          cmd_pos = 0;
						queue_reset();
	          return cmd_size;
		   }
			}
		}	
    }
    return 0;//没有形成完整的一帧
}
