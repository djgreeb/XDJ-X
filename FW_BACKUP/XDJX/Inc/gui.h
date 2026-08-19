/////////////////////////////////////////////////////////////////////////////////////////////////
// 	gui.h	- lcd graphic interface functions
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

extern const uint8_t width15P[95];

void RedrawWaveforms(uint8_t dk, uint32_t position);
void intDRAW_WAVEFORM_FRAME(uint8_t dk, uint32_t position);						//internal function for redraw waveform
void DrawLOGO(void);
void DrawStaticWFM(uint8_t dk, uint8_t Tpos);
void DrawMemBar(uint8_t dk, uint8_t strt, uint8_t fin);							//Draw memory fill
void DrawpxMem(uint16_t h, uint8_t col, uint8_t pos);						//For Draw fill
void DrawpxforBar(uint8_t dk, uint16_t h, uint8_t pos);						//For Draw fill
void DrawStaticTime(uint8_t dk);
void DrawMemoryMarker(uint8_t dk, uint8_t p, uint8_t type, uint16_t color);
void DrawMTriangle(uint16_t x, uint8_t y, uint16_t color);
void DrawCTriangle(uint16_t x, uint8_t y, uint16_t color);
void DrawCueMarker(uint8_t dk, uint8_t p);
void ShowTempo(uint8_t dk, uint16_t tempo);
void ShowBPM(uint8_t dk, uint16_t bpm);
void DrawTempoRange(uint8_t dk, uint8_t Range);
void DrawLoopMode(uint8_t dk, uint8_t lp, uint8_t act);
void DrawKey(uint8_t dk, uint8_t k, uint8_t col);
void ShowQUANTIZE(uint8_t color);
void ShowACUE(uint8_t acue);
void ShowBATT(uint8_t lvl);
void ShowAC(uint8_t tp);
uint16_t color_dim(uint16_t dim, uint16_t mx,  uint16_t col);
void ShowTrackName(uint8_t dk); 		//Show track and artist name on waform display
void trackname_parser(uint8_t dk, uint16_t num);
void DrawREKORDBOX(void);	
void SwitchInformationLayer(uint8_t LAY);
void int_DRAW_TRANSPARENT_BAR(uint16_t col);
void int_B_DRAW_ALL_LINES(void);													//internal function for Browser
void int_B_DRAW_ONE_LINE(uint8_t UPDOWN);									//internal function for Browser
void int_BIx_DRAW_ALL_LINES(uint8_t lvl);									//internal function for Browser + INFO
void int_BI_DRAW_ONE_LINE(uint8_t UPDOWN);								//internal function for Browser + INFO
void int_DRAW_STARS_RATING(uint16_t rat);									//internal function for Browser + INFO 
void int_VALUE_to_KEY(uint8_t val);												//internal function for show KEY 
void ReDrawScroll(uint16_t total_elements, uint16_t current_element_pos);
void NAVIGATOR(uint8_t UPDOWN);														//Navigate browser, TAG LIST and UTILITY	
void DrawPhasebarStatic(void);									//phase sync bar static
void DrawPhasebar(uint8_t pos);									//phase bar
void int_U_DRAW_ALL_LINES(void);									//internal function for UTILITY
void int_U_DRAW_ONE_LINE(uint8_t UPDOWN);									//internal function for UTILITY
void int_U_REDRAW_ONE_LINE(void);													//internal function for UTILITY for change parameter
void UTILITY_PARAMETER(uint8_t n_prmtr);									//write to Buf[] name state parameter for utility
void intDrawLayer0_NOINFO(uint8_t CurrentCursorPosition);	//draw layer 0 for without INFO BROWSER and TAGLIST
void intDrawLayer0_INFO(uint8_t CurrentCursorPosition);		//draw layer 0 for INFO BROWSER and TAGLIST
void intDrawTriangle(uint8_t CurrentCursorPosition, uint8_t x);			//draw triangle for browser with INFO
void int_T_DRAW_ALL_LINES(void);													//internal function for Browser
void int_T_DRAW_ONE_LINE(uint8_t UPDOWN);									//internal function for Browser
void int_TI_DRAW_ALL_LINES(void);													//internal function for Browser + INFO
void int_TI_DRAW_ONE_LINE(uint8_t UPDOWN);								//internal function for Browser + INFO
void int_B1_DRAW_ONE_LINE(uint8_t UPDOWN);								//internal function for Browser 1 level
void int_B2_DRAW_ONE_LINE(uint8_t UPDOWN);								//internal function for Browser 2 level
void intDrawLayer0_BROWSER_1_3(uint8_t CurrentCursorPosition);
void intDrawLayer0_ANIMATION(uint8_t CurrentCursorPosition);	//internal function for Browser animation finish
void intDrawLayer0_INFO_ANIMATION(uint8_t CurrentCursorPosition);
void intDrawLayer0_NOINFO_ANIMATION(uint8_t CurrentCursorPosition);
void intDrawAllIcons(void);																//internal function for Browser INFO and Taglist INFO
void intClearInfoWin(void);																//internal function for Browser INFO and Taglist INFO
void DrawWFMSample(uint8_t dk, uint8_t pos);					//Draw sample Size 1x19 for static waveform	

///////////////////////////////
//	
//	draw logo Pioneer DJ on display
void DrawLOGO(void)
	{
	uint8_t frame = 0;
	uint8_t VL = 0;	
	uint16_t n = 0;
	while(frame<26)							//change frame
		{	
		n = 0;	
		if(VL==0)
			{
			BSP_LCD_SelectLayer(1);	
			while(n<57600)						//draw picture
				{
				BSP_LCD_DrawPixel(n%480, (185-n/480), (0x8000+(ANIMATION[frame][n][1]>>3)+32*(ANIMATION[frame][n][2]>>3)+1024*(ANIMATION[frame][n][0]>>3)));						
				n++;		
				}	
			BSP_LCD_SetTransparency(1, 255);		//верхний слой виден
			BSP_LCD_SelectLayer(0);
			VL = 1;
			}
		else
			{
			BSP_LCD_SelectLayer(0);
			while(n<57600)						//draw picture
				{
				BSP_LCD_DrawPixel(n%480, (185-n/480), (0x8000+(ANIMATION[frame][n][1]>>3)+32*(ANIMATION[frame][n][2]>>3)+1024*(ANIMATION[frame][n][0]>>3)));	
				n++;		
				}		
			BSP_LCD_SetTransparency(1, 0);		//верхний не слой виден
			VL = 0;	
			}
		frame++;
		HAL_Delay(55);		
		}
		
	BSP_LCD_SetTransparency(1, 0);		//верхний не слой виден	
	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetTransparency(0, 255);	
	BSP_LCD_SelectLayer(0);	
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);	
		
	HAL_Delay(330);	
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	sprintf((char*)Buf, "%s", "ver.");
	BSP_LCD_DisplayStringAt(384, 210,Buf, LEFT_MODE);	
	BSP_LCD_DisplayStringAt(420, 210, FIRMWARE_VERSION, TRANSPARENT_MODE);	
			
	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);	
	BSP_LCD_DrawLine(101, 160, 379, 160);	
	BSP_LCD_DrawLine(101, 166, 379, 166);
	BSP_LCD_DrawLine(100, 161, 100, 165);
	BSP_LCD_DrawLine(380, 161, 380, 165);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	n = 0;	
	while(n<277)
		{
		BSP_LCD_DrawLine(102+n, 162, 102+n, 164);
		n++;	
		HAL_Delay(11);	
		}	
	HAL_Delay(1200);		
	return;	
	};		
	
	
//////////////////////////////////////////////	
//	
//	internal function for redraw waveform	
//	
void intDRAW_WAVEFORM_FRAME(uint8_t dk, uint32_t position)
	{
	uint8_t h, h1;	
	if(dk==dkA)
		{
		h = 0;
		h1 = 19;	
		}
	else
		{
		h = 50;
		h1 = 70;	
		}	
	uint8_t l;
	if(UT_SET[6]==0)
		{
		l = 0;
		}
	else
		{
		l = 146;	
		}		
		
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
	BSP_LCD_FillRect(0, 20+h, 480, 8);					//optimization		
	uint16_t i, BG_COLOR;
	uint32_t adr, sadr;	
	uint8_t amplitude;	
	uint16_t u, x, mn;
	uint32_t ofs;
	uint32_t xsum;	
	uint8_t	j, r;
	x = 0;
	u = 0;			
	if(position>=(240-l))				//Old code: 3min 20-30uS. New optimization 2uS on all positions
		{	
		u = 1024;
		mn = 512;
		xsum = ((DynamicWaveformZOOM*(position-(240-l)))*20)/3;	
		while(mn>2)
			{	
			if((BEATGRID[dk][u]-(BEATGRID[dk][u]%DynamicWaveformZOOM))<xsum)
				{
				if(mn>4)
					{					
					u+=mn;
					}
				else
					{					
					i = 1;
					}						
				}
			else
				{
				if(mn>4)
					{			
					u-=mn;
					}
				else
					{					
					i = 0;
					}						
				}
			mn/=2;	
			}
			
		if(i==1)
			{
			while((BEATGRID[dk][u]-(BEATGRID[dk][u]%DynamicWaveformZOOM))<xsum)
				{
				u++;	
				}		
			i = 0;		
			}			
		else
			{
			while((BEATGRID[dk][u]-(BEATGRID[dk][u]%DynamicWaveformZOOM))>xsum)
				{
				if(u==0)
					{					
					break;
					}
				u--;		
				}			
			u++;	
			}			
		}	
	BSP_LCD_FillRect(0, 65+h, 480, 5);			//optimization	

	for(i=0;i<480;i++)
		{	
		sadr = ((i+position)-(240-l));	
		adr = DynamicWaveformZOOM*sadr;	
		if(CUE_ADR[dk]<LOOP_OUT[dk])	
			{
			if(adr>=CUE_ADR[dk] && adr<LOOP_OUT[dk])	
				{
				if(loop_active[dk])
					{
					BG_COLOR = LOOP_ACTIVE_COLOR;	
					}
				else
					{
					BG_COLOR = LOOP_INACTIVE_COLOR;		
					}		
				}
			else
				{
				BG_COLOR = LCD_COLOR_BLACK;		
				}			
			}
		else
			{
			BG_COLOR = LCD_COLOR_BLACK;	
			}	
		if(adr<=all_long[dk])
			{
			if((((BEATGRID[dk][u+x]*3)/20)-(((BEATGRID[dk][u+x]*3)/20)%DynamicWaveformZOOM))==adr)	
				{	
				if(((u+x)%4)==((1-GRID_OFFSET[dk])&0x03))					//red grid
					{
					ForceDrawVLine(i, 23+h, 5, LCD_COLOR_RED);
					ForceDrawVLine(i, 65+h, 5, LCD_COLOR_RED);
					}
				else if(DynamicWaveformZOOM<8)			//white grid
					{	
					ForceDrawVLine(i, 23+h, 5, LCD_COLOR_WHITE);
					ForceDrawVLine(i, 65+h, 5, LCD_COLOR_WHITE);		
					}
				x++;	
				}	
			if(number_of_memory_cue_points[dk]>0)					//Draw MEMORY CUE triangle
				{
				for(j=0;j<number_of_memory_cue_points[dk];j++)
					{
					if((MEMORY_adr[dk][0][j]-(MEMORY_adr[dk][0][j]%DynamicWaveformZOOM))==adr)
						{
						if(i>2 && i<476)
							{
							BSP_LCD_SetTextColor(LCD_COLOR_RED);
							DrawMTriangle(i-3, 23+h, LCD_COLOR_RED);	
							}		
						}	
					}
				}	
			if(number_of_hot_cue_points[dk]>0)				//Draw HCUE squares
				{	
				for(j=0;j<8;j++)
					{
					if(HCUE_adr[dk][0][j]!=0xFFFF)							
						{
						if((HCUE_adr[dk][0][j]-(HCUE_adr[dk][0][j]%DynamicWaveformZOOM))==adr)
							{
							if(i>2 && i<476)
								{
								BSP_LCD_SetTextColor(HCUE16COLOR[dk][j]);	
								BSP_LCD_FillRect(i-3, 20+h, 7, 7);	
								for(r=0;r<25;r++)
									{
									if((hcicons[j][r/5]>>(7-(r%5)))&0x01)
										{
										BSP_LCD_DrawPixel(i+(r%5)-2, 21+(r/5)+h, LCD_COLOR_BLACK);	
										}
									}									
								}								
							}
						}
					}
				}				
			if((CUE_ADR[dk]-(CUE_ADR[dk]%DynamicWaveformZOOM))==adr) //Draw CUE triangle
				{
				if(i>2 && i<476)	
					{						
					DrawCTriangle(i-3, 66+h, LCD_COLOR_CUE);						
					}
				}
			}

		if(i==(239-l))																		///you can optimize 1 raz draw red line
			{
			bars[dk] = u+x;		
			}
		else if(i==(240-l))
			{
			}
		else
			{
			if((adr<all_long[dk]) && (adr<90000))
				{	
				if(DynamicWaveformZOOM==1)
					{		
					ofs = 0;	
					}
				else if(DynamicWaveformZOOM==2)	
					{	
					ofs = 90000;
					}
				else if(DynamicWaveformZOOM==4)	
					{	
					ofs = 135000;
					}				
				else if(DynamicWaveformZOOM==8)	
					{	
					ofs = 157500;
					}					
				else if(DynamicWaveformZOOM==16)	
					{				
					ofs = 168750;	
					}
				amplitude = (WFORMDYNAMIC[dk][ofs+sadr]&0x1F);	
				ForceDrawVLine(i, (46-amplitude)+h, 1+2*amplitude, COLOR_MAP[UT_SET[5]][WFORMDYNAMIC[dk][ofs+sadr]>>4]);
				if(amplitude<18)
					{						
					ForceDrawVLine(i, 28+h, 18-amplitude, BG_COLOR);	
					ForceDrawVLine(i, 47+amplitude+h, 18-amplitude, BG_COLOR);
					}	
				}
			else
				{
				ForceDrawVLine(i, 28+h, 37, LCD_COLOR_BLACK);
				}	
			}			
		}
	ForceDrawVLine(239-l, h1, 51, CURSOR_COLOR[dk]);	
	ForceDrawVLine(240-l, h1, 51, CURSOR_COLOR[dk]);	
	return;	
	};	

////////////////////////////////////////////////////////////////////////
//
//Function redraw bar on static waveform and redraw dynamic waveform
//position = 1/150 sec
void RedrawWaveforms(uint8_t dk, uint32_t position)
	{
	if(position>all_long[dk])
		{
		return;	
		}
	uint32_t clock_pos;	
	uint16_t h;
	if(dk==dkA)
		{
		h = 0;
		}
	else
		{
		h = 296;	
		}
			
	if(REMAIN_ENABLE[dk])
		{
		clock_pos = all_long[dk] - position;	
		}	
	else
		{
		clock_pos	= position;
		}
		
	if(dSHOW==WAVEFORM)
		{	
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
		BSP_LCD_SetFont(&Font18D);
		BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

		if(forcibly_redraw[dk]==1)
			{
			Prev10m[dk] = 0xFF;	
			Prev1m[dk] = 0xFF;
			Prev10s[dk] = 0xFF;
			Prev1s[dk] = 0xFF;
			Prev10f[dk] = 0xFF;
			Prev1f[dk] = 0xFF;
			PrevHf[dk] = 0xFF;				
			}

		if(Prev10m[dk]!=(clock_pos/90000)%10)			
			{
			Prev10m[dk] = (clock_pos/90000)%10;
			if(all_long[dk]<90000)
				{
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
				}
			sprintf((char *)Buf , "%0lu", Prev10m[dk]);				//10 Min
			BSP_LCD_DisplayStringAt(22+h, 170, Buf, LEFT_MODE);				
			if(all_long[dk]<90000)
				{
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
				}	
			}
		if(Prev1m[dk]!=(clock_pos/9000)%10)			
			{
			Prev1m[dk] = (clock_pos/9000)%10;	
			sprintf((char *)Buf , "%0lu", Prev1m[dk]);				//1 Min	
			BSP_LCD_DisplayStringAt(38+h, 170, Buf, LEFT_MODE);				
			}
		if(Prev10s[dk]!=(clock_pos/1500)%6)
			{
			Prev10s[dk] = (clock_pos/1500)%6;
			sprintf((char *)Buf , "%0lu", Prev10s[dk]);				//10 Sec	
			BSP_LCD_DisplayStringAt(68+h, 170, Buf, LEFT_MODE);		
			}
		if(Prev1s[dk]!=(clock_pos/150)%10)
			{
			Prev1s[dk] = (clock_pos/150)%10;
			sprintf((char *)Buf , "%0lu", Prev1s[dk]);				//1 Sec	
			BSP_LCD_DisplayStringAt(84+h, 170, Buf, LEFT_MODE);
			}		
		if(Prev10f[dk]!=((clock_pos/2)%75)/10)
			{
			Prev10f[dk] = ((clock_pos/2)%75)/10;
			BSP_LCD_SetFont(&Font14D);	
			sprintf((char *)Buf , "%0lu", Prev10f[dk]);				//10 F	
			BSP_LCD_DisplayStringAt(110+h, 174, Buf, LEFT_MODE);
			}		
		if(Prev1f[dk]!=((clock_pos/2)%75)%10)
			{
			Prev1f[dk] = ((clock_pos/2)%75)%10;
			BSP_LCD_SetFont(&Font14D);	
			sprintf((char *)Buf , "%0lu", Prev1f[dk]);				//1 F	
			BSP_LCD_DisplayStringAt(122+h, 174, Buf, LEFT_MODE);	
			}
		if(PrevHf[dk]!=clock_pos%2)
			{
			PrevHf[dk] = clock_pos%2;
			BSP_LCD_SetFont(&Font14D);		
			if(PrevHf[dk]%2==1)
				{
				sprintf((char*)Buf, "%s", "5");	
				}
			else
				{
				sprintf((char*)Buf, "%s", "0");	
				}
			BSP_LCD_DisplayStringAt(139+h, 174, Buf, LEFT_MODE);					
			}	
		}

	if(dk==dkA)
		{		
		if(deckTbuf[5][0]&0x10)						//detecting touch on touch on jog 
			{
			if(CURSOR_COLOR[dk]==LCD_COLOR_WHITE)
				{
				if(SLIPEN[dk]==0)
					{
					CURSOR_COLOR[dk] = LCD_COLOR_RED;
					}					
				else
					{					
					CURSOR_COLOR[dk] = LCD_COLOR_SLIP;
					}		
				forcibly_redraw[dk] = 1;	
				}
			}
		else
			{
			if(CURSOR_COLOR[dk]==LCD_COLOR_RED || CURSOR_COLOR[dk]==LCD_COLOR_SLIP)
				{
				CURSOR_COLOR[dk] = LCD_COLOR_WHITE;	
				forcibly_redraw[dk] = 1;	
				}
			}	
		}
	else
		{
		if(deckTbuf[5][8]&0x10)						//detecting touch on touch on jog 
			{
			if(CURSOR_COLOR[dk]==LCD_COLOR_WHITE)
				{
				if(SLIPEN[dk]==0)
					{
					CURSOR_COLOR[dk] = LCD_COLOR_RED;
					}					
				else
					{					
					CURSOR_COLOR[dk] = LCD_COLOR_SLIP;
					}		
				forcibly_redraw[dk] = 1;	
				}
			}
		else
			{
			if(CURSOR_COLOR[dk]==LCD_COLOR_RED || CURSOR_COLOR[dk]==LCD_COLOR_SLIP)
				{
				CURSOR_COLOR[dk] = LCD_COLOR_WHITE;	
				forcibly_redraw[dk] = 1;	
				}
			}
		}		
		
	DrawStaticWFM(dk, position*202/all_long[dk]);	
	
	if(dSHOW==WAVEFORM)
		{			
		position/= DynamicWaveformZOOM;					//zoom correction		
		if(dk==dkA)
			{
			if(position!=PreviousPositionDW[dk] || forcibly_redraw[dk]==1)				//6ms duty
				{			
				PreviousPositionDW[dk] = position;
				need_updt_dwf[dk] = 2;
				}
			}
		else
			{
			if(position!=PreviousPositionDW[dk] || forcibly_redraw[dk]==1)				//6ms duty
				{			
				PreviousPositionDW[dk] = position;
				need_updt_dwf[dk] = 2;
				}
		
			if(need_updt_dwf[dkA]>0 || need_updt_dwf[dkB]>0)	
				{
				if(VisibleLayer==0)
					{
					BSP_LCD_SelectLayer(1);	
					if(need_updt_dwf[dkA]>0)
						{						
						intDRAW_WAVEFORM_FRAME(dkA, PreviousPositionDW[dkA]);
						need_updt_dwf[dkA]--;
						}
					if(need_updt_dwf[dkB]>0)
						{			
						intDRAW_WAVEFORM_FRAME(dkB, PreviousPositionDW[dkB]);
						need_updt_dwf[dkB]--;
						}							
					//memcpy(&fbuf[114240], &fbuf[144960], 48960);							
					//HAL_DMA_GO(&hdma_memtomem_dma1_stream0, 0xC0049200, 0xC003D680, 8880);
					//HAL_DMA_Poll(&hdma_memtomem_dma1_stream0, HAL_DMA_FULL_TRANSFER, 8);
					LTDC_Layer2->CACR = 255;			//500nS
					LTDC->SRCR = LTDC_SRCR_VBR;		//	
					BSP_LCD_SelectLayer(0);
					VisibleLayer = 1;
					}
				else
					{
					BSP_LCD_SelectLayer(0);
					if(need_updt_dwf[dkA]>0)
						{						
						intDRAW_WAVEFORM_FRAME(dkA, PreviousPositionDW[dkA]);
						need_updt_dwf[dkA]--;	
						}
					if(need_updt_dwf[dkB]>0)
						{			
						intDRAW_WAVEFORM_FRAME(dkB, PreviousPositionDW[dkB]);
						need_updt_dwf[dkB]--;
						}		
					//memcpy(&fbuf[2400], &fbuf[33120], 48960);
					//HAL_DMA_GO(&hdma_memtomem_dma1_stream0, 0xC0012840, 0xC0006CC0, 8880);	
					//HAL_DMA_Poll(&hdma_memtomem_dma1_stream0, HAL_DMA_FULL_TRANSFER, 8);
					LTDC_Layer2->CACR = 0;					//500nS
					LTDC->SRCR = LTDC_SRCR_VBR;			//	
					VisibleLayer = 0;	
					}		
				}
				
			/////deck A//////	
			if(originalBPM[dkA]!=BPMGRID[dkA][bars[dkA]])										//вынести проверку за пределы (dSHOW==WAVEFORM)!!!
				{
				originalBPM[dkA] = BPMGRID[dkA][bars[dkA]];
				tempo_need_update[dkA] = 2;		
				}
			if(previous_bars[dkA]!=bars[dkA])
				{
				previous_bars[dkA] = bars[dkA];	
				if(originalBPM[dkA] != BPMGRID[dkA][bars[dkA]])										//вынести проверку за пределы (dSHOW==WAVEFORM)!!!
					{
					originalBPM[dkA] = BPMGRID[dkA][bars[dkA]];
					tempo_need_update[dkA] = 2;		
					}
				if((deckTbuf[5][0]&0x10)==0 && masterdeck==dkA)			//jog touch disable and deck A - master	
					{
					if((((2+GRID_OFFSET[dkA])&0x0003)+bars[dkA])%4==0)	
						{
						U_TX_DATA[0] = 0x96;	
						}
					else
						{
						U_TX_DATA[0] = 0x95;	
						}						
					U_TX_DATA[1] = mixerBPM>>8;
					U_TX_DATA[2] = mixerBPM%256;
					UART_TX(&huart1, U_TX_DATA, 3, 5);	
					}
				}
			/////deck B//////
			if(originalBPM[dkB] != BPMGRID[dkB][bars[dkB]])										//вынести проверку за пределы (dSHOW==WAVEFORM)!!!
				{
				originalBPM[dkB] = BPMGRID[dkB][bars[dkB]];
				tempo_need_update[dkB] = 2;		
				}
			if(previous_bars[dkB]!=bars[dkB])
				{
				previous_bars[dkB] = bars[dkB];	
				if(originalBPM[dkB] != BPMGRID[dkB][bars[dkB]])										//вынести проверку за пределы (dSHOW==WAVEFORM)!!!
					{
					originalBPM[dkB] = BPMGRID[dkB][bars[dkB]];
					tempo_need_update[dkB] = 2;		
					}
				if((deckTbuf[5][8]&0x10)==0 && masterdeck==dkB)			//jog touch disable and deck B - master	
					{
					if((((2+GRID_OFFSET[dkB])&0x0003)+bars[dkB])%4==0)	
						{
						U_TX_DATA[0] = 0x96;	
						}
					else
						{
						U_TX_DATA[0] = 0x95;	
						}						
					U_TX_DATA[1] = mixerBPM>>8;
					U_TX_DATA[2] = mixerBPM%256;
					UART_TX(&huart1, U_TX_DATA, 3, 5);
					}
				}	
			forcibly_redraw[dkA] = 0;	
			forcibly_redraw[dkB] = 0;	
			}	
		}	
	return;	
	};

