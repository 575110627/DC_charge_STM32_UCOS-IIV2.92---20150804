#ifndef _HMI_DRIVER_
#define _HMI_DRIVER_

#include "main.h"



void UpdateRTC(void);                                 //����RTC
void SendBeginCMD(void);                              //����֡ͷ
void SendDataLen(void);										            //�������ݳ���
void SendRdDataCMD(void);                             //�����������ͣ�0x80,81,82,83,84��
void SendADR(uint16_t address);                       //���͵�ַ
void SendRdLen(uint8_t	rd_len);                      //���ͽ��ճ���
void RdDataMemory(uint16_t address,uint8_t rd_len);   //�����ݴ洢��
void SendResetHMIRes(void);                           //��λ�������Ĵ���
void SendChangePageRes(void);                         //���½���Ĵ���
void WrDataMemory(uint16_t address,uint16_t data);    //д����16λ����Ӧ��ַ
void WrDataMemory32(uint16_t address,uint32_t data);  //д����32λ����Ӧ��ַ
void ChangePage(uint16_t page);                       //�л���Ŀ�����
void RdCurrentPage(void);									            //����ʱ����
void UpDataId(void);								                  //���¿���
void UpDataExpense(uint32_t expense);                 //�������ѽ��
void UpDataBalance(void);                             //�����������
void UpDataVoltage(uint16_t volage);                  //���µ�ѹ
void UpDataCurrent(uint16_t current);                 //���µ���
void UpDataKwh(uint16_t kwh);                         //���µ���
void UpDataUsedHour(uint16_t Uhour);                  //��������ʱ��Сʱ
void UpDataUsedMin(uint16_t Umin);                    //��������ʱ���
void UpDataRemainHour(uint16_t Rhour);                //���»���ʱ��Сʱ
void UpDataRemainMin(uint16_t Rmin);                  //���»���ʱ���
void UpDataSoc(uint8_t soc);                         //����SOC
void UpDateTemp(uint16_t te);                         //�����¶�
void UpDateHumi(uint16_t hu);                         //����ʪ��
void Reset_HMI(void);                                 //��λ������
void UpdateSystemParameter(void);                                          //����ϵͳ����
void Updata_MoneyAverageElectricity(void);                                //ƽ�����
void Updata_MoneyPeakElectricity(void);                                     //��ֵ���
void Updata_MoneyValleyElectricity(void);                                 //��ֵ���
void Updata_SetTimeHourStartPeakElectricity(void);               //��ֵ��ۿ�ʼʱ�䣨ʱ��
void Updata_SetTimeMinStartPeakElectricity(void);                 //��ֵ��ۿ�ʼʱ�䣨�֣�
void Updata_SetTimeHourStopPeakElectricity(void);                 //��ֵ��۽���ʱ�䣨ʱ��
void Updata_SetTimeMinStopPeakElectricity(void);                   //��ֵ��۽���ʱ�䣨�֣�
void Updata_SetTimeHourStartValleyElectricity(void);           //��ֵ��ۿ�ʼʱ�䣨ʱ��
void Updata_SetTimeMinStartValleyElectricity(void);             //��ֵ��ۿ�ʼʱ�䣨�֣�
void Updata_SetTimeHourStopValleyElectricity(void);             //��ֵ��۽���ʱ�䣨ʱ��
void Updata_SetTimeMinStopValleyElectricity(void);               //��ֵ��۽���ʱ�䣨�֣�
void Updata_IPAddress1(void);                                                         //IP��ַ��һλ
void Updata_IPAddress2(void);                                                         //IP��ַ�ڶ�λ
void Updata_IPAddress3(void);                                                         //IP��ַ����λ
void Updata_IPAddress4(void);                                                         //IP��ַ����λ
void Updata_FinalEnergy(void);
void Updata_Keyword(void);                                        //����
#endif
