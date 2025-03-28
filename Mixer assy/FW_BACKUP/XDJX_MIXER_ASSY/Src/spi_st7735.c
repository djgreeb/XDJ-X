#include "spi_st7735.h"
#include "font16.c"
#include "font12.c"
#include "font8.c"
#include "fontVFD.c"
//-------------------------------------------------------------------
extern SPI_HandleTypeDef hspi1;
//-------------------------------------------------------------------
uint8_t TFT_WIDTH;
uint8_t TFT_HEIGHT;
//-------------------------------------------------------------------
typedef struct
{
	uint16_t TextColor;
	uint16_t BackColor;
	sFONT *pFont;
}LCD_DrawPropTypeDef;
LCD_DrawPropTypeDef lcdprop;
//-------------------------------------------------------------------
void TFT_SendCommand(uint8_t cmd)
{
  DC_COMMAND();
  HAL_SPI_Transmit(&hspi1, &cmd, 1, 500);
}
//-------------------------------------------------------------------
void TFT_SendData(uint8_t dt)
{
	DC_DATA();
	HAL_SPI_Transmit(&hspi1, &dt, 1, 500);
}
//-------------------------------------------------------------------
static void TFT_WriteData(uint8_t* buff, size_t buff_size) {
	DC_DATA();
	while(buff_size > 0) {
		uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
		HAL_SPI_Transmit(&hspi1, buff, chunk_size, HAL_MAX_DELAY);
		buff += chunk_size;
		buff_size -= chunk_size;
	}
}
//-------------------------------------------------------------------
void TFT_reset(void)
{
	RESET_ACTIVE();
	HAL_Delay(50);
	RESET_IDLE();
}
//-------------------------------------------------------------------
static void TFT_SetAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
	{
  // column address set
  TFT_SendCommand(0x2A); // CASET
		{
    uint8_t data[4] = { 0, x0, 0, x1};
    TFT_WriteData(data, 4);
		}
  // row address set
  TFT_SendCommand(0x2B); // RASET
		{
    uint8_t data[4] = { 0, y0, 0, y1};
    TFT_WriteData(data, 4);
		}
  // write to RAM
  TFT_SendCommand(0x2C); // RAMWR
	}
//-------------------------------------------------------------------
void TFT_FillRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
	{
	uint32_t YO;
	uint32_t i;	
  TFT_SetAddrWindow(x1, y1, x2, y2);
  uint8_t data[2] = { color >> 8, color & 0xFF };
	DC_DATA();
	YO = (x2-x1+1)*(y2-y1+1);
  for(i=0; i<YO; i++)
		{
    HAL_SPI_Transmit(&hspi1, data, 2, 10);
		}
	}
//-------------------------------------------------------------------
void TFT_FillScreen(uint16_t color)
{
  TFT_FillRect(0, 0, TFT_WIDTH-1, TFT_HEIGHT-1, color);
}
//-------------------------------------------------------------------
void TFT_DrawPixel(uint8_t x, uint8_t y, uint16_t color)
	{	
  // column address set
  TFT_SendCommand(0x2A); // CASET
		{
    uint8_t data[2] = {0, x};
		DC_DATA();
		HAL_SPI_Transmit(&hspi1, data, 2, 10);
		}
  // row address set
  TFT_SendCommand(0x2B); // RASET
		{
    uint8_t data[2] = {0, y};
		DC_DATA();
		HAL_SPI_Transmit(&hspi1, data, 2, 10);
		}
	TFT_SendCommand(0x2C);
		{
		uint8_t clr[2] = {color>>8, color&0xFF};		
		DC_DATA();
		HAL_SPI_Transmit(&hspi1, clr, 2, 10);	
		}	
	return;
	}
