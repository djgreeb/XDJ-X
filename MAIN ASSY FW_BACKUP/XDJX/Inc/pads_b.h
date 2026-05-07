/////////////////////////////////////////////////////////////////////////////////////////////////
//  PADS B handler
//	processing of buttons, jog, pitch
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////



else if((deckRbuf[8]&0x01) && PAD_pressed[dkB][0]==0)		///////////PAD 0
	{
	if(lock_control[dkB]==0)	
		{	
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "PAD B0\n\r");	
		UART_TX(&huart4, U_TX_DATA, 8, 5);	
		#endif	
		}
	PAD_pressed[dkB][0] = 1;		
	}
else if((deckRbuf[8]&0x01)==0 && PAD_pressed[dkB][0]==1)
	{
	PAD_pressed[dkB][0] = 0;	
	}
else if((deckRbuf[8]&0x02) && PAD_pressed[dkB][1]==0)		///////////PAD 1
	{
	if(lock_control[dkB]==0)	
		{	
		need_seek[dkB] = 1;			
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "PAD B1\n\r");	
		UART_TX(&huart4, U_TX_DATA, 8, 5);	
		#endif	
		}
	PAD_pressed[dkB][1] = 1;		
	}
else if((deckRbuf[8]&0x02)==0 && PAD_pressed[dkB][1]==1)
	{
	PAD_pressed[dkB][1] = 0;	
	}		
else if((deckRbuf[8]&0x04) && PAD_pressed[dkB][2]==0)		///////////PAD 2
	{
	if(lock_control[dkB]==0)	
		{	
		need_seek[dkB] = 2;			
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "PAD B2\n\r");	
		UART_TX(&huart4, U_TX_DATA, 8, 5);	
		#endif	
		}
	PAD_pressed[dkB][2] = 1;		
	}
else if((deckRbuf[8]&0x04)==0 && PAD_pressed[dkB][2]==1)
	{
	PAD_pressed[dkB][2] = 0;	
	}				
else if((deckRbuf[8]&0x08) && PAD_pressed[dkB][3]==0)		///////////PAD 3
	{
	if(lock_control[dkB]==0)	
		{	
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "PAD B3\n\r");	
		UART_TX(&huart4, U_TX_DATA, 8, 5);	
		#endif	
		}
	PAD_pressed[dkB][3] = 1;		
	}
else if((deckRbuf[8]&0x08)==0 && PAD_pressed[dkB][3]==1)
	{
	PAD_pressed[dkB][3] = 0;	
	}		
else if((deckRbuf[8]&0x10) && PAD_pressed[dkB][4]==0)		///////////PAD 4
	{
	if(lock_control[dkB]==0)	
		{	
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "PAD B4\n\r");	
		UART_TX(&huart4, U_TX_DATA, 8, 5);	
		#endif	
		}
	PAD_pressed[dkB][4] = 1;		
	}
else if((deckRbuf[8]&0x10)==0 && PAD_pressed[dkB][4]==1)
	{
	PAD_pressed[dkB][4] = 0;	
	}			
else if((deckRbuf[8]&0x20) && PAD_pressed[dkB][5]==0)		///////////PAD 5
	{
	if(lock_control[dkB]==0)	
		{	
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "PAD B5\n\r");	
		UART_TX(&huart4, U_TX_DATA, 8, 5);	
		#endif	
		}
	PAD_pressed[dkB][5] = 1;		
	}
else if((deckRbuf[8]&0x20)==0 && PAD_pressed[dkB][5]==1)
	{
	PAD_pressed[dkB][5] = 0;	
	}	
else if((deckRbuf[8]&0x40) && PAD_pressed[dkB][6]==0)		///////////PAD 6
	{
	if(lock_control[dkB]==0)	
		{	
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "PAD B6\n\r");	
		UART_TX(&huart4, U_TX_DATA, 8, 5);	
		#endif	
		}
	PAD_pressed[dkB][6] = 1;		
	}
else if((deckRbuf[8]&0x40)==0 && PAD_pressed[dkB][6]==1)
	{
	PAD_pressed[dkB][6] = 0;	
	}	
else if((deckRbuf[8]&0x80) && PAD_pressed[dkB][7]==0)		///////////PAD 7
	{
	if(lock_control[dkB]==0)	
		{	
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "PAD B7\n\r");	
		UART_TX(&huart4, U_TX_DATA, 8, 5);	
		#endif	
		}
	PAD_pressed[dkB][7] = 1;		
	}
else if((deckRbuf[8]&0x80)==0 && PAD_pressed[dkB][7]==1)
	{
	PAD_pressed[dkB][7] = 0;	
	}
			
			
			