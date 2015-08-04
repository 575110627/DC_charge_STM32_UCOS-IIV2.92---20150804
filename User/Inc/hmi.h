#ifndef __HMI_H_
#define __HMI_H_
#include "cmd_queue.h"
#include "stm32f2xx_hal.h"

/*  ���ģʽѡ�������Ӧ����*/
extern uint16_t mainselect;                    //������ѡ��1�����磬2���ˣ�3����Ա��4���Ʋ���,5����������
extern uint16_t automaticcharge;               //���ģʽ���棬�Զ�����
extern uint16_t kwhcharge;                     //���ģʽ���棬��������
extern uint16_t moneycharge;                   //���ģʽ���棬������
extern uint16_t timecharge;                    //���ģʽ���棬��ʱ���
extern uint16_t setkwh;                        //���ģʽ���棬���õ���
extern uint16_t setmoney;                      //���ģʽ���棬���ý��
extern uint16_t sethour;                       //���ģʽ���棬����ʱ�䣨ʱ��
extern uint16_t setmin;                        //���ģʽ���棬����ʱ�䣨�֣�
extern uint16_t ButtonOKChargeMode;            //���ģʽ���棬ȷ������
extern uint16_t ButtonEscChargeMode;           //���ģʽ���棬���ذ���
extern uint16_t ButtonEscInsertPile;           //��ʾ����ǹͷ���棬���ذ���

/* ����ʱ*/
extern uint16_t TimeCountDownFlag;             //����ʱ��־λ
extern uint16_t TimeCountDownChargeMode;       //���ģʽѡ�񵹼�ʱ100s
extern uint16_t TimeCountDownStartCard;        //ˢ����ʼ��絹��ʱ30s
extern uint16_t TimeCountDownInputKeyword;		 //����Ա���������룩����ʱ100s
extern uint16_t TimeCountDownControlParaSet;	 //���Ʋ������õ���ʱ200s
extern uint16_t TimeCountDownAdministrator;	   //����Ա�������õ���ʱ200s
extern uint16_t TimeCountDownInsertPile;	     //��ʾ��ǹ�絹��ʱ60s

/*��־λ*/
extern uint16_t flag1;	                       //���ģʽѡ���У�flag1=0��ʾ������ȷ������ˢ����ʼ���
extern uint16_t modelflag1;	                   //���ģʽѡ��0δѡ��1�Զ���2������3��4ʱ��
extern uint16_t cardstartflag1;	               //��ʼˢ������־λ
extern uint16_t SendDataFlag;                  //������������ʪ�ȣ�����ѹ�����ȱ�־λ
extern uint16_t StopChargeFlag;                //���ֹͣ��簴����־λ
extern uint16_t InsertPileFlag;                //����ǹͷ��־λ 
 
extern uint16_t  tem1;
extern uint16_t  tem2;
extern uint16_t  tem3;
extern uint16_t  tem4;
extern uint16_t  tem5;
extern uint16_t  tem6; 
 
 /*���˽�����ر���*/
extern uint32_t idcard;                         //����
extern uint32_t expense;                        //���ѽ��
extern uint32_t balance;                        //�������
extern uint16_t voltage;                        //����ѹ
extern uint16_t current;                        //������
extern uint16_t kwh;                            //�ѳ����
extern uint16_t Uhour;                          //�ѳ�ʱ��ʱ
extern uint16_t Umin;                           //�ѳ�ʱ���
extern uint16_t Rhour;                          //����ʱ��ʱ
extern uint16_t Rmin;                           //����ʱ���
extern uint16_t soc;                            //SOC
extern uint16_t ButtonEscCheckOut;              //���˽����˳�����

 /*����Աģʽ������ر���*/
