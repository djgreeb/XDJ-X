/////////////////////////////////////////////////////////////////////////////////////////////////
// Audio processing handler 44100Hz
//	product: XDJ-X mixer assy
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////


int16_t ANTICLIP(float dt);		//convering float to int16 and anticlipping

typedef struct
	{
  // Filter #1 (Low band)
  double  lf;       // Frequency
  double  f1p0;     // Poles ...
  double  f1p1;
  double  f1p2;
  double  f1p3;

  // Filter #2 (High band)
  double  hf;       // Frequency
  double  f2p0;     // Poles ...
  double  f2p1;
  double  f2p2;
  double  f2p3;

  // Sample history buffer
  double  sdm1;     // Sample data minus 1
  double  sdm2;     //                   2
  double  sdm3;     //                   3

  // Gain Controls
  double  lg;       // low  gain
  double  mg;       // mid  gain
  double  hg;       // high gain
	} EQSTATE;

EQSTATE eql0;	
EQSTATE eqr0;	
EQSTATE eql1;	
EQSTATE eqr1;	

static double vsa = (1.0 / 4294967295.0);   // Very small amount (Denormal Fix)

// Recommended frequencies are ...
//
//  lowfreq  = 880  Hz
//  highfreq = 5000 Hz
//
// Set mixfreq to whatever rate your system is using (eg 48Khz)

void init_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq)
	{
  // Clear state
  memset(es,0,sizeof(EQSTATE));
  // Set Low/Mid/High gains to unity
  es->lg = 1.0;
  es->mg = 1.0;
  es->hg = 1.0;
  // Calculate filter cutoff frequencies
  es->lf = 2 * sin(PI * ((double)lowfreq / (double)mixfreq));
  es->hf = 2 * sin(PI * ((double)highfreq / (double)mixfreq));
	}
	
double do_3band(EQSTATE* es, double sample)
	{
  double  l,m,h;      // Low / Mid / High - Sample Values

  // Filter #1 (lowpass)
  es->f1p0  += (es->lf * (sample   - es->f1p0))+ vsa;
  es->f1p1  += (es->lf * (es->f1p0 - es->f1p1));
  es->f1p2  += (es->lf * (es->f1p1 - es->f1p2));
  es->f1p3  += (es->lf * (es->f1p2 - es->f1p3));
  l = es->f1p3;

  // Filter #2 (highpass)
  es->f2p0  += (es->hf * (sample   - es->f2p0)) + vsa;
  es->f2p1  += (es->hf * (es->f2p0 - es->f2p1));
  es->f2p2  += (es->hf * (es->f2p1 - es->f2p2));
  es->f2p3  += (es->hf * (es->f2p2 - es->f2p3));
  h = es->sdm3 - es->f2p3;

  // Calculate midrange (signal - (low + high))
 m = es->sdm3 - (h + l);

	//m = sample - (h + l);	
		
  // Scale, Combine and store
  l*= es->lg;
  m*= es->mg;
  h*= es->hg;

  // Shuffle history buffer
  es->sdm3 = es->sdm2;
  es->sdm2 = es->sdm1;
  es->sdm1 = sample;
  return(l + m + h);
	}
	
	
///////////////////////////////////////////////
//
//	CALC_CUTF_0	
//	
void CALC_CUTF_0(void)
	{
	fcutoff_0 = VR_VCFx116[pot_8b[CFX0]];		
	if(pot_8b[CFX0]!=128)
		{
		if(CFXON==8)
			{	
			resonanse_0 = VR_QFAC[pot_8b[PRMT]]*VR_VCF1minusKV[pot_8b[CFX0]];		
			}
		else
			{
			resonanse_0 = VR_QFAC[128]*VR_VCF1minusKV[pot_8b[CFX0]];			
			}			
		}
	else
		{	
		in1_0 = 0; in2_0 = 0; in3_0 = 0; in4_0 = 0; out1_0 = 0; out2_0 = 0; out3_0 = 0; out4_0 = 0;
		in1_1 = 0; in2_1 = 0; in3_1 = 0; in4_1 = 0; out1_1 = 0; out2_1 = 0; out3_1 = 0; out4_1 = 0;	
		in1_0h = 0; in2_0h = 0; in3_0h = 0; in4_0h = 0; out1_0h = 0; out2_0h = 0; out3_0h = 0; out4_0h = 0;
		in1_1h = 0; in2_1h = 0; in3_1h = 0; in4_1h = 0; out1_1h = 0; out2_1h = 0; out3_1h = 0; out4_1h = 0;				
		}		
	};

