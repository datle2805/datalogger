/**
  ******************************************************************************
  * @file    lcd_log.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-February-2014
  * @brief   This file provides all the USER_LCD Log firmware functions.
  *          
  *          The USER_LCD Log module allows to automatically set a header and footer
  *          on any application using the USER_LCD display and allows to dump user,
  *          debug and error messages by using the following macros: USER_LCD_ErrLog(),
  *          USER_LCD_UsrLog() and USER_LCD_DbgLog().
  *         
  *          It supports also the scroll feature by embedding an internal software
  *          cache for display. This feature allows to dump message sequentially
  *          on the display even if the number of displayed lines is bigger than
  *          the total number of line allowed by the display.
  *      
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include  <stdio.h>
#include  "userlog.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
* @{
*/

/** @addtogroup Common
  * @{
  */

/** @defgroup USER_LCD_LOG 
* @brief USER_LCD Log USER_LCD_Application module
* @{
*/ 

/** @defgroup USER_LCD_LOG_Private_Types
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USER_LCD_LOG_Private_Defines
* @{
*/ 

/**
* @}
*/ 

/* Define the display window settings */
#define     USER_YWINDOW_MIN         0

/** @defgroup USER_LCD_LOG_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USER_LCD_LOG_Private_Variables
* @{
*/ 

static uint32_t X0,Y0,XSIZE;

USER_LCD_LOG_line USER_LCD_CacheBuffer [USER_LCD_CACHE_DEPTH]; 
uint32_t USER_LCD_LineColor;
uint32_t USER_LCD_LineColorBk;
uint16_t USER_LCD_CacheBuffer_xptr;
uint16_t USER_LCD_CacheBuffer_yptr_top;
uint16_t USER_LCD_CacheBuffer_yptr_bottom;

uint16_t USER_LCD_CacheBuffer_yptr_top_bak;
uint16_t USER_LCD_CacheBuffer_yptr_bottom_bak;

FunctionalState USER_LCD_CacheBuffer_yptr_invert;
FunctionalState USER_LCD_ScrollActive;
FunctionalState USER_LCD_Lock;
FunctionalState USER_LCD_Scrolled;
uint16_t USER_LCD_ScrollBackStep;

GUI_MEMDEV_Handle backgroundDisplayMem;

static uint8_t u8_enable_display=0;

/**
* @}
*/ 


/** @defgroup USER_LCD_LOG_Private_FunctionPrototypes
* @{
*/ 

/**
* @}
*/ 


/** @defgroup USER_LCD_LOG_Private_Functions
* @{
*/ 


/**
  * @brief  Initializes the USER_LCD Log module 
  * @param  None
  * @retval None
  */

void USER_LCD_LOG_Init (uint16_t x0,uint16_t y0,uint16_t xSize,uint16_t ySize)
{
  /* Deinit USER_LCD cache */
  USER_LCD_LOG_DeInit();
  /* Clear the USER_LCD */
  USER_LCD_LOG_SetTestArea(x0,y0,xSize,ySize);
  X0=x0;
  Y0=y0;
  XSIZE=xSize;  
  
  USER_LCD_LOG_SetDisplayEnable(1);
}

/**
  * @brief DeInitializes the USER_LCD Log module. 
  * @param  None
  * @retval None
  */
void USER_LCD_LOG_DeInit(void)
{
  USER_LCD_LineColor = USER_LCD_LOG_TEXT_COLOR;
  USER_LCD_CacheBuffer_xptr = 0;
  USER_LCD_CacheBuffer_yptr_top = 0;
  USER_LCD_CacheBuffer_yptr_bottom = 0;
  
  USER_LCD_CacheBuffer_yptr_top_bak = 0;
  USER_LCD_CacheBuffer_yptr_bottom_bak = 0;
  
  USER_LCD_CacheBuffer_yptr_invert= ENABLE;
  USER_LCD_ScrollActive = DISABLE;
  USER_LCD_Lock = DISABLE;
  USER_LCD_Scrolled = DISABLE;
  USER_LCD_ScrollBackStep = 0;
}


/**
  * @brief  Display the application header on the USER_LCD screen 
  * @param  header: pointer to the string to be displayed
  * @retval None
  */
