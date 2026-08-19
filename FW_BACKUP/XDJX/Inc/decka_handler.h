/////////////////////////////////////////////////////////////////////////////////////////////////
//  DECK A handler
//	processing of buttons, jog, pitch
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

	if(CheckRXAcrc()==1)
		{
		uint32_t ptch;
		if((deckRbuf[1]&0x01) && PLAY_BTN_pressed[dkA]==0)										///////////PLAY button
			{
			if(lock_control[dkA]==0)	
				{
				if(CUE_BTN_pressed[dkA]==0)
					{
					if(play_enable[dkA])
						{
						play_enable[dkA] = 0;		
						change_speed[dkA] = NEED_DOWN;
						//change masterdeck	
						if(play_enable[dkB] && masterdeck!=dkB)
							{
							masterdeck = dkB;
							tempo_need_update[dkA] = 2;
							tempo_need_update[dkB] = 2;	
							}
						}
					else
						{
						if(SLIPEN[dkA])					//SLIP MODE ENABLE
							{	
							play_adr[dkA] = slip_pl_adr[dkA];	
							change_speed[dkA] = NO_CHANGE;
							slip_pl_enable[dkA] = 1;	
							}
						else if(CUE_ADR[dkA]!=(play_adr[dkA]/294) && (deckRbuf[2]&0x80)==0)			//when playback starts from any adress and touch disable
							{
							change_speed[dkA] = NEED_UP;
							}
						else																//when playback starts from CUE adress
							{
							change_speed[dkA] = NO_CHANGE;	
							}
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
				else
					{
					keep_to_play[dkA] = 1;	
					//change masterdeck	
					if(play_enable[dkB]==0 && masterdeck!=dkA)
						{
						masterdeck = dkA;
						tempo_need_update[dkA] = 2;
						tempo_need_update[dkB] = 2;	
						}		
					}
				}
			PLAY_BTN_pressed[dkA] = 1;	
			}
		else if((deckRbuf[1]&0x01)==0 && PLAY_BTN_pressed[dkA]==1)
			{
			PLAY_BTN_pressed[dkA] = 0;	
			}
		else if((deckRbuf[1]&0x02) && CUE_BTN_pressed[dkA]==0)										///////////CUE button
			{
			if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
				{	
				masterdeck = dkA;
				tempo_need_update[dkA] = 2;
				tempo_need_update[dkB] = 2;	
				}
			else	
				{
				if(lock_control[dkA]==0)	
					{					
					if(play_enable[dkA] && ((deckRbuf[2]&0x80)==0))								//return to CUE, when track playing				play && touch disable
						{
						pitch[dkA] = 0;	
						play_enable[dkA] = 0;		
						if(SLIPEN[dkA])					//SLIP MODE ENABLE
							{	
							slip_pl_enable[dkA] = 0;		
							slip_pl_adr[dkA] = 294*CUE_ADR[dkA];	
							}	
						CUE_OPERATION[dkA] = CUE_NEED_CALL;			
						}	
					else if((play_enable[dkA]==0) && (CUE_ADR[dkA]!=(play_adr[dkA]/294)))			//Set new CUE, when track stopped		
						{
						LOOP_OUT[dkA] = 0;	
						CUE_OPERATION[dkA] = CUE_NEED_SET;						
						}
					else if((play_enable[dkA]==0) && (CUE_ADR[dkA]==(play_adr[dkA]/294)))				//return to CUE adress, when track stopped
						{
						change_speed[dkA] = NO_CHANGE;	
						//play_adr = 294*CUE_ADR;		
						if(SLIPEN[dkA])					//SLIP MODE ENABLE
							{			
							slip_pl_adr[dkA] = play_adr[dkA];
							slip_pl_enable[dkA] = 1;	
							}	
						play_enable[dkA] = 1;							
						}
					else if(play_enable[dkA] && ((deckRbuf[2]&0x80)!=0) && (deckTbuf[2][0]&0x08))				//Set new CUE, when track played and press jog and JOG in Vinyl MODE
						{
						LOOP_OUT[dkA] = 0;	
						CUE_OPERATION[dkA] = CUE_NEED_SET;	
						play_enable[dkA] = 0;							
						}
					//change masterdeck	
					if(play_enable[dkB] && masterdeck!=dkB)
						{
						masterdeck = dkB;
						tempo_need_update[dkA] = 2;
						tempo_need_update[dkB] = 2;	
						}
					}
				}
			CUE_BTN_pressed[dkA] = 1;	
			}
		else if((deckRbuf[1]&0x02)==0 && CUE_BTN_pressed[dkA]==1)
			{
			if((GPIOB->IDR&0x00000200)!=0)						//SHIFT BUTTON not pressed
				{	
				if(lock_control[dkA]==0)	
					{	
					if(keep_to_play[dkA]==0)		//button play not pressed	
						{
						play_enable[dkA] = 0;
						pitch[dkA] = 0;		
						slip_pl_enable[dkA] = 0;	
						play_adr[dkA] = 294*CUE_ADR[dkA];	
						if(SLIPEN[dkA])					//SLIP MODE ENABLE
							{	
							slip_pl_adr[dkA] = play_adr[dkA];	
							}
			//			if((Tbuffer[19]&0x20)==0)   //CDJ mode
			//				{
			//				pitch = 0;	
			//				}
						}
					else
						{
						keep_to_play[dkA] = 0;	
						}
					offset_adress[dkA] = 0;																///	   temporary operation	
					offset_adressBIG[dkA] = 0;		
					}					
				}
			CUE_BTN_pressed[dkA] = 0;	
			}
		else if((deckRbuf[1]&0x80) && RELOOP_BTN_pressed[dkA]==0)										///////////RELOOP button
			{
			if(lock_control[dkA]==0)	
				{		
				if(loop_active[dkA])
					{
					loop_act_gui[dkA] = 0;	
					loop_active[dkA] = 0;	
					loopmode_need_update[dkA] = 1;	
					deckTbuf[2][0]|=0x10;			//loop leds on
					deckTbuf[3][0]|=0x08;			//
					}
				else if(loop_active[dkA]==0 && CUE_ADR[dkA]<LOOP_OUT[dkA])
					{	
					loop_act_gui[dkA] = 1;			
					loop_active[dkA] = 1;	
					loopmode_need_update[dkA] = 1;	
					}
				if(dSHOW==WAVEFORM)							//Redraw cue on dynamic waveform
					{
					forcibly_redraw[dkA] = 1;
					}		
				}						
			RELOOP_BTN_pressed[dkA] = 1;	
			}
		else if((deckRbuf[1]&0x80)==0 && RELOOP_BTN_pressed[dkA]==1)
			{
			RELOOP_BTN_pressed[dkA] = 0;	
			}		
		else if((deckRbuf[1]&0x20) && LP4_BTN_pressed[dkA]==0)										///////////LP4 button
			{
			if(lock_control[dkA]==0)	
				{	
				if(loop_active[dkA])
					{
					if(loop_lenght[dkA]>0)
						{
						loop_lenght[dkA]--;
						}						
					}
				else if((play_enable[dkA]==1) && (CUE_ADR[dkA]!=(play_adr[dkA]/294)) && loop_active[dkA]==0)			//Set new CUE, when track play		
					{
					LOOP_OUT[dkA] = 0;	
					CUE_OPERATION[dkA] = CUE_NEED_SET;
					loop_lenght[dkA] = 8;	
					}

				if(loop_active[dkA]==0)
					{
					if(UT_SET[QUANTIZE]==1 && dSHOW==WAVEFORM)
						{
//						if(((play_adr/294)>(BEATGRID[dkA][bars[dkA]-1]+((BEATGRID[dkA][bars[dkA]] - BEATGRID[dkA][bars[dkA]-1])/2))) || bars[dkA]==0)	
//							{
//							LOOP_OUT = BEATGRID[dkA][bars[dkA]];								//next bar >> |
//							}
//						else
//							{
//							if(CUE_ADR==BEATGRID[dkA][bars[dkA]-1])
//								{
//								LOOP_OUT = BEATGRID[dkA][bars[dkA]];								//next bar >> |	
//								}
//							else	
//								{
//								LOOP_OUT = BEATGRID[dkA][bars[dkA]-1];							//previous bar <<	|
//								}
//							}	
						}
					else
						{
						//LOOP_OUT = play_adr/294;
						}	
					if(originalBPM[dkA]!=0)
						{						
						LOOP_OUT[dkA] = (play_adr[dkA]/294)+(loop150[loop_lenght[dkA]]/originalBPM[dkA]);	//4 bits
						}
					loop_act_gui[dkA] = 1;						
					loop_active[dkA] = 1;	
					loopmode_need_update[dkA] = 1;	
					}
				else
					{
					if(originalBPM[dkA]!=0)
						{						
						LOOP_OUT[dkA] = CUE_ADR[dkA]+(loop150[loop_lenght[dkA]]/originalBPM[dkA]);	//4 bits
						}	
					CUE_OPERATION[dkA] = CUE_NEED_CALL;	
					}					
					
				}	
			LP4_BTN_pressed[dkA] = 1;
			}
		else if((deckRbuf[1]&0x20)==0 && LP4_BTN_pressed[dkA]==1)
			{
			LP4_BTN_pressed[dkA] = 0;	
			}		
		else if((deckRbuf[1]&0x40) && LP8_BTN_pressed[dkA]==0)										///////////LP8 button
			{
			if(lock_control[dkA]==0)	
				{	
				if(loop_active[dkA])
					{
					if(loop_lenght[dkA]<12)
						{
						loop_lenght[dkA]++;
						}						
					}
				else if((play_enable[dkA]==1) && (CUE_ADR[dkA]!=(play_adr[dkA]/294)) && loop_active[dkA]==0)			//Set new CUE, when track play		
					{
					LOOP_OUT[dkA] = 0;	
					CUE_OPERATION[dkA] = CUE_NEED_SET;
					loop_lenght[dkA] = 9;	
					}

				if(loop_active[dkA]==0)
					{
					if(UT_SET[QUANTIZE]==1 && dSHOW==WAVEFORM)
						{
//						if(((play_adr/294)>(BEATGRID[dkA][bars[dkA]-1]+((BEATGRID[dkA][bars[dkA]] - BEATGRID[dkA][bars[dkA]-1])/2))) || bars[dkA]==0)	
//							{
//							LOOP_OUT = BEATGRID[dkA][bars[dkA]];								//next bar >> |
//							}
//						else
//							{
//							if(CUE_ADR==BEATGRID[dkA][bars[dkA]-1])
//								{
//								LOOP_OUT = BEATGRID[dkA][bars[dkA]];								//next bar >> |	
//								}
//							else	
//								{
//								LOOP_OUT = BEATGRID[dkA][bars[dkA]-1];							//previous bar <<	|
//								}
//							}	
						}
					else
						{
						//LOOP_OUT = play_adr/294;
						}	
					if(originalBPM[dkA]!=0)
						{						
						LOOP_OUT[dkA] = (play_adr[dkA]/294)+(loop150[loop_lenght[dkA]]/originalBPM[dkA]);	//8 bits
						}
					loop_act_gui[dkA] = 1;		
					loop_active[dkA] = 1;	
					loopmode_need_update[dkA] = 1;	
					}
				else
					{
					LOOP_OUT[dkA] = CUE_ADR[dkA]+(loop150[loop_lenght[dkA]]/originalBPM[dkA]);	//8 bits
					}					
				}	
			LP8_BTN_pressed[dkA] = 1;
			}
		else if((deckRbuf[1]&0x40)==0 && LP8_BTN_pressed[dkA]==1)
			{
			LP8_BTN_pressed[dkA] = 0;	
			}				
		#include "pads_a.h"	
			
		if((deckRbuf[1]&0x04) && RVRS_BTN_pressed[dkA]==0)					///////////reverse switch position
			{
			if(RVRSEN[dkA])
				{
				if(SLIPEN[dkA])					//SLIP MODE ENABLE
					{	
					play_adr[dkA] = slip_pl_adr[dkA];	
					}		
				if(keep_slip[dkA])
					{
					keep_slip[dkA] = 0;	
					}
				deckTbuf[1][0]|= 0x08;				//disable red led reverse
				RVRSEN[dkA] = 0;
				}	
			else
				{
				if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
					{
					RVRS_BTN_pressed[dkA] = 2;	
					}	
				else
					{
					if(SLIPEN[dkA])			//SLIP MODE ENABLE
						{
						keep_slip[dkA] = 1;	
						}
					else
						{
						SLIPEN[dkA] = 1;	
						if(play_enable[dkA])
							{
							slip_pl_enable[dkA] = 1;	
							}
						slip_pl_adr[dkA] = play_adr[dkA]; 	
						}			
					RVRS_BTN_pressed[dkA] = 1;		
					}				
				deckTbuf[1][0]&= 0xF7; //enable red led reverse
				RVRSEN[dkA] = 1;						
				}
			}
		else if((deckRbuf[1]&0x04)==0 && RVRS_BTN_pressed[dkA]>0)
			{			
			if(RVRS_BTN_pressed[dkA]==1)
				{
				if(SLIPEN[dkA])					//SLIP MODE ENABLE
					{	
					play_adr[dkA] = slip_pl_adr[dkA];	
					}		
				if(keep_slip[dkA])
					{
					keep_slip[dkA] = 0;	
					}
				else
					{
					slip_pl_enable[dkA] = 0;	
					SLIPEN[dkA] = 0;	
					}
				deckTbuf[1][0]|= 0x08;				//disable red led reverse
				RVRSEN[dkA] = 0;						
				}
			RVRS_BTN_pressed[dkA] = 0;		
			}			
		
///////////////////////////////////////////////////JOG MECHANICAL PROCESS///////////////////////////////////////////////	
		if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
			{
			if(ROTENdkA)					//rotation detect
				{
				ptch = (256*deckRbuf[5]+deckRbuf[6]);
				if(ptch<86)
					{
					ptch = 86;	
					}
				seek_pos[dkA] = 5418000/ptch;
				if(deckRbuf[4]&0x80)				//foward/reverse rotation
					{
					need_seek[dkA] = 2;				
					}
				else
					{
					need_seek[dkA] = 1;
					}
				}	
			}
		else
			{	
			if(inertial_rotation[dkA])
				{
				if((RVRSEN[dkA]==0 && (ROTDISdkA || (((deckRbuf[4]&0x80)==0) && pitch[dkA]<potenciometr_tempo[dkA]))) || 
					((RVRSEN[dkA]) && ((ROTDISdkA && (deckRbuf[4]&0x80)==0) || ((deckRbuf[4]&0x80) && pitch[dkA]<potenciometr_tempo[dkA]))))		//if rotation foward and stopped	
					{
					inertial_rotation[dkA] = 0;	
					}
				}				
			if(play_enable[dkA] || (need_call_to_cue[dkA]==3 && (ROTDISdkA || (deckRbuf[2]&0x80)!=0)))			//touch disable && rotation disable or play enable
				{
				need_call_to_cue[dkA] = 0;	
				}	
			else if((deckRbuf[2]&0x80)==0 && need_call_to_cue[dkA]==2)
				{
				pitch[dkA] = 0;	
				play_adr[dkA] = 294*CUE_ADR[dkA];	
				need_call_to_cue[dkA] = 3;	
				}		
				
			if(((deckRbuf[2]&0x80)!=0 || (play_enable[dkA]==0 && (CUE_ADR[dkA]!=(play_adr[dkA]/294))) || inertial_rotation[dkA]) && (deckTbuf[2][0]&0x08))				/////////////(touch enable	|| play_enable[dkA]==0) && Vinyl mode enable
				{
				slip_pitch[dkA] = potenciometr_tempo[dkA];
				if(JOG_PRESSED[dkA]==0)
					{
					if((deckRbuf[2]&0x80)!=0) 			//touch enable
						{
						change_speed[dkA] = NEED_DOWN;	
						JOG_PRESSED[dkA] = 2;	
						}
					else
						{
						JOG_PRESSED[dkA] = 1;
						}				
					}	
				if(play_enable[dkA]==0)	
					{
					if(((deckRbuf[2]&0x80)!=0) && (CUE_ADR[dkA]==(play_adr[dkA]/294)) && need_call_to_cue[dkA]==0)				//touch enable + play _enable==0 + CUE_ADR==(play_adr/294)
						{
						need_call_to_cue[dkA] = 1;	
						}	
					else if(need_call_to_cue[dkA]==1 && (deckRbuf[2]&0x80)==0)			//touch disable	_/
						{
						need_call_to_cue[dkA] = 2;	
						}					
					}
				if(ROTENdkA)					//rotation detect
					{
					if(need_call_to_cue[dkA]<2)
						{
						change_speed[dkA] = NO_CHANGE;	
						
						if(((deckRbuf[4]&0x80)==0) && end_track[dkA])				//foward rotation + end_track[dkA]
							{
							pitch[dkA] = 0;
							change_speed[dkA] = NO_CHANGE;	
							}
						else
							{
							ptch = (256*deckRbuf[5]+deckRbuf[6]);
							if(ptch<86)
								{
								ptch = 86;	
								}
							ptch = 5574324/ptch;
							pitch[dkA] = ptch;						
							}
						slip_pitch[dkA] = potenciometr_tempo[dkA]; 	
						inertial_rotation[dkA] = 1;												
						}						
					}		
				else if(change_speed[dkA]==NO_CHANGE) 
					{			
					pitch[dkA] = 0;	
					}

				if(change_speed[dkA]==NO_CHANGE)
					{	
					if(deckRbuf[4]&0x80)				//foward/reverse rotation
						{
						rvrs[dkA] = 1;
						}
					else
						{
						rvrs[dkA] = 0;	
						}		
					}
				else
					{
					if(RVRSEN[dkA])					//reverse diode enable
						{
						rvrs[dkA] = 1;
						}
					else
						{
						rvrs[dkA] = 0;	
						}	
					}
				deckTbuf[5][0]|=0x10;	//touch enable circle on display
				//change masterdeck	
				if(play_enable[dkB] && masterdeck!=dkB)
					{
					masterdeck = dkB;
					tempo_need_update[dkA] = 2;
					tempo_need_update[dkB] = 2;	
					}	
				}		
			else if(ROTDISdkA && (deckRbuf[2]&0x80)==0)				///////////////////////touch disable and rotation disable
				{			
				slip_pitch[dkA] = potenciometr_tempo[dkA];
				if(JOG_PRESSED[dkA]>0) //jog PRESSED -> UNPRESSED
					{
					if(SLIPEN[dkA])					//SLIP MODE ENABLE
						{	
						change_speed[dkA] = NO_CHANGE;	
						}
					else if(JOG_PRESSED[dkA]==2 && play_enable[dkA])
						{
						change_speed[dkA] = NEED_UP;	
						}					
					JOG_PRESSED[dkA] = 0;	
					}
				if(play_enable[dkA])
					{	
					if(end_track[dkA] && RVRSEN[dkA]==0)					//stop on end (to remove noise at the end of the track)
						{
						change_speed[dkA] = NO_CHANGE;	
						pitch[dkA] = 0;	
						}	
					else if(change_speed[dkA]==NO_CHANGE)
						{
						pitch[dkA] = potenciometr_tempo[dkA];	
						}
					if(RVRSEN[dkA])					//reverse diode enable
						{
						rvrs[dkA] = 1;
						}
					else
						{
						rvrs[dkA] = 0;	
						}		
					}
				if(deckTbuf[5][0]&0x10)					//jog UNPRESSED
					{
					if(SLIPEN[dkA])					//SLIP MODE ENABLE
						{	
						play_adr[dkA] = slip_pl_adr[dkA];	
						}	
					deckTbuf[5][0]&=0xEF;				//disable touch circle on display
					}	
				}
			else if(ROTENdkA && play_enable[dkA])						//rotation detected			(pitch bend)	
				{
				if(end_track[dkA]==0)
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
					ptch = 120000/ptch;				//150000
						
					if(ptch>4225)
						{
						ptch = 4225;	
						}	
					if((((deckRbuf[4]&0x80)==0) && (RVRSEN[dkA]==0)) || ((deckRbuf[4]&0x80) && RVRSEN[dkA]))		//foward rotation and reverse off OR reverse rotation and reverse on (pitch bend)			
						{
						ptch+= potenciometr_tempo[dkA];
						if(ptch>20000)
							{
							ptch = 20000;	
							}
						pitch[dkA] = ptch;
						}
					else if(((deckRbuf[4]&0x80) && (RVRSEN[dkA]==0)) || (((deckRbuf[4]&0x80)==0) && RVRSEN[dkA]))	 //reverse rotation and reverse off OR foward rotation and reverse on(pitch bend)	
						{	
						if(ptch<potenciometr_tempo[dkA])
							{
							pitch[dkA] = potenciometr_tempo[dkA] - ptch;
							}
						else
							{
							pitch[dkA] = 0;	
							}
						}		
					}
				else
					{
					pitch[dkA] = 0;		
					}			
				if(RVRSEN[dkA])					//reverse diode enable
					{
					rvrs[dkA] = 1;
					}
				else
					{
					rvrs[dkA] = 0;	
					}	
				if(deckTbuf[5][0]&0x10)					//jog UNPRESSED
					{
					if(SLIPEN[dkA])					//SLIP MODE ENABLE
						{	
						play_adr[dkA] = slip_pl_adr[dkA];	
						}	
					deckTbuf[5][0]&=0xEF;				//disable touch circle on display	
					}	
				else
					{
					slip_pitch[dkA] = pitch[dkA];		
					}
				}	
			}			
				
			
			
			
	


			
		if((TXpage%2)==0)		// 1/2 cycles for this deck
			{	
			if((deckTbuf[4][0]&0x10)==0)				//TEMPRO RESET ON
				{
				potenciometr_tempo[dkA] = 10000;	
				}
			else				//////////////////////////////////////////TEMPO CALCULATION
				{	
				pot10b[dkA] = 0x100*deckRbuf[2] + deckRbuf[3]; 	
				pot10b[dkA]&= 0x03FF;
				if(pot10b[dkA]==512)	
					{
					potenciometr_tempo[dkA] = 10000;	
					}
				else if(pot10b[dkA]>512)					/////pitch>0%
					{
					potwd[dkA] = pot10b[dkA]-512;	
					if(tempo_range[dkA]==0)										//	6%
						{
						potwd[dkA]*= 2409;	
						potenciometr_tempo[dkA] = potwd[dkA]>>12; 	
						potenciometr_tempo[dkA]*= 2;
						}	
					else if(tempo_range[dkA]==1)										//	10%	
						{
						potwd[dkA]*= 1608;	
						potenciometr_tempo[dkA] = potwd[dkA]>>12; 	
						potenciometr_tempo[dkA]*= 5;	
						}	
					else if(tempo_range[dkA]==2)										//	16%	
						{
						potwd[dkA]*= 2569;	
						potenciometr_tempo[dkA] = potwd[dkA]>>12; 	
						potenciometr_tempo[dkA]*= 5;
						}	
					else																		//	WIDE	
						{
						potwd[dkA]*= 1608;	
						potenciometr_tempo[dkA] = potwd[dkA]>>12; 	
						potenciometr_tempo[dkA]*= 50;	
						}	
					potenciometr_tempo[dkA]+= 10000;	
					}
				else				/////pitch<0%
					{
					potwd[dkA] = 511-pot10b[dkA];		
					if(tempo_range[dkA]==0)										//	6%	
						{	
						potwd[dkA]*= 2409;	
						potenciometr_tempo[dkA] = potwd[dkA]>>12; 	
						potenciometr_tempo[dkA]*= 2;	
						}	
					else if(tempo_range[dkA]==1)										//	10%	
						{	
						potwd[dkA]*= 1608;	
						potenciometr_tempo[dkA] = potwd[dkA]>>12; 	
						potenciometr_tempo[dkA]*= 5;	
						}
					else if(tempo_range[dkA]==2)										//	16%	
						{	
						potwd[dkA]*= 2569;	
						potenciometr_tempo[dkA] = potwd[dkA]>>12; 	
						potenciometr_tempo[dkA]*= 5;
						}	
					else																			//	WIDE
						{	
						potwd[dkA]*= 1608;	
						potenciometr_tempo[dkA] = potwd[dkA]>>12; 	
						potenciometr_tempo[dkA]*= 50;
						}
					potenciometr_tempo[dkA] = 10000 - potenciometr_tempo[dkA];	
					}				
				}
			if(prev_potenciometr_tempo[dkA] != potenciometr_tempo[dkA])
				{
				prev_potenciometr_tempo[dkA] = potenciometr_tempo[dkA];	
				tempo_need_update[dkA] = 1;
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

			}
		else if(TXpage==3)
			{	

			}	
		else if(TXpage==4)
			{
//				if((Rbuffer[16]&0x10) && SEARCH_FF_BUTTON_pressed==0) 							///////////SEARCH FF>> Button
//					{
//					if(lock_control[dkA]==0)	
//						{	
//						if(play_enable[dkA] & play_adr<(all_long[dkA]+100000))	
//							{
//							//SEEK_AUDIOFRAME(play_adr+100000);	
//							}
//						else if(play_enable[dkA]==0 & play_adr/294<(all_long[dkA]+1))
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
//					if(lock_control[dkA]==0)	
//						{		
//						if(play_enable[dkA] & play_adr>100000)	
//							{
//							//SEEK_AUDIOFRAME(play_adr-100000);
//							}
//						else if(play_enable[dkA]==0 & play_adr>294)
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

			if(track_play_now[dkA]!=0)						//////////////////////////////LEDS/////////////////////////
				{					
				deckTbuf[0][0]|=0x10;				//CUE led on
					
				if(play_enable[dkA])
					{
					deckTbuf[0][0]|=0x18;	//PLAY and CUE led on
					}
				else											//Play led blink
					{	
					if(TIM_PLAY_LED[dkA])	
						{
						deckTbuf[0][0]|=0x08;	
						}
					else
						{
						deckTbuf[0][0]&=0xF7;
						}
					if(TIM_CUE_LED[dkA])	
						{
						deckTbuf[0][0]|=0x10;
						}
					else if(CUE_ADR[dkA]!=play_adr[dkA]/294)  
						{
						deckTbuf[0][0]&=0xEF;	
						}	
					}
						
				if(loop_active[dkA] || CUE_ADR[dkA]<LOOP_OUT[dkA])
					{	
					deckTbuf[3][0]|=0x10;							//RELOOP EXIT LED ON
					}
				else
					{
					deckTbuf[3][0]&=0xEF;							//RELOOP EXIT LED OFF	
					}
				}
			}		
		else if(TXpage==5)
			{				
			if((deckRbuf[1]&0x10) && JOG_MODE_BTN_pressed[dkA]==0) 							///////////Jog Mode button
				{
				if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
					{	
					if(REMAIN_ENABLE[dkA])
						{
						REMAIN_ENABLE[dkA] = 0;	
						}
					else
						{
						REMAIN_ENABLE[dkA] = 1;	
						}
					time_mode_need_update[dkA] = 1;	
					}
				else
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
					}
				JOG_MODE_BTN_pressed[dkA] = 1;	
				}
			else if((deckRbuf[1]&0x10)==0 && JOG_MODE_BTN_pressed[dkA]==1)	
				{
				JOG_MODE_BTN_pressed[dkA] = 0;	
				}
			else if((deckRbuf[2]&0x40) && TEMPO_BTN_pressed[dkA]==0) 							///////////TEMPO Button
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
					if(tempo_range[dkA]<3)
						{
						tempo_range[dkA]++;
						}
					else
						{
						tempo_range[dkA] = 0;	
						}
					tempo_range_need_update[dkA] = 1;
					}
				TEMPO_BTN_pressed[dkA] = 1;	
				}
			else if((deckRbuf[2]&0x40)==0 && TEMPO_BTN_pressed[dkA]==1)	
				{
				TEMPO_BTN_pressed[dkA] = 0;	
				}	
			else if((deckRbuf[2]&0x20) && CALL_NEXT_BTN_pressed[dkA]==0) 							///////////CALL NEXT Button	>
				{
				if(lock_control[dkA]==0)	
					{	
					if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
						{
						CUE_OPERATION[dkA] = MEMORY_NEED_NEXT_SET;	
						}
					else		//beat jump
						{
						if(originalBPM[dkA]!=0xFFFF  && originalBPM[dkA]!=0)
							{
							seek_pos[dkA] = 2116800000/originalBPM[dkA];	//44100*60*100*8	
							need_seek[dkA] = 1;	
							}								
						}
					}
				CALL_NEXT_BTN_pressed[dkA] = 1;	
				}
			else if((deckRbuf[2]&0x20)==0 && CALL_NEXT_BTN_pressed[dkA]==1)	
				{
				CALL_NEXT_BTN_pressed[dkA] = 0;	
				}
			else if((deckRbuf[2]&0x10) && CALL_PREV_BTN_pressed[dkA]==0) 							///////////CALL PREVIOUS Button <
				{
				if(lock_control[dkA]==0)	
					{		
					if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
						{	
						CUE_OPERATION[dkA] = MEMORY_NEED_PREVIOUS_SET;
						}	
					else		//beat jump
						{
						if(originalBPM[dkA]!=0xFFFF  && originalBPM[dkA]!=0)
							{
							seek_pos[dkA] = 2116800000/originalBPM[dkA];	//44100*60*100*8	
							need_seek[dkA] = 2;	
							}	
						}	
					}
				CALL_PREV_BTN_pressed[dkA] = 1;	
				}
			else if((deckRbuf[2]&0x10)==0 && CALL_PREV_BTN_pressed[dkA]==1)	
				{
				CALL_PREV_BTN_pressed[dkA] = 0;	
				}

			if((deckRbuf[1]&0x08) && SLIP_BTN_pressed[dkA]==0) 							///////////SLIP MODE Button
				{
				if(SLIPEN[dkA])					//ON_SLIP_MODE => OFF_SLIP_MODE
					{
					if(keep_slip[dkA])
						{
						keep_slip[dkA] = 0;	
						}
					else
						{
						slip_pl_enable[dkA] = 0;	
						SLIPEN[dkA] = 0;	
						deckTbuf[1][0]|=0x10;		//led on		
						}																
					}	
				else												//OFF_SLIP_MODE => ON_SLIP_MODE
					{	
					SLIPEN[dkA] = 1;
					deckTbuf[1][0]&=0xEF;		//led off	
					if(play_enable[dkA])
						{
						slip_pl_enable[dkA] = 1;	
						}
					slip_pl_adr[dkA] = play_adr[dkA]; 	
					}
				SLIP_BTN_pressed[dkA] = 1;	
				}
			else if((deckRbuf[1]&0x08)==0 && SLIP_BTN_pressed[dkA]==1)	
				{
				SLIP_BTN_pressed[dkA] = 0;	
				}		
			}			
		
			
		if(load_animation_en[dkA])
			{
			deckTbuf[TXpage][4] = 137;				//command load animation	
			}
		else if(track_play_now[dkA]==0)
			{
			deckTbuf[TXpage][4] = 135;
			deckTbuf[TXpage][5] = 85;	
			deckTbuf[TXpage][6] = 85; 	//cue none
			deckTbuf[0][0]&=0xE7;		//PLAY & CUE leds off
			deckTbuf[5][0]&=0xEF;				//disable touch circle on display		
			}
		else
			{
			deckTbuf[TXpage][6] = RED_CRCL_CUE_ADR[dkA]; 	//cue		
			if(SLIPEN[dkA])					//SLIP MODE ENABLE
				{	
				deckTbuf[TXpage][5] = (1000*(((slip_pl_adr[dkA]/588)%135)+1)/1589);							//slip
				}
			else
				{
				deckTbuf[TXpage][5] = 85;	
				}				
			deckTbuf[TXpage][4] = (play_adr[dkA]/588)%135;
			}
		}	


	////////////////////////////////////////////////from 1000hz (400) timer
	
	if(change_speed[dkA]==NEED_UP)
		{
		if(end_track[dkA])
			{
			change_speed[dkA] = NO_CHANGE;
			pitch[dkA] = 0;			
			}
		else
			{
			if(pitch[dkA]<potenciometr_tempo[dkA]-acceleration_UP)
				{
				pitch[dkA]+=acceleration_UP;	
				}
			else
				{
				change_speed[dkA] = NO_CHANGE;
				pitch[dkA] = potenciometr_tempo[dkA];	
				}		
			}
		}
	else if(change_speed[dkA]==NEED_DOWN)
		{
		if(end_track[dkA])
			{
			change_speed[dkA] = NO_CHANGE;
			pitch[dkA] = 0;			
			}
		else	
			{
			if(pitch[dkA]>acceleration_DOWN)
				{
				pitch[dkA]-=acceleration_DOWN;	
				}
			else
				{
				change_speed[dkA] = NO_CHANGE;
				pitch[dkA] = 0;	
				}		
			}
		}




	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



