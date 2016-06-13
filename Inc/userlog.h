/**
  ******************************************************************************
  * @file    userlog.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-February-2014
  * @brief   header for the lcd_log.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __USER_LCD_LOG_EMWIN_H__
#define  __USER_LCD_LOG_EMWIN_H__

/* Includes ------------------------------------------------------------------*/
#include "GUI.h"
#include "stm32f7xx_hal.h"
#include "string.h"
#include "retarget.h"
#include <stdio.h>


/** @addtogroup Utilities
  * @{
  */
  /* Enable debug session */
  #define __USER_USE_LOG_
  #define __USER_USE_WARNING_LOG_
  #define __USER_USE_NOSTIFY_LOG_
  #define __USER_USE_SOLID_BK_

  /* Define the Fonts  */
  #define     USER_LCD_LOG_HEADER_FONT                   GUI_Font16_ASCII
  #define     USER_LCD_LOG_FOOTER_FONT                   GUI_Font13_ASCII
  #define     USER_LCD_LOG_TEXT_FONT                     GUI_Font13_ASCII


  /* Define the LCD LOG Color  */
  #define     USER_LCD_LOG_DEFAULT_COLOR                 GUI_BLACK
  #define     USER_LCD_LOG_BACKGROUND_COLOR              0xC2C2C2
  #define     USER_LCD_LOG_BACKGROUND_COLOR_EX           0xB1B1B1
  #define     USER_LCD_LOG_TEXT_COLOR                    GUI_GRAY

  #define     USER_LCD_LOG_SOLID_BACKGROUND_COLOR        GUI_BLUE
  #define     USER_LCD_LOG_SOLID_TEXT_COLOR              GUI_GREEN

  /* Define the cache depth */
  #define     USER_CACHE_SIZE              100
  #define     USER_YWINDOW_SIZE            20

  #if (USER_YWINDOW_SIZE > 25)
    #error "Wrong USER_YWINDOW SIZE"
  #endif


  /** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup Common
  * @{
  */

/** @addtogroup LCD_LOG
  * @{
  */
  
/** @defgroup LCD_LOG
  * @brief 
  * @{
  */ 


/** @defgroup LCD_LOG_Exported_Defines
  * @{
  */ 
#define USER_LCD_SCROLL_ENABLED        1
#if (USER_LCD_SCROLL_ENABLED == 1)
 #define     USER_LCD_CACHE_DEPTH     (USER_YWINDOW_SIZE + USER_CACHE_SIZE)
#else
 #define     USER_LCD_CACHE_DEPTH     USER_YWINDOW_SIZE
#endif


#define USER_MAX_LAYER_NUMBER          2
/**
  * @}
  */ 

/** @defgroup LCD_LOG_Exported_Types
  * @{
  */ 
typedef struct _USER_LCD_LOG_line
{
  uint8_t  line[480/5];
  uint16_t len;
  uint32_t color;

}USER_LCD_LOG_line;


typedef enum
{
  USER_CENTER_MODE             = 0x01,    /* center mode */
  USER_RIGHT_MODE              = 0x02,    /* right mode  */     
  USER_LEFT_MODE               = 0x03,    /* left mode   */                                                                               
}USER_Text_AlignModeTypdef;

typedef struct 
{ 
  uint32_t  USER_TextColor; 
  uint32_t  USER_BackColor;  
  GUI_FONT     *USER_pFont;
}USER_LCD_DrawPropTypeDef;
/**
  * @}
  */ 

/** @defgroup LCD_LOG_Exported_Macros
  * @{
  */
#ifdef __USER_USE_ERROR_LOG_
#define  USER_LCD_ErrLog(...)    	USER_LCD_LOG_BackupColor();\
									USER_LCD_LineColor=GUI_RED;\
									GUI_SetFont(&USER_LCD_LOG_TEXT_FONT);\
                  TARGET_Set(TARGET_LCD);\
									printf("ERROR: ") ;\
									printf(__VA_ARGS__);\
                  TARGET_Set(TARGET_UART);\
									USER_LCD_LOG_RestoreColor();
