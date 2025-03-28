/////////////////////////////////////////////////////////////////////////////////////////////////
// 	tft gui
//	product: XDJ-X mixer assy
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

void TFT_DRAW_STATIC(void);
void TFT_CHSELECTOR(uint8_t ch);
void TFT_FXSELECTOR(uint8_t fx);
void TFT_DRAW_BT(uint8_t enb);


static const uint8_t icon_ch[5] = {0x69, 0x89, 0x8F, 0x89, 0x69};
static const uint8_t icon_select[20] = {0x67, 0x84, 0x67, 0x14, 0x67,
																				0xA3, 0x22, 0x23, 0x22, 0xBB,
																				0xCD, 0x10, 0x90, 0x10, 0xCC,
																				0xF0, 0x40, 0x40, 0x40, 0x40};
static const uint8_t icon_parameter[30] = {0xE3, 0x94, 0x94, 0xE7, 0x84,
																					 0x38, 0xA5, 0xA5, 0xB9, 0xA5,
																					 0xC8, 0x2D, 0x2A, 0xE8, 0x28,
																					 0xBD, 0xA0, 0xB8, 0xA0, 0xBC,
																					 0xF7, 0x44, 0x47, 0x44, 0x47,
																					 0xB8, 0x24, 0x24, 0x38, 0xA4};
static const uint8_t icon_bpm[21] = {0xFE, 0x92, 0x92, 0x92, 0x6C,
																		 0x00, 0x00, 0xFE, 0x88, 0x88,
																		 0x88, 0x70, 0x00, 0x00, 0xFE,
																		 0x40, 0x20, 0x10, 0x20, 0x40,
																		 0xFE};
static const uint8_t icon_ms[17] =  {0xFE, 0x40, 0x80, 0x80, 0x7E,
																		 0x40, 0x80, 0x80, 0x7E, 0x00,
																		 0x00, 0x64, 0x92, 0x92, 0x92,
																		 0x92, 0x4C};
static const uint8_t icon_auto[26] =  {0x3E, 0x48, 0x88, 0x48, 0x3E,
																			 0x00, 0x00, 0xFC, 0x02, 0x02,
																		   0x02, 0xFC, 0x00, 0x00, 0x80,
																			 0x80, 0xFE, 0x80, 0x80, 0x00,
																		   0x00, 0x7C, 0x82, 0x82, 0x82,
																		   0x7C};
static const uint16_t icon_bt[23] =  {0x0FE0, 0x1FF0, 0x3FF8, 0x7DFC, 0xFCFE,
																			0xFC3E, 0xFC1E, 0xEC8E, 0xC48E, 0xE01E,
																			0xF03E, 0xF87E, 0xF03E, 0xE01E, 0xC48E,
																			0xEC8E, 0xFC1E, 0xFC3E, 0xFCFE, 0x7DFC,
																			0x3FF8, 0x1FF0, 0x0FE0 };
																					 
