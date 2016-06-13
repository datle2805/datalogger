/**
  ******************************************************************************
  * @file   :function.h
  * @author :DatLe
  * @version:
  * @date   :  
  * @brief  :Header file for function.c module.
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
#ifndef __FUNCTION_H
#define __FUNCTION_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "timeout.h"
#include "rtc.h"  
#include "buzzer.h"
#ifdef __USING_UART_DEBUG
#include "debug.h" 
#endif
#include "stm32746g_discovery_sdram.h"
#include "GUI.h"
   
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_sd.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"
   
#include "userlog.h"/**
@code  
 
@endcode
*/

/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */

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
void LogMode(void);
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
