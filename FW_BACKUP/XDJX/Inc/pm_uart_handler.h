/////////////////////////////////////////////////////////////////////////////////////////////////
// 	Power manager uart handler 
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

	if(pm_new_data)									////////////////////////usart data descriptor-handler
		{
		if(pm_urx_buf[0]==0xA1 || pm_urx_buf[0]==0xA2 || pm_urx_buf[0]==0xA3)
			{
			if(pm_urx_buf[1]==0xFC)			//need turn off 
				{
				f_close(&file);	
				TIM8->CCR1 = 0;
				HAL_GPIO_WritePin(BACKLIGHT_EN_GPIO_Port, BACKLIGHT_EN_Pin, GPIO_PIN_RESET);	
				HAL_Delay(100);	
				sprintf((char*)U_TX_DATA, "Turn off\n\r", i);	
				HAL_UART_Transmit(&huart4, U_TX_DATA, 10, 5);						
				HAL_Delay(3000);
				U_TX_DATA[0] = 0xA1;	
				U_TX_DATA[1] = 0xFC;	
				HAL_UART_Transmit(&huart7, U_TX_DATA, 2, 5);	
				HAL_Delay(1000);	
				HAL_UART_Transmit(&huart7, U_TX_DATA, 2, 5);	
				HAL_Delay(1000);
				HAL_UART_Transmit(&huart7, U_TX_DATA, 2, 5);						
				}
			}	
//		sprintf((char*)&U_TX_DATA[0], "PM: ");
//		memcpy(&U_TX_DATA[5], pm_urx_buf, 2);	
//		sprintf((char*)&U_TX_DATA[7], "\n\r");	
//		HAL_UART_Transmit(&huart4, U_TX_DATA, 8, 5);		
		pm_new_data = 0;		
		};



	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