extern uint16_t  MoneyAverageElectricity;                     //ƽ�����
extern uint16_t  MoneyPeakElectricity;                        //��ֵ���
extern uint16_t  MoneyValleyElectricity;                      //��ֵ���
extern uint16_t  SetTimeHourStartPeakElectricity;             //��ֵ��ۿ�ʼʱ�䣨ʱ��
extern uint16_t  SetTimeMinStartPeakElectricity;              //��ֵ��ۿ�ʼʱ�䣨�֣�
extern uint16_t  SetTimeHourStopPeakElectricity;              //��ֵ��۽���ʱ�䣨ʱ��
extern uint16_t  SetTimeMinStopPeakElectricity;               //��ֵ��۽���ʱ�䣨�֣�
extern uint16_t  SetTimeHourStartValleyElectricity;           //��ֵ��ۿ�ʼʱ�䣨ʱ��
extern uint16_t  SetTimeMinStartValleyElectricity;            //��ֵ��ۿ�ʼʱ�䣨�֣�
extern uint16_t  SetTimeHourStopValleyElectricity;            //��ֵ��۽���ʱ�䣨ʱ��
extern uint16_t  SetTimeMinStopValleyElectricity;             //��ֵ��۽���ʱ�䣨�֣�
extern uint16_t  SaveAdministratorParameter;                  //�������Ա���ò���
extern uint16_t  IPAddress1;                                  //IP��ַ��һλ
extern uint16_t  IPAddress2;                                  //IP��ַ�ڶ�λ
extern uint16_t  IPAddress3;                                  //IP��ַ����λ
extern uint16_t  IPAddress4;                                  //IP��ַ����λ
extern uint32_t  FinalKwh;                                    //�ܵ���

 /*�������������ر���*/
extern uint16_t ButtonOKInputKeyword;                //�����������ȷ����
extern uint16_t ButtonEscInputKeyword;               //������������˳���
extern uint16_t ButtonEscAdministrator;	             //����Ա���ò��������˳���	
extern uint32_t KeywordSystem;                       //ϵͳĬ�����룬����������ʹ�����ƥ��                      
extern uint32_t KeywordInput;                        //������������棬���������
extern uint32_t KeywordSet;                          //����Ա������������������
extern uint32_t FinalKeywordSet;                     //����Ա�����������ռ�����

 /*���Ʋ���������ر���*/
extern uint16_t CtrlParaBMS;                     //������BMS���
extern uint16_t CtrlParaManualOperation;         //�ֶ����
extern uint16_t CtrlParaCutOffCurrent;           //���ý�ֹ����
extern uint16_t CtrlParaVoltage;                 //���ó���ѹ
extern uint16_t CtrlParaCurrent;                 //���ó�����
extern uint16_t ButtonOkCtrlPara;                //���Ʋ��������������  
extern uint16_t ButtonESCCtrlPara;               //���Ʋ����˳�����


extern uint16_t ButtonEscAdministrator;	         //����Ա���ò��������˳���		
extern uint32_t KeywordSystem;                   //ϵͳĬ�����룬����������ʹ�����ƥ�� 
extern uint32_t KeywordInput;                    //������������棬���������
extern uint32_t KeywordSet;                      //����Ա������������������
extern uint16_t ButtonOKInputKeyword;            //�����������ȷ����
extern uint16_t ButtonEscInputKeyword;           //������������˳���
extern uint16_t SaveAdministratorParameter;      //�������Ա���ò���

/*���崥��������֡�ṹ���Ա����*/
extern	PCTRL_MSG msg;                          //�ṹ��ָ�����msg
extern	  uint16_t  cmd_head;                   //֡ͷ13��14
extern		uint8_t 	data_len;                   //֡���ȣ�����֡���ͣ���ַ�����ݳ��ȣ�����
extern		uint8_t 	cmd_type;                   //֡���ͣ�80����д���ƼĴ�����81���ؼĴ�����82д���ݴ洢����83�����ݴ洢��
extern	  uint16_t  adr;                        //֡��ַ
extern		uint8_t 	rd_len;                     //���ݳ��ȣ�����ǲ������ݴ洢��������Ϊ��λ������ǲ������ƼĴ��������ֽ�Ϊ��λ
extern		uint16_t  hmidata[256];                  //��������

extern		uint16_t  size;
		


void SocShow(uint8_t soc);
void ChargeModChoose(void);
void CheckOut(void);
void Input_keyword(void);
void CtrlPara(void);
void Count_Down(void);
void Message_process(void);
void Stop_Charge(void);
void Administrator(void);

#endif
