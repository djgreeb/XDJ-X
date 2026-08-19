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

void SET_CUE(uint8_t dk, uint32_t nf_adr);


/////////////////////////////////////////////	
//seek adress in samples (44100 per second)
//
void SEEK_AUDIOFRAME(uint8_t dk, uint32_t seek_adr)
	{
	if(seek_adr>(294*all_long[dk]))
		{
		return;	
		}
	res = f_lseek(&file[dk], (((seek_adr&0xFFFFE000)<<2)+44));		
	if(FR_OK==res)
		{			
		end_adata[dk] = (seek_adr>>13);
		start_adata[dk] = end_adata[dk];
		play_adr[dk] = seek_adr;	
		fill_stp[dk] = 0;	
		if(SLIPEN[dk])					//SLIP MODE ENABLE
			{	
			slip_pl_adr[dk] = play_adr[dk];	
			}
		prevstrt[dk] = (start_adata[dk]*5628)/all_long[dk];		//	for update mem bar
		prevfin[dk]	= prevstrt[dk];				
		need_redraw_memline[dk] = 1;	
		}			
	return;	
	};

/////////////////////////////////////////////	
//SET CUE,create adress CUE_ADR
//create offset data
//copy audiodata from main audio buffer to cue_mem buffer
//	[..8..CUE_ADR..8..]
//	nf_adr - new frame adress in 1/150s
void SET_CUE(uint8_t dk, uint32_t nf_adr)
	{
	uint16_t c_adr;	
	uint32_t copy_cnt;				//internal counter
	uint32_t AIS;							//adress in samples 44k for CUE	
	uint32_t cpos;	
	CUE_ADR[dk] = nf_adr;	
	AIS = (294*CUE_ADR[dk])&0xFFFFE000;							//rounding up to 8192
	AIS-=65536;
	mem_offset_adress[dk] = (AIS&0xFFFFF)>>13;
	//audio buffer=>cue_mem buffer	(110ms copy process)		
	for(copy_cnt=0;copy_cnt<262144;copy_cnt+=16384)			//3,715 sec CUE buff
		{
		memcpy(&sdram.pcm[dk][0][0][copy_cnt+(128<<14)], &sdram.pcm[dk][0][0][(copy_cnt+(mem_offset_adress[dk]<<14))&0x1FFFFF], 32768);	
		}
	cpos = 202*CUE_ADR[dk];
	cpos/= all_long[dk];	
	DrawCueMarker(dk, cpos);	
	c_adr = ((nf_adr/2)%135)+1;	
	RED_CRCL_CUE_ADR[dk] = ((1000*c_adr/1589)+1)%85;
	if(dSHOW==WAVEFORM)							//Redraw cue on dynamic waveform
		{
		forcibly_redraw[dk] = 1;
		}	
//	REALTIME_CUE_LED_BLINK = 0;					//start blink led
	return;	
	};

/////////////////////////////////////////////	
//CAL CUE,seek audio frame
//
//
//	
void CALL_CUE(uint8_t dk)
	{
	uint32_t seek_adr = 294*CUE_ADR[dk];
	seek_adr&=0xFFFFE000;	
	if(start_adata[dk]<=(seek_adr>>13) && end_adata[dk]>(seek_adr>>13))	
		{
		play_adr[dk] = 294*CUE_ADR[dk];		
		if(SLIPEN[dk])					//SLIP MODE ENABLE
			{	
			slip_pl_adr[dk] = play_adr[dk];	
			}		
		}
	else
		{		
		res = f_lseek(&file[dk], ((seek_adr<<2)+44));		
		if(res==FR_OK)
			{
			end_adata[dk] = (seek_adr>>13);
			start_adata[dk] = end_adata[dk]; 	
			prevstrt[dk] = (start_adata[dk]*5628)/all_long[dk];		//	for update mem bar
			prevfin[dk]	= prevstrt[dk];														//					
			play_adr[dk] = 294*CUE_ADR[dk];		
			if(SLIPEN[dk])					//SLIP MODE ENABLE
				{	
				slip_pl_adr[dk] = play_adr[dk];	
				}			
			}
		}
	offset_adress[dk] = 128-mem_offset_adress[dk];	
	offset_adressBIG[dk] = offset_adress[dk]<<13;	
	}	
	
	
