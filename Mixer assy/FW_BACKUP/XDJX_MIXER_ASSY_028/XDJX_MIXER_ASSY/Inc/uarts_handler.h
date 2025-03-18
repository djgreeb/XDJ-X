/////////////////////////////////////////////////////////////////////////////////////////////////
//	uarts_handler
//	product: XDJ-X mixer assy
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////
//
//	Main assy USART transfer handler
//
void UART5_IRQHandler(void)
	{
	if(UART5->ISR & USART_ISR_RXNE_RXFNE)		//have a new data
		{		
		linkURX = (UART5->RDR);	
		link_current_time = HAL_GetTick();	
		if((link_current_time - link_usart_timeout)>20)					//5ms for hardware COM. 100ms for usb-com adapter
			{
			link_usart_data_cnt = 0;
			}
		link_usart_timeout = link_current_time;	
		if(link_usart_data_cnt<16)
			{			
			link_urx_buf[link_usart_data_cnt] = linkURX;	
			if(link_usart_data_cnt==1)
				{			
				link_new_data = 1;
				}
			}
		link_usart_data_cnt++;
		}		
  HAL_UART_IRQHandler(&huart5);
	};


////////////////////////////////////////////////////////////////////
//
//	Crossfader USART handler
//
void USART1_IRQHandler(void)
	{
	if(USART1->ISR & USART_ISR_RXNE_RXFNE)		//have a new data
		{		
		CRSF_midi = (USART1->RDR);		//0..127				
		}				
  HAL_UART_IRQHandler(&huart1);
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



