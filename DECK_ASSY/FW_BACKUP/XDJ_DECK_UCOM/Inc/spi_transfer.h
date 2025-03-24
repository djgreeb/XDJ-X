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



/////////////////////////////////////////////////////
//	
//	SPI DMA transfer handler
//	
void DMA2_Stream3_IRQHandler(void)
	{
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
			
	#ifdef DECK_1
	if(CheckRXCRC())
		{
		if(deckRbuf[0]==192)
			{
			if(deckRbuf[4]&0x02)
				{
				LED_CUE_H;	
				}
			else
				{
				LED_CUE_L;	
				}	
			if(deckRbuf[4]&0x01)
				{
				LED_PLAY_H;	
				}
			else
				{
				LED_PLAY_L;	
				}	
			if(deckRbuf[4]&0x80)			//Ring R
				{
				R_rx = 1;	
				}
			else
				{
				R_rx = 0;	
				}				
			if(deckRbuf[4]&0x40)			//Ring G
				{
				G_rx = 1;	
				}
			else
				{
				G_rx = 0;	
				}				
			if(deckRbuf[4]&0x20)			//Ring B
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
		else if(deckRbuf[0]==193)
			{
			if(deckRbuf[4]&0x02)
				{
				LED_SLIP_H;	
				}
			else
				{
				LED_SLIP_L;	
				}				
			if(deckRbuf[4]&0x01)
				{
				LED_REV_H;	
				}
			else
				{
				LED_REV_L;	
				}	
			R_rx<<=1;	
			if(deckRbuf[4]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[4]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[4]&0x20)			//Ring B
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
		else if(deckRbuf[0]==194)
			{
			if(deckRbuf[4]&0x02)
				{
				LED_4L_H;	
				}
			else
				{
				LED_4L_L;	
				}				
			if(deckRbuf[4]&0x01)
				{
				LED_VINYL_H;	
				}
			else
				{
				LED_VINYL_L;	
				}
			R_rx<<=1;	
			if(deckRbuf[4]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[4]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[4]&0x20)			//Ring B
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
		else if(deckRbuf[0]==195)
			{
			if(deckRbuf[4]&0x02)
				{
				LED_RELOOP_H;	
				}
			else
				{
				LED_RELOOP_L;	
				}
			if(deckRbuf[4]&0x01)
				{
				LED_8L_H;	
				}
			else
				{
				LED_8L_L;	
				}		
			R_rx<<=1;	
			if(deckRbuf[4]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[4]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[4]&0x20)			//Ring B
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
		else if(deckRbuf[0]==196)
			{
			if(deckRbuf[4]&0x01)
				{
				LED_MT_H;	
				}
			else
				{
				LED_MT_L;	
				}
			if(deckRbuf[4]&0x02)		//Vinyl round
				{
				VFL_DATA[0]|=BITON[4];				//vinyl label ON
				}
			else
				{
				VFL_DATA[0]&=BITOFF[4];				//vinyl label OFF
				}
			R_rx<<=1;	
			if(deckRbuf[4]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[4]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[4]&0x20)			//Ring B
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
		else if(deckRbuf[0]==197)
			{
			if(deckRbuf[4]&0x02)		//MEM round
				{
				VFL_DATA[0]|=BITON[5];				//MEM ON
				}
			else
				{
				VFL_DATA[0]&=BITOFF[5];				//MEM OFF
				}	
			if(deckRbuf[4]&0x01)		//SENS round
				{
				VFL_DATA[0]|=BITON[6];				//RING sensor ON	
				}
			else
				{
				VFL_DATA[0]&=BITOFF[6];				//RING sensor OFF	
				}	
			R_rx<<=1;	
			if(deckRbuf[4]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[4]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[4]&0x20)			//Ring B
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
		else if(deckRbuf[0]==198)
			{
			R_rx<<=1;	
			if(deckRbuf[4]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[4]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[4]&0x20)			//Ring B
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
		else if(deckRbuf[0]==199)
			{
			R_rx<<=1;	
			if(deckRbuf[4]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[4]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[4]&0x20)			//Ring B
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
		if(prev_pl_sect!=deckRbuf[7])
			{
			prev_pl_sect = deckRbuf[7];	
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
			if(prev_slip_sect<85)
				{
				draw_slip_sector(prev_slip_sect);	
				}
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
	deckTbuf[3] = 0x00;				
	deckTbuf[4] = 0x00;			
	deckTbuf[5] = 0x00;
	deckTbuf[6] = 0x00;	
	deckTbuf[7] = 0x00;	
	#endif

	
	#ifndef DECK_1
	if(CheckRXCRC())
		{
		if(deckRbuf[9]==192)
			{
			if(deckRbuf[13]&0x02)
				{
				LED_CUE_H;	
				}
			else
				{
				LED_CUE_L;	
				}	
			if(deckRbuf[13]&0x01)
				{
				LED_PLAY_H;	
				}
			else
				{
				LED_PLAY_L;	
				}	
			if(deckRbuf[13]&0x80)			//Ring R
				{
				R_rx = 1;	
				}
			else
				{
				R_rx = 0;	
				}				
			if(deckRbuf[13]&0x40)			//Ring G
				{
				G_rx = 1;	
				}
			else
				{
				G_rx = 0;	
				}				
			if(deckRbuf[13]&0x20)			//Ring B
				{
				B_rx = 1;	
				}
			else
				{
				B_rx = 0;	
				}	
			pad_tmp = 0x10000*deckRbuf[10] + 0x100*deckRbuf[11] + deckRbuf[12];
			if(PAD[0]!=pad_tmp)
				{
				PAD[0] = pad_tmp;	
				COLOR_SET(PAD[0], 0);	
				need_update_pads = 1;		
				}	
			}	
		else if(deckRbuf[9]==193)
			{
			if(deckRbuf[13]&0x02)
				{
				LED_SLIP_H;	
				}
			else
				{
				LED_SLIP_L;	
				}				
			if(deckRbuf[13]&0x01)
				{
				LED_REV_H;	
				}
			else
				{
				LED_REV_L;	
				}	
			R_rx<<=1;	
			if(deckRbuf[13]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[13]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[13]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[10] + 0x100*deckRbuf[11] + deckRbuf[12];
			if(PAD[1]!=pad_tmp)
				{
				PAD[1] = pad_tmp;	
				COLOR_SET(PAD[1], 1);	
				need_update_pads = 1;		
				}		
			}			
		else if(deckRbuf[9]==194)
			{
			if(deckRbuf[13]&0x02)
				{
				LED_4L_H;	
				}
			else
				{
				LED_4L_L;	
				}				
			if(deckRbuf[13]&0x01)
				{
				LED_VINYL_H;	
				}
			else
				{
				LED_VINYL_L;	
				}
			R_rx<<=1;	
			if(deckRbuf[13]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[13]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[13]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[10] + 0x100*deckRbuf[11] + deckRbuf[12];
			if(PAD[2]!=pad_tmp)
				{
				PAD[2] = pad_tmp;	
				COLOR_SET(PAD[2], 2);	
				need_update_pads = 1;		
				}		
			}				
		else if(deckRbuf[9]==195)
			{
			if(deckRbuf[13]&0x02)
				{
				LED_RELOOP_H;	
				}
			else
				{
				LED_RELOOP_L;	
				}
			if(deckRbuf[13]&0x01)
				{
				LED_8L_H;	
				}
			else
				{
				LED_8L_L;	
				}		
			R_rx<<=1;	
			if(deckRbuf[13]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[13]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[13]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[10] + 0x100*deckRbuf[11] + deckRbuf[12];
			if(PAD[3]!=pad_tmp)
				{
				PAD[3] = pad_tmp;	
				COLOR_SET(PAD[3], 3);	
				need_update_pads = 1;		
				}	
			}			
		else if(deckRbuf[9]==196)
			{
			if(deckRbuf[13]&0x01)
				{
				LED_MT_H;	
				}
			else
				{
				LED_MT_L;	
				}
			if(deckRbuf[13]&0x02)		//Vinyl round
				{
				VFL_DATA[0]|=BITON[4];				//vinyl label ON
				}
			else
				{
				VFL_DATA[0]&=BITOFF[4];				//vinyl label OFF
				}
			R_rx<<=1;	
			if(deckRbuf[13]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[13]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[13]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			pad_tmp = 0x10000*deckRbuf[10] + 0x100*deckRbuf[11] + deckRbuf[12];
			if(PAD[4]!=pad_tmp)
				{
				PAD[4] = pad_tmp;	
				COLOR_SET(PAD[4], 4);	
				need_update_pads = 1;		
				}
			}			
		else if(deckRbuf[9]==197)
			{
			if(deckRbuf[13]&0x02)		//MEM round
				{
				VFL_DATA[0]|=BITON[5];				//MEM ON
				}
			else
				{
				VFL_DATA[0]&=BITOFF[5];				//MEM OFF
				}	
			if(deckRbuf[13]&0x01)		//SENS round
				{
				VFL_DATA[0]|=BITON[6];				//RING sensor ON	
				}
			else
				{
				VFL_DATA[0]&=BITOFF[6];				//RING sensor OFF	
				}	
			R_rx<<=1;	
			if(deckRbuf[13]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[13]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[13]&0x20)			//Ring B
				{
				B_rx+=1;	
				}	
			pad_tmp = 0x10000*deckRbuf[10] + 0x100*deckRbuf[11] + deckRbuf[12];
			if(PAD[5]!=pad_tmp)
				{
				PAD[5] = pad_tmp;	
				COLOR_SET(PAD[5], 5);	
				need_update_pads = 1;		
				}					
			}				
		else if(deckRbuf[9]==198)
			{
			R_rx<<=1;	
			if(deckRbuf[13]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[13]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[13]&0x20)			//Ring B
				{
				B_rx+=1;	
				}	
			pad_tmp = 0x10000*deckRbuf[10] + 0x100*deckRbuf[11] + deckRbuf[12];
			if(PAD[6]!=pad_tmp)
				{
				PAD[6] = pad_tmp;	
				COLOR_SET(PAD[6], 6);	
				need_update_pads = 1;		
				}		
			}			
		else if(deckRbuf[9]==199)
			{
			R_rx<<=1;	
			if(deckRbuf[13]&0x80)			//Ring R
				{
				R_rx+=1;	
				}
			G_rx<<=1;	
			if(deckRbuf[13]&0x40)			//Ring G
				{
				G_rx+=1;	
				}			
			B_rx<<=1;		
			if(deckRbuf[13]&0x20)			//Ring B
				{
				B_rx+=1;	
				}
			TIM1->CCR1 = R_rx;
			TIM1->CCR2 = G_rx;
			TIM1->CCR3 = B_rx;	
			pad_tmp = 0x10000*deckRbuf[10] + 0x100*deckRbuf[11] + deckRbuf[12];
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
		if(prev_pl_sect!=deckRbuf[16])
			{
			prev_pl_sect = deckRbuf[16];	
			if(prev_pl_sect<137)
				{
				draw_playing_sector(prev_pl_sect);	
				}
			}			
		if(prev_cue_sect!=deckRbuf[15])
			{
			prev_cue_sect = deckRbuf[15];	
			if((prev_cue_sect<85) && (prev_slip_sect>84))
				{
				draw_cue_sector(prev_cue_sect);	
				}
			}		
		if(prev_slip_sect!=deckRbuf[14])
			{
			prev_slip_sect = deckRbuf[14];	
			if(prev_slip_sect<85)
				{
				draw_slip_sector(prev_slip_sect);	
				}
			}		
		}
	/* BUTTON PLAY ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_PLAY_Pin)==0)	
		{
		deckTbuf[10]|=BITON[7];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[7];
		}	
	/* BUTTON CUE ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_CUE_Pin)==0)
		{
		deckTbuf[10]|=BITON[6];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[6];
		}
	/* BUTTON REV ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_REV_Pin)==0)	
		{
		deckTbuf[10]|=BITON[5];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[5];
		}	
	/* BUTTON MT ---------------------------------------------------------*/
	if((GPIOC->IDR & BUTTON_MT_Pin)==0)	
		{
		deckTbuf[11]|=BITON[1];
		}	
	else
		{
		deckTbuf[11]&=BITOFF[1];
		}
	/* BUTTON NEXT ---------------------------------------------------------*/	
	if((GPIOC->IDR & BUTTON_NEXT_Pin)==0)	
		{
		deckTbuf[11]|=BITON[2];
		}	
	else
		{
		deckTbuf[11]&=BITOFF[2];
		}	
	/* BUTTON JOG ---------------------------------------------------------*/	
	if((GPIOB->IDR & BUTTON_JOG_Pin)==0)	
		{
		deckTbuf[11]|=BITON[0];
		}	
	else
		{
		deckTbuf[11]&=BITOFF[0];
		}	
	/* BUTTON 8L ---------------------------------------------------------*/			
	if((GPIOB->IDR & BUTTON_8L_Pin)==0)	
		{
		deckTbuf[10]|=BITON[1];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[1];
		}	
	/* BUTTON RELOOP ---------------------------------------------------------*/	
	if((GPIOB->IDR & BUTTON_RELOOP_Pin)==0)	
		{
		deckTbuf[10]|=BITON[0];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[0];
		}	
	/* BUTTON PREV ---------------------------------------------------------*/	
	if((GPIOA->IDR & BUTTON_PREV_Pin)==0)	
		{
		deckTbuf[11]|=BITON[3];
		}	
	else
		{
		deckTbuf[11]&=BITOFF[3];
		}		
	/* BUTTON 4L ---------------------------------------------------------*/		
	if((GPIOA->IDR & BUTTON_4L_Pin)==0)	
		{
		deckTbuf[10]|=BITON[2];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[2];
		}		
	/* BUTTON SLIP ---------------------------------------------------------*/
	if((GPIOA->IDR & BUTTON_SLIP_Pin)==0)	
		{
		deckTbuf[10]|=BITON[4];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[4];
		}	
	/* BUTTON VINYL ---------------------------------------------------------*/	
	if((GPIOA->IDR & BUTTON_VINYL_Pin)==0)	
		{
		deckTbuf[10]|=BITON[3];
		}	
	else
		{
		deckTbuf[10]&=BITOFF[3];
		}				
	/* PAD_0 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_0_Pin)==0)	
		{
		deckTbuf[9]|=BITON[7];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[7];
		}		
	/* PAD_1 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_1_Pin)==0)	
		{
		deckTbuf[9]|=BITON[6];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[6];
		}
	/* PAD_2 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_2_Pin)==0)	
		{
		deckTbuf[9]|=BITON[5];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[5];
		}	
	/* PAD_3 ---------------------------------------------------------*/		
	if((GPIOC->IDR & BUTTON_PAD_3_Pin)==0)	
		{
		deckTbuf[9]|=BITON[4];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[4];
		}				
	/* PAD_4 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_4_Pin)==0)	
		{
		deckTbuf[9]|=BITON[3];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[3];
		}		
	/* PAD_5 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_5_Pin)==0)	
		{
		deckTbuf[9]|=BITON[2];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[2];
		}		
	/* PAD_6 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_6_Pin)==0)	
		{
		deckTbuf[9]|=BITON[1];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[1];
		}
	/* PAD_7 ---------------------------------------------------------*/		
	if((GPIOB->IDR & BUTTON_PAD_7_Pin)==0)	
		{
		deckTbuf[9]|=BITON[0];
		}	
	else
		{
		deckTbuf[9]&=BITOFF[0];
		}
	deckTbuf[12] = 0x00;				
	deckTbuf[13] = 0x00;			
	deckTbuf[14] = 0x00;
	deckTbuf[15] = 0x00;	
	deckTbuf[16] = 0x00;	
	#endif			
	CalcTXCRC();	
	}	




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