/////////////////////////////////////////////	
//SET CUE to MEMORY adress, create adress CUE_ADR
//create offset data
//copy audiodata from circle audio buffer=>cue_mem buffer
//
//	nf_adr - new frame adress in 1/150s	
void SET_MEMORY_CUE_1(uint8_t dk, uint32_t nf_adr)
	{
	uint16_t c_adr;	
	uint32_t AIS;							//adress in samples 44k for CUE
	uint32_t cpos;	
	pitch[dk] = 0;	
	play_enable[dk] = 0;
	if(SLIPEN[dk])					//OFF_SLIP_MODE
		{
		SLIPEN[dk] = 0;	
		}	
	CUE_ADR[dk] = nf_adr;	
	AIS = (294*CUE_ADR[dk])&0xFFFFE000;							//rounding up to 8192
	res = f_lseek(&file[dk], ((AIS<<2)+44));
	if(res==FR_OK)
		{
		end_adata[dk] = (AIS>>13);
		start_adata[dk] = end_adata[dk]; 	
		prevstrt[dk] = (start_adata[dk]*5628)/all_long[dk];		//	for update mem bar
		prevfin[dk]	= prevstrt[dk];														//			
		play_adr[dk] = 294*CUE_ADR[dk];	
		}		
	AIS-=65536;	//8192*8
	mem_offset_adress[dk] = (AIS&0x000FFFFF)>>13;	
	cpos = 202*CUE_ADR[dk];
	cpos/= all_long[dk];	
	DrawCueMarker(dk, cpos);		
	c_adr = ((nf_adr/2)%135)+1;	
	RED_CRCL_CUE_ADR[dk] = ((1000*c_adr/1589)+1)%85;
	//REALTIME_CUE_LED_BLINK = 0;					//start blink led		
	return;		
	}
	
/////////////////////////////////////////////	
//SET CUE to MEMORY adress second part operation
//Fill CUE buffer
//
//
void SET_MEMORY_CUE_2(uint8_t dk)
	{
	uint32_t copy_cnt = 0;				//internal counter
	//audio buffer=>cue_mem buffer	(110ms copy process)	
	for(copy_cnt=0;copy_cnt<262144;copy_cnt+=16384)			//3,715 sec CUE buff
		{
		memcpy(&sdram.pcm[dk][0][0][copy_cnt+(128<<14)], &sdram.pcm[dk][0][0][(copy_cnt+(mem_offset_adress[dk]<<14))&0x1FFFFF], 32768);	
		}
	offset_adress[dk] = 128-mem_offset_adress[dk];	
	offset_adressBIG[dk] = offset_adress[dk]<<13;	
	return;	
	}	
	

