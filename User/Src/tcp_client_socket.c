/**
  ******************************************************************************
  * @file    tcp_client_socket.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    06-June-2011  
  * @brief   Basic http server implementation using LwIP socket API   
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
  
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "main.h"
//#include "fs.h"
//#include "fsdata.h"
#include "string.h"

//TCP�ͻ����������ȼ�
#define TCPCLIENT_THREAD_PRIO    ( tskIDLE_PRIORITY + 3 )

#define BUF_SIZE                32
char ClientRevBuf[BUF_SIZE]; 	//���ջ�����
/********************************************************************************************
*�������ƣ�void TCPClient(void *arg)
*��ڲ�������
*���ڲ�������
*����˵����tcp�ͻ�������
*******************************************************************************************/
void TCPClient(void *arg)
{
    struct sockaddr_in ServerAddr;
	int sock ;    
	int datalen;
    int ret;

	while(1)
    {
        //�����ͻ���
        sock = lwip_socket( AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(sock == -1 ){
                continue;
        }
        
        ServerAddr.sin_family = AF_INET;
        ServerAddr.sin_addr.s_addr = inet_addr("192.168.1.203");	//���ӷ�������ַ
        ServerAddr.sin_port = htons(8080);		//�������˿ں�
        
        //���ӷ�����
        if( lwip_connect(sock,(struct sockaddr*)&ServerAddr,sizeof(ServerAddr) ) == - 1)
        {
            lwip_close(sock);		//����ʧ�ܣ��رտͻ���
            continue;
        }
        while(1)        
        {            
            datalen = lwip_recv(sock,ClientRevBuf,BUF_SIZE,0);	//��������
            if(datalen > 0)
            {
                //���յ�����,���ԣ����͸�������
                ret = lwip_send(sock,ClientRevBuf,datalen,0);                    
            }
			else
			{
                //�������رյ��쳣
                lwip_close(sock);
                break;	//����������               
            }
                                    
        }        
    }
}


/********************************************************************************************
*�������ƣ�void tcp_client_socket_init(void)
*��ڲ�������
*���ڲ�������
*����˵������ʼ��tcp�ͻ���
*******************************************************************************************/
void tcp_client_socket_init()
{
  sys_thread_new("TCP", TCPClient, NULL, DEFAULT_THREAD_STACKSIZE * 2, TCPCLIENT_THREAD_PRIO);
}