///////////////////////////////////////////////
//
//	CALC_CUTF_1	
//	
void CALC_CUTF_1(void)
	{
	fcutoff_1 = VR_VCFx116[pot_8b[CFX1]];	
	if(pot_8b[CFX1]!=128)
		{
		if(CFXON==8)
			{	
			resonanse_1 = VR_QFAC[pot_8b[PRMT]]*VR_VCF1minusKV[pot_8b[CFX1]];
			}
		else
			{
			resonanse_1 = VR_QFAC[128]*VR_VCF1minusKV[pot_8b[CFX1]];	
			}		
		}
	else
		{	
		in1_2 = 0; in2_2 = 0; in3_2 = 0; in4_2 = 0; out1_2 = 0; out2_2 = 0; out3_2 = 0; out4_2 = 0;
		in1_3 = 0; in2_3 = 0; in3_3 = 0; in4_3 = 0; out1_3 = 0; out2_3 = 0; out3_3 = 0; out4_3 = 0;	
		in1_2h = 0; in2_2h = 0; in3_2h = 0; in4_2h = 0; out1_2h = 0; out2_2h = 0; out3_2h = 0; out4_2h = 0;
		in1_3h = 0; in2_3h = 0; in3_3h = 0; in4_3h = 0; out1_3h = 0; out2_3h = 0; out3_3h = 0; out4_3h = 0;		
		}		
	};	
	
///////////////////////////////////////////////////////////////LPF FILTERS////////////////////////////////////////////////////
	
/////////////////////////////////	
//
// filter 
//		
float MoogVCF_0(float input_0, float f_0, float fb_0)
	{	
  input_0 -= out4_0 * fb_0;
  input_0 *= 0.35013 * (f_0*f_0)*(f_0*f_0);		
  out1_0 = input_0 + 0.3 * in1_0 + (1 - f_0) * out1_0; // Pole 1
  in1_0  = input_0;
  out2_0 = out1_0 + 0.3 * in2_0 + (1 - f_0) * out2_0;  // Pole 2
  in2_0  = out1_0;
  out3_0 = out2_0 + 0.3 * in3_0 + (1 - f_0) * out3_0;  // Pole 3
  in3_0  = out2_0;
  out4_0 = out3_0 + 0.3 * in4_0 + (1 - f_0) * out4_0;  // Pole 4
  in4_0  = out3_0;
	return curr_TRIM_FILTER*out4_0;	
	};	

/////////////////////////////////	
//
// filter 
//		
float MoogVCF_1(float input_1, float f_1, float fb_1)
	{
  input_1 -= out4_1 * fb_1;
  input_1 *= 0.35013 * (f_1*f_1)*(f_1*f_1);		
  out1_1 = input_1 + 0.3 * in1_1 + (1 - f_1) * out1_1; // Pole 1
  in1_1  = input_1;
  out2_1 = out1_1 + 0.3 * in2_1 + (1 - f_1) * out2_1;  // Pole 2
  in2_1  = out1_1;
  out3_1 = out2_1 + 0.3 * in3_1 + (1 - f_1) * out3_1;  // Pole 3
  in3_1  = out2_1;
  out4_1 = out3_1 + 0.3 * in4_1 + (1 - f_1) * out4_1;  // Pole 4
  in4_1  = out3_1;
	return curr_TRIM_FILTER*out4_1;	
	};		

