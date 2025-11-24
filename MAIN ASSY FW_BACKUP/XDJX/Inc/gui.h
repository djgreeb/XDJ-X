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
void SwitchInformationLayer(uint8_t LAY);
void int_DRAW_TRANSPARENT_BAR(void);
void intDrawLayer0_NOINFO(uint8_t CurrentCursorPosition);	//draw layer 0 for without INFO BROWSER and TAGLIST
void int_B_DRAW_ALL_LINES(void);													//internal function for Browser
void int_B_DRAW_ONE_LINE(uint8_t UPDOWN);									//internal function for Browser
void ReDrawScroll(uint16_t total_elements, uint16_t current_element_pos);
void NAVIGATOR(uint8_t UPDOWN);														//Navigate browser, TAG LIST and UTILITY	
void DrawPhasebarStatic(void);									//phase sync bar static
void DrawPhasebar(uint8_t pos);									//phase bar


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
				BSP_LCD_DrawPixel(n%480, (170-n/480), (0x8000+(ANIMATION[frame][n][1]>>3)+32*(ANIMATION[frame][n][2]>>3)+1024*(ANIMATION[frame][n][0]>>3)));						
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
				BSP_LCD_DrawPixel(n%480, (170-n/480), (0x8000+(ANIMATION[frame][n][1]>>3)+32*(ANIMATION[frame][n][2]>>3)+1024*(ANIMATION[frame][n][0]>>3)));	
				n++;		
				}		
			BSP_LCD_SetTransparency(1, 0);		//верхний не слой виден
			VL = 0;	
			}
		frame++;
		HAL_Delay(55);		}
		
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
	BSP_LCD_DrawLine(101, 170, 379, 170);	
	BSP_LCD_DrawLine(101, 176, 379, 176);
	BSP_LCD_DrawLine(100, 171, 100, 175);
	BSP_LCD_DrawLine(380, 171, 380, 175);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	n = 0;	
	while(n<277)
		{
		BSP_LCD_DrawLine(102+n, 172, 102+n, 174);
		n++;	
		HAL_Delay(11);	
		}	
	HAL_Delay(600);		
	return;	
	};		
	
	
//////////////////////////////////////////////	
//	
//	internal function for redraw waveform	
//	
void intDRAW_WAVEFORM_FRAME(uint8_t dk, uint32_t position)
	{
	uint8_t h;	
	if(dk==dkA)
		{
		h = 0;
		}
	else
		{
		h = 50;	
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
	uint16_t i, adr, BG_COLOR;
	uint16_t u, x, mn;
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
		
	/////	
	//	
	//	need insert delay between BSP_LCD_FillRect's
	//
	/////	
		
	//BSP_LCD_FillRect(0, 65+h, 480, 5);			//optimization	
	BSP_LCD_DrawHLine(0, 65+h, 480);	
	BSP_LCD_DrawHLine(0, 65+h, 480);	
	BSP_LCD_DrawHLine(0, 66+h, 480);	
	BSP_LCD_DrawHLine(0, 67+h, 480);	
	BSP_LCD_DrawHLine(0, 68+h, 480);	
	BSP_LCD_DrawHLine(0, 69+h, 480);	
		
	for(i=0;i<480;i++)
		{	
		adr = DynamicWaveformZOOM*(i+position-(240-l));	
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
			if(adr<=all_long[dk])
				{
				uint8_t amplitude = (WFORMDYNAMIC[dk][adr]&0x1F);	
				if(amplitude>18)
					{
					amplitude = 18;	
					}
				if(DynamicWaveformZOOM==1)
					{	
					ForceDrawVLine(i, (46-amplitude)+h, 1+2*amplitude, COLOR_MAP[0][WFORMDYNAMIC[dk][adr]>>5]);
					if(amplitude<18)
						{						
						ForceDrawVLine(i, 28+h, 18-amplitude, BG_COLOR);	
						ForceDrawVLine(i, 47+amplitude+h, 18-amplitude, BG_COLOR);
						}
					}
				else 	
					{
					uint8_t color = (WFORMDYNAMIC[dk][adr]>>5);
					for(j=0;j<(DynamicWaveformZOOM-1);j++)
						{
						if((WFORMDYNAMIC[dk][adr+j+1]&0x1F)>amplitude)
							{
							amplitude	= (WFORMDYNAMIC[dk][adr+j+1]&0x1F);
							if(amplitude>18)
								{
								amplitude = 18;	
								}		
							if(amplitude>13)
								{
								color = (WFORMDYNAMIC[dk][adr+j+1]>>5);
								}
							}
						}		
					ForceDrawVLine(i, (46-amplitude)+h, 1+2*amplitude, COLOR_MAP[0][color]);
					if(amplitude<18)
						{
						ForceDrawVLine(i, 28+h, 18-amplitude, BG_COLOR);	
						ForceDrawVLine(i, 47+amplitude+h, 18-amplitude, BG_COLOR);
						}
					}
				}
			else
				{
				ForceDrawVLine(i, 28+h, 37, LCD_COLOR_BLACK);
				}	
			}			
		}

	if(dk==dkA)
		{		
		if(RED_VERTICAL_LINE[dk])							//detecting touch on sensor or touch on jog 
			{
			ForceDrawVLine(239-l, 19, 51, LCD_COLOR_RED);	
			ForceDrawVLine(240-l, 19, 51, LCD_COLOR_RED);		
			}
		else
			{
			ForceDrawVLine(239-l, 19, 51, LCD_COLOR_WHITE);	
			ForceDrawVLine(240-l, 19, 51, LCD_COLOR_WHITE);	
			}	
		}
	else
		{	
		if(RED_VERTICAL_LINE[dk])							//detecting touch on sensor or touch on jog 
			{
			ForceDrawVLine(239-l, 70, 51, LCD_COLOR_RED);	
			ForceDrawVLine(240-l, 70, 51, LCD_COLOR_RED);		
			}
		else
			{
			ForceDrawVLine(239-l, 70, 51, LCD_COLOR_WHITE);	
			ForceDrawVLine(240-l, 70, 51, LCD_COLOR_WHITE);	
			}		
		}
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
			Prev1m[dk] = 0xFF;
			Prev10s[dk] = 0xFF;
			Prev1s[dk] = 0xFF;
			Prev10f[dk] = 0xFF;
			Prev1f[dk] = 0xFF;
			PrevHf[dk] = 0xFF;				
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
			if(RED_VERTICAL_LINE[dk]==0)
				{
				RED_VERTICAL_LINE[dk] = 1;	
				forcibly_redraw[dk] = 1;	
				}
			}
		else
			{
			if(RED_VERTICAL_LINE[dk])
				{
				RED_VERTICAL_LINE[dk] = 0;	
				forcibly_redraw[dk] = 1;	
				}
			}	
		}
	else
		{
		if(deckTbuf[5][8]&0x10)						//detecting touch on touch on jog 
			{
			if(RED_VERTICAL_LINE[dk]==0)
				{
				RED_VERTICAL_LINE[dk] = 1;	
				forcibly_redraw[dk] = 1;	
				}
			}
		else
			{
			if(RED_VERTICAL_LINE[dk])
				{
				RED_VERTICAL_LINE[dk] = 0;	
				forcibly_redraw[dk] = 1;	
				}
			}
		}		
		
	DrawStaticWFM(dk, position*203/all_long[dk]);	
	
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
	}
	
	
