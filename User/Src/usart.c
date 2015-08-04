#include "stm32f2xx_hal.h"

extern UART_HandleTypeDef huart3;
uint8_t aRxBuffer;

void SendChar(uint8_t t)
{
		
	HAL_UART_Transmit(&huart3,(uint8_t *)&t,1, 0xFFFF);		
}


void MX_USART3_UART_Init(void)
{
    huart3.Instance        = USART3;
    huart3.Init.BaudRate   = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits   = UART_STOPBITS_1;
    huart3.Init.Parity     = UART_PARITY_NONE;
    huart3.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    huart3.Init.Mode       = UART_MODE_TX_RX;

    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
    /* Initialization Error */

    }
 //__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
  
    if(HAL_UART_Receive_IT(&huart3, (uint8_t *)&aRxBuffer, 1) != HAL_OK)
    {
   // Error_Handler();
    }
    queue_reset();
}