/////////////////////////////////	
//
// filter 
//		
float MoogVCF_2(float input_2, float f_2, float fb_2)
	{
  input_2 -= out4_2 * fb_2;
  input_2 *= 0.35013 * (f_2*f_2)*(f_2*f_2);		
  out1_2 = input_2 + 0.3 * in1_2 + (1 - f_2) * out1_2; // Pole 1
  in1_2  = input_2;
  out2_2 = out1_2 + 0.3 * in2_2 + (1 - f_2) * out2_2;  // Pole 2
  in2_2  = out1_2;
  out3_2 = out2_2 + 0.3 * in3_2 + (1 - f_2) * out3_2;  // Pole 3
  in3_2  = out2_2;
  out4_2 = out3_2 + 0.3 * in4_2 + (1 - f_2) * out4_2;  // Pole 4
  in4_2  = out3_2;
	return curr_TRIM_FILTER*out4_2;	
	};			

/////////////////////////////////	
//
// filter 
//		
float MoogVCF_3(float input_3, float f_3, float fb_3)
	{
  input_3 -= out4_3 * fb_3;
  input_3 *= 0.35013 * (f_3*f_3)*(f_3*f_3);		
  out1_3 = input_3 + 0.3 * in1_3 + (1 - f_3) * out1_3; // Pole 1
  in1_3  = input_3;
  out2_3 = out1_3 + 0.3 * in2_3 + (1 - f_3) * out2_3;  // Pole 2
  in2_3  = out1_3;
  out3_3 = out2_3 + 0.3 * in3_3 + (1 - f_3) * out3_3;  // Pole 3
  in3_3  = out2_3;
  out4_3 = out3_3 + 0.3 * in4_3 + (1 - f_3) * out4_3;  // Pole 4
  in4_3  = out3_3;
	return curr_TRIM_FILTER*out4_3;	
	};			
	
	
///////////////////////////////////////////////////////////////HPF FILTERS////////////////////////////////////////////////////


/////////////////////////////////	
//
// filter 
//		
float MoogHVCF_0(float input_0h, float f_0h, float fb_0h)
	{
	tmpin_0 = input_0h;	
  input_0h -= out4_0h * fb_0h;
  input_0h *= 0.35013 * (f_0h*f_0h)*(f_0h*f_0h);		
  out1_0h = input_0h + 0.3 * in1_0h + (1 - f_0h) * out1_0h; // Pole 1
  in1_0h  = input_0h;
  out2_0h = out1_0h + 0.3 * in2_0h + (1 - f_0h) * out2_0h;  // Pole 2
  in2_0h  = out1_0h;
  out3_0h = out2_0h + 0.3 * in3_0h + (1 - f_0h) * out3_0h;  // Pole 3
  in3_0h  = out2_0h;
  out4_0h = out3_0h + 0.3 * in4_0h + (1 - f_0h) * out4_0h;  // Pole 4
  in4_0h  = out3_0h;
  return tmpin_0-(curr_TRIM_FILTER*out4_0h);
	};	

/////////////////////////////////	
//
// filter 
//		
float MoogHVCF_1(float input_1h, float f_1h, float fb_1h)
	{
	tmpin_1 = input_1h;	
  input_1h -= out4_1h * fb_1h;
  input_1h *= 0.35013 * (f_1h*f_1h)*(f_1h*f_1h);		
  out1_1h = input_1h + 0.3 * in1_1h + (1 - f_1h) * out1_1h; // Pole 1
  in1_1h  = input_1h;
  out2_1h = out1_1h + 0.3 * in2_1h + (1 - f_1h) * out2_1h;  // Pole 2
  in2_1h  = out1_1h;
  out3_1h = out2_1h + 0.3 * in3_1h + (1 - f_1h) * out3_1h;  // Pole 3
  in3_1h  = out2_1h;
  out4_1h = out3_1h + 0.3 * in4_1h + (1 - f_1h) * out4_1h;  // Pole 4
  in4_1h  = out3_1h;
  return tmpin_1-(curr_TRIM_FILTER*out4_1h);
	};		

