/**
  ******************************************************************************
  * @file    timeout.h
  * @author  DatLe
  * @version V1.00
  * @date    27-July-2013
  * @brief   Header file for buzzer.c module.
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMEOUT_H
#define __TIMEOUT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
 #include "stdint.h"
 #include "stdbool.h"
/** @addtogroup hardware_define
  * @{
  */ 
	 
	#define TIMEOUT_TIME_DEFAULT				200

/** @defgroup Abstraction_Layer
  * @{
  */
  
/** @defgroup HARDWARE_RESOURCES
  * @{
  */

/**
@code  
 The table below gives an overview of the hardware resources supported by each 
 STM32 EVAL board.

@endcode
*/

/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */
  
typedef enum
{
  TO_NOCCUR=0,
  TO_OCCUR
}TO_StatusTypeDef;

typedef struct 
{
	TO_StatusTypeDef	Status;
  volatile bool     Enable;
  volatile uint32_t Time;
	volatile uint32_t CntTaget;	
}TO_TypeDef;


 
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
	void TO_Init(TO_TypeDef *uTO);
	void TO_Start(TO_TypeDef *uTO,uint32_t setTime);
	void TO_Stop(TO_TypeDef *uTO);
	TO_StatusTypeDef TO_ReadStatus(TO_TypeDef *uTO);
	void TO_Reset(TO_TypeDef *uTO);
	void TO_ClearStatus(TO_TypeDef *uTO);
  uint32_t TO_GetCurrentTime(void);
	static void TO_SetStatus(TO_TypeDef *uTO);
	
	void TO_ISR(void);
	void TO_Task(TO_TypeDef *uTO);

	
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif


#endif /* __LED7SEG_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */   

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
