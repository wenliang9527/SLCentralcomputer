
#ifndef __USERINT_H
#define __USERINT_H

/*---------------------------HAL库与标准库---------------------------*/
#define SL_HALLIB    0    //    HAL库切换  1HAL  0标准

#if     SL_HALLIB
#include "at32f403a_407_wk_config.h"
#include "wk_system.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#else
#include "at32f4xx.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#endif
/*-----------------------------------------------------------------------*/
/*LED与蜂鸣器*/
#define  INT_LEDBEEP			1
/*-----------------------------------------------------------------------*/
/**/
/*TMR*/
/*-----------------------------------------------------------------------*/
#define  INT_TMRER				1
/*USART*/
/*-----------------------------------------------------------------------*/
#define  INT_ATUSART			1
/*-----------------------------------------------------------------------*/
/*模拟SPI*/
#define  INT_SoftSPI			1
/*-----------------------------------------------------------------------*/
/*传感器检测*/
#define  INT_Sensor				1
/*-----------------------------------------------------------------------*/
/**/
#define  INT_MOScontrol		0
/*-----------------------------------------------------------------------*/
/**/
#define  INT_ReadXMC			0
/*-----------------------------------------------------------------------*/
/*读卡*/
#define  INT_Readcard			1
/*-----------------------------------------------------------------------*/
/*模拟IIC*/
#define  INT_SofIIC				1
/*-----------------------------------------------------------------------*/
/*DS18B20*/
#define  INT_DS18B20			1   //是否有18b20
#define DS_1    1             //一号
#define DS_2    0 
#define DS_3    0
/*-----------------------------------------------------------------------*/
/*W25Qxx存储*/
#define  INT_W25Qxx			  1
/*-----------------------------------------------------------------------*/

/************************************************************************/


/*----------flash库----------------*/
#define SL_FLASH_ADDRESS_START         (0x080B0000+1024*20 )   //Flash保存起始地址
#define FLASH_USER_START_ADD ((uint32_t)0x803E000)    //Flash保存起始地址

/*-----------------------SL_HALLIB-------------------------*/

/*-----------------------softSPI-------------------------*/
#if    	INT_SoftSPI
#define MSPICS_PIN    GPIO_Pins_4
#define MSPICS_GPIO_PORT    GPIOA

#define MSPISCK_PIN    GPIO_Pins_5
#define MSPISCK_GPIO_PORT    GPIOA

#define MSPIMISO_PIN    GPIO_Pins_6
#define MSPIMISO_GPIO_PORT    GPIOA

#define MSPIMOSI_PIN    GPIO_Pins_7
#define MSPIMOSI_GPIO_PORT    GPIOA



#define SL_SPI_MOSI_H    GPIO_SetBits(MSPIMOSI_GPIO_PORT,MSPIMOSI_PIN)
#define SL_SPI_MOSI_L    GPIO_ResetBits(MSPIMOSI_GPIO_PORT,MSPIMOSI_PIN)
#define SL_SPI_MISO      GPIO_ReadInputDataBit(MSPIMISO_GPIO_PORT,MSPIMISO_PIN)
#define SL_SPI_SCK_H    GPIO_SetBits(MSPISCK_GPIO_PORT,MSPISCK_PIN)
#define SL_SPI_SCK_L    GPIO_ResetBits(MSPISCK_GPIO_PORT,MSPISCK_PIN)
#define SL_SPI_CS_H    GPIO_SetBits(MSPICS_GPIO_PORT,MSPICS_PIN)
#define SL_SPI_CS_L    GPIO_ResetBits(MSPICS_GPIO_PORT,MSPICS_PIN)
#endif
/*-----------------------softIIC-------------------------*/
#if     INT_SofIIC
#define MIIC_GPIO_RCC_CLK       RCC_APB2PERIPH_GPIOA 
#define MIICSDA_PIN    GPIO_Pins_12
#define MIICSDA_GPIO_PORT    GPIOA
#define MIICSCL_PIN    GPIO_Pins_15
#define MIICSCL_GPIO_PORT    GPIOA
#define IIC_1_SCL_H    GPIO_SetBits(MIICSCL_GPIO_PORT,MIICSCL_PIN)
#define IIC_1_SCL_L    GPIO_ResetBits(MIICSCL_GPIO_PORT,MIICSCL_PIN)
#define IIC_1_SDA_H    GPIO_SetBits(MIICSDA_GPIO_PORT,MIICSDA_PIN)
#define IIC_1_SDA_L    GPIO_ResetBits(MIICSDA_GPIO_PORT,MIICSDA_PIN)
#define IIC_1_READ_SDA     GPIO_ReadInputDataBit(MIICSDA_GPIO_PORT,MIICSDA_PIN)
#endif
/*-----------------------LED BEEP-------------------------*/
#if     INT_LEDBEEP
#define BEEP_PIN    GPIO_Pins_15
#define BEEP_GPIO_PORT    GPIOE
#define LED1_PIN  GPIO_Pins_13
#define LED1_GPIO_PORT GPIOC
#endif
/*-----------------------Sensor-------------------------*/
#if    	INT_Sensor

