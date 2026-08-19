/////////////////////////////////////////////////////////////////////////////////////////////////
// 	deck_transfer
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t CheckRXAcrc(void);
uint8_t CheckRXBcrc(void);
void SET_JOG_COLOR(uint8_t dk, uint32_t col);
void CalcTXCRC(uint8_t txp);
void PADSA(uint8_t pd);
void PADSB(uint8_t pd);


//////////////////////////////////////////////////////////
//
//		Timer 400Hz
//
//	v.0.93				
//	4.5-12uS	
//	Duty: 0,2-0,3%
//
void TIM2_IRQHandler(void)
	{	
	//TEST_H;	
	if(TXpage<7)
		{
		TXpage++;	
		}
	else
		{
		TXpage = 0;	
		}
		
	#include "decka_handler.h"	
	#include "deckb_handler.h"	
				
	if(timer_time<48)
		{
		timer_time++;	
		}
	else
		{
		timer_time = 0;	
		LOOP_LEDS_BLINK++;					//loop leds	
		/// deck A ///		
		if(loop_active[dkA] && (track_play_now[dkA]!=0))
			{
			if(LOOP_LEDS_BLINK%4==0)	
				{
				deckTbuf[3][0]|=0x08;	
				deckTbuf[2][0]|=0x10;
				loop_act_gui[dkA] = 1;	
				loopmode_need_update[dkA] = 1;		
				}
			else if(LOOP_LEDS_BLINK%4==2)
				{
				if(loop_lenght[dkA]>8)   //8 or more beats	
					{
					deckTbuf[3][0]&=0xF7;	
					}
				else
					{
					deckTbuf[2][0]&=0xEF;	
					}
				loop_act_gui[dkA] = 0;	
				loopmode_need_update[dkA] = 1;			
				}
			}
		/// deck B ///		
		if(loop_active[dkB] && (track_play_now[dkB]!=0))
			{
			if(LOOP_LEDS_BLINK%4==0)	
				{
				deckTbuf[3][8]|=0x08;	
				deckTbuf[2][8]|=0x10;
				loop_act_gui[dkB] = 1;	
				loopmode_need_update[dkB] = 1;		
				}
			else if(LOOP_LEDS_BLINK%4==2)
				{
				if(loop_lenght[dkB]>8)   //8 or more beats	
					{
					deckTbuf[3][8]&=0xF7;	
					}
				else
					{
					deckTbuf[2][8]&=0xEF;	
					}
				loop_act_gui[dkB] = 0;	
				loopmode_need_update[dkB] = 1;			
				}
			}		
		/// deck A ///	
		if(ENC_LED_BLINK[dkA]<8)
			{	
			load_animation_en[dkA] = 1;	
			if(ENC_LED_BLINK[dkA]%2==0)
				{
				GPIOA->BSRR = 0x000A0000;	//LED_LOAD0_Pin|LED_ENC_Pin	
				}
			else
				{
				GPIOA->BSRR = 0x0000000A;  	//LED_LOAD0_Pin|LED_ENC_Pin		
				}
			ENC_LED_BLINK[dkA]++;	
			}
		else
			{
			load_animation_en[dkA] = 0;	
			}
		/// deck B ///	
		if(ENC_LED_BLINK[dkB]<8)
			{	
			load_animation_en[dkB] = 1;	
			if(ENC_LED_BLINK[dkB]%2==0)
				{
				GPIOA->BSRR = 0x000C0000;	//LED_LOAD0_Pin|LED_ENC_Pin	
				}
			else
				{
				GPIOA->BSRR = 0x0000000C;  	//LED_LOAD0_Pin|LED_ENC_Pin		
				}
			ENC_LED_BLINK[dkB]++;	
			}
		else
			{
			load_animation_en[dkB] = 0;	
			}
			
		if(LED_SD_timer<7)
			{
			LED_SD_timer++;		
			if(LED_SD_timer==4)
				{
				TIM_PLAY_LED[dkA] = 1;
				TIM_CUE_LED[dkA] = 0;	
				TIM_PLAY_LED[dkB] = 1;
				TIM_CUE_LED[dkB] = 0;		
				}
			else if(LED_SD_timer==2 || LED_SD_timer==6)
				{
				TIM_CUE_LED[dkA] = 1;
				TIM_CUE_LED[dkB] = 1;	
				}	
			}
		else
			{
			if(track_play_now[dkA]!=0)
				{
				TIM_CUE_LED[dkA] = 0;
				}
			TIM_PLAY_LED[dkA] = 0;	
			if(track_play_now[dkB]!=0)
				{
				TIM_CUE_LED[dkB] = 0;
				}
			TIM_PLAY_LED[dkB] = 0;	
			LED_SD_timer = 0;	
			}
		/// deck A ///	
		if((LED_SD_timer==1 || LED_SD_timer==5) && track_play_now[dkA]!=0)
			{
			if(((all_long[dkA]-(play_adr[dkA]/294))<4500) && end_track[dkA]==0)
				{		
				if(DRAWN_IN_REMAIN[dkA]==1)
					{
					need_DSW[dkA] = 1;	
					}
				else
					{
					need_DSW[dkA] = 2;	
					}
				}
			else	
				{	
				if((DRAWN_IN_REMAIN[dkA]==0) && (REMAIN_ENABLE[dkA]==1))
					{
					need_DSW[dkA] = 2;	
					}
				else if((DRAWN_IN_REMAIN[dkA]==1) && (REMAIN_ENABLE[dkA]==0))
					{
					need_DSW[dkA] = 1;			
					}
				if(jog_light[dkA]==0)	
					{	
					SET_JOG_COLOR(dkA, jog_work_color[dkA]);
					jog_light[dkA] = 1;	
					}
				}		
			}	
		else if((LED_SD_timer!=1 && LED_SD_timer!=5) && (track_play_now[dkA]!=0) && (all_long[dkA]-(play_adr[dkA]/294))<2250 && end_track[dkA]==0)
			{	
			if(DRAWN_IN_REMAIN[dkA]==1)
				{
				need_DSW[dkA] = 1;	
				}
			else
				{
				need_DSW[dkA] = 2;	
				}
			}	
		/// deck B ///	
		if((LED_SD_timer==1 || LED_SD_timer==5) && track_play_now[dkB]!=0)
			{
			if(((all_long[dkB]-(play_adr[dkB]/294))<4500) && end_track[dkB]==0)
				{		
				if(DRAWN_IN_REMAIN[dkB]==1)
					{
					need_DSW[dkB] = 1;	
					}
				else
					{
					need_DSW[dkB] = 2;	
					}
				}
			else	
				{	
				if((DRAWN_IN_REMAIN[dkB]==0) && (REMAIN_ENABLE[dkB]==1))
					{
					need_DSW[dkB] = 2;	
					}
				else if((DRAWN_IN_REMAIN[dkB]==1) && (REMAIN_ENABLE[dkB]==0))
					{
					need_DSW[dkB] = 1;			
					}
				if(jog_light[dkB]==0)	
					{	
					SET_JOG_COLOR(dkB, jog_work_color[dkB]);
					jog_light[dkB] = 1;	
					}
				}		
			}	
		else if((LED_SD_timer!=1 && LED_SD_timer!=5) && (track_play_now[dkB]!=0) && (all_long[dkB]-(play_adr[dkB]/294))<2250 && end_track[dkB]==0)
			{	
			if(DRAWN_IN_REMAIN[dkB]==1)
				{
				need_DSW[dkB] = 1;	
				}
			else
				{
				need_DSW[dkB] = 2;	
				}
			}				
//		if(HAL_GPIO_ReadPin(KEY_MENU_GPIO_Port, KEY_MENU_Pin)==0  &&	dSHOW != UTILITY && countUTILITY<6)		///counter for long press MENU button
//			{
//			countUTILITY++;	
//			}
//		else
//			{
//			countUTILITY = 0;	
//			}
		}	
		
	CalcTXCRC(TXpage);
	Decks_DMA(&hspi2, &deckTbuf[TXpage][0], deckRbuf, 16);		
	HAL_TIM_IRQHandler(&htim2);	
	//TEST_L;	
	}
	
