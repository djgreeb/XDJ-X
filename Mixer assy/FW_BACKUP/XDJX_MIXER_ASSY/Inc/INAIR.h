/////////////////////////////////////////////////////////////////////////////////////////////////
// 	IN AIR control faders
//	product: XDJ-X mixer assy
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////


if(pot_8b[FDR0]>10 && (decka_inair!=1))
	{
	decka_inair = 1;
	need_send_inair = 1;	
	}
else if(pot_8b[FDR0]<7 && (decka_inair!=0))
	{
	decka_inair = 0;
	need_send_inair = 1;	
	}
if(pot_8b[FDR1]>10 && (deckb_inair!=1))
	{
	deckb_inair = 1;
	need_send_inair = 1;	
	}
else if(pot_8b[FDR1]<7 && (deckb_inair!=0))
	{
	deckb_inair = 0;
	need_send_inair = 1;	
	}	
	
if(need_send_inair)
	{
	link_utx_buf[0] = 0x97;
	link_utx_buf[1] = decka_inair;
	link_utx_buf[2] =	deckb_inair;
	HAL_UART_Transmit(&huart5, link_utx_buf, 3, 5);
	need_send_inair = 0;	
	}

	
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																																												