#else
#define  USER_LCD_ErrLog(...)    
#endif

#ifdef __USER_USE_LOG_
#define  USER_LCD_UsrLog(...)    	USER_LCD_LOG_BackupColor();\
									USER_LCD_LineColor=USER_LCD_LOG_TEXT_COLOR;\
									GUI_SetFont(&USER_LCD_LOG_TEXT_FONT);\
									TARGET_Set(TARGET_LCD);\
									printf(__VA_ARGS__);\
                  TARGET_Set(TARGET_UART);\
									USER_LCD_LOG_RestoreColor();
#else
#define  USER_LCD_UsrLog(...)    
#endif


#ifdef __USER_USE_DBG_LOG_
#define  USER_LCD_DbgLog(...)    	USER_LCD_LOG_BackupColor();\
									USER_USER_LCD_LineColor = GUI_CYAN;\
									GUI_SetFont(&USER_LCD_LOG_TEXT_FONT);\
                  TARGET_Set(TARGET_LCD);\
									printf(__VA_ARGS__);\
                  TARGET_Set(TARGET_UART);\
									USER_LCD_LOG_RestoreColor();
#else
#define  USER_LCD_DbgLog(...)    
#endif

#ifdef __USER_USE_WARNING_LOG_
#define  USER_LCD_WarningLog(...)    	USER_LCD_LOG_BackupColor();\
									USER_LCD_LineColor=GUI_RED;\
									GUI_SetFont(&USER_LCD_LOG_TEXT_FONT);\
									TARGET_Set(TARGET_LCD);\
									printf(__VA_ARGS__);\
                  TARGET_Set(TARGET_UART);\
									USER_LCD_LOG_RestoreColor();
#else
#define  USER_LCD_WarningLog(...)    
#endif

#ifdef __USER_USE_NOSTIFY_LOG_
#define  USER_LCD_NostifyLog(...)    	USER_LCD_LOG_BackupColor();\
									USER_LCD_LineColor=GUI_LIGHTBLUE;\
									GUI_SetFont(&USER_LCD_LOG_TEXT_FONT);\
									TARGET_Set(TARGET_LCD);\
									printf(__VA_ARGS__);\
                  TARGET_Set(TARGET_UART);\
									USER_LCD_LOG_RestoreColor();
#else
#define  USER_LCD_NostifyLog(...)    
#endif

#define USER_LINE(x) ((x) * (GUI_GetFontSizeY()))
/**
  * @}
  */ 

/** @defgroup LCD_LOG_Exported_Variables
  * @{
  */ 
extern uint32_t USER_LCD_LineColor;

/**
  * @}
  */ 

/** @defgroup LCD_LOG_Exported_FunctionsPrototype
  * @{
  */ 
void USER_LCD_LOG_Init(uint16_t x0,uint16_t y0,uint16_t xSize,uint16_t ySize);
void USER_LCD_LOG_DeInit(void);
void USER_LCD_LOG_SetTestArea(uint16_t x0,uint16_t y0,uint16_t xSize,uint16_t ySize);
void USER_LCD_LOG_ClearTextZone(void);
void USER_LCD_LOG_UpdateDisplay (void);

void USER_LCD_LOG_BackupColor(void);
void USER_LCD_LOG_RestoreColor(void);

#if (USER_LCD_SCROLL_ENABLED == 1)
 ErrorStatus USER_LCD_LOG_ScrollBack(void);
 ErrorStatus USER_LCD_LOG_ScrollForward(void);
#endif

void USER_LCD_LOG_DisplayStringAt(uint16_t X, uint16_t Y, char *pText, USER_Text_AlignModeTypdef mode, GUI_COLOR color);
void USER_LCD_LOG_DisplayStringAtLine(uint16_t Line, char *ptr, GUI_COLOR color);
void USER_LCD_LOG_ClearStringLine(uint32_t Line);

void LCD_PutChar (char ch);
void USER_LCD_LOG_SetDisplayEnable(uint8_t onOff);
  

/**
  * @}
  */ 


#endif /* __USER_LCD_LOG_H__ */

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

/**
  * @}
  */  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
