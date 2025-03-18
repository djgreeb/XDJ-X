/////////////////////////////////////////////////////////////////////////////////////////////////
// 	UARTs IRQs
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
//	MIXER HANDLER
//
void USART1_IRQHandler(void)
	{
	if(USART1->ISR & USART_ISR_RXNE_RXFNE)		//have a new data
		{		
		mxrURX = (USART1->RDR);	
		mxr_current_time = HAL_GetTick();	
		if((mxr_current_time - mxr_usart_timeout)>100)					//5ms for hardware COM. 100ms for usb-com adapter
			{
			mxr_usart_data_cnt = 0;
			}
		mxr_usart_timeout = mxr_current_time;	

		if(mxr_usart_data_cnt<16)
			{			
			mxr_urx_buf[mxr_usart_data_cnt] = mxrURX;	
			if(mxr_usart_data_cnt==1)
				{			
				mxr_new_data = 1;
				}
			}
		mxr_usart_data_cnt++;
		}
  HAL_UART_IRQHandler(&huart1);
	}



//////////////////////////////////////////////////////////
//
//	Debug Handler
//
void UART4_IRQHandler(void)
	{
	if(UART4->ISR & USART_ISR_RXNE_RXFNE)		//have a new data
		{		
		dbgURX = (UART4->RDR);	
		dbg_current_time = HAL_GetTick();	
		if((dbg_current_time - dbg_usart_timeout)>100)					//5ms for hardware COM. 100ms for usb-com adapter
			{
			dbg_usart_data_cnt = 0;
			}
		dbg_usart_timeout = dbg_current_time;	
		if(dbg_usart_data_cnt<16)
			{			
			dbg_urx_buf[dbg_usart_data_cnt] = dbgURX;	
			if(dbg_usart_data_cnt==1)
				{			
				dbg_new_data = 1;
				}
			}
		dbg_usart_data_cnt++;
		}	
  HAL_UART_IRQHandler(&huart4);
	};


//////////////////////////////////////////////////////////
//
//		Power Manager
//
void UART7_IRQHandler(void)
	{		
	if(UART7->ISR & USART_ISR_RXNE_RXFNE)		//have a new data
		{		
		pmURX = (UART7->RDR);
		pm_current_time = HAL_GetTick();	
		if((pm_current_time - pm_usart_timeout)>10)					//5ms for hardware COM. 100ms for usb-com adapter
			{
			pm_usart_data_cnt = 0;	
			}
		pm_usart_timeout = pm_current_time;	
		if(pm_usart_data_cnt<2)
			{			
			pm_urx_buf[pm_usart_data_cnt] = pmURX;
			if(pm_usart_data_cnt==1)
				{			
				pm_new_data = 1;
				}				
			}
		pm_usart_data_cnt++;
		}
  HAL_UART_IRQHandler(&huart7);
	};
	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



