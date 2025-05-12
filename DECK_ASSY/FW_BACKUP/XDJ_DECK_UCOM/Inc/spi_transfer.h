/////////////////////////////////////////////////////////////////////////////////////////////////
// 	SPI TRANSFER MAIN ASSY <=> DECK UCOMs
//	product: XDJ-X deck ucom
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t pot_convcd(uint16_t dt);					//conversion with center detent



///////////////////////////////////////////////
//
//	out 0...1023, 512 - center
//	conversion with center detent
//				
//		
uint16_t pot_convcd(uint16_t dt)
	{
	uint32_t res;	
	if(dt<POTCL)
		{
		if(dt<POTL)
			{
			return 0;	
			}		
		else	//0...511
			{	
			res = dt-POTL;		//2000
			res*=POTxCD1;
			return (res>>12);
			}
		}		
	else if(dt<=POTCH)
		{
		return 512;	
		}	
	else		//512...1023
		{
		if(dt>POTH)
			{
			dt = POTH;	
			}	
		res = dt-POTCH;		//1914
		res*=POTxCD2;
		return ((res>>12)+512);
		}		
	};	




/////////////////////////////////////////////////////
//	
//	SPI DMA transfer handler
//
//		load 35-55uS
//
void DMA2_Stream3_IRQHandler(void)
	{		
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
	#ifdef DECK_1
	if(CheckRXCRC())
		{
		if((deckRbuf[0]&0x07)==0)
			{
			if(deckRbuf[0]&0x10)
				{
				LED_CUE_H;	
				}
			else
				{
				LED_CUE_L;	
				}	
			if(deckRbuf[0]&0x08)
				{
				LED_PLAY_H;	
				}
			else
				{
				LED_PLAY_L;	
				}	
			if(deckRbuf[0]&0x80)			//Ring R
				{
				R_rx = 1;	
				}
			else
				{
				R_rx = 0;	
				}				
			if(deckRbuf[0]&0x40)			//Ring G
				{
				G_rx = 1;	
				}
			else
				{
				G_rx = 0;	
				}				
			if(deckRbuf[0]&0x20)			//Ring B
				{
				B_rx = 1;	
				}
			else
				{
				B_rx = 0;	
				}	
			pad_tmp = 0x10000*deckRbuf[1] + 0x100*deckRbuf[2] + deckRbuf[3];
			if(PAD[0]!=pad_tmp)
				{
				PAD[0] = pad_tmp;	
				COLOR_SET(PAD[0], 0);	
				need_update_pads = 1;		
				}	
			}	
		else if((deckRbuf[0]&0x07)==1)
			{
			if(deckRbuf[0]&0x10)
				{
				LED_SLIP_H;	
				}
			else
				{
				LED_SLIP_L;	
				}				
			if(deckRbuf[0]&0x08)
				{
				LED_REV_H;	
				}
			else
				{
				LED_REV_L;	
				}	
			R_rx<<=1;	
			if(deckRbuf[0]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[0]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[0]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[1] + 0x100*deckRbuf[2] + deckRbuf[3];
			if(PAD[1]!=pad_tmp)
				{
				PAD[1] = pad_tmp;	
				COLOR_SET(PAD[1], 1);	
				need_update_pads = 1;		
				}		
			}			
		else if((deckRbuf[0]&0x07)==2)
			{
			if(deckRbuf[0]&0x10)
				{
				LED_4L_H;	
				}
			else
				{
				LED_4L_L;	
				}				
			if(deckRbuf[0]&0x08)
				{
				LED_VINYL_H;	
				}
			else
				{
				LED_VINYL_L;	
				}
			R_rx<<=1;	
			if(deckRbuf[0]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[0]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[0]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[1] + 0x100*deckRbuf[2] + deckRbuf[3];
			if(PAD[2]!=pad_tmp)
				{
				PAD[2] = pad_tmp;	
				COLOR_SET(PAD[2], 2);	
				need_update_pads = 1;		
				}		
			}				
		else if((deckRbuf[0]&0x07)==3)
			{
			if(deckRbuf[0]&0x10)
				{
				LED_RELOOP_H;	
				}
			else
				{
				LED_RELOOP_L;	
				}
			if(deckRbuf[0]&0x08)
				{
				LED_8L_H;	
				}
			else
				{
				LED_8L_L;	
				}		
			R_rx<<=1;	
			if(deckRbuf[0]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[0]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[0]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[1] + 0x100*deckRbuf[2] + deckRbuf[3];
			if(PAD[3]!=pad_tmp)
				{
				PAD[3] = pad_tmp;	
				COLOR_SET(PAD[3], 3);	
				need_update_pads = 1;		
				}	
			}			
		else if((deckRbuf[0]&0x07)==4)
			{
			if(deckRbuf[0]&0x10)
				{
				LED_MT_H;	
				}
			else
				{
				LED_MT_L;	
				}
			if(deckRbuf[0]&0x08)		//Vinyl round
				{
				VFL_DATA[0]|=BITON[4];				//vinyl label ON
				}
			else
				{
				VFL_DATA[0]&=BITOFF[4];				//vinyl label OFF
				}
			R_rx<<=1;	
			if(deckRbuf[0]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[0]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[0]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[1] + 0x100*deckRbuf[2] + deckRbuf[3];
			if(PAD[4]!=pad_tmp)
				{
				PAD[4] = pad_tmp;	
				COLOR_SET(PAD[4], 4);	
				need_update_pads = 1;		
				}
			}			
		else if((deckRbuf[0]&0x07)==5)
			{
			if(deckRbuf[0]&0x10)		//MEM round
				{
				VFL_DATA[0]|=BITON[5];				//MEM ON
				}
			else
				{
				VFL_DATA[0]&=BITOFF[5];				//MEM OFF
				}	
			if(deckRbuf[0]&0x08)		//SENS round
				{
				VFL_DATA[0]|=BITON[6];				//RING sensor ON	
				}
			else
				{
				VFL_DATA[0]&=BITOFF[6];				//RING sensor OFF	
				}	
			R_rx<<=1;	
			if(deckRbuf[0]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[0]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[0]&0x20)			//Ring B
				{
				B_rx+=1;	
				}	
			pad_tmp = 0x10000*deckRbuf[1] + 0x100*deckRbuf[2] + deckRbuf[3];
			if(PAD[5]!=pad_tmp)
				{
				PAD[5] = pad_tmp;	
				COLOR_SET(PAD[5], 5);	
				need_update_pads = 1;		
				}					
			}				
		else if((deckRbuf[0]&0x07)==6)
			{
			R_rx<<=1;	
			if(deckRbuf[0]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[0]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[0]&0x20)			//Ring B
				{
				B_rx+=1;	
				}	
			pad_tmp = 0x10000*deckRbuf[1] + 0x100*deckRbuf[2] + deckRbuf[3];
			if(PAD[6]!=pad_tmp)
				{
				PAD[6] = pad_tmp;	
				COLOR_SET(PAD[6], 6);	
				need_update_pads = 1;		
				}		
			}			
		else if((deckRbuf[0]&0x07)==7)
			{
			R_rx<<=1;	
			if(deckRbuf[0]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[0]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[0]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			TIM1->CCR1 = R_rx;
			TIM1->CCR2 = G_rx;
			TIM1->CCR3 = B_rx;	
			pad_tmp = 0x10000*deckRbuf[1] + 0x100*deckRbuf[2] + deckRbuf[3];
			if(PAD[7]!=pad_tmp)
				{
				PAD[7] = pad_tmp;	
				COLOR_SET(PAD[7], 7);	
				need_update_pads = 1;		
				}					
			if(need_update_pads==1)
				{
				need_update_pads = 2;			//force to send spi	
				}				
			}			
			
			
		if(prev_pl_sect!=deckRbuf[4])
			{
			prev_pl_sect = deckRbuf[4];	
			if(prev_pl_sect<137)
				{
				draw_playing_sector(prev_pl_sect);	
				}
			}			
		if(prev_cue_sect!=deckRbuf[6])
			{
			prev_cue_sect = deckRbuf[6];	
			if((prev_cue_sect<85) && (prev_slip_sect>84))
				{
				draw_cue_sector(prev_cue_sect);	
				}
			}		
		if(prev_slip_sect!=deckRbuf[5])
			{
			prev_slip_sect = deckRbuf[5];	
			draw_slip_sector(prev_slip_sect, prev_cue_sect);
			}		
		}
		
	/* BUTTON PLAY ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_PLAY_Pin)==0)	
		{
		deckTbuf[1]|=BITON[7];
		}	
	else
		{
		deckTbuf[1]&=BITOFF[7];
		}	
	/* BUTTON CUE ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_CUE_Pin)==0)
		{
		deckTbuf[1]|=BITON[6];
		}	
	else
		{
		deckTbuf[1]&=BITOFF[6];
		}
	/* BUTTON REV ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_REV_Pin)==0)	
		{
		deckTbuf[1]|=BITON[5];
		}	
	else
		{
		deckTbuf[1]&=BITOFF[5];
		}	
	/* BUTTON MT ---------------------------------------------------------*/
	if((GPIOC->IDR & BUTTON_MT_Pin)==0)	
		{
		deckTbuf[2]|=BITON[1];
		}	
	else
		{
		deckTbuf[2]&=BITOFF[1];
		}
	/* BUTTON NEXT ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_NEXT_Pin)==0)	
		{
		deckTbuf[2]|=BITON[2];
		}	
	else
		{
		deckTbuf[2]&=BITOFF[2];
		}	
	/* BUTTON JOG ---------------------------------------------------------*/	
	if((GPIOB->IDR & BUTTON_JOG_Pin)==0)	
		{
		deckTbuf[2]|=BITON[0];
		}	
	else
		{
		deckTbuf[2]&=BITOFF[0];
		}	
	/* BUTTON 8L ---------------------------------------------------------*/			
	if((GPIOB->IDR & BUTTON_8L_Pin)==0)	
		{
		deckTbuf[1]|=BITON[1];
		}	
	else
		{
		deckTbuf[1]&=BITOFF[1];
		}	
	/* BUTTON RELOOP ---------------------------------------------------------*/	
	if((GPIOB->IDR & BUTTON_RELOOP_Pin)==0)	
		{
		deckTbuf[1]|=BITON[0];
		}	
	else
		{
		deckTbuf[1]&=BITOFF[0];
		}	
	/* BUTTON PREV ---------------------------------------------------------*/	
	if((GPIOA->IDR & BUTTON_PREV_Pin)==0)	
		{
		deckTbuf[2]|=BITON[3];
		}	
	else
		{
		deckTbuf[2]&=BITOFF[3];
		}		
	/* BUTTON 4L ---------------------------------------------------------*/		
	if((GPIOA->IDR & BUTTON_4L_Pin)==0)	
		{
		deckTbuf[1]|=BITON[2];
		}	
	else
		{
		deckTbuf[1]&=BITOFF[2];
		}		
	/* BUTTON SLIP ---------------------------------------------------------*/
	if((GPIOA->IDR & BUTTON_SLIP_Pin)==0)	
		{
		deckTbuf[1]|=BITON[4];
		}	
	else
		{
		deckTbuf[1]&=BITOFF[4];
		}	
	/* BUTTON VINYL ---------------------------------------------------------*/	
	if((GPIOA->IDR & BUTTON_VINYL_Pin)==0)	
		{
		deckTbuf[1]|=BITON[3];
		}	
	else
		{
		deckTbuf[1]&=BITOFF[3];
		}				
	/* PAD_0 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_0_Pin)==0)	
		{
		deckTbuf[0]|=BITON[7];
		}	
	else
		{
		deckTbuf[0]&=BITOFF[7];
		}		
	/* PAD_1 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_1_Pin)==0)	
		{
		deckTbuf[0]|=BITON[6];
		}	
	else
		{
		deckTbuf[0]&=BITOFF[6];
		}
	/* PAD_2 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_2_Pin)==0)	
		{
		deckTbuf[0]|=BITON[5];
		}	
	else
		{
		deckTbuf[0]&=BITOFF[5];
		}	
	/* PAD_3 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_3_Pin)==0)	
		{
		deckTbuf[0]|=BITON[4];
		}	
	else
		{
		deckTbuf[0]&=BITOFF[4];
		}				
	/* PAD_4 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_4_Pin)==0)	
		{
		deckTbuf[0]|=BITON[3];
		}	
	else
		{
		deckTbuf[0]&=BITOFF[3];
		}		
	/* PAD_5 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_5_Pin)==0)	
		{
		deckTbuf[0]|=BITON[2];
		}	
	else
		{
		deckTbuf[0]&=BITOFF[2];
		}		
	/* PAD_6 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_6_Pin)==0)	
		{
		deckTbuf[0]|=BITON[1];
		}	
	else
		{
		deckTbuf[0]&=BITOFF[1];
		}
	/* PAD_7 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_7_Pin)==0)	
		{
		deckTbuf[0]|=BITON[0];
		}	
	else
		{
		deckTbuf[0]&=BITOFF[0];
		}
	#endif

	#ifndef DECK_1
	if(CheckRXCRC())
		{
		if((deckRbuf[8]&0x07)==0)
			{
			if(deckRbuf[8]&0x10)
				{
				LED_CUE_H;	
				}
			else
				{
				LED_CUE_L;	
				}	
			if(deckRbuf[8]&0x08)
				{
				LED_PLAY_H;	
				}
			else
				{
				LED_PLAY_L;	
				}	
			if(deckRbuf[8]&0x80)			//Ring R
				{
				R_rx = 1;	
				}
			else
				{
				R_rx = 0;	
				}				
			if(deckRbuf[8]&0x40)			//Ring G
				{
				G_rx = 1;	
				}
			else
				{
				G_rx = 0;	
				}				
			if(deckRbuf[8]&0x20)			//Ring B
				{
				B_rx = 1;	
				}
			else
				{
				B_rx = 0;	
				}	
			pad_tmp = 0x10000*deckRbuf[9] + 0x100*deckRbuf[10] + deckRbuf[11];
			if(PAD[0]!=pad_tmp)
				{
				PAD[0] = pad_tmp;	
				COLOR_SET(PAD[0], 0);	
				need_update_pads = 1;		
				}	
			}	
		else if((deckRbuf[8]&0x07)==1)
			{
			if(deckRbuf[8]&0x10)
				{
				LED_SLIP_H;	
				}
			else
				{
				LED_SLIP_L;	
				}				
			if(deckRbuf[8]&0x08)
				{
				LED_REV_H;	
				}
			else
				{
				LED_REV_L;	
				}	
			R_rx<<=1;	
			if(deckRbuf[8]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[8]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[8]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[9] + 0x100*deckRbuf[10] + deckRbuf[11];
			if(PAD[1]!=pad_tmp)
				{
				PAD[1] = pad_tmp;	
				COLOR_SET(PAD[1], 1);	
				need_update_pads = 1;		
				}		
			}			
		else if((deckRbuf[8]&0x07)==2)
			{
			if(deckRbuf[8]&0x10)
				{
				LED_4L_H;	
				}
			else
				{
				LED_4L_L;	
				}				
			if(deckRbuf[8]&0x08)
				{
				LED_VINYL_H;	
				}
			else
				{
				LED_VINYL_L;	
				}
			R_rx<<=1;	
			if(deckRbuf[8]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[8]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[8]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[9] + 0x100*deckRbuf[10] + deckRbuf[11];
			if(PAD[2]!=pad_tmp)
				{
				PAD[2] = pad_tmp;	
				COLOR_SET(PAD[2], 2);	
				need_update_pads = 1;		
				}		
			}				
		else if((deckRbuf[8]&0x07)==3)
			{
			if(deckRbuf[8]&0x10)
				{
				LED_RELOOP_H;	
				}
			else
				{
				LED_RELOOP_L;	
				}
			if(deckRbuf[8]&0x08)
				{
				LED_8L_H;	
				}
			else
				{
				LED_8L_L;	
				}		
			R_rx<<=1;	
			if(deckRbuf[8]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[8]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[8]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[9] + 0x100*deckRbuf[10] + deckRbuf[11];
			if(PAD[3]!=pad_tmp)
				{
				PAD[3] = pad_tmp;	
				COLOR_SET(PAD[3], 3);	
				need_update_pads = 1;		
				}	
			}			
		else if((deckRbuf[8]&0x07)==4)
			{
			if(deckRbuf[8]&0x10)
				{
				LED_MT_H;	
				}
			else
				{
				LED_MT_L;	
				}
			if(deckRbuf[8]&0x08)		//Vinyl round
				{
				VFL_DATA[0]|=BITON[4];				//vinyl label ON
				}
			else
				{
				VFL_DATA[0]&=BITOFF[4];				//vinyl label OFF
				}
			R_rx<<=1;	
			if(deckRbuf[8]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[8]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[8]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[9] + 0x100*deckRbuf[10] + deckRbuf[11];
			if(PAD[4]!=pad_tmp)
				{
				PAD[4] = pad_tmp;	
				COLOR_SET(PAD[4], 4);	
				need_update_pads = 1;		
				}
			}			
		else if((deckRbuf[8]&0x07)==5)
			{
			if(deckRbuf[8]&0x10)		//MEM round
				{
				VFL_DATA[0]|=BITON[5];				//MEM ON
				}
			else
				{
				VFL_DATA[0]&=BITOFF[5];				//MEM OFF
				}	
			if(deckRbuf[8]&0x08)		//SENS round
				{
				VFL_DATA[0]|=BITON[6];				//RING sensor ON	
				}
			else
				{
				VFL_DATA[0]&=BITOFF[6];				//RING sensor OFF	
				}	
			R_rx<<=1;	
			if(deckRbuf[8]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[8]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[8]&0x20)			//Ring B
				{
				B_rx+=1;	
				}	
			pad_tmp = 0x10000*deckRbuf[9] + 0x100*deckRbuf[10] + deckRbuf[11];
			if(PAD[5]!=pad_tmp)
				{
				PAD[5] = pad_tmp;	
				COLOR_SET(PAD[5], 5);	
				need_update_pads = 1;		
				}					
			}				
		else if((deckRbuf[8]&0x07)==6)
			{
			R_rx<<=1;	
			if(deckRbuf[8]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[8]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[8]&0x20)			//Ring B
				{
				B_rx+=1;	
				}	
			pad_tmp = 0x10000*deckRbuf[9] + 0x100*deckRbuf[10] + deckRbuf[11];
			if(PAD[6]!=pad_tmp)
				{
				PAD[6] = pad_tmp;	
				COLOR_SET(PAD[6], 6);	
				need_update_pads = 1;		
				}		
			}			
		else if((deckRbuf[8]&0x07)==7)
			{
			R_rx<<=1;	
			if(deckRbuf[8]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[8]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[8]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			TIM1->CCR1 = R_rx;
			TIM1->CCR2 = G_rx;
			TIM1->CCR3 = B_rx;	
			pad_tmp = 0x10000*deckRbuf[9] + 0x100*deckRbuf[10] + deckRbuf[11];
			if(PAD[7]!=pad_tmp)
				{
				PAD[7] = pad_tmp;	
				COLOR_SET(PAD[7], 7);	
				need_update_pads = 1;		
				}					
			if(need_update_pads==1)
				{
				need_update_pads = 2;			//force to send spi	
				}				
			}			
		if(prev_pl_sect!=deckRbuf[12])
			{
			prev_pl_sect = deckRbuf[12];	
			if(prev_pl_sect<137)
				{
				draw_playing_sector(prev_pl_sect);	
				}
			}			
		if(prev_cue_sect!=deckRbuf[14])
			{				
			prev_cue_sect = deckRbuf[14];	
			if((prev_cue_sect<85) && (prev_slip_sect>84))
				{
				draw_cue_sector(prev_cue_sect);	
				}
			}		
		if(prev_slip_sect!=deckRbuf[13])
			{
			prev_slip_sect = deckRbuf[13];
			draw_slip_sector(prev_slip_sect, prev_cue_sect);	
			}		
		}	
		
	/* BUTTON PLAY ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_PLAY_Pin)==0)	
		{
		deckTbuf[9]|=BITON[7];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[7];
		}	
	/* BUTTON CUE ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_CUE_Pin)==0)
		{
		deckTbuf[9]|=BITON[6];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[6];
		}
	/* BUTTON REV ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_REV_Pin)==0)	
		{
		deckTbuf[9]|=BITON[5];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[5];
		}	
	/* BUTTON MT ---------------------------------------------------------*/
	if((GPIOC->IDR & BUTTON_MT_Pin)==0)	
		{
		deckTbuf[10]|=BITON[1];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[1];
		}
	/* BUTTON NEXT ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_NEXT_Pin)==0)	
		{
		deckTbuf[10]|=BITON[2];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[2];
		}	
	/* BUTTON JOG ---------------------------------------------------------*/	
	if((GPIOB->IDR & BUTTON_JOG_Pin)==0)	
		{
		deckTbuf[10]|=BITON[0];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[0];
		}	
	/* BUTTON 8L ---------------------------------------------------------*/			
	if((GPIOB->IDR & BUTTON_8L_Pin)==0)	
		{
		deckTbuf[9]|=BITON[1];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[1];
		}	
	/* BUTTON RELOOP ---------------------------------------------------------*/	
	if((GPIOB->IDR & BUTTON_RELOOP_Pin)==0)	
		{
		deckTbuf[9]|=BITON[0];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[0];
		}	
	/* BUTTON PREV ---------------------------------------------------------*/	
	if((GPIOA->IDR & BUTTON_PREV_Pin)==0)	
		{
		deckTbuf[10]|=BITON[3];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[3];
		}		
	/* BUTTON 4L ---------------------------------------------------------*/		
	if((GPIOA->IDR & BUTTON_4L_Pin)==0)	
		{
		deckTbuf[9]|=BITON[2];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[2];
		}		
	/* BUTTON SLIP ---------------------------------------------------------*/
	if((GPIOA->IDR & BUTTON_SLIP_Pin)==0)	
		{
		deckTbuf[9]|=BITON[4];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[4];
		}	
	/* BUTTON VINYL ---------------------------------------------------------*/	
	if((GPIOA->IDR & BUTTON_VINYL_Pin)==0)	
		{
		deckTbuf[9]|=BITON[3];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[3];
		}				
	/* PAD_0 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_0_Pin)==0)	
		{
		deckTbuf[8]|=BITON[7];
		}	
	else
		{
		deckTbuf[8]&=BITOFF[7];
		}		
	/* PAD_1 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_1_Pin)==0)	
		{
		deckTbuf[8]|=BITON[6];
		}	
	else
		{
		deckTbuf[8]&=BITOFF[6];
		}
	/* PAD_2 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_2_Pin)==0)	
		{
		deckTbuf[8]|=BITON[5];
		}	
	else
		{
		deckTbuf[8]&=BITOFF[5];
		}	
	/* PAD_3 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_3_Pin)==0)	
		{
		deckTbuf[8]|=BITON[4];
		}	
	else
		{
		deckTbuf[8]&=BITOFF[4];
		}				
	/* PAD_4 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_4_Pin)==0)	
		{
		deckTbuf[8]|=BITON[3];
		}	
	else
		{
		deckTbuf[8]&=BITOFF[3];
		}		
	/* PAD_5 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_5_Pin)==0)	
		{
		deckTbuf[8]|=BITON[2];
		}	
	else
		{
		deckTbuf[8]&=BITOFF[2];
		}		
	/* PAD_6 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_6_Pin)==0)	
		{
		deckTbuf[8]|=BITON[1];
		}	
	else
		{
		deckTbuf[8]&=BITOFF[1];
		}
	/* PAD_7 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_7_Pin)==0)	
		{
		deckTbuf[8]|=BITON[0];
		}	
	else
		{
		deckTbuf[8]&=BITOFF[0];
		}
	#endif	


		
		
cnts = TIM4->CNT;	
_tims = tims;		
if(cnts<32768)
	{
	cnts = 32768 - cnts;
	#ifdef DECK_1
	deckTbuf[4]|=0x80;
	#endif
		
	#ifndef DECK_1
	deckTbuf[12]|=0x80;
	#endif	
	}
else
	{
	cnts-=32768; 	
	#ifdef DECK_1
	deckTbuf[4]&=0x7F;
	#endif
		
	#ifndef DECK_1
	deckTbuf[12]&=0x7F;
	#endif	

	if(cnts==0)
		{	
		#ifdef DECK_1
		deckTbuf[2]&=0xF7;	
		deckTbuf[5] = 0xFF;
		deckTbuf[6] = 0xFF;	
		#endif

		#ifndef DECK_1
		deckTbuf[10]&=0xF7;	
		deckTbuf[13] = 0xFF;
		deckTbuf[14] = 0xFF;	
		#endif	
		}
	}			
	
if(dma_div<22)
	{
	dma_div++;	
	}
else
	{
	dma_div = 0;
	}	
	
if(dma_div==0 || cnts>7)				// 1/23
	{
	if(cnts>0)
		{			
		JOGSPD = _tims/cnts;				
		if(JOGSPD>=0xFFFF)
			{
			JOGSPD = 0xFFFF;	
			#ifdef DECK_1
			deckTbuf[2]&=0xF7;	
			deckTbuf[5] = 0xFF;
			deckTbuf[6] = 0xFF;	
			#endif

			#ifndef DECK_1
			deckTbuf[10]&=0xF7;	
			deckTbuf[13] = 0xFF;
			deckTbuf[14] = 0xFF;	
			#endif		
			}
		else
			{
			#ifdef DECK_1
			deckTbuf[2]|=0x08;	
			deckTbuf[5] = JOGSPD>>8;
			deckTbuf[6] = JOGSPD%256;	
			#endif

			#ifndef DECK_1
			deckTbuf[10]|=0x08;		
			deckTbuf[13] = JOGSPD>>8;
			deckTbuf[14] = JOGSPD%256;	
			#endif					
			}			
			
		TIM4->CNT = 32768;
		tims = 0;	
		TIM2->CNT = 0;	
		new_measure_spd = 1;			
			

		}		
				
	}	

	///////////////////////////////////////////ADCs	
	pot_SUM-=pot_ADC[cnt_ad];		
	pot_ADC[cnt_ad] = HAL_ADC_GetValue(&hadc3);	
	pot_SUM+=pot_ADC[cnt_ad];	
	ADC_TMP = (pot_SUM+8)>>4;	
	if((ADC_TMP>(pot_out+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out))	
		{
		if((ADC_TMP>(pot_out+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out))	
			{
			pot_out = ADC_TMP;
			}
		else
			{
			pot_out = (pot_out+ADC_TMP+1)>>1;	
			}
		pot_10b = pot_convcd(pot_out);	
		}	
		
	if(cnt_ad<15)
		{
		cnt_ad++;	
		}			
	else
		{
		cnt_ad = 0;	
		}	
	
	
//	if(adc_div<7)
//		{
//			
//			
//			
//			
//		adc_div++;	
//		}
//	else
//		{
//		adc_div = 0;
//		}	
	
	
	#ifdef DECK_1
	deckTbuf[2]&=0xFC;
	deckTbuf[2]|=(pot_10b>>8);
	deckTbuf[3] = pot_10b%256;	
	#endif

	#ifndef DECK_1
	deckTbuf[10]&=0xFC;
	deckTbuf[10]|=(pot_10b>>8);
	deckTbuf[11] = pot_10b%256;	
	#endif
		
		
	//HAL_ADC_Start(&hadc1);
	//HAL_ADC_Start(&hadc2);
	HAL_ADC_Start(&hadc3);			//pitch pot
		
	CalcTXCRC();
	};	




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