void USER_LCD_LOG_SetTestArea (uint16_t x0,uint16_t y0,uint16_t xSize,uint16_t ySize)
{
  GUI_SetFont(&USER_LCD_LOG_TEXT_FONT);
  GUI_SetColor(USER_LCD_LOG_BACKGROUND_COLOR);
  GUI_FillRect(x0, y0-GUI_GetFontSizeY(), x0+xSize, y0+ySize);
  GUI_SetColor(USER_LCD_LOG_BACKGROUND_COLOR_EX);
  GUI_DrawRect(x0, y0-GUI_GetFontSizeY(), x0+xSize, y0+ySize);
}
/**
  * @brief  Clear the Text Zone 
  * @param  None 
  * @retval None
  */
void USER_LCD_LOG_ClearTextZone(void)
{
  uint8_t i=0;
  
  for (i= 0 ; i < USER_YWINDOW_SIZE; i++)
  {
    USER_LCD_LOG_ClearStringLine(i + USER_YWINDOW_MIN);
  }
  
  USER_LCD_LOG_DeInit();
}

/**
  * @brief  Redirect the printf to the USER_LCD
  * @param  c: character to be displayed
  * @param  f: output file pointer
  * @retval None
 */
//USER_LCD_LOG_PUTCHAR
void LCD_PutChar (char ch)
{
  uint32_t idx;
  if(USER_LCD_Lock == DISABLE)
  {
    if(USER_LCD_ScrollActive == ENABLE)
    {
      USER_LCD_CacheBuffer_yptr_bottom = USER_LCD_CacheBuffer_yptr_bottom_bak;
      USER_LCD_CacheBuffer_yptr_top    = USER_LCD_CacheBuffer_yptr_top_bak;
      USER_LCD_ScrollActive = DISABLE;
      USER_LCD_Scrolled = DISABLE;
      USER_LCD_ScrollBackStep = 0;
      
    }
    
    if(( USER_LCD_CacheBuffer[USER_LCD_CacheBuffer_yptr_bottom].len < XSIZE) &&  ( ch != '\n'))
    {
      USER_LCD_CacheBuffer[USER_LCD_CacheBuffer_yptr_bottom].line[USER_LCD_CacheBuffer_xptr++] = (uint16_t)ch;
      USER_LCD_CacheBuffer[USER_LCD_CacheBuffer_yptr_bottom].len+=GUI_GetCharDistX(ch);
    }   
    else 
    {
      if(USER_LCD_CacheBuffer_yptr_top >= USER_LCD_CacheBuffer_yptr_bottom)
      {
        
        if(USER_LCD_CacheBuffer_yptr_invert == DISABLE)
        {
          USER_LCD_CacheBuffer_yptr_top++;
          
          if(USER_LCD_CacheBuffer_yptr_top == USER_LCD_CACHE_DEPTH)
          {
            USER_LCD_CacheBuffer_yptr_top = 0;  
          }
        }
        else
        {
          USER_LCD_CacheBuffer_yptr_invert= DISABLE;
        }
      }
      
      for(idx = USER_LCD_CacheBuffer_xptr ; idx < sizeof(USER_LCD_CacheBuffer[USER_LCD_CacheBuffer_yptr_bottom].line)/sizeof(USER_LCD_CacheBuffer[USER_LCD_CacheBuffer_yptr_bottom].line[0]); idx++)
      {
        USER_LCD_CacheBuffer[USER_LCD_CacheBuffer_yptr_bottom].line[USER_LCD_CacheBuffer_xptr++] = 0;
      }   
      USER_LCD_CacheBuffer[USER_LCD_CacheBuffer_yptr_bottom].color = USER_LCD_LineColor;  
      
      USER_LCD_CacheBuffer[USER_LCD_CacheBuffer_yptr_bottom].len=0;
      USER_LCD_CacheBuffer_xptr = 0;
      
      USER_LCD_LOG_UpdateDisplay (); 
      
      USER_LCD_CacheBuffer_yptr_bottom ++; 
      
      if (USER_LCD_CacheBuffer_yptr_bottom == USER_LCD_CACHE_DEPTH) 
      {
        USER_LCD_CacheBuffer_yptr_bottom = 0;
        USER_LCD_CacheBuffer_yptr_top = 1;    
        USER_LCD_CacheBuffer_yptr_invert = ENABLE;
      }
      
      if( ch != '\n')
      {
        USER_LCD_CacheBuffer[USER_LCD_CacheBuffer_yptr_bottom].line[USER_LCD_CacheBuffer_xptr++] = (uint16_t)ch;
      }
    }
  }
  //return ch;
}
  
