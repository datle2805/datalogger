/**
  ******************************************************************************
  * @file   :debug.h
  * @author :DatLe
  * @version:v2_00
  * @date   :26/02/2016    
  * @brief  :Header file for debug.c module.
  ******************************************************************************
  ******************************************************************************  
  */
  /*
  history
  ------------------------------------------------------------------------------
  version  		author			date		    description
  ------------------------------------------------------------------------------
  v1.00			  DatLe			  26/02/2016  rebuild with RX
  
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stm32f7xx_hal.h"   
/**
@code  
 
@endcode
*/
#ifdef __USING_UART_DEBUG 
  #define UART_DBG                           USART6
  #define UART_DBG_CLK_ENABLE()              __HAL_RCC_USART6_CLK_ENABLE();
  #define UART_DBG_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
  #define UART_DBG_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE() 

  #define UART_DBG_FORCE_RESET()             __HAL_RCC_USART6_FORCE_RESET()
  #define UART_DBG_RELEASE_RESET()           __HAL_RCC_USART6_RELEASE_RESET()

  /* Definition for UART_DBG Pins */
  #define UART_DBG_TX_PIN                    GPIO_PIN_7
  #define UART_DBG_TX_GPIO_PORT              GPIOC  
  #define UART_DBG_TX_AF                     GPIO_AF8_USART6
  #define UART_DBG_RX_PIN                    GPIO_PIN_6
  #define UART_DBG_RX_GPIO_PORT              GPIOC 
  #define UART_DBG_RX_AF                     GPIO_AF8_USART6
  
  /* Definition for USARTz's NVIC */
  #define UART_DBG_IRQn                     USART6_IRQn
  #define UART_DBG_IRQHandler               USART6_IRQHandler
  
  #define UART_DBG_SPEED                    256000
  
  #define UART_DBG_DATA_LEN                 510
  #define UART_DBG_MAX_BUF_RX               10
  #define UART_DBG_RX_CALIBRE_TIME          4 

#endif
/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */
#ifdef __USING_UART_DEBUG
typedef struct
{
  uint16_t                  Len;
  uint8_t                   Data[UART_DBG_DATA_LEN];/* Pointer to Data field (include CRC data) */
}UART_DBG_DataTypeDef;

typedef struct 
{
  volatile uint16_t         Spos;
  volatile uint16_t         Rpos;
  UART_DBG_DataTypeDef      Data[UART_DBG_MAX_BUF_RX];
}UART_DBG_RxBuffer;

typedef struct
{  
  /* Interface */
  void                  (*InputTask)(void);
  void                  (*Calibre_Task)(void);
  /* ---------------------------------------- */
  void                  (*ClearRxBuffer)(void);

  void                  (*ISRRx)(void);
  /* Addition flag */  
  uint8_t               (*CheckDataOK)(void);/* Check Data is OK, pass check sum and ID */
  void                  (*ClearDataOK)(void);

  UART_DBG_DataTypeDef* (*CurrentRxData)(void);
}UART_DBG_DrvTypeDef;
#endif 
/**
  * @}
  */ 
  
/** @defgroup Exported_Constants
  * @{
  */

/**
  * @}
  */ 

/** @defgroup Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup Exported_Functions
  * @{
  */
#ifdef __USING_UART_DEBUG
void UART_DEBUG_Init(void);
void UART_DBG_Print(char *data);
#endif
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif


#endif /* ___H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */   

/******************* (C) COPYRIGHT 2014 ACIS *****END OF FILE****/