//////////////////////////////////////////////////
//
//	Size 203x19	
//	
//	Draw time position bar 0...202
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
		h = 271;	
		}		
	uint16_t i;
	if(Tpos<203)
		{	
		if(prevTpos[dk]!=Tpos)			
			{
			ForceDrawVLine(prevTpos[dk]+3+h, 200, 29, LCD_COLOR_BLACK);
			ForceDrawVLine(prevTpos[dk]+4+h, 200, 29, LCD_COLOR_BLACK);	
			ForceDrawVLine(prevTpos[dk]+3+h, 220-(WFORMSTATIC[dk][prevTpos[dk]]&0x1F), (WFORMSTATIC[dk][prevTpos[dk]]&0x1F)+1, 
				WS_COLOR_MAP[WFORMSTATIC[dk][prevTpos[dk]]>>7]);	
			ForceDrawVLine(prevTpos[dk]+4+h, 220-(WFORMSTATIC[dk][prevTpos[dk]+1]&0x1F), (WFORMSTATIC[dk][prevTpos[dk]+1]&0x1F)+1, 
				WS_COLOR_MAP[WFORMSTATIC[dk][prevTpos[dk]+1]>>7]);
			if(prevTpos[dk]>Tpos)						//___<<||___     moving
				{				
				i = prevTpos[dk] - Tpos;
				if(REMAIN_ENABLE[dk])
					{	
					while(i>0)
						{
						if((Tpos+i)%2==0 && (Tpos+i<201))
							{
							ForceDrawVLine(Tpos+i+5+h, 223, 4, LCD_COLOR_WHITE);	
							}
						else
							{
							ForceDrawVLine(Tpos+i+5+h, 223, 4, PBAR_COLOR_1);	
							}
							i--;	
						}
					}
				else
					{
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
					BSP_LCD_FillRect(Tpos+5+h, 224, (prevTpos[dk]-Tpos+2), 2);			
					while(i>0)
						{
						if(((Tpos+i)%2==0) && (Tpos+i<201))
							{
							BSP_LCD_DrawPixel(Tpos+i+5+h, 223, PBAR_COLOR_2);	
							BSP_LCD_DrawPixel(Tpos+i+5+h, 226, PBAR_COLOR_2);		
							}
						else if(((Tpos+i)%2==1) && (Tpos+i<201))
							{
							BSP_LCD_DrawPixel(Tpos+i+5+h, 223, PBAR_COLOR_3);	
							BSP_LCD_DrawPixel(Tpos+i+5+h, 226, PBAR_COLOR_3);		
							}
						i--;	
						}
					ForceDrawVLine(205+h, 223, 4, PBAR_COLOR_2);						//gray scroll vertical line in end					
					}
				}
			else if(prevTpos[dk]<Tpos)																			//___||>>___     moving
				{
				i = Tpos - prevTpos[dk]+1;	
				if(REMAIN_ENABLE[dk])																		
					{
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
					BSP_LCD_FillRect(prevTpos[dk]+4+h, 224, (Tpos - prevTpos[dk]+2), 2);						
					ForceDrawVLine(3+h, 223, 4, PBAR_COLOR_2);						//gray scroll vertical line in start	
					while(i>0)
						{
						if((prevTpos[dk]+i)>1)	
							{
							if((prevTpos[dk]+i)%2==0)
								{
								BSP_LCD_DrawPixel(1+prevTpos[dk]+i+h, 223, PBAR_COLOR_2);	
								BSP_LCD_DrawPixel(1+prevTpos[dk]+i+h, 226, PBAR_COLOR_2);	
								}
							else
								{
								BSP_LCD_DrawPixel(1+prevTpos[dk]+i+h, 223, PBAR_COLOR_3);	
								BSP_LCD_DrawPixel(1+prevTpos[dk]+i+h, 226, PBAR_COLOR_3);		
								}
							}
						i--;					
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
								ForceDrawVLine(1+prevTpos[dk]+i+h, 223, 4, LCD_COLOR_WHITE);		
								}
							else
								{
								ForceDrawVLine(1+prevTpos[dk]+i+h, 223, 4, PBAR_COLOR_1);		
								}
							}
						i--;					
						}
					}
				}			
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(Tpos+2+h, 223, 4, 4);					
			if(RED_VERTICAL_LINE[dk])						//detecting touch on sensor or touch on jog 
				{
				ForceDrawVLine(Tpos+3+h, 200, 29, LCD_COLOR_RED);		//red vertical scroll
				ForceDrawVLine(Tpos+4+h, 200, 29, LCD_COLOR_RED);		
				}
			else
				{
				ForceDrawVLine(Tpos+3+h, 200, 29, LCD_COLOR_WHITE);		//white vertical scroll
				ForceDrawVLine(Tpos+4+h, 200, 29, LCD_COLOR_WHITE);		
				}
			prevTpos[dk] = Tpos;	
			}
		else if(forcibly_redraw[dk]==1)
			{
			if(RED_VERTICAL_LINE[dk])						//detecting touch on sensor or touch on jog 
				{
				ForceDrawVLine(Tpos+3+h, 200, 29, LCD_COLOR_RED);		//red vertical scroll
				ForceDrawVLine(Tpos+4+h, 200, 29, LCD_COLOR_RED);			
				}
			else
				{
				ForceDrawVLine(Tpos+3+h, 200, 29, LCD_COLOR_WHITE);		//white vertical scroll
				ForceDrawVLine(Tpos+4+h, 200, 29, LCD_COLOR_WHITE);		
				}
			}
		return;	
		}
	else if(Tpos==REDRAW_IN_NREMAIN_MODE)
		{
		if(prevTpos[dk]<200)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(prevTpos[dk]+5+h, 224, 200-prevTpos[dk], 2);
			}				
		for(i=0;i<203;i++)						
			{			
			if(i<prevTpos[dk]-1)
				{
				if(i%2==0)							//drawing ||||||||||||||||||
					{
					ForceDrawVLine(i+3+h, 223, 4, LCD_COLOR_WHITE);		
					}
				else
					{
					ForceDrawVLine(i+3+h, 223, 4, PBAR_COLOR_1);		
					}
				}	
			else if(i>prevTpos[dk]+2)
				{
				if(i%2==0)						//drawing :::::::::::::::
					{
					BSP_LCD_DrawPixel(3+i+h, 223, PBAR_COLOR_2);	
					BSP_LCD_DrawPixel(3+i+h, 226, PBAR_COLOR_2);	
					}
				else
					{
					BSP_LCD_DrawPixel(3+i+h, 223, PBAR_COLOR_3);	
					BSP_LCD_DrawPixel(3+i+h, 226, PBAR_COLOR_3);		
					}	
				}
			}
		if(prevTpos[dk]<200)
			{
			ForceDrawVLine(205+h, 223, 4, PBAR_COLOR_2);						//gray scroll vertical line in end	
			}	
		DRAWN_IN_REMAIN[dk] = 0;	
		return;	
		}
	else if(Tpos==REDRAW_IN_REMAIN_MODE)
		{
		if(prevTpos[dk]>1)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(4+h, 224, prevTpos[dk]-1, 2);
			}
		for(i=0;i<203;i++)						
			{	
			if(i<prevTpos[dk]-1)
				{
				if(i%2==0)						//drawing :::::::::::::::
					{
					BSP_LCD_DrawPixel(3+i+h, 223, PBAR_COLOR_2);	
					BSP_LCD_DrawPixel(3+i+h, 226, PBAR_COLOR_2);	
					}
				else
					{
					BSP_LCD_DrawPixel(3+i+h, 223, PBAR_COLOR_3);	
					BSP_LCD_DrawPixel(3+i+h, 226, PBAR_COLOR_3);		
					}	
				}	
			else if(i>prevTpos[dk]+2)
				{
				if(i%2==0)							//drawing ||||||||||||||||||
					{
					ForceDrawVLine(i+3+h, 223, 4, LCD_COLOR_WHITE);		
					}
				else
					{
					ForceDrawVLine(i+3+h, 223, 4, PBAR_COLOR_1);		
					}
				}	
			}
		if(prevTpos[dk]>1)
			{
			ForceDrawVLine(3+h, 223, 4, PBAR_COLOR_2);						//gray scroll vertical line in start				
			}		
		DRAWN_IN_REMAIN[dk] = 1;	
		return;	
		}	
	else if(Tpos==DRAW_NEW_STATIC_WAVEFORM)						//Draw new static waveform		
		{
		prevTpos[dk] = 0;	
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(3+h, 200, 203, 33);				
		for(i=0;i<203;i++)						
			{			
			ForceDrawVLine(i+3+h, 220-(WFORMSTATIC[dk][i]&0x1F), (WFORMSTATIC[dk][i]&0x1F)+1, WS_COLOR_MAP[WFORMSTATIC[dk][i]>>7]);		
			if(REMAIN_ENABLE[dk])
				{
				if(i%2==0)
					{
					ForceDrawVLine(i+3+h, 223, 4, LCD_COLOR_WHITE);		
					}
				else
					{
					ForceDrawVLine(i+3+h, 223, 4, PBAR_COLOR_1);		
					}
				}
			else
				{
				if(i%2==0)
					{
					BSP_LCD_DrawPixel(3+i+h, 223, PBAR_COLOR_2);	
					BSP_LCD_DrawPixel(3+i+h, 226, PBAR_COLOR_2);	
					}
				else
					{
					BSP_LCD_DrawPixel(3+i+h, 223, PBAR_COLOR_3);	
					BSP_LCD_DrawPixel(3+i+h, 226, PBAR_COLOR_3);		
					}
				}
			}
		if(REMAIN_ENABLE[dk]==0)	
			{
			ForceDrawVLine(3+h, 223, 4, PBAR_COLOR_2);						//gray scroll vertical line in start	
			ForceDrawVLine(205+h, 223, 4, PBAR_COLOR_2);					//gray scroll vertical line in end	
			}
		}
	else if(Tpos==CLEAR_WAVEFORM_ARRAY)										//clear waveform array
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(1+h, 193, 208, 40);			
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
		h = 271;	
		}		
	if(type==NONE_MARK)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(h, 193, 209, 7);
		return;
		}
	else if(p<203)
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
		BSP_LCD_FillRect(h, 193, 209, 7);
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
		h = 271;	
		}			
	if(all_long[dk]<=135000) 			//15 min	
		{
		if(all_long[dk]<=67500) 			//7,5 min		
			{
			uint32_t a = 913500/all_long[dk];
			uint32_t npoint = all_long[dk]/4500;
			ForceDrawVLine(3+h, 229, 4, LCD_COLOR_WHITE);		
			while(npoint>0)
				{
				if(npoint*a<202)
					{				
					if(npoint%2==0)
						{
						ForceDrawVLine(3+(npoint*a)+h, 229, 4, LCD_COLOR_WHITE);
						}						
					else
						{
						ForceDrawVLine(3+(npoint*a)+h, 229, 3, LCD_COLOR_DARKGRAY);		
						}						
					}	
				npoint--;		
				}		
			}
		else
			{
			uint32_t a = 1827000/all_long[dk];
			uint32_t npoint = all_long[dk]/9000;
			ForceDrawVLine(3+h, 229, 4, LCD_COLOR_WHITE);		
			while(npoint>0)
				{
				if(npoint*a<202)
					{				
					ForceDrawVLine(3+(npoint*a)+h, 229, 4, LCD_COLOR_WHITE);	
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
//	Draw time position bar 0...202	
// 	p=0xFF - disable triangle 	
void DrawCueMarker(uint8_t dk, uint8_t p)
	{
	uint16_t h;	
	if(dk==dkA)
		{
		h = 0;	
		}		
	else
		{
		h = 271;	
		}				
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(h, 229, 209, 4);
	if(p<203)
		{
		DrawCTriangle(p+h, 229, LCD_COLOR_CUE);	
		}		
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
	uint8_t j;	
	if(dk==dkA)
		{
		for(j=0;j<188;j++)
			{			
			ForceDrawVLine(j, 140, 17, color_dim(188-j, 188,  LCD_COLOR_CUE));		
			}
		}
	else
		{		
		for(j=0;j<188;j++)
			{			
			ForceDrawVLine(479-j, 140, 17, color_dim(188-j, 188,  LCD_COLOR_BGREEN));		
			}		
		}
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);		
	if(dk==dkA)
		{
		BSP_LCD_FillRect(25, 123, 215, 17);
		BSP_LCD_SetTextColor(LCD_COLOR_CUE);
		BSP_LCD_DisplayStringAt(29, 124, artist_n[dk], TRANSPARENT_MODE);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
		BSP_LCD_DisplayStringAt(3, 141, track_n[dk], TRANSPARENT_MODE);		
		}
	else
		{
		BSP_LCD_FillRect(265, 123, 215, 17);		
		BSP_LCD_SetTextColor(LCD_COLOR_BGREEN);	
		BSP_LCD_DisplayStringAt(269, 124, artist_n[dk], TRANSPARENT_MODE);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
		BSP_LCD_DisplayStringAt(295, 141, track_n[dk], TRANSPARENT_MODE);			
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
		BSP_LCD_FillRect(0, 17, 480, 171);	
		int_DRAW_TRANSPARENT_BAR();
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		Buf[0] = 0x7C;			//playlist
		Buf[1] = 0x00;			//
		BSP_LCD_DisplayStringAt(4, 2, Buf, TRANSPARENT_MODE);	
		sprintf((char*)Buf, "%s", "Travel to Brest");		
		BSP_LCD_DisplayStringAt(21, 2, Buf, TRANSPARENT_MODE);		
		DrawPhasebarStatic();		
		prev_phase_pos = 77;	
			
		ShowTrackName(dkA);		
		ShowTrackName(dkB);		
		BSP_LCD_SetTextColor(LCD_COLOR_CUE);	
		BSP_LCD_FillRect(0, 123, 25, 17);	
		BSP_LCD_SetTextColor(LCD_COLOR_BGREEN);
		BSP_LCD_FillRect(240, 123, 25, 17);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		sprintf((char *)Buf , "%02lu", TRACK_n_PLAY[dkA]%100);			
		BSP_LCD_DisplayStringAt(5, 124, Buf, TRANSPARENT_MODE);
		sprintf((char *)Buf , "%02lu", TRACK_n_PLAY[dkB]%100);		
		BSP_LCD_DisplayStringAt(245, 124, Buf, TRANSPARENT_MODE);
		DrawStaticTime(dkA);
		DrawStaticTime(dkB);
		tempo_need_update[dkA] = 1;
		tempo_need_update[dkB] = 1;

		forcibly_redraw[dkA] = 1;	
		forcibly_redraw[dkB] = 1;					
							

		//DrawREKORDBOX();	
		
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
			BSP_LCD_FillRect(0, 17, 480, 171);	
			int_DRAW_TRANSPARENT_BAR();	
			intDrawLayer0_NOINFO(B0CurrentCursorPosition);	
			BSP_LCD_SelectLayer(1);	
			BSP_LCD_SetTransparency(1, 255);		
			BSP_LCD_SetFont(&Font15P);
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			sprintf((char*)Buf, "%s", "|");		
			BSP_LCD_DisplayStringAt(4, 2,Buf, TRANSPARENT_MODE);				
			sprintf((char*)Buf, "%s", TRACKLIST_NAME[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]);						
			BSP_LCD_DisplayStringAt(21, 2, Buf, TRANSPARENT_MODE);
			sprintf((char*)Buf, "%s", "Total Track");						
			BSP_LCD_DisplayStringAt(320,2,Buf, TRANSPARENT_MODE);
			sprintf((char *)Buf , "%1lu", TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition] - TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]);					
			BSP_LCD_DisplayStringAt(418,2,Buf, TRANSPARENT_MODE);		
			}
		else
			{
			BSP_LCD_SelectLayer(1);		
			BSP_LCD_SetTextColor(0x0000);	
			BSP_LCD_FillRect(50, 20+(19*B0CurrentCursorPosition), 15, 19);		
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
		BSP_LCD_Clear(0x00000000);	
		BSP_LCD_SelectLayer(0);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		BSP_LCD_FillRect(0, 0, 480, 182);	
		BSP_LCD_SetTextColor(0xFF173367);
		BSP_LCD_FillRect(0, 0, 480, 18);		
			
//		intDrawLayer0_NOINFO(TCurrentCursorPosition);
//			
		BSP_LCD_SelectLayer(1);	
		BSP_LCD_SetTransparency(1, 255);		
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		sprintf((char*)Buf, "%s", "TAG LIST");			
		BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);
		sprintf((char*)Buf, "%s", "Total Track");						
		BSP_LCD_DisplayStringAt(350,2,Buf, TRANSPARENT_MODE);
		sprintf((char *)Buf , "%01lu", TOTAL_TRACKS_IN_TAG_LIST);					
		BSP_LCD_DisplayStringAt(448,2,Buf, TRANSPARENT_MODE);	
//		int_T_DRAW_ALL_LINES();	
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
				BSP_LCD_Clear(0x00000000);	
				BSP_LCD_SelectLayer(0);
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
				BSP_LCD_FillRect(0, 0, 480, 182);		
				int_DRAW_TRANSPARENT_BAR();	
//			
//				intDrawLayer0_INFO(B0CurrentCursorPosition);
//								
				BSP_LCD_SelectLayer(1);	
				BSP_LCD_SetTransparency(1, 255);		
				BSP_LCD_SetFont(&Font15P);
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);					
				sprintf((char*)Buf, "%s", "|");		
				BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);				
				sprintf((char*)Buf, "%s", TRACKLIST_NAME[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]);						
				BSP_LCD_DisplayStringAt(23, 2, Buf, TRANSPARENT_MODE);
				sprintf((char*)Buf, "%s", "Total Track");						
				BSP_LCD_DisplayStringAt(350,2,Buf, TRANSPARENT_MODE);
				sprintf((char *)Buf , "%1lu", TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition] - TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]);					
				BSP_LCD_DisplayStringAt(448,2,Buf, TRANSPARENT_MODE);
				}
			else
				{
				BSP_LCD_SelectLayer(1);		
				BSP_LCD_SetTextColor(0x00000000);	
				BSP_LCD_FillRect(20, 20+(19*B0CurrentCursorPosition), 15, 19);		
				}
