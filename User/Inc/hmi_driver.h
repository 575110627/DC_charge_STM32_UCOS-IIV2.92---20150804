#ifndef _HMI_DRIVER_
#define _HMI_DRIVER_

#include "main.h"



void UpdateRTC(void);                                 //更新RTC
void SendBeginCMD(void);                              //发送帧头
void SendDataLen(void);										            //发送数据长度
void SendRdDataCMD(void);                             //发送数据类型（0x80,81,82,83,84）
void SendADR(uint16_t address);                       //发送地址
void SendRdLen(uint8_t	rd_len);                      //发送接收长度
void RdDataMemory(uint16_t address,uint8_t rd_len);   //读数据存储器
void SendResetHMIRes(void);                           //复位触摸屏寄存器
void SendChangePageRes(void);                         //更新界面寄存器
void WrDataMemory(uint16_t address,uint16_t data);    //写数据16位到相应地址
void WrDataMemory32(uint16_t address,uint32_t data);  //写数据32位到相应地址
void ChangePage(uint16_t page);                       //切换到目标界面
void RdCurrentPage(void);									            //读此时界面
void UpDataId(void);								                  //更新卡号
void UpDataExpense(uint32_t expense);                 //更新消费金额
void UpDataBalance(void);                             //更新消费余额
void UpDataVoltage(uint16_t volage);                  //更新电压
void UpDataCurrent(uint16_t current);                 //更新电流
void UpDataKwh(uint16_t kwh);                         //更新电能
void UpDataUsedHour(uint16_t Uhour);                  //更新已用时间小时
void UpDataUsedMin(uint16_t Umin);                    //更新已用时间分
void UpDataRemainHour(uint16_t Rhour);                //更新还需时间小时
void UpDataRemainMin(uint16_t Rmin);                  //更新还需时间分
void UpDataSoc(uint8_t soc);                         //更新SOC
void UpDateTemp(uint16_t te);                         //更新温度
void UpDateHumi(uint16_t hu);                         //更新湿度
void Reset_HMI(void);                                 //复位触摸屏
void UpdateSystemParameter(void);                                          //更新系统参数
void Updata_MoneyAverageElectricity(void);                                //平均电价
void Updata_MoneyPeakElectricity(void);                                     //峰值电价
void Updata_MoneyValleyElectricity(void);                                 //谷值电价
void Updata_SetTimeHourStartPeakElectricity(void);               //峰值电价开始时间（时）
void Updata_SetTimeMinStartPeakElectricity(void);                 //峰值电价开始时间（分）
void Updata_SetTimeHourStopPeakElectricity(void);                 //峰值电价结束时间（时）
void Updata_SetTimeMinStopPeakElectricity(void);                   //峰值电价结束时间（分）
void Updata_SetTimeHourStartValleyElectricity(void);           //谷值电价开始时间（时）
void Updata_SetTimeMinStartValleyElectricity(void);             //谷值电价开始时间（分）
void Updata_SetTimeHourStopValleyElectricity(void);             //谷值电价结束时间（时）
void Updata_SetTimeMinStopValleyElectricity(void);               //谷值电价结束时间（分）
void Updata_IPAddress1(void);                                                         //IP地址第一位
void Updata_IPAddress2(void);                                                         //IP地址第二位
void Updata_IPAddress3(void);                                                         //IP地址第三位
void Updata_IPAddress4(void);                                                         //IP地址第四位
void Updata_FinalEnergy(void);
void Updata_Keyword(void);                                        //密码
#endif