////////////////////////////////////////////////////////////
//Function Checksum for RX package deck A	
//
//
//	
uint8_t CheckRXAcrc(void)
	{
	uint8_t sm, n;			
	sm = 0;	
	for(n=0;n<7;n++)
		{
		sm+=deckRbuf[n];	
		}	
	if(sm==deckRbuf[7])
		{
		return 1;	
		}
	else
		{
		return 0;	
		}
	};
	
////////////////////////////////////////////////////////////
//Function Checksum for RX package deck B	
//
//
//	
uint8_t CheckRXBcrc(void)
	{
	uint8_t sm, n;			
	sm = 0;	
	for(n=0;n<7;n++)
		{
		sm+=deckRbuf[n+8];	
		}	
	if(sm==deckRbuf[15])
		{
		return 1;	
		}
	else
		{
		return 0;	
		}
	};

////////////////////////////////////////////////////////
//
//
//
void CalcTXCRC(uint8_t txp)
	{
	uint8_t n;
	deckTbuf[txp][7] = 0;
	deckTbuf[txp][15] = 0;	
	for(n=0;n<7;n++)
		{
		deckTbuf[txp][7]+=deckTbuf[txp][n];
		deckTbuf[txp][15]+=deckTbuf[txp][n+8];	
		}
	return;	
	};	