//			int_BIx_DRAW_ALL_LINES(0);	
			}
		else		
			{
			HAL_GPIO_WritePin(LED_BROWSE_GPIO_Port, LED_BROWSE_Pin, GPIO_PIN_SET);				
			HAL_GPIO_WritePin(GPIOH, LED_INFO_Pin|LED_TAGLIST_Pin|LED_MENU_Pin, GPIO_PIN_RESET);				
			BSP_LCD_SetTransparency(1, 0);	
			BSP_LCD_SelectLayer(1);
			BSP_LCD_Clear(0x00000000);	
			BSP_LCD_SelectLayer(0);
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(0, 0, 480, 182);		
//			int_DRAW_TRANSPARENT_BAR();
			
			if(BROWSE_LEVEL==1)	
				{
//				intDrawLayer0_BROWSER_1_3(B1CurrentCursorPosition);	
				BSP_LCD_SelectLayer(1);	
				BSP_LCD_SetTransparency(1, 255);		
				BSP_LCD_SetFont(&Font15P);
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				sprintf((char*)Buf, "%s", "[PLAYLIST]");						
				BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);
				}
			else if(BROWSE_LEVEL==2)	
				{
//				intDrawLayer0_BROWSER_1_3(B2CurrentCursorPosition);	
				BSP_LCD_SelectLayer(1);	
				BSP_LCD_SetTransparency(1, 255);		
				BSP_LCD_SetFont(&Font15P);
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				sprintf((char*)Buf, "%s", "/");		
				BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);				
				sprintf((char*)Buf, "%s", "SD card");						
				BSP_LCD_DisplayStringAt(23, 2, Buf, TRANSPARENT_MODE);					
				}
			else if(BROWSE_LEVEL==3)	
				{
//				intDrawLayer0_BROWSER_1_3(0);
				BSP_LCD_SelectLayer(1);	
				BSP_LCD_SetTransparency(1, 255);		
				BSP_LCD_SetFont(&Font15P);
				BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
				sprintf((char*)Buf, "%s", "SD card slot");						
				BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);
				}	