#define SERNUM   6
#define  SERCKCount    500

#define SER_1_set 1
#define SER_2_set 1
#define SER_3_set 1
#define SER_4_set 1
#define SER_5_set 1
#define SER_6_set 1
#define SER_7_set 1
#define SER_8_set 0
#if SER_1_set
#define MSIGNAL_1_PIN    GPIO_Pins_1
#define MSIGNAL_1_GPIO_PORT    GPIOD
#endif
#if SER_2_set
#define MSIGNAL_2_PIN    GPIO_Pins_3
#define MSIGNAL_2_GPIO_PORT    GPIOD
#endif
#if SER_3_set
#define MSIGNAL_3_PIN    GPIO_Pins_4
#define MSIGNAL_3_GPIO_PORT    GPIOD
#endif
#if SER_4_set
#define MSIGNAL_4_PIN    GPIO_Pins_5
#define MSIGNAL_4_GPIO_PORT    GPIOD
#endif
#if SER_5_set
#define MSIGNAL_5_PIN    GPIO_Pins_6
#define MSIGNAL_5_GPIO_PORT    GPIOD
#endif
#if SER_6_set
#define MSIGNAL_6_PIN    GPIO_Pins_7
#define MSIGNAL_6_GPIO_PORT    GPIOD
#endif
#if SER_7_set
#define MSIGNAL_7_PIN    GPIO_Pins_14
#define MSIGNAL_7_GPIO_PORT    GPIOE
#endif
#if SER_8_set
#define MSIGNAL_8_PIN    GPIO_Pins_4
#define MSIGNAL_8_GPIO_PORT    GPIOE
#endif
#endif
/*-----------------------DS18B20-------------------------*/
#if  INT_DS18B20
#define DS18B20_1_PIN    GPIO_Pins_3
#define DS18B20_1_GPIO_PORT    GPIOC
#define DS18B20_2_PIN    GPIO_Pins_3
#define DS18B20_2_GPIO_PORT    GPIOC
#define DS18B20_3_PIN    GPIO_Pins_3
#define DS18B20_3_GPIO_PORT    GPIOC
#endif
/*-----------------------W25Qxx-------------------------*/
#if INT_W25Qxx

	#define FLASH_SPI               SPI1
	#define SPIx_RCC_CLK            RCC_APB2PERIPH_SPI1
	#define SPIx_GPIO_RCC_CLK       RCC_APB2PERIPH_GPIOA 
	#define SPIx_PIN_NSS            GPIO_Pins_4
	#define SPIx_PORT_NSS           GPIOA
	#define SPIx_PIN_SCK            GPIO_Pins_5
	#define SPIx_PORT_SCK           GPIOA
	#define SPIx_PIN_MISO           GPIO_Pins_6
	#define SPIx_PORT_MISO          GPIOA
	#define SPIx_PIN_MOSI           GPIO_Pins_7
	#define SPIx_PORT_MOSI          GPIOA

	#define SPIx_DMA                DMA1
	#define SPIx_DMA_RCC_CLK        RCC_AHBPERIPH_DMA1
	#define SPIx_Rx_DMA_Channel     DMA1_Channel2
	#define SPIx_Rx_DMA_FLAG        DMA1_FLAG_TC2
	#define SPIx_Tx_DMA_Channel     DMA1_Channel3
	#define SPIx_Tx_DMA_FLAG        DMA1_FLAG_TC3
#endif
#if INT_ATUSART
/*----------USART----------------*/
#define	INT_USART1	1
#define	INT_USART2	1
#define	INT_USART3	1
#define	INT_USART4	1
#define	INT_USART5	1
#define	INT_USART6	1
#define	INT_USART7	1
#define	INT_USART8	1

#define Usart_1_sendOUTtime 5    // 串口1发送超时时间（单位：ms）
#define Usart_2_sendOUTtime 5    
#define Usart_3_sendOUTtime 5    
#define Usart_4_sendOUTtime 5
#define Usart_5_sendOUTtime 5
#define Usart_6_sendOUTtime 5
#define Usart_7_sendOUTtime 5
#define Usart_8_sendOUTtime 5

