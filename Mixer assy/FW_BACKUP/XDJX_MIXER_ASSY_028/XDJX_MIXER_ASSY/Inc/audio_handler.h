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



//////////////////////////////////////////////////////////
//
//					DAC1 HANDLER
//
void SAI1_IRQHandler(void)
	{	
	HAL_SAI_IRQHandler(&hsai_BlockA1);
	if(rs<19)
		{
		rs++;	
		}	
	else
		{
		rs = 0;	
		}		
	//HAL_SAI_Transmit_IT(&hsai_BlockA1, MASTER_BUS, 2);
	HAL_SAI_Transmit_IT(&hsai_BlockA1, &SINED[2*rs], 2);		
	}
	
	
//////////////////////////////////////////////////////////
//
//					DAC2 HANDLER (HEADPHONES)
//
void SAI2_IRQHandler(void)
	{
  HAL_SAI_IRQHandler(&hsai_BlockA2);
	//HAL_SAI_Transmit_IT(&hsai_BlockA2, HP_BUS, 2);		
	HAL_SAI_Transmit_IT(&hsai_BlockA2, &SINED[2*rs], 2);	
	}	

	
	
//////////////////////////////////////////////////////////
//
//					ADCs HANDLER
//	
void SAI3_IRQHandler(void)
	{
	HAL_SAI_IRQHandler(&hsai_BlockA3);	
	HAL_SAI_Receive_IT(&hsai_BlockA3, CH2IN, 2);
	HAL_SAI_IRQHandler(&hsai_BlockB3);	
	HAL_SAI_Receive_IT(&hsai_BlockB3, CH1IN, 2);	
	}	
	
	
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