//////////////////////////////////////////////////
//
//	Draw sample Size 1x19 for static waveform	
//	
void DrawWFMSample(uint8_t dk, uint8_t pos)
	{
	uint16_t h, color;	
	if(dk==dkA)
		{
		h = 0;	
		}		
	else
		{
		h = 272;	
		}
	if(UT_SET[5]==0)
		{
		color = COLOR_MAP[0][2*(WFST_AHB[dk][pos]>>5)];
		}
	else
		{
		color = WFST_RGB[dk][pos]; 	
		}			
	ForceDrawVLine(pos+3+h, 220-(WFST_AHB[dk][pos]&0x1F), (WFST_AHB[dk][pos]&0x1F)+1, color_dim(67, 100, color));
	ForceDrawVLine(pos+3+h, 220-WFST_AL[dk][pos], WFST_AL[dk][pos]+1, color);
	return;	
	};	

//////////////////////////////////////////////////
//
//	Size 202x19	
//	
//	Draw time position bar 0...201 (202 pixels wide)
//
//	DRAW_NEW_STATIC_WAVEFORM		203
//	CLEAR_WAVEFORM_ARRAY				204
//	MS_NOT_LOADED								205
//	REDRAW_IN_NREMAIN_MODE			206	
//	REDRAW_IN_REMAIN_MODE				207		
//	MS_ERROR										210
//
void DrawStaticWFM(uint8_t dk, uint8_t Tpos)
	{
	uint16_t h;	
	if(dk==dkA)
		{
		h = 0;	
		}		
	else
		{
		h = 272;	
		}		
	uint16_t i;
	if(Tpos<202)
		{	
		if(prevTpos[dk]!=Tpos)			
			{					
			ForceDrawVLine(prevTpos[dk]+3+h, 200, 29, LCD_COLOR_BLACK);
			ForceDrawVLine(prevTpos[dk]+4+h, 200, 29, LCD_COLOR_BLACK);	
			DrawWFMSample(dk, prevTpos[dk]);		
			if(prevTpos[dk]<201)	
				{
				DrawWFMSample(dk, prevTpos[dk]+1);	
				}
			if(prevTpos[dk]>Tpos)						//___<<||___     moving
				{				
				i = prevTpos[dk] - Tpos;
				if(REMAIN_ENABLE[dk])
					{	
					while(i>0)
						{
						if((Tpos+i)<200)
							{
							if((Tpos+i)%2==0)
								{
								ForceDrawVLine((Tpos+i)+5+h, 223, 4, LCD_COLOR_WHITE);	//drawing ||||||||||||||||||		
								}
							else
								{
								ForceDrawVLine((Tpos+i)+5+h, 223, 4, PBAR_COLOR_1);	//drawing ||||||||||||||||||		
								}	
							}	
						i--;	
						}
					}
				else
					{		
					while(i>0)
						{
						if((Tpos+i)<200)
							{
							if((Tpos+i)%2==0)
								{	
								BSP_LCD_DrawPixel(5+(Tpos+i)+h, 223, PBAR_COLOR_2);	//drawing :::::::::::::::
								BSP_LCD_DrawPixel(5+(Tpos+i)+h, 226, PBAR_COLOR_2);			
								}	
							else
								{	
								BSP_LCD_DrawPixel(5+(Tpos+i)+h, 223, PBAR_COLOR_3);	//drawing :::::::::::::::
								BSP_LCD_DrawPixel(5+(Tpos+i)+h, 226, PBAR_COLOR_3);			
								}	
							BSP_LCD_DrawPixel(5+(Tpos+i)+h, 224, LCD_COLOR_BLACK);		
							BSP_LCD_DrawPixel(5+(Tpos+i)+h, 225, LCD_COLOR_BLACK);		
							}
						i--;	
						}
					if(Tpos<199)
						{
						ForceDrawVLine(204+h, 223, 4, PBAR_COLOR_2);						//gray scroll vertical line in end		
						}			
					}
				}
			else if(prevTpos[dk]<Tpos)																			//___||>>___     moving
				{
				i = Tpos - prevTpos[dk] + 1;	
				if(REMAIN_ENABLE[dk])																		
					{					
					while(i>0)
						{
						if((prevTpos[dk]+i)>1)	
							{
							if((prevTpos[dk]+i)%2==0)
								{	
								BSP_LCD_DrawPixel(1+(prevTpos[dk]+i)+h, 223, PBAR_COLOR_2);	//drawing :::::::::::::::
								BSP_LCD_DrawPixel(1+(prevTpos[dk]+i)+h, 226, PBAR_COLOR_2);			
								}	
							else
								{	
								BSP_LCD_DrawPixel(1+(prevTpos[dk]+i)+h, 223, PBAR_COLOR_3);	//drawing :::::::::::::::
								BSP_LCD_DrawPixel(1+(prevTpos[dk]+i)+h, 226, PBAR_COLOR_3);			
								}	
							BSP_LCD_DrawPixel(1+(prevTpos[dk]+i)+h, 224, LCD_COLOR_BLACK);		
							BSP_LCD_DrawPixel(1+(prevTpos[dk]+i)+h, 225, LCD_COLOR_BLACK);
							}
						i--;					
						}
					if(Tpos>1)
						{
						ForceDrawVLine(3+h, 223, 4, PBAR_COLOR_2);						//gray scroll vertical line in start	
						}
					}
				else				
					{
					while(i>0)
						{
						if((prevTpos[dk]+i)>1)	
							{
							if((prevTpos[dk]+i)%2==0)
								{
								ForceDrawVLine((prevTpos[dk]+i)+1+h, 223, 4, LCD_COLOR_WHITE);	//drawing ||||||||||||||||||		
								}
							else
								{
								ForceDrawVLine((prevTpos[dk]+i)+1+h, 223, 4, PBAR_COLOR_1);	//drawing ||||||||||||||||||		
								}
							}
						i--;					
						}
					}
				}		
				
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);			
			BSP_LCD_DrawHLine(3+h, 227, 202);	
			if(prevfin[dk]>prevstrt[dk])
				{				
				BSP_LCD_SetTextColor(LCD_COLOR_RED);			
				BSP_LCD_DrawHLine(3+h+prevstrt[dk], 227, (prevfin[dk]-prevstrt[dk])+1);		
				}
			
			ForceDrawVLine(Tpos+2+h, 223, 5, LCD_COLOR_BLACK);
			ForceDrawVLine(Tpos+5+h, 223, 5, LCD_COLOR_BLACK);
				
			BSP_LCD_SetTextColor(CURSOR_COLOR[dk]);	
			BSP_LCD_DrawVLine(Tpos+3+h, 200, 29);		//white vertical scroll
			BSP_LCD_DrawVLine(Tpos+4+h, 200, 29);	
			prevTpos[dk] = Tpos;	
			}
		else if(forcibly_redraw[dk]==1)
			{				
			BSP_LCD_SetTextColor(CURSOR_COLOR[dk]);		
			BSP_LCD_DrawVLine(Tpos+3+h, 200, 29);		//white vertical scroll
			BSP_LCD_DrawVLine(Tpos+4+h, 200, 29);
			}
		return;	
		}
	else if(Tpos==REDRAW_IN_NREMAIN_MODE)
		{
		DRAWN_IN_REMAIN[dk] = 0;	
		for(i=0;i<202;i++)						
			{			
			if(i<prevTpos[dk]-1)
				{	
				DrawpxforBar(dk, h, i);					
				}	
			else if(i>prevTpos[dk]+2)
				{
				DrawpxforBar(dk, h, i);	
				}
			}
		if(prevTpos[dk]<199)
			{
			ForceDrawVLine(204+h, 223, 4, PBAR_COLOR_2);						//gray scroll vertical line in end	
			}		
		return;	
		}
	else if(Tpos==REDRAW_IN_REMAIN_MODE)
		{
		DRAWN_IN_REMAIN[dk] = 1;	
		for(i=0;i<202;i++)						
			{	
			if(i<prevTpos[dk]-1)
				{
				DrawpxforBar(dk, h, i);		
				}	
			else if(i>prevTpos[dk]+2)
				{
				DrawpxforBar(dk, h, i);					
				}	
			}
		if(prevTpos[dk]>1)
			{
			ForceDrawVLine(3+h, 223, 4, PBAR_COLOR_2);						//gray scroll vertical line in start					
			}	
		return;	
		}	
	else if(Tpos==DRAW_NEW_STATIC_WAVEFORM)						//Draw new static waveform		
		{
		prevTpos[dk] = 0;	
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		if(dk==dkA)
			{
			BSP_LCD_FillRect(0, 200, 209, 33);		
			}		
		else
			{
			BSP_LCD_FillRect(271, 200, 209, 33);
			}	
		for(i=0;i<202;i++)						
			{			
			DrawWFMSample(dk, i);		
			DrawpxforBar(dk, h, i);	
			}
		if(REMAIN_ENABLE[dk]==0)	
			{
			ForceDrawVLine(3+h, 223, 4, PBAR_COLOR_2);						//gray scroll vertical line in start	
			ForceDrawVLine(204+h, 223, 4, PBAR_COLOR_2);					//gray scroll vertical line in end	
			}
		}
	else if(Tpos==CLEAR_WAVEFORM_ARRAY)										//clear waveform array
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		if(dk==dkA)
			{
			BSP_LCD_FillRect(0, 193, 209, 40);		
			}		
		else
			{
			BSP_LCD_FillRect(271, 193, 209, 40);
			}			
		}
	else if(Tpos==MS_NOT_LOADED)
		{
		BSP_LCD_SetFont(&Font15P);	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "NOT LOADED");	
		BSP_LCD_DisplayStringAt(54+h, 206,Buf, LEFT_MODE);
		}
	else if(Tpos>MS_ERROR)													//Error message
		{
		BSP_LCD_SetFont(&Font15P);	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "ERROR");	
		BSP_LCD_DisplayStringAt(50+h, 206,Buf, LEFT_MODE);
		sprintf((char*)Buf, "%2lu", (Tpos-MS_ERROR));			//ERROR NUMBER
		BSP_LCD_DisplayStringAt(109+h, 206,Buf, LEFT_MODE);			
		}
	};
	
	
	
	
//////////////////////////////////////////////////
//
//	
//	Draw memory fill bar 0...201 (202 pixels wide)
//
void DrawMemBar(uint8_t dk, uint8_t strt, uint8_t fin)
	{	
	if(strt>=fin)
		{
		return;	
		}
	if(strt>201 || fin>201)
		{
		return;	
		}
	if(strt!=prevstrt[dk] || fin!=prevfin[dk])
		{
		uint8_t i;	
		uint16_t h;	
		if(dk==dkA)
			{
			h = 0;	
			}		
		else
			{
			h = 272;	
			}		
		if(strt>prevstrt[dk])		//move>>>
			{
			for(i=prevstrt[dk];i<strt;i++)
				{
				if((i+1)<prevTpos[dk] || (i>(prevTpos[dk]+2)))	
					{
					DrawpxMem(h, 0, i);
					}
				}				
			}
		else if(strt<prevstrt[dk])		//move<<<
			{
			for(i=strt;i<=prevstrt[dk];i++)
				{
				if((i+1)<prevTpos[dk] || (i>(prevTpos[dk]+2)))	
					{
					DrawpxMem(h, 1, i);
					}
				}				
			}		
			
		if(fin>prevfin[dk])		//move>>>
			{
			for(i=prevfin[dk];i<=fin;i++)
				{
				if((i+1)<prevTpos[dk] || (i>(prevTpos[dk]+2)))	
					{
					DrawpxMem(h, 1, i);
					}
				}				
			}
		else if(fin<prevfin[dk])		//move<<<
			{
			for(i=fin;i<prevfin[dk];i++)
				{
				if((i+1)<prevTpos[dk] || (i>(prevTpos[dk]+2)))	
					{
					DrawpxMem(h, 0, i);
					}
				}				
			}			
			
		prevstrt[dk] = strt;
		prevfin[dk] = fin; 
		}	
	return;	
	};
	
	
	
//////////////////////////////////////////////////
//
//	
//	
void DrawpxMem(uint16_t h, uint8_t col, uint8_t pos)
	{
	if(col==0)
		{
		BSP_LCD_DrawPixel(3+pos+h, 227, LCD_COLOR_BLACK);					
		}		
	else
		{
		BSP_LCD_DrawPixel(3+pos+h, 227, LCD_COLOR_RED);		
		}
	};	

//////////////////////////////////////////////////
//
//	
//
void DrawpxforBar(uint8_t dk, uint16_t h, uint8_t pos)
	{				
	if((DRAWN_IN_REMAIN[dk]==0 && (pos<prevTpos[dk])) || (DRAWN_IN_REMAIN[dk]==1 && (pos>prevTpos[dk])))	
		{
		if(pos%2==0)
			{
			ForceDrawVLine(pos+3+h, 223, 4, LCD_COLOR_WHITE);	//drawing ||||||||||||||||||		
			}
		else
			{
			ForceDrawVLine(pos+3+h, 223, 4, PBAR_COLOR_1);	//drawing ||||||||||||||||||		
			}			
		}
	else
		{
		if(pos%2==0)
			{	
			BSP_LCD_DrawPixel(3+pos+h, 223, PBAR_COLOR_2);	//drawing :::::::::::::::
			BSP_LCD_DrawPixel(3+pos+h, 226, PBAR_COLOR_2);			
			}	
		else
			{	
			BSP_LCD_DrawPixel(3+pos+h, 223, PBAR_COLOR_3);	//drawing :::::::::::::::
			BSP_LCD_DrawPixel(3+pos+h, 226, PBAR_COLOR_3);			
			}	
		BSP_LCD_DrawPixel(3+pos+h, 224, LCD_COLOR_BLACK);		
		BSP_LCD_DrawPixel(3+pos+h, 225, LCD_COLOR_BLACK);		
		}
	};

////////////////////////////////////////////////	
//Draw MEMORY red triangle on time bar	
// p=0xFF - disable all triangle	
// time position bar 0...202		
//#define NONE_MARK				0
//#define MEMORY_MARK			1
//#define HOT_CUE_MARK	 >1
void DrawMemoryMarker(uint8_t dk, uint8_t p, uint8_t type, uint16_t color)
	{
	uint16_t h;	
	if(dk==dkA)
		{
		h = 0;	
		}		
	else
		{
		h = 272;	
		}		
	if(type==NONE_MARK)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		if(dk==dkA)
			{
			BSP_LCD_FillRect(0, 193, 209, 7);	
			}		
		else
			{
			BSP_LCD_FillRect(271, 193, 209, 7);	
			}		
		return;
		}
	else if(p<202)
		{
		if(type>1)		//HOT CUEs
			{			
			uint8_t r;	
			BSP_LCD_SetTextColor(color); 	
			BSP_LCD_FillRect(p+h, 193, 7, 7);
			for(r=0;r<25;r++)
				{
				if((hcicons[type-2][r/5]>>(7-(r%5)))&0x01)
					{
					BSP_LCD_DrawPixel(p+h+(r%5)+1, 194+(r/5), LCD_COLOR_BLACK);	
					}
				}
			}
		else
			{
			DrawMTriangle(p+h, 196, LCD_COLOR_RED);
			}
		return;
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		if(dk==dkA)
			{
			BSP_LCD_FillRect(0, 193, 209, 7);	
			}		
		else
			{
			BSP_LCD_FillRect(271, 193, 209, 7);	
			}	
		}	
	return;	
	};		
		
	
//////////////////////////////////////////////	
//	
//	internal function for redraw waveform	
//		
void DrawMinuteMarkers(uint8_t dk)
	{	
	uint16_t h;	
	if(dk==dkA)
		{
		h = 0;	
		}		
	else
		{
		h = 272;	
		}			
	if(all_long[dk]<=135000) 			//15 min	
		{
		uint32_t a, npoint;	
		ForceDrawVLine(3+h, 229, 4, LCD_COLOR_WHITE);	
		if(all_long[dk]<=67500) 			//7,5 min		
			{
			npoint = all_long[dk]/4500;	
			while(npoint>0)
				{
				a = 911250*npoint;	
				a/= all_long[dk];			
					
				if(a<202)
					{	
					if(npoint%2==0)
						{
						ForceDrawVLine(3+a+h, 229, 4, LCD_COLOR_WHITE);
						}						
					else
						{
						ForceDrawVLine(3+a+h, 229, 3, LCD_COLOR_GRAY);		
						}
					}	
				npoint--;		
				}		
			}
		else
			{
			npoint = all_long[dk]/9000;	
			while(npoint>0)
				{
				a = 1822500*npoint;	
				a/= all_long[dk];			
				if(a<202)
					{
					ForceDrawVLine(3+a+h, 229, 4, LCD_COLOR_WHITE);	
					}	
				npoint--;		
				}				
			}			
		}
	return;	
	};	
	
