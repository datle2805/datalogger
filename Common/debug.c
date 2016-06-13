/**
  ******************************************************************************
  * @file    :debug.c
  * @author  :DatLe
  * @version :
  * @date    :
  * @brief   :
  *          This file should be added to the main application to use the provided
  *          functions that manage Leds, push-buttons, COM ports and low level 
  *          HW resources initialization of the different modules available on
  *          STM32 evaluation boards from STMicroelectronics.
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "debug.h"
#include "common.h"
#include "rtc.h"
#include "timeout.h"
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
#ifdef __USING_UART_DEBUG
UART_HandleTypeDef UartDebugHandle;
UART_DBG_DrvTypeDef *DBG;
#endif

#ifdef __USER_USE_LOG_
uint8_t PutCharTarget=0;
#endif
#ifdef __USING_UART_DEBUG
  static UART_DBG_RxBuffer Rx;
  static uint16_t RxCnt=0;
  static UART_DBG_DataTypeDef RxCache;
  static uint8_t Data; /* UART_DBG Data Get */
  static TO_TypeDef Calibre;
  static UART_DBG_DataTypeDef *RxData;

  static TO_TypeDef TxTO;

  static bool DataOK;
  
  static void             StoreRx(UART_DBG_DataTypeDef* Data);
  static UART_DBG_DataTypeDef* GetRx(void);
  static bool             CheckRx(void);
  static void             ClearRx(void);

  static void InputTask(void);
  static UART_DBG_DataTypeDef* CurrentRxData(void);
  static void ISRRx(void);
  static void Calibre_Task(void);
  static uint8_t CheckDataOK(void);
  static void ClearDataOK(void);

  UART_DBG_DrvTypeDef UART_DBG_Drv=
  {
    /* Interface */
    InputTask, /* Input Task */
    Calibre_Task,
    /* ---------------------------------------- */
    ClearRx,

    ISRRx,
    /* Config */
    CheckDataOK,/* Check Data is OK, pass check sum and ID */
    ClearDataOK,
    CurrentRxData,
  };
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
#ifdef __USING_DEBUG
//struct __FILE { int handle; /* Add whatever is needed */ };
//FILE __stdout;
//FILE __stdin;

//int fputc(int ch, FILE *f) {
//  if (DEMCR & TRCENA) {
//    while (ITM_Port32(0) == 0);
//    ITM_Port8(0) = ch;
//  }
//  return(ch);
//}
#endif

#ifdef __USING_UART_DEBUG
/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param huart: UART handle pointer
  * @retval None
  */
