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
			HAL_UART_Transmit(&huart4, deckRbuf, 16, 15);	
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
			}
		else if(dbg_urx_buf[0]==2)			//ring color
			{				
			SET_JOG_COLOR(0, 0x10000*dbg_urx_buf[1]+0x100*dbg_urx_buf[2]+dbg_urx_buf[3]);	
			SET_JOG_COLOR(1, 0x10000*dbg_urx_buf[1]+0x100*dbg_urx_buf[2]+dbg_urx_buf[3]);
			}
		else if(dbg_urx_buf[0]==3)
			{
			if(dbg_urx_buf[1]==0)		//pitch A
				{
				sprintf((char*)U_TX_DATA, "%04lu\n\r", 0x100*(deckRbuf[2]&0x0F)+deckRbuf[3]);
				HAL_UART_Transmit(&huart4, U_TX_DATA, 6, 5);						
				}				
			else if(dbg_urx_buf[1]==1)		//jog A cnt
				{
				sprintf((char*)U_TX_DATA, "%05lu\n\r", (0x100*deckRbuf[5])+deckRbuf[6]);
				HAL_UART_Transmit(&huart4, U_TX_DATA, 7, 5);						
				}
			else if(dbg_urx_buf[1]==2)		//pitch B
				{
				sprintf((char*)U_TX_DATA, "%04lu\n\r", 0x100*(deckRbuf[10]&0x0F)+deckRbuf[11]);
				HAL_UART_Transmit(&huart4, U_TX_DATA, 6, 5);						
				}				
			else if(dbg_urx_buf[1]==3)		//jog B cnt
				{
				sprintf((char*)U_TX_DATA, "%05lu\n\r", (0x100*deckRbuf[13])+deckRbuf[14]);
				HAL_UART_Transmit(&huart4, U_TX_DATA, 7, 5);						
				}
			else if(dbg_urx_buf[1]==4)		//jog A dir
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[4]);
				HAL_UART_Transmit(&huart4, U_TX_DATA, 5, 5);						
				}
			else if(dbg_urx_buf[1]==5)		//jog B dir
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[12]);
				HAL_UART_Transmit(&huart4, U_TX_DATA, 5, 5);						
				}
			else if(dbg_urx_buf[1]==6)		//A buttons 2
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[2]);
				HAL_UART_Transmit(&huart4, U_TX_DATA, 5, 5);						
				}
			else if(dbg_urx_buf[1]==7)		//B buttons 2
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[10]);
				HAL_UART_Transmit(&huart4, U_TX_DATA, 5, 5);						
				}
			else if(dbg_urx_buf[1]==8)		//A buttons 1
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[1]);
				HAL_UART_Transmit(&huart4, U_TX_DATA, 5, 5);						
				}
			else if(dbg_urx_buf[1]==9)		//B buttons 1
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[9]);
				HAL_UART_Transmit(&huart4, U_TX_DATA, 5, 5);						
				}
			}	
		else if(dbg_urx_buf[0]==4)			//load track
			{	
			PREPARE_LOAD_TRACK(dbg_urx_buf[1], dbg_urx_buf[2]);	
			}
			
		dbg_new_data = 0;		
		};
	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