/**
  * @brief  Update the text area display
  * @param  None
  * @retval None
  */
void USER_LCD_LOG_UpdateDisplay (void)
{
  #ifndef __USER_USE_SOLID_BK_
  GUI_MEMDEV_Handle tempMem;    
  void *pbk;
  void *ptemp; 
  #endif
  uint8_t cnt = 0 ;
  uint16_t length = 0 ;
  uint16_t ptr = 0, index = 0;
  if((USER_LCD_CacheBuffer_yptr_bottom  < (USER_YWINDOW_SIZE -1)) && 
     (USER_LCD_CacheBuffer_yptr_bottom  >= USER_LCD_CacheBuffer_yptr_top))
  {
    if(u8_enable_display)
    {
      USER_LCD_LOG_DisplayStringAtLine ((USER_YWINDOW_MIN + Y0/GUI_GetFontSizeY()+ USER_LCD_CacheBuffer_yptr_bottom),
                           (char *)(USER_LCD_CacheBuffer[cnt + USER_LCD_CacheBuffer_yptr_bottom].line),
                             USER_LCD_CacheBuffer[cnt + USER_LCD_CacheBuffer_yptr_bottom].color);
    }
  }
  else
  {
    if(USER_LCD_CacheBuffer_yptr_bottom < USER_LCD_CacheBuffer_yptr_top)
    {
      /* Virtual length for rolling */
      length = USER_LCD_CACHE_DEPTH + USER_LCD_CacheBuffer_yptr_bottom ;
    }
    else
    {
      length = USER_LCD_CacheBuffer_yptr_bottom;
    }
    
    ptr = length - USER_YWINDOW_SIZE + 1;

    GUI_SetFont(&USER_LCD_LOG_TEXT_FONT);
    #ifdef __USER_USE_SOLID_BK_ 
    for  (cnt = 0 ; cnt < USER_YWINDOW_SIZE ; cnt ++)
      {        
        index = (cnt + ptr )% USER_LCD_CACHE_DEPTH ;
        if(u8_enable_display)
        {
          USER_LCD_LOG_DisplayStringAtLine ((cnt + Y0/(GUI_GetFontSizeY()) + USER_YWINDOW_MIN), 
                               (char *)(USER_LCD_CacheBuffer[index].line),
                                 USER_LCD_CacheBuffer[index].color); 
        }                       
      }
    #else 
    tempMem=GUI_MEMDEV_Create(GUI_MEMDEV_GetXPos(backgroundDisplayMem),GUI_MEMDEV_GetYPos(backgroundDisplayMem),GUI_MEMDEV_GetXSize(backgroundDisplayMem),GUI_MEMDEV_GetYSize(backgroundDisplayMem));
    if(tempMem)
    {
      pbk=GUI_MEMDEV_GetDataPtr(backgroundDisplayMem);
      ptemp=GUI_MEMDEV_GetDataPtr(tempMem);
    
      memcpy((uint8_t *)ptemp,(uint8_t *)pbk,4*GUI_MEMDEV_GetXSize(backgroundDisplayMem)*GUI_MEMDEV_GetYSize(backgroundDisplayMem));

      GUI_MEMDEV_Select(backgroundDisplayMem);
      
      for  (cnt = 0 ; cnt < USER_YWINDOW_SIZE ; cnt ++)
      {        
        index = (cnt + ptr )% USER_LCD_CACHE_DEPTH ;
        if(u8_enable_display)
        {
          USER_LCD_LOG_DisplayStringAtLine ((cnt + Y0/(GUI_GetFontSizeY()) + USER_YWINDOW_MIN), 
                               (char *)(USER_LCD_CacheBuffer[index].line),
                                 USER_LCD_CacheBuffer[index].color); 
        }                       
      }
      
      GUI_MEMDEV_CopyToLCD(backgroundDisplayMem);
      GUI_MEMDEV_Select(0);
      
      memcpy((uint8_t *)pbk,(uint8_t *)ptemp,4*GUI_MEMDEV_GetXSize(backgroundDisplayMem)*GUI_MEMDEV_GetYSize(backgroundDisplayMem));
      GUI_MEMDEV_Delete(tempMem);
      tempMem=0;
    }
    #endif
      
  }
  
}

