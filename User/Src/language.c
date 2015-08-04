
#include "stm32f2xx_hal.h"
//*****************************************************
uint8_t nBkm = 0x00;
extern UART_HandleTypeDef UartHandle1;
//uint8_t Languagestatus=1;
//*****************************************************

#define HEADLEN       5  //���ݰ�ͷ�ĳ���
#define BKM_OFFSET    4  //������������ƫ��
#define LEN_OFFSET    2  //�����ֽڵ�ƫ������һ�㲻�ᳬ��255�ֽڣ����ֻʹ��1�ֽڳ��ȣ�
#define BKM_MAX      15	 //������������

//*****************************************************

//���ݰ�ͷ��0xFD + 2�ֽڳ��� + 1�ֽ������� + 1�ֽ��������)
const uint8_t head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};

//****************************************************

void USART1_UART_Init(void)
{
	
    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART6 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit    = One Stop bit
      - Parity      = ODD parity
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle1.Instance        = USART1;
  UartHandle1.Init.BaudRate   = 9600;
  UartHandle1.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle1.Init.StopBits   = UART_STOPBITS_1;
  UartHandle1.Init.Parity     = UART_PARITY_NONE;
  UartHandle1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle1.Init.Mode       = UART_MODE_TX_RX;

  if (HAL_UART_Init(&UartHandle1) != HAL_OK)
  {
    /* Initialization Error */

  }

}
//��������(����Ϊ0��ʾ�رձ�������)
void BkMusic(uint8_t num)
{
	num %= BKM_MAX + 1;
	nBkm = num;
}
 
//��������
void Speech(uint8_t *buf)
{
	uint8_t i = 0;          //ѭ����������
	uint8_t xor_language = 0x00;     //У�����ʼ��
	uint8_t ch = 0x00;
  uint8_t len = 0x00;

  while(buf[len++]);
  //len--;
	//�������ݰ�ͷ(0xFD + 2�ֽڳ��� + 1�ֽ������� + 1�ֽ��������)
	for(i = 0; i < HEADLEN; i++)
	{
		if(i == BKM_OFFSET)
			ch = nBkm << 3; //д�뱳������
		else if(i == LEN_OFFSET)
			ch = len + 3;
		else
			ch = head[i];

		xor_language ^= ch;
		HAL_UART_Transmit(&UartHandle1, (uint8_t *)&ch, 1, 0xFFFF);
   	}
	//������������
	for(i = 0; i < len; i++)
	{
		xor_language ^= buf[i];
		HAL_UART_Transmit(&UartHandle1, (uint8_t *)&(buf[i]), 1, 0xFFFF);
	}
  HAL_UART_Transmit(&UartHandle1, (uint8_t *)&xor_language, 1, 0xFFFF);
}
//��ͷ+����+У��λ

