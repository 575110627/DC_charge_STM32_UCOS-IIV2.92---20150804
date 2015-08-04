#include "stm32f2xx_hal.h"
typedef struct _QUEUE
{
    qsize _head;
    qsize _tail;
    qdata _data[QUEUE_MAX_SIZE];
} QUEUE;//�������ݽ��ն��нṹ��

static QUEUE que = {0,0,0};
static qdata cmd_state = 0;
static qsize cmd_pos = 0;
/**********************************************
*��    �ƣ� queue_reset
*��    ��   �Զ��л��������и�λ���������
**********************************************/

void queue_reset()
{
    que._head = que._tail = 0;
    cmd_pos = cmd_state = 0;
}

/**********************************************
*��    �ƣ� queue_push
*��    ��   ����һ�����ݵ����ڽ��ն��У�FIFO����
**********************************************/
void queue_push(qdata _data)
{   
    qsize pos = (que._head+1)%QUEUE_MAX_SIZE;
    if(pos!=que._tail)//����״̬
    {
        que._data[que._head] = _data;
        que._head = pos;
    }
}
/**********************************************
*��    �ƣ� queue_pop
*��    ��   �Ӵ��ڽ��ն���(FIFO)��ȡ��һ���ֽ�����
**********************************************/
void queue_pop(qdata* _data)
{
    if(que._tail!=que._head)//�ǿ�״̬
    {
        *_data = que._data[que._tail];
        que._tail = (que._tail+1)%QUEUE_MAX_SIZE;
    }
}
/**********************************************
*��    �ƣ� queue_size
*��    ��   ��ȡ���ڽ��ն���(FIFO)�ĳ���
**********************************************/
qsize queue_size()
{ 
	return ((que._head+QUEUE_MAX_SIZE-que._tail)%QUEUE_MAX_SIZE);
}

/******************************************************************
*��    �ƣ� queue_find_cmd
*��    ��   ��ȡ���ڽ��ն���(FIFO)��һ��������֡
            һ��������֡����13,14��ͷ��Ȼ�������cmd_len�����ݵ�һ������֡
********************************************************************/
qsize queue_find_cmd(qdata *buffer,qsize buf_len)
{
		
    qsize cmd_size = 0;
    qdata _data = 0;
    while((queue_size())>0)
	{
		//ȡһ������
		queue_pop(&_data);
		if(cmd_pos==0&&_data!=0x13)//֡ͷ��������
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
	    if(cmd_pos<buf_len)//��ֹ���������
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
    return 0;//û���γ�������һ֡
}
