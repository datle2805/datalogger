/**
  ******************************************************************************
  * @file    :common.c
  * @author  :DatLe
  * @version :v1_00
  * @date    :21/11/2015
  * @brief   :common function
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
#include "common.h"

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
/*------------------------------------------*Utilities*------------------------------------------*/
/* itoa:  convert n to characters in s */
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse_string(s);
}

/* reverse:  reverse string s in place */
void reverse_string(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/**
  * @brief : Reverse bit
  * @param : uint32_t x: input data  
  * @retval : reverse bit from x
  */
uint32_t reverse(uint32_t x)
{
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
    return((x >> 16) | (x << 16));
} 


/**
  * @brief : Load MCU Unique ID
  * @param : none  
  * @retval : u32 unique ID
  */
uint32_t u32_ReadUniqueID(void)
{
  #define U_ID          ((uint32_t *)0x1FFF7A10)
  
  static uint32_t u32_data[3];
  memset((uint8_t *)u32_data,0,3*4);
  
  u32_data[0]=(*(__IO uint32_t*)(U_ID));
  u32_data[1]=(*(__IO uint32_t*)(U_ID+4*1));
  u32_data[2]=(*(__IO uint32_t*)(U_ID+4*2));
  
  return u32_data[0]+u32_data[1]+u32_data[2];
}

uint32_t u32_ChecksumGen(uint8_t *pData,uint32_t u32_len)
{
  uint32_t i;
  uint32_t sum;
  sum=0;
  
  for(i=0;i<u32_len;i++)
  {
    sum+=*(((uint8_t *)pData)+i);
  }
  return sum;
}

void selectionSort(uint32_t *array,int length)//selection sort function
{
  int i,j,minat;
  uint32_t min;
  for(i=0;i<(length-1);i++)
  {
    minat=i;
    min=array[i];

    for(j=i+1;j<(length);j++) //select the min of the rest of array
    {
      if(min>array[j])   //ascending order for descending reverse
      {
        minat=j;  //the position of the min element
        min=array[j];
      }
    }
    int temp=array[i] ;
    array[i]=array[minat];  //swap
    array[minat]=temp;
  }
}

void selectionSortScript(ScriptMost_TypeDef *array,int length)
{
  int i,j,minat;
  ScriptMost_TypeDef min,temp;
  for(i=0;i<(length-1);i++)
  {
    minat=i;
    min.Cnt=array[i].Cnt;

    for(j=i+1;j<(length);j++) //select the min of the rest of array
    {
      if(min.Cnt>array[j].Cnt)   //ascending order for descending reverse
      {
        minat=j;  //the position of the min element
        min=array[j];
      }
    }
    temp=array[i] ;
    array[i]=array[minat];  //swap
    array[minat]=temp;
  }
}
uint8_t BCDConvert(char data)
{
  if((data>='0')&&(data<='9'))
  {
    return data-'0';
  }
  else if((data>='A')&&(data<='F'))
  {
    return data-'A'+0x0A;
  }
  else if((data>='a')&&(data<='f'))
  {
    return data-'a'+0x0A;
  }
  else
  {
    return 0;
  }
}

void U8ToBCD(uint8_t Num, char *Data)
{
  Num8 n8;
  
  n8.n8=Num;
  
  if(n8.n4.bcd0<0x0A)
  {
    *(Data+0)=n8.n4.bcd0+0x30;
  }
  else 
  {
    *(Data+0)=n8.n4.bcd0-0x0A+'A';
  }
  
  if(n8.n4.bcd1<0x0A)
  {
    *(Data+1)=n8.n4.bcd1+0x30;
  }
  else 
  {
    *(Data+1)=n8.n4.bcd1-0x0A+'A';
  }
}
/* Some of buffer */
void BUFUI8_Store(BUFUI8_TypeDef *Buf,uint8_t Data)
{
  if(!((Buf->Spos+1==Buf->Rpos)||((Buf->Spos+1==BUFFUI8_DATA_MAX)&&(!Buf->Rpos))))
  {
    /* Copy data to buffer */
    Buf->Data[Buf->Spos]=Data;    
    Buf->Spos++;
    if(Buf->Spos>(BUFFUI8_DATA_MAX-1)){Buf->Spos=0;}; 
  }
}

uint8_t BUFUI8_Get(BUFUI8_TypeDef *Buf)
{
  uint8_t Tmp;
  if(Buf->Rpos!=Buf->Spos)
  {
    Tmp=Buf->Data[Buf->Rpos];
    Buf->Rpos++;
  }
  else
  {
    Tmp=0;
  };
  if(Buf->Rpos>(BUFFUI8_DATA_MAX-1)){Buf->Rpos=0;};
  return Tmp;
}

uint8_t BUFUI8_Check(BUFUI8_TypeDef *Buf)
{
  if(Buf->Rpos==Buf->Spos)
  {
    return false;
  }
  else 
  {return true;};
}

void BUFUI8_Clear(BUFUI8_TypeDef *Buf)
{
  Buf->Rpos=0;
  Buf->Spos=0;
}


/*----------------------------------------*End Utilities*----------------------------------------*/
                                 
    
/******************* (C) COPYRIGHT 2014 ACIS *****END OF FILE****/