#if     SL_HALLIB
/*----------引脚初始化名称------------*/
//USTX1   USART定义名称
//USRX1
//USart1TR   485发送接收引脚名
#define Usart_1_485         0
#define Usart_2_485         0
#define Usart_3_485         0
#define Usart_4_485         0
#define Usart_5_485         0
#define Usart_6_485         0
#define Usart_7_485         0
#define Usart_8_485         0
/*---------*/
//中断函数  放入串口中断中
//USART1_interrupt_Function
//USART2_interrupt_Function
//USART3_interrupt_Function
//USART4_interrupt_Function
//USART5_interrupt_Function
//USART6_interrupt_Function
//USART7_interrupt_Function
//USART8_interrupt_Function
/*---------------------------*/
#if Usart_1_485
#define USart1TR_PIN    GPIO_Pins_10
#define USart1TR_GPIO_PORT    GPIOA
#define USart1TR_TX       gpio_bits_set(USart1TR_GPIO_PORT,USart1TR_PIN)
#define USart1TR_RX				gpio_bits_reset(USart1TR_GPIO_PORT,USart1TR_PIN)
#endif
#if Usart_2_485
#define USart2TR_PIN    GPIO_Pins_11
#define USart2TR_GPIO_PORT    GPIOA
#define USart2TR_TX       gpio_bits_set(USart2TR_GPIO_PORT,USart2TR_PIN)
#define USart2TR_RX				gpio_bits_reset(USart2TR_GPIO_PORT,USart2TR_PIN)
#endif
#if Usart_3_485
#define USart3TR_PIN    GPIO_Pins_12
#define USart3TR_GPIO_PORT    GPIOA
#define USart3TR_TX       gpio_bits_set(USart3TR_GPIO_PORT,USart3TR_PIN)
#define USart3TR_RX				gpio_bits_reset(USart3TR_GPIO_PORT,USart3TR_PIN)
#endif
#if Usart_4_485
#define USart4TR_PIN    GPIO_Pins_4
#define USart4TR_GPIO_PORT    GPIOB
#define USart4TR_TX       gpio_bits_set(USart4TR_GPIO_PORT,USart4TR_PIN)
#define USart4TR_RX				gpio_bits_reset(USart4TR_GPIO_PORT,USart4TR_PIN)
#endif
#if Usart_5_485
#define USart5TR_PIN    GPIO_Pins_13
#define USart5TR_GPIO_PORT    GPIOB
#define USart5TR_TX       gpio_bits_set(USart5TR_GPIO_PORT,USart5TR_PIN)
#define USart5TR_RX				gpio_bits_reset(USart5TR_GPIO_PORT,USart5TR_PIN)
#endif
#if Usart_6_485
#define USart6TR_PIN    GPIO_Pins_6
#define USart6TR_GPIO_PORT    GPIOA
#define USart6TR_TX       gpio_bits_set(USart6TR_GPIO_PORT,USart6TR_PIN)
#define USart6TR_RX				gpio_bits_reset(USart6TR_GPIO_PORT,USart6TR_PIN)
#endif
#if Usart_7_485
#define USart7TR_PIN    GPIO_Pins_5
#define USart7TR_GPIO_PORT    GPIOB
#define USart7TR_TX       gpio_bits_set(USart7TR_GPIO_PORT,USart7TR_PIN)
#define USart7TR_RX				gpio_bits_reset(USart7TR_GPIO_PORT,USart7TR_PIN)
#endif
#if Usart_8_485
#define USart8TR_PIN    GPIO_Pins_7
#define USart8TR_GPIO_PORT    GPIOA
#define USart8TR_TX       gpio_bits_set(USart8TR_GPIO_PORT,USart8TR_PIN)
#define USart8TR_RX				gpio_bits_reset(USart8TR_GPIO_PORT,USart8TR_PIN)
#endif
#else
#if INT_USART1
#define Usart_1_Remap      0    //是否重映射
#define Usart_1_Baudrate   19200  //波特率
#define Usart_1_485         0
#if Usart_1_Remap
#define USTX1_PIN    GPIO_Pins_6
#define USTX1_GPIO_PORT    GPIOB
#define USRX1_PIN    GPIO_Pins_7
#define USRX1_GPIO_PORT    GPIOB
#else
#define USTX1_PIN    GPIO_Pins_9
#define USTX1_GPIO_PORT    GPIOA
#define USRX1_PIN    GPIO_Pins_10
#define USRX1_GPIO_PORT    GPIOA
#endif
#if Usart_1_485
#define USart1TR_PIN    GPIO_Pins_10
#define USart1TR_GPIO_PORT    GPIOA
#define USart1TR_TX       GPIO_SetBits(USart1TR_GPIO_PORT,USart1TR_PIN)
#define USart1TR_RX				GPIO_ResetBits(USart1TR_GPIO_PORT,USart1TR_PIN)
#endif
#endif
#if INT_USART2
#define Usart_2_Remap      0    //是否重映射
#define Usart_2_Baudrate   19200
#define Usart_2_485         0
#if Usart_2_Remap
#define USTX2_PIN    GPIO_Pins_5
#define USTX2_GPIO_PORT    GPIOD
#define USRX2_PIN    GPIO_Pins_6
#define USRX2_GPIO_PORT    GPIOD
#else
#define USTX2_PIN    GPIO_Pins_2
#define USTX2_GPIO_PORT    GPIOA
#define USRX2_PIN    GPIO_Pins_3
#define USRX2_GPIO_PORT    GPIOA
#endif
#if Usart_2_485
#define USart2TR_PIN    GPIO_Pins_11
#define USart2TR_GPIO_PORT    GPIOA
#define USart2TR_TX       GPIO_SetBits(USart2TR_GPIO_PORT,USart2TR_PIN)
#define USart2TR_RX				GPIO_ResetBits(USart2TR_GPIO_PORT,USart2TR_PIN)
#endif