/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
void TFT_DRAW_STATIC(void)
	{
	uint8_t i;
	TFT_DrawLine(TFT_VFDBLUE, 2, 31, 40, 31);		
	TFT_DrawLine(TFT_VFDBLUE, 1, 32, 40, 32);	
	TFT_DrawLine(TFT_VFDBLUE, 0, 33, 40, 33);	
	TFT_DrawLine(TFT_VFDBLUE, 1, 34, 40, 34);		
	TFT_DrawLine(TFT_VFDBLUE, 2, 35, 40, 35);		
		
	TFT_DrawLine(TFT_VFDBLUE, 87, 31, 125, 31);		
	TFT_DrawLine(TFT_VFDBLUE, 87, 32, 126, 32);	
	TFT_DrawLine(TFT_VFDBLUE, 87, 33, 127, 33);	
	TFT_DrawLine(TFT_VFDBLUE, 87, 34, 126, 34);		
	TFT_DrawLine(TFT_VFDBLUE, 87, 35, 125, 35);		
		
	TFT_DrawLine(TFT_VFDBLUE, 2, 65, 37, 65);		
	TFT_DrawLine(TFT_VFDBLUE, 1, 66, 37, 66);	
	TFT_DrawLine(TFT_VFDBLUE, 0, 67, 37, 67);	
	TFT_DrawLine(TFT_VFDBLUE, 1, 68, 37, 68);		
	TFT_DrawLine(TFT_VFDBLUE, 2, 69, 37, 69);		
		
	TFT_DrawLine(TFT_VFDBLUE, 90, 65, 125, 65);		
	TFT_DrawLine(TFT_VFDBLUE, 90, 66, 126, 66);	
	TFT_DrawLine(TFT_VFDBLUE, 90, 67, 127, 67);	
	TFT_DrawLine(TFT_VFDBLUE, 90, 68, 126, 68);		
	TFT_DrawLine(TFT_VFDBLUE, 90, 69, 125, 69);

	for(i=0;i<7;i++)
		{
		if(i==3)
			{			
			TFT_FillRect(19*i, 134, 13+19*i, 143, TFT_RED);	
			TFT_FillRect(19*i, 147, 13+19*i, 150, TFT_RED);		
			}
		else
			{
			TFT_FillRect(19*i, 134, 13+19*i, 143, TFT_VFDOFF);	
			TFT_FillRect(19*i, 147, 13+19*i, 150, TFT_WHITE);	
			}			
		}
	for(i=0;i<26;i++)
		{	
		if(i==12 || i==13)
			{
			TFT_FillRect(5*i, 155, 2+5*i, 159, TFT_RED);	
			}
		else
			{			
			TFT_FillRect(5*i, 155, 2+5*i, 159, TFT_VFDOFF);
			}
		}
	for(i=0;i<40;i++)					//CH
		{
		if((icon_ch[i/8]<<(i%8))&0x80)
			{
			TFT_DrawPixel(44+i%8, 31+i/8, TFT_VFDBLUE);	
			}			
		}
	for(i=0;i<160;i++)	//SELECT
		{
		if((icon_select[i/8]<<(i%8))&0x80)
			{
			TFT_DrawPixel(56+i%8+((i/8)/5)*8, 31+(i/8)%5, TFT_VFDBLUE);	
			}			
		}
	for(i=0;i<240;i++)	//PARAMETER
		{
		if((icon_parameter[i/8]<<(i%8))&0x80)
			{
			TFT_DrawPixel(41+i%8+((i/8)/5)*8, 65+(i/8)%5, TFT_VFDBLUE);	
			}			
		}	
	for(i=0;i<168;i++)	//BPM
		{
		if((icon_bpm[i/8]<<(i%8))&0x80)
			{
			TFT_DrawPixel(95+(i/8), 90+i%8, TFT_WHITE);	
			}			
		}		
	for(i=0;i<136;i++)	//MS
		{
		if((icon_ms[i/8]<<(i%8))&0x80)
			{
			TFT_DrawPixel(95+(i/8), 119+i%8, TFT_WHITE);	
			}			
		}			
	for(i=0;i<208;i++)	//AUTO
		{
		if((icon_auto[i/8]<<(i%8))&0x80)
			{
			TFT_DrawPixel(95+(i/8), 77+i%8, TFT_RED);	
			}			
		}			
		
	///////////////DRAW 1
	TFT_DrawLine(TFT_WHITE, 30, 44, 30, 56);	
	TFT_DrawLine(TFT_WHITE, 29, 44, 29, 56);	
	TFT_DrawLine(TFT_WHITE, 28, 45, 28, 56);	
	TFT_DrawLine(TFT_WHITE, 27, 46, 27, 48);	
	TFT_DrawLine(TFT_WHITE, 26, 47, 26, 49);
	TFT_DrawPixel(25, 48, TFT_WHITE);
	TFT_DrawPixel(25, 49, TFT_WHITE);
	///////////////DRAW 2	
	TFT_DrawLine(TFT_WHITE, 59, 56, 67, 56);	
	TFT_DrawLine(TFT_WHITE, 59, 55, 67, 55);	
	TFT_DrawLine(TFT_WHITE, 59, 54, 62, 54);	
	TFT_DrawLine(TFT_WHITE, 60, 53, 63, 53);		
	TFT_DrawLine(TFT_WHITE, 61, 52, 64, 52);	
	TFT_DrawLine(TFT_WHITE, 62, 51, 65, 51);	
	TFT_DrawLine(TFT_WHITE, 63, 50, 66, 50);	
	TFT_DrawLine(TFT_WHITE, 64, 49, 67, 49);	
	TFT_DrawLine(TFT_WHITE, 65, 48, 67, 48);
	TFT_DrawLine(TFT_WHITE, 65, 47, 67, 47);	
	TFT_DrawLine(TFT_WHITE, 65, 46, 67, 46);		
	TFT_DrawLine(TFT_WHITE, 59, 47, 61, 47);	
	TFT_DrawLine(TFT_WHITE, 59, 46, 61, 46);
	TFT_DrawLine(TFT_WHITE, 60, 45, 66, 45);
	TFT_DrawLine(TFT_WHITE, 61, 44, 65, 44);	
	///////////////DRAW M	
	TFT_DrawLine(TFT_WHITE, 92, 44, 92, 56);	
	TFT_DrawLine(TFT_WHITE, 93, 44, 93, 56);	
	TFT_DrawLine(TFT_WHITE, 94, 44, 94, 56);	
	TFT_DrawLine(TFT_WHITE, 102, 44, 102, 56);	
	TFT_DrawLine(TFT_WHITE, 103, 44, 103, 56);	
	TFT_DrawLine(TFT_WHITE, 104, 44, 104, 56);
	TFT_DrawLine(TFT_WHITE, 95, 44, 95, 48);
	TFT_DrawLine(TFT_WHITE, 101, 44, 101, 48);
	TFT_DrawLine(TFT_WHITE, 96, 46, 96, 52);
	TFT_DrawLine(TFT_WHITE, 100, 46, 100, 52);	
	TFT_DrawLine(TFT_WHITE, 97, 49, 97, 55);
	TFT_DrawLine(TFT_WHITE, 99, 49, 99, 55);
	TFT_DrawLine(TFT_WHITE, 98, 53, 98, 56);

	sprintf((char*)STR_BUFF, "123");	
	TFT_SetTextColor(TFT_WHITE);
	TFT_SetFont(&FontVFD);					
	TFT_String(38, 77, STR_BUFF);	
	sprintf((char*)STR_BUFF, "490");
	TFT_String(38, 106, STR_BUFF);			
//	TFT_DrawPixel(0, 0, TFT_RED);	
//	TFT_DrawPixel(0, 159, TFT_RED);	
//	TFT_DrawPixel(127, 0, TFT_RED);	
//	TFT_DrawPixel(127, 159, TFT_RED);	
	return;
	};

	
