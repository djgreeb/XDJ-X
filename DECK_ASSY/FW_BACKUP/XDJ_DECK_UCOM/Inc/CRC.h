/////////////////////////////////////////////////////////////////////////////////////////////////
// 	CRCs funtions
//	product: XDJ-X deck ucom
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t n;
uint8_t sm;

void CalcTXCRC(void);
uint8_t CheckRXCRC(void);

////////////////////////////////////////////////////////
//
//
//
void CalcTXCRC(void)
	{
	#ifdef DECK_1	
	deckTbuf[7] = 0;	
	for(n=0;n<7;n++)
		{
		deckTbuf[7]+=deckTbuf[n];	
		}
	#endif

	#ifndef DECK_1
	deckTbuf[15] = 0;	
	for(n=8;n<15;n++)
		{
		deckTbuf[15]+=deckTbuf[n];	
		}
	#endif
	};


////////////////////////////////////////////////////////
//
//
//
uint8_t CheckRXCRC(void)
	{
	sm = 0;		
		
	#ifdef DECK_1	
	for(n=0;n<7;n++)
		{
		sm+=deckRbuf[n];	
		}
	if(sm==deckRbuf[7])
		{
		return 1;	
		}		
	else
		{
		return 0;	
		}
	#endif

	#ifndef DECK_1	
	for(n=8;n<15;n++)
		{
		sm+=deckRbuf[n];	
		}
	if(sm==deckRbuf[15])
		{
		return 1;	
		}		
	else
		{
		return 0;	
		}	
	#endif
	}


	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	
