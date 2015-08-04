/**
  ******************************************************************************
  * @file    stm32f2xx_hal_msp_template.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    25-March-2014
  * @brief   HAL MSP module.
  *          This file template is located in the HAL folder and should be copied 
  *          to the user folder.
  *         
  @verbatim
 ===============================================================================
                     ##### How to use this driver #####
 ===============================================================================
    [..]
    This file is generated automatically by MicroXplorer and eventually modified 
    by the user

  @endverbatim
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

/** @addtogroup STM32F2xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

extern uint8_t pData[1];
extern uint8_t aRxBuffer;
extern TIM_HandleTypeDef    TimHandle3;
extern TIM_HandleTypeDef    TimHandle4;
uint8_t tim3count=0;
extern uint8_t Tim3ICWaitCount;
extern uint8_t SerBfr[37];		   //数据帧定义
extern uint8_t SerBfrIdx;				   //数据帧索引
extern uint8_t SerDTvalid;	               //数据正确接

struct _StrCAN CANBMS;
uint8_t i;
/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
  /* NOTE : This function is generated automatically by MicroXplorer and eventually  
            modified by the user
   */ 
}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None  
  * @retval None
  */
void HAL_MspDeInit(void)
{
  /* NOTE : This function is generated automatically by MicroXplorer and eventually  
            modified by the user
   */
}

/**
  * @brief  Initializes the PPP MSP.
  * @param  None
  * @retval None
  */
void HAL_PPP_MspInit(void)
{
  /* NOTE : This function is generated automatically by MicroXplorer and eventually  
            modified by the user
   */ 
}

/**
  * @brief  DeInitializes the PPP MSP.
  * @param  None  
  * @retval None
  */
void HAL_PPP_MspDeInit(void)
{
  /* NOTE : This function is generated automatically by MicroXplorer and eventually  
            modified by the user
   */
}

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
   if(huart->Instance == USART1)
   {
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
	//UART1
 	__GPIOA_CLK_ENABLE();   //TX/RX_CLK_ENABLE
	__USART1_CLK_ENABLE();    //UART4_CLK_ENABLE
  

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO PA9 pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_9;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* UART RX GPIO PA10 pin configuration  */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /*##-2- Configure the NVIC for USARTx ########################################*/
   /* Set Interrupt Group Priority */ 
  HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
  
  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
	//USART3
	if(huart->Instance == USART3)
	{
				__GPIOB_CLK_ENABLE();   //TX/RX_CLK_ENABLE
				__USART3_CLK_ENABLE();    //UART4_CLK_ENABLE
				//TX PC12
				GPIO_InitStruct.Pin       = GPIO_PIN_10;
				GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
				GPIO_InitStruct.Pull      = GPIO_PULLUP;
				GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
				GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

				HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
				/* UART RX PD2 GPIO pin configuration  */
				GPIO_InitStruct.Pin = GPIO_PIN_11;
				GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

				HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
				
				/*##-2- Configure the NVIC for USARTx ########################################*/
				 /* ESet Interrupt Group Priority */ 
				HAL_NVIC_SetPriority(USART3_IRQn, 3, 1);
				
				/* Enable the TIMx global Interrupt */
				HAL_NVIC_EnableIRQ(USART3_IRQn);
		}	
	//UART4
	if(huart->Instance == UART4)
	{
			__GPIOC_CLK_ENABLE();   //TX/RX_CLK_ENABLE
			__UART4_CLK_ENABLE();    //UART4_CLK_ENABLE
			//TX PC10
			GPIO_InitStruct.Pin       = GPIO_PIN_10;
			GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Pull      = GPIO_PULLUP;
			GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
			GPIO_InitStruct.Alternate = GPIO_AF8_UART4;

			HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
			/* UART RX PC11 GPIO pin configuration  */
			GPIO_InitStruct.Pin = GPIO_PIN_11;
			GPIO_InitStruct.Alternate = GPIO_AF8_UART4;

			HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
			
			/*##-2- Configure the NVIC for USARTx ########################################*/
			 /* ESet Interrupt Group Priority */ 
			HAL_NVIC_SetPriority(UART4_IRQn, 3, 2);
			
			/* Enable the TIMx global Interrupt */
			HAL_NVIC_EnableIRQ(UART4_IRQn);
		}
		if(huart->Instance == UART5)
		{
				//UART5
				__GPIOC_CLK_ENABLE();   //TX_CLK_ENABLE
				__GPIOD_CLK_ENABLE();    //RX_CLK_ENABLE
				__UART5_CLK_ENABLE();    //UART5_CLK_ENABLE
				//TX PC12
				GPIO_InitStruct.Pin       = GPIO_PIN_12;
				GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
				GPIO_InitStruct.Pull      = GPIO_PULLUP;
				GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
				GPIO_InitStruct.Alternate = GPIO_AF8_UART5;

				HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
				/* UART RX PD2 GPIO pin configuration  */
				GPIO_InitStruct.Pin = GPIO_PIN_2;
				GPIO_InitStruct.Alternate = GPIO_AF8_UART5;

				HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
				
				/*##-2- Configure the NVIC for USARTx ########################################*/
				 /* Set Interrupt Group Priority */ 
				HAL_NVIC_SetPriority(UART5_IRQn, 3, 3);
				/* Enable the TIMx global Interrupt */
				HAL_NVIC_EnableIRQ(UART5_IRQn);
	}
	if(huart->Instance == USART6)
	{
			//UART6
			__GPIOC_CLK_ENABLE();   //TX/RX_CLK_ENABLE
			__USART6_CLK_ENABLE();    //USART6_CLK_ENABLE
			//TX PC6
			GPIO_InitStruct.Pin       = GPIO_PIN_6;
			GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Pull      = GPIO_PULLUP;
			GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
			GPIO_InitStruct.Alternate = GPIO_AF8_USART6;

			HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
			/* UART RX PC7 GPIO pin configuration  */
			GPIO_InitStruct.Pin = GPIO_PIN_7;
			GPIO_InitStruct.Alternate = GPIO_AF8_USART6;

			HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
			
			/*##-2- Configure the NVIC for USARTx ########################################*/
			 /* Set Interrupt Group Priority */ 
			HAL_NVIC_SetPriority(USART6_IRQn, 3, 4);
			/* Enable the TIMx global Interrupt */
			HAL_NVIC_EnableIRQ(USART6_IRQn);
	}
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/

}