//			int_BIx_DRAW_ALL_LINES(BROWSE_LEVEL);	
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
		BSP_LCD_Clear(0x00000000);	
		BSP_LCD_SelectLayer(0);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
		BSP_LCD_FillRect(0, 0, 480, 182);				
		BSP_LCD_SetTextColor(0xFF173367);
		BSP_LCD_FillRect(0, 0, 480, 18);		
		
//		intDrawLayer0_INFO(TCurrentCursorPosition);

		BSP_LCD_SelectLayer(1);	
		BSP_LCD_SetTransparency(1, 255);		
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		sprintf((char*)Buf, "%s", "TAG LIST");			
		BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);
		sprintf((char*)Buf, "%s", "Total Track");						
		BSP_LCD_DisplayStringAt(350,2,Buf, TRANSPARENT_MODE);
		sprintf((char *)Buf , "%01lu", TOTAL_TRACKS_IN_TAG_LIST);					
		BSP_LCD_DisplayStringAt(448,2,Buf, TRANSPARENT_MODE);	
//		int_TI_DRAW_ALL_LINES();		
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
			BSP_LCD_Clear(0x00000000);	
			}	
		BSP_LCD_SelectLayer(0);
		if(update_all_page)	
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(0, 0, 480, 182);	
			BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
			BSP_LCD_FillRect(0, 0, 480, 18);		
			BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
			BSP_LCD_DrawRect(0, 18, 14, 152);	
			BSP_LCD_SetTextColor(0xFF0F0F0);	
			BSP_LCD_FillRect(14, 18, 466, 152);	
			BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
			BSP_LCD_DrawLine(14,18,479,18);
			BSP_LCD_DrawLine(14,37,479,37);
			BSP_LCD_DrawLine(14,56,479,56);
			BSP_LCD_DrawLine(14,75,479,75);
			BSP_LCD_DrawLine(14,94,479,94);
			BSP_LCD_DrawLine(14,113,479,113);
			BSP_LCD_DrawLine(14,132,479,132);
			BSP_LCD_DrawLine(14,151,479,151);
			BSP_LCD_DrawLine(0,170,479,170);
			BSP_LCD_DrawLine(479,18,479,170);
			BSP_LCD_DrawLine(270,18,270,170);				//vertical line
			}
