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

void RedrawWaveforms(uint32_t position);
void intDRAW_WAVEFORM_FRAME(uint32_t position);						//internal function for redraw waveform
void DrawLOGO(void);
void DrawStaticWFM(uint8_t Tpos);
void DrawMemoryMarker(uint8_t p, uint8_t type, uint16_t color);
void DrawMTriangle(uint16_t x, uint8_t y, uint16_t color);
void DrawCTriangle(uint16_t x, uint8_t y, uint16_t color);
void DrawCueMarker(uint8_t p);

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
//				pixldt = 0x8000+(ANIMATION[frame][n][1]>>3)+32*(ANIMATION[frame][n][2]>>3)+1024*(ANIMATION[frame][n][0]>>3);		
//				sprintf((char*)U_TX_DATA, "0x%04X, ", pixldt);	
//				HAL_UART_Transmit(&huart4, U_TX_DATA, 8, 5);
//				if(n%16==15)
//					{
//					sprintf((char*)U_TX_DATA, "\n\r");		
//					HAL_UART_Transmit(&huart4, U_TX_DATA, 2, 5);	
//					}						
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
//				pixldt = 0x8000+(ANIMATION[frame][n][1]>>3)+32*(ANIMATION[frame][n][2]>>3)+1024*(ANIMATION[frame][n][0]>>3);		
//				sprintf((char*)U_TX_DATA, "0x%04X, ", pixldt);	
//				HAL_UART_Transmit(&huart4, U_TX_DATA, 8, 5);
//				if(n%16==15)
//					{
//					sprintf((char*)U_TX_DATA, "\n\r");		
//					HAL_UART_Transmit(&huart4, U_TX_DATA, 2, 5);	
//					}		
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
void intDRAW_WAVEFORM_FRAME(uint32_t position)
	{
	uint16_t i, adr;
	uint16_t u, x; 	
	uint8_t	j, r;
	x = 0;
	u = 0;	
				
	if(position>=240)
		{
		while((BEATGRID[u]-(BEATGRID[u]%DynamicWaveformZOOM))<(DynamicWaveformZOOM*(position-240)))
			{
			u++;	
			}	
		}	
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
	BSP_LCD_FillRect(0, 20, 480, 50);			
	for(i=0;i<480;i++)
		{	
		adr = DynamicWaveformZOOM*(i+position-240);	
//		if(CUE_ADR<LOOP_OUT)	
//			{
//			if(adr>=CUE_ADR && adr<LOOP_OUT)	
//				{
//				if(loop_active)
//					{
//					BG_COLOR = LOOP_ACTIVE_COLOR;	
//					}
//				else
//					{
//					BG_COLOR = LOOP_INACTIVE_COLOR;		
//					}		
//				}
//			else
//				{
//				BG_COLOR = LCD_COLOR_BLACK;		
//				}			
//			}
//		else
//			{
//			BG_COLOR = LCD_COLOR_BLACK;	
//			}	
		if(adr<=all_long)
			{
			if((BEATGRID[u+x]-(BEATGRID[u+x]%DynamicWaveformZOOM))==adr)	
				{
				if(((u+x)%4)==((1-GRID_OFFSET)&0x03))					//red grid
					{
					ForceDrawVLine(i, 23, 5, LCD_COLOR_RED);
					ForceDrawVLine(i, 65, 5, LCD_COLOR_RED);
					}
				else if(DynamicWaveformZOOM<8)			//white grid
					{	
					ForceDrawVLine(i, 23, 5, LCD_COLOR_WHITE);
					ForceDrawVLine(i, 65, 5, LCD_COLOR_WHITE);		
					}
				x++;	
				}	
			if(number_of_memory_cue_points>0)					//Draw MEMORY CUE triangle
				{
				for(j=0;j<number_of_memory_cue_points;j++)
					{
					if((MEMORY_adr[0][j]-(MEMORY_adr[0][j]%DynamicWaveformZOOM))==adr)
						{
						if(i>2 && i<476)
							{
							BSP_LCD_SetTextColor(LCD_COLOR_RED);
							DrawMTriangle(i-3, 23, LCD_COLOR_RED);	
							}		
						}	
					}
				}	
			if(number_of_hot_cue_points>0)				//Draw HCUE squares
				{	
				for(j=0;j<8;j++)
					{
					if(HCUE_adr[0][j]!=0xFFFF)							
						{
						if((HCUE_adr[0][j]-(HCUE_adr[0][j]%DynamicWaveformZOOM))==adr)
							{
							if(i>2 && i<476)
								{
								BSP_LCD_SetTextColor(HCUE16COLOR[j]);	
								BSP_LCD_FillRect(i-3, 20, 7, 7);	
								for(r=0;r<25;r++)
									{
									if((hcicons[j][r/5]>>(7-(r%5)))&0x01)
										{
										BSP_LCD_DrawPixel(i+(r%5)-2, 21+(r/5), LCD_COLOR_BLACK);	
										}
									}									
								}								
							}
						}
					}
				}				
			if((CUE_ADR-(CUE_ADR%DynamicWaveformZOOM))==adr) //Draw CUE triangle
				{
				if(i>2 && i<476)	
					{						
					DrawCTriangle(i-3, 66, CUE_COLOR);						
					}
				}	
			}

		if(i==239)																		///you can optimize 1 raz draw red line
			{
			//bars = u+x;		
			}
		else if(i==240)
			{
			}
		else
			{
			if(adr<=all_long)
				{
				if(DynamicWaveformZOOM==1)
					{	
					ForceDrawVLine(i, 46-(WFORMDYNAMIC[adr]&0x1F), 1+2*(WFORMDYNAMIC[adr]&0x1F), COLOR_MAP[0][WFORMDYNAMIC[adr]>>5]);		//124-125px center		
					}
				else 	
					{
					uint8_t amplitude = (WFORMDYNAMIC[adr]&0x1F);		
					if(amplitude>18)
						{
						amplitude = 18;	
						}						
					uint8_t color = (WFORMDYNAMIC[adr]>>5);
					for(j=0;j<(DynamicWaveformZOOM-1);j++)
						{
						if((WFORMDYNAMIC[adr+j+1]&0x1F)>amplitude)
							{
							amplitude	= (WFORMDYNAMIC[adr+j+1]&0x1F);
							if(amplitude>18)
								{
								amplitude = 18;	
								}		
							if(amplitude>13)
								{
								color = (WFORMDYNAMIC[adr+j+1]>>5);
								}
							}
						}		
					ForceDrawVLine(i, 46-amplitude, 1+2*amplitude, COLOR_MAP[0][color]);		//124-125px center	
					}
				}
			else
				{

				}	
			}			
		}
	ForceDrawVLine(239, 19, 102, LCD_COLOR_WHITE);	
	ForceDrawVLine(240, 19, 102, LCD_COLOR_WHITE);		
	return;	
	};	


	
	