////////////////////////////////////////////////////////////
//
//		set jog ring color
//		dk - number of deck
//	
void SET_JOG_COLOR(uint8_t dk, uint32_t col)
	{
	uint8_t r, g, b, i;
	if(dk>0)
		{
		dk = 8;	
		}
	r = (col>>16)&0x000000FF;	
	b = (col>>8)&0x000000FF;
	g = col&0x000000FF;
	for(i=0;i<8;i++)
		{
		if((r>>(7-i))&0x01)
			{
			deckTbuf[i][dk]|=	0x80;
			}
		else
			{
			deckTbuf[i][dk]&=	0x7F;
			}			
		if((g>>(7-i))&0x01)
			{
			deckTbuf[i][dk]|=	0x40;
			}
		else
			{
			deckTbuf[i][dk]&=	0xBF;
			}				
		if((b>>(7-i))&0x01)
			{
			deckTbuf[i][dk]|=	0x20;
			}
		else
			{
			deckTbuf[i][dk]&=	0xDF;
			}						
		}
	return;	
	};	
	
	
////////////////////////////////////////////////////////
//
//
//	
void PADSA(uint8_t pd)
	{	
	if(lock_control[dkA]==0)	
		{	
		if(HCUE_adr[dkA][0][pd]!=0xFFFF)
			{
			seek_pos[dkA]	= 294*HCUE_adr[dkA][0][pd];
			need_seek[dkA] = 3;
			}
		if(play_enable[dkA]==0)
			{
			change_speed[dkA] = NO_CHANGE;	
			play_enable[dkA] = 1;
			//change masterdeck	
			if(play_enable[dkB]==0 && masterdeck!=dkA)
				{
				masterdeck = dkA;
				tempo_need_update[dkA] = 2;
				tempo_need_update[dkB] = 2;	
				}		
			}	
		}
	return;	
	};	

	
////////////////////////////////////////////////////////
//
//
//	
void PADSB(uint8_t pd)
	{
	if(lock_control[dkB]==0)	
		{	
		if(HCUE_adr[dkB][0][pd]!=0xFFFF)
			{
			seek_pos[dkB]	= 294*HCUE_adr[dkB][0][pd];
			need_seek[dkB] = 3;
			}
		if(play_enable[dkB]==0)
			{
			change_speed[dkB] = NO_CHANGE;	
			play_enable[dkB] = 1;
			//change masterdeck	
			if(play_enable[dkA]==0 && masterdeck!=dkB)
				{
				masterdeck = dkB;
				tempo_need_update[dkA] = 2;
				tempo_need_update[dkB] = 2;	
				}	
			}	
		}
	return;	
	};		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



