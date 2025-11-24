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
			UART_TX(&huart4, U_TX_DATA, 6, 5);		
			}
		else if(dbg_urx_buf[0]==1)
			{
			UART_TX(&huart4, deckRbuf, 16, 15);	
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
				UART_TX(&huart4, U_TX_DATA, 6, 5);						
				}				
			else if(dbg_urx_buf[1]==1)		//jog A cnt
				{
				sprintf((char*)U_TX_DATA, "%05lu\n\r", (0x100*deckRbuf[5])+deckRbuf[6]);
				UART_TX(&huart4, U_TX_DATA, 7, 5);						
				}
			else if(dbg_urx_buf[1]==2)		//pitch B
				{
				sprintf((char*)U_TX_DATA, "%04lu\n\r", 0x100*(deckRbuf[10]&0x0F)+deckRbuf[11]);
				UART_TX(&huart4, U_TX_DATA, 6, 5);						
				}				
			else if(dbg_urx_buf[1]==3)		//jog B cnt
				{
				sprintf((char*)U_TX_DATA, "%05lu\n\r", (0x100*deckRbuf[13])+deckRbuf[14]);
				UART_TX(&huart4, U_TX_DATA, 7, 5);						
				}
			else if(dbg_urx_buf[1]==4)		//jog A dir
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[4]);
				UART_TX(&huart4, U_TX_DATA, 5, 5);						
				}
			else if(dbg_urx_buf[1]==5)		//jog B dir
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[12]);
				UART_TX(&huart4, U_TX_DATA, 5, 5);						
				}
			else if(dbg_urx_buf[1]==6)		//A buttons 2
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[2]);
				UART_TX(&huart4, U_TX_DATA, 5, 5);						
				}
			else if(dbg_urx_buf[1]==7)		//B buttons 2
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[10]);
				UART_TX(&huart4, U_TX_DATA, 5, 5);						
				}
			else if(dbg_urx_buf[1]==8)		//A buttons 1
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[1]);
				UART_TX(&huart4, U_TX_DATA, 5, 5);						
				}
			else if(dbg_urx_buf[1]==9)		//B buttons 1
				{
				sprintf((char*)U_TX_DATA, "%03lu\n\r", deckRbuf[9]);
				UART_TX(&huart4, U_TX_DATA, 5, 5);						
				}
			}	
		else if(dbg_urx_buf[0]==4)			//load track
			{	
			PREPARE_LOAD_TRACK(dkA, dbg_urx_buf[1], dbg_urx_buf[2]);	
			}
		else if(dbg_urx_buf[0]==5)					//read eeprom memory on main assy $05
			{	
			uint8_t pntr = 0;
			uint8_t n;
			uint8_t s;	
			for(s=0;s<128;s++)
				{					
				pntr = 0;		
				for(n=0;n<16;n++)
					{
					if(n==7)
						{
						sprintf((char*)&U_TX_DATA[pntr], "%02lX    ", RD_EEMEM(16*s+n));	
						pntr+=6;	
						}
					else
						{
						sprintf((char*)&U_TX_DATA[pntr], "%02lX ", RD_EEMEM(16*s+n));
						pntr+=3;			
						}
					}							
				U_TX_DATA[51] = 13;
				U_TX_DATA[52] = 10;				
				UART_TX(&huart4, U_TX_DATA, 53, 5);		
				}	
			}	
		else if(dbg_urx_buf[0]==6)					//write eeprom memory on main assy $06$nn$nn$xx
			{
			WR_EEMEM((256*dbg_urx_buf[1]+dbg_urx_buf[2]), dbg_urx_buf[3]);
			sprintf((char*)U_TX_DATA, "%s", "writed\n\r");
			UART_TX(&huart4, U_TX_DATA, 8, 5);
			}
		else if(dbg_urx_buf[0]==7)						//earse eeprom memory on main assy $07
			{
			sprintf((char*)U_TX_DATA, "%s", "wait 2 minutes\n\r");	
			UART_TX(&huart4, U_TX_DATA, 16, 5);	
			uint16_t s;	
			for(s=0;s<2048;s++)
				{
				WR_EEMEM(s, 0xFF);
				}
			sprintf((char*)U_TX_DATA, "%s", "fill mem\n\r");
			UART_TX(&huart4, U_TX_DATA, 10, 5);	
			}
		else if(dbg_urx_buf[0]==8)					//write utility $08$nn$xx
			{				
			UT_SET[dbg_urx_buf[1]] = dbg_urx_buf[2];
			sprintf((char*)U_TX_DATA, "%s", "writed\n\r");
			UART_TX(&huart4, U_TX_DATA, 8, 5);
			}
		else if(dbg_urx_buf[0]==9)					//send keys table
			{	
			uint8_t n;
			for(n=0;n<25;n++)
				{				
				sprintf((char*)&U_TX_DATA, "%03lu   ", KEYSmatchtable[n]);	
				UART_TX(&huart4, U_TX_DATA, 6, 5);

				UART_TX(&huart4, KEYS[n], 4, 5);
				sprintf((char*)U_TX_DATA, "\n\r");
				UART_TX(&huart4, U_TX_DATA, 2, 5);
				}
			}	
		else if(dbg_urx_buf[0]==10)					//pads color
			{	
			for(j=0;j<8;j++)
				{
				deckTbuf[j][1] = dbg_urx_buf[1];
				deckTbuf[j][2] = dbg_urx_buf[2];
				deckTbuf[j][3] = dbg_urx_buf[3];
				deckTbuf[j][9] = dbg_urx_buf[1];
				deckTbuf[j][10] = dbg_urx_buf[2];
				deckTbuf[j][11] = dbg_urx_buf[3];
				}	
			}	
		dbg_new_data = 0;		
		};
	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



