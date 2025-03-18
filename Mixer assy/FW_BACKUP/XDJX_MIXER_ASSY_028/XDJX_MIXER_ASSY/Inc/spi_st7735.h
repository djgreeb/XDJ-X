#ifndef __SPI_ILI9341_H
#define __SPI_ILI9341_H
//-------------------------------------------------------------------
#include "stm32h7xx_hal.h"
#include "main.h"
#include <cstdlib>
#include "fonts.h"
//-------------------------------------------------------------------
#define RESET_ACTIVE() 		GPIOB->BSRR = 0x00100000		//HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_RESET)
#define RESET_IDLE() 			GPIOB->BSRR = 0x00000010		//HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET)
#define CS_ACTIVE() 			GPIOB->BSRR = 0x00400000		//HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET)
#define CS_IDLE() 				GPIOB->BSRR = 0x00000040		//HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET)
#define DC_COMMAND()  		GPIOD->BSRR = 0x00200000		//HAL_GPIO_WritePin(GPIOD, TFT_DC_Pin, GPIO_PIN_RESET)
#define DC_DATA() 				GPIOD->BSRR = 0x00000020		//HAL_GPIO_WritePin(GPIOD, TFT_DC_Pin, GPIO_PIN_SET)
//-------------------------------------------------------------------
#define TFT_MADCTL_MY  0x80
#define TFT_MADCTL_MX  0x40
#define TFT_MADCTL_MV  0x20
#define TFT_MADCTL_ML  0x10
#define TFT_MADCTL_RGB 0x00
#define TFT_MADCTL_BGR 0x08
#define TFT_MADCTL_MH  0x04
#define TFT_ROTATION (TFT_MADCTL_MX | TFT_MADCTL_BGR)
#define	TFT_BLACK   0x0000
#define	TFT_BLUE    0x001F
#define	TFT_RED     0xF800
#define	TFT_GREEN   0x07E0
#define TFT_CYAN    0x07FF
#define TFT_MAGENTA 0xF81F
#define TFT_YELLOW  0xFFE0
#define TFT_WHITE   0xFFFF
#define TFT_VFDBLUE   0x6DDF
#define TFT_VFDOFF   0x2945

//-------------------------------------------------------------------
#define swap(a,b) {int16_t t=a;a=b;b=t;}
//-------------------------------------------------------------------

void TFT_ini(uint16_t w_size, uint16_t h_size);
void TFT_FillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
void TFT_FillScreen(uint16_t color);
void TFT_DrawPixel(uint8_t x, uint8_t y, uint16_t color);
void TFT_DrawLine(uint16_t color, uint8_t x1, uint8_t y1,
											uint8_t x2, uint8_t y2);
void TFT_DrawRect(uint16_t color, uint8_t x1, uint8_t y1,
											uint8_t x2, uint8_t y2);
void TFT_DrawCircle(uint8_t x0, uint8_t y0, int r, uint16_t color);
void TFT_SetTextColor(uint16_t color);
void TFT_SetBackColor(uint16_t color);
void TFT_SetFont(sFONT *pFonts);
void TFT_DrawChar(uint8_t x, uint8_t y, uint8_t c);
void VFD_DrawChar(uint8_t x, uint8_t y, uint8_t c);
void TFT_String(uint8_t x, uint8_t y, char *str);
void TFT_SetRotation(void);

//-------------------------------------------------------------------
#endif /* __SPI_ILI9341_H */