/////////////////////////////////	
//
// filter 
//		
float MoogHVCF_2(float input_2h, float f_2h, float fb_2h)
	{
	tmpin_2 = input_2h;	
  input_2h -= out4_2h * fb_2h;
  input_2h *= 0.35013 * (f_2h*f_2h)*(f_2h*f_2h);		
  out1_2h = input_2h + 0.3 * in1_2h + (1 - f_2h) * out1_2h; // Pole 1
  in1_2h  = input_2h;
  out2_2h = out1_2h + 0.3 * in2_2h + (1 - f_2h) * out2_2h;  // Pole 2
  in2_2h  = out1_2h;
  out3_2h = out2_2h + 0.3 * in3_2h + (1 - f_2h) * out3_2h;  // Pole 3
  in3_2h  = out2_2h;
  out4_2h = out3_2h + 0.3 * in4_2h + (1 - f_2h) * out4_2h;  // Pole 4
  in4_2h  = out3_2h;
  return tmpin_2-(curr_TRIM_FILTER*out4_2h);
	};			

/////////////////////////////////	
//
// filter 
//		
float MoogHVCF_3(float input_3h, float f_3h, float fb_3h)
	{
	tmpin_3 = input_3h;	
  input_3h -= out4_3h * fb_3h;
  input_3h *= 0.35013 * (f_3h*f_3h)*(f_3h*f_3h);		
  out1_3h = input_3h + 0.3 * in1_3h+ (1 - f_3h) * out1_3h; // Pole 1
  in1_3h  = input_3h;
  out2_3h = out1_3h + 0.3 * in2_3h + (1 - f_3h) * out2_3h;  // Pole 2
  in2_3h  = out1_3h;
  out3_3h = out2_3h + 0.3 * in3_3h + (1 - f_3h) * out3_3h;  // Pole 3
  in3_3h  = out2_3h;
  out4_3h = out3_3h + 0.3 * in4_3h + (1 - f_3h) * out4_3h;  // Pole 4
  in4_3h  = out3_3h;
  return tmpin_3-(curr_TRIM_FILTER*out4_3h);
	};			
	
	
//////////////////////////////////////////////////////////
//	CFX process
//	
float CFX0r(float in)
	{
	if(CFXON==8)
		{
		if(pot_8b[CFX0]<128)	//LPF
			{
			return MoogVCF_0(in, fcutoff_0, resonanse_0);
			}	
		else if(pot_8b[CFX0]==128)		//disable filter
			{
			return in;	
			}
		else	//HPF
			{
			return MoogHVCF_0(in, fcutoff_0, resonanse_0);
			}	
		}
	else if(CFXON==6)
		{
		nsmpl++;	
		if(pot_8b[CFX0]<128)	//LPF
			{
			return in + NSLV[pot_8b[CFX0]]*MoogVCF_0(noise[(nsmpl+33000)&0xFFFF]*pot_8b[PRMT], fcutoff_0, resonanse_0);
			}	
		else if(pot_8b[CFX0]==128)		//disable filter
			{
			return in;	
			}
		else	//HPF
			{
			return in + NSLV[pot_8b[CFX0]]*MoogHVCF_0(noise[(nsmpl+33000)&0xFFFF]*pot_8b[PRMT], fcutoff_0, resonanse_0);
			}		
		}	
	else
		{
		return in;	
		}
	};	

//////////////////////////////////////////////////////////
//	CFX process
//	
float CFX0l(float in)
	{
	if(CFXON==8)
		{
		if(pot_8b[CFX0]<128)	//LPF
			{
			return MoogVCF_1(in, fcutoff_0, resonanse_0);
			}	
		else if(pot_8b[CFX0]==128)		//disable filter
			{
			return in;	
			}
		else	//HPF
			{
			return MoogHVCF_1(in, fcutoff_0, resonanse_0);
			}	
		}
	else if(CFXON==6)
		{
		if(pot_8b[CFX0]<128)	//LPF
			{
			return in + NSLV[pot_8b[CFX0]]*MoogVCF_1(noise[nsmpl]*pot_8b[PRMT], fcutoff_0, resonanse_0);
			}	
		else if(pot_8b[CFX0]==128)		//disable filter
			{
			return in;	
			}
		else	//HPF
			{
			return in + NSLV[pot_8b[CFX0]]*MoogHVCF_1(noise[nsmpl]*pot_8b[PRMT], fcutoff_0, resonanse_0);
			}		
		}	
	else
		{
		return in;	
		}
	};	
	
