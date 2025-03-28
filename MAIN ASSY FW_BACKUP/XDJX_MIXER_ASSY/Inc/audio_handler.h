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



//////////////////////////////////////////////////////////
//
//					DAC1 HANDLER (HEADPHONES)
//
void SAI1_IRQHandler(void)
	{	
	HAL_SAI_IRQHandler(&hsai_BlockA1);
//	if(rs<19)
//		{
//		rs++;	
//		}	
//	else
//		{
//		rs = 0;	
//		}
	//TRIM
	POSTEQCH0[0] = CH0IN[0]*TRM0_ATT;
	POSTEQCH0[1] = CH0IN[1]*TRM0_ATT;
	POSTEQCH1[0] = CH1IN[0]*TRM1_ATT;
	POSTEQCH1[1] = CH1IN[1]*TRM1_ATT;
		
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
	HAL_SAI_Receive_IT(&hsai_BlockA3, CH0IN, 2);
	HAL_SAI_IRQHandler(&hsai_BlockB3);	
	HAL_SAI_Receive_IT(&hsai_BlockB3, CH1IN, 2);	
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



