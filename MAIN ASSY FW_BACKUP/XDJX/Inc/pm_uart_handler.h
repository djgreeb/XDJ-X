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
			if(batt_status!=pm_urx_buf[0])
				{
				batt_status = pm_urx_buf[0];
				batt_need_update|=0xF0;	
				}				
			if(pm_urx_buf[1]<101)
				{
				if(batt_prcnt!=((pm_urx_buf[1]+5)/10))
					{
					batt_prcnt = ((pm_urx_buf[1]+5)/10);	
					batt_need_update|=0x0F;		
					}
				}
			else if(pm_urx_buf[1]==0xFC)			//need turn off 
				{
				f_close(&file);	
				TIM8->CCR1 = 0;
				HAL_GPIO_WritePin(BACKLIGHT_EN_GPIO_Port, BACKLIGHT_EN_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, LED_LOAD1_Pin|LED_LOAD0_Pin|LED_ENC_Pin, GPIO_PIN_RESET);
				for(j=0;j<19;j++)		//write to eeprom
					{		
					WR_EEMEM(j, UT_SET[j]);
					}		
				HAL_GPIO_WritePin(GPIOH, P8EN_Pin, GPIO_PIN_RESET);			
				lock_control[dkA] = 1;		
				pitch[dkA] = 0;	
				play_enable[dkA] = 0;					
				play_adr[dkA] = 0;	
				all_long[dkA] = 0;		
				end_adata[dkA] = 0xFFFF;
				U_TX_DATA[0] = 0x97;			//Turn off mixer
				U_TX_DATA[1] = 0xFC;			//
				U_TX_DATA[2] = 0xFC;			//
				UART_TX(&huart1, U_TX_DATA, 3, 5);
				#if defined(DEBUG_UART_EN)	
				sprintf((char*)U_TX_DATA, "Turn off\n\r");	
				UART_TX(&huart4, U_TX_DATA, 10, 5);
				#endif
				HAL_Delay(200);
				U_TX_DATA[0] = 0x97;			//Turn off mixer
				U_TX_DATA[1] = 0xFC;			//
				U_TX_DATA[2] = 0xFC;	
				UART_TX(&huart1, U_TX_DATA, 3, 5);	
				HAL_Delay(2800);
				U_TX_DATA[0] = 0xA1;	
				U_TX_DATA[1] = 0xFC;	
				UART_TX(&huart7, U_TX_DATA, 2, 5);	
				HAL_Delay(1000);	
				UART_TX(&huart7, U_TX_DATA, 2, 5);	
				HAL_Delay(1000);
				UART_TX(&huart7, U_TX_DATA, 2, 5);						
				}
			}	
		else
			{
			//firmware	
			}			
//		sprintf((char*)&U_TX_DATA[0], "PM: ");
//		sprintf((char*)&U_TX_DATA[4], "%04lu\n\r", pm_urx_buf[1]);	
//		UART_TX(&huart4, U_TX_DATA, 10, 5);		
		pm_new_data = 0;		
		};



	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



