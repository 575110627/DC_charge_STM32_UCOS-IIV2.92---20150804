/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
	 #include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


extern uint8_t aRxBuffer;
void SendChar(uint8_t t);
void MX_USART3_UART_Init(void);






#ifdef __cplusplus
}
#endif

#endif /* __PM_H */