//////////////////////////////////////////////////////////
//	CFX process
//	
float CFX1r(float in)
	{
	if(CFXON==8)
		{
		if(pot_8b[CFX1]<128)	//LPF
			{
			return MoogVCF_2(in, fcutoff_1, resonanse_1);
			}	
		else if(pot_8b[CFX1]==128)		//disable filter
			{
			return in;	
			}
		else	//HPF
			{
			return MoogHVCF_2(in, fcutoff_1, resonanse_1);
			}	
		}
	else if(CFXON==6)
		{	
		if(pot_8b[CFX1]<128)	//LPF
			{
			return in + NSLV[pot_8b[CFX1]]*MoogVCF_2(noise[(nsmpl+33000)&0xFFFF]*pot_8b[PRMT], fcutoff_1, resonanse_1);
			}	
		else if(pot_8b[CFX1]==128)		//disable filter
			{
			return in;	
			}
		else	//HPF
			{
			return in + NSLV[pot_8b[CFX1]]*MoogHVCF_2(noise[(nsmpl+33000)&0xFFFF]*pot_8b[PRMT], fcutoff_1, resonanse_1);
			}		
		}	
	else
		{
		return in;	
		}
	};	
	
//////////////////////////////////////////////////////////
//	CFX process
//	
float CFX1l(float in)
	{
	if(CFXON==8)
		{
		if(pot_8b[CFX1]<128)	//LPF
			{
			return MoogVCF_3(in, fcutoff_1, resonanse_1);
			}	
		else if(pot_8b[CFX1]==128)		//disable filter
			{
			return in;	
			}
		else	//HPF
			{
			return MoogHVCF_3(in, fcutoff_1, resonanse_1);
			}	
		}
	else if(CFXON==6)
		{	
		if(pot_8b[CFX1]<128)	//LPF
			{
			return in + NSLV[pot_8b[CFX1]]*MoogVCF_3(noise[nsmpl]*pot_8b[PRMT], fcutoff_1, resonanse_1);
			}	
		else if(pot_8b[CFX1]==128)		//disable filter
			{
			return in;	
			}
		else	//HPF
			{
			return in + NSLV[pot_8b[CFX1]]*MoogHVCF_3(noise[nsmpl]*pot_8b[PRMT], fcutoff_1, resonanse_1);
			}	
		}	
	else
		{
		return in;	
		}
	};	
	
	
	