//		if(edit_parameter==0)
//			{
//			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
//			BSP_LCD_FillRect(14, (18+(19*UCurrentCursorPosition)), 256, 9);			////Draw selected cursor	
//			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
//			BSP_LCD_FillRect(14, (27+(19*UCurrentCursorPosition)), 256, 5);
//			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
//			BSP_LCD_FillRect(14, (32+(19*UCurrentCursorPosition)), 256, 5);
//			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_3);		
//			BSP_LCD_FillRect(271, (18+(19*UCurrentCursorPosition)), 208, 9);			////Draw selected cursor	
//			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_5);
//			BSP_LCD_FillRect(271, (27+(19*UCurrentCursorPosition)), 208, 5);
//			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_4);
//			BSP_LCD_FillRect(271, (32+(19*UCurrentCursorPosition)), 208, 5);				
//			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
//			FillTriangle(20, 20, 27, 20+(19*UCurrentCursorPosition), 34+(19*UCurrentCursorPosition), 27 +(19*UCurrentCursorPosition));	
//			}
//		else
//			{
//			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_3);		
//			BSP_LCD_FillRect(14, (18+(19*UCurrentCursorPosition)), 256, 9);			////Draw selected cursor	
//			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_5);
//			BSP_LCD_FillRect(14, (27+(19*UCurrentCursorPosition)), 256, 5);
//			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_4);
//			BSP_LCD_FillRect(14, (32+(19*UCurrentCursorPosition)), 256, 5);
//			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);				
//			BSP_LCD_FillRect(271, (18+(19*UCurrentCursorPosition)), 208, 9);			////Draw selected cursor	
//			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
//			BSP_LCD_FillRect(271, (27+(19*UCurrentCursorPosition)), 208, 5);
//			BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
//			BSP_LCD_FillRect(271, (32+(19*UCurrentCursorPosition)), 208, 5);				
//			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
//			FillTriangle(276, 276, 283, 20+(19*UCurrentCursorPosition), 34+(19*UCurrentCursorPosition), 27 +(19*UCurrentCursorPosition));	
//			}		
		BSP_LCD_SelectLayer(1);	
		BSP_LCD_SetTransparency(1, 255);
		BSP_LCD_SetFont(&Font15P);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		if(update_all_page)
			{	
			sprintf((char*)Buf, "%s", "UTILITY");			
			BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);
			}
