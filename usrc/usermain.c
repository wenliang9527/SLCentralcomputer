

#include "usermain.h"


#define  USERTIMERNUM   500

u32 TMRouttimeCount=0;
u32 TMR1MStimeCount=0;


/*-----------------------------------------------------------------------------------------------------------*/
/*串口数据处理函数 将使用的串口数据处理函数放入对应的串口中*/
#if     INT_USART1
void US_1_dataHandle()
{
}
#endif
#if     INT_USART2
void US_2_dataHandle()
{
}
#endif
#if     INT_USART3
void US_3_dataHandle()
{
}
#endif
#if     INT_USART4
void US_4_dataHandle()
{
}
#endif
#if     INT_USART5
void US_5_dataHandle()
{
}
#endif
#if     INT_USART6
void US_6_dataHandle()
{
}
#endif
#if     INT_USART7
void US_7_dataHandle()
{
}
#endif	
#if     INT_USART8
void US_8_dataHandle()
{
}
#endif

	
/**
定时执行特定操作的函数。
 */
void IntervalExecute_Function(void)
{

    if(TMR1MStimeCount-TMRouttimeCount>=USERTIMERNUM)
		{
				TMRouttimeCount=TMR1MStimeCount;
			
		}
}







void USARTHandle_Thread()
	{
		USART_Rece_Polling();
		USART_Send_Polling();
	}