//////////////////////////////////////////////	
//	
//	Draw Memory triangle
//		
void DrawMTriangle(uint16_t x, uint8_t y, uint16_t color)
	{
	BSP_LCD_SetTextColor(color);	
	BSP_LCD_DrawHLine(x, y, 7);	
	BSP_LCD_DrawHLine(x+1, y+1, 5);	
	BSP_LCD_DrawHLine(x+2, y+2, 3);	
	BSP_LCD_DrawPixel(x+3, y+3, color);	
	};
	
//////////////////////////////////////////////	
//	
//	Draw Cue triangle
//		
void DrawCTriangle(uint16_t x, uint8_t y, uint16_t color)
	{
	BSP_LCD_SetTextColor(color);	
	BSP_LCD_DrawHLine(x, y+3, 7);	
	BSP_LCD_DrawHLine(x+1, y+2, 5);	
	BSP_LCD_DrawHLine(x+2, y+1, 3);	
	BSP_LCD_DrawPixel(x+3, y, color);			
	};


//////////////////////////////////////////////	
//		
//Draw CUE orange triangle on time bar
//	Draw time position bar 0...201	
// 	p=0xFF - disable triangle 	
void DrawCueMarker(uint8_t dk, uint8_t p)
	{
	uint16_t h;	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
	if(dk==dkA)
		{
		h = 0;
		BSP_LCD_FillRect(0, 229, 209, 4);	
		}		
	else
		{
		h = 272;	
		BSP_LCD_FillRect(271, 229, 209, 4);	
		}				
	if(p>201)
		{
		p = 201;
		}	
	DrawCTriangle(p+h, 229, LCD_COLOR_CUE);		
	DrawMinuteMarkers(dk);	
	}

	
//////////////////////////////////////////////	
//		
//Show tempo func
//
//
void ShowTempo(uint8_t dk, uint16_t tempo)
	{
	uint8_t h;	
	if(dk==dkA)
		{
		h = 0;	
		}		
	else
		{
		h = 77;	
		}	
	uint8_t sbf[2]={0};
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font14D);
	if(tempo==10000)	
		{
		sprintf((char*)sbf, "%s", " ");			//NONE	
		}	
	else if(tempo>10000)
		{
		sprintf((char*)sbf, "%s", "+");			//PLUS	
		}
	else
		{
		sprintf((char*)sbf, "%s", "-");			//MINUS
		}	
	if(tempo>10000)	
		{
		tempo = tempo - 10000;	
		}
	else
		{
		tempo = 10000-tempo;	
		}	
	sprintf((char*)Buf, "%s", ".");			//dot for pitch
	
	if(tempo_range[dk]==3)							//wide range
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(198+h, 174, 2, 14);	
		BSP_LCD_FillRect(207+h, 174, 3, 14);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_DisplayStringAt(210+h, 174, Buf, LEFT_MODE);
		sprintf((char*)Buf, "%3lu", tempo/100);			//PITCH
		if(Buf[0]!=32)
			{
			BSP_LCD_DisplayChar(174+h, 174, Buf[0]);
			BSP_LCD_DisplayStringAt(169+h, 174, sbf, LEFT_MODE);		
			}	
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(169+h, 178, 6, 6);	
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			BSP_LCD_DisplayStringAt(175+h, 174, sbf, LEFT_MODE);
			}
		BSP_LCD_DisplayChar(186+h, 174, Buf[1]);	
		BSP_LCD_DisplayChar(198+h, 174, Buf[2]);		
		sprintf((char*)Buf, "%01lu", (tempo%100)/10);			//PITCH
		BSP_LCD_DisplayChar(215+h, 174, Buf[0]);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(210+h, 174, 2, 14);	
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_DisplayStringAt(198+h, 174,Buf, LEFT_MODE);
		sprintf((char*)Buf, "%2lu", tempo/100);			//PITCH
		if(Buf[0]!=32)
			{
			BSP_LCD_DisplayChar(174+h, 174, Buf[0]);
			BSP_LCD_DisplayStringAt(169+h, 174, sbf, LEFT_MODE);		
			}	
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(169+h, 178, 6, 6);	
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			BSP_LCD_DisplayStringAt(175+h, 174, sbf, LEFT_MODE);
			}
		BSP_LCD_DisplayChar(186+h, 174, Buf[1]);	
		sprintf((char*)Buf, "%02lu", tempo%100);			//PITCH
		BSP_LCD_DisplayChar(203+h, 174, Buf[0]);	
		BSP_LCD_DisplayChar(215+h, 174, Buf[1]);		
		}
	return;	
	};

	
//////////////////////////////////////////////	
//		
//show bpm and bpm shield
//
// input bpm*10
//	
void ShowBPM(uint8_t dk, uint16_t bpm)
	{		
	uint8_t h;	
	if(dk==dkA)
		{
		h = 0;	
		}		
	else
		{
		h = 53;	
		}		
	uint32_t clr;

	if(masterdeck==dk)
		{
		clr	= LCD_COLOR_CUE;	
		}
	else
		{		
		clr	= LCD_COLOR_WHITE;	
		}
		
	if(bpm!=0xFFFF)
		{
		bpm = bpm%10000; 	
		BSP_LCD_SetTextColor(clr);
		BSP_LCD_SetFont(&Font14D);
		sprintf((char*)Buf, "%3lu", bpm/10);			//BPM count
		BSP_LCD_DisplayChar(191+h, 145, Buf[0]);
		BSP_LCD_DisplayChar(203+h, 145, Buf[1]);
		BSP_LCD_DisplayChar(215+h, 145, Buf[2]);			
		BSP_LCD_DrawPixel(226+h, 157, clr);	//
		BSP_LCD_DrawPixel(226+h, 158, clr);	//	DOT
		BSP_LCD_DrawPixel(227+h, 157, clr);	//
		BSP_LCD_DrawPixel(227+h, 158, clr);	//
		BSP_LCD_SetFont(&Font9D);
		sprintf((char*)Buf, "%01lu", bpm%10);			//BPM count
		BSP_LCD_DisplayStringAt(230+h, 150, Buf, LEFT_MODE);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(191+h, 145, 44, 14);	
		BSP_LCD_SetTextColor(clr);		
		}
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(194+h, 162, 41, 5);	
	BSP_LCD_SetTextColor(clr);		
	BSP_LCD_SetFont(&FontBMP);
	if(masterdeck==dk)
		{
		sprintf((char*)Buf, "%s", ";<");						//MASTER
		BSP_LCD_DisplayStringAt(194+h, 162, Buf, TRANSPARENT_MODE);	
		}
	else
		{		
		sprintf((char*)Buf, "%s", "1");						//BPM
		BSP_LCD_DisplayStringAt(215+h, 162, Buf, TRANSPARENT_MODE);
		}		
	BSP_LCD_DrawLine(190+h, 141, 236+h, 141);					//BPM shield
	BSP_LCD_DrawLine(190+h, 169, 236+h, 169);
	BSP_LCD_DrawPixel(189+h, 142, clr);
	BSP_LCD_DrawPixel(237+h, 142, clr);
	BSP_LCD_DrawLine(188+h, 143, 188+h, 167);
	BSP_LCD_DrawLine(238+h, 143, 238+h, 167);
	BSP_LCD_DrawPixel(189+h, 168, clr);
	BSP_LCD_DrawPixel(237+h, 168, clr);		
	return;
	};

//////////////////////////////////////////////	
//		
//function drawing range display
// 0 - 6%			0
// 1 - 10%		1
// 2 - 16%		2
// 3 - WIDE		3
void DrawTempoRange(uint8_t dk, uint8_t Range)
	{
	uint8_t h;	
	if(dk==dkA)
		{
		h = 0;	
		}		
	else
		{
		h = 32;	
		}	
	if(Range==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
		}
	else if(Range==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);	
		}	
	else if(Range==2)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		}			
	BSP_LCD_FillRect(210+h, 224, 28, 9);
	BSP_LCD_DrawLine(209+h, 225, 209+h, 231);	
	BSP_LCD_DrawLine(238+h, 225, 238+h, 231);
	BSP_LCD_SetFont(&Font7W);
	if(Range==3)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		}
	if(Range==0)
		{
		sprintf((char*)Buf, "$");				
		BSP_LCD_DisplayStringAt(217+h, 225, Buf, TRANSPARENT_MODE);	
		sprintf((char*)Buf, "6");				
		BSP_LCD_DisplayStringAt(225+h, 225, Buf, TRANSPARENT_MODE);	
		}
	else if(Range==1)
		{
		sprintf((char*)Buf, "$");				
		BSP_LCD_DisplayStringAt(214+h, 225, Buf, TRANSPARENT_MODE);	
		sprintf((char*)Buf, "1");				
		BSP_LCD_DisplayStringAt(222+h, 225, Buf, TRANSPARENT_MODE);	
		sprintf((char*)Buf, "0");				
		BSP_LCD_DisplayStringAt(227+h, 225, Buf, TRANSPARENT_MODE);
		}	
	else if(Range==2)
		{
		sprintf((char*)Buf, "$");				
		BSP_LCD_DisplayStringAt(214+h, 225, Buf, TRANSPARENT_MODE);	
		sprintf((char*)Buf, "1");				
		BSP_LCD_DisplayStringAt(222+h, 225, Buf, TRANSPARENT_MODE);	
		sprintf((char*)Buf, "6");				
		BSP_LCD_DisplayStringAt(227+h, 225, Buf, TRANSPARENT_MODE);
		}	
	else
		{
		sprintf((char*)Buf, "W");				
		BSP_LCD_DisplayStringAt(211+h, 225, Buf, TRANSPARENT_MODE);	
		sprintf((char*)Buf, "I");				
		BSP_LCD_DisplayStringAt(221+h, 225, Buf, TRANSPARENT_MODE);				
		sprintf((char*)Buf, "D");				
		BSP_LCD_DisplayStringAt(225+h, 225, Buf, TRANSPARENT_MODE);			
		sprintf((char*)Buf, "E");				
		BSP_LCD_DisplayStringAt(233+h, 225, Buf, TRANSPARENT_MODE);	
		}
	};	
	
	
//////////////////////////////////////////////	
//		
//function drawing loop mode
// lp 0...12 or --  1/64....64  9=>8 beats
// act 0...1
//
void DrawLoopMode(uint8_t dk, uint8_t lp, uint8_t act)
	{		
	uint8_t h;	
	if(dk==dkA)
		{
		h = 0;	
		}		
	else
		{
		h = 32;	
		}	
	extern uint8_t TRANSPARENT_MODE_ENABLE;	
	if(act==0)
		{
		BSP_LCD_SetTextColor(LOOP_INACTIVELP_COLOR);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_CUE);	
		}		
	BSP_LCD_FillRect(210+h, 214, 28, 9);
	BSP_LCD_DrawLine(209+h, 215, 209+h, 221);	
	BSP_LCD_DrawLine(238+h, 215, 238+h, 221);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
	BSP_LCD_SetFont(&Font7W);
	TRANSPARENT_MODE_ENABLE = 1;	
	if(lp==0)
		{
		sprintf((char*)Buf, "1");		
		BSP_LCD_DisplayChar(212+h, 215, Buf[0]);	
		sprintf((char*)Buf, "/");
		BSP_LCD_DisplayChar(217+h, 215, Buf[0]);			
		sprintf((char*)Buf, "6");		
		BSP_LCD_DisplayChar(222+h, 215, Buf[0]);		
		sprintf((char*)Buf, "4");	
		BSP_LCD_DisplayChar(230+h, 215, Buf[0]);
		}		
	else if(lp==1)
		{
		sprintf((char*)Buf, "1");	
		BSP_LCD_DisplayChar(212+h, 215, Buf[0]);	
		sprintf((char*)Buf, "/");	
		BSP_LCD_DisplayChar(217+h, 215, Buf[0]);	
		sprintf((char*)Buf, "3");	
		BSP_LCD_DisplayChar(222+h, 215, Buf[0]);		
		sprintf((char*)Buf, "2");	
		BSP_LCD_DisplayChar(230+h, 215, Buf[0]);			
		}		
	else if(lp==2)
		{
		sprintf((char*)Buf, "1");
		BSP_LCD_DisplayChar(213+h, 215, Buf[0]);	
		sprintf((char*)Buf, "/");		
		BSP_LCD_DisplayChar(218+h, 215, Buf[0]);	
		sprintf((char*)Buf, "1");	
		BSP_LCD_DisplayChar(223+h, 215, Buf[0]);	
		sprintf((char*)Buf, "6");	
		BSP_LCD_DisplayChar(228+h, 215, Buf[0]);		
		}	
	else if(lp==3)
		{
		sprintf((char*)Buf, "1");	
		BSP_LCD_DisplayChar(216+h, 215, Buf[0]);	
		sprintf((char*)Buf, "/");
		BSP_LCD_DisplayChar(221+h, 215, Buf[0]);	
		sprintf((char*)Buf, "8");			
		BSP_LCD_DisplayChar(226+h, 215, Buf[0]);	
		}	
	else if(lp==4)
		{
		sprintf((char*)Buf, "1");	
		BSP_LCD_DisplayChar(216+h, 215, Buf[0]);	
		sprintf((char*)Buf, "/");	
		BSP_LCD_DisplayChar(221+h, 215, Buf[0]);	
		sprintf((char*)Buf, "4");	
		BSP_LCD_DisplayChar(226+h, 215, Buf[0]);	
		}	
	else if(lp==5)
		{
		sprintf((char*)Buf, "1");				
		BSP_LCD_DisplayChar(216+h, 215, Buf[0]);	
		sprintf((char*)Buf, "/");		
		BSP_LCD_DisplayChar(221+h, 215, Buf[0]);	
		sprintf((char*)Buf, "2");		
		BSP_LCD_DisplayChar(226+h, 215, Buf[0]);	
		}	
	else if(lp==6)
		{
		sprintf((char*)Buf, "1");		
		BSP_LCD_DisplayChar(222+h, 215, Buf[0]);		
		}	
	else if(lp==7)
		{
		sprintf((char*)Buf, "2");	
		BSP_LCD_DisplayChar(221+h, 215, Buf[0]);		
		}	
	else if(lp==8)
		{
		sprintf((char*)Buf, "4");	
		BSP_LCD_DisplayChar(221+h, 215, Buf[0]);		
		}	
	else if(lp==9)
		{
		sprintf((char*)Buf, "8");
		BSP_LCD_DisplayChar(221+h, 215, Buf[0]);
		}	
	else if(lp==10)
		{
		sprintf((char*)Buf, "1");
		BSP_LCD_DisplayChar(218+h, 215, Buf[0]);		
		sprintf((char*)Buf, "6");		
		BSP_LCD_DisplayChar(223+h, 215, Buf[0]);	
		}	
	else if(lp==11)
		{
		sprintf((char*)Buf, "3");				
		BSP_LCD_DisplayChar(217+h, 215, Buf[0]);		
		sprintf((char*)Buf, "2");				
		BSP_LCD_DisplayChar(225+h, 215, Buf[0]);	
		}	
	else if(lp==12)
		{
		sprintf((char*)Buf, "6");		
		BSP_LCD_DisplayChar(217+h, 215, Buf[0]);		
		sprintf((char*)Buf, "4");		
		BSP_LCD_DisplayChar(225+h, 215, Buf[0]);	
		}				
	else
		{
		sprintf((char*)Buf, "-");			
		BSP_LCD_DisplayChar(217+h, 215, Buf[0]);
		BSP_LCD_DisplayChar(225+h, 215, Buf[0]);	
		}
	};
	
	
///////////////////////////
//	
//	show QUANTIZE on display
//	0 - none 1 - red
//
//	
void ShowQUANTIZE(uint8_t color)
	{
	BSP_LCD_SetFont(&FontBMP);
	if(color==0)
		{
		BSP_LCD_SetTextColor(LOOP_INACTIVELP_COLOR);	
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		}	
	sprintf((char*)Buf, "%s", " !");						//QUANTIZE	
	BSP_LCD_DisplayStringAt(209, 192, Buf, TRANSPARENT_MODE);	
	};

///////////////////////////	
//show A.CUE on display
//	0 - none	
//	1 - red
//	2 - white	
//	
void ShowACUE(uint8_t acue)
	{	
	BSP_LCD_SetFont(&FontBMP);	
	if(acue==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);	
		}	
	else if(acue==2)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}
	else
		{
		BSP_LCD_SetTextColor(LOOP_INACTIVELP_COLOR);
		}
	sprintf((char*)Buf, "%s", "#$");						//A.CUE
	BSP_LCD_DisplayStringAt(241, 192,Buf, TRANSPARENT_MODE);
	};


///////////////////////////	
//show battery
//	0...10	
//	
void ShowBATT(uint8_t lvl)
	{
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
	BSP_LCD_FillRect(464, 6, 10, 4);	
	if(lvl>10)
		{
		return;	
		}	
	if(lvl==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_DrawHLine(463, 4, 12);
		BSP_LCD_DrawHLine(463, 11, 12);	
		BSP_LCD_DrawVLine(462, 5, 6);	
		BSP_LCD_DrawVLine(475, 5, 6);	
		BSP_LCD_DrawPixel(476, 6, LCD_COLOR_RED);	
		BSP_LCD_DrawPixel(476, 9, LCD_COLOR_RED);	
		BSP_LCD_DrawPixel(477, 7, LCD_COLOR_RED);		
		BSP_LCD_DrawPixel(477, 8, LCD_COLOR_RED);	
		BSP_LCD_DrawPixel(477, 6, BATT_REDR);				
		BSP_LCD_DrawPixel(477, 9, BATT_REDR);	
		BSP_LCD_DrawPixel(462, 4, BATT_REDL);
		BSP_LCD_DrawPixel(475, 4, BATT_REDL);
		BSP_LCD_DrawPixel(462, 11, BATT_REDL);
		BSP_LCD_DrawPixel(475, 11, BATT_REDL);			
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_DrawHLine(463, 4, 12);
		BSP_LCD_DrawHLine(463, 11, 12);	
		BSP_LCD_DrawVLine(462, 5, 6);	
		BSP_LCD_DrawVLine(475, 5, 6);	
		BSP_LCD_DrawPixel(476, 6, LCD_COLOR_WHITE);	
		BSP_LCD_DrawPixel(476, 9, LCD_COLOR_WHITE);	
		BSP_LCD_DrawPixel(477, 7, LCD_COLOR_WHITE);		
		BSP_LCD_DrawPixel(477, 8, LCD_COLOR_WHITE);	
		BSP_LCD_DrawPixel(477, 6, BATT_GRAYR);				
		BSP_LCD_DrawPixel(477, 9, BATT_GRAYR);	
		BSP_LCD_DrawPixel(462, 4, BATT_GRAYL);
		BSP_LCD_DrawPixel(475, 4, BATT_GRAYL);
		BSP_LCD_DrawPixel(462, 11, BATT_GRAYL);
		BSP_LCD_DrawPixel(475, 11, BATT_GRAYL);	
		if(lvl==1)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_RED);
			BSP_LCD_DrawVLine(464, 6, 4);	
			}
		else if(lvl==2)
			{
			BSP_LCD_SetTextColor(BATT_YELLOW);
			BSP_LCD_DrawVLine(464, 6, 4);	
			BSP_LCD_DrawVLine(465, 6, 4);	
			}	
		else
			{
			BSP_LCD_SetTextColor(BATT_GREEN);	
			BSP_LCD_FillRect(464, 6, lvl, 4);	
			}
		}
	};
	
	