/**
  * @}
  */
/**
  * @brief CAN MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for DMA interrupt request enable
  * @param hcan: CAN handle pointer
  * @retval None
  */
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  if(hcan->Instance == CAN2)
  {
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* CAN2 Periph clock enable */
		__CAN2_CLK_ENABLE();
		/* Enable GPIO clock ****************************************/
		__GPIOB_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* CAN2 TX GPIO pin configuration */
		GPIO_InitStruct.Pin = GPIO_PIN_13;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Alternate =  GPIO_AF9_CAN2;

		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* CAN2 RX GPIO pin configuration */
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Alternate =  GPIO_AF9_CAN2;

		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/*##-3- Configure the NVIC #################################################*/
		/* NVIC configuration for CAN1 Reception complete interrupt */
		HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 1, 2);
		HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);			
  }
	////////////////
	//CAN1
	///////////////
	if(hcan->Instance == CAN1)
    {
		__CAN1_CLK_ENABLE();
		/* Enable GPIO clock ****************************************/
		__GPIOA_CLK_ENABLE();//__GPIOA_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* CAN1 TX GPIO pin configuration */
		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Alternate =  GPIO_AF9_CAN1;

		HAL_GPIO_Init(GPIOA ,&GPIO_InitStruct);

		/* CAN1 RX GPIO pin configuration */
		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Alternate =  GPIO_AF9_CAN1;

		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/*##-3- Configure the NVIC #################################################*/
		/* NVIC configuration for CAN1 Reception complete interrupt */
		HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 1);
		HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	}
}

/**
  * @brief CAN MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hcan: CAN handle pointer
  * @retval None
  */
