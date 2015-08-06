/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/Src/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-March-2014
  * @brief   This example describes how to configure and use GPIOs through 
  *          the STM32F2xx HAL API.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_cfg.h"
#include "cpu_core.h"
#include  "cpu_cfg.h"
#include "ucos_ii.h"
#include <includes.h>
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/lwip_timers.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "app_ethernet.h"
//#include "tcp_echoserver.h"
#include "tcp_echoclient.h"

/** @addtogroup STM32F2xx_HAL_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStruct;   //GPIO结构体
/* UART handler declaration */
TIM_HandleTypeDef    TimHandle2;
TIM_HandleTypeDef    TimHandle3;
TIM_HandleTypeDef    TimHandle4;
TIM_HandleTypeDef    TimHandle5;
UART_HandleTypeDef   UartHandle1;
UART_HandleTypeDef   UartHandle3;
UART_HandleTypeDef   huart3;
UART_HandleTypeDef   UartHandle4;
UART_HandleTypeDef   UartHandle5;
UART_HandleTypeDef   UartHandle6;
I2C_HandleTypeDef    I2C1Handle;
CAN_HandleTypeDef    CanHandleToBMS;
CAN_HandleTypeDef    CanHandleToDC;

strETHData_Typedef ETHData;
struct netif gnetif;

uint8_t pData[1];
uint8_t USART1_Buffer[10];
uint8_t USART1_Length=0;
uint8_t SPIWriteBuff[600];
uint8_t SPIReadBuff[600];
/* Private function prototypes -----------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

static void SystemResource_Config(void);
static void GPIO_Config(void);
static void SystemClock_Config(void);


static void BSP_Config(void);
static void Netif_Config(void);


static void TIM_Config(void);
static void ModulesInit(void );//各模块初始化


OS_STK	SYSMonitorTaskStack[SYSMonitorTaskStackLengh]= {0};
OS_STK	LED1TaskStack[LED1TaskStackLengh]= {0};
OS_STK	LED2TaskStack[LED2TaskStackLengh]= {0};
OS_STK	LED3TaskStack[LED3TaskStackLengh]= {0};
void SYSMonitorTask(void *pdata);			    //
void LED1Task(void *pdata);			    //
void LED2Task(void *pdata);			    //
void LED3Task(void *pdata);			    //
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t FlagCycle2s =0;
	//uint16_t i,j,k;
	uint16_t crc= 0xffff;
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();    //配置系统时钟
	/* -1- Enable GPIOG, GPIOC and GPIOI Clock (to be able to program the configuration registers) */
	//SystemResource_Config();//系统资源配置	
    GPIO_Config();
		/* Configure the BSP */
  //BSP_Config(); 
  /* Initilaize the LwIP stack */
  //lwip_init();
  
  /* Configure the Network interface */
  //Netif_Config();  
  
  /* tcp echo server Init */
  //tcp_echoserver_init();
	//tcp_echoclient_connect();
  //tcp_echoclient_connect();
  /* Notify user about the netwoek interface config */
  //User_notification(&gnetif);
	
	//ModulesInit( );//各模块初始化
	//HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7 , GPIO_PIN_SET);	
	//SystemSelfingCheck( );
	//HAL_TIM_Base_Start_IT(&TimHandle2);
	//HAL_TIM_Base_Start_IT(&TimHandle3);
	//CURRENT = 4000;
	//Read_Sensor( );	
	//unSystemParameterConfigHandle.SystemParameterConfigHandle.APrice = 200;
	//Fun485Flag=3;
	//Cmd.SendFlag = 0;
	//Cmd.ReceiveFlag = 0;
	//Picc.Value = 0;
  //ShowPileChargeFullOrFree(1);
	//ReadSystemParameter();
    /*
	unIAPSystemParameter.IAPSystemParameter.IAPState = 2;
	
	unIAPSystemParameter.unbuffer[1] = 1;
	unIAPSystemParameter.unbuffer[2] = 14;
	unIAPSystemParameter.unbuffer[3] = 16;
	unIAPSystemParameter.unbuffer[4] = 17;
	//crc = CalcCrc(unIAPSystemParameter.unbuffer,5);
	unIAPSystemParameter.IAPSystemParameter.CRCHigh = (uint8_t)((crc>>8)&0xff);
	unIAPSystemParameter.IAPSystemParameter.CRCLow = (uint8_t)(crc&0xff);
	//HAL_I2C_Mem_Write(&I2C1Handle,FM24CL04WRITEADDRESS+2,0x64,I2C_MEMADD_SIZE_8BIT,unIAPSystemParameter.unbuffer,7,0xff);
    */
    OSInit();
    OSTaskCreate (SYSMonitorTask,(void *)0,
                  &SYSMonitorTaskStack[SYSMonitorTaskStackLengh-1],
                  SYSMonitorTask_PRIO);
    OSStart();
  