///////////////////////////	
//show icon AC
//	0 - none
//	1 - charge
//	2 - Full (power socket)
//		
void ShowAC(uint8_t tp)	
	{
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
	BSP_LCD_FillRect(452, 3, 7, 9);
	if(tp==1)		//lightning
		{
		BSP_LCD_SetTextColor(LCD_COLOR_CUE);	
		BSP_LCD_DrawHLine(453, 7, 5);		
		BSP_LCD_DrawPixel(454, 6, LCD_COLOR_CUE);	
		BSP_LCD_DrawPixel(455, 6, LCD_COLOR_CUE);	
		BSP_LCD_DrawPixel(455, 5, LCD_COLOR_CUE);	
		BSP_LCD_DrawPixel(455, 8, LCD_COLOR_CUE);	
		BSP_LCD_DrawPixel(456, 8, LCD_COLOR_CUE);		
		BSP_LCD_DrawPixel(455, 9, LCD_COLOR_CUE);		
		BSP_LCD_DrawPixel(456, 4, LI_COLOR_3);		
		BSP_LCD_DrawPixel(454, 10, LI_COLOR_3);		
		BSP_LCD_DrawPixel(456, 5, LI_COLOR_2);		
		BSP_LCD_DrawPixel(454, 9, LI_COLOR_2);	
		BSP_LCD_DrawPixel(453, 8, LI_COLOR_1);	
		BSP_LCD_DrawPixel(454, 8, LI_COLOR_1);
		BSP_LCD_DrawPixel(456, 6, LI_COLOR_1);	
		BSP_LCD_DrawPixel(457, 6, LI_COLOR_1);
		BSP_LCD_DrawPixel(457, 3, LI_COLOR_1);	
		BSP_LCD_DrawPixel(453, 11, LI_COLOR_1);
		}
	else if(tp==2)		//socket
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
		BSP_LCD_FillRect(453, 6, 5, 3);	
		BSP_LCD_DrawPixel(453, 4, LCD_COLOR_WHITE);	
		BSP_LCD_DrawPixel(453, 5, LCD_COLOR_WHITE);	
		BSP_LCD_DrawPixel(457, 4, LCD_COLOR_WHITE);	
		BSP_LCD_DrawPixel(457, 5, LCD_COLOR_WHITE);	
		BSP_LCD_DrawPixel(454, 9, LCD_COLOR_WHITE);	
		BSP_LCD_DrawPixel(455, 9, LCD_COLOR_WHITE);
		BSP_LCD_DrawPixel(456, 9, LCD_COLOR_WHITE);	
		BSP_LCD_DrawPixel(455, 10, LCD_COLOR_WHITE);
		BSP_LCD_DrawPixel(455, 11, LCD_COLOR_WHITE);			
		BSP_LCD_DrawPixel(452, 6, BATT_GRAYL);	
		BSP_LCD_DrawPixel(458, 6, BATT_GRAYL);	
		BSP_LCD_DrawPixel(453, 9, SOCK_GRAY);		
		BSP_LCD_DrawPixel(457, 9, SOCK_GRAY);		
		}
	};

	
///////////////////////////	
// calculate color dimming
//
//
//
//		
uint16_t color_dim(uint16_t dim, uint16_t mx,  uint16_t col)
	{
	uint8_t r,g,b;	
	uint16_t tmp;
	r = (col&0x7FFF)>>10;		
	g = (col&0x03FF)>>5;
	b = col&0x001F;
	tmp = r*dim;	
	r = tmp/mx;	
	tmp = g*dim;	
	g = tmp/mx;	
	tmp = b*dim;	
	b = tmp/mx;			
	return 0x8000 + (r<<10) + (g<<5) + b;	
	};	
	
	
	
///////////////////////////////////////////		
//	show track name and artist string
//
//
//		
void ShowTrackName(uint8_t dk)
	{
	uint16_t j, px;	
	uint32_t c, h;
	uint8_t r,g,b;		
	if(dk==dkA)
		{
		for(j=0;j<3196;j++)
			{		
			h = 460*(j%188);
			h = h/187;
			px = h +(480*(j/188));
			c = (COLOR_MAP_TRACKBAR[curr_trck_color[dk]]&0x7FFF)>>10;
			c*= panelbar[px];	
			c+=128;	
			c>>=8;	
			r = c;
			c = (COLOR_MAP_TRACKBAR[curr_trck_color[dk]]&0x03FF)>>5; 	
			c*= panelbar[px];
			c+=128;	
			c>>=8;	
			g = c;
			c = COLOR_MAP_TRACKBAR[curr_trck_color[dk]]&0x001F; 	
			c*= panelbar[px];
			c+=128;
			c>>=8;	
			b = c;	
			BSP_LCD_DrawPixel(j%188, 140+(j/188), 0x8000+(r<<10)+(g<<5)+b);			
			}
		}
	else
		{		
		for(j=0;j<3196;j++)
			{		
			h = 460*(j%188);
			h = h/187;
			px = h +(480*(j/188));
			c = (COLOR_MAP_TRACKBAR[curr_trck_color[dk]]&0x7FFF)>>10;
			c*= panelbar[px];	
			c+=128;	
			c>>=8;	
			r = c;
			c = (COLOR_MAP_TRACKBAR[curr_trck_color[dk]]&0x03FF)>>5; 	
			c*= panelbar[px];
			c+=128;	
			c>>=8;	
			g = c;
			c = COLOR_MAP_TRACKBAR[curr_trck_color[dk]]&0x001F; 	
			c*= panelbar[px];
			c+=128;
			c>>=8;	
			b = c;
			BSP_LCD_DrawPixel(479-(j%188), 140+(j/188), 0x8000+(r<<10)+(g<<5)+b);			
			}			
		}
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(0x9CE7);		
	if(dk==dkA)
		{
		BSP_LCD_FillRect(24, 123, 216, 17);
		//ForceDrawVLine(25, 123, 17, LCD_COLOR_DDARKGRAY);	
		BSP_LCD_SetTextColor(COLOR_MAP_TRACKBAR[curr_trck_color[dk]]);
		BSP_LCD_DisplayStringAt(28, 124, artist_n[dk], TRANSPARENT_MODE);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
		BSP_LCD_DisplayStringAt(3, 141, track_n[dk], TRANSPARENT_MODE);		
		}
	else
		{
		BSP_LCD_FillRect(264, 123, 216, 17);
		//ForceDrawVLine(265, 123, 17, LCD_COLOR_DDARKGRAY);		
		BSP_LCD_SetTextColor(COLOR_MAP_TRACKBAR[curr_trck_color[dk]]);	
		BSP_LCD_DisplayStringAt(268, 124, artist_n[dk], TRANSPARENT_MODE);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
		BSP_LCD_DisplayStringAt(295, 141, track_n[dk], TRANSPARENT_MODE);			
		}	
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);	
	if(dk==dkA)
		{
		BSP_LCD_FillRect(0, 123, 24, 17);				
//		ForceDrawVLine(0, 123, 17, LCD_COLOR_GGRAY);		
		}
	else
		{		
		BSP_LCD_FillRect(240, 123, 24, 17);	
	//	ForceDrawVLine(240, 123, 17, LCD_COLOR_GGRAY);		
		}
	return;	
	};		
	
///////////////////////////////////////////		
// parser track name to artist and tr. name string
// num - track number in playlist array
// dk - decka	
//	
void trackname_parser(uint8_t dk, uint16_t num)
	{
	uint8_t	i, art, trn, w;
	for(i=0;i<32;i++)		//clear arrays
		{
		artist_n[dk][i] = 0;	
		track_n[dk][i] = 0;	
		}	
	for(i=2;i<53;i++)
		{
		if(playlist[num][i]==45)
			{
			if(playlist[num][i-1]==32 && playlist[num][i+1]==32)
				{
				art = i-2;
				trn = i+2;	
				i = 100;	
				}
			}
		}		
	if(i==100)
		{
		for(i=0;i<32;i++)
			{
			artist_n[dk][i] = playlist[num][i];
			}
		}
	else
		{
		for(i=0;(i<(art+1) && (i<32));i++)
			{
			artist_n[dk][i] = playlist[num][i];
			}	
		for(i=0;(i<(54-trn) && (i<32));i++)
			{	
			track_n[dk][i] = playlist[num][i+trn];
			}	
		}
	w = 0;
	for(i=0;i<32;i++)				//trimming the length of a string
		{
		if(artist_n[dk][i]>31)
			{			
			w+=width15P[artist_n[dk][i]-32]+2;
			}
		if(w>213)
			{
			artist_n[dk][i] = 0;	
			}			
		}	
	w = 0;
	for(i=0;i<32;i++)				//trimming the length of a string
		{
		if(track_n[dk][i]>31)
			{			
			w+=width15P[track_n[dk][i]-32]+2;
			}
		if(w>186)
			{
			track_n[dk][i] = 0;	
			}			
		}		
	return;	
	};	
	
	
	
//////////////////////////////////////////////////////////	
//Switch Dynamic Waveform/Browser
//
//	
void SwitchInformationLayer(uint8_t LAY)
	{
	uint8_t update_all_page = 0;
	if(dSHOW!=LAY)
		{
		dSHOW = LAY;
		update_all_page = 1;	
		}	
	uint16_t E;	
	if(LAY==WAVEFORM)		//X => WAVEFORM
		{
		HAL_GPIO_WritePin(LED_BROWSE_GPIO_Port, LED_BROWSE_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOH, LED_TAGLIST_Pin|LED_INFO_Pin|LED_MENU_Pin, GPIO_PIN_RESET);
		BSP_LCD_SelectLayer(1);
		BSP_LCD_SetTransparency(1, 0);	
		BSP_LCD_Clear(0x0000);
		BSP_LCD_SelectLayer(0);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		BSP_LCD_FillRect(0, 18, 480, 172);	
		int_DRAW_TRANSPARENT_BAR(LCD_COLOR_BR);
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		sprintf((char*)Buf, "%s", "/");			
		BSP_LCD_DisplayStringAt(4, 1, Buf, TRANSPARENT_MODE);				
		BSP_LCD_DisplayStringAt(21, 1, SDCARD_NAME, TRANSPARENT_MODE);		
		DrawPhasebarStatic();		
		prev_phase_pos = 77;	
		ShowTrackName(dkA);		
		ShowTrackName(dkB);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		sprintf((char *)Buf , "%02lu", TRACK_n_PLAY[dkA]%100);			
		BSP_LCD_DisplayStringAt(4, 124, Buf, TRANSPARENT_MODE);
		sprintf((char *)Buf , "%02lu", TRACK_n_PLAY[dkB]%100);		
		BSP_LCD_DisplayStringAt(244, 124, Buf, TRANSPARENT_MODE);
		DrawStaticTime(dkA);
		DrawStaticTime(dkB);
		tempo_need_update[dkA] = 1;
		tempo_need_update[dkB] = 1;
		if(TRACK_n_PLAY[dkA]==0 && TRACK_n_PLAY[dkB]==0)
			{
			DrawREKORDBOX();
			Prev10m[dkA] = 0xFF;	
			Prev1m[dkA] = 0xFF;
			Prev10s[dkA] = 0xFF;
			Prev1s[dkA] = 0xFF;
			Prev10f[dkA] = 0xFF;
			Prev1f[dkA] = 0xFF;
			PrevHf[dkA] = 0xFF;
			Prev10m[dkB] = 0xFF;	
			Prev1m[dkB] = 0xFF;
			Prev10s[dkB] = 0xFF;
			Prev1s[dkB] = 0xFF;
			Prev10f[dkB] = 0xFF;
			Prev1f[dkB] = 0xFF;
			PrevHf[dkB] = 0xFF;		
			}
		else
			{
			forcibly_redraw[dkA] = 1;	
			forcibly_redraw[dkB] = 1;		
			}			
		return;
		}
	else if(LAY==BROWSER)													//X => BROWSER
		{
		if(update_all_page==1)
			{
			HAL_GPIO_WritePin(LED_BROWSE_GPIO_Port, LED_BROWSE_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOH, LED_TAGLIST_Pin|LED_INFO_Pin|LED_MENU_Pin, GPIO_PIN_RESET);	
			BSP_LCD_SetTransparency(1, 0);	
			BSP_LCD_SelectLayer(1);
			BSP_LCD_Clear(0x0000);
			BSP_LCD_SelectLayer(0);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(0, 19, 12, 169);	
			int_DRAW_TRANSPARENT_BAR(LCD_COLOR_BR);	
			intDrawLayer0_NOINFO(B0CursorPos);	
			BSP_LCD_SelectLayer(1);	
			BSP_LCD_SetTransparency(1, 255);		
			BSP_LCD_SetFont(&Font15P);
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			sprintf((char*)Buf, "%s", "|");		
			BSP_LCD_DisplayStringAt(4, 1,Buf, TRANSPARENT_MODE);				
			sprintf((char*)Buf, "%s", TRACKLIST_NAME[B1CursorPos+BPlaylistPos-1]);						
			BSP_LCD_DisplayStringAt(21, 1, Buf, TRANSPARENT_MODE);
			sprintf((char*)Buf, "%s", "Total Track");						
			BSP_LCD_DisplayStringAt(320, 1,Buf, TRANSPARENT_MODE);
			sprintf((char *)Buf , "%1lu", TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos] - TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]);					
			BSP_LCD_DisplayStringAt(418, 1,Buf, TRANSPARENT_MODE);		
			}
		else
			{
			BSP_LCD_SelectLayer(1);		
			BSP_LCD_SetTextColor(0x0000);	
			BSP_LCD_FillRect(50, 20+(19*B0CursorPos), 15, 19);		
			}		
		int_B_DRAW_ALL_LINES();	
		return;
		}
	else if(LAY==TAG_LIST)													//X => TAG LIST
		{			
		HAL_GPIO_WritePin(LED_BROWSE_GPIO_Port, LED_BROWSE_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOH, LED_INFO_Pin|LED_MENU_Pin, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(GPIOH, LED_TAGLIST_Pin, GPIO_PIN_SET);				
		BSP_LCD_SetTransparency(1, 0);	
		BSP_LCD_SelectLayer(1);
		BSP_LCD_Clear(0x0000);	
		BSP_LCD_SelectLayer(0);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(0, 19, 12, 169);		
		int_DRAW_TRANSPARENT_BAR(LCD_COLOR_TG);
		intDrawLayer0_NOINFO(TCursorPos);
		BSP_LCD_SelectLayer(1);	
		BSP_LCD_SetTransparency(1, 255);		
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		sprintf((char*)Buf, "%s", "TAG LIST");			
		BSP_LCD_DisplayStringAt(4, 1,Buf, TRANSPARENT_MODE);
		sprintf((char*)Buf, "%s", "Total Track");						
		BSP_LCD_DisplayStringAt(320, 1,Buf, TRANSPARENT_MODE);
		sprintf((char *)Buf , "%01lu", TOTAL_TRACKS_IN_TAGLIST);					
		BSP_LCD_DisplayStringAt(418, 1,Buf, TRANSPARENT_MODE);	
		int_T_DRAW_ALL_LINES();	
		return;	
		}
	else if(LAY==BROWSER_INFO)													//X => BROWSER_INFO
		{
		if(BROWSE_LEVEL==0)	
			{
			if(update_all_page==1)
				{	
				HAL_GPIO_WritePin(LED_BROWSE_GPIO_Port, LED_BROWSE_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOH, LED_INFO_Pin, GPIO_PIN_SET);					
				HAL_GPIO_WritePin(GPIOH, LED_TAGLIST_Pin|LED_MENU_Pin, GPIO_PIN_RESET);				
				BSP_LCD_SetTransparency(1, 0);	
				BSP_LCD_SelectLayer(1);
				BSP_LCD_Clear(0x0000);	
				BSP_LCD_SelectLayer(0);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
				BSP_LCD_FillRect(0, 19, 12, 169);		
				int_DRAW_TRANSPARENT_BAR(LCD_COLOR_BR);	
				intDrawLayer0_INFO(B0CursorPos);		
				BSP_LCD_SelectLayer(1);	
				BSP_LCD_SetTransparency(1, 255);		
				BSP_LCD_SetFont(&Font15P);
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);					
				sprintf((char*)Buf, "%s", "|");		
				BSP_LCD_DisplayStringAt(4, 1,Buf, TRANSPARENT_MODE);				
				sprintf((char*)Buf, "%s", TRACKLIST_NAME[B1CursorPos+BPlaylistPos-1]);						
				BSP_LCD_DisplayStringAt(21, 1, Buf, TRANSPARENT_MODE);
				sprintf((char*)Buf, "%s", "Total Track");						
				BSP_LCD_DisplayStringAt(320, 1,Buf, TRANSPARENT_MODE);
				sprintf((char *)Buf , "%1lu", TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos] - TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]);					
				BSP_LCD_DisplayStringAt(418, 1,Buf, TRANSPARENT_MODE);
				}
			else
				{
				BSP_LCD_SelectLayer(1);		
				BSP_LCD_SetTextColor(0x0000);	
				BSP_LCD_FillRect(20, 20+(19*B0CursorPos), 15, 19);		
				}
			int_BIx_DRAW_ALL_LINES(0);	
			}
		else		
			{
			HAL_GPIO_WritePin(LED_BROWSE_GPIO_Port, LED_BROWSE_Pin, GPIO_PIN_SET);				
			HAL_GPIO_WritePin(GPIOH, LED_INFO_Pin|LED_TAGLIST_Pin|LED_MENU_Pin, GPIO_PIN_RESET);				
			BSP_LCD_SetTransparency(1, 0);	
			BSP_LCD_SelectLayer(1);
			BSP_LCD_Clear(0x0000);	
			BSP_LCD_SelectLayer(0);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(0, 18, 480, 172);		
			int_DRAW_TRANSPARENT_BAR(LCD_COLOR_BR);
			
			if(BROWSE_LEVEL==1)	
				{
				intDrawLayer0_BROWSER_1_3(B1CursorPos);	
				BSP_LCD_SelectLayer(1);	
				BSP_LCD_SetTransparency(1, 255);		
				BSP_LCD_SetFont(&Font15P);
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				sprintf((char*)Buf, "%s", "[PLAYLIST]");						
				BSP_LCD_DisplayStringAt(4, 1,Buf, TRANSPARENT_MODE);
				}
			else if(BROWSE_LEVEL==2)	
				{
				intDrawLayer0_BROWSER_1_3(B2CursorPos);	
				BSP_LCD_SelectLayer(1);	
				BSP_LCD_SetTransparency(1, 255);		
				BSP_LCD_SetFont(&Font15P);
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				sprintf((char*)Buf, "%s", "/");		
				BSP_LCD_DisplayStringAt(4,1,Buf, TRANSPARENT_MODE);				
				sprintf((char*)Buf, "%s", "SD card");						
				BSP_LCD_DisplayStringAt(21, 1, Buf, TRANSPARENT_MODE);					
				}
			else if(BROWSE_LEVEL==3)	
				{
				intDrawLayer0_BROWSER_1_3(0);
				BSP_LCD_SelectLayer(1);	
				BSP_LCD_SetTransparency(1, 255);		
				BSP_LCD_SetFont(&Font15P);
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				sprintf((char*)Buf, "%s", "SD card slot");						
				BSP_LCD_DisplayStringAt(4, 1,Buf, TRANSPARENT_MODE);
				}	
			int_BIx_DRAW_ALL_LINES(BROWSE_LEVEL);	
			}
		return;
		}
	else if(LAY==TAG_LIST_INFO)													//X => TAG LIST INFO
		{
		HAL_GPIO_WritePin(LED_BROWSE_GPIO_Port, LED_BROWSE_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOH, LED_MENU_Pin, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(GPIOH, LED_TAGLIST_Pin|LED_INFO_Pin, GPIO_PIN_SET);				
		BSP_LCD_SetTransparency(1, 0);	
		BSP_LCD_SelectLayer(1);
		BSP_LCD_Clear(0x0000);	
		BSP_LCD_SelectLayer(0);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		BSP_LCD_FillRect(0, 19, 12, 169);	
		int_DRAW_TRANSPARENT_BAR(LCD_COLOR_TG);
		intDrawLayer0_INFO(TCursorPos);
		BSP_LCD_SelectLayer(1);	
		BSP_LCD_SetTransparency(1, 255);		
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		sprintf((char*)Buf, "%s", "TAG LIST");			
		BSP_LCD_DisplayStringAt(4, 1,Buf, TRANSPARENT_MODE);
		sprintf((char*)Buf, "%s", "Total Track");						
		BSP_LCD_DisplayStringAt(320, 1,Buf, TRANSPARENT_MODE);
		sprintf((char *)Buf , "%01lu", TOTAL_TRACKS_IN_TAGLIST);					
		BSP_LCD_DisplayStringAt(418, 1,Buf, TRANSPARENT_MODE);	
		int_TI_DRAW_ALL_LINES();		
		return;	
		}
	else if(LAY==UTILITY)															//X => UTILITY
		{			
		if(update_all_page)
			{
			HAL_GPIO_WritePin(LED_BROWSE_GPIO_Port, LED_BROWSE_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOH, LED_MENU_Pin, GPIO_PIN_SET);	
			HAL_GPIO_WritePin(GPIOH, LED_TAGLIST_Pin|LED_INFO_Pin, GPIO_PIN_RESET);		
			}	
		BSP_LCD_SetTransparency(1, 0);	
		BSP_LCD_SelectLayer(1);	
		if(update_all_page)
			{
			BSP_LCD_Clear(0x0000);	
			}	
		BSP_LCD_SelectLayer(0);
		if(pm_urx_buf[1]<101)
			{
			u_battery_level = pm_urx_buf[1];	
			}	
		else
			{
			u_battery_level = 10*batt_prcnt;	
			}		
		if(update_all_page)	
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(0, 18, 480, 172);
			int_DRAW_TRANSPARENT_BAR(LCD_COLOR_UT);
			BSP_LCD_SetTextColor(LCD_COLOR_BGUTILITY);
			BSP_LCD_FillRect(12, 18, 468, 152);	
			BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
			BSP_LCD_DrawLine(0, 18, 479, 18);
			BSP_LCD_DrawLine(12, 37, 479, 37);
			BSP_LCD_DrawLine(12, 56, 479, 56);
			BSP_LCD_DrawLine(12, 75, 479, 75);
			BSP_LCD_DrawLine(12, 94, 479, 94);
			BSP_LCD_DrawLine(12, 113, 479, 113);
			BSP_LCD_DrawLine(12, 132, 479, 132);
			BSP_LCD_DrawLine(12, 151, 479, 151);
			BSP_LCD_DrawLine(12, 170, 479, 170);
			BSP_LCD_DrawLine(0, 189, 479, 189);	
			BSP_LCD_DrawLine(12, 18, 12, 188);			//vertical line	
			BSP_LCD_DrawLine(270,18,270,188);				//vertical line		
			BSP_LCD_DrawLine(479,18,479,188);				//vertical line
			}
		if(edit_parameter==0)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
			BSP_LCD_FillRect(12, (18+(19*UCurrentCursorPosition)), 258, 9);			////Draw selected cursor	
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
			BSP_LCD_FillRect(12, (27+(19*UCurrentCursorPosition)), 258, 5);
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
			BSP_LCD_FillRect(12, (32+(19*UCurrentCursorPosition)), 258, 5);
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_3);		
			BSP_LCD_FillRect(271, (18+(19*UCurrentCursorPosition)), 208, 9);			////Draw selected cursor	
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_5);
			BSP_LCD_FillRect(271, (27+(19*UCurrentCursorPosition)), 208, 5);
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_4);
			BSP_LCD_FillRect(271, (32+(19*UCurrentCursorPosition)), 208, 5);				
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			FillTriangle(20, 20, 27, 20+(19*UCurrentCursorPosition), 34+(19*UCurrentCursorPosition), 27 +(19*UCurrentCursorPosition));	
			}
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_3);		
			BSP_LCD_FillRect(12, (18+(19*UCurrentCursorPosition)), 258, 9);			////Draw selected cursor	
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_5);
			BSP_LCD_FillRect(12, (27+(19*UCurrentCursorPosition)), 258, 5);
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_4);
			BSP_LCD_FillRect(12, (32+(19*UCurrentCursorPosition)), 258, 5);
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);				
			BSP_LCD_FillRect(271, (18+(19*UCurrentCursorPosition)), 208, 9);			////Draw selected cursor	
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
			BSP_LCD_FillRect(271, (27+(19*UCurrentCursorPosition)), 208, 5);
			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
			BSP_LCD_FillRect(271, (32+(19*UCurrentCursorPosition)), 208, 5);				
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			FillTriangle(276, 276, 283, 20+(19*UCurrentCursorPosition), 34+(19*UCurrentCursorPosition), 27 +(19*UCurrentCursorPosition));	
			}		
		BSP_LCD_SelectLayer(1);	
		BSP_LCD_SetTransparency(1, 255);
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		if(update_all_page)
			{	
			sprintf((char*)Buf, "%s", "UTILITY");			
			BSP_LCD_DisplayStringAt(4, 1, Buf, TRANSPARENT_MODE);
			}
		int_U_DRAW_ALL_LINES();	
		return;	
		}
	else if(LAY==BROWSER_NAVI)		//X => BROWSER NAVIGATION WITH ANIMATION
		{
		CURRENT_LAY = ActiveLayer;		
		BSP_LCD_SetTransparency(1, 0);	
		if(ActiveLayer!=1)
			{
			BSP_LCD_SelectLayer(1);
			}	
		BSP_LCD_Clear(0x0000);
			
		if(PREVIOUS_BROWSE_LEVEL<BROWSE_LEVEL)				//reverse  >> push back
			{
			if(BROWSE_LEVEL==1 && BROWSER_INFO_enable==0)	
				{
				info_animation_enable = 0;			
				}
			else
				{
				BSP_LCD_SetTextColor(LCD_COLOR_PAPER_TRANSP);	
				if(BROWSE_LEVEL==1)
					{
					BSP_LCD_FillRect(361, 18, 119, 171);		
					info_animation_enable = 2;	
					}
				else
					{
					BSP_LCD_FillRect(270, 18, 210, 171);		
					info_animation_enable = 1;
					}
				}
			animation_en = 2;		
			}
		else																				//forward   <<
			{
			if(BROWSE_LEVEL==0 && BROWSER_INFO_enable==0)	
				{
				info_animation_enable = 0;		
				}
			else if(BROWSE_LEVEL==0 && BROWSER_INFO_enable==1)	
				{	
				info_animation_enable = 2;		
				}
			else
				{	
				info_animation_enable = 1;		
				}	
			BSP_LCD_SetTextColor(LCD_COLOR_PAPER_TRANSP);		
			BSP_LCD_FillRect(270, 18, 210, 171);		
			animation_en = 1;	
			}	
		BSP_LCD_SetTransparency(1, 255);
		animation_time = HAL_GetTick();	
		BSP_LCD_SelectLayer(0);
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		BSP_LCD_DrawHLine(0, 18, 12);	
		BSP_LCD_DrawHLine(0, 188, 12);	
		for(E=0;E<5;E++)							/////Draw gray lines
			{
			BSP_LCD_FillRect(12, 18+(38*E), 468, 19);		
			}
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		for(E=0;E<4;E++)
			{
			BSP_LCD_FillRect(12, 37+(38*E), 468, 19);	
			}		
		BSP_LCD_SelectLayer(CURRENT_LAY);
		return;
		}
	else if(LAY==BR_NAVI_END)		//X => BROWSER NAVIGATION WITH ANIMATION END
		{
		CURRENT_LAY = ActiveLayer;		
		if(BROWSE_LEVEL==0) //in playlist 	
			{	
			BSP_LCD_SelectLayer(0);		
			if(BROWSER_INFO_enable)
				{
				intDrawLayer0_INFO_ANIMATION(B0CursorPos);	
				HAL_GPIO_WritePin(GPIOH, LED_INFO_Pin, GPIO_PIN_SET);			
				}
			else
				{
				intDrawLayer0_NOINFO_ANIMATION(B0CursorPos);		
				}	
			BSP_LCD_SelectLayer(1);
			BSP_LCD_Clear(0x0000);		
			BSP_LCD_SetTransparency(1, 255);
			BSP_LCD_SetFont(&Font15P);			
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);				
			sprintf((char*)Buf, "%s", "|");		
			BSP_LCD_DisplayStringAt(4, 1,Buf, TRANSPARENT_MODE);				
			sprintf((char*)Buf, "%s", TRACKLIST_NAME[B1CursorPos+BPlaylistPos-1]);						
			BSP_LCD_DisplayStringAt(21, 1, Buf, TRANSPARENT_MODE);
			sprintf((char*)Buf, "%s", "Total Track");						
			BSP_LCD_DisplayStringAt(320, 1,Buf, TRANSPARENT_MODE);
			sprintf((char *)Buf , "%1lu", TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos] - TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]);				
			BSP_LCD_DisplayStringAt(418, 1,Buf, TRANSPARENT_MODE);	
			if(BROWSER_INFO_enable)
				{
				int_BIx_DRAW_ALL_LINES(0);	
				}
			else
				{
				int_B_DRAW_ALL_LINES();		
				}
			}
		else if(BROWSE_LEVEL==1) //playlists 	
			{
			BSP_LCD_SelectLayer(0);		
			intDrawLayer0_ANIMATION(B1CursorPos);	
			BSP_LCD_SelectLayer(1);	
			BSP_LCD_Clear(0x0000);	
			BSP_LCD_SetTransparency(1, 255);
			BSP_LCD_SetFont(&Font15P);			
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			sprintf((char*)Buf, "%s", "[PLAYLIST]");						
			BSP_LCD_DisplayStringAt(4, 1,Buf, TRANSPARENT_MODE);
			int_BIx_DRAW_ALL_LINES(BROWSE_LEVEL);	
			HAL_GPIO_WritePin(GPIOH, LED_INFO_Pin, GPIO_PIN_RESET);	
			}
		else if(BROWSE_LEVEL==2) //SD Card 	
			{
			BSP_LCD_SelectLayer(0);			
			intDrawLayer0_ANIMATION(B2CursorPos);		
			BSP_LCD_SelectLayer(1);	
			BSP_LCD_Clear(0x0000);		
			BSP_LCD_SetTransparency(1, 255);
			BSP_LCD_SetFont(&Font15P);	
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
			sprintf((char*)Buf, "%s", "/");		
			BSP_LCD_DisplayStringAt(4, 1,Buf, TRANSPARENT_MODE);				
			sprintf((char*)Buf, "%s", "SD card");						
			BSP_LCD_DisplayStringAt(21, 1, Buf, TRANSPARENT_MODE);								
			int_BIx_DRAW_ALL_LINES(BROWSE_LEVEL);	
			HAL_GPIO_WritePin(GPIOH, LED_INFO_Pin, GPIO_PIN_RESET);	
			}	
		else if(BROWSE_LEVEL==3) //SD Card info 	
			{
			BSP_LCD_SelectLayer(0);			
			intDrawLayer0_ANIMATION(0);		
			BSP_LCD_SelectLayer(1);	
			BSP_LCD_Clear(0x0000);		
			BSP_LCD_SetTransparency(1, 255);
			BSP_LCD_SetFont(&Font15P);			
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			sprintf((char*)Buf, "%s", "SD card slot");						
			BSP_LCD_DisplayStringAt(4, 1,Buf, TRANSPARENT_MODE);
			int_BIx_DRAW_ALL_LINES(BROWSE_LEVEL);		
			HAL_GPIO_WritePin(GPIOH, LED_INFO_Pin, GPIO_PIN_RESET);	
			}
		if(BROWSER_INFO_enable)
			{	
			dSHOW = BROWSER_INFO; 
			}
		else
			{
			dSHOW = BROWSER;	
			}
		BSP_LCD_SelectLayer(CURRENT_LAY);	
		return;	
		}
	};	
	
	