void UART_DEBUG_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  UART_DBG_TX_GPIO_CLK_ENABLE();
  UART_DBG_RX_GPIO_CLK_ENABLE();
  /* Enable USART1 clock */
  UART_DBG_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = UART_DBG_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = UART_DBG_TX_AF;
  
  HAL_GPIO_Init(UART_DBG_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = UART_DBG_RX_PIN;
  GPIO_InitStruct.Alternate = UART_DBG_RX_AF;
    
  HAL_GPIO_Init(UART_DBG_RX_GPIO_PORT, &GPIO_InitStruct);
  
  
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = ODD parity
      - BaudRate = 19200 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartDebugHandle.Instance          = UART_DBG;
  
  UartDebugHandle.Init.BaudRate     = UART_DBG_SPEED;
  UartDebugHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartDebugHandle.Init.StopBits     = UART_STOPBITS_1;
  UartDebugHandle.Init.Parity       = UART_PARITY_NONE;
  UartDebugHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartDebugHandle.Init.Mode         = UART_MODE_TX_RX;
  UartDebugHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    
  if(HAL_UART_Init(&UartDebugHandle) != HAL_OK)
  {
    /* Initialization Error */ 
  }
  
  /* NVIC for USART1 */
  HAL_NVIC_SetPriority(UART_DBG_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(UART_DBG_IRQn); 

  /* For next time getting data */
  HAL_UART_Receive_IT(&UartDebugHandle, &Data, 1); 
  
  DBG=&UART_DBG_Drv; 
  
  /* Output a message on Hyperterminal using printf function */
  TARGET_Set(TARGET_UART);
  printf(">>>>>>>>>>>>>UART debug for STM32F7 discovery kit <<<<<<<<<<<<<<\r\n");
}

void UART_DBG_Print(char *data)
{
  static uint32_t Cnt=0;
  static char tmp[200];
  static char tmp1[20];
  Time_TypeDef CurrentTime;
  
  memset(tmp,0,100);
  memset(tmp1,0,20);
  Cnt++;
  itoa(Cnt,tmp1);
  strcpy(tmp,">");
  strcat(tmp,tmp1);
  strcat(tmp,"| ");
  CurrentTime=RTC_GetCurrentTime();
  itoa(CurrentTime.Hour,tmp1);
  strcat(tmp,tmp1);
  strcat(tmp,": ");
  itoa(CurrentTime.Min,tmp1);
  strcat(tmp,tmp1);
  strcat(tmp,": ");
  itoa(CurrentTime.Sec,tmp1);
  strcat(tmp,tmp1);
  strcat(tmp,": ");
  strcat(tmp,data);
  strcat(tmp,"\r\n");
  
  TARGET_Set(TARGET_UART);
  printf("%s",tmp);
}

/**
  * @brief : Input task handle of UART_DBG1 module
  * @param : none  
  * @retval : none
  */
static void InputTask(void)
{
  if(CheckRx())/* If get data */
  {
    RxData=GetRx();
    DataOK=1;
  }
}
/**
  * @brief : Get current RX data received in cache
  * @param : none  
  * @retval : RX data received
  */
static UART_DBG_DataTypeDef* CurrentRxData(void)
{
  return RxData;
}

static void ISRRx(void)
{
  /* For next time getting data */
  RxCache.Data[RxCnt]=Data;
  RxCnt++;
  RxCache.Len=RxCnt;
  TO_Start(&Calibre,UART_DBG_RX_CALIBRE_TIME); 
  if(RxCnt>(UART_DBG_DATA_LEN-2))
  {
    /* Store to buffer */
    StoreRx(&RxCache);
    memset((uint8_t *)&RxCache,0,sizeof(RxCache));
    RxCnt=0;
    TO_Stop(&Calibre);
  }
  /* TO start for re-calibre data counter */
  HAL_UART_Receive_IT(&UartDebugHandle, &Data, 1);
} 

/**
  * @brief : Calibre task, if no RX signal get in ... time, auto calibre rx counter
  * @param : none  
  * @retval : none
  */
static void Calibre_Task(void)
{
  static uint32_t Cnt=0;
  if(TO_ReadStatus(&Calibre))
  {
    TO_ClearStatus(&Calibre);
    /* Reset counter */
    RxCache.Len=RxCnt;
    RxCnt=0;
    /* Store to buffer */
    StoreRx(&RxCache);
    memset((uint8_t *)&RxCache,0,sizeof(RxCache));
  }
  TO_Task(&Calibre);
}
static uint8_t CheckDataOK(void)
{
  return DataOK;
}
static void ClearDataOK(void)
{
  DataOK=0;
}

static void StoreRx(UART_DBG_DataTypeDef* Data)
{
  if(!((Rx.Spos+1==Rx.Rpos)||((Rx.Spos+1==UART_DBG_MAX_BUF_RX)&&(!Rx.Rpos))))
  {
    /* Copy data to buffer */
    memcpy((uint8_t *)&Rx.Data[Rx.Spos],(uint8_t *)Data,sizeof(Rx.Data[Rx.Spos]));    
    Rx.Spos++;
    if(Rx.Spos>UART_DBG_MAX_BUF_RX-1){Rx.Spos=0;}; 
  }  
}
static UART_DBG_DataTypeDef* GetRx(void)
{
  static UART_DBG_DataTypeDef* Tmp;
  if(Rx.Rpos!=Rx.Spos)
  {
    Tmp=&Rx.Data[Rx.Rpos];
    Rx.Rpos++;
  }
  else
  {
    Tmp=0;
  };
  if(Rx.Rpos>UART_DBG_MAX_BUF_RX-1){Rx.Rpos=0;};
  return Tmp;
}

static bool CheckRx(void)
{
  if(Rx.Rpos==Rx.Spos)
  {
    return false;
  }
  else 
  {return true;};
}
static void ClearRx(void)
{
  uint16_t j;
  Rx.Spos=0;
  Rx.Rpos=0;
  for(j=0;j<UART_DBG_MAX_BUF_RX;j++)
  {
    /* Memset */
    memset((uint8_t *)&Rx.Data[j],0,UART_DBG_DATA_LEN);
  }
}

/* callback */
void TARGET_UART_Put(char data)
{
  HAL_UART_Transmit(&UartDebugHandle, (uint8_t *)&data, 1, 0x00FF); 
}
#endif
    
/******************* (C) COPYRIGHT 2015 ACIS *****END OF FILE****/