/////////////////////////////////////////////////////////////////////////////////////////////////
//
//	ch 0..2
//
//
void TFT_CHSELECTOR(uint8_t ch)
	{
	if(ch>2)
		{
		return;		
		}
	if(previous_ch!=ch)
		{
		uint8_t shft;	
		if(previous_ch==1)
			{
			shft = 35;	
			}
		else if(previous_ch==2)
			{
			shft = 70;	
			}
		else
			{
			shft = 0;	
			}
		TFT_DrawRect(TFT_BLACK, 15+shft, 40, 41+shft, 60);	
		TFT_DrawLine(TFT_BLACK, 16+shft, 39, 40+shft, 39);	
		TFT_DrawLine(TFT_BLACK, 16+shft, 61, 40+shft, 61);
		TFT_DrawLine(TFT_BLACK, 14+shft, 41, 14+shft, 59);	
		TFT_DrawLine(TFT_BLACK, 42+shft, 41, 42+shft, 59);
		TFT_DrawPixel(14+shft, 40, TFT_BLACK);	
		TFT_DrawPixel(15+shft, 39, TFT_BLACK);	
		TFT_DrawPixel(41+shft, 39, TFT_BLACK);
		TFT_DrawPixel(42+shft, 40, TFT_BLACK);	
		TFT_DrawPixel(42+shft, 60, TFT_BLACK);	
		TFT_DrawPixel(41+shft, 61, TFT_BLACK);	
		TFT_DrawPixel(15+shft, 61, TFT_BLACK);	
		TFT_DrawPixel(14+shft, 60, TFT_BLACK);	
		TFT_DrawPixel(16+shft, 41, TFT_BLACK);
		TFT_DrawPixel(40+shft, 41, TFT_BLACK);
		TFT_DrawPixel(40+shft, 59, TFT_BLACK);
		TFT_DrawPixel(16+shft, 59, TFT_BLACK);
		if(ch==1)
			{
			shft = 35;	
			}
		else if(ch==2)
			{
			shft = 70;	
			}
		else
			{
			shft = 0;	
			}	
		TFT_DrawRect(TFT_RED, 15+shft, 40, 41+shft, 60);	
		TFT_DrawLine(TFT_RED, 16+shft, 39, 40+shft, 39);	
		TFT_DrawLine(TFT_RED, 16+shft, 61, 40+shft, 61);
		TFT_DrawLine(TFT_RED, 14+shft, 41, 14+shft, 59);	
		TFT_DrawLine(TFT_RED, 42+shft, 41, 42+shft, 59);
		TFT_DrawPixel(14+shft, 40, 0x9800);	
		TFT_DrawPixel(15+shft, 39, 0x9800);	
		TFT_DrawPixel(41+shft, 39, 0x9800);
		TFT_DrawPixel(42+shft, 40, 0x9800);	
		TFT_DrawPixel(42+shft, 60, 0x9800);	
		TFT_DrawPixel(41+shft, 61, 0x9800);	
		TFT_DrawPixel(15+shft, 61, 0x9800);	
		TFT_DrawPixel(14+shft, 60, 0x9800);	
		TFT_DrawPixel(16+shft, 41, 0x9800);
		TFT_DrawPixel(40+shft, 41, 0x9800);
		TFT_DrawPixel(40+shft, 59, 0x9800);
		TFT_DrawPixel(16+shft, 59, 0x9800);	
		previous_ch = ch;	
		}
	return;	
	};
	
	