/////////////////////////////////	
//
//internal function transparent bar for Browser, waveform
//
void int_DRAW_TRANSPARENT_BAR(uint16_t col)
	{
	uint16_t j;	
	uint32_t c;
	uint8_t r,g,b;		
	for(j=0;j<8160;j++)
		{				
		if(panelbar[j]!=0)
			{			
			c = (col&0x7FFF)>>10;
			c*= panelbar[j];	
			c+=128;	
			c>>=8;	
			r = c;
			c = (col&0x03FF)>>5; 	
			c*= panelbar[j];
			c+=128;	
			c>>=8;	
			g = c;
			c = col&0x001F; 	
			c*= panelbar[j];
			c+=128;
			c>>=8;	
			b = c;	
			BSP_LCD_DrawPixel(j%480, j/480, 0x8000+(r<<10)+(g<<5)+b);	
			}
		}
	return;	
	};		
	

//////////////////////////////////////////////////	
//	
//	draw layer 0 for without INFO BROWSER and TAGLIS	
void intDrawLayer0_NOINFO(uint8_t CurrentCursorPosition)
	{
	uint8_t E;		
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);	
	BSP_LCD_DrawHLine(0, 18, 12);	
	BSP_LCD_DrawHLine(0, 188, 12);	
	for(E=0;E<5;E++)							/////Draw gray lines
		{
		if(CurrentCursorPosition%2==1 | (CurrentCursorPosition/2) != E)
			{		
			BSP_LCD_FillRect(12, 18+(38*E), 468, 19);
			}			
		}
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
	for(E=0;E<4;E++)
		{
		if(CurrentCursorPosition%2==0 | (CurrentCursorPosition/2) != E)
			{				
			BSP_LCD_FillRect(12, 37+(38*E), 468, 19);
			}			
		}
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*CurrentCursorPosition)), 468, 9);			////Draw selected cursor	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*CurrentCursorPosition)), 468, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*CurrentCursorPosition)), 468, 5);
	return;	
	};
	
	
	
/////////////////////////////////////////////////	
//
//draw layer 0 for INFO BROWSER and TAGLIST
//+91
//
void intDrawLayer0_INFO(uint8_t CurrentCursorPosition)
	{
	uint8_t E;	
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
	BSP_LCD_DrawHLine(0, 18, 12);	
	BSP_LCD_DrawHLine(0, 188, 12);
	for(E=0;E<5;E++)															/////Draw gray lines
		{
		if(CurrentCursorPosition%2==1 | (CurrentCursorPosition/2) != E)
			{		
			BSP_LCD_FillRect(12, 18+(38*E), 349, 19);
			}			
		}
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
	for(E=0;E<4;E++)
		{
		if(CurrentCursorPosition%2==0 | (CurrentCursorPosition/2) != E)
			{				
			BSP_LCD_FillRect(12, 37+(38*E), 349, 19);
			}			
		}
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*CurrentCursorPosition)), 335, 9);			////Draw selected cursor
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*CurrentCursorPosition)), 335, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*CurrentCursorPosition)), 335, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_PAPER);					//Draw paper rectangle
	BSP_LCD_FillRect(361, 18, 119, 171);	
	BSP_LCD_SetTextColor(LCD_COLOR_SHADOW);					//Shadow
	BSP_LCD_DrawRect(362, 19, 117, 169);
	intDrawTriangle(CurrentCursorPosition, 91);					//Draw triangle
	intDrawAllIcons();
	return;	
	};	
	
////////////////////////////////////////////////////
//draw triangle for browser with INFO
//
void intDrawTriangle(uint8_t CurrentCursorPosition, uint8_t x)
	{
	BSP_LCD_SetTextColor(LCD_COLOR_PAPER);
	BSP_LCD_DrawLine(270+x, 18, 270+x, 188)	;
	BSP_LCD_SetTextColor(LCD_COLOR_SHADOW);
	BSP_LCD_DrawLine(271+x, 19, 271+x, 187);
	for(uint16_t j = 0;j<304;j++)
		{
		BSP_LCD_DrawPixel(256+(j&0x0F)+x, ((36+(19*CurrentCursorPosition))-(j>>4)), (0x8000+256*strelka[2*j+1]+strelka[2*j]));
		}	
	BSP_LCD_DrawPixel(271+x, 18, LCD_COLOR_PAPER);	
	BSP_LCD_DrawPixel(271+x, 19, LCD_COLOR_SHADOW);	
	BSP_LCD_DrawPixel(271+x, 188, LCD_COLOR_PAPER);	
	BSP_LCD_DrawPixel(271+x, 187, LCD_COLOR_SHADOW);		
	return;
	}	
	
/////////////////////////////////	
//
//internal function for Browser
void int_B_DRAW_ALL_LINES(void)
	{
	uint16_t E;
	for(E=0;E<9 && TOTAL_TRACKS_IN_CURRENT_PLAYLIST>(E+BTrackPos-1);E++)
		{					
		if(E==B0CursorPos && (playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]-1][54]%2)==1)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
			}
		else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]-1][54]%2)==1)		
			{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
			}
		else if(E==B0CursorPos)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			}
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			}
		sprintf((char *)Buf , "%03lu", E+BTrackPos);					
		BSP_LCD_DisplayStringAt(20,20+(19*E),Buf, TRANSPARENT_MODE);
		BSP_LCD_DisplayStringAt(69,20+(19*E), playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]-1], TRANSPARENT_MODE);
			
		if(TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]==track_play_now[dkA] ||
			 TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]==track_play_now[dkB])
			{
			sprintf((char*)Buf, "%s", ">");	
			}
		else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]-1][54]&0x2)==0)
			{
			sprintf((char*)Buf, "%s", "~");  
			}	
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_RED);
			sprintf((char*)Buf, "%s", "<");	
			}			
		BSP_LCD_DisplayStringAt(50, 20+(19*E),Buf, TRANSPARENT_MODE);	
		}	
		
	BSP_LCD_SelectLayer(0);
	ReDrawScroll(TOTAL_TRACKS_IN_CURRENT_PLAYLIST, BTrackPos);	
	};	
	
	
/////////////////////////////////	
//
//internal function for Browser
void int_B_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{		
	if(B0CursorPos%2==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		}
		
	BSP_LCD_FillRect(12, 18+(19*B0CursorPos), 468, 19);
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
		
	if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}		
	sprintf((char *)Buf , "%03lu", BTrackPos+B0CursorPos);					
	BSP_LCD_DisplayStringAt(20,20+(19*B0CursorPos),Buf, TRANSPARENT_MODE);	
	BSP_LCD_DisplayStringAt(69,20+(19*B0CursorPos), playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]-1], TRANSPARENT_MODE);	

	if(TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1] == track_play_now[dkA] ||
		 TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1] == track_play_now[dkB])
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]-1][54]&0x2)==0)
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "<");		
		}
	BSP_LCD_DisplayStringAt(50,20+(19*B0CursorPos),Buf, TRANSPARENT_MODE);	
	if(UPDOWN==1)
		{
		B0CursorPos++;
		}
	else
		{
		B0CursorPos--;	
		}
		
	if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}		
	sprintf((char *)Buf , "%03lu", BTrackPos+B0CursorPos);					
	BSP_LCD_DisplayStringAt(20,20+(19*B0CursorPos),Buf, TRANSPARENT_MODE);	
	BSP_LCD_DisplayStringAt(69,20+(19*B0CursorPos), playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]-1], TRANSPARENT_MODE);	
		
	if(TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1] == track_play_now[dkA] ||
		 TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1] == track_play_now[dkB])	
		{
		sprintf((char*)Buf, "%s", ">");	
		}			
	else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]-1][54]&0x2)==0)
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "<");		
		}
	BSP_LCD_DisplayStringAt(50,20+(19*B0CursorPos),Buf, TRANSPARENT_MODE);	
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*B0CursorPos)), 468, 9);			//////////////////////////////	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*B0CursorPos)), 468, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*B0CursorPos)), 468, 5);
	return;			
	};		


