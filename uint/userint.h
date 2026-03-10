
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
#endif
/*-----------------------------------------------------------------------*/
/*LED与蜂鸣器*/
#define  INT_LEDBEEP			1
/*-----------------------------------------------------------------------*/
/**/
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
/*----------USART----------------*/
#define	INT_USART1	1
#define	INT_USART2	1
#define	INT_USART3	1
#define	INT_USART4	1
#define	INT_USART5	1
#define	INT_USART6	1
#define	INT_USART7	1
#define	INT_USART8	1

/*----------flash库----------------*/
#define SL_FLASH_ADDRESS_START         (0x080B0000+1024*20 )   //Flash保存起始地址
#define FLASH_USER_START_ADD ((uint32_t)0x803E000)    //Flash保存起始地址

/*-----------------------SL_HALLIB-------------------------*/
#if     SL_HALLIB
 //HAL引脚定义在图像界面初始化完成
#else
/*-----------------------softSPI-------------------------*/
#if    	INT_SoftSPI
#define MSPICS_PIN    GPIO_Pins_4
#define MSPICS_GPIO_PORT    GPIOE
#define MSPIMISO_PIN    GPIO_Pins_5
#define MSPIMISO_GPIO_PORT    GPIOE
#define MSPIMOSI_PIN    GPIO_Pins_14
#define MSPIMOSI_GPIO_PORT    GPIOB
#define MSPISCK_PIN    GPIO_Pins_15
#define MSPISCK_GPIO_PORT    GPIOD

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


#endif
/*-----------------------SL_HALLIB-------------------------*/






















#endif





