//////////////////////////////////////////////////////////
//
//			AUDIO HANDLER
//			
//	v.1.67				
//	5-7uS	
//	Duty: 21-27%
//	
void SAI1_IRQHandler(void)
	{	
	//TEST_H;		
	SAI_IRQHndlr(&hsai_BlockA1);																	//	Duty: 7%		
  SAI_IRQHndlr(&hsai_BlockB1);																	//	
	SAI_Transmit(&hsai_BlockA1, (uint8_t*)&deckout[dkA][0], 2);		//
	SAI_Transmit(&hsai_BlockB1, (uint8_t*)&deckout[dkB][0], 2);		//
		
		
	/////////////////////////////////////////			DECK A			//////////////////////////////////////////////////	
	if(((play_adr[dkA]+step_postn[dkA]+3)<=(294*all_long[dkA])))							//change all_long[dkA] extract!
		{
		end_track[dkA] = 0;	
		}
	else
		{
		end_track[dkA] = 1;		
		}			
		
	if(SLIPEN[dkA] && ((slip_pl_adr[dkA]+((slip_postn[dkA]+slip_pitch[dkA])/10000))<(294*all_long[dkA])) && slip_pl_enable[dkA])					//SLIP MODE ENABLE
		{
		slip_postn[dkA]+= slip_pitch[dkA];
		slip_pl_adr[dkA]+=slip_postn[dkA]/10000;	
		slip_postn[dkA] = slip_postn[dkA]%10000;	
		}
	
	if(prev_pitch[dkA]!=pitch[dkA])			//calculate TRIM curve
		{
		if(pitch[dkA]<4000)
			{
			PRETRIM[dkA] = pitch[dkA];
			PRETRIM[dkA]*= 0.000225F;
			}			
		else
			{
			PRETRIM[dkA] = 0.90F;	
			}			
		prev_pitch[dkA] = pitch[dkA];	
		}	
	TRIM[dkA] = 0.005*((199*TRIM[dkA])+PRETRIM[dkA]);	
		
	postn[dkA]+= pitch[dkA];
	if(postn[dkA]>9999)	
		{
		step_postn[dkA] = postn[dkA]/10000;				
		if(rvrs[dkA]==0 && end_track[dkA]==0)					
			{			
			play_adr[dkA]+= step_postn[dkA];	
			if(step_postn[dkA]==1)
				{
				LR[dkA][0][0] = LR[dkA][0][1];
				LR[dkA][1][0] = LR[dkA][1][1];
				LR[dkA][0][1] = LR[dkA][0][2];
				LR[dkA][1][1] = LR[dkA][1][2];
				LR[dkA][0][2] = LR[dkA][0][3];
				LR[dkA][1][2] = LR[dkA][1][3];					
				}
			else
				{	
				sdram_adr[dkA] = play_adr[dkA]&0xFFFFF;	
				sdram_adr[dkA]+= offset_adressBIG[dkA];
				LR[dkA][0][0] = sdram.pcm[dkA][0][sdram_adr[dkA]][0];							
				LR[dkA][1][0] = sdram.pcm[dkA][0][sdram_adr[dkA]][1];
				sdram_adr[dkA] = (play_adr[dkA]+1)&0xFFFFF;
				sdram_adr[dkA]+= offset_adressBIG[dkA];	
				LR[dkA][0][1] = sdram.pcm[dkA][0][sdram_adr[dkA]][0];							
				LR[dkA][1][1] = sdram.pcm[dkA][0][sdram_adr[dkA]][1];	
				sdram_adr[dkA] = (play_adr[dkA]+2)&0xFFFFF;
				sdram_adr[dkA]+= offset_adressBIG[dkA];	
				LR[dkA][0][2] = sdram.pcm[dkA][0][sdram_adr[dkA]][0];									
				LR[dkA][1][2] = sdram.pcm[dkA][0][sdram_adr[dkA]][1];
				}
			sdram_adr[dkA] = (play_adr[dkA]+3)&0xFFFFF;	
			sdram_adr[dkA]+=	offset_adressBIG[dkA];	
			LR[dkA][0][3] = sdram.pcm[dkA][0][sdram_adr[dkA]][0];							
			LR[dkA][1][3] = sdram.pcm[dkA][0][sdram_adr[dkA]][1];		
			}
		else if(rvrs[dkA]==1 && play_adr[dkA]>=step_postn[dkA])
			{
			play_adr[dkA]-= step_postn[dkA];
			if(step_postn[dkA]==1)
				{
				LR[dkA][0][0] = LR[dkA][0][1];
				LR[dkA][1][0] = LR[dkA][1][1];
				LR[dkA][0][1] = LR[dkA][0][2];
				LR[dkA][1][1] = LR[dkA][1][2];
				LR[dkA][0][2] = LR[dkA][0][3];
				LR[dkA][1][2] = LR[dkA][1][3];
				sdram_adr[dkA] = (play_adr[dkA])&0xFFFFF;	
				sdram_adr[dkA]+=	offset_adressBIG[dkA];		
				LR[dkA][0][3] = sdram.pcm[dkA][0][sdram_adr[dkA]][0];							
				LR[dkA][1][3] = sdram.pcm[dkA][0][sdram_adr[dkA]][1];		
				}
			else
				{
				sdram_adr[dkA] = play_adr[dkA]&0xFFFFF;	
				sdram_adr[dkA]+=	offset_adressBIG[dkA];		
				LR[dkA][0][3] = sdram.pcm[dkA][0][sdram_adr[dkA]][0];							
				LR[dkA][1][3] = sdram.pcm[dkA][0][sdram_adr[dkA]][1];
				sdram_adr[dkA] = (play_adr[dkA]+1)&0xFFFFF;
				sdram_adr[dkA]+=	offset_adressBIG[dkA];	
				LR[dkA][0][2] = sdram.pcm[dkA][0][sdram_adr[dkA]][0];								
				LR[dkA][1][2] = sdram.pcm[dkA][0][sdram_adr[dkA]][1];		
				sdram_adr[dkA] = (play_adr[dkA]+2)&0xFFFFF;
				sdram_adr[dkA]+=	offset_adressBIG[dkA];
				LR[dkA][0][1] = sdram.pcm[dkA][0][sdram_adr[dkA]][0];									
				LR[dkA][1][1] = sdram.pcm[dkA][0][sdram_adr[dkA]][1];
				sdram_adr[dkA] = (play_adr[dkA]+3)&0xFFFFF;
				sdram_adr[dkA]+=	offset_adressBIG[dkA];
				LR[dkA][0][0] = sdram.pcm[dkA][0][sdram_adr[dkA]][0];							
				LR[dkA][1][0] = sdram.pcm[dkA][0][sdram_adr[dkA]][1];			
				}	
			}	
		postn[dkA]%=10000;	
			
		even1 = LR[dkA][0][2];
		even1+= LR[dkA][0][1];
		odd1 = LR[dkA][0][2];
		odd1-= LR[dkA][0][1];
		even2 = LR[dkA][0][3];
		even2+= LR[dkA][0][0];
		odd2 = LR[dkA][0][3];
		odd2-= LR[dkA][0][0];
		c0l[dkA] = (float)even1*COEF[0];
		r0l[dkA] = (float)even2*COEF[1];
		c0l[dkA]+= r0l[dkA];
		c1l[dkA] = (float)odd1*COEF[2];
		r1l[dkA] = (float)odd2*COEF[3];
		c1l[dkA]+= r1l[dkA];
		c2l[dkA] = (float)even1*COEF[4]; 
		r2l[dkA] = (float)even2*COEF[5];
		c2l[dkA]+= r2l[dkA];
		c3l[dkA] = (float)odd1*COEF[6];
		r3l[dkA] = (float)odd2*COEF[7];
		c3l[dkA]+= r3l[dkA];	
			
		even1 = LR[dkA][1][2];
		even1+= LR[dkA][1][1];
		odd1 = LR[dkA][1][2];
		odd1-= LR[dkA][1][1];
		even2 = LR[dkA][1][3];
		even2+= LR[dkA][1][0]; 
		odd2 = LR[dkA][1][3];
		odd2-= LR[dkA][1][0];
		c0r[dkA] = (float)even1*COEF[0];
		r0r[dkA] = (float)even2*COEF[1];
		c0r[dkA]+= r0r[dkA];
		c1r[dkA] = (float)odd1*COEF[2];
		r1r[dkA] = (float)odd2*COEF[3];
		c1r[dkA]+= r1r[dkA];
		c2r[dkA] = (float)even1*COEF[4]; 
		r2r[dkA] = (float)even2*COEF[5];
		c2r[dkA]+= r2r[dkA];
		c3r[dkA] = (float)odd1*COEF[6];
		r3r[dkA] = (float)odd2*COEF[7];
		c3r[dkA]+= r3r[dkA];	
		}	
	T[dkA] = postn[dkA];
	T[dkA]/= 10000;
	T[dkA]-= 1/2.0F;
	SAMPLE_BUFFER = c0l[dkA]+T[dkA]*(c1l[dkA]+T[dkA]*(c2l[dkA]+T[dkA]*c3l[dkA]));
	firR[dkA][1] = firR[dkA][0];																								//////////////
	firR[dkA][0] = SAMPLE_BUFFER;																					//	FIR High Pass		
	deckout[dkA][0] = TRIM[dkA]*(FIRCoef[0]*firR[dkA][0] + FIRCoef[1]*firR[dkA][1]);			//	TRIM
	SAMPLE_BUFFER = c0r[dkA]+T[dkA]*(c1r[dkA]+T[dkA]*(c2r[dkA]+T[dkA]*c3r[dkA]));
	firL[dkA][1] = firL[dkA][0];																								//////////////
	firL[dkA][0] = SAMPLE_BUFFER;																					//	FIR High Pass		
	deckout[dkA][1] = TRIM[dkA]*(FIRCoef[0]*firL[dkA][0] + FIRCoef[1]*firL[dkA][1]);			//	TRIM

	
	/////////////////////////////////////////			DECK B			//////////////////////////////////////////////////
	if(((play_adr[dkB]+step_postn[dkB]+3)<=(294*all_long[dkB])))							//change all_long[dkB] extract!
		{
		end_track[dkB] = 0;	
		}
	else
		{
		end_track[dkB] = 1;		
		}			
		
	if(SLIPEN[dkB] && ((slip_pl_adr[dkB]+((slip_postn[dkB]+slip_pitch[dkB])/10000))<(294*all_long[dkB])) && slip_pl_enable[dkB])					//SLIP MODE ENABLE
		{
		slip_postn[dkB]+= slip_pitch[dkB];
		slip_pl_adr[dkB]+=slip_postn[dkB]/10000;	
		slip_postn[dkB] = slip_postn[dkB]%10000;	
		}
	
	if(prev_pitch[dkB]!=pitch[dkB])			//calculate TRIM curve
		{
		if(pitch[dkB]<4000)
			{
			PRETRIM[dkB] = pitch[dkB];
			PRETRIM[dkB]*= 0.000225F;
			}			
		else
			{
			PRETRIM[dkB] = 0.90F;	
			}			
		prev_pitch[dkB] = pitch[dkB];	
		}	
	TRIM[dkB] = 0.005*((199*TRIM[dkB])+PRETRIM[dkB]);	
		
	postn[dkB]+= pitch[dkB];
	if(postn[dkB]>9999)	
		{
		step_postn[dkB] = postn[dkB]/10000;				
		if(rvrs[dkB]==0 && end_track[dkB]==0)					
			{			
			play_adr[dkB]+= step_postn[dkB];	
			if(step_postn[dkB]==1)
				{
				LR[dkB][0][0] = LR[dkB][0][1];
				LR[dkB][1][0] = LR[dkB][1][1];
				LR[dkB][0][1] = LR[dkB][0][2];
				LR[dkB][1][1] = LR[dkB][1][2];
				LR[dkB][0][2] = LR[dkB][0][3];
				LR[dkB][1][2] = LR[dkB][1][3];					
				}
			else
				{	
				sdram_adr[dkB] = play_adr[dkB]&0xFFFFF;	
				sdram_adr[dkB]+= offset_adressBIG[dkB];
				LR[dkB][0][0] = sdram.pcm[dkB][0][sdram_adr[dkB]][0];							
				LR[dkB][1][0] = sdram.pcm[dkB][0][sdram_adr[dkB]][1];
				sdram_adr[dkB] = (play_adr[dkB]+1)&0xFFFFF;
				sdram_adr[dkB]+= offset_adressBIG[dkB];	
				LR[dkB][0][1] = sdram.pcm[dkB][0][sdram_adr[dkB]][0];							
				LR[dkB][1][1] = sdram.pcm[dkB][0][sdram_adr[dkB]][1];	
				sdram_adr[dkB] = (play_adr[dkB]+2)&0xFFFFF;
				sdram_adr[dkB]+= offset_adressBIG[dkB];	
				LR[dkB][0][2] = sdram.pcm[dkB][0][sdram_adr[dkB]][0];									
				LR[dkB][1][2] = sdram.pcm[dkB][0][sdram_adr[dkB]][1];
				}
			sdram_adr[dkB] = (play_adr[dkB]+3)&0xFFFFF;	
			sdram_adr[dkB]+=	offset_adressBIG[dkB];	
			LR[dkB][0][3] = sdram.pcm[dkB][0][sdram_adr[dkB]][0];							
			LR[dkB][1][3] = sdram.pcm[dkB][0][sdram_adr[dkB]][1];		
			}
		else if(rvrs[dkB]==1 && play_adr[dkB]>=step_postn[dkB])
			{
			play_adr[dkB]-= step_postn[dkB];
			if(step_postn[dkB]==1)
				{
				LR[dkB][0][0] = LR[dkB][0][1];
				LR[dkB][1][0] = LR[dkB][1][1];
				LR[dkB][0][1] = LR[dkB][0][2];
				LR[dkB][1][1] = LR[dkB][1][2];
				LR[dkB][0][2] = LR[dkB][0][3];
				LR[dkB][1][2] = LR[dkB][1][3];
				sdram_adr[dkB] = (play_adr[dkB])&0xFFFFF;	
				sdram_adr[dkB]+=	offset_adressBIG[dkB];		
				LR[dkB][0][3] = sdram.pcm[dkB][0][sdram_adr[dkB]][0];							
				LR[dkB][1][3] = sdram.pcm[dkB][0][sdram_adr[dkB]][1];		
				}
			else
				{
				sdram_adr[dkB] = play_adr[dkB]&0xFFFFF;	
				sdram_adr[dkB]+=	offset_adressBIG[dkB];		
				LR[dkB][0][3] = sdram.pcm[dkB][0][sdram_adr[dkB]][0];							
				LR[dkB][1][3] = sdram.pcm[dkB][0][sdram_adr[dkB]][1];
				sdram_adr[dkB] = (play_adr[dkB]+1)&0xFFFFF;
				sdram_adr[dkB]+=	offset_adressBIG[dkB];	
				LR[dkB][0][2] = sdram.pcm[dkB][0][sdram_adr[dkB]][0];								
				LR[dkB][1][2] = sdram.pcm[dkB][0][sdram_adr[dkB]][1];		
				sdram_adr[dkB] = (play_adr[dkB]+2)&0xFFFFF;
				sdram_adr[dkB]+=	offset_adressBIG[dkB];
				LR[dkB][0][1] = sdram.pcm[dkB][0][sdram_adr[dkB]][0];									
				LR[dkB][1][1] = sdram.pcm[dkB][0][sdram_adr[dkB]][1];
				sdram_adr[dkB] = (play_adr[dkB]+3)&0xFFFFF;
				sdram_adr[dkB]+=	offset_adressBIG[dkB];
				LR[dkB][0][0] = sdram.pcm[dkB][0][sdram_adr[dkB]][0];							
				LR[dkB][1][0] = sdram.pcm[dkB][0][sdram_adr[dkB]][1];			
				}	
			}	
		postn[dkB]%=10000;	
			
		even1 = LR[dkB][0][2];
		even1+= LR[dkB][0][1];
		odd1 = LR[dkB][0][2];
		odd1-= LR[dkB][0][1];
		even2 = LR[dkB][0][3];
		even2+= LR[dkB][0][0];
		odd2 = LR[dkB][0][3];
		odd2-= LR[dkB][0][0];
		c0l[dkB] = (float)even1*COEF[0];
		r0l[dkB] = (float)even2*COEF[1];
		c0l[dkB]+= r0l[dkB];
		c1l[dkB] = (float)odd1*COEF[2];
		r1l[dkB] = (float)odd2*COEF[3];
		c1l[dkB]+= r1l[dkB];
		c2l[dkB] = (float)even1*COEF[4]; 
		r2l[dkB] = (float)even2*COEF[5];
		c2l[dkB]+= r2l[dkB];
		c3l[dkB] = (float)odd1*COEF[6];
		r3l[dkB] = (float)odd2*COEF[7];
		c3l[dkB]+= r3l[dkB];	
			
		even1 = LR[dkB][1][2];
		even1+= LR[dkB][1][1];
		odd1 = LR[dkB][1][2];
		odd1-= LR[dkB][1][1];
		even2 = LR[dkB][1][3];
		even2+= LR[dkB][1][0]; 
		odd2 = LR[dkB][1][3];
		odd2-= LR[dkB][1][0];
		c0r[dkB] = (float)even1*COEF[0];
		r0r[dkB] = (float)even2*COEF[1];
		c0r[dkB]+= r0r[dkB];
		c1r[dkB] = (float)odd1*COEF[2];
		r1r[dkB] = (float)odd2*COEF[3];
		c1r[dkB]+= r1r[dkB];
		c2r[dkB] = (float)even1*COEF[4]; 
		r2r[dkB] = (float)even2*COEF[5];
		c2r[dkB]+= r2r[dkB];
		c3r[dkB] = (float)odd1*COEF[6];
		r3r[dkB] = (float)odd2*COEF[7];
		c3r[dkB]+= r3r[dkB];	
		}	
	T[dkB] = postn[dkB];
	T[dkB]/= 10000;
	T[dkB]-= 1/2.0F;
	SAMPLE_BUFFER = c0l[dkB]+T[dkB]*(c1l[dkB]+T[dkB]*(c2l[dkB]+T[dkB]*c3l[dkB]));
	firR[dkB][1] = firR[dkB][0];																								//////////////
	firR[dkB][0] = SAMPLE_BUFFER;																					//	FIR High Pass		
	deckout[dkB][0] = TRIM[dkB]*(FIRCoef[0]*firR[dkB][0] + FIRCoef[1]*firR[dkB][1]);			//	TRIM
	SAMPLE_BUFFER = c0r[dkB]+T[dkB]*(c1r[dkB]+T[dkB]*(c2r[dkB]+T[dkB]*c3r[dkB]));
	firL[dkB][1] = firL[dkB][0];																								//////////////
	firL[dkB][0] = SAMPLE_BUFFER;																					//	FIR High Pass		
	deckout[dkB][1] = TRIM[dkB]*(FIRCoef[0]*firL[dkB][0] + FIRCoef[1]*firL[dkB][1]);	
	//TEST_L;		
	}
	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