/////////////////////////////////	
//
//internal function for Browser + INFO
// +91
//	
void int_BIx_DRAW_ALL_LINES(uint8_t lvl)
	{
	uint16_t E, j, k;
	if(lvl==0)					//tracks
		{
		for(E=0;E<9 && TOTAL_TRACKS_IN_CURRENT_PLAYLIST>(E+BTrackPos-1);E++)
			{				
			if(E==B0CursorPos && (playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]-1][54]%2)==1)
				{
				BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
				}
			else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]-1][54]%2)==1)	
				{
				BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
				}
			else if(E==B0CursorPos)
				{
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
				}
			else
				{
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				}
			BSP_LCD_DisplayStringAt(39,20+(19*E), playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]-1], INFOTRACK_MODE);		
				
			if(TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]==track_play_now[dkA] ||
				TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]==track_play_now[dkB])
				{
				sprintf((char*)Buf, "%s", ">");	
				}	
			else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+E+BTrackPos-1]-1][54]&0x2)==0)	
				{
				sprintf((char*)Buf, "%s", "~");
				}		
			else
				{
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
				sprintf((char*)Buf, "%s", "<");		
				}				
			BSP_LCD_DisplayStringAt(20,20+(19*E),Buf, INFOTRACK_MODE);		
			}	
		intClearInfoWin();
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		sprintf((char *)Buf , "%03lu", B0CursorPos+BTrackPos);	
		BSP_LCD_DisplayStringAt(387, 21,Buf, TRANSPARENT_MODE);	
		sprintf((char *)Buf , "%2lu.%1lu bpm", original_tempo[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]/10, 
			original_tempo[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]%10);		

		BSP_LCD_DisplayStringAt(387, 78, Buf, TRANSPARENT_MODE);
		int_VALUE_to_KEY(key_id[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]);	
		BSP_LCD_DisplayStringAt(387, 97, Buf, TRANSPARENT_MODE);
			
		sprintf((char *)Buf , "%02lu:%02lu", duration[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]/60, 
			duration[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]%60);		
		BSP_LCD_DisplayStringAt(387, 59, Buf, TRANSPARENT_MODE);
		int_DRAW_STARS_RATING(rating[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]);
		BSP_LCD_SelectLayer(0);
		ReDrawScroll(TOTAL_TRACKS_IN_CURRENT_PLAYLIST, BTrackPos);		
		}
	else if(lvl==1)						//playlists
		{	
		for(E=0;E<9 && TOTAL_TRACKLISTS>(E+BPlaylistPos-1);E++)
			{
			if(E==B1CursorPos)
				{
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
				}
			else
				{
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				}	
			BSP_LCD_DisplayStringAt(39,20+(19*E), TRACKLIST_NAME[E+BPlaylistPos-1], INFO_MODE);		
			sprintf((char*)Buf, "%s", "|");		
			BSP_LCD_DisplayStringAt(20,20+(19*E),Buf, INFO_MODE);		
			}	
		BSP_LCD_SetTextColor(0x0000);					//Draw paper rectangle
		BSP_LCD_FillRect(272, 18, 208, 171);						

		for(j=0;j<8;j++)							////Dots
			{
			for(k=0;k<99;k++)
				{
				BSP_LCD_DrawPixel(276+2*k, 36+19*j, LCD_COLOR_DARK_1);
				}
			}	
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);	
		for(E=0;(E<9 && (TRACKLIST_OFFSET[BPlaylistPos+B1CursorPos] - TRACKLIST_OFFSET[BPlaylistPos+B1CursorPos-1])>E);E++)
			{
			sprintf((char*)Buf, "%s", "~");		
			BSP_LCD_DisplayStringAt(280, 21+(19*E),Buf, TRANSPARENT_MODE);	
			sprintf((char*)Buf, "%s", playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[BPlaylistPos+B1CursorPos-1]+E]-1]);						
			BSP_LCD_DisplayStringAt(299, 21+(19*E),Buf, TRANSPARENT_MODE);	
			}	
		BSP_LCD_SelectLayer(0);	
		ReDrawScroll(TOTAL_TRACKLISTS, BPlaylistPos);		
		}
	else if(lvl==2)
		{
		for(E=0;E<5;E++)
			{
			if(E==B2CursorPos)
				{
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
				}
			else
				{
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				}	
			if(E==0)
				{
				sprintf((char*)Buf, "%s", "[FILENAME]");			
				}
			else if(E==1)
				{
				sprintf((char*)Buf, "%s", "[FOLDER]");			
				}	
			else if(E==2)
				{
				sprintf((char*)Buf, "%s", "[PLAYLIST]");			
				}	
			else if(E==3)
				{
				sprintf((char*)Buf, "%s", "[TRACK]");			
				}	
			else if(E==4)
				{
				sprintf((char*)Buf, "%s", "[SEARCH]");			
				}	
				
			BSP_LCD_DisplayStringAt(20, 20+(19*E), Buf, INFO_MODE);			
			}
		BSP_LCD_SetTextColor(0x0000);					//Draw paper rectangle
		BSP_LCD_FillRect(272, 18, 208, 171);	

		for(j=0;j<8;j++)							////Dots
			{
			for(k=0;k<99;k++)
				{
				BSP_LCD_DrawPixel(276+2*k, 36+19*j, LCD_COLOR_DARK_1);
				}
			}	
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);	
			
		for(E=0;E<9;E++)
			{
			if(B2CursorPos==0 || B2CursorPos==3)
				{
				if(E<TOTAL_TRACKS)
					{
					BSP_LCD_DisplayStringAt(299,21+(19*E), playlist[E], TRANSPARENT_MODE);	
					sprintf((char*)Buf, "%s", "~");		
					BSP_LCD_DisplayStringAt(280, 21+(19*E),Buf, TRANSPARENT_MODE);		
					}
				}
			else if(B2CursorPos==1)				
				{
				if(E==0)
					{
					sprintf((char*)Buf, "%s", "EMPTY");		
					BSP_LCD_DisplayStringAt(345, 59 ,Buf, TRANSPARENT_MODE);
					sprintf((char*)Buf, "%s", "or not support");		
					BSP_LCD_DisplayStringAt(319, 78 ,Buf, TRANSPARENT_MODE);	
					}
				}
			else if(B2CursorPos==2)				
				{
				if(E<TOTAL_TRACKLISTS)
					{
					BSP_LCD_DisplayStringAt(299,21+(19*E), TRACKLIST_NAME[E], TRANSPARENT_MODE);
					sprintf((char*)Buf, "%s", "|");		
					BSP_LCD_DisplayStringAt(280, 21+(19*E),Buf, TRANSPARENT_MODE);		
					}
				}			
			}	
		BSP_LCD_SelectLayer(0);	
		ReDrawScroll(4, 0);	
		}	
	else if(lvl==3)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);				
		sprintf((char*)Buf, "%s", "/");		
		BSP_LCD_DisplayStringAt(20,20, Buf, TRANSPARENT_MODE);					
		sprintf((char*)Buf, "%s", "SD card");						
		BSP_LCD_DisplayStringAt(39, 20, Buf, TRANSPARENT_MODE);	
		BSP_LCD_SetTextColor(0x0000);					//Draw paper rectangle
		BSP_LCD_FillRect(272, 18, 208, 171);	
		for(j=0;j<8;j++)							////Dots
			{
			for(k=0;k<99;k++)
				{
				BSP_LCD_DrawPixel(276+2*k, 36+19*j, LCD_COLOR_DARK_1);
				}
			}	
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);						
		BSP_LCD_DisplayStringAt(280, 21, SDCARD_NAME, TRANSPARENT_MODE);				//Flash name
		sprintf((char*)Buf, "%1lu songs", TOTAL_TRACKS);						
		BSP_LCD_DisplayStringAt(280, 40, Buf, TRANSPARENT_MODE);		
		sprintf((char*)Buf, "%1lu playlists", TOTAL_TRACKLISTS);						
		BSP_LCD_DisplayStringAt(280, 59, Buf, TRANSPARENT_MODE);	
		BSP_LCD_DisplayStringAt(280, 78, SD_DATE, TRANSPARENT_MODE);			//DATE
		if(used_mem>999)
			{
			if(used_mem>9999)
				{
				sprintf((char*)Buf, "%1lu.%01lu GB used", used_mem/1000, (used_mem%1000)/100);		
				}
			else
				{
				sprintf((char*)Buf, "%1lu.%02lu GB used", used_mem/1000, (used_mem%1000)/10);		
				}
			}
		else
			{
			sprintf((char*)Buf, "%1lu MB used", used_mem);	
			}		
		BSP_LCD_DisplayStringAt(280, 97, Buf, TRANSPARENT_MODE);	
		if(free_mem>999)
			{	
			if(free_mem>9999)
				{
				sprintf((char*)Buf, "%1lu.%01lu GB free", free_mem/1000, (free_mem%1000)/100);		
				}
			else
				{
				sprintf((char*)Buf, "%1lu.%02lu GB free", free_mem/1000, (free_mem%1000)/10);		
				}
			}
		else
			{
			sprintf((char*)Buf, "%1lu MB free", free_mem);	
			}							
		BSP_LCD_DisplayStringAt(280, 116, Buf, TRANSPARENT_MODE);	
		BSP_LCD_SelectLayer(0);
		ReDrawScroll(1, 0);			
		}	
	return;	
	};


/////////////////////////////////	
//
//internal function for Browser + INFO
//+91
//
void int_BI_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{
	if(B0CursorPos%2==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		}
	BSP_LCD_FillRect(12, 18+(19*B0CursorPos), 349, 19);
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
	if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}		
	BSP_LCD_DisplayStringAt(39,20+(19*B0CursorPos), playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1], INFOTRACK_MODE);	
	if(TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]==track_play_now[dkA] ||
		TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]==track_play_now[dkB])
		{
		sprintf((char*)Buf, "%s", ">");	
		}		
	else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]-1][54]&0x2)==0)
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "<");		
		}
	BSP_LCD_DisplayStringAt(20,20+(19*B0CursorPos),Buf, INFOTRACK_MODE);		
	if(UPDOWN==1)
		{
		B0CursorPos++;
		}
	else
		{
		B0CursorPos--;	
		}
	if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]-1][54]%2)==1)
		{			
		BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}	
	BSP_LCD_DisplayStringAt(39,20+(19*B0CursorPos), playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1	], INFOTRACK_MODE);
	if(TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]==track_play_now[dkA] ||
		TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]==track_play_now[dkB])		
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+BTrackPos+B0CursorPos-1]-1][54]&0x2)==0)	
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "<");		
		}
	BSP_LCD_DisplayStringAt(20,20+(19*B0CursorPos),Buf, INFOTRACK_MODE);	
	intClearInfoWin();
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);		
	sprintf((char *)Buf , "%03lu", B0CursorPos+BTrackPos);
	BSP_LCD_DisplayStringAt(387, 21,Buf, TRANSPARENT_MODE);	
	sprintf((char *)Buf , "%2lu.%1lu bpm", original_tempo[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]/10, 
		original_tempo[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]%10);		
	BSP_LCD_DisplayStringAt(387, 78, Buf, TRANSPARENT_MODE);
	int_VALUE_to_KEY(key_id[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]);		
	BSP_LCD_DisplayStringAt(387, 97, Buf, TRANSPARENT_MODE);	
	sprintf((char *)Buf , "%02lu:%02lu", duration[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]/60, 
		duration[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]%60);		
	BSP_LCD_DisplayStringAt(387, 59, Buf, TRANSPARENT_MODE);	
	int_DRAW_STARS_RATING(rating[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CursorPos+BPlaylistPos-1]+B0CursorPos+BTrackPos-1]-1]);
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*B0CursorPos)), 335, 9);			//////////////////////////////	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*B0CursorPos)), 335, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*B0CursorPos)), 335, 5);
	intDrawTriangle(B0CursorPos, 91);			
	return;			
	};			
	

/////////////////////////////////	
//
//internal function for TAG LIST
void int_T_DRAW_ALL_LINES(void)
	{
	uint16_t E;
	for(E=0;E<9 && TOTAL_TRACKS_IN_TAGLIST>(E+TTrackPos-1);E++)
		{
		if(E==TCursorPos && ((TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkA] || TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkB]) || (playlist[TAGLIST_BASE[E+TTrackPos-1]-1][54]%2)==1))
			{
			BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
			}
		else if((TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkA] || TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkB]) || (playlist[TAGLIST_BASE[E+TTrackPos-1]-1][54]%2)==1)	
			{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
			}
		else if(E==TCursorPos)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			}
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			}
	
		if(TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkA] || TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkB])
			{
			sprintf((char*)Buf, "%s", ">");	
			}
		else
			{
			sprintf((char*)Buf, "%s", "~");
			}					
		BSP_LCD_DisplayStringAt(20,20+(19*E),Buf, TRANSPARENT_MODE);	
		BSP_LCD_DisplayStringAt(39,20+(19*E), playlist[TAGLIST_BASE[E+TTrackPos-1]-1], TRANSPARENT_MODE);		
		}	
	BSP_LCD_SelectLayer(0);
	ReDrawScroll(TOTAL_TRACKS_IN_TAGLIST, TTrackPos);	
	};	
	
/////////////////////////////////	
//
//internal function for TAG LIST
void int_T_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{
	if(TCursorPos%2==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		}
	BSP_LCD_FillRect(12, 18+(19*TCursorPos), 468, 19);
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
	if((TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkA] || TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkB]) || (playlist[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}		
		
	if(TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkA] || TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkB])
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	BSP_LCD_DisplayStringAt(20,20+(19*TCursorPos),Buf, TRANSPARENT_MODE);	
	BSP_LCD_DisplayStringAt(39,20+(19*TCursorPos), playlist[TAGLIST_BASE[TCursorPos+TTrackPos-1]-1], TRANSPARENT_MODE);		
	if(UPDOWN==TAGLIST_UP)
		{
		TCursorPos++;
		}
	else
		{
		TCursorPos--;	
		}
	if((TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkA] || TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkB]) || (playlist[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}		
	if(TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkA] || TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkB])
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	BSP_LCD_DisplayStringAt(20,20+(19*TCursorPos),Buf, TRANSPARENT_MODE);	
	BSP_LCD_DisplayStringAt(39,20+(19*TCursorPos), playlist[TAGLIST_BASE[TCursorPos+TTrackPos-1]-1], TRANSPARENT_MODE);
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*TCursorPos)), 468, 9);			//////////////////////////////	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*TCursorPos)), 468, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*TCursorPos)), 468, 5);
	return;			
	};		
	
/////////////////////////////////	
//
//internal function for TAG LIST + INFO
//+91
//	
void int_TI_DRAW_ALL_LINES(void)
	{
	uint16_t E;
	for(E=0;E<9 && TOTAL_TRACKS_IN_TAGLIST>(E+TTrackPos-1);E++)
		{
		if(E==TCursorPos && ((TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkA] || TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkB]) || (playlist[TAGLIST_BASE[E+TTrackPos-1]-1][54]%2)==1))
			{
			BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
			}
		else if((TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkA] || TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkB]) || (playlist[TAGLIST_BASE[E+TTrackPos-1]-1][54]%2)==1)	
			{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
			}
		else if(E==TCursorPos)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			}
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			}
			
		if(TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkA] || TAGLIST_BASE[E+TTrackPos-1]==track_play_now[dkB])
			{
			sprintf((char*)Buf, "%s", ">");	
			}
		else
			{
			sprintf((char*)Buf, "%s", "~");
			}					
		BSP_LCD_DisplayStringAt(20,20+(19*E),Buf, INFOTRACK_MODE);	
		BSP_LCD_DisplayStringAt(39,20+(19*E), playlist[TAGLIST_BASE[E+TTrackPos-1]-1], INFOTRACK_MODE);	
		}	
	intClearInfoWin();
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	if(TOTAL_TRACKS_IN_TAGLIST>0)
		{
		sprintf((char *)Buf , "%03lu", TAGLIST_BASE[TTrackPos+TCursorPos-1]);		
		BSP_LCD_DisplayStringAt(387, 21,Buf, TRANSPARENT_MODE);
		sprintf((char *)Buf , "%2lu.%1lu bpm", original_tempo[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]/10, original_tempo[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]%10);		
		BSP_LCD_DisplayStringAt(387, 78, Buf, TRANSPARENT_MODE);
		int_VALUE_to_KEY(key_id[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]);	
		BSP_LCD_DisplayStringAt(387, 97, Buf, TRANSPARENT_MODE);
		sprintf((char *)Buf , "%02lu:%02lu", duration[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]/60, duration[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]%60);		
		BSP_LCD_DisplayStringAt(387, 59, Buf, TRANSPARENT_MODE);		
		int_DRAW_STARS_RATING(rating[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]);		
		}
	BSP_LCD_SelectLayer(0);
	ReDrawScroll(TOTAL_TRACKS_IN_TAGLIST, TTrackPos);	
	};	
	
/////////////////////////////////	
//
//internal function for TAG LIST + INFO
//+91
//	
void int_TI_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{
	if(TCursorPos%2==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		}
	BSP_LCD_FillRect(12, 18+(19*TCursorPos), 349, 19);
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
	if((TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkA] || TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkB]) || (playlist[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}		
	if(TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkA] || TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkB])
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	BSP_LCD_DisplayStringAt(20,20+(19*TCursorPos),Buf, INFOTRACK_MODE);	
	BSP_LCD_DisplayStringAt(39,20+(19*TCursorPos), playlist[TAGLIST_BASE[TCursorPos+TTrackPos-1]-1], INFOTRACK_MODE);		
	if(UPDOWN==TAGLIST_UP)
		{
		TCursorPos++;
		}
	else
		{
		TCursorPos--;	
		}
	if((TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkA] || TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkB]) || (playlist[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}		
	if(TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkA] || TAGLIST_BASE[TTrackPos+TCursorPos-1]==track_play_now[dkB])
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	BSP_LCD_DisplayStringAt(20,20+(19*TCursorPos),Buf, INFOTRACK_MODE);	
	BSP_LCD_DisplayStringAt(39,20+(19*TCursorPos), playlist[TAGLIST_BASE[TCursorPos+TTrackPos-1]-1], INFOTRACK_MODE);
	intClearInfoWin();
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	sprintf((char *)Buf , "%03lu", TAGLIST_BASE[TTrackPos+TCursorPos-1]);		
	BSP_LCD_DisplayStringAt(387, 21,Buf, TRANSPARENT_MODE);
	sprintf((char *)Buf , "%2lu.%1lu bpm", original_tempo[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]/10, original_tempo[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]%10);		
	BSP_LCD_DisplayStringAt(387, 78, Buf, TRANSPARENT_MODE);
	int_VALUE_to_KEY(key_id[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]);	
	BSP_LCD_DisplayStringAt(387, 97, Buf, TRANSPARENT_MODE);
	sprintf((char *)Buf , "%02lu:%02lu", duration[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]/60, duration[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]%60);		
	BSP_LCD_DisplayStringAt(387, 59, Buf, TRANSPARENT_MODE);		
	int_DRAW_STARS_RATING(rating[TAGLIST_BASE[TTrackPos+TCursorPos-1]-1]);
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*TCursorPos)), 335, 9);			//////////////////////////////	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*TCursorPos)), 335, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*TCursorPos)), 335, 5);
	intDrawTriangle(TCursorPos, 91);	
	return;			
	};			
	
/////////////////////////////////	
//
//internal function for Browser level 1
void int_B1_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{
	uint8_t E;	
	if(B1CursorPos%2==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		}
	BSP_LCD_FillRect(12, 18+(19*B1CursorPos), 258, 19);
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(39,20+(19*B1CursorPos), TRACKLIST_NAME[B1CursorPos+BPlaylistPos-1], INFO_MODE);		
	sprintf((char*)Buf, "%s", "|");		
	BSP_LCD_DisplayStringAt(20,20+(19*B1CursorPos),Buf, INFO_MODE);	
	
	if(UPDOWN==7)
		{
		B1CursorPos++;
		}
	else if(UPDOWN==6)
		{
		B1CursorPos--;	
		}
		
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(39,20+(19*B1CursorPos), TRACKLIST_NAME[B1CursorPos+BPlaylistPos-1], INFO_MODE);		
	sprintf((char*)Buf, "%s", "|");		
	BSP_LCD_DisplayStringAt(20,20+(19*B1CursorPos),Buf, INFO_MODE);	
		
	BSP_LCD_SetTextColor(0x0000);					//Draw paper rectangle
	BSP_LCD_FillRect(272, 18, 208, 171);

	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);	
	for(E=0;(E<9 && (TRACKLIST_OFFSET[BPlaylistPos+B1CursorPos] - TRACKLIST_OFFSET[BPlaylistPos+B1CursorPos-1])>E);E++)
		{
		sprintf((char*)Buf, "%s", "~");		
		BSP_LCD_DisplayStringAt(280, 21+(19*E),Buf, TRANSPARENT_MODE);	
		sprintf((char*)Buf, "%s", playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[BPlaylistPos+B1CursorPos-1]+E]-1]);						
		BSP_LCD_DisplayStringAt(299, 21+(19*E),Buf, TRANSPARENT_MODE);	
		}	
	
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*B1CursorPos)), 244, 9);			//////////////////////////////	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*B1CursorPos)), 244, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*B1CursorPos)), 244, 5);
	intDrawTriangle(B1CursorPos, 0);			
	return;			
	};			
	
/////////////////////////////////	
//
//internal function for Browser level 2
void int_B2_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{
	uint8_t E;	
	if(B2CursorPos%2==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		}
	BSP_LCD_FillRect(12, 18+(19*B2CursorPos), 258, 19);
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			
	if(B2CursorPos==0)
		{
		sprintf((char*)Buf, "%s", "[FILENAME]");			
		}
	else if(B2CursorPos==1)
		{
		sprintf((char*)Buf, "%s", "[FOLDER]");			
		}	
	else if(B2CursorPos==2)
		{
		sprintf((char*)Buf, "%s", "[PLAYLIST]");			
		}	
	else if(B2CursorPos==3)
		{
		sprintf((char*)Buf, "%s", "[TRACK]");			
		}		
	else if(B2CursorPos==4)
		{
		sprintf((char*)Buf, "%s", "[SEARCH]");			
		}
	BSP_LCD_DisplayStringAt(20, 20+(19*B2CursorPos), Buf, INFO_MODE);			


	if(UPDOWN==BROWSER2_UP)
		{
		B2CursorPos++;
		}
	else if(UPDOWN==BROWSER2_DOWN)
		{
		B2CursorPos--;	
		}

	BSP_LCD_SetTextColor(0x0000);					//Draw paper rectangle
	BSP_LCD_FillRect(272, 18, 208, 171);	
	
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);	
		
	for(E=0;E<9;E++)
		{
		if(B2CursorPos==0 || B2CursorPos==3)
			{
			if(E<TOTAL_TRACKS)
				{
				BSP_LCD_DisplayStringAt(299,21+(19*E), playlist[E], TRANSPARENT_MODE);	
				sprintf((char*)Buf, "%s", "~");		
				BSP_LCD_DisplayStringAt(280, 21+(19*E),Buf, TRANSPARENT_MODE);		
				}	
			}
		else if(B2CursorPos==1)				
			{
			if(E==0)
				{
				sprintf((char*)Buf, "%s", "EMPTY");		
				BSP_LCD_DisplayStringAt(345, 59 ,Buf, TRANSPARENT_MODE);	
				sprintf((char*)Buf, "%s", "or not support");		
				BSP_LCD_DisplayStringAt(319, 78 ,Buf, TRANSPARENT_MODE);	
				}
			}
		else if(B2CursorPos==2)				
			{
			if(E<TOTAL_TRACKLISTS)
				{
				BSP_LCD_DisplayStringAt(299,21+(19*E), TRACKLIST_NAME[E], TRANSPARENT_MODE);
				sprintf((char*)Buf, "%s", "|");		
				BSP_LCD_DisplayStringAt(280, 21+(19*E),Buf, TRANSPARENT_MODE);		
				}
			}			
		}	
					
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		
	if(B2CursorPos==0)
		{
		sprintf((char*)Buf, "%s", "[FILENAME]");			
		}
	else if(B2CursorPos==1)
		{
		sprintf((char*)Buf, "%s", "[FOLDER]");			
		}	
	else if(B2CursorPos==2)
		{
		sprintf((char*)Buf, "%s", "[PLAYLIST]");			
		}	
	else if(B2CursorPos==3)
		{
		sprintf((char*)Buf, "%s", "[TRACK]");			
		}		
	else if(B2CursorPos==4)
		{
		sprintf((char*)Buf, "%s", "[SEARCH]");			
		}			
	BSP_LCD_DisplayStringAt(20, 20+(19*B2CursorPos), Buf, INFO_MODE);				
			
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*B2CursorPos)), 244, 9);			//////////////////////////////	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*B2CursorPos)), 244, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*B2CursorPos)), 244, 5);
	intDrawTriangle(B2CursorPos, 0);			
	return;			
	};			
	
