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
  es->f1p0  += (es->lf * (sample   - es->f1p0)) + vsa;
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
	
	
	
	
	
	

//////////////////////////////////////////////////////////
//
//					DAC1 HANDLER (HEADPHONES)
//
void SAI1_IRQHandler(void)
	{	
	HAL_SAI_IRQHandler(&hsai_BlockA1);

	//TRIM +EQ		
	POSTEQCH0[0] = do_3band(&eql0, CH0IN[0]*TRM0_ATT);	
	POSTEQCH0[1] = do_3band(&eqr0, CH0IN[1]*TRM0_ATT);	
	POSTEQCH1[0] = do_3band(&eql1, CH1IN[0]*TRM1_ATT);	
	POSTEQCH1[1] = do_3band(&eqr1, CH1IN[1]*TRM1_ATT);	
	
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
		lvl_ch1
			= lvl_tmp;	
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
	//summ	
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
	
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