//		int_U_DRAW_ALL_LINES();	
		return;	
		}
	else if(LAY==BROWSER_NAVI)		//X => BROWSER NAVIGATION WITH ANIMATION
		{
		CURRENT_LAY = ActiveLayer;		
		BSP_LCD_SetTransparency(1, 0);	
		if(ActiveLayer !=1)
			{
			BSP_LCD_SelectLayer(1);
			}	
		BSP_LCD_Clear(0x00000000);
//			
//		if(PREVIOUS_BROWSE_LEVEL<BROWSE_LEVEL)				//reverse
//			{
//			if(BROWSE_LEVEL==1 && BROWSER_INFO_enable==0)	
//				{
//				info_animation_enable = 0;			
//				}
//			else
//				{
//				BSP_LCD_SetTextColor(LCD_COLOR_PAPER_TRANSP);		
//				BSP_LCD_FillRect(270, 18, 210, 152);		
//				info_animation_enable = 1;
//				}
//			animation_en = 2;		
//			}
//		else																				//forward
//			{
//			if(BROWSE_LEVEL==0 && BROWSER_INFO_enable==0)	
//				{
//				info_animation_enable = 0;		
//				}
//			else
//				{	
//				info_animation_enable = 1;		
//				}
//			BSP_LCD_SetTextColor(LCD_COLOR_PAPER_TRANSP);		
//			BSP_LCD_FillRect(270, 18, 210, 152);		
//			animation_en = 1;	
//			}	
//		BSP_LCD_SetTransparency(1, 255);
//		animation_time = HAL_GetTick();	
//		BSP_LCD_SelectLayer(0);
//		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
//		BSP_LCD_DrawRect(0, 18, 14, 152);	
//		for(E=0;E<4;E++)							/////Draw gray lines
//			{
//			BSP_LCD_FillRect(14, 18+(38*E), 466, 19);		
//			}
//		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
//		for(E=0;E<4;E++)
//			{
//			BSP_LCD_FillRect(14, 37+(38*E), 466, 19);	
//			}		
		BSP_LCD_SelectLayer(CURRENT_LAY);
		return;
		}
	else if(LAY==BR_NAVI_END)		//X => BROWSER NAVIGATION WITH ANIMATION END
		{
		CURRENT_LAY = ActiveLayer;		
//		if(BROWSE_LEVEL==0) //in playlist 	
//			{	
//			BSP_LCD_SelectLayer(0);		
//			if(BROWSER_INFO_enable)
//				{
//				intDrawLayer0_INFO_ANIMATION(B0CurrentCursorPosition);	
//				HAL_GPIO_WritePin(LED_INFO_GPIO_Port, LED_INFO_Pin, GPIO_PIN_SET);					
//				}
//			else
//				{
//				intDrawLayer0_NOINFO_ANIMATION(B0CurrentCursorPosition);		
//				}	
//			BSP_LCD_SelectLayer(1);
//			BSP_LCD_Clear(0x00000000);		
//			BSP_LCD_SetTransparency(1, 255);
//			BSP_LCD_SetFont(&Font15P);			
//			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);				
//			sprintf((char*)Buf, "%s", "|");		
//			BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);				
//			sprintf((char*)Buf, "%s", TRACKLIST_NAME[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]);						
//			BSP_LCD_DisplayStringAt(23, 2, Buf, TRANSPARENT_MODE);
//			sprintf((char*)Buf, "%s", "Total Track");						
//			BSP_LCD_DisplayStringAt(350, 2,Buf, TRANSPARENT_MODE);
//			sprintf((char *)Buf , "%1lu", TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition] - TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]);				
//			BSP_LCD_DisplayStringAt(448, 2,Buf, TRANSPARENT_MODE);	
//			if(BROWSER_INFO_enable)
//				{
//				int_BIx_DRAW_ALL_LINES(0);	
//				}
//			else
//				{
//				int_B_DRAW_ALL_LINES();		
//				}
//			}
//		else if(BROWSE_LEVEL==1) //playlists 	
//			{
//			BSP_LCD_SelectLayer(0);		
//			intDrawLayer0_ANIMATION(B1CurrentCursorPosition);	
//			BSP_LCD_SelectLayer(1);	
//			BSP_LCD_Clear(0x00000000);	
//			BSP_LCD_SetTransparency(1, 255);
//			BSP_LCD_SetFont(&Font15P);			
//			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
//			sprintf((char*)Buf, "%s", "[PLAYLIST]");						
//			BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);
//			int_BIx_DRAW_ALL_LINES(BROWSE_LEVEL);	
//			HAL_GPIO_WritePin(LED_INFO_GPIO_Port, LED_INFO_Pin, GPIO_PIN_RESET);	
//			}
//		else if(BROWSE_LEVEL==2) //SD Card 	
//			{
//			BSP_LCD_SelectLayer(0);			
//			intDrawLayer0_ANIMATION(B2CurrentCursorPosition);		
//			BSP_LCD_SelectLayer(1);	
//			BSP_LCD_Clear(0x00000000);		
//			BSP_LCD_SetTransparency(1, 255);
//			BSP_LCD_SetFont(&Font15P);	
//			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
//			sprintf((char*)Buf, "%s", "/");		
//			BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);				
//			sprintf((char*)Buf, "%s", "SD card");						
//			BSP_LCD_DisplayStringAt(23, 2, Buf, TRANSPARENT_MODE);								
//			int_BIx_DRAW_ALL_LINES(BROWSE_LEVEL);		
//			HAL_GPIO_WritePin(LED_INFO_GPIO_Port, LED_INFO_Pin, GPIO_PIN_RESET);		
//			}	
//		else if(BROWSE_LEVEL==3) //SD Card info 	
//			{
//			BSP_LCD_SelectLayer(0);			
//			intDrawLayer0_ANIMATION(0);		
//			BSP_LCD_SelectLayer(1);	
//			BSP_LCD_Clear(0x00000000);		
//			BSP_LCD_SetTransparency(1, 255);
//			BSP_LCD_SetFont(&Font15P);			
//			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
//			sprintf((char*)Buf, "%s", "SD card slot");						
//			BSP_LCD_DisplayStringAt(5,2,Buf, TRANSPARENT_MODE);
//			int_BIx_DRAW_ALL_LINES(BROWSE_LEVEL);		
//			HAL_GPIO_WritePin(LED_INFO_GPIO_Port, LED_INFO_Pin, GPIO_PIN_RESET);		
//			}
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
void int_DRAW_TRANSPARENT_BAR(void)
	{
	uint16_t j;
	for(j=0;j<452;j++)
		{			
		ForceDrawVLine(j, 0, 16, color_dim(452-j, 452, 0x9195));		
		}	
	for(j=0;j<480;j++)
		{				
		BSP_LCD_DrawPixel(j, 16, color_dim(480-j, 480, 0x9195));
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
	BSP_LCD_DrawHLine(0, 17, 12);	
	BSP_LCD_DrawHLine(0, 187, 12);	
	for(E=0;E<5;E++)							/////Draw gray lines
		{
		if(CurrentCursorPosition%2==1 | (CurrentCursorPosition/2) != E)
			{		
			BSP_LCD_FillRect(12, 17+(38*E), 468, 19);
			}			
		}
	BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
	for(E=0;E<4;E++)
		{
		if(CurrentCursorPosition%2==0 | (CurrentCursorPosition/2) != E)
			{				
			BSP_LCD_FillRect(12, 36+(38*E), 468, 19);
			}			
		}
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (17+(19*CurrentCursorPosition)), 468, 9);			////Draw selected cursor	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (26+(19*CurrentCursorPosition)), 468, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (31+(19*CurrentCursorPosition)), 468, 5);
	return;	
	};
	
	
	
	
/////////////////////////////////	
//
//internal function for Browser
void int_B_DRAW_ALL_LINES(void)
	{
	uint16_t E;
	for(E=0;E<9 && TOTAL_TRACKS_IN_CURRENT_PLAYLIST>(E+BCurrentTrackPosition-1);E++)
		{					
		if(E==B0CurrentCursorPosition && (playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+E+BCurrentTrackPosition-1]-1][54]%2)==1)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
			}
		else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+E+BCurrentTrackPosition-1]-1][54]%2)==1)		
			{
			BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
			}
		else if(E==B0CurrentCursorPosition)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			}
		else
			{
			BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
			}
		sprintf((char *)Buf , "%03lu", E+BCurrentTrackPosition);					
		BSP_LCD_DisplayStringAt(20,20+(19*E),Buf, TRANSPARENT_MODE);
		BSP_LCD_DisplayStringAt(69,20+(19*E), playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+E+BCurrentTrackPosition-1]-1], TRANSPARENT_MODE);
			
		if(TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+E+BCurrentTrackPosition-1]==track_play_now[dkA] ||
			 TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+E+BCurrentTrackPosition-1]==track_play_now[dkB])
			{
			sprintf((char*)Buf, "%s", ">");	
			}
		else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+E+BCurrentTrackPosition-1]-1][54]&0x2)==0)
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
	ReDrawScroll(TOTAL_TRACKS_IN_CURRENT_PLAYLIST, BCurrentTrackPosition);	
	};	
	
	
	
	
