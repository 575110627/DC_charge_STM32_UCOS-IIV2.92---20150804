#include "stm32f2xx_hal.h"


static GPIO_InitTypeDef  GPIO_InitStruct_AM2321;
static uint8_t Read_SensorData(void)	;	 //�������ֽ�
#define AM2321_DATA_IN  HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)
#define AM2321_OUT_HIGH HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_SET);
#define AM2321_OUT_LOW  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_RESET);


void  AM2321_Set_Input(void)
{
	__GPIOG_CLK_ENABLE();
	GPIO_InitStruct_AM2321.Pin = (GPIO_PIN_12);
    GPIO_InitStruct_AM2321.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct_AM2321.Pull = GPIO_PULLUP;
    GPIO_InitStruct_AM2321.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct_AM2321);
}
	
void  AM2321_Set_Output(void)
{
	__GPIOG_CLK_ENABLE();
	GPIO_InitStruct_AM2321.Pin = (GPIO_PIN_12);
    GPIO_InitStruct_AM2321.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct_AM2321.Pull = GPIO_PULLUP;
    GPIO_InitStruct_AM2321.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct_AM2321);
}
uint8_t Read_SensorData(void)		 //�������ֽ�																								  ubyte Read_SensorData(void)		 //�������ֽ�
{
	uint8_t i;
	uint8_t buffer,tmp;
	uint8_t retry=0;
	buffer = 0;
	for(i=0;i<8;i++)
	{
		//����ϴε͵�ƽ�Ƿ����
		while(!(AM2321_DATA_IN ))
		{
			retry++;
			if(retry >80)
		     break;
			HAL_Delay1us(1);
		}
		retry=0;
		//��ʱMin=26us Max50us ��������"0" �ĸߵ�ƽ
		HAL_Delay1us(20);
    HAL_Delay1us(30);
		//�жϴ�������������λ
		tmp =0;
		if(AM2321_DATA_IN)
		{
			tmp=1;
		}
		//�ȴ��ߵ�ƽ ����
		while(AM2321_DATA_IN)
		{
			retry++;
			if(retry >80)
			  break;
			HAL_Delay1us(1);
		}
		buffer <<=1;
		buffer |= tmp;	
	}
	return buffer;
}

/********************************************\
|* ���ܣ� ��������              	        *|
\********************************************/
uint8_t Read_Sensor(void)
{
  uint8_t i;
	uint16_t retry=0;
	//��ʪ�ȱ���
  uint8_t Sensor_Data[5] = {0,0,0,0,1};
	//������ʼ�ź�����(Min=800US typ=1ms Max=20Ms) 
	AM2321_Set_Output();
	AM2321_OUT_LOW;
	//��ʱ10Ms 
	//HAL_Delay1us(1000);
	HAL_Delay(1);
	AM2321_OUT_HIGH;
	//�ͷ����� ��ʱ(Min=20us typ=30us Max=200us)
	HAL_Delay1us(30); 	
	//������Ϊ���� �жϴ�������Ӧ�ź� 	 
  //��������
	AM2321_Set_Input();
	//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	//if(AM2321_DATA_IN == GPIO_PIN_RESET)		   //�յ��������ź�
	   //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
	 HAL_Delay1us(50);
	if(AM2321_DATA_IN == GPIO_PIN_RESET)  		   //�յ��������ź�
	{		
		 
	   while(AM2321_DATA_IN ==0)
	   {
		   retry++;
		   HAL_Delay1us(1);
		   if(retry >= 100 )
					break;
	   }
	   retry = 0;
	    //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	   while(AM2321_DATA_IN)
	   {
			 retry++;
			 HAL_Delay1us(1);
			 if(retry >= 100)
			 {
					break;
			 } 		
	   }
	    // ���ݽ���	������������40λ���� 
	    // ��5���ֽ� ��λ����  5���ֽڷֱ�Ϊʪ�ȸ�λ ʪ�ȵ�λ �¶ȸ�λ �¶ȵ�λ У���
	    // У���Ϊ��ʪ�ȸ�λ+ʪ�ȵ�λ+�¶ȸ�λ+�¶ȵ�λ
	  for(i=0;i<5;i++)
		{
		  Sensor_Data[i] = Read_SensorData();
		}
		//if((Sensor_Data[0]+Sensor_Data[1]+Sensor_Data[2]+Sensor_Data[3])!=Sensor_Data[4]) 
			if(0)
			return 0;
		else
		{
			if((Sensor_Data[2]&0x80) == 1)
			{
				Sensor_Data[2] = Sensor_Data[2]&0x7F;
				AM2321Temperature = Sensor_Data[2]*256 + Sensor_Data[3] ;
				AM2321Temperature = -AM2321Temperature + 400;
			}
			else
			{
				AM2321Temperature = Sensor_Data[2]*256 + Sensor_Data[3] + 400 ;
			}
		    Am2321Humidity = Sensor_Data[0]*256 + Sensor_Data[1];
			return 1;
		}	
	}	
	return 0;	
}    



