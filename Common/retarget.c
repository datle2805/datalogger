/**
  ******************************************************************************
  * @file    :retarget.c
  * @author  :DatLe
  * @version :v1_00
  * @date    :25/02/2016
  * @brief   :
  *          This file should be added to the main application to use the provided
  *          functions that manage Leds, push-buttons, COM ports and low level 
  *          HW resources initialization of the different modules available on
  *          STM32 evaluation boards from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 
  /*
  history
  ------------------------------------------------------------------------------
  version  		author			date		      description
  ------------------------------------------------------------------------------
  v1.00			  DatLe			  25/02/2016    Initial build
  
  */
/* Includes ------------------------------------------------------------------*/
#include "retarget.h"
/** @addtogroup Utilities
  * @{
  */ 
  
/** @defgroup Abstraction_Layer
  * @{
  */ 
  
/** @defgroup Public_Variables
  * @{
  */


/** @defgroup Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup Private_Defines
  * @{
  */

/**
  * @}
  */ 

/** @defgroup Private_Macros
  * @{
  */ 
  
/**
  * @}
  */ 

/** @defgroup Private_Variables
  * @{
  */
static TARGET_EnumTypeDef enu_Target=TARGET_UART;                     

/**
  * @}
  */ 


/** @defgroup FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup Private_Functions
  * @{
  */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
struct __FILE { int handle; /* Add whatever is needed */ };
FILE __stdout;
FILE __stdin;
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  static char data;
  data=ch;
  if(enu_Target==TARGET_UART)
  {
    TARGET_UART_Put(data);
  }
  else if(enu_Target==TARGET_LCD)
  {
    TARGET_LCD_Put(data);
  }
  else
  {
    /* Do nothing */
  }

  return ch;
}

void TARGET_Set(TARGET_EnumTypeDef Target)
{
	enu_Target=Target;
}
TARGET_EnumTypeDef TARGET_Get(void)
{
  return enu_Target;
}

__weak void TARGET_UART_Put(char data){}
__weak void TARGET_LCD_Put(char data){}

/******************* (C) COPYRIGHT 2016 DATLE *****END OF FILE****/

