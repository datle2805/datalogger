/**
  ******************************************************************************
  * @file   :retarget.h
  * @author :DatLe
  * @version:v1_00
  * @date   :  
  * @brief  :Header file for retarget.c module.
  ******************************************************************************
  ******************************************************************************  
  */
  /*
  history
  ------------------------------------------------------------------------------
  version  		author			date		    description
  ------------------------------------------------------------------------------
  v1.00			  DatLe			  26/02/2016  Initial build
  
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RE_TARGET_H
#define __RE_TARGET_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stdlib.h"
/**
@code  
 
@endcode
*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */
typedef enum
{
  TARGET_UART=0,
  TARGET_LCD,
  
  TARGET_NUM
}TARGET_EnumTypeDef;
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
void TARGET_Set(TARGET_EnumTypeDef Target);
TARGET_EnumTypeDef TARGET_Get(void);

void TARGET_UART_Put(char data);
void TARGET_LCD_Put(char data);
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

/******************* (C) COPYRIGHT 2016 ACIS *****END OF FILE****/
