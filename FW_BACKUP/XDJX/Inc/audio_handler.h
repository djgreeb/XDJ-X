/////////////////////////////////////////////////////////////////////////////////////////////////
// Audio processing handler 44100Hz
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

void SET_CUE(uint32_t nf_adr);


/////////////////////////////////////////////	
//SET CUE,create adress CUE_ADR
//create offset data
//copy audiodata from main audio buffer to cue_mem buffer
//
//	nf_adr - new frame adress in 1/150s
void SET_CUE(uint32_t nf_adr)
	{
	uint16_t c_adr;	
	uint32_t copy_cnt = 0;				//internal counter
	uint32_t AIS = 0;							//adress in samples 44k for CUE	
	uint32_t cpos;	
	CUE_ADR = nf_adr;	
	AIS = (294*CUE_ADR)&0xFFFFE000;							//rounding up to 8192
	AIS-=81920;
	mem_offset_adress = (AIS&0xFFFFF)>>13;
		
	for(copy_cnt=0;copy_cnt<327680;copy_cnt++)
		{
		PCM[(copy_cnt>>14)+128][(copy_cnt>>1)&0x1FFF][copy_cnt&0x01] = PCM[((copy_cnt>>14)+mem_offset_adress)&0x7F][(copy_cnt>>1)&0x1FFF][copy_cnt&0x01];	
		}
	cpos = 203*CUE_ADR;
	cpos/= all_long;	
	DrawCueMarker(cpos);	
	c_adr = ((nf_adr/2)%135)+1;	
	RED_CIRCLE_CUE_ADR = ((1000*c_adr/1589)+1)%85;
//	if(dSHOW==WAVEFORM)							//Redraw cue on dynamic waveform
//		{
//		forcibly_redraw = 1;
//		}	
//	REALTIME_CUE_LED_BLINK = 0;					//start blink led
	return;	
	};

/////////////////////////////////////////////	
//CAL CUE,seek audio frame
//
//
//	
void CALL_CUE(void)
	{
	uint32_t seek_adr = 294*CUE_ADR;
	seek_adr &= 0xFFFFE000;	
	if(FR_OK==f_lseek(&file, ((seek_adr<<2)+44)))
		{
		end_adr_valid_data = (seek_adr>>13);
		start_adr_valid_data = end_adr_valid_data; 	
		play_adr = 294*CUE_ADR;		
		if(SLIPEN)					//SLIP MODE ENABLE
			{	
			slip_play_adr = play_adr;	
			}			
		}
	offset_adress = 128-mem_offset_adress;	
	}	
	
	
/////////////////////////////////////////////	
//SET CUE to MEMORY adress, create adress CUE_ADR
//create offset data
//copy audiodata from main audio buffer to cue_mem buffer
//
//	nf_adr - new frame adress in 1/150s	
void SET_MEMORY_CUE_1(uint32_t nf_adr)
	{
	uint16_t c_adr;	
	uint32_t AIS = 0;							//adress in samples 44k for CUE
	uint32_t cpos;	
	pitch = 0;	
	play_enable = 0;
	if(SLIPEN)					//OFF_SLIP_MODE
		{
		SLIPEN = 0;	
		}	
	CUE_ADR = nf_adr;	
	AIS = (294*CUE_ADR)&0xFFFFE000;							//rounding up to 8192
	if(FR_OK==f_lseek(&file, ((AIS<<2)+44)))
		{
		end_adr_valid_data = (AIS>>13);
		start_adr_valid_data = end_adr_valid_data; 	
		play_adr = 294*CUE_ADR;		
		}

	AIS-=81920;
	mem_offset_adress = (AIS&0xFFFFF)>>13;	
	cpos = 203*CUE_ADR;
	cpos/= all_long;	
	DrawCueMarker(cpos);		
	c_adr = ((nf_adr/2)%135)+1;	
	RED_CIRCLE_CUE_ADR = ((1000*c_adr/1589)+1)%85;
	REALTIME_CUE_LED_BLINK = 0;					//start blink led		
	return;		
	}
	
/////////////////////////////////////////////	
//SET CUE to MEMORY adress second part operation
//Fill CUE buffer
//
//
void SET_MEMORY_CUE_2(void)
	{
	uint32_t copy_cnt = 0;				//internal counter
	for(copy_cnt=0;copy_cnt<327680;copy_cnt++)
		{
		//////////////////////replace on memcpy	
		PCM[(copy_cnt>>14)+128][(copy_cnt>>1)&0x1FFF][copy_cnt&0x01] = PCM[((copy_cnt>>14)+mem_offset_adress)&0x7F][(copy_cnt>>1)&0x1FFF][copy_cnt&0x01];	
		}	
	offset_adress = 128-mem_offset_adress;	
	return;	
	}	
	