//////////////////////////////////////////////////////////
//
//					DAC1 HANDLER (HEADPHONES)
//
void SAI1_IRQHandler(void)
	{	
	HAL_SAI_IRQHandler(&hsai_BlockA1);

	//TRIM+EQ		
	POSTEQCH0[0] = do_3band(&eql0, CFX0r(CH0IN[0]*TRM0_ATT));	
	POSTEQCH0[1] = do_3band(&eqr0, CFX0l(CH0IN[1]*TRM0_ATT));	
	POSTEQCH1[0] = do_3band(&eql1, CFX1r(CH1IN[0]*TRM1_ATT));	
	POSTEQCH1[1] = do_3band(&eqr1, CFX1l(CH1IN[1]*TRM1_ATT));	
	
	if(POSTEQCH0[0]<0)
		{
		lvl_tmp = 0-POSTEQCH0[0];	
		}
	else
		{
		lvl_tmp = POSTEQCH0[0];	
		}
	if(lvl_tmp>lvl_ch0)
		{
		lvl_ch0 = lvl_tmp;	
		}
		
	if(POSTEQCH0[1]<0)
		{
		lvl_tmp = 0-POSTEQCH0[1];	
		}
	else
		{
		lvl_tmp = POSTEQCH0[1];	
		}
	if(lvl_tmp>lvl_ch0)
		{
		lvl_ch0 = lvl_tmp;	
		}
		
	if(POSTEQCH1[0]<0)
		{
		lvl_tmp = 0-POSTEQCH1[0];	
		}
	else
		{
		lvl_tmp = POSTEQCH1[0];	
		}
	if(lvl_tmp>lvl_ch1)
		{
		lvl_ch1 = lvl_tmp;	
		}
		
	if(POSTEQCH1[1]<0)
		{
		lvl_tmp = 0-POSTEQCH1[1];	
		}
	else
		{
		lvl_tmp = POSTEQCH1[1];	
		}
	if(lvl_tmp>lvl_ch1)
		{
		lvl_ch1 = lvl_tmp;	
		}	
				
	if(CUE1ON)							///calc CUE1 + CUE2
		{
		CUESUM[0] = POSTEQCH0[0]; 
		CUESUM[1] = POSTEQCH0[1];	
		}
	else		
		{
		CUESUM[0] = 0.000; 
		CUESUM[1] = 0.000;		
		}
	if(CUE2ON)
		{
		CUESUM[0]+= POSTEQCH1[0]; 
		CUESUM[1]+= POSTEQCH1[1];	
		}	
	CUESUM[0]*= MIXN1_ATT; 
	CUESUM[1]*= MIXN1_ATT;		
		
		
	//faders
	POSTEQCH0[0]*=FDR0_ATT;	
	POSTEQCH0[1]*=FDR0_ATT;
	POSTEQCH1[0]*=FDR1_ATT;	
	POSTEQCH1[1]*=FDR1_ATT;
	//crossfader
	POSTEQCH0[0]*=CRSF0_ATT;	
	POSTEQCH0[1]*=CRSF0_ATT;
	POSTEQCH1[0]*=CRSF1_ATT;	
	POSTEQCH1[1]*=CRSF1_ATT;	
	//summ	
	POSTEQCH0[0]+=POSTEQCH1[0];	
	POSTEQCH0[1]+=POSTEQCH1[1];
		
		
	
	if(BFXON==1)
		{
		rngaddr++;	
		if(rngaddr>=rngsize)
			{
			rngaddr = 0;	
			}	
			
		if(BFXn==1)			/////////////////	ECHO	/////////////////////
			{
			BFXTMP[0] = ORIGATT*POSTEQCH0[0];
			BFXTMP[1] = ORIGATT*POSTEQCH0[1];	
			POSTEQCH0[0]+= 1.3*RINGBUF[0][rngaddr];	
			POSTEQCH0[1]+= 1.3*RINGBUF[1][rngaddr];	
			BFXTMP[0]+= EFFATT*RINGBUF[0][rngaddr];
			BFXTMP[1]+= EFFATT*RINGBUF[1][rngaddr];		
			RINGBUF[0][rngaddr] = ANTICLIP(POSTEQCH0[0]/2);
			RINGBUF[1][rngaddr] = ANTICLIP(POSTEQCH0[1]/2);				
			POSTEQCH0[0] = BFXTMP[0]; 	
			POSTEQCH0[1] = BFXTMP[1];		
			}			
		else if(BFXn==0)			/////////////////	DELAY	/////////////////////
			{
			BFXTMP[0] = EFFATT*RINGBUF[0][rngaddr];	
			BFXTMP[1] = EFFATT*RINGBUF[1][rngaddr];	
			RINGBUF[0][rngaddr] = ANTICLIP(POSTEQCH0[0]/2);
			RINGBUF[1][rngaddr] = ANTICLIP(POSTEQCH0[1]/2);		
			POSTEQCH0[0] = ORIGATT*POSTEQCH0[0] + BFXTMP[0];
			POSTEQCH0[1] = ORIGATT*POSTEQCH0[1] + BFXTMP[1];					
			}
		else if(BFXn==4)			/////////////////	FLANGER	/////////////////////
			{	
			BFXTMP[0] = ORIGATT*POSTEQCH0[0];
			BFXTMP[1] = ORIGATT*POSTEQCH0[1];	
			POSTEQCH0[0]+= 1.7*RINGBUF[0][rngaddr];	
			POSTEQCH0[1]+= 1.7*RINGBUF[1][rngaddr];	
			BFXTMP[0]+= EFFATT*RINGBUF[0][rngaddr];
			BFXTMP[1]+= EFFATT*RINGBUF[1][rngaddr];		
			RINGBUF[0][rngaddr] = ANTICLIP(POSTEQCH0[0]/2);
			RINGBUF[1][rngaddr] = ANTICLIP(POSTEQCH0[1]/2);				
			POSTEQCH0[0] = BFXTMP[0]; 	
			POSTEQCH0[1] = BFXTMP[1];		
			}		
		else if(BFXn==3)			/////////////////	TRANS	/////////////////////
			{
			POSTEQCH0[0]*= ORIGATT;
			POSTEQCH0[1]*= ORIGATT;
			}	
		}
		
		
	//summ	cue
	CUESUM[0]+= (MIXN0_ATT*POSTEQCH0[0]); 
	CUESUM[1]+= (MIXN0_ATT*POSTEQCH0[1]);		
	//headphone level
	CUESUM[0]*= HPHN_ATT; 
	CUESUM[1]*= HPHN_ATT;
	
	HP_BUS[0] = ANTICLIP(CUESUM[0]);
	HP_BUS[1] = ANTICLIP(CUESUM[1]);
	
	MASTER_BUS[0] = ANTICLIP(POSTEQCH0[0]);
	MASTER_BUS[1] = ANTICLIP(POSTEQCH0[1]);		
		
	if(MASTER_BUS[0]<0)
		{
		lvl_tmpm = 0-MASTER_BUS[0];	
		}
	else
		{
		lvl_tmpm = MASTER_BUS[0];	
		}
	if(lvl_tmpm>lvl_ml)
		{
		lvl_ml = lvl_tmpm;	
		}	
		
	if(MASTER_BUS[1]<0)
		{
		lvl_tmpm = 0-MASTER_BUS[1];	
		}
	else
		{
		lvl_tmpm = MASTER_BUS[1];	
		}
	if(lvl_tmpm>lvl_mr)
		{
		lvl_mr = lvl_tmpm;	
		}	
	HAL_SAI_Transmit_IT(&hsai_BlockA1, HP_BUS, 2);
	//HAL_SAI_Transmit_IT(&hsai_BlockA1, &SINED[2*rs], 2);	
	//HAL_SAI_Transmit_IT(&hsai_BlockA1, CH1IN, 2);			
	}
	
	
