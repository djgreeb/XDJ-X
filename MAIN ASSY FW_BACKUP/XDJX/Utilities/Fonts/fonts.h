/**
  ******************************************************************************
  * @file    fonts.h
  * @author  
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONTS_H
#define __FONTS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;
extern sFONT Font8;
extern sFONT Font11D;
extern sFONT Font13D;
extern sFONT Font15P;
extern sFONT Font14D;
extern sFONT Font18D;
extern sFONT Font20D;
extern sFONT Font23D;
extern sFONT Font26D;
extern sFONT FontBMP;




/** @defgroup FONTS_Exported_Constants
  * @{
  */ 
#define LINE(x) ((x) * (((sFONT *)BSP_LCD_GetFont())->Height))



#ifdef __cplusplus
}
#endif
  
#endif /* __FONTS_H */


/************************/
