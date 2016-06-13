/**
  ******************************************************************************
  * @file    buzzer.h
  * @author  DatLe
  * @version v2.00
  * @date    28-Aug-2014
  * @brief   Header file for led.c module.
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
#ifndef __BUZZER_H
#define __BUZZER_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "stm32f7xx.h"
   #include "stdbool.h"    

  /* Select some of configs here!!!!!!!!! */
    //#define BUZZER_ON_LOW_LEVEL
    #define BUZZER_OFF_LOW_LEVEL
  /* ----------------------------------------------------------------------- */
  /* Definition for TIMx clock resources */
  #define BUZZER_TIMx                             TIM12
  #define BUZZER_TIMx_CLK_ENABLE                  __HAL_RCC_TIM12_CLK_ENABLE

  /* Definition for USARTx Pins */
  #define BUZZER_PIN                              GPIO_PIN_6    /* CH2 */
  #define BUZZER_GPIO_PORT                        GPIOH
  #define BUZZER_GPIO_CLK_ENABLE()                __GPIOH_CLK_ENABLE()  
  #define BUZZER_GPIO_CLK_DISABLE()               __GPIOH_CLK_DISABLE()                                                
/**
@code  
 
@endcode
*/

/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */
typedef struct
{
  bool Enable;
	
	uint32_t Counter;
  uint32_t BipTime;
	uint32_t BipTimeTrue;
  uint32_t BipTimeFalse;
  
  uint32_t Freq;
}Buzzer_TypeDef;

/**
  * @}
  */ 
  
/** @defgroup Exported_Constants
  * @{
  */
  #define BUZZER_BIP_TIME_TRUE								  4
  #define BUZZER_BIP_TIME_FALSE                 15
  #define BUZZER_EX_FREQ1                       3240
/**
  * @}
  */ 

/** @defgroup Exported_Macros
  * @{
  */
  #ifdef BUZZER_ON_LOW_LEVEL
    #define BUZZER_ON   HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);
    #define BUZZER_OFF  HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_SET);
  #else
    #define BUZZER_ON   HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_SET);
    #define BUZZER_OFF  HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);
  #endif
/**
  * @}
  */ 

/** @defgroup Exported_Functions
  * @{
  */
		void Buzzer_Init (void);

		static void Buzzer_On(void);
    static void Buzzer_Off(void);
    void Buzzer_BipTrue(void);
    void Buzzer_BipFalse(void);
		void Buzzer_ISR(void);

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif


#endif /* __LED_H */

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
