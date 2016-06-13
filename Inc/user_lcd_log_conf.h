/**
  ******************************************************************************
  * @file    ../Inc/lcd_log_conf.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    26-June-2014
  * @brief   lcd_log configuration template file.
  *          This file should be copied to the application folder and modified 
  *          as follows:
  *            - Rename it to 'lcd_log_conf.h'.
  *            - Update the name of the LCD driver's header file, depending on
  *               the EVAL board you are using, see line40 below (be default this  
  *               file will generate compile error unless you do this modification).
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
#ifndef  __USER_LCD_LOG_CONF_H
#define  __USER_LCD_LOG_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>


/** @addtogroup LCD_LOG
  * @{
  */
/** 
  * @brief  LCD color  
  */ 

/** @defgroup LCD_LOG
  * @brief This file is the 
  * @{
  */ 


/** @defgroup LCD_LOG_CONF_Exported_Defines
  * @{
  */ 

/* Comment the line below to disable the scroll back and forward features */
/* #define     LCD_SCROLL_ENABLED */
            
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
#define     USER_CACHE_SIZE              10
#define     USER_YWINDOW_SIZE            14

#if (USER_YWINDOW_SIZE > 25)
  #error "Wrong USER_YWINDOW SIZE"
#endif

/* Redirect the printf to the LCD */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define USER_LCD_LOG_PUTCHAR int __io_putchar(int ch)
#else
#define USER_LCD_LOG_PUTCHAR int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/** @defgroup LCD_LOG_CONF_Exported_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup LCD_LOG_Exported_Macros
  * @{
  */ 


/**
  * @}
  */ 

/** @defgroup LCD_LOG_CONF_Exported_Variables
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup LCD_LOG_CONF_Exported_FunctionsPrototype
  * @{
  */ 

/**
  * @}
  */ 


#endif /* __LCD_LOG_CONF_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