////////////////////////////////////////////////////////////////////////
//
//Function redraw bar on static waveform and redraw dynamic waveform
//position = 1/150 sec
void RedrawWaveforms(uint32_t position)
	{
	if(position>all_long)
		{
		return;	
		}
	uint32_t clock_pos;	

	if(REMAIN_ENABLE)
		{
		clock_pos = all_long - position;	
		}	
	else
		{
		clock_pos	= position;
		}
		
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
	BSP_LCD_SetFont(&Font18D);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

	if(forcibly_redraw==1)
		{
		Prev10m = 0xFF;
		Prev1m = 0xFF;
		Prev10s = 0xFF;
		Prev1s = 0xFF;
		Prev10f = 0xFF;
		Prev1f = 0xFF;
		PrevHf = 0xFF;				
		}

	if(Prev10m != (clock_pos/90000)%10)			
		{
//		Prev10m = (clock_pos/90000)%10;	
//		sprintf((char *)Buf , "%0lu", Prev10m);				//10 Min	
//		BSP_LCD_DisplayStringAt(22, 168, Buf, LEFT_MODE);	
		}
	if(Prev1m != (clock_pos/9000)%10)			
		{
		Prev1m = (clock_pos/9000)%10;	
		sprintf((char *)Buf , "%0lu", Prev1m);				//1 Min	
		BSP_LCD_DisplayStringAt(38, 168, Buf, LEFT_MODE);				
		}
	if(Prev10s != (clock_pos/1500)%6)
		{
		Prev10s = (clock_pos/1500)%6;
		sprintf((char *)Buf , "%0lu", Prev10s);				//10 Sec	
		BSP_LCD_DisplayStringAt(68, 168, Buf, LEFT_MODE);		
		}
	if(Prev1s != (clock_pos/150)%10)
		{
		Prev1s = (clock_pos/150)%10;
		sprintf((char *)Buf , "%0lu", Prev1s);				//1 Sec	
		BSP_LCD_DisplayStringAt(84, 168, Buf, LEFT_MODE);
		}		
	if(Prev10f != ((clock_pos/2)%75)/10)
		{
		Prev10f = ((clock_pos/2)%75)/10;
		BSP_LCD_SetFont(&Font14D);	
		sprintf((char *)Buf , "%0lu", Prev10f);				//10 F	
		BSP_LCD_DisplayStringAt(110, 172, Buf, LEFT_MODE);
		}		
	if(Prev1f != ((clock_pos/2)%75)%10)
		{
		Prev1f = ((clock_pos/2)%75)%10;
		BSP_LCD_SetFont(&Font14D);	
		sprintf((char *)Buf , "%0lu", Prev1f);				//1 F	
		BSP_LCD_DisplayStringAt(123, 172, Buf, LEFT_MODE);	
		}
	if(PrevHf != clock_pos%2)
		{
		PrevHf = clock_pos%2;
		BSP_LCD_SetFont(&Font14D);		
		if(PrevHf%2==1)
			{
			sprintf((char*)Buf, "%s", "5");	
			}
		else
			{
			sprintf((char*)Buf, "%s", "0");	
			}
		BSP_LCD_DisplayStringAt(140, 172, Buf, LEFT_MODE);					
		}		
		
	DrawStaticWFM(position*203/all_long);	
		
	position = position/DynamicWaveformZOOM;					//zoom correction	
		
	if(position!=PreviousPositionDW)
		{
		PreviousPositionDW = position;
		if(VisibleLayer==0)
			{
			BSP_LCD_SelectLayer(1);	
			intDRAW_WAVEFORM_FRAME(position);		
			//memcpy(&fbuf[114240], &fbuf[144960], 48960);							
			//HAL_DMA_GO(&hdma_memtomem_dma1_stream0, 0xC0049200, 0xC003D680, 8880);
			//HAL_DMA_Poll(&hdma_memtomem_dma1_stream0, HAL_DMA_FULL_TRANSFER, 8);
			BSP_LCD_SetTransparency(1, 255);		//high layer disable
			BSP_LCD_SelectLayer(0);
			VisibleLayer = 1;
			}
		else
			{
			BSP_LCD_SelectLayer(0);
			intDRAW_WAVEFORM_FRAME(position);	
			//memcpy(&fbuf[2400], &fbuf[33120], 48960);
			//HAL_DMA_GO(&hdma_memtomem_dma1_stream0, 0xC0012840, 0xC0006CC0, 8880);	
			//HAL_DMA_Poll(&hdma_memtomem_dma1_stream0, HAL_DMA_FULL_TRANSFER, 8);
			BSP_LCD_SetTransparency(1, 0);		//high layer disable
			VisibleLayer = 0;	
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
void DrawStaticWFM(uint8_t Tpos)
	{
	uint16_t i;
	if(Tpos<203)
		{	
		if(previousTpos != Tpos)			
			{
			ForceDrawVLine(previousTpos+3, 199, 30, LCD_COLOR_BLACK);
			ForceDrawVLine(previousTpos+4, 199, 30, LCD_COLOR_BLACK);	
			ForceDrawVLine(previousTpos+3, 219-(WFORMSTATIC[previousTpos]&0x1F), (WFORMSTATIC[previousTpos]&0x1F)+1, 
				WS_COLOR_MAP[WFORMSTATIC[previousTpos]>>7]);	
			ForceDrawVLine(previousTpos+4, 219-(WFORMSTATIC[previousTpos+1]&0x1F), (WFORMSTATIC[previousTpos+1]&0x1F)+1, 
				WS_COLOR_MAP[WFORMSTATIC[previousTpos+1]>>7]);
			if(previousTpos>Tpos)						//___<<||___     moving
				{				
				i = previousTpos - Tpos;
				if(REMAIN_ENABLE)
					{	
					while(i>0)
						{
						if((Tpos+i)%2==0 && (Tpos+i<201))
							{
							ForceDrawVLine(Tpos+i+5, 222, 4, LCD_COLOR_WHITE);	
							}
						else
							{
							ForceDrawVLine(Tpos+i+5, 222, 4, PBAR_COLOR_1);	
							}
							i--;	
						}
					}
				else
					{
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
					BSP_LCD_FillRect(Tpos+5, 223, (previousTpos-Tpos+2), 2);			
					while(i>0)
						{
						if(((Tpos+i)%2==0) && (Tpos+i<201))
							{
							BSP_LCD_DrawPixel(Tpos+i+5, 222, PBAR_COLOR_2);	
							BSP_LCD_DrawPixel(Tpos+i+5, 225, PBAR_COLOR_2);		
							}
						else if(((Tpos+i)%2==1) && (Tpos+i<201))
							{
							BSP_LCD_DrawPixel(Tpos+i+5, 222, PBAR_COLOR_3);	
							BSP_LCD_DrawPixel(Tpos+i+5, 225, PBAR_COLOR_3);		
							}
						i--;	
						}
					ForceDrawVLine(205, 222, 4, PBAR_COLOR_2);						//gray scroll vertical line in end					
					}
				}
			else if(previousTpos<Tpos)																			//___||>>___     moving
				{
				i = Tpos - previousTpos+1;	
				if(REMAIN_ENABLE)																		
					{
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
					BSP_LCD_FillRect(previousTpos+4, 223, (Tpos - previousTpos+2), 2);						
					ForceDrawVLine(3, 222, 4, PBAR_COLOR_2);						//gray scroll vertical line in start	
					while(i>0)
						{
						if((previousTpos+i)>1)	
							{
							if((previousTpos+i)%2==0)
								{
								BSP_LCD_DrawPixel(1+previousTpos+i, 222, PBAR_COLOR_2);	
								BSP_LCD_DrawPixel(1+previousTpos+i, 225, PBAR_COLOR_2);	
								}
							else
								{
								BSP_LCD_DrawPixel(1+previousTpos+i, 222, PBAR_COLOR_3);	
								BSP_LCD_DrawPixel(1+previousTpos+i, 225, PBAR_COLOR_3);		
								}
							}
						i--;					
						}
					}
				else				
					{
					while(i>0)
						{
						if((previousTpos+i)>1)	
							{
							if((previousTpos+i)%2==0)
								{
								ForceDrawVLine(1+previousTpos+i, 222, 4, LCD_COLOR_WHITE);		
								}
							else
								{
								ForceDrawVLine(1+previousTpos+i, 222, 4, PBAR_COLOR_1);		
								}
							}
						i--;					
						}
					}
				}			
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_FillRect(Tpos+2, 222, 4, 4);					
			if(RED_VERTICAL_LINE)						//detecting touch on sensor or touch on jog 
				{
				ForceDrawVLine(Tpos+3, 199, 30, LCD_COLOR_RED);		//red vertical scroll
				ForceDrawVLine(Tpos+4, 199, 30, LCD_COLOR_RED);		
				}
			else
				{
				ForceDrawVLine(Tpos+3, 199, 30, LCD_COLOR_WHITE);		//white vertical scroll
				ForceDrawVLine(Tpos+4, 199, 30, LCD_COLOR_WHITE);		
				}
			previousTpos = Tpos;	
			}
		else if(forcibly_redraw==1)
			{
			if(RED_VERTICAL_LINE)						//detecting touch on sensor or touch on jog 
				{
				ForceDrawVLine(Tpos+3, 199, 30, LCD_COLOR_RED);		//red vertical scroll
				ForceDrawVLine(Tpos+4, 199, 30, LCD_COLOR_RED);			
				}
			else
				{
				ForceDrawVLine(Tpos+3, 199, 30, LCD_COLOR_WHITE);		//white vertical scroll
				ForceDrawVLine(Tpos+4, 199, 30, LCD_COLOR_WHITE);		
				}
			}
		return;	
		}
	else if(Tpos==REDRAW_IN_NREMAIN_MODE)
		{
		if(previousTpos<200)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(previousTpos+5, 223, 200-previousTpos, 2);
			}				
		for(i=0;i<203;i++)						
			{			
			if(i<previousTpos-1)
				{
				if(i%2==0)							//drawing ||||||||||||||||||
					{
					ForceDrawVLine(i+3, 222, 4, LCD_COLOR_WHITE);		
					}
				else
					{
					ForceDrawVLine(i+3, 222, 4, PBAR_COLOR_1);		
					}
				}	
			else if(i>previousTpos+2)
				{
				if(i%2==0)						//drawing :::::::::::::::
					{
					BSP_LCD_DrawPixel(3+i, 222, PBAR_COLOR_2);	
					BSP_LCD_DrawPixel(3+i, 225, PBAR_COLOR_2);	
					}
				else
					{
					BSP_LCD_DrawPixel(3+i, 222, PBAR_COLOR_3);	
					BSP_LCD_DrawPixel(3+i, 225, PBAR_COLOR_3);		
					}	
				}
			}
		if(previousTpos<200)
			{
			ForceDrawVLine(205, 222, 4, PBAR_COLOR_2);						//gray scroll vertical line in end	
			}	
		DRAWN_IN_REMAIN = 0;	
		return;	
		}
	else if(Tpos==REDRAW_IN_REMAIN_MODE)
		{
		if(previousTpos>1)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
			BSP_LCD_FillRect(4, 223, previousTpos-1, 2);
			}
		for(i=0;i<203;i++)						
			{	
			if(i<previousTpos-1)
				{
				if(i%2==0)						//drawing :::::::::::::::
					{
					BSP_LCD_DrawPixel(3+i, 222, PBAR_COLOR_2);	
					BSP_LCD_DrawPixel(3+i, 225, PBAR_COLOR_2);	
					}
				else
					{
					BSP_LCD_DrawPixel(3+i, 222, PBAR_COLOR_3);	
					BSP_LCD_DrawPixel(3+i, 225, PBAR_COLOR_3);		
					}	
				}	
			else if(i>previousTpos+2)
				{
				if(i%2==0)							//drawing ||||||||||||||||||
					{
					ForceDrawVLine(i+3, 222, 4, LCD_COLOR_WHITE);		
					}
				else
					{
					ForceDrawVLine(i+3, 222, 4, PBAR_COLOR_1);		
					}
				}	
			}
		if(previousTpos>1)
			{
			ForceDrawVLine(3, 222, 4, PBAR_COLOR_2);						//gray scroll vertical line in start				
			}		
		DRAWN_IN_REMAIN = 1;	
		return;	
		}	
	else if(Tpos==DRAW_NEW_STATIC_WAVEFORM)						//Draw new static waveform		
		{
		previousTpos = 0;	
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(3, 199, 203, 34);				
		for(i=0;i<203;i++)						
			{			
			ForceDrawVLine(i+3, 219-(WFORMSTATIC[i]&0x1F), (WFORMSTATIC[i]&0x1F)+1, WS_COLOR_MAP[WFORMSTATIC[i]>>7]);		
			if(REMAIN_ENABLE)
				{
				if(i%2==0)
					{
					ForceDrawVLine(i+3, 222, 4, LCD_COLOR_WHITE);		
					}
				else
					{
					ForceDrawVLine(i+3, 222, 4, PBAR_COLOR_1);		
					}
				}
			else
				{
				if(i%2==0)
					{
					BSP_LCD_DrawPixel(3+i, 222, PBAR_COLOR_2);	
					BSP_LCD_DrawPixel(3+i, 225, PBAR_COLOR_2);	
					}
				else
					{
					BSP_LCD_DrawPixel(3+i, 222, PBAR_COLOR_3);	
					BSP_LCD_DrawPixel(3+i, 225, PBAR_COLOR_3);		
					}
				}
			}
		if(REMAIN_ENABLE==0)	
			{
			ForceDrawVLine(3, 222, 4, PBAR_COLOR_2);						//gray scroll vertical line in start	
			ForceDrawVLine(205, 222, 4, PBAR_COLOR_2);					//gray scroll vertical line in end	
			}
		}
	else if(Tpos==CLEAR_WAVEFORM_ARRAY)										//clear waveform array
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(1, 192, 208, 41);			
		}
	else if(Tpos==MS_NOT_LOADED)
		{
		BSP_LCD_SetFont(&Font15P);	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "NOT LOADED");	
		BSP_LCD_DisplayStringAt(193, 202,Buf, LEFT_MODE);
		}
	else if(Tpos>MS_ERROR)													//Error message
		{
		BSP_LCD_SetFont(&Font15P);	
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		sprintf((char*)Buf, "%s", "ERROR");	
		BSP_LCD_DisplayStringAt(50, 202,Buf, LEFT_MODE);
		sprintf((char*)Buf, "%2lu", (Tpos-MS_ERROR));			//ERROR NUMBER
		BSP_LCD_DisplayStringAt(109, 202,Buf, LEFT_MODE);			
		}
	};
	