//------------------------------------------------------------------
void TFT_DrawLine(uint16_t color, uint8_t x1, uint8_t y1,
											uint8_t x2, uint8_t y2)
{
  int steep = abs(y2-y1)>abs(x2-x1);
  if(steep)
  {
    swap(x1,y1);
    swap(x2,y2);
  }
  if(x1>x2)
  {
    swap(x1,x2);
    swap(y1,y2);
  }
  int dx,dy;
  dx=x2-x1;
  dy=abs(y2-y1);
  int err=dx/2;
  int ystep;
  if(y1<y2) ystep=1;
  else ystep=-1;
  for(;x1<=x2;x1++)
  {
    if(steep) TFT_DrawPixel(y1,x1,color);
    else TFT_DrawPixel(x1,y1,color);
    err-=dy;
    if(err<0)
    {
      y1 += ystep;
      err+=dx;
    }
  }
}
//-------------------------------------------------------------------
void TFT_DrawRect(uint16_t color, uint8_t x1, uint8_t y1,
											uint8_t x2, uint8_t y2)
{
	TFT_DrawLine(color,x1,y1,x2,y1);
	TFT_DrawLine(color,x2,y1,x2,y2);
	TFT_DrawLine(color,x1,y1,x1,y2);
	TFT_DrawLine(color,x1,y2,x2,y2);
}


//-------------------------------------------------------------------
void TFT_DrawCircle(uint8_t x0, uint8_t y0, int r, uint16_t color)
{
	int f = 1-r;
	int ddF_x=1;
	int ddF_y=-2*r;
	int x = 0;
	int y = r;
	TFT_DrawPixel(x0,y0+r,color);
	TFT_DrawPixel(x0,y0-r,color);
	TFT_DrawPixel(x0+r,y0,color);
	TFT_DrawPixel(x0-r,y0,color);
	while (x<y)
	{
		if (f>=0)
		{
			y--;
			ddF_y+=2;
			f+=ddF_y;
		}
		x++;
		ddF_x+=2;
		f+=ddF_x;
		TFT_DrawPixel(x0+x,y0+y,color);
		TFT_DrawPixel(x0-x,y0+y,color);
		TFT_DrawPixel(x0+x,y0-y,color);
		TFT_DrawPixel(x0-x,y0-y,color);
		TFT_DrawPixel(x0+y,y0+x,color);
		TFT_DrawPixel(x0-y,y0+x,color);
		TFT_DrawPixel(x0+y,y0-x,color);
		TFT_DrawPixel(x0-y,y0-x,color);
	}
}

//-------------------------------------------------------------------
void TFT_SetTextColor(uint16_t color)
{
  lcdprop.TextColor=color;
}
//------------------------------------
void TFT_SetBackColor(uint16_t color)
{
  lcdprop.BackColor=color;
}
//------------------------------------
void TFT_SetFont(sFONT *pFonts)
{
  lcdprop.pFont=pFonts;
}
//-------------------------------------------------------------------
void TFT_DrawChar(uint8_t x, uint8_t y, uint8_t c)
{
  uint32_t i = 0, j = 0;
  uint16_t height, width;
  uint8_t offset;
  uint8_t *c_t;
  uint8_t *pchar;
  uint32_t line=0;
  height = lcdprop.pFont->Height;
  width  = lcdprop.pFont->Width;
  offset = 8 *((width + 7)/8) -  width ;
  c_t = (uint8_t*) &(lcdprop.pFont->table[(c-' ') * lcdprop.pFont->Height * ((lcdprop.pFont->Width + 7) / 8)]);
  for(i = 0; i < height; i++)
  {
    pchar = ((uint8_t *)c_t + (width + 7)/8 * i);
    switch(((width + 7)/8))
    {
      case 1:
          line =  pchar[0];      
          break;
      case 2:
          line =  (pchar[0]<< 8) | pchar[1];
          break;
      case 3:
      default:
        line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];      
        break;
    }
    for (j = 0; j < width; j++)
    {
      if(line & (1 << (width- j + offset- 1))) 
      {
        TFT_DrawPixel((x + j), y, lcdprop.TextColor);
      }
      else
      {
        TFT_DrawPixel((x + j), y, lcdprop.BackColor);
      } 
    }
    y++;      
  }
}