void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan)
{
  /*##-1- Reset peripherals ##################################################*/
  if(hcan->Instance == CAN2)
  {
			__CAN2_FORCE_RESET();
			__CAN2_RELEASE_RESET();

			/*##-2- Disable peripherals and GPIO Clocks ################################*/
			/* De-initialize the CAN1 TX GPIO pin */
			HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);
			/* De-initialize the CAN1 RX GPIO pin */
			HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12);

			/*##-4- Disable the NVIC for CAN reception #################################*/
			HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
	}
	 if(hcan->Instance == CAN1)
  {
			__CAN1_FORCE_RESET();
			__CAN1_RELEASE_RESET();

			/*##-2- Disable peripherals and GPIO Clocks ################################*/
			/* De-initialize the CAN1 TX GPIO pin */
			HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);
			/* De-initialize the CAN1 RX GPIO pin */
			HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);

			/*##-4- Disable the NVIC for CAN reception #################################*/
			HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
	}
}
/**
  * @brief  Initializes the TIM Base MSP.
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_Base_MspInit could be implemented in the user file
   */
	if(htim->Instance == TIM2)
	{
	  __TIM2_CLK_ENABLE();
	  /*##-2- Configure the NVIC for TIMx ########################################*/
	  /* Set Interrupt Group Priority */ 
	  HAL_NVIC_SetPriority(TIM2_IRQn, 2, 0);
	  /* Enable the TIMx global Interrupt */
	  HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
	if(htim->Instance == TIM3)
	{
	  __TIM3_CLK_ENABLE();
	  /*##-2- Configure the NVIC for TIMx ########################################*/
	  /* Set Interrupt Group Priority */ 
	  HAL_NVIC_SetPriority(TIM3_IRQn, 2, 1);
	  /* Enable the TIMx global Interrupt */
	  HAL_NVIC_EnableIRQ(TIM3_IRQn);
	}
	
	if(htim->Instance == TIM4)
	{
		  __TIM4_CLK_ENABLE();
	  /*##-2- Configure the NVIC for TIMx ########################################*/
	  /* Set Interrupt Group Priority */ 
	  HAL_NVIC_SetPriority(TIM4_IRQn, 2, 2);
	  /* Enable the TIMx global Interrupt */
	  HAL_NVIC_EnableIRQ(TIM4_IRQn);
	}
	if(htim->Instance == TIM5)
	{
	  __TIM5_CLK_ENABLE();
	  /*##-2- Configure the NVIC for TIMx ########################################*/
	  /* Set Interrupt Group Priority */ 
	  HAL_NVIC_SetPriority(TIM5_IRQn, 2, 3);
	  /* Enable the TIMx global Interrupt */
	  HAL_NVIC_EnableIRQ(TIM5_IRQn);
	}
	
}


/**
  * @brief I2C MSP Init.
  * @param  hi2c : Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
 void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2C_MspInit could be implemented in the user file
   */
	
	GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable GPIO Clocks #################################################*/
  /* Enable GPIO TX/RX clock */
  __GPIOB_CLK_ENABLE();
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* I2C SCL GPIO pin configuration  */
  GPIO_InitStruct.Pin       = GPIO_PIN_6;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
  /* I2C SDA GPIO pin configuration  */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
   
  /*##-3- Enable I2C peripheral Clock ########################################*/ 
  /* Enable I2C1 clock */
  __I2C1_CLK_ENABLE();
}


/**
  * @brief  Tx Transfer completed callbacks.
  * @param  huart: UART handle
  * @retval None
  */
 void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
