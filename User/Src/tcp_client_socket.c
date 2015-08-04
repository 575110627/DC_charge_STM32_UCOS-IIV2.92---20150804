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

//TCP客户端任务优先级
#define TCPCLIENT_THREAD_PRIO    ( tskIDLE_PRIORITY + 3 )

#define BUF_SIZE                32
char ClientRevBuf[BUF_SIZE]; 	//接收缓冲区
/********************************************************************************************
*函数名称：void TCPClient(void *arg)
*入口参数：无
*出口参数：无
*功能说明：tcp客户端任务
*******************************************************************************************/
void TCPClient(void *arg)
{
    struct sockaddr_in ServerAddr;
	int sock ;    
	int datalen;
    int ret;

	while(1)
    {
        //创建客户端
        sock = lwip_socket( AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(sock == -1 ){
                continue;
        }
        
        ServerAddr.sin_family = AF_INET;
        ServerAddr.sin_addr.s_addr = inet_addr("192.168.1.203");	//连接服务器地址
        ServerAddr.sin_port = htons(8080);		//服务器端口号
        
        //连接服务器
        if( lwip_connect(sock,(struct sockaddr*)&ServerAddr,sizeof(ServerAddr) ) == - 1)
        {
            lwip_close(sock);		//连接失败，关闭客户端
            continue;
        }
        while(1)        
        {            
            datalen = lwip_recv(sock,ClientRevBuf,BUF_SIZE,0);	//接收数据
            if(datalen > 0)
            {
                //接收的数据,测试，回送给服务器
                ret = lwip_send(sock,ClientRevBuf,datalen,0);                    
            }
			else
			{
                //服务器关闭等异常
                lwip_close(sock);
                break;	//重连服务器               
            }
                                    
        }        
    }
}


/********************************************************************************************
*函数名称：void tcp_client_socket_init(void)
*入口参数：无
*出口参数：无
*功能说明：初始化tcp客户端
*******************************************************************************************/
void tcp_client_socket_init()
{
  sys_thread_new("TCP", TCPClient, NULL, DEFAULT_THREAD_STACKSIZE * 2, TCPCLIENT_THREAD_PRIO);
}