/* 
 while (1)
  {
	  if(CheckEmergencyStopButtonState())   //急停按钮被按下
			{
				ShowPileFaultState(1);
				ubDCModue = CAN_STOPDCMODUE;
				CAN_mSendDCModuleStartOrStop(0x00);
				ResetACRwlay( ); //启动交流接触器
				ResetDCRwlay(); //启动直流接触器
				ResetBMSRwlay(); //启动直流接触器
			}
		else
			{
				ShowPileFaultState(0);
			}
	  ETH_Event_Process();
    sys_check_timeouts();
 	  CommandProcess();  //读卡数据处理
	  Read_Time( &RTCReadBCDTime);
 		Fun_485();
		HMI_Event_Process();
    Collect_System_State_Process();//采集系统状态子程序		
		//ReadHisRec(5,ReadBuffer ,100) ;
 	  switch(SystemState)
 	  {
			case 0: //Free_State_Process();
					break;
			case 1: Charge_State_Process( );
					break;
			case 2: //Error_State_Process( );
					break;
			default:break;
     }
			if(FlagIfCycleSend1000ms)
			{
					FlagIfCycleSend1000ms = 0;
					UpDateTemp((AM2321Temperature-400)/10);                  //更新温度
					UpDateHumi((Am2321Humidity)/10);                         //更新湿度
					CheckOut();
					FlagCycle2s ++;
					if(FlagCycle2s %2)
					{
						Fun485Flag=3;
					}
					else
					{
						Fun485Flag=8;
						FlagCycle2s = 0;
					}
					if(1 == FlagShowInformation)
					{
							FlagIfShowInformation5S ++ ;
					}
					FlagIfCloseDoor ++;
				  if(FlagIfCloseDoor >=5)
					{
							CloseDoor();    //关闭仓门
							FlagOpenDoor = 0;
							FlagIfCloseDoor = 0;
					}
					//ETH_Send_HeartbeatMeg();
					HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7);	
			}
   }
   */
	 
}
static void SystemResource_Config(void)
{
	I2C_Config();
	GPIO_Config();
	CAN_Config();	
    TIM_Config();
	SPI_Config();
	USART6_UART_Init( );
	USART1_UART_Init( );
	SPI_Init();
}
static void ModulesInit(void)//各模块初始化
{
	PM_Init();
	MX_USART3_UART_Init();
	//MX_UART5_UART_Init();
	DS18B20_Init();
	CAN_mInitData();//CAN通信中数据初始化
	DataInit( );
}
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 120000000
  *            HCLK(Hz)                       = 120000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 240
  *            PLL_P                          = 2
  *            PLL_Q                          = 5
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

/**
  * @brief  Configurates the BSP.
  * @param  None
  * @retval None
  */
static void BSP_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   
   __GPIOA_CLK_ENABLE();
   
  GPIO_InitStructure.Pin = GPIO_PIN_3;
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL ;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0xf, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); 
  
  /* Initialize STM322xG-EVAL's LEDs */
  //BSP_LED_Init(LED1);
  //BSP_LED_Init(LED2);
  //BSP_LED_Init(LED3);
  //BSP_LED_Init(LED4);
  
  /* Set Systick Interrupt to the highest priority */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0x0, 0x0);
}

/**
  * @brief  Configurates the network interface
  * @param  None
  * @retval None
  */
static void Netif_Config(void)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
  
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
  
  /* add the network interface */    
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
  
  /*  Registers the default network interface */
  netif_set_default(&gnetif);
  
  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }
  
  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&gnetif, ethernetif_update_config);
}





#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
 // while (1)
  {
  }
}
#endif

/**
  * @brief  Configures the GPIO.
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__GPIOE_CLK_ENABLE();
	__GPIOF_CLK_ENABLE();
	__GPIOG_CLK_ENABLE();
	//GPIOA
	GPIO_InitStruct.Pin = (GPIO_PIN_15|GPIO_PIN_6|GPIO_PIN_4);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = (GPIO_PIN_5);
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//GPIOB
	GPIO_InitStruct.Pin = (GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_1);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1, GPIO_PIN_SET);   //黄灯初始化不亮
	//GPIOC
	GPIO_InitStruct.Pin = (GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_2);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2, GPIO_PIN_SET);     //交流接触器初始化断开
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2, GPIO_PIN_SET);     //交流接触器初始化断开
	//GPIOD
	GPIO_InitStruct.Pin = (GPIO_PIN_3|GPIO_PIN_12);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	//GPIOE
	GPIO_InitStruct.Pin = (GPIO_PIN_3|GPIO_PIN_5);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = (GPIO_PIN_4);
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	//GPIOF
	GPIO_InitStruct.Pin = (GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_14);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10, GPIO_PIN_SET);      //初始化辅助电源继电器断开
	//GPIOG
  GPIO_InitStruct.Pin = (GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5|GPIO_PIN_9);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/**
  * @brief  Configures the UART.
  * @param  None
  * @retval None
  */