/////////////////////////////////	
//
//internal function for Browser
void int_B_DRAW_ONE_LINE(uint8_t UPDOWN)							
	{		
	if(B0CurrentCursorPosition%2==0)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_1);
		}
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DARK_2);
		}
		
	BSP_LCD_FillRect(12, 17+(19*B0CurrentCursorPosition), 468, 19);
	BSP_LCD_SelectLayer(1);	
	BSP_LCD_SetTransparency(1, 255);	
	BSP_LCD_SetFont(&Font15P);
		
	if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+BCurrentTrackPosition+B0CurrentCursorPosition-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		}		
	sprintf((char *)Buf , "%03lu", BCurrentTrackPosition+B0CurrentCursorPosition);					
	BSP_LCD_DisplayStringAt(20,20+(19*B0CurrentCursorPosition),Buf, TRANSPARENT_MODE);	
	BSP_LCD_DisplayStringAt(69,20+(19*B0CurrentCursorPosition), playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+BCurrentTrackPosition+B0CurrentCursorPosition-1]-1], TRANSPARENT_MODE);	

	if(TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+BCurrentTrackPosition+B0CurrentCursorPosition-1] == track_play_now[dkA] ||
		 TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+BCurrentTrackPosition+B0CurrentCursorPosition-1] == track_play_now[dkB])
		{
		sprintf((char*)Buf, "%s", ">");	
		}	
	else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+BCurrentTrackPosition+B0CurrentCursorPosition-1]-1][54]&0x2)==0)
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "<");		
		}
	BSP_LCD_DisplayStringAt(50,20+(19*B0CurrentCursorPosition),Buf, TRANSPARENT_MODE);	
	if(UPDOWN==1)
		{
		B0CurrentCursorPosition++;
		}
	else
		{
		B0CurrentCursorPosition--;	
		}
		
	if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+BCurrentTrackPosition+B0CurrentCursorPosition-1]-1][54]%2)==1)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_DGREEN);	
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		}		
	sprintf((char *)Buf , "%03lu", BCurrentTrackPosition+B0CurrentCursorPosition);					
	BSP_LCD_DisplayStringAt(20,20+(19*B0CurrentCursorPosition),Buf, TRANSPARENT_MODE);	
	BSP_LCD_DisplayStringAt(69,20+(19*B0CurrentCursorPosition), playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+BCurrentTrackPosition+B0CurrentCursorPosition-1]-1], TRANSPARENT_MODE);	
		
	if(TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+BCurrentTrackPosition+B0CurrentCursorPosition-1] == track_play_now[dkA] ||
		 TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+BCurrentTrackPosition+B0CurrentCursorPosition-1] == track_play_now[dkB])	
		{
		sprintf((char*)Buf, "%s", ">");	
		}			
	else if((playlist[TRACKS_DATABASE[TRACKLIST_OFFSET[B1CurrentCursorPosition+BCurrentPlaylistPosition-1]+BCurrentTrackPosition+B0CurrentCursorPosition-1]-1][54]&0x2)==0)
		{
		sprintf((char*)Buf, "%s", "~");
		}		
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "<");		
		}
	BSP_LCD_DisplayStringAt(50,20+(19*B0CurrentCursorPosition),Buf, TRANSPARENT_MODE);	
	BSP_LCD_SelectLayer(0);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);		
	BSP_LCD_FillRect(12, (17+(19*B0CurrentCursorPosition)), 468, 9);			//////////////////////////////	
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_2);
	BSP_LCD_FillRect(12, (26+(19*B0CurrentCursorPosition)), 468, 5);
	BSP_LCD_SetTextColor(LCD_COLOR_LIGHT_1);
	BSP_LCD_FillRect(12, (31+(19*B0CurrentCursorPosition)), 468, 5);
	return;			
	};		
	
	
/////////////////////////////////	
//	
//Redraw scroll on new position	
//	161px max
//	
void ReDrawScroll(uint16_t total_elements, uint16_t current_element_pos)
	{
	ForceDrawVLine(4, ScrollPosition+23, ScrollLong-2, LCD_COLOR_BLACK);							//
	ForceDrawVLine(5, ScrollPosition+22, ScrollLong, LCD_COLOR_BLACK);								//	SCROLL
	ForceDrawVLine(6, ScrollPosition+22, ScrollLong, LCD_COLOR_BLACK);								//
	ForceDrawVLine(7, ScrollPosition+23, ScrollLong-2, LCD_COLOR_BLACK);							//
	///Calculate scroll	
	if(total_elements<10)
		{
		ScrollLong = 161;
		ScrollPosition = 0;	
		}
	else
		{
		ScrollLong = 1449/total_elements;
		if(ScrollLong>161)
			{
			ScrollLong = 161;	
			}
		else if(ScrollLong<5)
			{
			ScrollLong = 5;	
			}	
		ScrollPosition = ((current_element_pos-1)*(161-ScrollLong))/(total_elements-9);	
		}
	ForceDrawVLine(4, ScrollPosition+23, ScrollLong-2, LCD_COLOR_WHITE);							//
	ForceDrawVLine(5, ScrollPosition+22, ScrollLong, 	LCD_COLOR_WHITE);								//	SCROLL
	ForceDrawVLine(6, ScrollPosition+22, ScrollLong, 	LCD_COLOR_WHITE);								//
	ForceDrawVLine(7, ScrollPosition+23, ScrollLong-2, LCD_COLOR_WHITE);							//		
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
			if(B0CurrentCursorPosition==8)												//All lines update++
				{
				if(BCurrentTrackPosition == TOTAL_TRACKS_IN_CURRENT_PLAYLIST-8)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x0000);
					BSP_LCD_FillRect(12, 17, 468, 171);		
					BSP_LCD_SetFont(&Font15P);
					BCurrentTrackPosition++;
					if(dSHOW==BROWSER)
						{
						int_B_DRAW_ALL_LINES();
						}
					else
						{
						//int_BIx_DRAW_ALL_LINES(0);	
						}
					return;
					}
				}
			else																								//One line update++
				{
				if(TOTAL_TRACKS_IN_CURRENT_PLAYLIST>9 || B0CurrentCursorPosition<(TOTAL_TRACKS_IN_CURRENT_PLAYLIST-1))
					{
					if(dSHOW==BROWSER)
						{	
						int_B_DRAW_ONE_LINE(UPDOWN);
						}
					else
						{
						//int_BI_DRAW_ONE_LINE(UPDOWN);	
						}
					}
				return;
				}
			}
		else if(UPDOWN==BROWSER0_DOWN)
			{
			if(B0CurrentCursorPosition==0)											//All lines update--
				{
				if(BCurrentTrackPosition == 1)
					{
					return;	
					}
				else
					{
					BSP_LCD_SelectLayer(1);	
					BSP_LCD_SetTransparency(1, 255);
					BSP_LCD_SetTextColor(0x0000);
					BSP_LCD_FillRect(12, 17, 468, 171);	
					BSP_LCD_SetFont(&Font15P);
					BCurrentTrackPosition--;
					if(dSHOW==BROWSER)
						{
						int_B_DRAW_ALL_LINES();
						}
					else
						{
						//int_BIx_DRAW_ALL_LINES(0);	
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
					//int_BI_DRAW_ONE_LINE(UPDOWN);	
					}	
				return;			
				}	
			}	
//		else if(UPDOWN==BROWSER1_UP)
//			{				
//			if(B1CurrentCursorPosition==8)												//All lines update++
//				{
//				if(BCurrentPlaylistPosition == TOTAL_TRACKLISTS-8)
//					{
//					return;	
//					}
//				else
//					{
//					BSP_LCD_SelectLayer(1);	
//					BSP_LCD_SetTransparency(1, 255);
//					BSP_LCD_SetTextColor(0x0000);
//					BSP_LCD_FillRect(12, 17, 468, 171);	
//					BSP_LCD_SetFont(&Font15P);
//					BCurrentPlaylistPosition++;
//					int_BIx_DRAW_ALL_LINES(1);
//					return;
//					}
//				}
//			else																								//One line update++
//				{
//				if(TOTAL_TRACKLISTS>9 || B1CurrentCursorPosition<(TOTAL_TRACKLISTS-1))
//					{
//					int_B1_DRAW_ONE_LINE(UPDOWN);	 
//					}
//				return;
//				}	
//			}
//		else if(UPDOWN==BROWSER1_DOWN)
//			{
//			if(B1CurrentCursorPosition==0)											//All lines update--
//				{
//				if(BCurrentPlaylistPosition == 1)
//					{
//					return;	
//					}
//				else
//					{
//					BSP_LCD_SelectLayer(1);	
//					BSP_LCD_SetTransparency(1, 255);
//					BSP_LCD_SetTextColor(0x0000);
//					BSP_LCD_FillRect(12, 17, 468, 171);	
//					BSP_LCD_SetFont(&Font15P);
//					BCurrentPlaylistPosition--;
//					int_BIx_DRAW_ALL_LINES(1);	
//					return;						
//					}	
//				}
//			else														//One line update--
//				{
//				int_B1_DRAW_ONE_LINE(UPDOWN);	 
//				return;			
//				}	
//			}	
//		else if(UPDOWN==BROWSER2_UP)
//			{
//			if(B2CurrentCursorPosition==3)												//All lines update++
//				{
//				return;	
//				}
//			else																								//One line update++
//				{
//				int_B2_DRAW_ONE_LINE(UPDOWN);	 
//				return;
//				}			
//			}
//		else if(UPDOWN==BROWSER2_DOWN)
//			{	
//			if(B2CurrentCursorPosition==0)											//All lines update--
//				{
//				return;	
//				}
//			else														//One line update--
//				{
//				int_B2_DRAW_ONE_LINE(UPDOWN);	 
//				return;			
//				}		
//			}
//		return;
		}