/////////////////////////////////////////////////	
//
//draw layer 0 for BROWSER 1-3 level
//
void intDrawLayer0_BROWSER_1_3(uint8_t CurrentCursorPosition)
	{		
	uint8_t E;	
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
	BSP_LCD_DrawHLine(0, 18, 12);	
	BSP_LCD_DrawHLine(0, 188, 12);	
	for(E=0;E<5;E++)															/////Draw gray lines
		{
		if(CurrentCursorPosition%2==1 | (CurrentCursorPosition/2) != E)
			{		
			BSP_LCD_FillRect(12, 18+(38*E), 258, 19);
			}			
		}
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
	for(E=0;E<4;E++)
		{
		if(CurrentCursorPosition%2==0 | (CurrentCursorPosition/2) != E)
			{				
			BSP_LCD_FillRect(12, 37+(38*E), 258, 19);
			}			
		}
	uint8_t j, k;	
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*CurrentCursorPosition)), 244, 9);			////Draw selected cursor
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*CurrentCursorPosition)), 244, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*CurrentCursorPosition)), 244, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_PAPER);					//Draw paper rectangle
	BSP_LCD_FillRect(270, 18, 210, 171);	
	BSP_LCD_SetTextColor(LCD_COLOR_SHADOW);					//Shadow
	BSP_LCD_DrawRect(271, 19, 208, 169);
	intDrawTriangle(CurrentCursorPosition, 0);					//Draw triangle	
	for(j=0;j<8;j++)							////Dots
		{
		for(k=0;k<99;k++)
			{
			BSP_LCD_DrawPixel(276+2*k, 36+19*j, LCD_COLOR_DARK_1);
			}
		}	
	return;	
	};	
	

/////////////////////////////////////////////////	
//
//draw layer 0 for animation 1-3 level
//
void intDrawLayer0_ANIMATION(uint8_t CurrentCursorPosition)
	{		
	uint8_t j, k;	
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*CurrentCursorPosition)), 244, 9);			////Draw selected cursor
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*CurrentCursorPosition)), 244, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*CurrentCursorPosition)), 244, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_PAPER);					//Draw paper rectangle
	BSP_LCD_FillRect(270, 18, 210, 171);	
	BSP_LCD_SetTextColor(LCD_COLOR_SHADOW);					//Shadow
	BSP_LCD_DrawRect(271, 19, 208, 169);
	intDrawTriangle(CurrentCursorPosition, 0);					//Draw triangle	
	for(j=0;j<8;j++)							////Dots
		{
		for(k=0;k<99;k++)
			{
			BSP_LCD_DrawPixel(276+2*k, 36+19*j, LCD_COLOR_DARK_1);
			}
		}	
	return;	
	};		
	
	
/////////////////////////////////////////////////	
//
//draw layer 0 for INFO BROWSER ANIMATION
//+91
void intDrawLayer0_INFO_ANIMATION(uint8_t CurrentCursorPosition)
	{
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
	BSP_LCD_DrawHLine(0, 18, 12);	
	BSP_LCD_DrawHLine(0, 188, 12);	
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(12, (18+(19*CurrentCursorPosition)), 335, 9);			////Draw selected cursor
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*CurrentCursorPosition)), 335, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*CurrentCursorPosition)), 335, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_PAPER);					//Draw paper rectangle
	BSP_LCD_FillRect(361, 18, 119, 171);	
	BSP_LCD_SetTextColor(LCD_COLOR_SHADOW);					//Shadow
	BSP_LCD_DrawRect(362, 19, 117, 169);
	intDrawTriangle(CurrentCursorPosition, 91);					//Draw triangle	
	intDrawAllIcons();	
	return;	
	};		
	
//////////////////////////////////////////////////	
//	
//	draw layer 0 for without INFO BROWSER ANIMATION
void intDrawLayer0_NOINFO_ANIMATION(uint8_t CurrentCursorPosition)
	{
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
	BSP_LCD_DrawHLine(0, 18, 12);	
	BSP_LCD_DrawHLine(0, 188, 12);	
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*CurrentCursorPosition)), 468, 9);			////Draw selected cursor	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*CurrentCursorPosition)), 468, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*CurrentCursorPosition)), 468, 5);
	return;	
	};		
	
/////////////////////////////////	
//
//internal function for Browser + INFO 
//+91	
void int_DRAW_STARS_RATING(uint16_t rat)
	{
	uint16_t j, k;
	for(j=0;j<5;j++)							////STARS
		{		
		for(k=0;k<208;k++)
			{
			if((iSTAR[k/16]>>(k%16))%2)
				{
				BSP_LCD_DrawPixel((j*14)+385-k%16, 116+k/16, COLOR_MAP_RATING[1][rat&0x0F]);
				}	
			}
		if(((rat>>8)&0x07)>j)
			{
			for(k=0;k<208;k++)		////STARS FILLED
				{
				if((iSTAR_FILLED[k/16]>>(k%16))%2)
					{
					BSP_LCD_DrawPixel((j*14)+385-k%16, 116+k/16, COLOR_MAP_RATING[0][rat&0x0F]);
					}	
				}		
			}
		else
			{
			for(k=0;k<208;k++)		////STARS FILLED PAPER COLOR
				{
				if((iSTAR_FILLED[k/16]>>(k%16))%2)
					{
					BSP_LCD_DrawPixel((j*14)+385-k%16, 116+k/16, LCD_COLOR_PAPER);
					}	
				}	
			}			
		}
	return;	
	};
	
	
/////////////////////////////////	
//
//internal function for Browser INFO and Taglist INFO
//	+91
//	
void intDrawAllIcons(void)
	{
	uint8_t j, k;
	for(j=0;j<8;j++)							////Dots
		{
		for(k=0;k<54;k++)
			{
			BSP_LCD_DrawPixel(367+2*k, 36+19*j, LCD_COLOR_DARK_1);
			}
		}
	for(k=0;k<192;k++)					////NUMBER
		{
		if((iNUM[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(385-k%16, 22+k/16, LCD_COLOR_DARK_1);
			}	
		}	
	for(k=0;k<192;k++)					////ARTIST
		{
		if((iARTIST[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(386-k%16, 40+k/16, LCD_COLOR_DARK_1);
			}	
		}	
	for(k=0;k<192;k++)					////TIME
		{
		if((iTIME[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(386-k%16, 59+k/16, LCD_COLOR_DARK_1);
			}	
		}
	for(k=0;k<192;k++)					////BPM
		{
		if((iBPM[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(385-k%16, 78+k/16, LCD_COLOR_DARK_1);
			}	
		}
	for(k=0;k<224;k++)					////TONE
		{
		if((iTONE[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(385-k%16, 97+k/16, LCD_COLOR_DARK_1);
			}	
		}
	for(k=0;k<208;k++)					////DISC
		{
		if((iDISC[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(385-k%16, 135+k/16, LCD_COLOR_DARK_1);
			}	
		}
	for(k=0;k<224;k++)					////COMENTS
		{
		if((iCOMMENTS[k/16]>>(k%16))%2)
			{
			BSP_LCD_DrawPixel(385-k%16, 153+k/16, LCD_COLOR_DARK_1);
			}	
		}	
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
	sprintf((char*)Buf, "%s", "/");			
	BSP_LCD_DisplayStringAt(372, 171, Buf, TRANSPARENT_MODE);	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
	BSP_LCD_DisplayStringAt(387, 171, SDCARD_NAME, TRANSPARENT_MODE);
	return;	
	};			

/////////////////////////////////	
//internal function for Browser INFO and Taglist INFO	
//
void intClearInfoWin(void)
	{
	BSP_LCD_SetTextColor(0x0000);
	BSP_LCD_FillRect(387, 22, 26, 12);
	BSP_LCD_FillRect(387, 60, 50, 12);
	BSP_LCD_FillRect(387, 79, 77, 14);
	BSP_LCD_FillRect(387, 97, 54, 15);
	};						
	
/////////////////////////////////	
//	
//Redraw scroll on new position	
//	161px max
//	
void ReDrawScroll(uint16_t total_elements, uint16_t current_element_pos)
	{
	ForceDrawVLine(4, ScrollPos+24, ScrollLong-2, LCD_COLOR_BLACK);							//
	ForceDrawVLine(5, ScrollPos+23, ScrollLong, LCD_COLOR_BLACK);								//	SCROLL
	ForceDrawVLine(6, ScrollPos+23, ScrollLong, LCD_COLOR_BLACK);								//
	ForceDrawVLine(7, ScrollPos+24, ScrollLong-2, LCD_COLOR_BLACK);							//
	///Calculate scroll	
	if(total_elements<10)
		{
		ScrollLong = 161;
		ScrollPos = 0;	
		return;	
		}
	else
		{
		ScrollLong = 1449/total_elements;
		if(ScrollLong>161)
			{
			ScrollLong = 161;
			return;	
			}
		else if(ScrollLong<5)
			{
			ScrollLong = 5;	
			}	
		ScrollPos = ((current_element_pos-1)*(161-ScrollLong))/(total_elements-9);	
		}
	ForceDrawVLine(4, ScrollPos+24, ScrollLong-2, LCD_COLOR_WHITE);							//
	ForceDrawVLine(5, ScrollPos+23, ScrollLong, 	LCD_COLOR_WHITE);							//	SCROLL
	ForceDrawVLine(6, ScrollPos+23, ScrollLong, 	LCD_COLOR_WHITE);							//
	ForceDrawVLine(7, ScrollPos+24, ScrollLong-2, LCD_COLOR_WHITE);							//		
	};



///////////////////////////////////////////////////////////////////
//Navigate function for browser, taglis and utility
//
//	BROWSER0_DOWN		0 position track-- (cursor up)
//	BROWSER0_UP			1 position track++ (cursor down)
//	TAGLIST_DOWN		2	position track-- (cursor up)
//	TAGLIST_UP			3	position track++ (cursor down)
//	
void NAVIGATOR(uint8_t UPDOWN)
	{	
	if(dSHOW==BROWSER || dSHOW==BROWSER_INFO)		///////////////////////Browser mode selected
		{
		if(UPDOWN==BROWSER0_UP)
			{
			if(B0CursorPos==8)												//All lines update++
				{
				if(BTrackPos == TOTAL_TRACKS_IN_CURRENT_PLAYLIST-8)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x0000);
					BSP_LCD_SetFont(&Font15P);
					BTrackPos++;
					if(dSHOW==BROWSER)
						{
						BSP_LCD_FillRect(12, 18, 468, 171);		
						int_B_DRAW_ALL_LINES();
						}
					else
						{
						BSP_LCD_FillRect(12, 18, 349, 171);		
						int_BIx_DRAW_ALL_LINES(0);	
						}
					return;
					}
				}
			else																								//One line update++
				{
				if(TOTAL_TRACKS_IN_CURRENT_PLAYLIST>9 || B0CursorPos<(TOTAL_TRACKS_IN_CURRENT_PLAYLIST-1))
					{
					if(dSHOW==BROWSER)
						{	
						int_B_DRAW_ONE_LINE(UPDOWN);
						}
					else
						{
						int_BI_DRAW_ONE_LINE(UPDOWN);	
						}
					}
				return;
				}
			}
		else if(UPDOWN==BROWSER0_DOWN)
			{
			if(B0CursorPos==0)											//All lines update--
				{
				if(BTrackPos == 1)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x0000);
					BSP_LCD_SetFont(&Font15P);
					BTrackPos--;
					if(dSHOW==BROWSER)
						{
						BSP_LCD_FillRect(12, 18, 468, 171);		
						int_B_DRAW_ALL_LINES();
						}
					else
						{
						BSP_LCD_FillRect(12, 18, 349, 171);		
						int_BIx_DRAW_ALL_LINES(0);	
						}	
					return;						
					}	
				}
			else														//One line update--
				{
				if(dSHOW==BROWSER)
					{	
					int_B_DRAW_ONE_LINE(UPDOWN);
					}
				else
					{
					int_BI_DRAW_ONE_LINE(UPDOWN);	
					}	
				return;			
				}	
			}	
		else if(UPDOWN==BROWSER1_UP)
			{				
			if(B1CursorPos==8)												//All lines update++
				{
				if(BPlaylistPos == TOTAL_TRACKLISTS-8)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x0000);
					BSP_LCD_FillRect(12, 18, 468, 171);	
					BSP_LCD_SetFont(&Font15P);
					BPlaylistPos++;
					int_BIx_DRAW_ALL_LINES(1);
					return;
					}
				}
			else																								//One line update++
				{
				if(TOTAL_TRACKLISTS>9 || B1CursorPos<(TOTAL_TRACKLISTS-1))
					{
					int_B1_DRAW_ONE_LINE(UPDOWN);	 
					}
				return;
				}	
			}
		else if(UPDOWN==BROWSER1_DOWN)
			{
			if(B1CursorPos==0)											//All lines update--
				{
				if(BPlaylistPos == 1)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x0000);
					BSP_LCD_FillRect(12, 18, 468, 171);	
					BSP_LCD_SetFont(&Font15P);
					BPlaylistPos--;
					int_BIx_DRAW_ALL_LINES(1);	
					return;						
					}	
				}
			else														//One line update--
				{
				int_B1_DRAW_ONE_LINE(UPDOWN);	 
				return;			
				}	
			}	
		else if(UPDOWN==BROWSER2_UP)
			{
			if(B2CursorPos==4)												//All lines update++
				{
				return;	
				}
			else																								//One line update++
				{
				int_B2_DRAW_ONE_LINE(UPDOWN);	 
				return;
				}			
			}
		else if(UPDOWN==BROWSER2_DOWN)
			{	
			if(B2CursorPos==0)											//All lines update--
				{
				return;	
				}
			else														//One line update--
				{
				int_B2_DRAW_ONE_LINE(UPDOWN);	 
				return;			
				}		
			}
		return;
		}
	else if(dSHOW==TAG_LIST || dSHOW==TAG_LIST_INFO)	///////////////////TAG LIST mode selected
		{
		if(UPDOWN==TAGLIST_UP)
			{
			if(TCursorPos==8)												//All lines update++
				{
				if(TTrackPos == TOTAL_TRACKS_IN_TAGLIST-8)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x0000);	
					BSP_LCD_SetFont(&Font15P);
					TTrackPos++;
					if(dSHOW==TAG_LIST)
						{
						BSP_LCD_FillRect(12, 18, 468, 171);		
						int_T_DRAW_ALL_LINES();
						}
					else
						{
						BSP_LCD_FillRect(12, 18, 349, 171);		
						int_TI_DRAW_ALL_LINES();	
						}
					return;
					}
				}
			else																								//One line update++
				{
				if(TOTAL_TRACKS_IN_TAGLIST>9 || TCursorPos<(TOTAL_TRACKS_IN_TAGLIST-1))
					{
					if(dSHOW==TAG_LIST)
						{	
						int_T_DRAW_ONE_LINE(UPDOWN);
						}
					else
						{
						int_TI_DRAW_ONE_LINE(UPDOWN);	
						}
					}
				return;
				}
			}
		else if(UPDOWN==TAGLIST_DOWN)
			{
			if(TCursorPos==0)											//All lines update--
				{
				if(TTrackPos == 1)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x0000);
					BSP_LCD_SetFont(&Font15P);
					TTrackPos--;
					if(dSHOW==TAG_LIST)
						{
						BSP_LCD_FillRect(12, 18, 468, 171);	
						int_T_DRAW_ALL_LINES();
						}
					else
						{
						BSP_LCD_FillRect(12, 18, 349, 171);		
						int_TI_DRAW_ALL_LINES();	
						}	
					return;						
					}	
				}
			else														//One line update--
				{
				if(dSHOW==TAG_LIST)
					{	
					int_T_DRAW_ONE_LINE(UPDOWN);
					}
				else
					{
					int_TI_DRAW_ONE_LINE(UPDOWN);	
					}	
				return;			
				}	
			}	
		return;	
		}
	else if(dSHOW==UTILITY)		///////////////////////UTILITY mode selected
		{
		if(UPDOWN==UTILITY_UP)
			{
			if(UCurrentCursorPosition==8)												//All lines update++
				{
				if(CurrentUPosition == TOTAL_U_POSITIONS-8)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x0000);
					BSP_LCD_FillRect(12, 18, 468, 171);	
					BSP_LCD_SetFont(&Font15P);
					CurrentUPosition++;
					int_U_DRAW_ALL_LINES();
					return;
					}
				}
			else if(TOTAL_U_POSITIONS>9 || UCurrentCursorPosition<(TOTAL_U_POSITIONS-1))								//One line update++
				{
				int_U_DRAW_ONE_LINE(UPDOWN);
				return;
				}
			}
		else if(UPDOWN==UTILITY_DOWN)
			{
			if(UCurrentCursorPosition==0)											//All lines update--
				{
				if(CurrentUPosition == 1)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x0000);
					BSP_LCD_FillRect(12, 18, 468, 171);
					BSP_LCD_SetFont(&Font15P);
					CurrentUPosition--;
					int_U_DRAW_ALL_LINES();
					return;						
					}	
				}
			else														//One line update--
				{
				int_U_DRAW_ONE_LINE(UPDOWN);
				return;			
				}	
			}	
		return;
		}
	return;
	};

void DrawStaticTime(uint8_t dk)
	{
	uint16_t h;
	if(dk==dkA)
		{
		h = 0;
		}
	else
		{
		h = 296;	
		}	
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(63+h, 176, 2, 3);	
	BSP_LCD_FillRect(63+h, 183, 2, 3);
	BSP_LCD_FillRect(134+h, 185, 2, 3);
	BSP_LCD_SetFont(&FontBMP);
	BSP_LCD_SetTextColor(LCD_COLOR_GGRAY);	
	sprintf((char*)Buf, "%s", "-");						//M	
	BSP_LCD_DisplayStringAt(55+h, 181, Buf, TRANSPARENT_MODE);		
	sprintf((char*)Buf, "%s", ".");						//S	
	BSP_LCD_DisplayStringAt(101+h, 181, Buf, TRANSPARENT_MODE);		
	sprintf((char*)Buf, "%s", "/");						//F	
	BSP_LCD_DisplayStringAt(152+h, 181, Buf, TRANSPARENT_MODE);	
	sprintf((char*)Buf, "%s", "0");						//%	
	if(dk==dkA)
		{
		BSP_LCD_DisplayStringAt(227, 180, Buf, TRANSPARENT_MODE);	
		}
	else
		{
		BSP_LCD_DisplayStringAt(304, 180, Buf, TRANSPARENT_MODE);	
		}				
	return;	
	};	
	
//////////////////////////////////////////////	
//		
//function drawing key
//
// 
//	
void DrawKey(uint8_t dk, uint8_t k, uint8_t col)
	{	
	uint8_t h;	
	if(dk==dkA)
		{
		h = 0;	
		}		
	else
		{
		h = 32;	
		}		
	if(col==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BGREEN);	
		}			
	BSP_LCD_FillRect(210+h, 204, 28, 9);
	BSP_LCD_DrawLine(209+h, 205, 209+h, 211);	
	BSP_LCD_DrawLine(238+h, 205, 238+h, 211);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
	BSP_LCD_SetFont(&Font7W);
	if(k>25 || k==0)
		{
		return;		
		}
	sprintf((char*)Buf, "%s", KEYS[k-1]);	
	Buf[4] = 0;		//3 symbols only	
	BSP_LCD_DisplayStringAt(KEY_FONT_W[KEYSmatchtable[k-1]]+h, 205, Buf, TRANSPARENT_MODE);			
	return;	
	};	
	
	
/////////////////////////////////	
//
//internal function for show KEY 	
void int_VALUE_to_KEY(uint8_t val)
	{
	if(val>25 || val==0)
		{
		sprintf((char*)Buf, "%s", " ");
		Buf[1] = 0;	
		return;		
		}
	sprintf((char*)Buf, "%s", KEYS[val-1]);	
	Buf[4] = 0;	
	return;	
	};	
	
	
//////////////////////////////////////////////	
//
//phase sync bar static on lay 0
//	
void DrawPhasebarStatic(void)
	{
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
	BSP_LCD_DrawRect(162, 5, 157, 7);	
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);	
	BSP_LCD_FillRect(163, 6, 155, 5);		
	ForceDrawVLine(201, 6, 5, LCD_COLOR_WHITE);
	ForceDrawVLine(240, 6, 5, LCD_COLOR_WHITE);	
	ForceDrawVLine(279, 6, 5, LCD_COLOR_WHITE);		
	};
	
	
