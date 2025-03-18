/**
  ******************************************************************************
  * @file    stm32746g_discovery_lcd.h
  ******************************************************************************
  ******************************************************************************
  */  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32746G_DISCOVERY_LCD_H
#define __STM32746G_DISCOVERY_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
/* Include LCD component Driver */

/* Include SDRAM Driver */
#include "stm32746g_discovery_sdram.h"
#include "../../../Utilities/Fonts/fonts.h"
  
 
typedef struct 
{ 
  uint16_t TextColor; 
  uint16_t BackColor;  
  sFONT    *pFont;
}LCD_DrawPropTypeDef;   
   
typedef struct 
{
  int16_t X;
  int16_t Y;
}Point, * pPoint; 
   
static uint32_t            ActiveLayer = 0;				/////////////////////////////////////

/** 
  * @brief  Line mode structures definition  
  */ 
typedef enum
{
  CENTER_MODE             = 0x01,    /* Center mode */
  RIGHT_MODE              = 0x02,    /* Right mode  */
  LEFT_MODE               = 0x03,     /* Left mode   */
	TRANSPARENT_MODE        = 0x04,     /* transparent mode   */
	INFO_MODE        				= 0x05,     /* transparent+info mode*/
	WAVEFORM_MODE        		= 0x06,     /* transparent mode+waveform mode   */
}Text_AlignModeTypdef;

#define MAX_LAYER_NUMBER       ((uint32_t)2)

#define LCD_LayerCfgTypeDef    LTDC_LayerCfgTypeDef

#define LTDC_ACTIVE_LAYER	     ((uint32_t)1) /* Layer 1 */
  
#define LCD_OK                 ((uint8_t)0x00)
#define LCD_ERROR              ((uint8_t)0x01)

#define LCD_FB_START_ADDRESS       ((uint32_t)0xC0000000)

#define LCD_COLOR_BLUE          ((uint16_t)0x801F)
#define LCD_COLOR_GREEN         ((uint16_t)0x83E0)
#define LCD_COLOR_RED           ((uint16_t)0xFC00)
#define LCD_COLOR_WHITE					((uint16_t)0xFFFF)
#define LCD_COLOR_BLACK         ((uint16_t)0x8000)
#define LCD_COLOR_TRANSPARENT   ((uint16_t)0x0000)

/** 
  * @brief LCD default font 
  */ 
#define LCD_DEFAULT_FONT        Font8     

uint8_t  BSP_LCD_Init(void);
uint8_t  BSP_LCD_DeInit(void);
uint32_t BSP_LCD_GetXSize(void);
uint32_t BSP_LCD_GetYSize(void);
void     BSP_LCD_SetXSize(uint32_t imageWidthPixels);
void     BSP_LCD_SetYSize(uint32_t imageHeightPixels);

/* Functions using the LTDC controller */
void     BSP_LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FrameBuffer);
void     BSP_LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency);
void     BSP_LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address);
void     BSP_LCD_SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue);
void     BSP_LCD_ResetColorKeying(uint32_t LayerIndex);
void     BSP_LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

void     BSP_LCD_SelectLayer(uint32_t LayerIndex);
void     BSP_LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State);

void     BSP_LCD_SetTextColor(uint16_t Color);
uint16_t BSP_LCD_GetTextColor(void);
void     BSP_LCD_SetBackColor(uint16_t Color);
uint16_t BSP_LCD_GetBackColor(void);
void     BSP_LCD_SetFont(sFONT *fonts);
sFONT    *BSP_LCD_GetFont(void);

uint32_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void     BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code);
void     BSP_LCD_Clear(uint16_t Color);
void     BSP_LCD_ClearStringLine(uint32_t Line);
void     BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr);
void     BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode);
void     BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

void     BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void 		 ForceDrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint32_t color);
void     BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void     BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount);
void     BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void     BSP_LCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp);

void     BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount);
void     BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);

void     BSP_LCD_DisplayOff(void);
void     BSP_LCD_DisplayOn(void);

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
void     BSP_LCD_MspInit(LTDC_HandleTypeDef *hltdc, void *Params);
void     BSP_LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params);



#ifdef __cplusplus
}
#endif

#endif /* __STM32746G_DISCOVERY_LCD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
