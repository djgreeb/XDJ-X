/////////////////////////////////////////////////////////////////////////////////////////////////
// 	Debug uart handler 
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

	if(dbg_new_data)									////////////////////////usart data descriptor-handler
		{
		HAL_Delay(1);
		if(dbg_urx_buf[0]==0)
			{
			deckTbuf[0][dbg_urx_buf[1]] = dbg_urx_buf[2];
			sprintf((char*)U_TX_DATA, "done\n\r");	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 6, 5);		
			}
		else if(dbg_urx_buf[0]==1)
			{
			HAL_UART_Transmit(&huart4, deckRbuf, 18, 15);	
			deckRbuf[0] = 0xFF;	
			deckRbuf[1] = 0xFF;
			deckRbuf[2] = 0xFF;
			deckRbuf[3] = 0xFF;
			deckRbuf[4] = 0xFF;	
			deckRbuf[5] = 0xFF;	
			deckRbuf[6] = 0xFF;
			deckRbuf[7] = 0xFF;
			deckRbuf[8] = 0xFF;
			deckRbuf[9] = 0xFF;
			deckRbuf[10] = 0xFF;	
			deckRbuf[11] = 0xFF;
			deckRbuf[12] = 0xFF;
			deckRbuf[13] = 0xFF;
			deckRbuf[14] = 0xFF;	
			deckRbuf[15] = 0xFF;	
			deckRbuf[16] = 0xFF;
			deckRbuf[17] = 0xFF;			
			}
		else if(dbg_urx_buf[0]==2)			//sai sample
			{
			SAMPLE_0[dbg_urx_buf[1]] = dbg_urx_buf[2];
			sprintf((char*)U_TX_DATA, "SAI\n\r");	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 5, 5);			
			}
		else if(dbg_urx_buf[0]==3)
			{
			HAL_UART_Transmit(&huart4, &rslt, 1, 5);	
			}	
			
		dbg_new_data = 0;		
		};
	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



