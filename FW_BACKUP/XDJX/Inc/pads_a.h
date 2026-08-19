/////////////////////////////////////////////////////////////////////////////////////////////////
//  PADS A handler
//	processing of buttons, jog, pitch
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////


else if((deckRbuf[0]&0x01) && PAD_pressed[dkA][0]==0)		///////////PAD 0
	{
	PADSA(0);
	PAD_pressed[dkA][0] = 1;		
	}
else if((deckRbuf[0]&0x01)==0 && PAD_pressed[dkA][0]==1)
	{
	PAD_pressed[dkA][0] = 0;	
	}
else if((deckRbuf[0]&0x02) && PAD_pressed[dkA][1]==0)		///////////PAD 1
	{
	PADSA(1);
	PAD_pressed[dkA][1] = 1;		
	}
else if((deckRbuf[0]&0x02)==0 && PAD_pressed[dkA][1]==1)
	{
	PAD_pressed[dkA][1] = 0;	
	}		
else if((deckRbuf[0]&0x04) && PAD_pressed[dkA][2]==0)		///////////PAD 2
	{
	PADSA(2);
	PAD_pressed[dkA][2] = 1;		
	}
else if((deckRbuf[0]&0x04)==0 && PAD_pressed[dkA][2]==1)
	{
	PAD_pressed[dkA][2] = 0;	
	}				
else if((deckRbuf[0]&0x08) && PAD_pressed[dkA][3]==0)		///////////PAD 3
	{
	PADSA(3);
	PAD_pressed[dkA][3] = 1;		
	}
else if((deckRbuf[0]&0x08)==0 && PAD_pressed[dkA][3]==1)
	{
	PAD_pressed[dkA][3] = 0;	
	}		
else if((deckRbuf[0]&0x10) && PAD_pressed[dkA][4]==0)		///////////PAD 4
	{
	PADSA(4);
	PAD_pressed[dkA][4] = 1;		
	}
else if((deckRbuf[0]&0x10)==0 && PAD_pressed[dkA][4]==1)
	{
	PAD_pressed[dkA][4] = 0;	
	}			
else if((deckRbuf[0]&0x20) && PAD_pressed[dkA][5]==0)		///////////PAD 5
	{
	PADSA(5);
	PAD_pressed[dkA][5] = 1;		
	}
else if((deckRbuf[0]&0x20)==0 && PAD_pressed[dkA][5]==1)
	{
	PAD_pressed[dkA][5] = 0;	
	}	
else if((deckRbuf[0]&0x40) && PAD_pressed[dkA][6]==0)		///////////PAD 6
	{
	PADSA(6);
	PAD_pressed[dkA][6] = 1;		
	}
else if((deckRbuf[0]&0x40)==0 && PAD_pressed[dkA][6]==1)
	{
	PAD_pressed[dkA][6] = 0;	
	}	
else if((deckRbuf[0]&0x80) && PAD_pressed[dkA][7]==0)		///////////PAD 7
	{
	PADSA(7);
	PAD_pressed[dkA][7] = 1;		
	}
else if((deckRbuf[0]&0x80)==0 && PAD_pressed[dkA][7]==1)
	{
	PAD_pressed[dkA][7] = 0;	
	}