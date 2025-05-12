/////////////////////////////////////////////////////////////////////////////////////////////////
// 	VFL UI generate func
//	product: XDJ-X deck ucom
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

void draw_playing_sector(uint8_t sc);
void draw_cue_sector(uint8_t sc);
void draw_slip_sector(uint8_t sc, uint8_t cue);





/////////////////////////////////////////////////////
//
//	Timer 1315*4Hz
//
//
void TIM5_IRQHandler(void)
	{
	if(tim5div<11)
		{
		tim5div++;	
			
		if(tim5div==1)
			{
			LAT_L;
			BK_L;	
			}
		else if(tim5div==2)
			{
			HAL_SPI_Transmit_DMA(&hspi3, &VFL_DATA[0], 16);	
			}			
		else if(tim5div==3)
			{
			BK_H;	
			}				
		else if(tim5div==4)
			{
			LAT_H;	
			}	
		else if(tim5div==5)
			{
			LAT_L;
			BK_L;		
			}	
		else if(tim5div==6)
			{
			HAL_SPI_Transmit_DMA(&hspi3, &VFL_DATA[16], 16);	
			}	
		else if(tim5div==7)
			{
			BK_H;	
			}	
		else if(tim5div==8)
			{
			LAT_H;	
			}	
		else if(tim5div==9)
			{
			LAT_L;
			BK_L;		
			}	
		else if(tim5div==10)
			{
			HAL_SPI_Transmit_DMA(&hspi3, &VFL_DATA[32], 16);	
			}	
		else if(tim5div==11)
			{
			BK_H;	
			}
		}
	else
		{	
		tim5div = 0;
		LAT_H;
		}
  HAL_TIM_IRQHandler(&htim5);
	}







/////////////////////////////////////////////////////
//
//	0...134.135.136
//
void draw_playing_sector(uint8_t sc)
	{
	uint8_t i;
	if(sc<135)
		{
		for(i=0;i<135;i++)
			{
			if(i>=sc && i<(sc+5) || (sc>129 && i<((sc+140)%135)))
				{						//black
				VFL_DATA[PSEGM[i]>>3]&=BITOFF[PSEGM[i]%8];
				}
			else					//white
				{
				VFL_DATA[PSEGM[i]>>3]|=BITON[PSEGM[i]%8];	
				}
			}				
		return;	
		}
	else if(sc==135)
		{
		for(i=0;i<135;i++)
			{
			VFL_DATA[PSEGM[i]>>3]&=BITOFF[PSEGM[i]%8];
			}				
		return;
		}
	else if(sc==136)
		{
		for(i=0;i<135;i++)
			{
			VFL_DATA[PSEGM[i]>>3]|=BITON[PSEGM[i]%8];	
			}
		return;
		}
	};


/////////////////////////////////////////////////////
//
//	0...84
//	85 - clear	
//	
void draw_cue_sector(uint8_t sc)
	{
	uint8_t i;	
	if(sc<85)
		{
		for(i=0;i<85;i++)
			{	
			if(i>=sc && i<(sc+2) || (sc>82 && i<((sc+87)%85)))
				{						//red
				VFL_DATA[CSEGM[i]>>3]|=BITON[CSEGM[i]%8];		
				}
			else					//black
				{
				VFL_DATA[CSEGM[i]>>3]&=BITOFF[CSEGM[i]%8];
				}
			}			
		}
	else if(sc==85)			//CUE without cue on jog display 
		{
		for(i=0;i<85;i++)
			{				//black
			VFL_DATA[CSEGM[i]>>3]&=BITOFF[CSEGM[i]%8];	
			}				
		}
	return;	
	};
	
/////////////////////////////////////////////////////
//
//	0...84
//	85 - clear
//	
void draw_slip_sector(uint8_t sc, uint8_t cue)
	{
	uint8_t i;
	if(sc<85)
		{
		for(i=0;i<85;i++)
			{	
			if((i>=sc && i<(sc+3)) || (sc>81 && i<((sc+88)%85)))
				{						//black	
				VFL_DATA[CSEGM[i]>>3]&=BITOFF[CSEGM[i]%8];
				}
			else if(cue<85 && (i==((cue+84)%85) || i==((cue+2)%85)))	
				{
				VFL_DATA[CSEGM[i]>>3]&=BITOFF[CSEGM[i]%8];	
				}
			else					//red
				{
				VFL_DATA[CSEGM[i]>>3]|=BITON[CSEGM[i]%8];		
				}	
			}
		}
	else if(sc==85)
		{
		for(i=0;i<85;i++)
			{	//black
			VFL_DATA[CSEGM[i]>>3]&=BITOFF[CSEGM[i]%8];
			}
		prev_cue_sect = 0xFF;		//forcebly redraw cue	
		}		
	return;	
	}
	
	





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

