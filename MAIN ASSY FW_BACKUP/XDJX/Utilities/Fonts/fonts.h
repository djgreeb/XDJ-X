/**
  ******************************************************************************
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


typedef struct _tFont
{    
  const uint8_t *table;
  uint8_t Width;
  uint8_t Height;
  
} sFONT;
extern sFONT Font7W;
extern sFONT Font9D;
extern sFONT Font15P;
extern sFONT Font14D;
extern sFONT Font18D;
extern sFONT FontBMP;

#define LINE(x) ((x) * (((sFONT *)BSP_LCD_GetFont())->Height))



#ifdef __cplusplus
}
#endif
  
#endif /* __FONTS_H */


/*********************************************************************************/