/**
  * @}Config TIM 
  */ 
void TIM_Config()
{
  TimHandle2.Instance = TIM2;
	 
	/* Initialize TIM1 peripheral as follow:
			 + Period = 10000 - 1
			 + Prescaler = ((SystemCoreClock/2)/10000) - 1
			 + ClockDivision = 0
			 + Counter direction = Up
	*/
	TimHandle2.Init.Period = 5000 - 1;
	TimHandle2.Init.Prescaler = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
	TimHandle2.Init.ClockDivision = 0;
	TimHandle2.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TimHandle2);	
	HAL_TIM_Base_Start_IT(&TimHandle2);
	
	
	TimHandle3.Instance = TIM3;
			 
	/* Initialize TIM3 peripheral as follow:
			 + Period = 10000 - 1
			 + Prescaler = ((SystemCoreClock/2)/10000) - 1
			 + ClockDivision = 0
			 + Counter direction = Up
	*/
	TimHandle3.Init.Period = 100 - 1;
	TimHandle3.Init.Prescaler = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
	TimHandle3.Init.ClockDivision = 0;
	TimHandle3.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TimHandle3);

	
	TimHandle4.Instance = TIM4;
	/* Initialize TIM4 peripheral as follow:
			 + Period = 10000 - 1
			 + Prescaler = ((SystemCoreClock/2)/10000) - 1
			 + ClockDivision = 0
			 + Counter direction = Up
	*/
	TimHandle4.Init.Period = 30-1;
	TimHandle4.Init.Prescaler = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
	TimHandle4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;;
	TimHandle4.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TimHandle4);
	//HAL_TIM_Base_Start_IT(&TimHandle4);
	
	TimHandle5.Instance = TIM5;
	 
	/* Initialize TIM1 peripheral as follow:
			 + Period = 10000 - 1
			 + Prescaler = ((SystemCoreClock/2)/10000) - 1
			 + ClockDivision = 0
			 + Counter direction = Up
	*/
	TimHandle5.Init.Period = 10000 - 1;
	TimHandle5.Init.Prescaler = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
	TimHandle5.Init.ClockDivision = 0;
	TimHandle5.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TimHandle5);	
	HAL_TIM_Base_Start_IT(&TimHandle5);
	
	
}



/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_3)
  {
    ethernetif_set_link(&gnetif);
  }
}

/*********************************************************************************************************
** Function name:       SYSMonitorTask
** Descriptions:        主任务，该任务可用来创建其他任务
** input parameters:    pvData: 没有使用
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void SYSMonitorTask(void *pvData)
{
    uint8_t err = 0;
    //
    pvData = pvData;
    OSTaskCreate (LED1Task,(void *)0,
                  &LED1TaskStack[LED1TaskStackLengh-1],
                  LED1Task_PRIO);

    OSTaskCreate (LED2Task,(void *)0,
                  &LED2TaskStack[LED2TaskStackLengh-1],
                  LED2Task_PRIO);

    OSTaskCreate (LED3Task,(void *)0,
                  &LED3TaskStack[LED3TaskStackLengh-1],
                  LED3Task_PRIO);
    /******************************************************************************************/
    //
    for (;;)
    {
       // HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7 , GPIO_PIN_SET);
        OSTimeDly(500);
//HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7 , GPIO_PIN_RESET);

        OSTimeDly(500);


    }
}

void LED1Task(void *pdata)			    //
{
    uint8_t err;
    //
    pdata=pdata;

    for (;;)
    {
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7 , GPIO_PIN_SET);
        OSTimeDly(300);
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7 , GPIO_PIN_RESET);
        OSTimeDly(300);
    }
}
void LED2Task(void *pdata)		    //
{
    uint8_t err;
    //
    pdata=pdata;

    for (;;)
    {
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8, GPIO_PIN_SET);
        OSTimeDly(500);
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8, GPIO_PIN_RESET);
        OSTimeDly(500);
    }
}
void LED3Task(void *pdata)			    //
{
    uint8_t err;
    //
    pdata=pdata;

    for (;;)
    {
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9 , GPIO_PIN_SET);
        OSTimeDly(1000);
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9 , GPIO_PIN_RESET);
        OSTimeDly(1000);
    }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/