/////////////////////////////////////////////////////////////////////////////////////////////////
//
//	fx 0..7
//
//
void TFT_FXSELECTOR(uint8_t fx)
	{
	if(fx<8)
		{			
		if(fx==0)
			{
			sprintf((char*)STR_BUFF, "DELAY  ");	
			}				
		else if(fx==1)
			{
			sprintf((char*)STR_BUFF, "ECHO   ");	
			}	 	
		else if(fx==2)
			{
			sprintf((char*)STR_BUFF, "REVERB ");	
			}	
		else if(fx==3)
			{
			sprintf((char*)STR_BUFF, "TRANS  ");	
			}	
		else if(fx==4)
			{
			sprintf((char*)STR_BUFF, "FLANGER");	
			}	
		else if(fx==5)
			{
			sprintf((char*)STR_BUFF, "SLPROLL");	
			}	
		else if(fx==6)
			{
			sprintf((char*)STR_BUFF, "ROLL   ");	
			}	
		else
			{
			sprintf((char*)STR_BUFF, "VINYL  ");	
			}					
		TFT_SetTextColor(TFT_WHITE);
		TFT_SetFont(&FontVFD);					
		TFT_String(0, 1, STR_BUFF);		
		}
	return;	
	};
		
/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
void TFT_DRAW_BT(uint8_t enb)
	{
	uint16_t i;		
	if(enb==1)
		{
		for(i=0;i<368;i++)
			{
			if((icon_bt[i/16]<<(i%16))&0x8000)
				{
				TFT_DrawPixel(13+i%16, 76+i/16, TFT_BLUE);	
				}			
			}
		}		
	else		//draw black	
		{
		TFT_FillRect(13, 76, 28, 99, TFT_BLACK);
		}		
	return;	
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
																																									