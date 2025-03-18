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
			deckTbuf[dbg_urx_buf[1]] = dbg_urx_buf[2];
			sprintf((char*)U_TX_DATA, "done\n\r");	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 6, 5);		
			}
		else if(dbg_urx_buf[0]==1)
			{
			HAL_UART_Transmit(&huart4, deckRbuf, 20, 5);	
			}
		else if(dbg_urx_buf[0]==2)			//sai sample
			{
			SAMPLE[dbg_urx_buf[1]] = dbg_urx_buf[2];
			sprintf((char*)U_TX_DATA, "SAI\n\r");	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 5, 5);			
			}
		else if(dbg_urx_buf[0]==3)			//deckRbuf	
			{
			HAL_UART_Transmit(&huart4, deckRbuf, 20, 5);	
			}
		dbg_new_data = 0;		
		};
	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