//////////////////////////////////////////////	
//
//phase sync bar static on lay 0
//	input: 0...77...155	
//
//		
void DrawPhasebar(uint8_t pos)
	{			
	if(pos>154)
		{
		return;	
		}		
	if(pos!=prev_phase_pos)
		{	
		//pos = (prev_phase_pos+pos)/2;
		if(pos>77)
			{
			if(pos>prev_phase_pos)
				{
				if(prev_phase_pos<77)
					{
					BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);		
					BSP_LCD_FillRect(163+prev_phase_pos, 7, 78-prev_phase_pos, 3);		
					if(prev_phase_pos<=38)
						{					
						ForceDrawVLine(201, 7, 3, LCD_COLOR_WHITE);	
						}	
					}	
				BSP_LCD_SetTextColor(LCD_COLOR_CUE);
				if(prev_phase_pos<=77)
					{
					BSP_LCD_FillRect(241, 7, pos-78, 3);		
					}					
				else
					{	
					BSP_LCD_FillRect(163+prev_phase_pos, 7, pos-prev_phase_pos, 3);	
					}
				if(prev_phase_pos<=116 && pos>116)
					{
					ForceDrawVLine(279, 7, 3, LCD_COLOR_GRAY);	//	
					}				
				}
			else
				{
				BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);		
				BSP_LCD_FillRect(164+pos, 7, prev_phase_pos-pos, 3);		
				if(prev_phase_pos>=116 && pos<116)
					{
					ForceDrawVLine(279, 7, 3, LCD_COLOR_WHITE);		
					}		
				}	
			}
		else		//pos<=77
			{
			if(pos>prev_phase_pos)
				{
				BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);		
				BSP_LCD_FillRect(163+prev_phase_pos, 7, pos-prev_phase_pos, 3);		
				if(prev_phase_pos<=38 && pos>38)
					{
					ForceDrawVLine(201, 7, 3, LCD_COLOR_WHITE);		
					}			
				}
			else
				{
				if(prev_phase_pos>77)
					{
					BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);		
					BSP_LCD_FillRect(241, 7, prev_phase_pos-77, 3);
					if(prev_phase_pos>=116)
						{					
						ForceDrawVLine(279, 7, 3, LCD_COLOR_WHITE);	
						}	
					}	
				BSP_LCD_SetTextColor(LCD_COLOR_CUE);		
				if(prev_phase_pos>=77)
					{
					BSP_LCD_FillRect(164+pos, 7, 77-pos, 3);		
					}					
				else
					{	
					BSP_LCD_FillRect(164+pos, 7, prev_phase_pos-pos, 3);	
					}
				if(prev_phase_pos>=38 && pos<38)
					{
					ForceDrawVLine(201, 7, 3, LCD_COLOR_GRAY);	//	
					}						
				}		
			}	
		ForceDrawVLine(163+pos, 7, 3, LCD_COLOR_RED);
		prev_phase_pos = pos;	
		}		
	return;	
	};		
	
/////////////////////////////////	
//
//internal function for UTILITY
void int_U_DRAW_ALL_LINES(void)
	{
	uint16_t E;
	for(E=0;E<9 && TOTAL_U_POSITIONS>(E+CurrentUPosition-1);E++)
		{
		if(E==UCurrentCursorPosition)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			}
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			}
		BSP_LCD_DisplayStringAt(35, 21+(19*E), UTILITY_BASE[E+CurrentUPosition-1], TRANSPARENT_MODE);
		UTILITY_PARAMETER(E+CurrentUPosition-1);	
		BSP_LCD_DisplayStringAt(292, 21+(19*E),Buf, TRANSPARENT_MODE);			
		}	
	BSP_LCD_SelectLayer(0);
	ReDrawScroll(TOTAL_U_POSITIONS, CurrentUPosition);	
	};	
	
/////////////////////////////////	
//
//internal function for UTILITY
void int_U_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{
	BSP_LCD_SetTextColor(LCD_COLOR_BGUTILITY);				
	BSP_LCD_FillRect(12, 18+(19*UCurrentCursorPosition), 258, 19);
	BSP_LCD_FillRect(271, 18+(19*UCurrentCursorPosition), 208, 19);	
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
	BSP_LCD_DrawLine(12, 18+(19*UCurrentCursorPosition), 479, 18+(19*UCurrentCursorPosition));
	BSP_LCD_DrawLine(12, 18+(19*UCurrentCursorPosition), 12, 37+(19*UCurrentCursorPosition));
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_DisplayStringAt(35, 21+(19*UCurrentCursorPosition), UTILITY_BASE[UCurrentCursorPosition+CurrentUPosition-1], TRANSPARENT_MODE);	
	UTILITY_PARAMETER(UCurrentCursorPosition+CurrentUPosition-1);	
	BSP_LCD_DisplayStringAt(292, 21+(19*UCurrentCursorPosition),Buf, TRANSPARENT_MODE);		

	if(UPDOWN==UTILITY_UP)
		{
		UCurrentCursorPosition++;
		}
	else
		{
		UCurrentCursorPosition--;	
		}
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(35,21+(19*UCurrentCursorPosition), UTILITY_BASE[UCurrentCursorPosition+CurrentUPosition-1], TRANSPARENT_MODE);
	UTILITY_PARAMETER(UCurrentCursorPosition+CurrentUPosition-1);	
	BSP_LCD_DisplayStringAt(292, 21+(19*UCurrentCursorPosition),Buf, TRANSPARENT_MODE);				
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (18+(19*UCurrentCursorPosition)), 258, 9);			////Draw selected cursor	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (27+(19*UCurrentCursorPosition)), 258, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (32+(19*UCurrentCursorPosition)), 258, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_3);		
	BSP_LCD_FillRect(271, (18+(19*UCurrentCursorPosition)), 208, 9);			////Draw selected cursor	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_5);
	BSP_LCD_FillRect(271, (27+(19*UCurrentCursorPosition)), 208, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_4);
	BSP_LCD_FillRect(271, (32+(19*UCurrentCursorPosition)), 208, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	FillTriangle(20, 20, 27, 20+(19*UCurrentCursorPosition), 34+(19*UCurrentCursorPosition), 27 +(19*UCurrentCursorPosition));		
	return;			
	};		
	
/////////////////////////////////	
//
//internal function for UTILITY for change parameter
void int_U_REDRAW_ONE_LINE(void)							
	{			
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetTextColor(0x0000);	
	BSP_LCD_FillRect(271, 18+(19*UCurrentCursorPosition), 208, 19);		
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	UTILITY_PARAMETER(UCurrentCursorPosition+CurrentUPosition-1);	
	BSP_LCD_DisplayStringAt(292, 21+(19*UCurrentCursorPosition),Buf, TRANSPARENT_MODE);
	BSP_LCD_SelectLayer(0);		
	return;			
	};			
	
//////////////////////////////////////////
//
//	returns the text of the parameter value
//write to Buf[] name state parameter for utility
//
void UTILITY_PARAMETER(uint8_t n_prmtr)
	{
	switch (n_prmtr)
		{
		case 0:		//DECKS
			{
			Buf[0] = 0;				
			break;	
			}	
		case 1:		//PLAY MODE
			{
			if(UT_SET[0]==0)
				{
				sprintf((char*)Buf, "%s", "SINGLE");	
				}		
			else
				{
				sprintf((char*)Buf, "%s", "CONTINUE");
				}					
			break;	
			}
		case 2:		//LOAD LOCK
			{
			if(UT_SET[1]==0)
				{
				sprintf((char*)Buf, "%s", "UNLOCK");		
				}
			else
				{
				sprintf((char*)Buf, "%s", "LOCK");		
				}
			break;	
			}	
		case 3:		//AUTO CUE MODE
			{
			switch (UT_SET[2])
				{
				case 0:	
					{
					sprintf((char*)Buf, "%s", "-36dB");	
					break;		
					}	
				case 1:	
					{
					sprintf((char*)Buf, "%s", "-42dB");	
					break;		
					}	
				case 2:	
					{
					sprintf((char*)Buf, "%s", "-48dB");	
					break;		
					}	
				case 3:	
					{
					sprintf((char*)Buf, "%s", "-54dB");	
					break;		
					}	
				case 4:	
					{
					sprintf((char*)Buf, "%s", "-60dB");	
					break;		
					}	
				case 5:	
					{
					sprintf((char*)Buf, "%s", "-66dB");	
					break;		
					}	
				case 6:	
					{
					sprintf((char*)Buf, "%s", "-72dB");	
					break;		
					}	
				case 7:	
					{
					sprintf((char*)Buf, "%s", "-78dB");	
					break;		
					}	
				case 8:	
					{
					sprintf((char*)Buf, "%s", "MEMORY");	
					break;		
					}	
				case 9:	
					{
					sprintf((char*)Buf, "%s", "FIRST BEAT");	
					break;		
					}	
			default: break;		
				}
			break;		
			}		
		case 4:		//QUANTIZE
			{
			if(UT_SET[3]==0)
				{
				sprintf((char*)Buf, "%s", "OFF");	
				}		
			else
				{
				sprintf((char*)Buf, "%s", "ON");
				}					
			break;	
			}
		case 5:		//TEMPO RANGE DEFAULT
			{
			if(UT_SET[4]==0)
				{
				sprintf((char*)Buf, "%s", "6%");	
				}	
			else if(UT_SET[4]==1)
				{
				sprintf((char*)Buf, "%s", "10%");	
				}	
			else if(UT_SET[4]==2)
				{
				sprintf((char*)Buf, "%s", "16%");	
				}	
			else
				{
				sprintf((char*)Buf, "%s", "WIDE");	
				}				
			break;			
			}	
		case 6:		//COLOR WAVEFORM
			{
			if(UT_SET[5]==0)
				{
				sprintf((char*)Buf, "%s", "BLUE");	
				}		
			else
				{
				sprintf((char*)Buf, "%s", "RGB");	
				}					
			break;			
			}	
		case 7:		//WAVEFORM POSITION
			{
			if(UT_SET[6]==0)
				{
				sprintf((char*)Buf, "%s", "CENTER");	
				}		
			else
				{
				sprintf((char*)Buf, "%s", "LEFT");	
				}					
			break;			
			}
		case 8:		//VINYL START 
			{
			sprintf((char*)Buf, "%01lu", UT_SET[7]);				
			break;			
			}
		case 9:		//VINYL BRAKE 
			{
			sprintf((char*)Buf, "%01lu", UT_SET[8]);				
			break;			
			}
		case 10:		//JOG ON AIR
			{
			if(UT_SET[9]==0)
				{
				sprintf((char*)Buf, "%s", "OFF");	
				}		
			else
				{
				sprintf((char*)Buf, "%s", "ON");
				}					
			break;	
			}		
		case 11:		//JOG COLOR ON AIR
			{
			if(UT_SET[10]==0)
				{
				sprintf((char*)Buf, "%s", "WHITE");	
				}		
			else if(UT_SET[10]==1)
				{
				sprintf((char*)Buf, "%s", "PINK");
				}					
			else if(UT_SET[10]==2)
				{
				sprintf((char*)Buf, "%s", "RED");
				}	
			else if(UT_SET[10]==3)
				{
				sprintf((char*)Buf, "%s", "ORANGE");
				}	
			else if(UT_SET[10]==4)
				{
				sprintf((char*)Buf, "%s", "YELLOW");
				}	
			else if(UT_SET[10]==5)
				{
				sprintf((char*)Buf, "%s", "GREEN");
				}	
			else if(UT_SET[10]==6)
				{
				sprintf((char*)Buf, "%s", "AQUA");
				}	
			else if(UT_SET[10]==7)
				{
				sprintf((char*)Buf, "%s", "BLUE");
				}	
			else if(UT_SET[10]==8)
				{
				sprintf((char*)Buf, "%s", "PURPLE");
				}	
			else
				{
				sprintf((char*)Buf, "%s", "TRACK COLOR");
				}	
			break;	
			}			
		case 12:		//JOG COLOR INACTIVE
			{
			if(UT_SET[11]==0)
				{
				sprintf((char*)Buf, "%s", "WHITE");	
				}		
			else if(UT_SET[11]==1)
				{
				sprintf((char*)Buf, "%s", "PINK");
				}					
			else if(UT_SET[11]==2)
				{
				sprintf((char*)Buf, "%s", "RED");
				}	
			else if(UT_SET[11]==3)
				{
				sprintf((char*)Buf, "%s", "ORANGE");
				}	
			else if(UT_SET[11]==4)
				{
				sprintf((char*)Buf, "%s", "YELLOW");
				}	
			else if(UT_SET[11]==5)
				{
				sprintf((char*)Buf, "%s", "GREEN");
				}	
			else if(UT_SET[11]==6)
				{
				sprintf((char*)Buf, "%s", "AQUA");
				}	
			else if(UT_SET[11]==7)
				{
				sprintf((char*)Buf, "%s", "BLUE");
				}	
			else if(UT_SET[11]==8)
				{
				sprintf((char*)Buf, "%s", "PURPLE");
				}	
			else
				{
				sprintf((char*)Buf, "%s", "TRACK COLOR");
				}	
			break;	
			}	
		case 13:		//JOG BRIGHTNESS
			{
			sprintf((char*)Buf , "%0lu", UT_SET[12]+1);			
			break;	
			}
		case 14:		//LCD BRIGHTNESS
			{
			sprintf((char*)Buf , "%0lu", UT_SET[13]+1);			
			break;	
			}				
		case 15:		//PADS MODE 
			{
			if(UT_SET[14]==0)
				{
				sprintf((char*)Buf, "%s", "HOT CUE");	
				}		
			else
				{
				sprintf((char*)Buf, "%s", "GATE CUE");
				}					
			break;		
			}			
		case 16:		//SPACE
			{
			Buf[0] = 0;				
			break;	
			}		
		case 17:		//MIXER
			{
			Buf[0] = 0;				
			break;	
			}		
		case 18:		//FADER CURVE 
			{
			if(UT_SET[15]==0)
				{
				sprintf((char*)Buf, "%s", "LINEAR");	
				}		
			else
				{
				sprintf((char*)Buf, "%s", "SLOW");
				}					
			break;		
			}			
		case 19:		//SPEAKERS  
			{
			if(spk_on==1)
				{
				sprintf((char*)Buf, "%s", "ON");		
				}				
			else
				{
				sprintf((char*)Buf, "%s", "OFF");		
				}
			break;		
			}	
		case 20:		//EQ LOW-MID
			{
			sprintf((char*)Buf , "%02lu Hz", 5*UT_SET[16]+20);			
			break;	
			}	
		case 21:		//EQ MID-HIGH 
			{
			sprintf((char*)Buf , "%02lu Hz", 50*UT_SET[17]+2000);			
			break;	
			}		
		case 22:		//HEADPHONES CUE
			{
			if(UT_SET[18]==0)
				{
				sprintf((char*)Buf, "%s", "PRE EQ");	
				}		
			else
				{
				sprintf((char*)Buf, "%s", "POST EQ");
				}					
			break;		
			}		
		case 23:		//SPACE
			{
			Buf[0] = 0;				
			break;	
			}	
		case 24:		//VERSION No.
			{
			Buf[0] = 0;				
			break;	
			}
		case 25:		//DECK 1
			{
			sprintf((char*)Buf, "Ver. %01lu.%02lu", (deckRbuf[4]&0x7F)/100, (deckRbuf[4]&0x7F)%100);					
			break;	
			}		
		case 26:		//DECK 2
			{
			sprintf((char*)Buf, "Ver. %01lu.%02lu", (deckRbuf[12]&0x7F)/100, (deckRbuf[12]&0x7F)%100);					
			break;	
			}			
		case 27:		//MAIN ASSY
			{
			sprintf((char*)Buf, "%s", "Ver. ");
			Buf[5] = FIRMWARE_VERSION[0];
			Buf[6] = FIRMWARE_VERSION[1];
			Buf[7] = FIRMWARE_VERSION[2];
			Buf[8] = FIRMWARE_VERSION[3];						
			Buf[9] = 0;
			break;	
			}
		case 28:		//MIXER ASSY
			{
			sprintf((char*)Buf, "%s", "Ver. 0.51");
			break;	
			}		
		case 29:		//POWER MANAGER
			{
			sprintf((char*)Buf, "%s", "Ver. 0.29");
			break;	
			}				
		case 30:		//SPACE
			{
			Buf[0] = 0;				
			break;	
			}	
		case 31:		//GENERAL 
			{
			Buf[0] = 0;				
			break;	
			}	
		case 32:		//BATT. INFO 
			{
			sprintf((char*)Buf , "%01lu%s", u_battery_level, "%");			
			break;	
			}
		case 33:		//DEVICE UID
			{
			sprintf((char*)&Buf[0] , "%08lX", DEVICE_UID[0]);
			sprintf((char*)&Buf[4] , "%08lX", DEVICE_UID[1]);
			sprintf((char*)&Buf[8] , "%08lX", DEVICE_UID[2]);				
			break;	
			}
		case 34:		//DEVELOPER
			{
			sprintf((char*)Buf, "%s", "ANATSKA ANDREI");	
			break;	
			}	
		default:
			break;	
		}	
	return;	
	};			
	

/////////////////////////////////	
//
//internal function for UTILITY for realtime reload parameters
//
//	
void int_reload_parameter_realtime(void)
	{
	extern void SET_JOG_COLOR(uint8_t dk, uint32_t col);	
		
	if(UCurrentCursorPosition+CurrentUPosition==9)				//VINYL START
		{
		acceleration_UP = UT_SET[7];	
		}
	else if(UCurrentCursorPosition+CurrentUPosition==10)				//VINYL BRAKE
		{
		acceleration_DOWN = UT_SET[8];	
		}
	else if(UCurrentCursorPosition+CurrentUPosition==15)				//LCD BRIGHTNESS
		{	
		TIM8->CCR1 = LCDPWM_MAP[UT_SET[13]];
		}
	else if((UCurrentCursorPosition+CurrentUPosition)>10 && (UCurrentCursorPosition+CurrentUPosition)<15)				//JOG		
		{
		if(prev_inair[dkA]==0)		//inactive
			{
			if(UT_SET[11]<9)
				{
				jog_work_color[dkA] = jog_color[UT_SET[12]][UT_SET[11]];	
				}					
			else
				{
				jog_work_color[dkA] = jog_color[UT_SET[12]][curr_trck_color[dkA]];		
				}
			}
		else		//in air
			{
			if(UT_SET[10]<9)
				{
				jog_work_color[dkA] = jog_color[UT_SET[12]][UT_SET[10]];	
				}					
			else
				{
				jog_work_color[dkA] = jog_color[UT_SET[12]][curr_trck_color[dkA]];		
				}		
			}				
		SET_JOG_COLOR(dkA, jog_work_color[dkA]);	

		if(prev_inair[dkB]==0)		//inactive
			{
			if(UT_SET[11]<9)
				{
				jog_work_color[dkB] = jog_color[UT_SET[12]][UT_SET[11]];	
				}					
			else
				{
				jog_work_color[dkB] = jog_color[UT_SET[12]][curr_trck_color[dkB]];		
				}
			}
		else		//in air
			{
			if(UT_SET[10]<9)
				{
				jog_work_color[dkB] = jog_color[UT_SET[12]][UT_SET[10]];	
				}					
			else
				{
				jog_work_color[dkB] = jog_color[UT_SET[12]][curr_trck_color[dkB]];		
				}		
			}				
		SET_JOG_COLOR(dkB, jog_work_color[dkB]);
		}

		
//	else if(UCurrentCursorPosition+CurrentUPosition==8)			//change waveform color
//		{
//		if(track_play_now!=0)
//			{
//			DrawStaticWFM(DRAW_NEW_STATIC_WAVEFORM);
//			if(REMAIN_ENABLE)
//				{
//				DrawStaticWFM(REDRAW_IN_REMAIN_MODE);		
//				}
//			else
//				{
//				DrawStaticWFM(REDRAW_IN_NREMAIN_MODE);	
//				}	
//			DrawMinuteMarkers();	
//			}
//		}
	

//	else if(UCurrentCursorPosition+CurrentUPosition==11)			//change jog brightness
//		{
//		TIM12->CCR1 = JOG_BRIGHTNESS[UTILITY_SETTINGS[10]];
//		}	
//	else if(UCurrentCursorPosition+CurrentUPosition==3)			//change auto cue level
//		{
//		if(UTILITY_SETTINGS[2]==8)
//			{
//			ShowACUE(2);	
//			}
//		else
//			{
//			ShowACUE(1);	
//			}
//		}		
//	else if(UCurrentCursorPosition+CurrentUPosition==10)			//jog indicator
//		{
//		TIM12->CCR1 = JOG_BRIGHTNESS[UTILITY_SETTINGS[10]];
//		}		
	};		
	
	
	
/////////////////////////////////	
//
// rekordbox logo
//
//		
void DrawREKORDBOX(void)
	{
	uint16_t i;
	for(i=0;i<10496;i++)
		{
		if((rb_logo[2*i+1]!=0x80) || (rb_logo[2*i]!=0))
			{
			BSP_LCD_DrawPixel(112+(i%256), 47+(i>>8), 256*rb_logo[2*i+1] + rb_logo[2*i]);	
			}	
		}	
	};		
	
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