//////////////////////////////////////////////////////////
//
//					DAC2 HANDLER (MASTER)
//
void SAI2_IRQHandler(void)
	{
  HAL_SAI_IRQHandler(&hsai_BlockA2);
	//HAL_SAI_Transmit_IT(&hsai_BlockA2, HP_BUS, 2);		
	//HAL_SAI_Transmit_IT(&hsai_BlockA2, &SINED[2*rs], 2);
	HAL_SAI_Transmit_IT(&hsai_BlockA2, MASTER_BUS, 2);	
	}	

	
//////////////////////////////////////////////////////////
//
//					ADCs HANDLER
//	
void SAI3_IRQHandler(void)
	{
	HAL_SAI_IRQHandler(&hsai_BlockA3);	
	HAL_SAI_Receive_IT(&hsai_BlockA3, (uint8_t*)CH0IN, 2);
	HAL_SAI_IRQHandler(&hsai_BlockB3);	
	HAL_SAI_Receive_IT(&hsai_BlockB3, (uint8_t*)CH1IN, 2);	
	}	
	
	
//////////////////////////////////////////////////////////
//
//		convering float to int16 and anticlipping	
//	
int16_t ANTICLIP(float dt)
	{
	if(dt>32767.0)
		{
		return 32767;
		}	
	else if(dt<-32768.0)			
		{
		return -32768;	
		}
	else
		{
		return (int)dt;	
		}			
	};
	
	
//////////////////////////////////////////////////////////
//
//		calculate ring buffer size
//	
CHANGE_RNGSIZE(void)
	{
	uint32_t sz;	
	if(BFXn==4)	//FLANGER
		{
		return;	
		}	
	sz = 2116800000/BPM_link;	
	sz>>=(7-rngdiv[BFXn]);
	if(sz<=rngmaxsize)
		{
		rngsize = sz;	
		}		
	else
		{
		rngsize = rngmaxsize;	
		}
	need_draw_ms = 1;	
	return;	
	};	
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



