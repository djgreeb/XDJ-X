/////////////////////////////////////////////////////////////////////////////////////////////////
//  Encoder handler
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////
//
//	encoder interrupt
//
void TIM3_IRQHandler(void)
	{
	uint32_t TM, CTM;
	CTM = HAL_GetTick();	
	TM = CTM - enc_prev_time;	
	if(TIM3->CR1 & TIM_CR1_DIR)
		{
		if((TM>12 && enc_prev_dir==1) || (TM>100 && enc_prev_dir==0))	
			{
			enc_prev_time = CTM;
			enc_need_down = 1;		
			enc_prev_dir = 1;		
			}
		}
	else
		{
		if((TM>12 && enc_prev_dir==0) || (TM>100 && enc_prev_dir==1))	
			{	
			enc_prev_time = CTM; 	
			enc_need_up = 1;
			enc_prev_dir = 0;		
			}					
		}				
  HAL_TIM_IRQHandler(&htim3);
	}
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



