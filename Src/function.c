/**
  ******************************************************************************
  * @file    :function.c
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
#include "function.h"
/** @addtogroup Utilities
  * @{
  */ 
  
/** @defgroup Abstraction_Layer
  * @{
  */ 
  
/** @defgroup Public_Variables
  * @{
  */
#ifdef __USING_UART_DEBUG
  extern UART_HandleTypeDef UartDebugHandle;
  extern UART_DBG_DrvTypeDef *DBG;
#endif

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
void LogMode(void)
{
  static UART_DBG_DataTypeDef* RxData;
  char *ReceiveNumPtr;
  static FIL File;
  uint32_t FileSize;
  static uint32_t Cnt=0;
  #ifdef __USING_UART_DEBUG
  if(DBG->CheckDataOK())
  {
    DBG->ClearDataOK();
    RxData=DBG->CurrentRxData();
    if(RxData)
    {
      /* Save to log file */
      if(f_open(&File, "log.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK)
      {
        FileSize=f_size(&File);
        f_lseek(&File,FileSize);
        if (f_puts((const char *)RxData->Data, &File) > 0)
        {
          /* Push OK */
        }
        f_close(&File);
      }
      else /* if file is not exit */
      {
        /* Create new file */
        if(f_open(&File, "log.txt", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) 
        {
          FileSize=f_size(&File);
          f_lseek(&File,FileSize);
          if (f_puts((const char *)RxData->Data, &File) > 0)
          {
            /* Push OK */
          }
          f_close(&File);
        }
      };
      
      if(strlen((char *)(DBG->CurrentRxData()->Data)))
      {
        ReceiveNumPtr = strtok ((char *)DBG->CurrentRxData()->Data,"\r\n");
        while (ReceiveNumPtr != NULL)
        {        
          /* Push to LCD display */
          USER_LCD_UsrLog("%s\n",ReceiveNumPtr);
          ReceiveNumPtr = strtok (NULL,"\r\n");
        }
        Buzzer_BipTrue();
      }
    }
  }
  #endif
}
/******************* (C) COPYRIGHT 2016 DATLE *****END OF FILE****/
