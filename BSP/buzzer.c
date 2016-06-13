/**
  ******************************************************************************
  * @file    buzzer.c
  * @author  DatLe
  * @version V1.00
  * @date    23-May-2013
  * @brief   STM32xx-EVAL abstraction layer. 
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
  
/* Includes ------------------------------------------------------------------*/
#include "buzzer.h"

/** @addtogroup Utilities
  * @{
  */ 
  

/** @defgroup Abstraction_Layer
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
static Buzzer_TypeDef Buzzer;
/* Timer handler declaration */
static TIM_HandleTypeDef    TimHandle;
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
  * @brief  Configures Buzzer GPIO and load default value.
  * @param   
  *   This parameter can be one of following parameters:
  *     @arg none
  * @retval None
  */

void Buzzer_Init(void)
{
  uint32_t uhPrescalerValue;
  /* Timer Output Compare Configuration Structure declaration */
  TIM_OC_InitTypeDef sConfig;
  GPIO_InitTypeDef   GPIO_InitStruct;
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  BUZZER_TIMx_CLK_ENABLE();
    
  /* Enable GPIO Channels Clock */
  BUZZER_GPIO_CLK_ENABLE();
  
  /* Configure PC.6 (TIM3_Channel1), PC.7 (TIM3_Channel2), PC.8 (TIM3_Channel3),
     PC.9 (TIM3_Channel4) in output, push-pull, alternate function mode
  */
  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
  
  GPIO_InitStruct.Pin = BUZZER_PIN;
  HAL_GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStruct);

  /* Compute the prescaler value to have TIM4 counter clock equal to 60 MHz */
  uhPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 6000000) - 1;

  /*##-1- Configure the TIM peripheral #######################################*/ 
  /* -----------------------------------------------------------------------
  TIM4 Configuration: generate 4 PWM signals with 4 different duty cycles.
    
    In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM4CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM4CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM4 counter clock at 10 MHz, the prescaler is computed as follows:
       Prescaler = (TIM4CLK / TIM4 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /10 MHz) - 1
                                              
    To get TIM4 output clock at 3 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM4 counter clock / TIM4 output clock) - 1
           = 6650
                  
    TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%
    TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR)* 100 = 37.5%
    TIM4 Channel3 duty cycle = (TIM4_CCR3/ TIM4_ARR)* 100 = 25%
    TIM4 Channel4 duty cycle = (TIM4_CCR4/ TIM4_ARR)* 100 = 12.5%

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency     
  ----------------------------------------------------------------------- */ 
  
  /* Initialize TIMx peripheral as follow:
       + Prescaler = (SystemCoreClock/2)/10000000
       + Period = 6650
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Instance = BUZZER_TIMx;
  
  TimHandle.Init.Prescaler = uhPrescalerValue;
  TimHandle.Init.Period = 6650;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
  }
  
  /*##-2- Configure the PWM channels #########################################*/ 
  /* Common configuration for all channels */
  sConfig.OCMode = TIM_OCMODE_PWM1;
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode = TIM_OCFAST_DISABLE;

  /* Set the pulse value for channel 2 */
  sConfig.Pulse = 3325;  
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
  }

  /*##-3- Start PWM signals generation #######################################*/ 

  /* Start channel 2 */
  if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM Generation Error */
  }
  
  Buzzer_Off();
  
  Buzzer.Enable = false;
	Buzzer.Counter = 0;
	Buzzer.BipTimeTrue = BUZZER_BIP_TIME_TRUE;
  Buzzer.BipTimeFalse = BUZZER_BIP_TIME_FALSE;
  Buzzer.BipTime = Buzzer.BipTimeTrue;
  
  Buzzer.Freq=3325;
}

/**
  * @brief : Turn ON buzzer 1
  * @param : none  
  * @retval : none
  */
static void Buzzer_On(void)
{
  TIM_OC_InitTypeDef sConfig;
  sConfig.OCMode = TIM_OCMODE_PWM1;
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode = TIM_OCFAST_DISABLE;

  /* Set the pulse value for channel 1 */
  sConfig.Pulse = Buzzer.Freq;  
  HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);
}

/**
  * @brief : Turn ON buzzer 1
  * @param : none  
  * @retval : none
  */
static void Buzzer_Off(void)
{
  TIM_OC_InitTypeDef sConfig;
  sConfig.OCMode = TIM_OCMODE_PWM1;
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode = TIM_OCFAST_DISABLE;

  /* Set the pulse value for channel 4 */
  sConfig.Pulse = 0;  
  HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
}

void Buzzer_SetFreq(uint32_t Freq)
{
  Buzzer.Freq=Freq;
}


void Buzzer_BipTrue(void)
{
  Buzzer.Enable= true;
  Buzzer.Counter= 0;
  Buzzer.BipTime= Buzzer.BipTimeTrue;
  Buzzer_On();
}

/**
  * @brief : Bip the buzzer with duration time is bip false time
  * @param : none  
  * @retval : none
  */
void Buzzer_BipFalse(void)
{
  Buzzer.Enable= true;
  Buzzer.Counter= 0;
  Buzzer.BipTime= Buzzer.BipTimeFalse;
  Buzzer_On();
}

/**
  * @brief : Buzzer ISR handle
  * @param : none  
  * @retval : none
  */
void Buzzer_ISR(void)
{
  if(Buzzer.Enable)
	{
		Buzzer.Counter++;
		if(Buzzer.Counter>Buzzer.BipTime)
		{
			Buzzer.Enable=false;
			Buzzer.Counter=0;
			Buzzer_Off();
		}
	}
}


/**
  * @}
  */ 

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
