/**
  ******************************************************************************
  * @file    :rtc.c
  * @author  :DatLe
  * @version :v1_00 
  * @date    :10/01/2015
  * @brief   :RTC handle
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
#include "rtc.h"

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
RTC_HandleTypeDef RtcHandle;
const char *MONTH[]={  "xxx",
                       "January",
                       "February",
                       "March",
                       "April",
                       "May",
                       "June",
                       "July",
                       "August",
                       "September",
                       "October",
                       "November",
                       "December"};  
/**
  * @}
  */ 


/** @defgroup FunctionPrototypes
  * @{
  */
/**
  * @brief : Init RTC function and check backup data to re-load value 
  * @param : none  
  * @retval : none
  */
void RTC_Init(void)
{
  /*##-1- Configure the RTC peripheral #######################################*/
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  HAL_RTC_Init(&RtcHandle);
  /*##-2- Check if Data stored in BackUp register0: No Need to reconfigure RTC#*/
  /* Read the Back Up Register 0 Data */
  if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR0) != 0x32F2)
  {  
    /* Configure RTC Calendar */
    RTC_CalendarConfig();
  }
}
/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void RTC_CalendarConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
    
  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2014 */
  sdatestructure.Year = 15;
  sdatestructure.Month = RTC_MONTH_MAY;
  sdatestructure.Date = 22;
  sdatestructure.WeekDay = RTC_WEEKDAY_SUNDAY;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
  } 
  
  /*##-2- Configure the Time #################################################*/
  /* Set Time: 02:00:00 */
  stimestructure.Hours = 15;
  stimestructure.Minutes = 25;
  stimestructure.Seconds = 0;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
  }
  
  /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
  HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR0,0x32F2);  
}

/**
  * @brief : SET Calendar extend
  * @param : RTC_DateTypeDef and RTC_TimeTypeDef
  * @retval : none
  */
uint8_t RTC_CalendarConfigEx(uint8_t Day,uint8_t Month,uint8_t Year,uint8_t Hour,uint8_t Min,uint8_t Sec)
{
  static RTC_DateTypeDef sdatestructure;
  static RTC_TimeTypeDef stimestructure; 

  /* Checking data input */
  if(IS_RTC_DATE(Day)&&IS_RTC_MONTH(Month)&&IS_RTC_YEAR(Year)&&
     IS_RTC_HOUR24(Hour)&&IS_RTC_MINUTES(Min)&&IS_RTC_SECONDS(Sec))
  {
    /*##-1- Configure the Date #################################################*/
    /* Set Date: Tuesday February 18th 2014 */
    sdatestructure.Year = Year;
    sdatestructure.Month = Month;
    sdatestructure.Date = Day;
    //sdatestructure.WeekDay = RTC_WEEKDAY_SUNDAY;
    
    if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,FORMAT_BIN) != HAL_OK)
    {
      /* Initialization Error */
    } 
    
    /*##-2- Configure the Time #################################################*/
    /* Set Time: 02:00:00 */
    stimestructure.Hours = Hour;
    stimestructure.Minutes = Min;
    stimestructure.Seconds = Sec;
    //stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
    stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
    stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
    
    if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,FORMAT_BIN) != HAL_OK)
    {
      /* Initialization Error */
    }
    
    /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
    HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR0,0x32F2);
    
    HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR1,0);
    
    HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR2,0);
    
    return 1;
  }
  else
  {
    return 0;
  };
}

/**
  * @brief : SET Calendar extend
  * @param : RTC_DateTypeDef and RTC_TimeTypeDef
  * @retval : none
  */
uint8_t RTC_CalendarConfigExFull(uint8_t Day,uint8_t Month,uint8_t Year,uint8_t Hour,uint8_t Min,uint8_t Sec, uint8_t TimeZone, uint8_t DayLightTime)
{
  static RTC_DateTypeDef sdatestructure;
  static RTC_TimeTypeDef stimestructure; 

  /* Checking data input */
  if(IS_RTC_DATE(Day)&&IS_RTC_MONTH(Month)&&IS_RTC_YEAR(Year)&&
     IS_RTC_HOUR24(Hour)&&IS_RTC_MINUTES(Min)&&IS_RTC_SECONDS(Sec))
  {
    /*##-1- Configure the Date #################################################*/
    /* Set Date: Tuesday February 18th 2014 */
    sdatestructure.Year = Year;
    sdatestructure.Month = Month;
    sdatestructure.Date = Day;
    //sdatestructure.WeekDay = RTC_WEEKDAY_SUNDAY;
    
    if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,FORMAT_BIN) != HAL_OK)
    {
      /* Initialization Error */
    } 
    
    /*##-2- Configure the Time #################################################*/
    /* Set Time: 02:00:00 */
    stimestructure.Hours = Hour;
    stimestructure.Minutes = Min;
    stimestructure.Seconds = Sec;
    //stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
    stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
    stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
    
    if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,FORMAT_BIN) != HAL_OK)
    {
      /* Initialization Error */
    }
    
    /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
    HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR0,0x32F2);
    
    HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR1,TimeZone);
    
    HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR2,DayLightTime);
    return 1;
  }
  else
  {
    return 0;
  };
}


