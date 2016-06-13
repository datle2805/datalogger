/**
  ******************************************************************************
  * @file   :rtc.h
  * @author :DatLe
  * @version:v1_00 
  * @date   :10/01/2015    
  * @brief  :Header file for rtc.c module.
  ******************************************************************************
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
   #include "stm32f7xx.h"
   
/**
@code  
 
@endcode
*/
#define RTC_CLOCK_SOURCE_LSE  
//#define RTC_CLOCK_SOURCE_LSI

#ifdef RTC_CLOCK_SOURCE_LSI
  #define RTC_ASYNCH_PREDIV  0x7F
  #define RTC_SYNCH_PREDIV   0x0130
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
  #define RTC_ASYNCH_PREDIV  0x7F
  #define RTC_SYNCH_PREDIV   0x00FF
#endif

/**
  * @}
  */
  
/** @defgroup Exported_Types
  * @{
  */
typedef struct
{
  uint8_t Month;
  uint8_t Date;
  uint16_t Year;
  uint8_t Hour;
  uint8_t Min;
  uint8_t Sec;
  uint8_t TimeZone;
  uint8_t DayLight;
}Time_TypeDef;
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
  void RTC_Init(void);
  void RTC_CalendarConfig(void);
  uint8_t RTC_CalendarConfigEx(uint8_t Day,uint8_t Month,uint8_t Year,uint8_t Hour,uint8_t Min,uint8_t Sec);
  uint8_t RTC_CalendarConfigExFull(uint8_t Day,uint8_t Month,uint8_t Year,uint8_t Hour,uint8_t Min,uint8_t Sec, uint8_t TimeZone, uint8_t DayLightTime);
  Time_TypeDef RTC_GetCurrentTime(void);
  uint32_t DateTimetoLong(Time_TypeDef CurrentTime);
  Time_TypeDef LongToDateTime(uint32_t u32_data);
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