//   if(huart->Instance == USART6)
// 	{
// 		if(SerBfrIdx < SerBfr[FRAMELEN]) 
// 		{
//             HAL_UART_Transmit(huart, (uint8_t *)&(SerBfr[SerBfrIdx]), 1, 0xFFFF);;
//             SerBfrIdx++;
//         } 
// 		else 
// 		{
//             SerBfrIdx = FRAMELEN;    	
// 		}
// 	}
// 	HAL_UART_Transmit_IT(huart , pData, 1);
// 	
}
/**
  * @}UART Callback 
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static unsigned char bTemp, Piccflag=0;
	if(huart->Instance == USART1)   //
	{
	}
	if(huart->Instance == USART3)    //触摸屏
	{
		queue_push(aRxBuffer);
		HAL_UART_Receive_IT(huart, (uint8_t *)&aRxBuffer, 1);
	}
	
	if(huart->Instance == UART5)   //电表
	{
		UART5RxBuff[UART5RxBuffCount++] = pData[0];
		if(UART5RxBuffCount ==24)
		{
			
			UART5RxBuffCount=0;
		} 
		
		HAL_TIM_Base_Init(&TimHandle4);
	    __HAL_TIM_CLEAR_IT(&TimHandle4, TIM_IT_UPDATE);
	    HAL_TIM_Base_Start_IT(&TimHandle4);
		HAL_UART_Receive_IT(huart, pData, 1);
  }
	if(huart->Instance == USART6)   //读卡器
	{
			/* 清串口中断标记 */
		//bTemp = USART_ReceiveData(USART1);
		bTemp = pData[0];
		/* 上条命令处理完成才接收 */
		if(Cmd.ReceiveFlag == 0)
		{
			/* 7F 标记，为 0 表示上一个数据不是7F */
			if(Piccflag == 0)
			{
				/* 上一个标记不是7F，这个是，打上标记 */
				if(bTemp == 0x7F){Piccflag = 1;}
				/* 把值存进接收缓存 */
				Cmd.ReceiveBuffer[Cmd.ReceivePoint++] = bTemp;
			}
			else
			{
				Piccflag = 0;
				/* 上一个是7F，这一个不是表示收到【命令头】 */
				if(bTemp != 0x7F)
				{
					Cmd.ReceivePoint = 0;
					Cmd.ReceiveBuffer[Cmd.ReceivePoint++] = bTemp;
				}
				/* 上一个是7F，这一个也是，则忽略当前的7F */
			}
			if(Cmd.ReceivePoint >= 32)
			{
				Cmd.ReceivePoint = 0;
			}
			/* 接收指针大于 2 个开始对比命令长度和接收指针，一致表示收到完整命令 */
			if(Cmd.ReceivePoint > 2)
			{
				if(Cmd.ReceivePoint == Cmd.ReceiveBuffer[0]+1)
				{
					Cmd.ReceiveFlag = 1;
				}
			}
		}
		HAL_UART_Receive_IT(huart, pData, 1); 
	}
	
}
/**
  * @} CAN Callback
  */ 
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *CanHandle)
{
	if(CanHandle->pRxMsg->IDE == CAN_ID_EXT)   //判断扩展帧
	{
		if(CanHandle->Instance == CAN1)  //判断BMS的CAN进入中断
		{
					CANBMS.IDE = CanHandle->pRxMsg->ExtId;
					CANBMS.DLC = CanHandle->pRxMsg->DLC;
					for(i=0;i<8;i++)
					{
						CANBMS.Data[i] = CanHandle->pRxMsg->Data[i];		
					}
					CANqueue_push(CANBMS);
			HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0);
		}
	  if(CanHandle->Instance == CAN2)
		{					
			uCANBufID_DC = CanHandle->pRxMsg->ExtId;
			for(i=0;i<8;i++)
			{
				//来自DC的数据存入缓存
				ubCANBuf_DC[i]=CanHandle->pRxMsg->Data[i];
			}
			//CAN_DataProcessing();
			HAL_CAN_Receive_IT(CanHandle, CAN_FIFO1);
		}
	}
}
/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM2)
	{
		Timer1CountNumber++; //每500ms加1，10s复位，定时器1用
		FlagCycle500ms = 1;   //500ms循环定时标志位，默认为0；定时器1置位，使用完清零
		//HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7 );
		if(Timer1CountNumber%2 == 0)
		{			
			FlagCycle1s = 1;      //1s循环定时标志位，默认为0；定时器1置位，使用完清零
			if(Timer1CountNumber%20 == 0)
			{
				FlagCycle10s = 1;     //10s循环定时标志位，默认为0；定时器1置位，使用完清零
			  if(Timer1CountNumber%60 == 0) 
				{
					Timer1CountNumber = 0;
					FlagCycle30s = 1; //30s循环定时标志位，默认为0；定时器1置位，使用完清零
				}
			}
		}
	}	
	
	if(htim->Instance == TIM3)
	{
		
		PMCount485++;//电表用
		ubCANTim3Count ++;
		uwCANTimeOutCount ++;
		uwCANDCTimeOutCount ++;
		FlagIfCycleSend10ms = 1;
		tim3count++;
		if(Tim3ICWaitCount>0)
		{
		Tim3ICWaitCount --;
		}
		if(tim3count%25 ==0)   //250ms发一次DC电源模块数据
		{
			tim3count = 0;
			FlagIfSendDCMeg = 1;
		}
		if(ubCANTim3Count%3== 0)  //50ms发送一次
		{
				FlagIfCycleSend50ms = 1;
				if(ubCANTim3Count%10 == 0)
				{
				    WriteBlockTimeFlag = 1;
						if(ubCANTim3Count%25 == 0)  //250ms发送一次 
						{
								FlagIfCycleSend250ms = 1; 
								if(ubCANTim3Count%50 == 0)  //500ms发送一次 
								{
										FlagIfCycleSend500ms = 1;
										if(ubCANTim3Count%100 == 0)  //1s发送一次 100*10ms=1s
										{
											FlagIfCycleSend1000ms = 1;
											ubCANTim3Count = 0;
										}  	
								}       	
						}	
				}
		}
		if(uwCANTimeOutCount == 10*ubTimeN100ms)
		{
					FlagIfTimeout =1;
					uwCANTimeOutCount = 0;
		}
		if(uwCANDCTimeOutCount == 500)
		{
				 FlagIfDCTimeout = 1;
				 uwCANDCTimeOutCount = 0;
		}	
	}
	if(htim->Instance == TIM4)
	{
		HAL_TIM_Base_Stop_IT(&TimHandle4);	
		//i++;
		Tim4Flag=0x01;
   }
    if(htim->Instance == TIM5)
	{
		if(mainselect==1)
				TimeCountDownChargeMode--;	 
		if(cardstartflag1==1)
		    TimeCountDownStartCard--;	
		if(mainselect==3)
		    TimeCountDownInputKeyword--;
		if(mainselect==4)
		    TimeCountDownControlParaSet--;
		if(mainselect==6)
		    TimeCountDownAdministrator--;
		if(1==InsertPileFlag)
        TimeCountDownInsertPile--;	
	}
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