Time_TypeDef RTC_GetCurrentTime(void)
{
  RTC_TimeTypeDef Time;
  RTC_DateTypeDef Date;
  Time_TypeDef cTime;
  /* Update time */
  HAL_RTC_GetTime(&RtcHandle, &Time, FORMAT_BIN);
  HAL_RTC_GetDate(&RtcHandle, &Date, FORMAT_BIN);
  cTime.Date=Date.Date;
  cTime.Month=Date.Month;
  cTime.Year=Date.Year+2000;
  
  cTime.Hour=Time.Hours;
  cTime.Min=Time.Minutes;
  cTime.Sec=Time.Seconds;
  
  cTime.TimeZone=HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1);
  cTime.DayLight=HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR2);
  return cTime;
}


uint32_t DateTimetoLong(Time_TypeDef CurrentTime)
{
	static const uint8_t MONTH[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	static const uint8_t MONTH_EX[12]={31,29,31,30,31,30,31,31,30,31,30,31};
	
	uint32_t ui32Time=0;

	uint32_t tmp1=0, tmp2=0, tmp3=0;
	
	if (CurrentTime.Year < 1970)
	{
		CurrentTime.Year = 1970;
	}
	
	if (!((CurrentTime.Month >= 1) && (CurrentTime.Month <= 12)))
	{
		CurrentTime.Month = 1; 
	}

	if (!((CurrentTime.Date >= 1) && (CurrentTime.Date <= 31)))
	{
		CurrentTime.Date = 1; 
	}

	if (CurrentTime.Hour >23)
	{
		CurrentTime.Hour = 0; 
	}

	if (CurrentTime.Min >59)
	{
		CurrentTime.Min = 0; 
	}

	if (CurrentTime.Sec >59)
	{
		CurrentTime.Sec = 0; 
	}
	
	tmp1 = CurrentTime.Year - 1970;
	tmp2 = (tmp1 / 4) * (365 * 3 + 366) * 24 * 3600;
	tmp1 %= 4;
	switch (tmp1)
	{	
		case 0:
			ui32Time = tmp2;
		break;
		case 1:
			ui32Time = tmp2 + 365 * 24 * 3600;
		break;
		case 2:
			ui32Time = tmp2 + 2 * 365 * 24 * 3600;
		break;
		case 3:
			ui32Time = tmp2 + (365 * 2 + 366) * 24 * 3600;
		break;
		default:
		break;
	}
	
	if (tmp1 == 2)
	{
		tmp3=0;
    for (tmp2 = 0; tmp2 < (CurrentTime.Month - 1); tmp2++)
		{
			tmp3 += MONTH_EX[tmp2];
		}
	}
	else
	{
		tmp3=0;
    for (tmp2 = 0; tmp2 < (CurrentTime.Month - 1); tmp2++)
		{
			tmp3 += MONTH[tmp2];
		}		
	}
	
	ui32Time += (((24 * (tmp3 + CurrentTime.Date)) +  CurrentTime.Hour) * 3600) + (CurrentTime.Min * 60) + CurrentTime.Sec;
	
	return ui32Time;
}

/**
  * @brief : Convert long to date time
  * @param :   
  * @retval : 
  */
Time_TypeDef LongToDateTime(uint32_t u32_data)
{
  static const uint8_t MONTH[12]={31,28,31,30,31,30,31,31,30,31,30,31};
  static const uint8_t MONTH_EX[12]={31,29,31,30,31,30,31,31,30,31,30,31};
  Time_TypeDef Time; 
  uint32_t temp1=0,temp2=0;
  uint32_t sum=0;
  uint8_t i=0;

  Time.Sec=u32_data%60;
  Time.Min=(u32_data/60)%60;
  Time.Hour=(u32_data/3600)%24;
  
  temp1=(u32_data/((3600*24*(365*3+366))));
  temp1*=4;
  temp1+=1970;
  temp2=(u32_data%((3600*24*(365*3+366))))/(24*3600);
  if(temp2<365)
  {
    sum=0;
    for(i=0;i<11;i++)
    {
      sum+=MONTH[i];
      if(temp2<sum)
      {
        sum-=MONTH[i];
        Time.Date=temp2-sum+1;
        break;
      }
    }
    Time.Month=i+1;
  }
  else if((temp2>=365)&&(temp2<(365*2)))
  {
    temp2-=365;
    temp1++;
    sum=0;
    for(i=0;i<11;i++)
    {
      sum+=MONTH[i];
      if(temp2<sum)
      {
        sum-=MONTH[i];
        Time.Date=temp2-sum+1;
        break;
      }
    }
    Time.Month=i+1;
  }
  else if(temp2>=(365*2)&&(temp2<(365*2+366)))
  {
    temp2-=(365*2);
    temp1+=2;
    sum=0;
    for(i=0;i<11;i++)
    {
      sum+=MONTH_EX[i];
      if(temp2<sum)
      {
        sum-=MONTH_EX[i];
        Time.Date=temp2-sum+1;
        break;
      }
    }
    Time.Month=i+1;
  }
  else
  {
    temp2-=(365*2+366);
    temp1+=3;
    sum=0;
    for(i=0;i<11;i++)
    {
      sum+=MONTH[i];
      if(temp2<sum)
      {
        sum-=MONTH[i];
        Time.Date=temp2-sum+1;
        break;
      }
    }
    Time.Month=i+1;
  };
  
  Time.Year=temp1;
  
  return Time; 
}

/**
  * @}
  */ 


/** @defgroup Private_Functions
  * @{
  */ 
                                 
    
/******************* (C) COPYRIGHT 2014 ACIS *****END OF FILE****/
