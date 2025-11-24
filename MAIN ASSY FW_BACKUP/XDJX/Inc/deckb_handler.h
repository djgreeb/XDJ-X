/////////////////////////////////////////////////////////////////////////////////////////////////
//  DECK B handler
//	processing of buttons, jog, pitch
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////



	if(CheckRXBcrc()==1)
		{
		uint32_t ptch;
		if((deckRbuf[9]&0x01) && PLAY_BTN_pressed[dkB]==0)										///////////PLAY button
			{
			if(lock_control[dkB]==0)	
				{
				if(CUE_BTN_pressed[dkB]==0)
					{
					if(play_enable[dkB])
						{
						play_enable[dkB] = 0;		
						change_speed[dkB] = NEED_DOWN;
						//change masterdeck	
						if(play_enable[dkA] && masterdeck!=dkA)
							{
							masterdeck = dkA;
							tempo_need_update[dkA] = 2;
							tempo_need_update[dkB] = 2;	
							}		
						}
					else
						{
						if(SLIPEN[dkB])					//SLIP MODE ENABLE
							{	
							play_adr[dkB] = slip_pl_adr[dkB];	
							change_speed[dkB] = NO_CHANGE;
							slip_pl_enable[dkB] = 1;	
							}
						else if(CUE_ADR[dkB]!=(play_adr[dkB]/294) && (deckRbuf[10]&0x80)==0)			//when playback starts from any adress and touch disable
							{
							change_speed[dkB] = NEED_UP;
							}
						else																//when playback starts from CUE adress
							{
							change_speed[dkB] = NO_CHANGE;	
							}
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
				else
					{
					keep_to_play[dkB] = 1;	
					//change masterdeck	
					if(play_enable[dkA]==0 && masterdeck!=dkB)
						{
						masterdeck = dkB;
						tempo_need_update[dkA] = 2;
						tempo_need_update[dkB] = 2;	
						}		
					}
				}
			PLAY_BTN_pressed[dkB] = 1;	
			}
		else if((deckRbuf[9]&0x01)==0 && PLAY_BTN_pressed[dkB]==1)
			{
			PLAY_BTN_pressed[dkB] = 0;	
			}
		else if((deckRbuf[9]&0x02) && CUE_BTN_pressed[dkB]==0)										///////////CUE button
			{
			if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
				{	
				masterdeck = dkB;
				tempo_need_update[dkA] = 2;
				tempo_need_update[dkB] = 2;	
				}
			else
				{
				if(lock_control[dkB]==0)	
					{	
					if(play_enable[dkB] && ((deckRbuf[10]&0x80)==0))								//return to CUE, when track playing				play && touch disable
						{
						pitch[dkB] = 0;	
						play_enable[dkB] = 0;		
						if(SLIPEN[dkB])					//SLIP MODE ENABLE
							{	
							slip_pl_enable[dkB] = 0;		
							slip_pl_adr[dkB] = 294*CUE_ADR[dkB];	
							}	
						CUE_OPERATION[dkB] = CUE_NEED_CALL;			
						}	
					else if((play_enable[dkB]==0) && (CUE_ADR[dkB]!=(play_adr[dkB]/294)))			//Set new CUE, when track stopped		
						{
						LOOP_OUT[dkB] = 0;	
						CUE_OPERATION[dkB] = CUE_NEED_SET;						
						}
					else if((play_enable[dkB]==0) && (CUE_ADR[dkB]==(play_adr[dkB]/294)))				//return to CUE adress, when track stopped
						{
						change_speed[dkB] = NO_CHANGE;	
						//play_adr = 294*CUE_ADR;		
						if(SLIPEN[dkB])					//SLIP MODE ENABLE
							{			
							slip_pl_adr[dkB] = play_adr[dkB];
							slip_pl_enable[dkB] = 1;	
							}	
						play_enable[dkB] = 1;							
						}
					else if(play_enable[dkB] && ((deckRbuf[10]&0x80)!=0) && (deckTbuf[2][8]&0x08))				//Set new CUE, when track played and press jog and JOG in Vinyl MODE
						{
						LOOP_OUT[dkB] = 0;	
						CUE_OPERATION[dkB] = CUE_NEED_SET;	
						play_enable[dkB] = 0;							
						}
					//change masterdeck	
					if(play_enable[dkA] && masterdeck!=dkA)
						{
						masterdeck = dkA;
						tempo_need_update[dkA] = 2;
						tempo_need_update[dkB] = 2;	
						}				
					}
				}	
			CUE_BTN_pressed[dkB] = 1;	
			}
		else if((deckRbuf[9]&0x02)==0 && CUE_BTN_pressed[dkB]==1)
			{
			if((GPIOB->IDR&0x00000200)!=0)						//SHIFT BUTTON not pressed
				{		
				if(lock_control[dkB]==0)	
					{	
					if(keep_to_play[dkB]==0)		//button play not pressed	
						{
						play_enable[dkB] = 0;
						pitch[dkB] = 0;		
						slip_pl_enable[dkB] = 0;	
						play_adr[dkB] = 294*CUE_ADR[dkB];	
						if(SLIPEN[dkB])					//SLIP MODE ENABLE
							{	
							slip_pl_adr[dkB] = play_adr[dkB];	
							}
			//			if((Tbuffer[19]&0x20)==0)   //CDJ mode
			//				{
			//				pitch = 0;	
			//				}
						}
					else
						{
						keep_to_play[dkB] = 0;	
						}
					offset_adress[dkB] = 0;																///	   temporary operation	
					offset_adressBIG[dkB] = 0;		
					}
				}
			CUE_BTN_pressed[dkB] = 0;	
			}
		else if((deckRbuf[9]&0x80) && RELOOP_BTN_pressed[dkB]==0)										///////////RELOOP button
			{
			if(lock_control[dkB]==0)	
				{		
				if(loop_active[dkB])
					{
					loop_act_gui[dkB] = 0;	
					loop_active[dkB] = 0;	
					loopmode_need_update[dkB] = 1;	
					deckTbuf[2][8]|=0x10;			//loop leds on
					deckTbuf[3][8]|=0x08;			//
					}
				else if(loop_active[dkB]==0 && CUE_ADR[dkB]<LOOP_OUT[dkB])
					{	
					loop_act_gui[dkB] = 1;			
					loop_active[dkB] = 1;	
					loopmode_need_update[dkB] = 1;	
					}
				if(dSHOW==WAVEFORM)							//Redraw cue on dynamic waveform
					{
					forcibly_redraw[dkB] = 1;
					}		
				}						
			RELOOP_BTN_pressed[dkB] = 1;	
			}
		else if((deckRbuf[9]&0x80)==0 && RELOOP_BTN_pressed[dkB]==1)
			{
			RELOOP_BTN_pressed[dkB] = 0;	
			}		
		else if((deckRbuf[9]&0x20) && LP4_BTN_pressed[dkB]==0)										///////////LP4 button
			{
			if(lock_control[dkB]==0)	
				{	
				if(loop_active[dkB])
					{
					if(loop_lenght[dkB]>0)
						{
						loop_lenght[dkB]--;
						}						
					}
				else if((play_enable[dkB]==1) && (CUE_ADR[dkB]!=(play_adr[dkB]/294)) && loop_active[dkB]==0)			//Set new CUE, when track play		
					{
					LOOP_OUT[dkB] = 0;	
					CUE_OPERATION[dkB] = CUE_NEED_SET;
					loop_lenght[dkB] = 8;	
					}

				if(loop_active[dkB]==0)
					{
					if(UT_SET[QUANTIZE]==1 && dSHOW==WAVEFORM)
						{
//						if(((play_adr/294)>(BEATGRID[dkB][bars[dkB]-1]+((BEATGRID[dkB][bars[dkB]] - BEATGRID[dkB][bars[dkB]-1])/2))) || bars[dkB]==0)	
//							{
//							LOOP_OUT = BEATGRID[dkB][bars[dkB]];								//next bar >> |
//							}
//						else
//							{
//							if(CUE_ADR==BEATGRID[dkB][bars[dkB]-1])
//								{
//								LOOP_OUT = BEATGRID[dkB][bars[dkB]];								//next bar >> |	
//								}
//							else	
//								{
//								LOOP_OUT = BEATGRID[dkB][bars[dkB]-1];							//previous bar <<	|
//								}
//							}	
						}
					else
						{
						//LOOP_OUT = play_adr/294;
						}	
					if(originalBPM[dkB]!=0)
						{						
						LOOP_OUT[dkB] = (play_adr[dkB]/294)+(loop150[loop_lenght[dkB]]/originalBPM[dkB]);	//4 bits
						}
					loop_act_gui[dkB] = 1;						
					loop_active[dkB] = 1;	
					loopmode_need_update[dkB] = 1;	
					}
				else
					{
					if(originalBPM[dkB]!=0)
						{						
						LOOP_OUT[dkB] = CUE_ADR[dkB]+(loop150[loop_lenght[dkB]]/originalBPM[dkB]);	//4 bits
						}	
					CUE_OPERATION[dkB] = CUE_NEED_CALL;	
					}					
					
				}	
			LP4_BTN_pressed[dkB] = 1;
			}
		else if((deckRbuf[9]&0x20)==0 && LP4_BTN_pressed[dkB]==1)
			{
			LP4_BTN_pressed[dkB] = 0;	
			}		
		else if((deckRbuf[9]&0x40) && LP8_BTN_pressed[dkB]==0)										///////////LP8 button
			{
			if(lock_control[dkB]==0)	
				{	
				if(loop_active[dkB])
					{
					if(loop_lenght[dkB]<12)
						{
						loop_lenght[dkB]++;
						}						
					}
				else if((play_enable[dkB]==1) && (CUE_ADR[dkB]!=(play_adr[dkB]/294)) && loop_active[dkB]==0)			//Set new CUE, when track play		
					{
					LOOP_OUT[dkB] = 0;	
					CUE_OPERATION[dkB] = CUE_NEED_SET;
					loop_lenght[dkB] = 9;	
					}

				if(loop_active[dkB]==0)
					{
					if(UT_SET[QUANTIZE]==1 && dSHOW==WAVEFORM)
						{
//						if(((play_adr/294)>(BEATGRID[dkB][bars[dkB]-1]+((BEATGRID[dkB][bars[dkB]] - BEATGRID[dkB][bars[dkB]-1])/2))) || bars[dkB]==0)	
//							{
//							LOOP_OUT = BEATGRID[dkB][bars[dkB]];								//next bar >> |
//							}
//						else
//							{
//							if(CUE_ADR==BEATGRID[dkB][bars[dkB]-1])
//								{
//								LOOP_OUT = BEATGRID[dkB][bars[dkB]];								//next bar >> |	
//								}
//							else	
//								{
//								LOOP_OUT = BEATGRID[dkB][bars[dkB]-1];							//previous bar <<	|
//								}
//							}	
						}
					else
						{
						//LOOP_OUT = play_adr/294;
						}	
					if(originalBPM[dkB]!=0)
						{						
						LOOP_OUT[dkB] = (play_adr[dkB]/294)+(loop150[loop_lenght[dkB]]/originalBPM[dkB]);	//8 bits
						}
					loop_act_gui[dkB] = 1;		
					loop_active[dkB] = 1;	
					loopmode_need_update[dkB] = 1;	
					}
				else
					{
					LOOP_OUT[dkB] = CUE_ADR[dkB]+(loop150[loop_lenght[dkB]]/originalBPM[dkB]);	//8 bits
					}					
				}	
			LP8_BTN_pressed[dkB] = 1;
			}
		else if((deckRbuf[9]&0x40)==0 && LP8_BTN_pressed[dkB]==1)
			{
			LP8_BTN_pressed[dkB] = 0;	
			}				
			
		if((deckRbuf[9]&0x04) && RVRS_BTN_pressed[dkB]==0)					///////////reverse switch position
			{
			if(RVRSEN[dkB])
				{
				if(SLIPEN[dkB])					//SLIP MODE ENABLE
					{	
					play_adr[dkB] = slip_pl_adr[dkB];	
					}		
				if(keep_slip[dkB])
					{
					keep_slip[dkB] = 0;	
					}
				deckTbuf[1][8]|= 0x08;				//disable red led reverse
				RVRSEN[dkB] = 0;
				}	
			else
				{
				if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
					{
					RVRS_BTN_pressed[dkB] = 2;	
					}	
				else
					{
					if(SLIPEN[dkB])			//SLIP MODE ENABLE
						{
						keep_slip[dkB] = 1;	
						}
					else
						{
						SLIPEN[dkB] = 1;	
						if(play_enable[dkB])
							{
							slip_pl_enable[dkB] = 1;	
							}
						slip_pl_adr[dkB] = play_adr[dkB]; 	
						}			
					RVRS_BTN_pressed[dkB] = 1;		
					}				
				deckTbuf[1][8]&= 0xF7; //enable red led reverse
				RVRSEN[dkB] = 1;						
				}
			}
		else if((deckRbuf[9]&0x04)==0 && RVRS_BTN_pressed[dkB]>0)
			{			
			if(RVRS_BTN_pressed[dkB]==1)
				{
				if(SLIPEN[dkB])					//SLIP MODE ENABLE
					{	
					play_adr[dkB] = slip_pl_adr[dkB];	
					}		
				if(keep_slip[dkB])
					{
					keep_slip[dkB] = 0;	
					}
				else
					{
					slip_pl_enable[dkB] = 0;	
					SLIPEN[dkB] = 0;	
					}
				deckTbuf[1][8]|= 0x08;				//disable red led reverse
				RVRSEN[dkB] = 0;						
				}
			RVRS_BTN_pressed[dkB] = 0;		
			}			
		
///////////////////////////////////////////////////JOG MECHANICAL PROCESS///////////////////////////////////////////////			
		if(inertial_rotation[dkB])
			{
			if((RVRSEN[dkB]==0 && (ROTDISdkB || (((deckRbuf[12]&0x80)==0) && pitch[dkB]<potenciometr_tempo[dkB]))) || 
				((RVRSEN[dkB]) && ((ROTDISdkB && (deckRbuf[12]&0x80)==0) || ((deckRbuf[12]&0x80) && pitch[dkB]<potenciometr_tempo[dkB]))))		//if rotation foward and stopped	
				{
				inertial_rotation[dkB] = 0;	
				}
			}				
		if(play_enable[dkB] || (need_call_to_cue[dkB]==3 && (ROTDISdkB || (deckRbuf[10]&0x80)!=0)))			//touch disable && rotation disable or play enable
			{
			need_call_to_cue[dkB] = 0;	
			}	
		else if((deckRbuf[10]&0x80)==0 && need_call_to_cue[dkB]==2)
			{
			pitch[dkB] = 0;	
			play_adr[dkB] = 294*CUE_ADR[dkB];	
			need_call_to_cue[dkB] = 3;	
			}		
			
		if(((deckRbuf[10]&0x80)!=0 || (play_enable[dkB]==0 && (CUE_ADR[dkB]!=(play_adr[dkB]/294))) || inertial_rotation[dkB]) && (deckTbuf[2][8]&0x08))				/////////////(touch enable	|| play_enable[dkB]==0) && Vinyl mode enable
			{
			slip_pitch[dkB] = potenciometr_tempo[dkB];
			if(JOG_PRESSED[dkB]==0)
				{
				if((deckRbuf[10]&0x80)!=0) 			//touch enable
					{
					change_speed[dkB] = NEED_DOWN;	
					JOG_PRESSED[dkB] = 2;	
					}
				else
					{
					JOG_PRESSED[dkB] = 1;
					}				
				}	
			if(play_enable[dkB]==0)	
				{
				if(((deckRbuf[10]&0x80)!=0) && (CUE_ADR[dkB]==(play_adr[dkB]/294)) && need_call_to_cue[dkB]==0)				//touch enable + play _enable==0 + CUE_ADR==(play_adr/294)
					{
					need_call_to_cue[dkB] = 1;	
					}	
				else if(need_call_to_cue[dkB]==1 && (deckRbuf[10]&0x80)==0)			//touch disable	_/
					{
					need_call_to_cue[dkB] = 2;	
					}					
				}
			if(ROTENdkB)					//rotation detect
				{
				if(need_call_to_cue[dkB]<2)
					{					
					change_speed[dkB] = NO_CHANGE;	
					
					if(((deckRbuf[12]&0x80)==0) && end_track[dkB])				//foward rotation + end_track[dkB]
						{
						pitch[dkB] = 0;
						change_speed[dkB] = NO_CHANGE;	
						}
					else
						{
						ptch = (256*deckRbuf[13]+deckRbuf[14]);
						if(ptch<86)
							{
							ptch = 86;	
							}
						ptch = 5574324/ptch;
						pitch[dkB] = ptch;						
						}
					slip_pitch[dkB] = potenciometr_tempo[dkB]; 	
					inertial_rotation[dkB] = 1;						
					}
				}		
			else if(change_speed[dkB]==NO_CHANGE) 
				{			
				pitch[dkB] = 0;	
				}

			if(change_speed[dkB]==NO_CHANGE)
				{	
				if(deckRbuf[12]&0x80)				//foward/reverse rotation
					{
					rvrs[dkB] = 1;
					}
				else
					{
					rvrs[dkB] = 0;	
					}		
				}
			else
				{
				if(RVRSEN[dkB])					//reverse diode enable
					{
					rvrs[dkB] = 1;
					}
				else
					{
					rvrs[dkB] = 0;	
					}	
				}
			deckTbuf[5][8]|=0x10;	//touch enable circle on display	
			//change masterdeck	
			if(play_enable[dkA] && masterdeck!=dkA)
				{
				masterdeck = dkA;
				tempo_need_update[dkA] = 2;
				tempo_need_update[dkB] = 2;	
				}	
			}		
		else if(ROTDISdkB && (deckRbuf[10]&0x80)==0)				///////////////////////touch disable and rotation disable
			{			
			slip_pitch[dkB] = potenciometr_tempo[dkB];
			if(JOG_PRESSED[dkB]>0) //jog PRESSED -> UNPRESSED
				{
				if(SLIPEN[dkB])					//SLIP MODE ENABLE
					{	
					change_speed[dkB] = NO_CHANGE;	
					}
				else if(JOG_PRESSED[dkB]==2 && play_enable[dkB])
					{
					change_speed[dkB] = NEED_UP;	
					}					
				JOG_PRESSED[dkB] = 0;	
				}
			if(play_enable[dkB])
				{	
				if(end_track[dkB] && RVRSEN[dkB]==0)					//stop on end (to remove noise at the end of the track)
					{
					change_speed[dkB] = NO_CHANGE;	
					pitch[dkB] = 0;	
					}	
				else if(change_speed[dkB]==NO_CHANGE)
					{
					pitch[dkB] = potenciometr_tempo[dkB];	
					}
				if(RVRSEN[dkB])					//reverse diode enable
					{
					rvrs[dkB] = 1;
					}
				else
					{
					rvrs[dkB] = 0;	
					}		
				}
			if(deckTbuf[5][8]&0x10)					//jog UNPRESSED
				{
				if(SLIPEN[dkB])					//SLIP MODE ENABLE
					{	
					play_adr[dkB] = slip_pl_adr[dkB];	
					}	
				deckTbuf[5][8]&=0xEF;				//disable touch circle on display
				}	
			}
		else if(ROTENdkB && play_enable[dkB])						//rotation detected			(pitch bend)	
			{
			if(end_track[dkB]==0)
				{
				ptch = (256*deckRbuf[13]+deckRbuf[14]);
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
				if((((deckRbuf[12]&0x80)==0) && (RVRSEN[dkB]==0)) || ((deckRbuf[12]&0x80) && RVRSEN[dkB]))		//foward rotation and reverse off OR reverse rotation and reverse on (pitch bend)			
					{
					ptch+= potenciometr_tempo[dkB];
					if(ptch>20000)
						{
						ptch = 20000;	
						}
					pitch[dkB] = ptch;
					}
				else if(((deckRbuf[12]&0x80) && (RVRSEN[dkB]==0)) || (((deckRbuf[12]&0x80)==0) && RVRSEN[dkB]))	 //reverse rotation and reverse off OR foward rotation and reverse on(pitch bend)	
					{
					if(ptch<potenciometr_tempo[dkB])
						{
						pitch[dkB] = potenciometr_tempo[dkB] - ptch;
						}
					else
						{
						pitch[dkB] = 0;	
						}
					}		
				}
			else
				{
				pitch[dkB] = 0;		
				}			
			if(RVRSEN[dkB])					//reverse diode enable
				{
				rvrs[dkB] = 1;
				}
			else
				{
				rvrs[dkB] = 0;	
				}	
			if(deckTbuf[5][8]&0x10)					//jog UNPRESSED
				{
				if(SLIPEN[dkB])					//SLIP MODE ENABLE
					{	
					play_adr[dkB] = slip_pl_adr[dkB];	
					}	
				deckTbuf[5][8]&=0xEF;				//disable touch circle on display	
				}	
			else
				{
				slip_pitch[dkB] = pitch[dkB];		
				}
			}



			
		if((TXpage%2)==1)		// 1/2 cycles for this deck
			{	
			if((deckTbuf[4][8]&0x10)==0)				//TEMPRO RESET ON
				{
				potenciometr_tempo[dkB] = 10000;	
				}
			else				//////////////////////////////////////////TEMPO CALCULATION
				{	
				pot10b[dkB] = 0x100*deckRbuf[10] + deckRbuf[11]; 	
				pot10b[dkB]&= 0x03FF;
				if(pot10b[dkB]==512)	
					{
					potenciometr_tempo[dkB] = 10000;	
					}
				else if(pot10b[dkB]>512)					/////pitch>0%
					{
					potwd[dkB] = pot10b[dkB]-512;	
					if(tempo_range[dkB]==0)										//	6%
						{
						potwd[dkB]*= 2409;	
						potenciometr_tempo[dkB] = potwd[dkB]>>12; 	
						potenciometr_tempo[dkB]*= 2;
						}	
					else if(tempo_range[dkB]==1)										//	10%	
						{
						potwd[dkB]*= 1608;	
						potenciometr_tempo[dkB] = potwd[dkB]>>12; 	
						potenciometr_tempo[dkB]*= 5;	
						}	
					else if(tempo_range[dkB]==2)										//	16%	
						{
						potwd[dkB]*= 2569;	
						potenciometr_tempo[dkB] = potwd[dkB]>>12; 	
						potenciometr_tempo[dkB]*= 5;
						}	
					else																		//	WIDE	
						{
						potwd[dkB]*= 1608;	
						potenciometr_tempo[dkB] = potwd[dkB]>>12; 	
						potenciometr_tempo[dkB]*= 50;	
						}	
					potenciometr_tempo[dkB]+= 10000;	
					}
				else				/////pitch<0%
					{
					potwd[dkB] = 511-pot10b[dkB];		
					if(tempo_range[dkB]==0)										//	6%	
						{	
						potwd[dkB]*= 2409;	
						potenciometr_tempo[dkB] = potwd[dkB]>>12; 	
						potenciometr_tempo[dkB]*= 2;	
						}	
					else if(tempo_range[dkB]==1)										//	10%	
						{	
						potwd[dkB]*= 1608;	
						potenciometr_tempo[dkB] = potwd[dkB]>>12; 	
						potenciometr_tempo[dkB]*= 5;	
						}
					else if(tempo_range[dkB]==2)										//	16%	
						{	
						potwd[dkB]*= 2569;	
						potenciometr_tempo[dkB] = potwd[dkB]>>12; 	
						potenciometr_tempo[dkB]*= 5;
						}	
					else																			//	WIDE
						{	
						potwd[dkB]*= 1608;	
						potenciometr_tempo[dkB] = potwd[dkB]>>12; 	
						potenciometr_tempo[dkB]*= 50;
						}
					potenciometr_tempo[dkB] = 10000 - potenciometr_tempo[dkB];	
					}				
				}
			if(prev_potenciometr_tempo[dkB] != potenciometr_tempo[dkB])
				{
				prev_potenciometr_tempo[dkB] = potenciometr_tempo[dkB];	
				tempo_need_update[dkB] = 1;
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
//					if(lock_control[dkB]==0)	
//						{	
//						if(play_enable[dkB] & play_adr<(all_long[dkB]+100000))	
//							{
//							//SEEK_AUDIOFRAME(play_adr+100000);	
//							}
//						else if(play_enable[dkB]==0 & play_adr/294<(all_long[dkB]+1))
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
//					if(lock_control[dkB]==0)	
//						{		
//						if(play_enable[dkB] & play_adr>100000)	
//							{
//							//SEEK_AUDIOFRAME(play_adr-100000);
//							}
//						else if(play_enable[dkB]==0 & play_adr>294)
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

			if(track_play_now[dkB]!=0)						//////////////////////////////LEDS/////////////////////////
				{					
				deckTbuf[0][8]|=0x10;				//CUE led on
					
				if(play_enable[dkB])
					{
					deckTbuf[0][8]|=0x18;	//PLAY and CUE led on
					}
				else											//Play led blink
					{	
					if(TIM_PLAY_LED[dkB])	
						{
						deckTbuf[0][8]|=0x08;	
						}
					else
						{
						deckTbuf[0][8]&=0xF7;
						}
					if(TIM_CUE_LED[dkB])	
						{
						deckTbuf[0][8]|=0x10;
						}
					else if(CUE_ADR[dkB]!=play_adr[dkB]/294)  
						{
						deckTbuf[0][8]&=0xEF;	
						}	
					}
						
				if(loop_active[dkB] || CUE_ADR[dkB]<LOOP_OUT[dkB])
					{	
					deckTbuf[3][8]|=0x10;							//RELOOP EXIT LED ON
					}
				else
					{
					deckTbuf[3][8]&=0xEF;							//RELOOP EXIT LED OFF	
					}
				}
			}		
		else if(TXpage==5)
			{				
			if((deckRbuf[9]&0x10) && JOG_MODE_BTN_pressed[dkB]==0) 							///////////Jog Mode button
				{
				if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
					{	
					if(REMAIN_ENABLE[dkB])
						{
						REMAIN_ENABLE[dkB] = 0;	
						}
					else
						{
						REMAIN_ENABLE[dkB] = 1;	
						}
					time_mode_need_update[dkB] = 1;	
					}
				else
					{
					if(deckTbuf[2][8]&0x08)			//VINYL => CDJ
						{
						deckTbuf[2][8]&=0xF7;
						deckTbuf[4][8]&=0xF7;						
						}	
					else												//CDJ => VINYL
						{	
						deckTbuf[2][8]|=0x08;
						deckTbuf[4][8]|=0x08;	
						}	
					}
				JOG_MODE_BTN_pressed[dkB] = 1;	
				}
			else if((deckRbuf[9]&0x10)==0 && JOG_MODE_BTN_pressed[dkB]==1)	
				{
				JOG_MODE_BTN_pressed[dkB] = 0;	
				}
			else if((deckRbuf[10]&0x40) && TEMPO_BTN_pressed[dkB]==0) 							///////////TEMPO Button
				{
				if((GPIOB->IDR & 0x00000200)==0)						//SHIFT BUTTON pressed
					{
					if(deckTbuf[4][8]&0x10)				//ON_RESET => OFF_RESET
						{
						deckTbuf[4][8]&=0xEF;
						}	
					else												//OFF_RESET => ON_RESET
						{
						deckTbuf[4][8]|=0x10;
						}						
					}
				else
					{
					if(tempo_range[dkB]<3)
						{
						tempo_range[dkB]++;
						}
					else
						{
						tempo_range[dkB] = 0;	
						}
					tempo_range_need_update[dkB] = 1;
					}
				TEMPO_BTN_pressed[dkB] = 1;	
				}
			else if((deckRbuf[10]&0x40)==0 && TEMPO_BTN_pressed[dkB]==1)	
				{
				TEMPO_BTN_pressed[dkB] = 0;	
				}	
			else if((deckRbuf[10]&0x20) && CALL_NEXT_BTN_pressed[dkB]==0) 							///////////CALL NEXT Button	>
				{
				if(lock_control[dkB]==0)	
					{		
					CUE_OPERATION[dkB] = MEMORY_NEED_NEXT_SET;
					}
				CALL_NEXT_BTN_pressed[dkB] = 1;	
				}
			else if((deckRbuf[10]&0x20)==0 && CALL_NEXT_BTN_pressed[dkB]==1)	
				{
				CALL_NEXT_BTN_pressed[dkB] = 0;	
				}
			else if((deckRbuf[10]&0x10) && CALL_PREV_BTN_pressed[dkB]==0) 							///////////CALL PREVIOUS Button <
				{
				if(lock_control[dkB]==0)	
					{		
					CUE_OPERATION[dkB] = MEMORY_NEED_PREVIOUS_SET;
					}
				CALL_PREV_BTN_pressed[dkB] = 1;	
				}
			else if((deckRbuf[10]&0x10)==0 && CALL_PREV_BTN_pressed[dkB]==1)	
				{
				CALL_PREV_BTN_pressed[dkB] = 0;	
				}

			if((deckRbuf[9]&0x08) && SLIP_BTN_pressed[dkB]==0) 							///////////SLIP MODE Button
				{
				if(SLIPEN[dkB])					//ON_SLIP_MODE => OFF_SLIP_MODE
					{
					if(keep_slip[dkB])
						{
						keep_slip[dkB] = 0;	
						}
					else
						{
						slip_pl_enable[dkB] = 0;	
						SLIPEN[dkB] = 0;	
						deckTbuf[1][8]|=0x10;		//led on		
						}																
					}	
				else												//OFF_SLIP_MODE => ON_SLIP_MODE
					{	
					SLIPEN[dkB] = 1;
					deckTbuf[1][8]&=0xEF;		//led off	
					if(play_enable[dkB])
						{
						slip_pl_enable[dkB] = 1;	
						}
					slip_pl_adr[dkB] = play_adr[dkB]; 	
					}
				SLIP_BTN_pressed[dkB] = 1;	
				}
			else if((deckRbuf[9]&0x08)==0 && SLIP_BTN_pressed[dkB]==1)	
				{
				SLIP_BTN_pressed[dkB] = 0;	
				}		
			}			
		
			
		if(load_animation_en[dkB])
			{
			//Tbuffer[21] = 0;					//disable red cue marker
			//Tbuffer[23] &= 0xDF;			//disable touch circle on display		
			deckTbuf[TXpage][12] = 137;				//command load animation	
			}
		else if(track_play_now[dkB]==0)
			{
			deckTbuf[TXpage][12] = 135;
			deckTbuf[TXpage][13] = 85;	
			deckTbuf[TXpage][14] = 85; 	//cue none
			deckTbuf[0][8]&=0xE7;		//PLAY & CUE leds off
			deckTbuf[5][8]&=0xEF;				//disable touch circle on display		
			}
		else
			{
			deckTbuf[TXpage][14] = RED_CRCL_CUE_ADR[dkB]; 	//cue		
			if(SLIPEN[dkB])					//SLIP MODE ENABLE
				{	
				deckTbuf[TXpage][13] = (1000*(((slip_pl_adr[dkB]/588)%135)+1)/1589);							//slip
				}
			else
				{
				deckTbuf[TXpage][13] = 85;	
				}				
			deckTbuf[TXpage][12] = (play_adr[dkB]/588)%135;
			}
		}	


	////////////////////////////////////////////////from 1000hz (400) timer
	
	if(change_speed[dkB]==NEED_UP)
		{
		if(end_track[dkB])
			{
			change_speed[dkB] = NO_CHANGE;
			pitch[dkB] = 0;			
			}
		else
			{
			if(pitch[dkB]<potenciometr_tempo[dkB]-acceleration_UP)
				{
				pitch[dkB]+=acceleration_UP;	
				}
			else
				{
				change_speed[dkB] = NO_CHANGE;
				pitch[dkB] = potenciometr_tempo[dkB];	
				}		
			}
		}
	else if(change_speed[dkB]==NEED_DOWN)
		{
		if(end_track[dkB])
			{
			change_speed[dkB] = NO_CHANGE;
			pitch[dkB] = 0;			
			}
		else	
			{
			if(pitch[dkB]>acceleration_DOWN)
				{
				pitch[dkB]-=acceleration_DOWN;	
				}
			else
				{
				change_speed[dkB] = NO_CHANGE;
				pitch[dkB] = 0;	
				}		
			}
		}






	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