#if( USER_LCD_SCROLL_ENABLED == 1)
/**
  * @brief  Display previous text frame
  * @param  None
  * @retval Status
  */
ErrorStatus USER_LCD_LOG_ScrollBack(void)
{
    
  if(USER_LCD_ScrollActive == DISABLE)
  {
    
    USER_LCD_CacheBuffer_yptr_bottom_bak = USER_LCD_CacheBuffer_yptr_bottom;
    USER_LCD_CacheBuffer_yptr_top_bak    = USER_LCD_CacheBuffer_yptr_top;
    
    
    if(USER_LCD_CacheBuffer_yptr_bottom > USER_LCD_CacheBuffer_yptr_top) 
    {
      
      if ((USER_LCD_CacheBuffer_yptr_bottom - USER_LCD_CacheBuffer_yptr_top) <=  USER_YWINDOW_SIZE)
      {
        USER_LCD_Lock = DISABLE;
        return ERROR;
      }
    }
    USER_LCD_ScrollActive = ENABLE;
    
    if((USER_LCD_CacheBuffer_yptr_bottom  > USER_LCD_CacheBuffer_yptr_top)&&
       (USER_LCD_Scrolled == DISABLE ))
    {
      USER_LCD_CacheBuffer_yptr_bottom--;
      USER_LCD_Scrolled = ENABLE;
    }
  }
  
  if(USER_LCD_ScrollActive == ENABLE)
  {
    USER_LCD_Lock = ENABLE;
    
    if(USER_LCD_CacheBuffer_yptr_bottom > USER_LCD_CacheBuffer_yptr_top) 
    {
      
      if((USER_LCD_CacheBuffer_yptr_bottom  - USER_LCD_CacheBuffer_yptr_top) <  USER_YWINDOW_SIZE )
      {
        USER_LCD_Lock = DISABLE;
        return ERROR;
      }
      
      USER_LCD_CacheBuffer_yptr_bottom --;
    }
    else if(USER_LCD_CacheBuffer_yptr_bottom <= USER_LCD_CacheBuffer_yptr_top)
    {
      
      if((USER_LCD_CACHE_DEPTH  - USER_LCD_CacheBuffer_yptr_top + USER_LCD_CacheBuffer_yptr_bottom) < USER_YWINDOW_SIZE)
      {
        USER_LCD_Lock = DISABLE;
        return ERROR;
      }
      USER_LCD_CacheBuffer_yptr_bottom --;
      
      if(USER_LCD_CacheBuffer_yptr_bottom == 0xFFFF)
      {
        USER_LCD_CacheBuffer_yptr_bottom = USER_LCD_CACHE_DEPTH - 2;
      }
    }
    USER_LCD_ScrollBackStep++;
    USER_LCD_LOG_UpdateDisplay();
    USER_LCD_Lock = DISABLE;
  }
  return SUCCESS;
}

/**
  * @brief  Display next text frame
  * @param  None
  * @retval Status
  */
ErrorStatus USER_LCD_LOG_ScrollForward(void)
{
  
  if(USER_LCD_ScrollBackStep != 0)
  {
    if(USER_LCD_ScrollActive == DISABLE)
    {
      
      USER_LCD_CacheBuffer_yptr_bottom_bak = USER_LCD_CacheBuffer_yptr_bottom;
      USER_LCD_CacheBuffer_yptr_top_bak    = USER_LCD_CacheBuffer_yptr_top;
      
      if(USER_LCD_CacheBuffer_yptr_bottom > USER_LCD_CacheBuffer_yptr_top) 
      {
        
        if ((USER_LCD_CacheBuffer_yptr_bottom - USER_LCD_CacheBuffer_yptr_top) <=  USER_YWINDOW_SIZE)
        {
          USER_LCD_Lock = DISABLE;
          return ERROR;
        }
      }
      USER_LCD_ScrollActive = ENABLE;
      
      if((USER_LCD_CacheBuffer_yptr_bottom  > USER_LCD_CacheBuffer_yptr_top)&&
         (USER_LCD_Scrolled == DISABLE ))
      {
        USER_LCD_CacheBuffer_yptr_bottom--;
        USER_LCD_Scrolled = ENABLE;
      }
      
    }
    
    if(USER_LCD_ScrollActive == ENABLE)
    {
      USER_LCD_Lock = ENABLE;
      USER_LCD_ScrollBackStep--;
      
      if(++USER_LCD_CacheBuffer_yptr_bottom == USER_LCD_CACHE_DEPTH)
      {
        USER_LCD_CacheBuffer_yptr_bottom = 0;
      }
      
      USER_LCD_LOG_UpdateDisplay();
      USER_LCD_Lock = DISABLE;
      
    } 
    return SUCCESS;
  }
  else // USER_LCD_ScrollBackStep == 0 
  {
    USER_LCD_Lock = DISABLE;
    return ERROR;
  }  
}
#endif /* USER_LCD_SCROLL_ENABLED */