#endif
#if INT_USART3
#define Usart_3_Remap      0    //是否重映射
#define Usart_3_Baudrate   19200
#define Usart_3_485         0
#if Usart_3_Remap
#define USTX3_PIN    GPIO_Pins_8
#define USTX3_GPIO_PORT    GPIOD
#define USRX3_PIN    GPIO_Pins_9
#define USRX3_GPIO_PORT    GPIOD
#else
#define USTX3_PIN    GPIO_Pins_10
#define USTX3_GPIO_PORT    GPIOB
#define USRX3_PIN    GPIO_Pins_11
#define USRX3_GPIO_PORT    GPIOB
#endif	
#if Usart_3_485
#define USart3TR_PIN    GPIO_Pins_12
#define USart3TR_GPIO_PORT    GPIOA
#define USart3TR_TX       GPIO_SetBits(USart3TR_GPIO_PORT,USart3TR_PIN)
#define USart3TR_RX				GPIO_ResetBits(USart3TR_GPIO_PORT,USart3TR_PIN)
#endif

#endif
#if INT_USART4
#define Usart_4_Remap      0    //是否重映射
#define Usart_4_Baudrate   19200
#define Usart_4_485         0
#if Usart_4_Remap
#define USTX4_PIN    GPIO_Pins_0
#define USTX4_GPIO_PORT    GPIOA
#define USRX4_PIN    GPIO_Pins_1
#define USRX4_GPIO_PORT    GPIOA
#else
#define USTX4_PIN    GPIO_Pins_10
#define USTX4_GPIO_PORT    GPIOC
#define USRX4_PIN    GPIO_Pins_11
#define USRX4_GPIO_PORT    GPIOC
#endif	
#if Usart_4_485
#define USart4TR_PIN    GPIO_Pins_4
#define USart4TR_GPIO_PORT    GPIOB
#define USart4TR_TX       GPIO_SetBits(USart4TR_GPIO_PORT,USart4TR_PIN)
#define USart4TR_RX				GPIO_ResetBits(USart4TR_GPIO_PORT,USart4TR_PIN)
#endif  
#endif

#if INT_USART5
#define Usart_5_Remap      0    //是否重映射
#define Usart_5_Baudrate   19200
#define Usart_5_485         0
#if Usart_5_Remap
#define USTX5_PIN    GPIO_Pins_9
#define USTX5_GPIO_PORT    GPIOB
#define USRX5_PIN    GPIO_Pins_8
#define USRX5_GPIO_PORT    GPIOB
#else
#define USTX5_PIN    GPIO_Pins_12
#define USTX5_GPIO_PORT    GPIOC
#define USRX5_PIN    GPIO_Pins_2
#define USRX5_GPIO_PORT    GPIOD
#endif
#if Usart_5_485
#define USart5TR_PIN    GPIO_Pins_5
#define USart5TR_GPIO_PORT    GPIOB
#define USart5TR_TX       GPIO_SetBits(USart5TR_GPIO_PORT,USart5TR_PIN)
#define USart5TR_RX				GPIO_ResetBits(USart5TR_GPIO_PORT,USart5TR_PIN)

#endif
#endif

