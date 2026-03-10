


#ifndef __USERMAIN_H
#define __USERMAIN_H

/* includes -----------------------------------------------------------------------*/
#include "userint.h"

/*-----------------------------------------------------------------------*/
#if     INT_LEDBEEP
#include "LEDBEEP.h"
#endif
/*-----------------------------------------------------------------------*/
#if     INT_SofIIC
#include "SofIIC.h"
#endif
/*-----------------------------------------------------------------------*/
#if     INT_ATUSART
#include "ATUSARTuser.h"
#include "USendList.h"
#endif
/*-----------------------------------------------------------------------*/
#if    	INT_SoftSPI
#include "SofSPI.h"
#endif
/*-----------------------------------------------------------------------*/
#if    	INT_Sensor
#include "Sensor.h"
#endif
/*-----------------------------------------------------------------------*/
#if     INT_MOScontrol
#include "MOScontrol.h"
#endif
/*-----------------------------------------------------------------------*/
#if     INT_ReadXMC
#include "ReadXMCdata.h"
#endif
/*-----------------------------------------------------------------------*/
#if     INT_Readcard
#include "Readcard.h"
#endif
/*-----------------------------------------------------------------------*/
#if     INT_W25Qxx
#include "spi_flash.h"
#endif
/*-----------------------------------------------------------------------*/
#if     INT_DS18B20
#include "DS18B20.h"
#endif
/*-----------------------------------------------------------------------*/
#define Queuelen  100

typedef __packed union
{
  __packed struct {uint16_t Value;};
  __packed struct {uint8_t LowByte, HighByte;};
  __packed struct {uint8_t Bytes[2];};
}TWordRec, *PWordRec;

/*-----------------------------------------------------------------------*/

extern uint32_t TMR1MStimeCount;
//定时器中断执行函数
extern void Put_in_timer_Interrupted(void);
extern void USARTHandle_Thread(void);
#endif