//	else if(dSHOW==TAG_LIST || dSHOW==TAG_LIST_INFO)	///////////////////TAG LIST mode selected
//		{
//		if(UPDOWN==TAGLIST_UP)
//			{
//			if(TCurrentCursorPosition==7)												//All lines update++
//				{
//				if(TCurrentTrackPosition == TOTAL_TRACKS_IN_TAG_LIST-7)
//					{
//					return;	
//					}
//				else
//					{
//					BSP_LCD_SelectLayer(1);	
//					BSP_LCD_SetTransparency(1, 255);
//					BSP_LCD_SetTextColor(0x00000000);
//					BSP_LCD_FillRect(14, 18, 466, 152);		
//					BSP_LCD_SetFont(&Font15P);
//					TCurrentTrackPosition++;
//					if(dSHOW==TAG_LIST)
//						{
//						int_T_DRAW_ALL_LINES();
//						}
//					else
//						{
//						int_TI_DRAW_ALL_LINES();	
//						}
//					return;
//					}
//				}
//			else																								//One line update++
//				{
//				if(TOTAL_TRACKS_IN_TAG_LIST>8 || TCurrentCursorPosition<(TOTAL_TRACKS_IN_TAG_LIST-1))
//					{
//					if(dSHOW==TAG_LIST)
//						{	
//						int_T_DRAW_ONE_LINE(UPDOWN);
//						}
//					else
//						{
//						int_TI_DRAW_ONE_LINE(UPDOWN);	
//						}
//					}
//				return;
//				}
//			}
//		else if(UPDOWN==TAGLIST_DOWN)
//			{
//			if(TCurrentCursorPosition==0)											//All lines update--
//				{
//				if(TCurrentTrackPosition == 1)
//					{
//					return;	
//					}
//				else
//					{
//					BSP_LCD_SelectLayer(1);	
//					BSP_LCD_SetTransparency(1, 255);
//					BSP_LCD_SetTextColor(0x00000000);
//					BSP_LCD_FillRect(14, 18, 466, 152);	
//					BSP_LCD_SetFont(&Font15P);
//					TCurrentTrackPosition--;
//					if(dSHOW==TAG_LIST)
//						{
//						int_T_DRAW_ALL_LINES();
//						}
//					else
//						{
//						int_TI_DRAW_ALL_LINES();	
//						}	
//					return;						
//					}	
//				}
//			else														//One line update--
//				{
//				if(dSHOW==TAG_LIST)
//					{	
//					int_T_DRAW_ONE_LINE(UPDOWN);
//					}
//				else
//					{
//					int_TI_DRAW_ONE_LINE(UPDOWN);	
//					}	
//				return;			
//				}	
//			}	
//		return;	
//		}
//	else if(dSHOW==UTILITY)		///////////////////////UTILITY mode selected
//		{
//		if(UPDOWN==UTILITY_UP)
//			{
//			if(UCurrentCursorPosition==7)												//All lines update++
//				{
//				if(CurrentUPosition == TOTAL_U_POSITIONS-7)
//					{
//					return;	
//					}
//				else
//					{
//					BSP_LCD_SelectLayer(1);	
//					BSP_LCD_SetTransparency(1, 255);
//					BSP_LCD_SetTextColor(0x00000000);
//					BSP_LCD_FillRect(14, 18, 466, 152);		
//					BSP_LCD_SetFont(&Font15P);
//					CurrentUPosition++;
//					int_U_DRAW_ALL_LINES();
//					return;
//					}
//				}
//			else if(TOTAL_U_POSITIONS>8 || UCurrentCursorPosition<(TOTAL_U_POSITIONS-1))								//One line update++
//				{
//				int_U_DRAW_ONE_LINE(UPDOWN);
//				return;
//				}
//			}
//		else if(UPDOWN==UTILITY_DOWN)
//			{
//			if(UCurrentCursorPosition==0)											//All lines update--
//				{
//				if(CurrentUPosition == 1)
//					{
//					return;	
//					}
//				else
//					{
//					BSP_LCD_SelectLayer(1);	
//					BSP_LCD_SetTransparency(1, 255);
//					BSP_LCD_SetTextColor(0x00000000);
//					BSP_LCD_FillRect(14, 18, 466, 152);	
//					BSP_LCD_SetFont(&Font15P);
//					CurrentUPosition--;
//					int_U_DRAW_ALL_LINES();
//					return;						
//					}	
//				}
//			else														//One line update--
//				{
//				int_U_DRAW_ONE_LINE(UPDOWN);
//				return;			
//				}	
//			}	
//		return;
//		}
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
	BSP_LCD_SetTextColor(LCD_COLOR_GRAY);	
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
	
	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