/**
  * @}
  */
/**
  * @brief  Displays a maximum of 60 char on the USER_LCD.
  * @param  X: pointer to x position (in pixel)
  * @param  Y: pointer to y position (in pixel)    
  * @param  pText: pointer to string to display on USER_LCD
  * @param  mode: The display mode
  *    This parameter can be one of the following values:
  *                @arg CENTER_MODE 
  *                @arg RIGHT_MODE
  *                @arg LEFT_MODE   
  * @retval None
  */
void USER_LCD_LOG_DisplayStringAt(uint16_t X, uint16_t Y, char *pText, USER_Text_AlignModeTypdef mode, GUI_COLOR color)
{
  
  /* Clear first */
  GUI_SetFont(&USER_LCD_LOG_TEXT_FONT);
  #ifdef __USER_USE_SOLID_BK_
  GUI_SetColor(USER_LCD_LOG_BACKGROUND_COLOR);
  GUI_FillRect(X,Y,X0+XSIZE,Y+GUI_GetFontSizeY());
  #endif
  GUI_SetColor(color);
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);
  switch (mode)
  {
  case USER_CENTER_MODE:
    {
      GUI_SetTextAlign(GUI_TA_HCENTER);
      //GUI_DispStringAtCEOL(pText,X,Y);
      GUI_DispStringAt(pText,X,Y);
      break;
    }
  case USER_LEFT_MODE:
    {
      GUI_SetTextAlign(GUI_TA_LEFT);
      //GUI_DispStringAtCEOL(pText,X,Y);
      GUI_DispStringAt(pText,X,Y);
      break;
    }
  case USER_RIGHT_MODE:
    {
      GUI_SetTextAlign(GUI_TA_RIGHT);
      //GUI_DispStringAtCEOL(pText,X,Y);
      GUI_DispStringAt(pText,X,Y);
      break;
    }
  default:
    {
      GUI_SetTextAlign(GUI_TA_LEFT);
      //GUI_DispStringAtCEOL(pText,X,Y);
      GUI_DispStringAt(pText,X,Y);
      break;
    }
  }
}

/**
  * @brief  Displays a maximum of 20 char on the USER_LCD.
  * @param  Line: the Line where to display the character shape
  * @param  ptr: pointer to string to display on USER_LCD
  * @retval None
  */
void USER_LCD_LOG_DisplayStringAtLine(uint16_t Line, char *ptr, GUI_COLOR color)
{
  USER_LCD_LOG_DisplayStringAt(X0, USER_LINE(Line), ptr, USER_LEFT_MODE, color);
}

/**
  * @brief  Clears the selected line.
  * @param  Line: the line to be cleared
  * @retval None
  */
void USER_LCD_LOG_ClearStringLine(uint32_t Line)
{
  GUI_SetColor(USER_LCD_LOG_BACKGROUND_COLOR);
  GUI_FillRect(X0,USER_LINE((Line-1)),X0+XSIZE,USER_LINE((Line)));
}


void USER_LCD_LOG_BackupColor(void)
{
  USER_LCD_LineColorBk=GUI_GetColor();
}
void USER_LCD_LOG_RestoreColor(void)
{
  GUI_SetColor(USER_LCD_LineColorBk);
}

void USER_LCD_LOG_SetDisplayEnable(uint8_t onOff)
{
  u8_enable_display=onOff;
}

void TARGET_LCD_Put(char data)
{
  LCD_PutChar(data);
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
