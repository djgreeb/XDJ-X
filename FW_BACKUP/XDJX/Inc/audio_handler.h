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



//////////////////////////////////////////////////////////
//
//			AUDIO HANDLER
//
void SAI1_IRQHandler(void)
	{		
	HAL_SAI_IRQHandler(&hsai_BlockA1);
  HAL_SAI_IRQHandler(&hsai_BlockB1);

	if(rs<19)
		{
		rs++;	
		}	
	else
		{
		rs = 0;	
		}	



	if(rs1<49)
		{
		rs1++;	
		}	
	else
		{
		rs1 = 0;	
		}			

		
	if(s<11000)
		{
		s++;	
		}	
	else
		{
		s = 0;	
		}		
		
		
	//SINED882[4*rs1+2] = REKBX[s][s%3]; 

	SINED[2] = audio[4*(play_adr%7945)] + 0x100*audio[4*(play_adr%7945)+1];
	SINED[3] = audio[4*(play_adr%7945)+2] + 0x100*audio[4*(play_adr%7945)+3];	
		
	HAL_SAI_Transmit_IT(&hsai_BlockA1, (uint8_t*)&SINED882[4*rs1], 2);
	HAL_SAI_Transmit_IT(&hsai_BlockB1, (uint8_t*)&SINED[0], 2);	
	play_adr++;	
	if(play_adr>6000000)
		{
		play_adr = 500000;	
		}	
	}
	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