////////////////////////////////////////////////	
//Draw MEMORY red triangle on time bar	
// p=0xFF - disable all triangle	
// time position bar 0...202		
//#define NONE_MARK				0
//#define MEMORY_MARK			1
//#define HOT_CUE_MARK	 >1
void DrawMemoryMarker(uint8_t p, uint8_t type, uint16_t color)
	{
	if(type==NONE_MARK)
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(0, 192, 209, 7);
		return;
		}
	else if(p<203)
		{
		if(type>1)		//HOT CUEs
			{			
			uint8_t r;	
			BSP_LCD_SetTextColor(color); 	
			BSP_LCD_FillRect(p, 192, 7, 7);
			for(r=0;r<25;r++)
				{
				if((hcicons[type-2][r/5]>>(7-(r%5)))&0x01)
					{
					BSP_LCD_DrawPixel(p+(r%5)+1, 193+(r/5), LCD_COLOR_BLACK);	
					}
				}
			}
		else
			{
			DrawMTriangle(p, 195, LCD_COLOR_RED);
			}
		return;
		}	
	else
		{
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_FillRect(0, 192, 209, 7);
		}	
	return;	
	};		
		
	
//////////////////////////////////////////////	
//	
//	internal function for redraw waveform	
//		
void DrawMinuteMarkers(void)
	{	
	uint32_t a = 1827000/all_long;
	uint32_t npoint = all_long/9000;		
	while(npoint>0)
		{
		ForceDrawVLine(3, 229, 4, LCD_COLOR_WHITE);	
		if(npoint*a<202)
			{				
			ForceDrawVLine(3+npoint*a, 229, 4, LCD_COLOR_WHITE);	
			}	
		npoint--;		
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
void DrawCueMarker(uint8_t p)
	{
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_FillRect(0, 229, 209, 4);
	if(p<203)
		{
		DrawCTriangle(p, 229, CUE_COLOR);	
		}		
	DrawMinuteMarkers();	
	}

	
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



