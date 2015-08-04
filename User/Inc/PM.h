/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PM_H
#define __PM_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
	 #include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

typedef enum 
{
	PM485_ERROR    = 0x00,
    PM485_OK       = 0x01
} PM485;

#define ACVoltageRegister 0x32
#define ACCurrentRegister 0x38
#define ACEnergyRegister  0x0a
#define ACFrequencyRegister 0x4b
#define ACRectivePowerRegister 0x42
#define ACApparentPowerRegister 0x46

void PM_Init(void);
void MX_UART5_UART_Init(void);
PM485 Send_485(uint8_t address,uint8_t addressAC);
PM485 Receive_485(void);
void Fun_485(void);
void Fun_Receive_Send(uint8_t address,uint8_t addressAC);
void GetACEnergy(void);
uint16_t CalcCrc(uint8_t *chData,unsigned short uNo);



#ifdef __cplusplus
}
#endif

#endif /* __PM_H */
