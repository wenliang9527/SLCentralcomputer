/**
 **************************************************************************
 * File Name    : at32f4xx_it.c
 * Description  : at32f4xx interrupt service routines.
 * Date         : 2018-03-15
 * Version      : V1.0.4
 **************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "at32f4xx_it.h"
#include "ExternComm.h"
#include "PMS7003.h"
#include "FST_DCI.h"
#include "CoreProcessor.h"
#include "Sensor.h"


u8 adtime05mm;


void TimingDelay_Decrement(void);

/**
  * @brief  This function handles NMI exception.
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
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
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
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  
}

/******************************************************************************/
/*                 at32f4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_at32f403_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

void TMR1_OV_TMR10_IRQHandler(void)
{
  if(TMR_GetINTStatus(TMR1, TMR_INT_Overflow) != RESET)
  {
//		CM_morinterrupt(&Rotatem);
    TMR_ClearITPendingBit(TMR1, TMR_INT_Overflow);
  }
  TMR_ClearFlag(TMR1, TMR_FLAG_Update);
}




void TMR2_GLOBAL_IRQHandler(void)
{
  if (TMR_GetINTStatus(TMR2, TMR_INT_Overflow) != RESET)
  {
    TMR_ClearITPendingBit(TMR2, TMR_INT_Overflow);
  }
  TMR_ClearFlag(TMR2, TMR_FLAG_Update);
}

void TMR4_GLOBAL_IRQHandler(void)
{
  if (TMR_GetINTStatus(TMR4, TMR_INT_Overflow) != RESET)
  {
    TMR_ClearITPendingBit(TMR4, TMR_INT_Overflow);
//    CM_morinterrupt();
	}
  TMR_ClearFlag(TMR4, TMR_FLAG_Update);
}

void TMR5_GLOBAL_IRQHandler(void)
{
  if (TMR_GetINTStatus(TMR5, TMR_INT_Overflow) != RESET)
  {
    TMR_ClearITPendingBit(TMR5, TMR_INT_Overflow);
  }
  TMR_ClearFlag(TMR5, TMR_FLAG_Update);
}

void TMR6_GLOBAL_IRQHandler(void)
{
  if(TMR_GetINTStatus(TMR6, TMR_INT_Overflow) != RESET)
  {
    TMR_ClearITPendingBit(TMR6, TMR_INT_Overflow);
  }
  TMR_ClearFlag(TMR6, TMR_FLAG_Update);
}

void TMR7_GLOBAL_IRQHandler(void)
{
  if(TMR_GetINTStatus(TMR7, TMR_INT_Overflow) != RESET)
  {
    TMR_ClearITPendingBit(TMR7, TMR_INT_Overflow);
  }
  TMR_ClearFlag(TMR7, TMR_FLAG_Update);
}

//void TMR1_OV_TMR10_IRQHandler(void)
//{
//  if(TMR_GetINTStatus(TMR10, TMR_INT_Overflow) != RESET)
//  {
//    TMR_ClearITPendingBit(TMR10, TMR_INT_Overflow);
//  }
//  TMR_ClearFlag(TMR10, TMR_FLAG_Update);
//}

void TMR1_TRG_COM_TMR11_IRQHandler(void)
{
  if(TMR_GetINTStatus(TMR11, TMR_INT_Overflow) != RESET)
  {
    
		 adtime05mm++;
		 if(adtime05mm>=2)
		 {
			 adtime05mm=0;
			 it_CoreProcessor1ms();
			 it_ADC1ms();
		 }
		 TMR_ClearITPendingBit(TMR11, TMR_INT_Overflow);
  }
  TMR_ClearFlag(TMR11, TMR_FLAG_Update);
}

void TMR8_OV_TMR13_IRQHandler(void)
{
  if (TMR_GetINTStatus(TMR13, TMR_INT_Overflow) != RESET)
  {
    TMR_ClearITPendingBit(TMR13, TMR_INT_Overflow);
  }
  TMR_ClearFlag(TMR13, TMR_FLAG_Update);
}

void TMR8_TRG_COM_TMR14_IRQHandler(void)
{
  if(TMR_GetINTStatus(TMR14, TMR_INT_Overflow) != RESET)
  {
    TMR_ClearITPendingBit(TMR14, TMR_INT_Overflow);
  }
  TMR_ClearFlag(TMR14, TMR_FLAG_Update);
}


void USART1_IRQHandler(void)
{
 USART1_Usbat();
}
void USART2_IRQHandler(void) 
{
 Usart2_ScreenTxdRxd();       // 接串口显示屏
}
void USART3_IRQHandler(void)
{
  USART3_STM32F103();         //从控SAM32F103RCT6
}
void UART4_IRQHandler(void)
{
  Usart4_dn232();            // 主板与中位机通讯
}

void UART5_IRQHandler(void)
{
  USART5_PC232();           //pc_232 计算机
}

void USART6_IRQHandler(void)
{
 Usart6_Rs485TRd();
}

void UART7_IRQHandler(void)//485
{
 Usart7_Rs485TRd();          //pc_232 计算机
}

void UART8_IRQHandler(void)
{
  USART8_579TR();          //579网口
}



void DMA1_Channel1_IRQHandler(void)
{
  if (DMA_GetITStatus(DMA1_INT_TC1))
  {    
   // it_ADCProcessSampleData();
  }
  DMA_ClearITPendingBit(DMA1_INT_GL1);
}







