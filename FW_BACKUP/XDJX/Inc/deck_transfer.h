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
void SET_JOG_COLOR(uint8_t dk, uint32_t col);


//////////////////////////////////////////////////////////
//
//		Timer 400Hz
//
void TIM2_IRQHandler(void)
	{	
	if(TXpage<7)
		{
		TXpage++;	
		}
	else
		{
		TXpage = 0;	
		}
		
	if(CheckRXAcrc()==1)
		{
		uint32_t ptch;
		if((deckRbuf[1]&0x01) && PLAY_BUTTON_pressed==0)										///////////PLAY button
			{
			if(lock_control==0)	
				{
				if(CUE_BUTTON_pressed==0)
					{
					if(play_enable)
						{
						play_enable = 0;		
						change_speed = NEED_DOWN;
						}
					else
						{
						if(SLIPEN)					//SLIP MODE ENABLE
							{	
							play_adr = slip_play_adr;	
							change_speed = NO_CHANGE;
							slip_play_enable = 1;	
							}
						else if(CUE_ADR!=(play_adr/294) && (deckRbuf[2]&0x80)==0)			//when playback starts from any adress and touch disable
							{
							change_speed = NEED_UP;
							}
						else																//when playback starts from CUE adress
							{
							change_speed = NO_CHANGE;	
							}
						play_enable = 1;	
						}
					}
				else
					{
					keep_to_play = 1;	
					}
				}
			PLAY_BUTTON_pressed = 1;	
			}
		else if((deckRbuf[1]&0x01)==0 && PLAY_BUTTON_pressed==1)
			{
			PLAY_BUTTON_pressed = 0;	
			}
		else if((deckRbuf[1]&0x02) && CUE_BUTTON_pressed==0)										///////////CUE button
			{
			if(lock_control==0)	
				{	
				if(play_enable && ((deckRbuf[2]&0x80)==0))								//return to CUE, when track playing				play && touch disable
					{
					pitch = 0;	
					play_enable = 0;		
					if(SLIPEN)					//SLIP MODE ENABLE
						{	
						slip_play_enable = 0;		
						slip_play_adr = 294*CUE_ADR;	
						}	
					CUE_OPERATION = CUE_NEED_CALL;			
					}	
				else if((play_enable==0) && (CUE_ADR!=(play_adr/294)))			//Set new CUE, when track stopped		
					{
					LOOP_OUT = 0;	
					CUE_OPERATION = CUE_NEED_SET;						
					}
				else if((play_enable==0) && (CUE_ADR==(play_adr/294)))				//return to CUE adress, when track stopped
					{
					change_speed = NO_CHANGE;	
					//play_adr = 294*CUE_ADR;		
					if(SLIPEN)					//SLIP MODE ENABLE
						{			
						slip_play_adr = play_adr;
						slip_play_enable = 1;	
						}	
					play_enable = 1;							
					}
				else if(play_enable && ((deckRbuf[2]&0x80)!=0) && (deckTbuf[2][0]&0x08))				//Set new CUE, when track played and press jog and JOG in Vinyl MODE
					{
					LOOP_OUT = 0;	
					CUE_OPERATION = CUE_NEED_SET;	
					play_enable = 0;							
					}
				}
			CUE_BUTTON_pressed = 1;	
			}
		else if((deckRbuf[1]&0x02)==0 && CUE_BUTTON_pressed==1)
			{
			if(lock_control==0)	
				{	
				if(keep_to_play==0)		//button play not pressed	
					{
					play_enable = 0;
					pitch = 0;		
					slip_play_enable = 0;	
					play_adr = 294*CUE_ADR;	
					if(SLIPEN)					//SLIP MODE ENABLE
						{	
						slip_play_adr = play_adr;	
						}
		//			if((Tbuffer[19]&0x20)==0)   //CDJ mode
		//				{
		//				pitch = 0;	
		//				}
					}
				else
					{
					keep_to_play = 0;	
					}
				offset_adress = 0;																///	   temporary operation		
				}
			CUE_BUTTON_pressed = 0;	
			}
//		else if((Rbuffer[14]&0x4) && REALTIME_CUE_BUTTON_pressed==0)										///////////REALTIME CUE button
//			{
//			if(lock_control==0)	
//				{	
//				if((play_enable==1) && (CUE_ADR!=(play_adr/294)) && loop_active==0)			//Set new CUE, when track play		
//					{
//					LOOP_OUT = 0;	
//					CUE_OPERATION = CUE_NEED_SET;	
//					}
//				}
//			REALTIME_CUE_BUTTON_pressed = 1;	
//			}
//		else if((Rbuffer[14]&0x4)==0 && REALTIME_CUE_BUTTON_pressed==1)
//			{
//			REALTIME_CUE_BUTTON_pressed = 0;	
//			}			
//		else if((Rbuffer[14]&0x08) && LOOP_OUT_BUTTON_pressed==0)										///////////LOOP OUT button
//			{
//			if(lock_control==0)	
//				{	
//				if(loop_active==0 && CUE_ADR<play_adr/294)
//					{
//					if(QUANTIZE && dSHOW==WAVEFORM)
//						{
//						if(((play_adr/294)>(BEATGRID[bars-1]+((BEATGRID[bars] - BEATGRID[bars-1])/2))) || bars==0)	
//							{
//							LOOP_OUT = BEATGRID[bars];								//next bar >> |
//							}
//						else
//							{
//							if(CUE_ADR==BEATGRID[bars-1])
//								{
//								LOOP_OUT = BEATGRID[bars];								//next bar >> |	
//								}
//							else	
//								{
//								LOOP_OUT = BEATGRID[bars-1];							//previous bar <<	|
//								}
//							CUE_OPERATION = CUE_NEED_CALL;		
//							}	
//						}
//					else
//						{
//						LOOP_OUT = play_adr/294;
//						CUE_OPERATION = CUE_NEED_CALL;	
//						}	
//					loop_active = 1;	
//					}
//				}
//			LOOP_OUT_BUTTON_pressed = 1;	
//			}
//		else if((Rbuffer[14]&0x08)==0 && LOOP_OUT_BUTTON_pressed==1)
//			{
//			LOOP_OUT_BUTTON_pressed = 0;	
//			}			
		else if((deckRbuf[1]&0x80) && RELOOP_BUTTON_pressed==0)										///////////RELOOP button
			{
			track_need_load = 1;	
//			if(lock_control==0)	
//				{		
//				if(loop_active)
//					{
//					loop_active = 0;			
//					}
//				else if(loop_active==0 && CUE_ADR<LOOP_OUT)
//					{
//					loop_active = 1;	
//					}
//				if(dSHOW==WAVEFORM)							//Redraw cue on dynamic waveform
//					{
//					forcibly_redraw = 1;
//					}		
//				}	
			RELOOP_BUTTON_pressed = 1;	
			}
		else if((deckRbuf[1]&0x80)==0 && RELOOP_BUTTON_pressed==1)
			{
			RELOOP_BUTTON_pressed = 0;	
			}					
			
		if((deckRbuf[1]&0x04) && REVERSE_BUTTON_pressed==0)					///////////reverse switch position
			{
			if(REVEN)
				{
				if(SLIPEN)					//SLIP MODE ENABLE
					{	
					play_adr = slip_play_adr;	
					}		
				if(keep_slip)
					{
					keep_slip = 0;	
					}
				deckTbuf[1][0]|= 0x08;				//disable red led reverse
				REVEN = 0;
				}	
			else
				{
				if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
					{
					REVERSE_BUTTON_pressed = 2;	
					}	
				else
					{
					if(SLIPEN)			//SLIP MODE ENABLE
						{
						keep_slip = 1;	
						}
					else
						{
						SLIPEN = 1;	
						if(play_enable)
							{
							slip_play_enable = 1;	
							}
						slip_play_adr = play_adr; 	
						}			
					REVERSE_BUTTON_pressed = 1;		
					}				
				deckTbuf[1][0]&= 0xF7; //enable red led reverse
				REVEN = 1;						
				}
			}
		else if((deckRbuf[1]&0x04)==0 && REVERSE_BUTTON_pressed>0)
			{			
			if(REVERSE_BUTTON_pressed==1)
				{
				if(SLIPEN)					//SLIP MODE ENABLE
					{	
					play_adr = slip_play_adr;	
					}		
				if(keep_slip)
					{
					keep_slip = 0;	
					}
				else
					{
					slip_play_enable = 0;	
					SLIPEN = 0;	
					}
				deckTbuf[1][0]|= 0x08;				//disable red led reverse
				REVEN = 0;						
				}
			REVERSE_BUTTON_pressed = 0;		
			}			
			
			
			
			
///////////////////////////////////////////////////JOG MECHANICAL PROCESS///////////////////////////////////////////////			
			
			
		if(inertial_rotation)					//have a mistacke 2025
			{
			if((REVEN==0 && ((deckRbuf[5]==0xFF && deckRbuf[6]==0xFF) || (((deckRbuf[4]&0x80)==0) && pitch<potenciometer_tempo))) || 
				((REVEN) && ((deckRbuf[5]==0xFF && deckRbuf[6]==0xFF && (deckRbuf[4]&0x80)==0) || ((deckRbuf[4]&0x80) && pitch<potenciometer_tempo))))		//if rotation foward and stopped	
				{
				inertial_rotation = 0;	
				}
			}				
		if(play_enable || (need_call_to_cue==3 && ((deckRbuf[5]==0xFF && deckRbuf[6]==0xFF) || (deckRbuf[2]&0x80)!=0)))			//touch disable && rotation disable or play enable
			{
			need_call_to_cue = 0;	
			}	
		else if((deckRbuf[2]&0x80)==0 && need_call_to_cue==2)
			{
			pitch = 0;	
			play_adr = 294*CUE_ADR;	
			need_call_to_cue = 3;	
			}		
			
		if(((deckRbuf[2]&0x80)!=0 || (play_enable==0 && (CUE_ADR!=(play_adr/294))) || inertial_rotation) && (deckTbuf[2][0]&0x08))				/////////////(touch enable	|| play_enable==0) && Vinyl mode enable
			{
			pitch_for_slip = potenciometer_tempo;	
			
			if(JOG_PRESSED==0)
				{
				if((deckRbuf[2]&0x80)!=0) 			//touch enable
					{
					change_speed = NEED_DOWN;	
					JOG_PRESSED = 2;	
					}
				else
					{
					JOG_PRESSED = 1;
					}				
				}	
			if(play_enable==0)	
				{
				if(((deckRbuf[2]&0x80)!=0) && (CUE_ADR==(play_adr/294)) && need_call_to_cue==0)				//touch enable + play_enable==0 + CUE_ADR==(play_adr/294)
					{
					need_call_to_cue = 1;	
					}		
				//if(need_call_to_cue==1 && (Rbuffer[12]&0x20)==0 && play_enable==0 && (Rbuffer[12]&0x80)==0)			//touch disable		//problem on XDJ-RX2
				else if(need_call_to_cue==1 && (deckRbuf[2]&0x80)==0)			//touch disable	_/
					{
					need_call_to_cue = 2;	
					}					
				}
			if(deckRbuf[5]!=0xFF || deckRbuf[6]!=0xFF)					//rotation detect
				{
				if(need_call_to_cue<2)
					{					
					change_speed = NO_CHANGE;	
					
					if(((deckRbuf[4]&0x80)==0) && end_of_track)				//foward rotation + end_of_track
						{
						pitch = 0;
						change_speed = NO_CHANGE;	
						}
					else
						{
						ptch = (256*deckRbuf[5]+deckRbuf[6]);
						if(ptch<86)
							{
							ptch = 86;	
							}
						ptch = 5574324/ptch;
						pitch = ptch;						
						}
					pitch_for_slip = potenciometer_tempo; 	
					inertial_rotation = 1;						
					}
				}		
			else if(change_speed==NO_CHANGE) 
				{			
				pitch = 0;	
				}

			if(change_speed==NO_CHANGE)
				{	
				if(deckRbuf[4]&0x80)				//foward/reverse rotation
					{
					reverse = 1;
					}
				else
					{
					reverse = 0;	
					}		
				}
			else
				{
				if(REVEN)					//reverse diode enable
					{
					reverse = 1;
					}
				else
					{
					reverse = 0;	
					}	
				}
			deckTbuf[5][0]|=0x10;	//touch enable circle on display	
			}		
		else if(deckRbuf[5]==0xFF && deckRbuf[6]==0xFF && (deckRbuf[2]&0x80)==0)				///////////////////////touch disable and rotation disable
			{			
			pitch_for_slip = potenciometer_tempo;
			if(JOG_PRESSED>0) //jog PRESSED -> UNPRESSED
				{
				if(SLIPEN)					//SLIP MODE ENABLE
					{	
					change_speed = NO_CHANGE;	
					}
				else if(JOG_PRESSED==2 && play_enable)
					{
					change_speed = NEED_UP;	
					}					
				JOG_PRESSED = 0;	
				}
			if(play_enable)
				{	
				if(end_of_track && REVEN==0)					//stop on end (to remove noise at the end of the track)
					{
					change_speed = NO_CHANGE;	
					pitch = 0;	
					}	
				else if(change_speed==NO_CHANGE)
					{
					pitch = potenciometer_tempo;	
					}
				if(REVEN)					//reverse diode enable
					{
					reverse = 1;
					}
				else
					{
					reverse = 0;	
					}		
				}
			if(deckTbuf[5][0]&0x10)					//jog UNPRESSED
				{
				if(SLIPEN)					//SLIP MODE ENABLE
					{	
					play_adr = slip_play_adr;	
					}	
				deckTbuf[5][0]&=0xEF;				//disable touch circle on display
				}	
			}
		else if((deckRbuf[5]!=0xFF || deckRbuf[6]!=0xFF) && play_enable)						//rotation detected			(pitch bend)	
			{
			if(end_of_track==0)
				{
				ptch = (256*deckRbuf[5]+deckRbuf[6]);
				if(ptch>139)
					{
					ptch = ptch-139;	
					}
				else
					{
					ptch = 1;	
					}
				ptch = 150000/ptch;
					
				if(ptch>4225)
					{
					ptch = 4225;	
					}	
				if((((deckRbuf[4]&0x80)==0) && (REVEN==0)) || ((deckRbuf[4]&0x80) && REVEN))		//foward rotation and reverse off OR reverse rotation and reverse on (pitch bend)			
					{
					ptch+= potenciometer_tempo;
					if(ptch>20000)
						{
						ptch = 20000;	
						}
					pitch = ptch;
					}
				else if(((deckRbuf[4]&0x80) && (REVEN==0)) || (((deckRbuf[4]&0x80)==0) && REVEN))	 //reverse rotation and reverse off OR foward rotation and reverse on(pitch bend)	
					{
					if(ptch<potenciometer_tempo)
						{
						pitch = potenciometer_tempo - ptch;
						}
					else
						{
						pitch = 0;	
						}
					}		
				}
			else
				{
				pitch = 0;		
				}			
			if(REVEN)					//reverse diode enable
				{
				reverse = 1;
				}
			else
				{
				reverse = 0;	
				}	
			if(deckTbuf[5][0]&0x10)					//jog UNPRESSED
				{
				if(SLIPEN)					//SLIP MODE ENABLE
					{	
					play_adr = slip_play_adr;	
					}	
				deckTbuf[5][0]&=0xEF;				//disable touch circle on display	
				}	
			else
				{
				pitch_for_slip = pitch;		
				}
			}



			
		if((TXpage%2)==0)		// 1/2 cycles for this deck
			{	
			if((deckTbuf[4][0]&0x10)==0)				//TEMPRO RESET ON
				{
				potenciometer_tempo = 10000;	
				}
			else				//////////////////////////////////////////TEMPO CALCULATION
				{	
				pot10b = 0x100*deckRbuf[2] + deckRbuf[3]; 	
				pot10b&= 0x03FF;
				if(pot10b==512)	
					{
					potenciometer_tempo = 10000;	
					}
				else if(pot10b>512)					/////pitch>0%
					{
					potwd = pot10b-512;	
					if(tempo_range==0)										//	6%
						{
						potwd*= 2409;	
						potenciometer_tempo = potwd>>12; 	
						potenciometer_tempo*= 2;
						}	
					else if(tempo_range==1)										//	10%	
						{
						potwd*= 1608;	
						potenciometer_tempo = potwd>>12; 	
						potenciometer_tempo*= 5;	
						}	
					else if(tempo_range==2)										//	16%	
						{
						potwd*= 2569;	
						potenciometer_tempo = potwd>>12; 	
						potenciometer_tempo*= 5;
						}	
					else																		//	WIDE	
						{
						potwd*= 1608;	
						potenciometer_tempo = potwd>>12; 	
						potenciometer_tempo*= 50;	
						}	
					potenciometer_tempo+= 10000;	
					}
				else				/////pitch<0%
					{
					potwd = 511-pot10b;		
					if(tempo_range==0)										//	6%	
						{	
						potwd*= 2409;	
						potenciometer_tempo = potwd>>12; 	
						potenciometer_tempo*= 2;	
						}	
					else if(tempo_range==1)										//	10%	
						{	
						potwd*= 1608;	
						potenciometer_tempo = potwd>>12; 	
						potenciometer_tempo*= 5;	
						}
					else if(tempo_range==2)										//	16%	
						{	
						potwd*= 2569;	
						potenciometer_tempo = potwd>>12; 	
						potenciometer_tempo*= 5;
						}	
					else																			//	WIDE
						{	
						potwd*= 1608;	
						potenciometer_tempo = potwd>>12; 	
						potenciometer_tempo*= 50;
						}
					potenciometer_tempo = 10000 - potenciometer_tempo;	
					}				
				}
			if(previous_potenciometer_tempo != potenciometer_tempo)
				{
				previous_potenciometer_tempo = potenciometer_tempo;	
				tempo_need_update = 1;
				}	
			}
			
			
			////////////////////////////////////////cicle divider////////////////////////////
			
		if(TXpage==0)
			{

			}
		if(TXpage==1)
			{

			}
		else if(TXpage==2)
			{
//				if((Rbuffer[18]&0x20) && TIME_MODE_BUTTON_pressed==0) 					///////////TIME MODE Button
//					{	
//					TIME_MODE_BUTTON_pressed = 1;	
//					}
//				else if((Rbuffer[18]&0x20)==0 && TIME_MODE_BUTTON_pressed==1)	
//					{
//					if(REMAIN_ENABLE)
//						{
//						REMAIN_ENABLE = 0;	
//						}
//					else
//						{
//						REMAIN_ENABLE = 1;	
//						}
//					time_mode_need_update = 1;		
//					TIME_MODE_BUTTON_pressed = 0;	
//					}	
//				else if((Rbuffer[18]&0x40) && QUANTIZE_BUTTON_pressed==0) 					///////////QUANTIZE Button
//					{
//					if(QUANTIZE)
//						{
//						QUANTIZE = 0;	
//						}
//					else
//						{
//						QUANTIZE = 1;	
//						}	
//					quantize_mode_need_update = 1;	
//					QUANTIZE_BUTTON_pressed = 1;	
//					}
//				else if((Rbuffer[18]&0x40)==0 && QUANTIZE_BUTTON_pressed==1)	
//					{		
//					QUANTIZE_BUTTON_pressed = 0;	
//					}
			}
		else if(TXpage==3)
			{

					
			}	
		else if(TXpage==4)
			{
//				if((Rbuffer[16]&0x10) && SEARCH_FF_BUTTON_pressed==0) 							///////////SEARCH FF>> Button
//					{
//					if(lock_control==0)	
//						{	
//						if(play_enable & play_adr<(all_long+100000))	
//							{
//							//SEEK_AUDIOFRAME(play_adr+100000);	
//							}
//						else if(play_enable==0 & play_adr/294<(all_long+1))
//							{	
//							play_adr+=294;
//							}
//						}
//					SEARCH_FF_BUTTON_pressed = 1;	
//					}
//				else if((Rbuffer[16]&0x10)==0 && SEARCH_FF_BUTTON_pressed==1)	
//					{
//					SEARCH_FF_BUTTON_pressed = 0;	
//					}	
//				else if((Rbuffer[16]&0x8) && SEARCH_REW_BUTTON_pressed==0) 							///////////SEARCH <<REW Button
//					{
//					if(lock_control==0)	
//						{		
//						if(play_enable & play_adr>100000)	
//							{
//							//SEEK_AUDIOFRAME(play_adr-100000);
//							}
//						else if(play_enable==0 & play_adr>294)
//							{	
//							play_adr-=294;
//							}
//						}	
//					SEARCH_REW_BUTTON_pressed = 1;	
//					}
//				else if((Rbuffer[16]&0x8)==0 && SEARCH_REW_BUTTON_pressed==1)	
//					{
//					SEARCH_REW_BUTTON_pressed = 0;	
//					}		

			if(track_play_now!=0)						//////////////////////////////LEDS/////////////////////////
				{					
				deckTbuf[0][0]|=0x10;				//CUE led on
					
				if(play_enable)
					{
					deckTbuf[0][0]|=0x10;	//PLAY and CUE led on
					}
				else											//Play led blink
					{	
					if(TIM_PLAY_LED)	
						{
						deckTbuf[0][0]|=0x08;	
						}
					else
						{
						deckTbuf[0][0]&=0xF7;
						}
					if(TIM_CUE_LED)	
						{
						deckTbuf[0][0]|=0x10;
						}
					else if(CUE_ADR!=play_adr/294)  
						{
						deckTbuf[0][0]&=0xEF;	
						}	
					}
						
//					if(loop_active)
//						{
//						if(LOOP_LEDS_BLINK%4==0)	
//							{
//							Tbuffer[17] |= 0x0C;	
//							}
//						else if(LOOP_LEDS_BLINK%4==2)
//							{
//							Tbuffer[17] &= 0xF3;	
//							}
//						}	
//					else
//						{
//						if(TIM_REALTIME_CUE_LED)	
//							{
//							Tbuffer[17] |= 0x0C;		
//							}
//						else
//							{
//							Tbuffer[17] &= 0xFB;	
//							}				
//						}				
//					if(loop_active || CUE_ADR<LOOP_OUT)
//						{
//						Tbuffer[17] |= 0x10;							//RELOOP EXIT LED ON
//						}
//					else
//						{
//						Tbuffer[17] &= 0xEF;							//RELOOP EXIT LED OFF	
//						}
				}
			}		
		else if(TXpage==5)
			{				
			if((deckRbuf[1]&0x10) && JOG_MODE_BUTTON_pressed==0) 							///////////Jog Mode button
				{
				if(deckTbuf[2][0]&0x08)			//VINYL => CDJ
					{
					deckTbuf[2][0]&=0xF7;
					deckTbuf[4][0]&=0xF7;						
					}	
				else												//CDJ => VINYL
					{	
					deckTbuf[2][0]|=0x08;
					deckTbuf[4][0]|=0x08;	
					}
				JOG_MODE_BUTTON_pressed = 1;	
				}
			else if((deckRbuf[1]&0x10)==0 && JOG_MODE_BUTTON_pressed==1)	
				{
				JOG_MODE_BUTTON_pressed = 0;	
				}
			else if((deckRbuf[2]&0x40) && TEMPO_BUTTON_pressed==0) 							///////////TEMPO Button
				{
				if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
					{
					if(deckTbuf[4][0]&0x10)				//ON_RESET => OFF_RESET
						{
						deckTbuf[4][0]&=0xEF;
						}	
					else												//OFF_RESET => ON_RESET
						{
						deckTbuf[4][0]|=0x10;
						}						
					}
				else
					{
					if(tempo_range<3)
						{
						tempo_range++;
						}
					else
						{
						tempo_range = 0;	
						}
					tempo_range_need_update = 1;
					}
				TEMPO_BUTTON_pressed = 1;	
				}
			else if((deckRbuf[2]&0x40)==0 && TEMPO_BUTTON_pressed==1)	
				{
				TEMPO_BUTTON_pressed = 0;	
				}	
			else if((deckRbuf[2]&0x20) && CALL_NEXT_BUTTON_pressed==0) 							///////////CALL NEXT Button	>
				{
				if(lock_control==0)	
					{		
					CUE_OPERATION = MEMORY_NEED_NEXT_SET;
					}
				CALL_NEXT_BUTTON_pressed = 1;	
				}
			else if((deckRbuf[2]&0x20)==0 && CALL_NEXT_BUTTON_pressed==1)	
				{
				CALL_NEXT_BUTTON_pressed = 0;	
				}
			else if((deckRbuf[2]&0x10) && CALL_PREVIOUS_BUTTON_pressed==0) 							///////////CALL PREVIOUS Button <
				{
				if(lock_control==0)	
					{		
					CUE_OPERATION = MEMORY_NEED_PREVIOUS_SET;
					}
				CALL_PREVIOUS_BUTTON_pressed = 1;	
				}
			else if((deckRbuf[2]&0x10)==0 && CALL_PREVIOUS_BUTTON_pressed==1)	
				{
				CALL_PREVIOUS_BUTTON_pressed = 0;	
				}


				if((deckRbuf[1]&0x08) && SLIP_BUTTON_pressed==0) 							///////////SLIP MODE Button
					{
					if(SLIPEN)					//ON_SLIP_MODE => OFF_SLIP_MODE
						{
//						if(UTILITY_SETTINGS[6]==1 && keep_slip==0 && (Tbuffer[17]&0x20))			//for switch rev?
//							{
//							keep_slip = 1;				
//							}
//						else
							{	
							if(keep_slip)
								{
								keep_slip = 0;	
								}
							else
								{
								slip_play_enable = 0;	
								SLIPEN = 0;	
								deckTbuf[1][0]|=0x10;		//led on		
								}									
							}							
						}	
					else												//OFF_SLIP_MODE => ON_SLIP_MODE
						{	
						SLIPEN = 1;
						deckTbuf[1][0]&=0xEF;		//led off	
						if(play_enable)
							{
							slip_play_enable = 1;	
							}
						slip_play_adr = play_adr; 	
						}
					SLIP_BUTTON_pressed = 1;	
					}
				else if((deckRbuf[1]&0x08)==0 && SLIP_BUTTON_pressed==1)	
					{
					SLIP_BUTTON_pressed = 0;	
					}		
			}			
		

				
		if(load_animation_enable)
			{
			//Tbuffer[21] = 0;					//disable red cue marker
			//Tbuffer[23] &= 0xDF;			//disable touch circle on display		
			deckTbuf[TXpage][4] = 135;				//command load animation	
			}
		else if(track_play_now==0)
			{
			deckTbuf[TXpage][4] = 135;
			deckTbuf[TXpage][5] = 85;	
			deckTbuf[TXpage][6] = 85; 	//cue none
			deckTbuf[0][0]&=0xE7;		//PLAY & CUE leds off
			deckTbuf[5][0]&=0xEF;				//disable touch circle on display		
			}
		else
			{
			deckTbuf[TXpage][6] = RED_CIRCLE_CUE_ADR; 	//cue		
			if(SLIPEN)					//SLIP MODE ENABLE
				{	
				deckTbuf[TXpage][5] = (1000*(((slip_play_adr/588)%135)+1)/1589);							//slip
				}
			else
				{
				deckTbuf[TXpage][5] = 85;	
				}				
			deckTbuf[TXpage][4] = (play_adr/588)%135;
			}




			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
		
			
				
		deckTbuf[TXpage][12] = 135;			//deck b 	
		}	
		
		


	deckTbuf[TXpage][14] = 85;  	//cue
	deckTbuf[TXpage][13] = 85;	//slip
	Decks_DMA(&hspi2, &deckTbuf[TXpage][0], deckRbuf, 16);
	
	
	////////////////////////////////////////////////from 1000hz (400) timer
	
	if(change_speed==NEED_UP)
		{
		if(end_of_track)
			{
			change_speed = NO_CHANGE;
			pitch = 0;			
			}
		else
			{
			if(pitch<potenciometer_tempo-acceleration_UP)
				{
				pitch+=acceleration_UP;	
				}
			else
				{
				change_speed = NO_CHANGE;
				pitch = potenciometer_tempo;	
				}		
			}
		}
	else if(change_speed==NEED_DOWN)
		{
		if(end_of_track)
			{
			change_speed = NO_CHANGE;
			pitch = 0;			
			}
		else	
			{
			if(pitch>acceleration_DOWN)
				{
				pitch-=acceleration_DOWN;	
				}
			else
				{
				change_speed = NO_CHANGE;
				pitch = 0;	
				}		
			}
		}

	if(timer_time<48)
		{
		timer_time++;	
		}
	else
		{
		timer_time = 0;
				
//		if(REALTIME_CUE_LED_BLINK<16)
//			{
//			if(REALTIME_CUE_LED_BLINK%4==0)
//				{
//				TIM_REALTIME_CUE_LED = 0;
//				}
//			else if(REALTIME_CUE_LED_BLINK%4==2)
//				{
//				TIM_REALTIME_CUE_LED = 1;
//				}
//			REALTIME_CUE_LED_BLINK++;	
//			}
			
		//LOOP_LEDS_BLINK++;					//loop leds	
			
//		if(ENCODER_LED_BLINK<8)
//			{	
//			load_animation_enable = 1;	
//			if(ENCODER_LED_BLINK%2==0)
//				{
//				HAL_GPIO_WritePin(GPIOB, LED_ENCODER_Pin, GPIO_PIN_RESET);	
//				}
//			else
//				{
//				HAL_GPIO_WritePin(GPIOB, LED_ENCODER_Pin, GPIO_PIN_SET);
//				}
//			ENCODER_LED_BLINK++;	
//			}
//		else
//			{
//			load_animation_enable = 0;	
//			}
			
		if(LED_SD_timer<7)
			{
			LED_SD_timer++;		
			if(LED_SD_timer==4)
				{
				TIM_PLAY_LED = 1;
				TIM_CUE_LED = 0;		
				//HAL_GPIO_WritePin(LED_SD_GPIO_Port, LED_SD_Pin, GPIO_PIN_SET);	
				}
			else if(LED_SD_timer==2 || LED_SD_timer==6)
				{
				TIM_CUE_LED = 1;					
				}	
			}
		else
			{
			if(track_play_now==0)
				{
				//HAL_GPIO_WritePin(LED_SD_GPIO_Port, LED_SD_Pin, GPIO_PIN_SET);
				TIM_PLAY_LED = 0;	
				}
			else
				{
				TIM_PLAY_LED = 0;	
				TIM_CUE_LED = 0;	
				//HAL_GPIO_WritePin(LED_SD_GPIO_Port, LED_SD_Pin, GPIO_PIN_RESET);	
				}
			LED_SD_timer = 0;	
			}
			
//		if((LED_SD_timer==1 || LED_SD_timer==5) && track_play_now!=0)
//			{
//			if(((all_long-(play_adr/294))<4500) && end_of_track==0)
//				{		
//				if(DRAWN_IN_REMAIN==1)
//					{
//					need_DSW = 1;	
//					}
//				else
//					{
//					need_DSW = 2;	
//					}
//				}
//			else	
//				{	
//				if((DRAWN_IN_REMAIN==0) && (REMAIN_ENABLE==1))
//					{
//					need_DSW = 2;	
//					}
//				else if((DRAWN_IN_REMAIN==1) && (REMAIN_ENABLE==0))
//					{
//					need_DSW = 1;			
//					}
//				if(TIM12->CCR1==0)	
//					{
//					TIM12->CCR1 = JOG_BRIGHTNESS[UTILITY_SETTINGS[10]];		
//					}
//				}		
//			}	
//		else if((LED_SD_timer!=1 && LED_SD_timer!=5) && (track_play_now!=0) && (all_long-(play_adr/294))<2250 && end_of_track==0)
//			{	
//			if(DRAWN_IN_REMAIN==1)
//				{
//				need_DSW = 1;	
//				}
//			else
//				{
//				need_DSW = 2;	
//				}
//			}	
			
//		if(HAL_GPIO_ReadPin(KEY_MENU_GPIO_Port, KEY_MENU_Pin)==0  &&	dSHOW != UTILITY && countUTILITY<6)		///counter for long press MENU button
//			{
//			countUTILITY++;	
//			}
//		else
//			{
//			countUTILITY = 0;	
//			}
		}	
		
		
		
		
		
		
		
		
		
		
		
		
	HAL_TIM_IRQHandler(&htim2);	
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
	}



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
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