//-------------------------------------------------------------------
void VFD_DrawChar(uint8_t x, uint8_t y, uint8_t c)
	{
  uint32_t i = 0, j = 0;
  uint8_t *c_t;
  uint8_t *pchar;
  uint32_t line=0;
  c_t = (uint8_t*) &(lcdprop.pFont->table[(c-' ')* 7]);
  for(i=0;i<7;i++)
		{
    pchar = ((uint8_t *)c_t + i);
    line =  pchar[0];      
    for (j=0;j<5;j++)
			{
      if(line&(1<<(7-j))) 
				{
        TFT_DrawPixel((x + 3*j), y+i*2, lcdprop.TextColor);
				TFT_DrawPixel((x + 3*j), y+i*2+1, lcdprop.TextColor);
				TFT_DrawPixel((x + 3*j)+1, y+i*2, lcdprop.TextColor);	
				TFT_DrawPixel((x + 3*j)+1, y+i*2+1, lcdprop.TextColor);		
				}
      else
				{
        TFT_DrawPixel((x + 3*j), y+i*2, lcdprop.BackColor);
				TFT_DrawPixel((x + 3*j), y+i*2+1, lcdprop.BackColor);
				TFT_DrawPixel((x + 3*j)+1, y+i*2, lcdprop.BackColor);	
				TFT_DrawPixel((x + 3*j)+1, y+i*2+1, lcdprop.BackColor);		
				} 
			}
    y++;      
		}
	}
//-------------------------------------------------------------------
void TFT_String(uint8_t x,uint8_t y, char *str)
{
  while(*str)
  {
		if((lcdprop.pFont->Height)==7)				//VFD font
			{
			VFD_DrawChar(x,y,str[0]);					
			x+=19;				
			}			
		else
			{
			TFT_DrawChar(x,y,str[0]);
			x+=lcdprop.pFont->Width;	
			}

    (void)*str++;
  }
}
//-------------------------------------------------------------------
void TFT_SetRotation(void)
	{
  TFT_SendCommand(0x36);
  TFT_SendData(0xC0);						//RGB, MX=1, MY=1
  TFT_WIDTH = 128;
  TFT_HEIGHT = 160;	
	}
