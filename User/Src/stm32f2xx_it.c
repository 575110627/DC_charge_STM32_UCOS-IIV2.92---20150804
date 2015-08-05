/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/Src/stm32f2xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-March-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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
#include "stm32f2xx_it.h"
#include "main.h"

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
extern UART_HandleTypeDef UartHandle1;
extern UART_HandleTypeDef UartHandle3;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef UartHandle4;
extern UART_HandleTypeDef UartHandle5;
extern UART_HandleTypeDef UartHandle6;
extern CAN_HandleTypeDef    CanHandleToBMS;
extern CAN_HandleTypeDef    CanHandleToDC;
extern TIM_HandleTypeDef    TimHandle2;
extern TIM_HandleTypeDef    TimHandle3;
extern TIM_HandleTypeDef    TimHandle4;
extern TIM_HandleTypeDef    TimHandle5;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */

}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
 
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
 
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */

  
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*
void PendSV_Handler(void)
{
}
*/
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle1);
	//HAL_GPIO_WritePin( GPIOF, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_SET); 
	
}
void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart3);
	
}
void UART4_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle4);
}
void USART6_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle6);
	
}

void UART5_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UartHandle5);	
}


void CAN1_RX0_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&CanHandleToBMS);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15 ,GPIO_PIN_SET);	
	//HAL_GPIO_WritePin( GPIOF,GPIO_PIN_10|GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7, GPIO_PIN_RESET);
}


void CAN2_RX1_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&CanHandleToDC);
}

// /**
//   * @}
//   */ 
// void CAN2_TX_IRQHandler(void)
// {
//     HAL_CAN_IRQHandler(&CanHandleToDC);
// }
/**
  * @}
  */
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TimHandle2);	
}
/**
  * @}
  */
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TimHandle3);
}

/**
  * @}
  */
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TimHandle4);	
}
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TimHandle5);
}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles External line 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{ 
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
}	

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