//////////////////////////////////////////////////////////
//
//			AUDIO HANDLER
//
void SAI1_IRQHandler(void)
	{		
	HAL_SAI_IRQHandler(&hsai_BlockA1);
  HAL_SAI_IRQHandler(&hsai_BlockB1);
	HAL_SAI_Transmit_IT(&hsai_BlockA1, (uint8_t*)&deckAout[0], 2);
	HAL_SAI_Transmit_IT(&hsai_BlockB1, (uint8_t*)&deckAout[0], 2);	
	

if(((play_adr+step_position+3)<=(294*all_long)))						//change all_long extract!
		{
		end_of_track = 0;	
		}
	else
		{
		end_of_track = 1;		
		}			
		
	if(SLIPEN && ((slip_play_adr+((slip_position+pitch_for_slip)/10000))<(294*all_long)) && slip_play_enable)					//SLIP MODE ENABLE
		{
		slip_position+= pitch_for_slip;
		slip_play_adr+=slip_position/10000;	
		slip_position = slip_position%10000;	
		}
	
	if(previous_pitch!=pitch)			//calculate TRIM curve
		{
		if(pitch<3000)
			{
			PRETRIM = pitch;
			PRETRIM*= 0.000283F;
			PRETRIM+= 0.05F;	
			}			
		else
			{
			PRETRIM = 0.90F;	
			}			
		previous_pitch = pitch;	
		}	
	TRIM = 0.005*((199*TRIM)+PRETRIM);	
		
	position+= pitch;
	if(position>9999)	
		{
		step_position = position/10000;				
		if(reverse==0 && end_of_track==0)					
			{			
			play_adr+= step_position;	
			if(step_position==1)
				{
				LR[0][0] = LR[0][1];
				LR[1][0] = LR[1][1];
				LR[0][1] = LR[0][2];
				LR[1][1] = LR[1][2];
				LR[0][2] = LR[0][3];
				LR[1][2] = LR[1][3];					
				}
			else
				{
				sdram_adr = play_adr&0xFFFFF;						
				LR[0][0] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];							
				LR[1][0] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];
				sdram_adr = (play_adr+1)&0xFFFFF;
				LR[0][1] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];								
				LR[1][1] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];		
				sdram_adr = (play_adr+2)&0xFFFFF;
				LR[0][2] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];									
				LR[1][2] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];
				}
			sdram_adr = (play_adr+3)&0xFFFFF;	
			LR[0][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];							
			LR[1][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];		
			}
		else if(reverse==1 && play_adr>=step_position)
			{
			play_adr-= step_position;
			if(step_position==1)
				{
				LR[0][0] = LR[0][1];
				LR[1][0] = LR[1][1];
				LR[0][1] = LR[0][2];
				LR[1][1] = LR[1][2];
				LR[0][2] = LR[0][3];
				LR[1][2] = LR[1][3];
				sdram_adr = (play_adr)&0xFFFFF;	
				LR[0][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];							
				LR[1][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];		
				}
			else
				{
				sdram_adr = play_adr&0xFFFFF;						
				LR[0][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];							
				LR[1][3] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];
				sdram_adr = (play_adr+1)&0xFFFFF;
				LR[0][2] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];								
				LR[1][2] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];		
				sdram_adr = (play_adr+2)&0xFFFFF;
				LR[0][1] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];									
				LR[1][1] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];
				sdram_adr = (play_adr+3)&0xFFFFF;	
				LR[0][0] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][0];							
				LR[1][0] = PCM[(sdram_adr>>13)+offset_adress][sdram_adr&0x1FFF][1];			
				}	
			}	
		position = position%10000;	
			
		even1l = LR[0][2];
		even1l+= LR[0][1];
		odd1l = LR[0][2];
		odd1l-= LR[0][1];
		even2l = LR[0][3];
		even2l+= LR[0][0];
		odd2l = LR[0][3];
		odd2l-= LR[0][0];
		c0l = (float)even1l*COEF[0];
		r0l = (float)even2l*COEF[1];
		c0l+= r0l;
		c1l = (float)odd1l*COEF[2];
		r1l = (float)odd2l*COEF[3];
		c1l+= r1l;
		c2l = (float)even1l*COEF[4]; 
		r2l = (float)even2l*COEF[5];
		c2l+= r2l;
		c3l = (float)odd1l*COEF[6];
		r3l = (float)odd2l*COEF[7];
		c3l+= r3l;	
			
		even1r = LR[1][2];
		even1r+= LR[1][1];
		odd1r = LR[1][2];
		odd1r-= LR[1][1];
		even2r = LR[1][3];
		even2r+= LR[1][0]; 
		odd2r = LR[1][3];
		odd2r-= LR[1][0];
		c0r = (float)even1r*COEF[0];
		r0r = (float)even2r*COEF[1];
		c0r+= r0r;
		c1r = (float)odd1r*COEF[2];
		r1r = (float)odd2r*COEF[3];
		c1r+= r1r;
		c2r = (float)even1r*COEF[4]; 
		r2r = (float)even2r*COEF[5];
		c2r+= r2r;
		c3r = (float)odd1r*COEF[6];
		r3r = (float)odd2r*COEF[7];
		c3r+= r3r;	
		}	
	T = position;
	T/= 10000;
	T-= 1/2.0F;
	SAMPLE_BUFFER = c0l+T*(c1l+T*(c2l+T*c3l));
	firR[1] = firR[0];																								//////////////
	firR[0] = SAMPLE_BUFFER;																					//	FIR High Pass		
	deckAout[0] = TRIM*(FIRCoef[0]*firR[0] + FIRCoef[1]*firR[1]);			//	TRIM
	SAMPLE_BUFFER = c0r+T*(c1r+T*(c2r+T*c3r));
	firL[1] = firL[0];																								//////////////
	firL[0] = SAMPLE_BUFFER;																					//	FIR High Pass		
	deckAout[1] = TRIM*(FIRCoef[0]*firL[0] + FIRCoef[1]*firL[1]);			//	TRIM	
	}
	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