//-------------------------------------------------------------------
void TFT_FontsIni(void)
{
  Font8.Height = 8;
  Font8.Width = 5;
  Font12.Height = 12;
  Font12.Width = 7;
  Font16.Height = 16;
  Font16.Width = 11;
  lcdprop.BackColor=TFT_BLACK;
  lcdprop.TextColor=TFT_GREEN;
  lcdprop.pFont=&Font16;
}
//-------------------------------------------------------------------
void TFT_ini(uint16_t w_size, uint16_t h_size)
{
  uint8_t data[15];
  CS_ACTIVE();
  TFT_reset();
  //Software Reset
  TFT_SendCommand(0x01);
  HAL_Delay(1000);
//  //Power Control A  
//  data[0] = 0x39;
//  data[1] = 0x2C;
//  data[2] = 0x00;
//  data[3] = 0x34;
//  data[4] = 0x02;
//  TFT_SendCommand(0xCB);
//  TFT_WriteData(data, 5);
//  //Power Control B
//  data[0] = 0x00;
//  data[1] = 0xC1;
//  data[2] = 0x30;
//  TFT_SendCommand(0xCF);
//  TFT_WriteData(data, 3);
//  //Driver timing control A
//  data[0] = 0x85;
//  data[1] = 0x00;
//  data[2] = 0x78;
//  TFT_SendCommand(0xE8);
//  TFT_WriteData(data, 3);
//  //Driver timing control B
//  data[0] = 0x00;
//  data[1] = 0x00;
//  TFT_SendCommand(0xEA);
//  TFT_WriteData(data, 2);
//  //Power on Sequence control
//  data[0] = 0x64;
//  data[1] = 0x03;
//  data[2] = 0x12;
//  data[3] = 0x81;
//  TFT_SendCommand(0xED);
//  TFT_WriteData(data, 4);
//  //Pump ratio control
//  data[0] = 0x20;
//  TFT_SendCommand(0xF7);
//  TFT_WriteData(data, 1);
//  //Power Control,VRH[5:0]
//  data[0] = 0x10;
//  TFT_SendCommand(0xC0);
//  TFT_WriteData(data, 1);
//  //Power Control,SAP[2:0];BT[3:0]
//  data[0] = 0x10;
//  TFT_SendCommand(0xC1);
//  TFT_WriteData(data, 1);
//  //VCOM Control 1
//  data[0] = 0x3E;
//  data[1] = 0x28;
//  TFT_SendCommand(0xC5);
//  TFT_WriteData(data, 2);
//  //VCOM Control 2
//  data[0] = 0x86;
//  TFT_SendCommand(0xC7);
//  TFT_WriteData(data, 1);
	
	
	//Memory Acsess Control
  data[0] = 0x08;				//0x48
  TFT_SendCommand(0x36);
  TFT_WriteData(data, 1);
	
	
	
	
  //Pixel Format Set
  data[0] = 0x05;//16bit  
  TFT_SendCommand(0x3A);
  TFT_WriteData(data, 1);
	
	
  //Frame Rratio Control, Standard RGB Color
  data[0] = 0x00;					//0x00
  data[1] = 0x10;					//0x18
  TFT_SendCommand(0xB1);
  TFT_WriteData(data, 2);
	
	
  //Display Function Control
  data[0] = 0x08;
  data[1] = 0x82;
  data[2] = 0x27;//320 строк
  TFT_SendCommand(0xB6);
  TFT_WriteData(data, 3);	
//	
//	
//  //Enable 3G (пока не знаю что это за режим)
//  data[0] = 0x00;//не включаем
//  TFT_SendCommand(0xF2);
//  TFT_WriteData(data, 1);
	
	
  //Gamma set
  data[0] = 0x01;//Gamma Curve (G2.2) (Кривая цветовой гаммы)
  TFT_SendCommand(0x26);
  TFT_WriteData(data, 1);
  //Positive Gamma  Correction
  data[0] = 0x0F;
  data[1] = 0x31;
  data[2] = 0x2B;
  data[3] = 0x0C;
  data[4] = 0x0E;
  data[5] = 0x08;
  data[6] = 0x4E;
  data[7] = 0xF1;
  data[8] = 0x37;
  data[9] = 0x07;
  data[10] = 0x10;
  data[11] = 0x03;
  data[12] = 0x0E;
  data[13] = 0x09;
  data[14] = 0x00;
  TFT_SendCommand(0xE0);
  TFT_WriteData(data, 15);
  //Negative Gamma  Correction
  data[0] = 0x00;
  data[1] = 0x0E;
  data[2] = 0x14;
  data[3] = 0x03;
  data[4] = 0x11;
  data[5] = 0x07;
  data[6] = 0x31;
  data[7] = 0xC1;
  data[8] = 0x48;
  data[9] = 0x08;
  data[10] = 0x0F;
  data[11] = 0x0C;
  data[12] = 0x31;
  data[13] = 0x36;
  data[14] = 0x0F;
  TFT_SendCommand(0xE1);
  TFT_WriteData(data, 15);
	
  TFT_SendCommand(0x11);		//SLEEP OUT
  HAL_Delay(120);
  //Display ON
  data[0] = TFT_ROTATION;
  TFT_SendCommand(0x29);
  TFT_WriteData(data, 1);
  TFT_WIDTH = w_size;
  TFT_HEIGHT = h_size;
  TFT_FontsIni();
}
//-------------------------------------------------------------------