#if INT_USART6
#define Usart_6_Remap      0    //是否重映射
#define Usart_6_Baudrate   19200
#define Usart_6_485         0
#if Usart_6_Remap
#define USTX6_PIN    GPIO_Pins_4
#define USTX6_GPIO_PORT    GPIOA
#define USRX6_PIN    GPIO_Pins_5
#define USRX6_GPIO_PORT    GPIOA
#else
#define USTX6_PIN    GPIO_Pins_6
#define USTX6_GPIO_PORT    GPIOC
#define USRX6_PIN    GPIO_Pins_7
#define USRX6_GPIO_PORT    GPIOC
#endif
#if Usart_6_485
#define USart6TR_PIN    GPIO_Pins_6
#define USart6TR_GPIO_PORT    GPIOA
#define USart6TR_TX       GPIO_SetBits(USart6TR_GPIO_PORT,USart6TR_PIN)
#define USart6TR_RX				GPIO_ResetBits(USart6TR_GPIO_PORT,USart6TR_PIN)
#endif
#endif	
#if INT_USART7
#define Usart_7_Remap      0    //是否重映射
#define Usart_7_Baudrate   19200
#define Usart_7_485         0
#if Usart_7_Remap
#define USTX7_PIN    GPIO_Pins_4
#define USTX7_GPIO_PORT    GPIOB
#define USRX7_PIN    GPIO_Pins_3
#define USRX7_GPIO_PORT    GPIOB
#else
#define USTX7_PIN    GPIO_Pins_8
#define USTX7_GPIO_PORT    GPIOE
#define USRX7_PIN    GPIO_Pins_7
#define USRX7_GPIO_PORT    GPIOE
#endif
#if Usart_7_485
#define USart7TR_PIN    GPIO_Pins_7
#define USart7TR_GPIO_PORT    GPIOA
#define USart7TR_TX       GPIO_SetBits(USart7TR_GPIO_PORT,USart7TR_PIN)
#define USart7TR_RX				GPIO_ResetBits(USart7TR_GPIO_PORT,USart7TR_PIN)
#endif

#endif	
#if INT_USART8
#define Usart_8_Remap      0    //是否重映射
#define Usart_8_Baudrate   19200
#define Usart_8_485         0

#if Usart_8_Remap
#define USTX8_PIN    GPIO_Pins_2
#define USTX8_GPIO_PORT    GPIOC
#define USRX8_PIN    GPIO_Pins_3
#define USRX8_GPIO_PORT    GPIOC
#else
#define USTX8_PIN    GPIO_Pins_1
#define USTX8_GPIO_PORT    GPIOE
#define USRX8_PIN    GPIO_Pins_0
#define USRX8_GPIO_PORT    GPIOE
#endif
#if Usart_8_485
#define USart8TR_PIN    GPIO_Pins_8
#define USart8TR_GPIO_PORT    GPIOC
#define USart8TR_TX       GPIO_SetBits(USart8TR_GPIO_PORT,USart8TR_PIN)
#define USart8TR_RX				GPIO_ResetBits(USart8TR_GPIO_PORT,USart8TR_PIN)
#endif
#endif
#endif


#endif



#if INT_TMRER
#define Timer_TMR_1     0
#define Timer_TMR_2     1
#define Timer_TMR_3     0
#define Timer_TMR_4     1
#define Timer_TMR_5     0
#define Timer_TMR_6     0
#define Timer_TMR_7     0
#define Timer_TMR_8     0
#define Timer_TMR_9     0
#define Timer_TMR_10     0
#define Timer_TMR_11     1

#define TMR_1_OVERvalue     1000
#define TMR_1_Fredivision   192
#define TMR_2_OVERvalue     1000
#define TMR_2_Fredivision   192
#define TMR_3_OVERvalue     1000
#define TMR_3_Fredivision   192
#define TMR_4_OVERvalue     1000
#define TMR_4_Fredivision   192
#define TMR_5_OVERvalue     1000
#define TMR_5_Fredivision   192
#define TMR_6_OVERvalue     1000
#define TMR_6_Fredivision   192
#define TMR_7_OVERvalue     1000
#define TMR_7_Fredivision   192
#define TMR_8_OVERvalue     1000
#define TMR_8_Fredivision   192
#define TMR_9_OVERvalue     1000
#define TMR_9_Fredivision   192
#define TMR_10_OVERvalue     1000
#define TMR_10_Fredivision   192
#define TMR_11_OVERvalue     1000
#define TMR_11_Fredivision   192

#endif

/*-----------------------SL_HALLIB-------------------------*/

#endif





















