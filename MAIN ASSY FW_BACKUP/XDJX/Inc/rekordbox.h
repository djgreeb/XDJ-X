/////////////////////////////////////////////////////////////////////////////////////////////////
//  Rekordbox database parser based on CDJ-1000mk3 new life project
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t DATABASE_PARSER(void);
uint8_t PlaylistID_to_Pos(uint8_t ID);										//convert playlist ID to position in Tracklist name
uint16_t LOAD_TRACKA(uint16_t TRACK_NUMBER);
uint16_t LOAD_TRACKB(uint16_t TRACK_NUMBER);
void PREPARE_LOAD_TRACK(uint8_t dk, uint16_t TRACK_NUMBER, uint16_t TRACK_IN_PLAYLIST);
uint16_t FindCurrBar(uint8_t dk, uint32_t pos);		//search current bar number


////////////////////////////////////////////////////////////////////////
//
//
//Rekordbox database parser ver. 0.43
//Functions:
//Open and read file export.pdb
//find all tracks
//write trackname in massive
//write adress position datatrack in file export.pdb
//return number of tracks or 0 if error enable 
//extended to 8 hot cues reading from *.EXT files
//bug fixed: path_ANLZ expanded to 46 bytes, for final 0 
//BEATGRID and BPMGRID changed 4096->2048
//
////////////////////////////////////////////////////////////////////////
uint16_t DATABASE_PARSER(void)
	{	
	#if defined(DEBUG_UART_EN)		
	sprintf((char*)U_TX_DATA, "Start Rekordbox parser ver. 0.42\n\r");	
	UART_TX(&huart4, U_TX_DATA, 34, 55);	
	#endif		
	res = f_open(&file, path_export, FA_READ);
	if (res != FR_OK)
		{
		return 0xFFFE;	
		}		
	uint32_t FILSIZE = f_size(&file);
	char str[4] = ".DAT";
	uint32_t crsr = 0;
	uint32_t BPM_CNT = 0;
	uint8_t	cycle_en;
	uint16_t all_trks = 0;			//512 max
	uint16_t T_ID = 0;
	uint16_t i  = 0;	
	uint32_t NEXT_PAGE_0, LAST_PAGE_0, P_PAGE_0;	
	uint32_t NEXT_PAGE_5, LAST_PAGE_5; //page for KEY
	uint32_t NEXT_PAGE_7, LAST_PAGE_7; //page for PLAYLIST tree
	uint32_t NEXT_PAGE_8, LAST_PAGE_8; //page for PLAYLIST entries		
	uint32_t NEXT_PAGE_19, LAST_PAGE_19; //page for history, flash name, date
	for(T_ID=0;T_ID<28160;T_ID++)					//clear arrays
		{	
		playlist[T_ID/55][T_ID%55] = 0;
		}		
	for(T_ID=0;T_ID<512;T_ID++)					//clear arrays
		{	
		parcser_adress[T_ID] = 0;
		original_tempo[T_ID] = 0;	
		rating[T_ID] = 0;	
		duration[T_ID] = 0;	
		key_id[T_ID] = 0;		
		}		
	for(T_ID=0;T_ID<1024;T_ID++)
		{
		WFORMDYNAMIC[dkA][4096+T_ID] = 0xFF;				//give out buffer for track ID in WFORMDYNAMIC[dkA]	offset 8192
		}
	T_ID = 0;	
		
	res = f_read(&file, WFORMDYNAMIC[dkA], 4096, &nbytes);
	if (res != FR_OK)
		{
		return 0;	
		}	
	crsr = 28;		
	while(crsr<(28+16*(WFORMDYNAMIC[dkA][8] + 256*WFORMDYNAMIC[dkA][9] + 65536*WFORMDYNAMIC[dkA][10] + 16777216*WFORMDYNAMIC[dkA][11])))	//scan first page and search link for all type pages with tables 
		{
		if(WFORMDYNAMIC[dkA][crsr]==0 && 
			 WFORMDYNAMIC[dkA][crsr+1]==0 && 
			 WFORMDYNAMIC[dkA][crsr+2]==0 && 
			 WFORMDYNAMIC[dkA][crsr+3]==0)	//Track metadata: title, artist, genre, artwork ID, playing time, etc
			{
			NEXT_PAGE_0 = WFORMDYNAMIC[dkA][crsr+8] + 256*WFORMDYNAMIC[dkA][crsr+9] + 65536*WFORMDYNAMIC[dkA][crsr+10] + 16777216*WFORMDYNAMIC[dkA][crsr+11];
			LAST_PAGE_0 = WFORMDYNAMIC[dkA][crsr+12] + 256*WFORMDYNAMIC[dkA][crsr+13] + 65536*WFORMDYNAMIC[dkA][crsr+14] + 16777216*WFORMDYNAMIC[dkA][crsr+15];
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "LP0 %08lu\n\r", LAST_PAGE_0);	
			UART_TX(&huart4, U_TX_DATA, 14, 55);	
			#endif		
			}
		else if(WFORMDYNAMIC[dkA][crsr]==5 && 
						WFORMDYNAMIC[dkA][crsr+1]==0 && 
						WFORMDYNAMIC[dkA][crsr+2]==0 && 
						WFORMDYNAMIC[dkA][crsr+3]==0)	//Musical keys, for reference by tracks, searching, and key matching
			{
			NEXT_PAGE_5 = WFORMDYNAMIC[dkA][crsr+8] + 256*WFORMDYNAMIC[dkA][crsr+9] + 65536*WFORMDYNAMIC[dkA][crsr+10] + 16777216*WFORMDYNAMIC[dkA][crsr+11];	
			LAST_PAGE_5 = WFORMDYNAMIC[dkA][crsr+12] + 256*WFORMDYNAMIC[dkA][crsr+13] + 65536*WFORMDYNAMIC[dkA][crsr+14] + 16777216*WFORMDYNAMIC[dkA][crsr+15];	
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "LP5 %08lu\n\r", LAST_PAGE_5);	
			UART_TX(&huart4, U_TX_DATA, 14, 55);	
			#endif	
			}	
		else if(WFORMDYNAMIC[dkA][crsr]==7 && 
						WFORMDYNAMIC[dkA][crsr+1]==0 && 
						WFORMDYNAMIC[dkA][crsr+2]==0 && 
						WFORMDYNAMIC[dkA][crsr+3]==0)	//Describes the hierarchical tree structure of available playlists and folders grouping them
			{
			NEXT_PAGE_7 = WFORMDYNAMIC[dkA][crsr+8] + 256*WFORMDYNAMIC[dkA][crsr+9] + 65536*WFORMDYNAMIC[dkA][crsr+10] + 16777216*WFORMDYNAMIC[dkA][crsr+11];	
			LAST_PAGE_7 = WFORMDYNAMIC[dkA][crsr+12] + 256*WFORMDYNAMIC[dkA][crsr+13] + 65536*WFORMDYNAMIC[dkA][crsr+14] + 16777216*WFORMDYNAMIC[dkA][crsr+15];
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "LP7 %08lu\n\r", LAST_PAGE_7);	
			UART_TX(&huart4, U_TX_DATA, 14, 55);	
			#endif					
			}		
		else if(WFORMDYNAMIC[dkA][crsr]==8 && 
						WFORMDYNAMIC[dkA][crsr+1]==0 && 
						WFORMDYNAMIC[dkA][crsr+2]==0 && 
						WFORMDYNAMIC[dkA][crsr+3]==0)	//Links tracks to playlists, in the right order
			{
			NEXT_PAGE_8 = WFORMDYNAMIC[dkA][crsr+8] + 256*WFORMDYNAMIC[dkA][crsr+9] + 65536*WFORMDYNAMIC[dkA][crsr+10] + 16777216*WFORMDYNAMIC[dkA][crsr+11];	
			LAST_PAGE_8 = WFORMDYNAMIC[dkA][crsr+12] + 256*WFORMDYNAMIC[dkA][crsr+13] + 65536*WFORMDYNAMIC[dkA][crsr+14] + 16777216*WFORMDYNAMIC[dkA][crsr+15];	
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "LP8 %08lu\n\r", LAST_PAGE_8);	
			UART_TX(&huart4, U_TX_DATA, 14, 55);	
			#endif				
			}	
		else if(WFORMDYNAMIC[dkA][crsr]==19 && 
						WFORMDYNAMIC[dkA][crsr+1]==0 && 
						WFORMDYNAMIC[dkA][crsr+2]==0 && 
						WFORMDYNAMIC[dkA][crsr+3]==0)	//Links tracks to playlists, in the right order
			{
			NEXT_PAGE_19 = WFORMDYNAMIC[dkA][crsr+8] + 256*WFORMDYNAMIC[dkA][crsr+9] + 65536*WFORMDYNAMIC[dkA][crsr+10] + 16777216*WFORMDYNAMIC[dkA][crsr+11];	
			LAST_PAGE_19 = WFORMDYNAMIC[dkA][crsr+12] + 256*WFORMDYNAMIC[dkA][crsr+13] + 65536*WFORMDYNAMIC[dkA][crsr+14] + 16777216*WFORMDYNAMIC[dkA][crsr+15];
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "LP19 %08lu\n\r", LAST_PAGE_19);	
			UART_TX(&huart4, U_TX_DATA, 15, 55);	
			#endif					
			}		
		crsr+=16;		
		}
	crsr = 0;
		
	f_lseek(&file, 4096*NEXT_PAGE_0); 				//first page with track metadata: title, artist, genre, artwork ID, playing time, etc
	
	while(NEXT_PAGE_0<(FILSIZE>>12) && LAST_PAGE_0>=NEXT_PAGE_0)	
		{
		res = f_read(&file, WFORMDYNAMIC[dkA], 4096, &nbytes);
		if (res != FR_OK)
			{
			return 0;	
			}
		crsr = 0;
		P_PAGE_0 = NEXT_PAGE_0;
		NEXT_PAGE_0 = WFORMDYNAMIC[dkA][12] + 256*WFORMDYNAMIC[dkA][13] + 65536*WFORMDYNAMIC[dkA][14] + 16777216*WFORMDYNAMIC[dkA][15];		
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "NP0 %08lu\n\r", NEXT_PAGE_0);	
		UART_TX(&huart4, U_TX_DATA, 14, 55);	
		#endif			
		if(WFORMDYNAMIC[dkA][8]==0 && WFORMDYNAMIC[dkA][9]==0 && WFORMDYNAMIC[dkA][10]==0 && WFORMDYNAMIC[dkA][11]==0 && (WFORMDYNAMIC[dkA][27]&0x40)==0 && WFORMDYNAMIC[dkA][36]==0 && WFORMDYNAMIC[dkA][37]==0)		//check page type
			{			
			while(crsr<4095)
				{
				if(WFORMDYNAMIC[dkA][crsr]==0x24 && WFORMDYNAMIC[dkA][crsr+1]==0x00 && WFORMDYNAMIC[dkA][crsr+8]==0x44 && WFORMDYNAMIC[dkA][crsr+9]==0xAC 
					&& WFORMDYNAMIC[dkA][crsr+48]==0x83 && WFORMDYNAMIC[dkA][crsr+49]==0x05 && WFORMDYNAMIC[dkA][crsr+82]==0x10)		//check 0x2400 and sample rate 44100 bitrate 1411kbps 16Bits
					{	
					T_ID = WFORMDYNAMIC[dkA][crsr+72] + 256*WFORMDYNAMIC[dkA][crsr+73];		
					if(T_ID>512)
						{	
						f_close(&file);	
						return 0xFFFF;	
						}
						
					if(WFORMDYNAMIC[dkA][4096+2*(T_ID-1)]==0xFF && WFORMDYNAMIC[dkA][4097+2*(T_ID-1)]==0xFF)		//check track new or 
						{
						all_trks++;	
						}	
					WFORMDYNAMIC[dkA][4096+2*(T_ID-1)] = WFORMDYNAMIC[dkA][crsr+72];	
					WFORMDYNAMIC[dkA][4097+2*(T_ID-1)] = WFORMDYNAMIC[dkA][crsr+73];		
					crsr+= 32;	
					key_id[T_ID-1] = WFORMDYNAMIC[dkA][crsr];
					crsr+= 24;					
					BPM_CNT = WFORMDYNAMIC[dkA][crsr] + 256*WFORMDYNAMIC[dkA][crsr+1] + 65535*WFORMDYNAMIC[dkA][crsr+2];	
					BPM_CNT = BPM_CNT/10;	
					original_tempo[T_ID-1] = BPM_CNT;
					crsr+= 28;	
					duration[T_ID-1] = WFORMDYNAMIC[dkA][crsr] + 256*WFORMDYNAMIC[dkA][crsr+1];	
					crsr+= 4;
					rating[T_ID-1] = WFORMDYNAMIC[dkA][crsr] + 256*WFORMDYNAMIC[dkA][crsr+1];
					crsr+= 4;						
					cycle_en = 1;							
					while(crsr<4095 && cycle_en)
						{	
						if(WFORMDYNAMIC[dkA][crsr]==str[1])
							{
							if(WFORMDYNAMIC[dkA][crsr-1]==str[0] && 
								 WFORMDYNAMIC[dkA][crsr+1]==str[2] && 
								 WFORMDYNAMIC[dkA][crsr+2]==str[3])				//".DAT" Finded!
								{
								parcser_adress[T_ID-1] = (4096*P_PAGE_0)+crsr;										//save position ".[D]AT"	
								crsr = crsr+16;									
								for(i=0;((i<54) && (WFORMDYNAMIC[dkA][crsr+i]!=3));i++)						//copy track name	
									{		
									if((WFORMDYNAMIC[dkA][crsr+i]<32) || (WFORMDYNAMIC[dkA][crsr+i]>126))				//filtering UNICODE symbols
										{
										playlist[T_ID-1][i] = 32;	
										}										
									else
										{										
										playlist[T_ID-1][i] = WFORMDYNAMIC[dkA][crsr+i];
										}		
									}
								while(i<54)																				//Fill spaces
									{
									playlist[T_ID-1][i] = 0;
									i++;		
									}									
								playlist[T_ID-1][53] = 0;
								playlist[T_ID-1][54] = 0;	
									
								i = 0;	
								crsr+= 10;	
								cycle_en = 0;	
								}
							}
						crsr++;	
						}	
					}
				crsr++;	
				}				
			}
		f_lseek(&file, 4096*NEXT_PAGE_0);
		}	
	f_lseek(&file, 4096*NEXT_PAGE_5); 				//first page with musical keys, for reference by tracks, searching, and key matching
	cycle_en = 1;		
	while(NEXT_PAGE_5<(FILSIZE>>12) && cycle_en)	
		{
		if(LAST_PAGE_5==NEXT_PAGE_5)			//scan last page
			{
			cycle_en = 0;	
			}
		res = f_read(&file, WFORMDYNAMIC[dkA], 4096, &nbytes);
		if (res != FR_OK)
			{
			return 0;	
			}
		crsr = 0;
		NEXT_PAGE_5 = WFORMDYNAMIC[dkA][12] + 256*WFORMDYNAMIC[dkA][13] + 65536*WFORMDYNAMIC[dkA][14] + 16777216*WFORMDYNAMIC[dkA][15];						
		if(WFORMDYNAMIC[dkA][8]==5 && WFORMDYNAMIC[dkA][9]==0 && WFORMDYNAMIC[dkA][10]==0 && WFORMDYNAMIC[dkA][11]==0 && (WFORMDYNAMIC[dkA][27]&0x40)==0 && WFORMDYNAMIC[dkA][36]==0 && WFORMDYNAMIC[dkA][37]==0 && WFORMDYNAMIC[dkA][24]!=0)		//check page type
			{	
			P_PAGE_0 = WFORMDYNAMIC[dkA][24];	
			crsr = 40;	
			while(P_PAGE_0>0)	
				{					
				KEYS[WFORMDYNAMIC[dkA][crsr]-1][0] = WFORMDYNAMIC[dkA][crsr+9];
				KEYS[WFORMDYNAMIC[dkA][crsr]-1][1] = WFORMDYNAMIC[dkA][crsr+10];	
				KEYS[WFORMDYNAMIC[dkA][crsr]-1][2] = WFORMDYNAMIC[dkA][crsr+11];
				KEYS[WFORMDYNAMIC[dkA][crsr]-1][3] = WFORMDYNAMIC[dkA][crsr+12];					
					
				for(i==0;i<24;i++)
					{
					if(KEYS[WFORMDYNAMIC[dkA][crsr]-1][0]==KEY_MATCH[i][0] && KEYS[WFORMDYNAMIC[dkA][crsr]-1][1]==KEY_MATCH[i][1] && KEYS[WFORMDYNAMIC[dkA][crsr]-1][2]==KEY_MATCH[i][2])	
						{
						KEYSmatchtable[WFORMDYNAMIC[dkA][crsr]-1] = i;
						i = 100;	
						}	
					}	
				if(i<99)
					{
					KEYSmatchtable[WFORMDYNAMIC[dkA][crsr]-1] = 25;		//this string key not found in table	
					}					
				i = 0;	
	
				if(KEYS[WFORMDYNAMIC[dkA][crsr]-1][3]<33 || KEYS[WFORMDYNAMIC[dkA][crsr]-1][3]>125)				//cut string
					{
					KEYS[WFORMDYNAMIC[dkA][crsr]-1][3] = 0;	
					}
					
				if(WFORMDYNAMIC[dkA][crsr+8]==5)			//string lenght 1 symbol
					{
					KEYS[WFORMDYNAMIC[dkA][crsr]-1][1] = 0;
					crsr+=12;	
					}
				else if(WFORMDYNAMIC[dkA][crsr+8]==7)			//string lenght 2 symbols
					{
					KEYS[WFORMDYNAMIC[dkA][crsr]-1][2] = 0;
					crsr+=12;	
					}	
				else if(WFORMDYNAMIC[dkA][crsr+8]==9)			//string lenght 3 symbols
					{
					KEYS[WFORMDYNAMIC[dkA][crsr]-1][3] = 0;
					crsr+=12;	
					}
				else
					{	
					crsr = crsr + 12 + 4*((((WFORMDYNAMIC[dkA][crsr+8]-1)/2)-1)/4);		
					}					
				P_PAGE_0--;	
				}
			}
			
		f_lseek(&file, 4096*NEXT_PAGE_5);	
		}	
	f_lseek(&file, 4096*NEXT_PAGE_7); 				//first page for PLAYLIST tree
	cycle_en = 1;		
	for(i=0;i<4200;i++)					//prepare 0 buffer
		{
		WFORMDYNAMIC[dkA][8192+i] = 0;	
		}		
	while(LAST_PAGE_7<(FILSIZE>>12) && cycle_en)	
		{
		if(LAST_PAGE_7==NEXT_PAGE_7)			//scan last page
			{
			cycle_en = 0;	
			}
		res = f_read(&file, WFORMDYNAMIC[dkA], 4096, &nbytes);
		if (res != FR_OK)
			{
			return 0;	
			}
		crsr = 0;
		NEXT_PAGE_7 = WFORMDYNAMIC[dkA][12] + 256*WFORMDYNAMIC[dkA][13] + 65536*WFORMDYNAMIC[dkA][14] + 16777216*WFORMDYNAMIC[dkA][15];			
		if(WFORMDYNAMIC[dkA][8]==7 && WFORMDYNAMIC[dkA][9]==0 && WFORMDYNAMIC[dkA][10]==0 && WFORMDYNAMIC[dkA][11]==0 && (WFORMDYNAMIC[dkA][27]&0x40)==0 && WFORMDYNAMIC[dkA][36]==0 && WFORMDYNAMIC[dkA][37]==0 && WFORMDYNAMIC[dkA][24]!=0)		//check page type
			{	
			P_PAGE_0 = WFORMDYNAMIC[dkA][24];
			crsr = 40;	
			P_PAGE_0 = P_PAGE_0 + 1;		
			while(P_PAGE_0>0)	
				{		
				BPM_CNT = (((WFORMDYNAMIC[dkA][crsr+20]-1)/2)-1);		//playlist lenght name	
				if(WFORMDYNAMIC[dkA][crsr+16]==0 && WFORMDYNAMIC[dkA][crsr+17]==0 && WFORMDYNAMIC[dkA][crsr+18]==0 && WFORMDYNAMIC[dkA][crsr+19]==0)			//row is playlist
					{
					//WFORMDYNAMIC[dkA][crsr+12]			//playlist ID		
					for(i=0;(i<BPM_CNT && i<20); i++)
						{
						WFORMDYNAMIC[dkA][8192+21*(WFORMDYNAMIC[dkA][crsr+12]-1)+i] = WFORMDYNAMIC[dkA][crsr+21+i];	
						}
					WFORMDYNAMIC[dkA][8192+21*(WFORMDYNAMIC[dkA][crsr+12]-1)+20] = WFORMDYNAMIC[dkA][crsr+12];			//playlist ID	 	
					}					
				crsr = crsr + 24 + 4*(BPM_CNT/4);
				P_PAGE_0--;	
				}
			}			
		f_lseek(&file, 4096*NEXT_PAGE_7);	
		}
	TOTAL_TRACKLISTS = 0;
	uint8_t inp = 0;
	uint8_t cnt;	
	for(BPM_CNT=1;BPM_CNT<255;BPM_CNT++)	
		{
		for(i=0;i<200;i++)
			{
			if(WFORMDYNAMIC[dkA][8192+20+21*i]==BPM_CNT)				//have a playlist
				{
				for(cnt=0;cnt<21;cnt++)
					{
					TRACKLIST_NAME[TOTAL_TRACKLISTS][cnt] = WFORMDYNAMIC[dkA][8192+cnt+21*i];
					}
				inp = 1;	
				}
			}
		if(inp)	
			{
			if(TOTAL_TRACKLISTS<20)
				{
				TOTAL_TRACKLISTS++;
				}
			inp = 0;	
			}	
		}	
			
	f_lseek(&file, 4096*NEXT_PAGE_8); 				//first page for PLAYLIST tree
	
	cycle_en = 1;	
	for(i=0;i<20480;i++)					//prepare 0 buffer
		{
		WFORMDYNAMIC[dkA][8192+i] = 0xFF;	
		}		
		
	while(LAST_PAGE_8<(FILSIZE>>12) && cycle_en)	
		{
		if(LAST_PAGE_8==NEXT_PAGE_8)			//scan last page
			{
			cycle_en = 0;	
			}
		res = f_read(&file, WFORMDYNAMIC[dkA], 4096, &nbytes);
		if (res != FR_OK)
			{
			return 0;	
			}
		crsr = 0;
		NEXT_PAGE_8 = WFORMDYNAMIC[dkA][12] + 256*WFORMDYNAMIC[dkA][13] + 65536*WFORMDYNAMIC[dkA][14] + 16777216*WFORMDYNAMIC[dkA][15];			
		if(WFORMDYNAMIC[dkA][8]==8 && WFORMDYNAMIC[dkA][9]==0 && WFORMDYNAMIC[dkA][10]==0 && WFORMDYNAMIC[dkA][11]==0 && (WFORMDYNAMIC[dkA][27]&0x40)==0 && WFORMDYNAMIC[dkA][36]==0 && WFORMDYNAMIC[dkA][37]==0 && (WFORMDYNAMIC[dkA][24]!=0 || WFORMDYNAMIC[dkA][25]!=0))		//check page type
			{	
			P_PAGE_0 = WFORMDYNAMIC[dkA][34] + 256*WFORMDYNAMIC[dkA][35];
			crsr = 40;	
			P_PAGE_0 = P_PAGE_0 + 1;	
			while(P_PAGE_0>0)	
				{
				i = PlaylistID_to_Pos(WFORMDYNAMIC[dkA][crsr+8]);
				if(i==20)
					{
					return 0xFFF1;				//send to error
					}
				T_ID = i*1024+(2*(WFORMDYNAMIC[dkA][crsr]+256*WFORMDYNAMIC[dkA][crsr+1]-1));
				WFORMDYNAMIC[dkA][8192+T_ID] = WFORMDYNAMIC[dkA][crsr+4];			
				WFORMDYNAMIC[dkA][8193+T_ID] = WFORMDYNAMIC[dkA][crsr+5];	
				crsr+=12;
				P_PAGE_0--;	
				}
			}			
		f_lseek(&file, 4096*NEXT_PAGE_8);	
		}
				
	TRACKLIST_OFFSET[0] = 0;
	crsr = 0;	
	for(i=0;i<TOTAL_TRACKLISTS;i++)
		{
		TRACKLIST_NAME[i][20] = 0;
		cycle_en = 1;		
		for(BPM_CNT=0; (BPM_CNT<512); BPM_CNT++)
			{
			T_ID = (i*1024)+(2*BPM_CNT);
			if((WFORMDYNAMIC[dkA][8192+T_ID] + 256*WFORMDYNAMIC[dkA][8193+T_ID])==0xFFFF)
				{
				cycle_en = 0;
				}
			else
				{
				TRACKS_DATABASE[TRACKLIST_OFFSET[i]+BPM_CNT] = WFORMDYNAMIC[dkA][8192+T_ID] + 256*WFORMDYNAMIC[dkA][8193+T_ID]; 	
				crsr++;	
				}
			}
		TRACKLIST_OFFSET[i+1] = crsr;				
		}
		
	f_lseek(&file, 4096*NEXT_PAGE_19); 				//first page with history, flash name, date
	cycle_en = 1;		
	while(NEXT_PAGE_19<(FILSIZE>>12) && cycle_en)	
		{
		if(LAST_PAGE_19==NEXT_PAGE_19)			//scan last page
			{
			cycle_en = 0;	
			}
			
		res = f_read(&file, WFORMDYNAMIC[dkA], 4096, &nbytes);
		if (res != FR_OK)
			{
			return 0;	
			}
		crsr = 0;
		NEXT_PAGE_19 = WFORMDYNAMIC[dkA][12] + 256*WFORMDYNAMIC[dkA][13] + 65536*WFORMDYNAMIC[dkA][14] + 16777216*WFORMDYNAMIC[dkA][15];						
		if(WFORMDYNAMIC[dkA][8]==19 && WFORMDYNAMIC[dkA][9]==0 && WFORMDYNAMIC[dkA][10]==0 && WFORMDYNAMIC[dkA][11]==0 && (WFORMDYNAMIC[dkA][27]&0x40)==0 && WFORMDYNAMIC[dkA][36]==0 && WFORMDYNAMIC[dkA][37]==0 && WFORMDYNAMIC[dkA][24]!=0)		//check page type
			{	
			P_PAGE_0 = WFORMDYNAMIC[dkA][24];	
			crsr = 40;
			while(P_PAGE_0>0)	
				{
				if(WFORMDYNAMIC[dkA][crsr]==0x80 && WFORMDYNAMIC[dkA][crsr+1]==0x02)
					{
					BPM_CNT = crsr;	
					crsr+=12;
						
					T_ID = WFORMDYNAMIC[dkA][crsr]-1;			//date lenght sting
					T_ID = T_ID/2;
					T_ID-=1;						
					
					for(i=0;(i<T_ID && i<13);i++)
						{
						SD_DATE[i] = WFORMDYNAMIC[dkA][crsr+i+1];	
						}
					SD_DATE[i] = 0;		//end string	
					crsr = crsr + T_ID + 3;	
						
					T_ID = WFORMDYNAMIC[dkA][crsr]-1; 			//date lenght sting	
					T_ID = T_ID/2;
					T_ID-=1;		
					crsr = crsr + T_ID + 1;
					T_ID = WFORMDYNAMIC[dkA][crsr]-1; 			//date lenght sting	
					T_ID = T_ID/2;
					T_ID-=1;
							
					for(i=0;(i<T_ID && i<18);i++)	
						{
						SDCARD_NAME[i] = WFORMDYNAMIC[dkA][crsr+i+1];	
						}	
					SDCARD_NAME[i] = 0;		//end string		
					crsr = crsr+T_ID+6;	
					BPM_CNT = 4-((crsr - BPM_CNT)%4);
					crsr = crsr +	BPM_CNT;
					P_PAGE_0--;			
					}
				else
					{
					P_PAGE_0 = 0;	
					}
				}
			}
		f_lseek(&file, 4096*NEXT_PAGE_19);	
		}
	f_close(&file);					//Close file Export.pdb
	return all_trks;
	}





////////////////////////////////////////////////////////////////////////
//convert playlist ID to position in Tracklist name
//
//
///////////////////////////////////////////////////////////////////////	
uint8_t PlaylistID_to_Pos(uint8_t ID)
	{
	uint8_t K;
	for(K=0;K<20;K++)
		{
		if(ID==TRACKLIST_NAME[K][20])
			{
			return K;	
			}
		}
	return 20;
	}


////////////////////////////////////////////////////////////////////////
//	open export.pdb file, extract path for ANLZXXXX.DAT file
//	open ANLZXXXX.DAT file, extract path for audio file
//	extract static waveform data, bpm	
//	open ANLZXXXX.EXT file, extract dynamic waveform data, all_long[dkA] data
//	open audio file
//
//	output: error code
////////////////////////////////////////////////////////////////////////	
uint16_t LOAD_TRACKA(uint16_t TRACK_NUMBER)
		{
		if(TRACK_NUMBER==0)
			{
			return 1;				//invalid track number
			}	
		uint16_t ERROR = 0;
		uint16_t E=0;
		uint32_t mem_pos;	
		res = f_open(&file, path_export, FA_READ);
		if (res != FR_OK)
			{
			return 2;	//cannot open database
			}
		f_lseek(&file, (parcser_adress[TRACK_NUMBER-1]-42));			//find 0.DAT in file		
		char path_ANLZ[46];
		res = f_read(&file, path_ANLZ, sizeof(path_ANLZ), &nbytes);
		if (res != FR_OK)
			{
			return 3;		//data not read
			}					
		f_close(&file);					//Close file Export.pdb
		path_ANLZ[0] = 48;
		path_ANLZ[1] = 58;
		path_ANLZ[45] = 0;
			
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "\n\r");	
		UART_TX(&huart4, U_TX_DATA, 2, 5);		
		UART_TX(&huart4, path_ANLZ, 45, 15);	
		UART_TX(&huart4, U_TX_DATA, 2, 5);		
		#endif		
			
		res = f_open(&file, path_ANLZ, FA_READ);
		if (res != FR_OK)
			{	
			return 4;	//cannot open ANLZXXXX.DAT file
			}
		uint32_t FILSIZE = f_size(&file);
		if(FILSIZE>WFD_SIZE)
			{
			return 26;	
			}
		res = f_read(&file, WFORMDYNAMIC[dkA], FILSIZE, &nbytes);
		if (res != FR_OK)
			{
			return 5;	//cannot read ANLZXXXX.DAT file		
			}
		f_close(&file);					//Close file ANLZXXXX.DAT		
		uint32_t fsz;	
		fsz = WFORMDYNAMIC[dkA][8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkA][9];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][11];
		if(fsz != FILSIZE)
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[dkA][4];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][5];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][6];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][7];
		uint32_t StPosHead = fsz;		
		if(WFORMDYNAMIC[dkA][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkA][StPosHead+1] != 80 ||
			 WFORMDYNAMIC[dkA][StPosHead+2] != 84 || 
			 WFORMDYNAMIC[dkA][StPosHead+3] != 72)		//Check PPHT position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[dkA][StPosHead+4];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+5];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+6];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+7];								//head size
		uint32_t SPP = fsz+StPosHead+1;	
		fsz = WFORMDYNAMIC[dkA][StPosHead+12];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkA][StPosHead+13];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+14];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+15];							//Path Size
		char path_AUDIOTRACK[(fsz/2)+2];			//Create a Path for audiotrack
		while(E<(fsz+4))
			{
			path_AUDIOTRACK[(E/2)+2] = WFORMDYNAMIC[dkA][SPP+E];			//Fill path
			E=E+2;	
			}
		path_AUDIOTRACK[0] = 48;
		path_AUDIOTRACK[1] = 58;	
		fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+9];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+10];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+11];	
		StPosHead = StPosHead+fsz;	//PVBR position
		if(WFORMDYNAMIC[dkA][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkA][StPosHead+1] != 86 ||
			 WFORMDYNAMIC[dkA][StPosHead+2] != 66 || 
			 WFORMDYNAMIC[dkA][StPosHead+3] != 82)	//Check PVBR position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkA][StPosHead+9];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+11];		
		StPosHead = StPosHead+fsz;	//PQTZ position
		if(WFORMDYNAMIC[dkA][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkA][StPosHead+1] != 81 ||
			 WFORMDYNAMIC[dkA][StPosHead+2] != 84 || 
			 WFORMDYNAMIC[dkA][StPosHead+3] != 90)	//Check PQTZ position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[dkA][StPosHead+4];				
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkA][StPosHead+5];		
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+6];		
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+7];				//Head size
		uint32_t number_of_entries;
		number_of_entries = WFORMDYNAMIC[dkA][StPosHead+20];				
		number_of_entries<<=8;	
		number_of_entries+=WFORMDYNAMIC[dkA][StPosHead+21];		
		number_of_entries<<=8;
		number_of_entries+=WFORMDYNAMIC[dkA][StPosHead+22];		
		number_of_entries<<=8;
		number_of_entries+=WFORMDYNAMIC[dkA][StPosHead+23];				//calculate number_of_entries
		if(number_of_entries>2048)
			{
			number_of_entries = 2048;	
			}
		SPP = StPosHead + fsz + 2;						//start first BPM data.
		E = 0;
		GRID_OFFSET[dkA] = WFORMDYNAMIC[dkA][SPP-1];							//find first beat 1...4
		while(E<number_of_entries)
			{
			BPMGRID[dkA][E] = WFORMDYNAMIC[dkA][SPP+(E*8)];	
			BPMGRID[dkA][E]<<=8;
			BPMGRID[dkA][E]+= WFORMDYNAMIC[dkA][SPP+1+(E*8)];		
			BEATGRID[dkA][E] = WFORMDYNAMIC[dkA][SPP+2+(E*8)];	
			BEATGRID[dkA][E]<<=8;
			BEATGRID[dkA][E]+= WFORMDYNAMIC[dkA][SPP+3+(E*8)];	
			BEATGRID[dkA][E]<<=8;
			BEATGRID[dkA][E]+= WFORMDYNAMIC[dkA][SPP+4+(E*8)];	
			BEATGRID[dkA][E]<<=8;
			BEATGRID[dkA][E]+= WFORMDYNAMIC[dkA][SPP+5+(E*8)];	
			E++;	
			}
		if(E==2048)
			{
			BEATGRID[dkA][2047] = 0xFFFFFFFF;	
			}
		else
			{
			BEATGRID[dkA][E] = 0xFFFFFFFF;	
			BPMGRID[dkA][E] = BPMGRID[dkA][E-1];	
			}
		originalBPM[dkA] = BPMGRID[dkA][0];			//SEND ORIGINAL BPM		
		fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkA][StPosHead+9];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+11];	
		StPosHead = StPosHead+fsz;	//PWAV position	
		if(WFORMDYNAMIC[dkA][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkA][StPosHead+1] != 87 ||
			 WFORMDYNAMIC[dkA][StPosHead+2] != 65 || 
			 WFORMDYNAMIC[dkA][StPosHead+3] != 86)		//Check PWAV position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[dkA][StPosHead+4];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+5];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+6];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+7];				//PWAV Header size
		fsz+=StPosHead;			
		uint16_t ampl;
		uint32_t y;	

		for(E=0;E<203;E++)					//Fill Static Waveform 400->203
			{
			y = 506*E;			//506
			y>>=8;	
			ampl = (WFORMDYNAMIC[dkA][fsz+y]&0x1F)*192;	 //convert amplitude 24->18
			ampl>>=8;
			if(ampl>18)
				{
				ampl = 18;	
				}
			WFORMSTATIC[dkA][E] = WFORMDYNAMIC[dkA][fsz+y]&0x80;	//color	
			WFORMSTATIC[dkA][E]|= ampl;	
			}

		fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkA][StPosHead+9];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+11];				//PWAV TAG size
		StPosHead+=fsz;													//PWV2 start adress
		if(WFORMDYNAMIC[dkA][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkA][StPosHead+1] != 87 ||
			 WFORMDYNAMIC[dkA][StPosHead+2] != 86 || 
			 WFORMDYNAMIC[dkA][StPosHead+3] != 50)		//Check PWV2 position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkA][StPosHead+9];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkA][StPosHead+11];				//PWV2 Tag size
		StPosHead = StPosHead+fsz;	//PCOB position				
		if(WFORMDYNAMIC[dkA][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkA][StPosHead+1] != 67 ||
			 WFORMDYNAMIC[dkA][StPosHead+2] != 79 || 
			 WFORMDYNAMIC[dkA][StPosHead+3] != 66)		//Check PCOB position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}		
		DrawMemoryMarker(dkA, 0xFF, NONE_MARK, 0x8000);
		uint32_t PCOB2_adr;
			
		//////////////////prepare HOT CUES	
		for(E=0;E<8;E++)									//Clear ícue massive
			{
			HCUE_type[dkA][E] = 0xFF;	 							//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 		
			HCUE_adr[dkA][0][E] = 0xFFFF;		
			HCUE_adr[dkA][1][E] = 0xFFFF;	
			HCUE32COLOR[dkA][E] = 0xFFFF;	
			}
		
		if(WFORMDYNAMIC[dkA][StPosHead+15]==1)    //check type PCOB - for HOT CUE points
			{
			number_of_hot_cue_points[dkA] = WFORMDYNAMIC[dkA][StPosHead+19]&0xF;				//number of hotcue points		
//			if(number_of_hot_cue_points[dkA]>0)
//				{
//				MemoryCuePyramid_ENABLE = 3;	
//				}
			fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
			fsz<<=8;	
			fsz+=WFORMDYNAMIC[dkA][StPosHead+9];
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkA][StPosHead+11];				//PCOB TAG size
			PCOB2_adr = fsz+StPosHead;							//start adress PCOB2
			fsz = WFORMDYNAMIC[dkA][StPosHead+4];			
			fsz<<=8;	
			fsz+=WFORMDYNAMIC[dkA][StPosHead+5];
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkA][StPosHead+6];	
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkA][StPosHead+7];				//PCOB head size
			StPosHead+= fsz;		
			uint8_t HCUE_NAME = 0;
			E = 0;	
			while(E<number_of_hot_cue_points[dkA])	
				{	
				fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[dkA][StPosHead+9];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkA][StPosHead+11];				//PCPT TAG size
				SPP = StPosHead+fsz;										//Next PCPT adress

				HCUE_NAME = WFORMDYNAMIC[dkA][StPosHead+15];
				if(WFORMDYNAMIC[dkA][StPosHead+19]!=0)				//when hcue active
					{
					if((HCUE_NAME<4) && (HCUE_NAME>0))
						{
						HCUE_type[dkA][HCUE_NAME-1] = 0x02;			//write 
						}
					}
				fsz = WFORMDYNAMIC[dkA][StPosHead+4];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[dkA][StPosHead+5];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkA][StPosHead+6];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkA][StPosHead+7];				//PCPT Head size
				StPosHead+= fsz;		
		
				if((HCUE_NAME<4) && (HCUE_NAME>0))
					{
					HCUE_NAME--;
					HCUE_adr[dkA][0][HCUE_NAME] = WFORMDYNAMIC[dkA][StPosHead+4];
					HCUE_adr[dkA][0][HCUE_NAME] = HCUE_adr[dkA][0][HCUE_NAME]<<8;
					HCUE_adr[dkA][0][HCUE_NAME]+= WFORMDYNAMIC[dkA][StPosHead+5];
					HCUE_adr[dkA][0][HCUE_NAME] = HCUE_adr[dkA][0][HCUE_NAME]<<8;
					HCUE_adr[dkA][0][HCUE_NAME]+= WFORMDYNAMIC[dkA][StPosHead+6];
					HCUE_adr[dkA][0][HCUE_NAME] = HCUE_adr[dkA][0][HCUE_NAME]<<8;	
					HCUE_adr[dkA][0][HCUE_NAME]+= WFORMDYNAMIC[dkA][StPosHead+7];
					if(WFORMDYNAMIC[dkA][StPosHead]==2)						//when hot cue type=loop
						{
						HCUE_type[dkA][HCUE_NAME]&= 0x03;	
						HCUE_type[dkA][HCUE_NAME] |= 0x01;		
						HCUE_adr[dkA][1][HCUE_NAME] = WFORMDYNAMIC[dkA][StPosHead+8];
						HCUE_adr[dkA][1][HCUE_NAME] = HCUE_adr[dkA][1][HCUE_NAME]<<8;
						HCUE_adr[dkA][1][HCUE_NAME]+= WFORMDYNAMIC[dkA][StPosHead+9];
						HCUE_adr[dkA][1][HCUE_NAME] = HCUE_adr[dkA][1][HCUE_NAME]<<8;
						HCUE_adr[dkA][1][HCUE_NAME]+= WFORMDYNAMIC[dkA][StPosHead+10];
						HCUE_adr[dkA][1][HCUE_NAME] = HCUE_adr[dkA][1][HCUE_NAME]<<8;	
						HCUE_adr[dkA][1][HCUE_NAME]+= WFORMDYNAMIC[dkA][StPosHead+11];
						HCUE_adr[dkA][1][HCUE_NAME] = (HCUE_adr[dkA][1][HCUE_NAME]*3)/20; 	//translate ms to 1/150s frames
						}
					}
				StPosHead = SPP;	
				E++;	
				}
			}
						
		//////////////////prepare MEMORY CUES
		for(E=0;E<10;E++)									//Clear memory cue massive
			{
			MEMORY_type[dkA][E] = 0;						//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 		
			MEMORY_adr[dkA][0][E] = 0xFFFF;
			MEMORY_adr[dkA][1][E] = 0xFFFF;	
			}
		StPosHead = PCOB2_adr;	
		if(WFORMDYNAMIC[dkA][StPosHead+15]==0)    //check type PCOB - for MEMORY CUE points
			{
			number_of_memory_cue_points[dkA] = WFORMDYNAMIC[dkA][StPosHead+19]&0xF;				//number of MEMORY points		
//			if(number_of_memory_cue_points[dkA]>0)
//				{
//				MemoryCuePyramid_ENABLE = 3;	
//				}
			fsz = WFORMDYNAMIC[dkA][StPosHead+4];			
			fsz<<=8;	
			fsz+=WFORMDYNAMIC[dkA][StPosHead+5];
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkA][StPosHead+6];	
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkA][StPosHead+7];				//PCOB head size
			StPosHead = StPosHead+fsz;		

			E = 0;	
			while(E<number_of_memory_cue_points[dkA])	
				{	
				fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkA][StPosHead+9];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkA][StPosHead+11];				//PCPT TAG size
				SPP = StPosHead+fsz;										//Next PCPT adress

				if(WFORMDYNAMIC[dkA][StPosHead+19]!=0)				//when hcue active
					{
					MEMORY_type[dkA][E] = 2;			//write 
					}	
				fsz = WFORMDYNAMIC[dkA][StPosHead+4];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[dkA][StPosHead+5];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkA][StPosHead+6];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkA][StPosHead+7];				//PCPT Head size
				StPosHead = StPosHead+fsz;		
		
				MEMORY_adr[dkA][0][E] = WFORMDYNAMIC[dkA][StPosHead+4];
				MEMORY_adr[dkA][0][E] = MEMORY_adr[dkA][0][E]<<8;
				MEMORY_adr[dkA][0][E]+= WFORMDYNAMIC[dkA][StPosHead+5];
				MEMORY_adr[dkA][0][E] = MEMORY_adr[dkA][0][E]<<8;
				MEMORY_adr[dkA][0][E]+= WFORMDYNAMIC[dkA][StPosHead+6];
				MEMORY_adr[dkA][0][E] = MEMORY_adr[dkA][0][E]<<8;	
				MEMORY_adr[dkA][0][E]+= WFORMDYNAMIC[dkA][StPosHead+7];	
				if(WFORMDYNAMIC[dkA][StPosHead]==2)						//when hot cue type=loop
					{
					MEMORY_type[dkA][E] |= 0x1;		
					MEMORY_adr[dkA][1][E] = WFORMDYNAMIC[dkA][StPosHead+8];
					MEMORY_adr[dkA][1][E] = MEMORY_adr[dkA][1][E]<<8;
					MEMORY_adr[dkA][1][E]+= WFORMDYNAMIC[dkA][StPosHead+9];
					MEMORY_adr[dkA][1][E] = MEMORY_adr[dkA][1][E]<<8;
					MEMORY_adr[dkA][1][E]+= WFORMDYNAMIC[dkA][StPosHead+10];
					MEMORY_adr[dkA][1][E] = MEMORY_adr[dkA][1][E]<<8;	
					MEMORY_adr[dkA][1][E]+= WFORMDYNAMIC[dkA][StPosHead+11];	
					MEMORY_adr[dkA][1][E] = (MEMORY_adr[dkA][1][E]*3)/20; 	//translate ms to 1/150s frames
					}
				StPosHead = SPP;	
				E++;	
				}
			}
		path_ANLZ[42] = 69;								//	
		path_ANLZ[43] = 88;								//	Replace *.DAT to *.EXT
		path_ANLZ[44] = 84;								//	for open EXT file with dynamic waveform	
		path_ANLZ[45] = 0;
			
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "\n\r");	
		UART_TX(&huart4, U_TX_DATA, 2, 5);		
		UART_TX(&huart4, path_ANLZ, 45, 15);	
		UART_TX(&huart4, U_TX_DATA, 2, 5);		
		#endif		
	
		res = f_open(&file, path_ANLZ, FA_READ);	
		if (res != FR_OK)
			{
			ERROR = 7;	//cannot open ANLZXXXX.EXT file
			}
		else
			{
			FILSIZE = f_size(&file);
			fsz = FILSIZE;
			if(fsz>WFD_SIZE)
				{
				fsz = WFD_SIZE;	
				}
			res = f_read(&file, WFORMDYNAMIC[dkA], fsz, &nbytes);
			if(res != FR_OK)
				{
				ERROR = 8;	//ANLZXXXX.EXT file is damadge
				f_close(&file);					//Close file ANLZXXXX.EXT							
				}
			else
				{
				f_close(&file);					//Close file ANLZXXXX.EXT					
				fsz = WFORMDYNAMIC[dkA][8];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[dkA][9];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkA][10];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkA][11];	//file size		
				if(fsz != FILSIZE)
					{
					ERROR = 9;	//ANLZXXXX.EXT file is damadge
					}
				else		
					{	
					fsz = WFORMDYNAMIC[dkA][4];			
					fsz<<=8;	
					fsz+=WFORMDYNAMIC[dkA][5];	
					fsz<<=8;
					fsz+=WFORMDYNAMIC[dkA][6];	
					fsz<<=8;
					fsz+=WFORMDYNAMIC[dkA][7];	//Header size
					StPosHead = fsz;	
					if(WFORMDYNAMIC[dkA][StPosHead] != 80 | 
						 WFORMDYNAMIC[dkA][StPosHead+1] != 80 | 
					   WFORMDYNAMIC[dkA][StPosHead+2] != 84 | 
					   WFORMDYNAMIC[dkA][StPosHead+3] != 72)		//Check PPTH position in file
						{
						ERROR = 10;	//ANLZXXXX.EXT file is damadge				
						}
					else
						{		
						fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
						fsz<<=8;	
						fsz+=WFORMDYNAMIC[dkA][StPosHead+9];	
						fsz<<=8;
						fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
						fsz<<=8;
						fsz+=WFORMDYNAMIC[dkA][StPosHead+11];	//Tag size
						StPosHead += fsz;		
						if(WFORMDYNAMIC[dkA][StPosHead] != 80 | 
							 WFORMDYNAMIC[dkA][StPosHead+1] != 87 | 
						   WFORMDYNAMIC[dkA][StPosHead+2] != 86 | 
						   WFORMDYNAMIC[dkA][StPosHead+3] != 51)		//Check PWV3 position in file
							{
							return 11;	//ANLZXXXX.EXT file is damadge			
							}
						else
							{	
							fsz = WFORMDYNAMIC[dkA][StPosHead+4];			
							fsz<<=8;	
							fsz+=WFORMDYNAMIC[dkA][StPosHead+5];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[dkA][StPosHead+6];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[dkA][StPosHead+7];	//Header size
							uint32_t START_POS_WF = StPosHead + fsz; //Start position waveform	 //old SPP
									
							fsz = WFORMDYNAMIC[dkA][StPosHead+16];			
							fsz<<=8;	
							fsz+=WFORMDYNAMIC[dkA][StPosHead+17];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[dkA][StPosHead+18];	
							fsz<<=8;
							uint32_t WF_DATA_SIZE = fsz+WFORMDYNAMIC[dkA][StPosHead+19];		//waveform data size	//old fsz
							uint32_t j;							//Data shift	
								
							fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
							fsz<<=8;	
							fsz+=WFORMDYNAMIC[dkA][StPosHead+9];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[dkA][StPosHead+11];	//Tag size
							StPosHead += fsz;		
							if(WFORMDYNAMIC[dkA][StPosHead] != 80 | 
								 WFORMDYNAMIC[dkA][StPosHead+1] != 67 | 
								 WFORMDYNAMIC[dkA][StPosHead+2] != 79 | 
								 WFORMDYNAMIC[dkA][StPosHead+3] != 66)		//Check PCOB position in file
								{
								return 11;	//ANLZXXXX.EXT file is damadge			
								}
							else
								{
								fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
								fsz<<=8;	
								fsz+=WFORMDYNAMIC[dkA][StPosHead+9];	
								fsz<<=8;
								fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
								fsz<<=8;
								fsz+=WFORMDYNAMIC[dkA][StPosHead+11];	//Tag size
								StPosHead += fsz;		
								if(WFORMDYNAMIC[dkA][StPosHead] != 80 | 
								 WFORMDYNAMIC[dkA][StPosHead+1] != 67 | 
								 WFORMDYNAMIC[dkA][StPosHead+2] != 79 | 
								 WFORMDYNAMIC[dkA][StPosHead+3] != 66)		//Check PCO2 position in file
									{
									return 11;	//ANLZXXXX.EXT file is damadge			
									}	
								else
									{
									fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
									fsz<<=8;	
									fsz+=WFORMDYNAMIC[dkA][StPosHead+9];	
									fsz<<=8;
									fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
									fsz<<=8;
									fsz+=WFORMDYNAMIC[dkA][StPosHead+11];	//Tag size
									StPosHead += fsz;		
									if(WFORMDYNAMIC[dkA][StPosHead] != 80 | 
									 WFORMDYNAMIC[dkA][StPosHead+1] != 67 | 
									 WFORMDYNAMIC[dkA][StPosHead+2] != 79 | 
									 WFORMDYNAMIC[dkA][StPosHead+3] != 50)		//Check PCO2 position in file
										{
										return 11;	//ANLZXXXX.EXT file is damadge			
										}	
									else
										{
										#if defined(DEBUG_UART_EN)		
										sprintf((char*)U_TX_DATA, "Enter PCO2\n\r");	
										UART_TX(&huart4, U_TX_DATA, 12, 5);	
										#endif		
											
											
										if(WFORMDYNAMIC[dkA][StPosHead+15]==1)				//HOT CUE TYPE
											{
											number_of_hot_cue_points[dkA] = WFORMDYNAMIC[dkA][StPosHead+17];
											fsz = WFORMDYNAMIC[dkA][StPosHead+4];			
											fsz<<=8;	
											fsz+=WFORMDYNAMIC[dkA][StPosHead+5];	
											fsz<<=8;
											fsz+=WFORMDYNAMIC[dkA][StPosHead+6];	
											fsz<<=8;
											fsz+=WFORMDYNAMIC[dkA][StPosHead+7];	//header size
											StPosHead += fsz;	
											
											for(j=0;j<number_of_hot_cue_points[dkA];j++)			
												{	
												if(WFORMDYNAMIC[dkA][StPosHead] != 80 | 
													 WFORMDYNAMIC[dkA][StPosHead+1] != 67 | 
													 WFORMDYNAMIC[dkA][StPosHead+2] != 80 | 
													 WFORMDYNAMIC[dkA][StPosHead+3] != 50)		//Check PCP2 position in file
														{
														j = 100;	
														return 11;	//ANLZXXXX.EXT file is damadge			
														}
												else
													{
													#if defined(DEBUG_UART_EN)		
													sprintf((char*)U_TX_DATA, "Extract HOT CUE\n\r");	
													UART_TX(&huart4, U_TX_DATA, 17, 5);	
													#endif		
													if(WFORMDYNAMIC[dkA][StPosHead+15]>0)
														{
														HCUE_adr[dkA][0][WFORMDYNAMIC[dkA][StPosHead+15]-1] = WFORMDYNAMIC[dkA][StPosHead+20]; 		
														HCUE_adr[dkA][0][WFORMDYNAMIC[dkA][StPosHead+15]-1]<<=8;	
														HCUE_adr[dkA][0][WFORMDYNAMIC[dkA][StPosHead+15]-1]+=WFORMDYNAMIC[dkA][StPosHead+21];
														HCUE_adr[dkA][0][WFORMDYNAMIC[dkA][StPosHead+15]-1]<<=8;	
														HCUE_adr[dkA][0][WFORMDYNAMIC[dkA][StPosHead+15]-1]+=WFORMDYNAMIC[dkA][StPosHead+22];	
														HCUE_adr[dkA][0][WFORMDYNAMIC[dkA][StPosHead+15]-1]<<=8;	
														HCUE_adr[dkA][0][WFORMDYNAMIC[dkA][StPosHead+15]-1]+=WFORMDYNAMIC[dkA][StPosHead+23];
														if(WFORMDYNAMIC[dkA][StPosHead+16]==2)						//when hot cue type=loop
															{
															HCUE_adr[dkA][1][WFORMDYNAMIC[dkA][StPosHead+15]-1] = WFORMDYNAMIC[dkA][StPosHead+24]; 		
															HCUE_adr[dkA][1][WFORMDYNAMIC[dkA][StPosHead+15]-1]<<=8;	
															HCUE_adr[dkA][1][WFORMDYNAMIC[dkA][StPosHead+15]-1]+=WFORMDYNAMIC[dkA][StPosHead+25];
															HCUE_adr[dkA][1][WFORMDYNAMIC[dkA][StPosHead+15]-1]<<=8;	
															HCUE_adr[dkA][1][WFORMDYNAMIC[dkA][StPosHead+15]-1]+=WFORMDYNAMIC[dkA][StPosHead+26];	
															HCUE_adr[dkA][1][WFORMDYNAMIC[dkA][StPosHead+15]-1]<<=8;	
															HCUE_adr[dkA][1][WFORMDYNAMIC[dkA][StPosHead+15]-1]+=WFORMDYNAMIC[dkA][StPosHead+27];	
															}
														E=WFORMDYNAMIC[dkA][StPosHead+42];	
														E<<=8;
														E+=WFORMDYNAMIC[dkA][StPosHead+43];	//len_comment	
														HCUE32COLOR[dkA][WFORMDYNAMIC[dkA][StPosHead+15]-1] = WFORMDYNAMIC[dkA][StPosHead+45+E];																
														HCUE32COLOR[dkA][WFORMDYNAMIC[dkA][StPosHead+15]-1]<<=8;	
														HCUE32COLOR[dkA][WFORMDYNAMIC[dkA][StPosHead+15]-1]+= WFORMDYNAMIC[dkA][StPosHead+46+E];		
														HCUE32COLOR[dkA][WFORMDYNAMIC[dkA][StPosHead+15]-1]<<=8;		
														HCUE32COLOR[dkA][WFORMDYNAMIC[dkA][StPosHead+15]-1]+= WFORMDYNAMIC[dkA][StPosHead+47+E];	
														}														
													fsz = WFORMDYNAMIC[dkA][StPosHead+8];			
													fsz<<=8;	
													fsz+=WFORMDYNAMIC[dkA][StPosHead+9];	
													fsz<<=8;
													fsz+=WFORMDYNAMIC[dkA][StPosHead+10];	
													fsz<<=8;
													fsz+=WFORMDYNAMIC[dkA][StPosHead+11];	//len_entry
													StPosHead += fsz;		
													}													
												}												
											}
											
					///////add memory cues reading
											
									if(WF_DATA_SIZE>1)				//delete 2 end frames (exclude noise) 
											{
											WF_DATA_SIZE-=2;	
											}
										else
											{
											WF_DATA_SIZE = 0;	
											}									
										all_long[dkA] = WF_DATA_SIZE;
//										if(all_long[dkA]>57009)		//crop all long data for sldz
//											{
//											all_long[dkA] = 57009;	
//											}
											
										if(WF_DATA_SIZE>(WFD_SIZE-START_POS_WF))
											{
											WF_DATA_SIZE = WFD_SIZE-START_POS_WF;	
											}

//										if(WF_DATA_SIZE>57008)		//crop data for sldz		
//											{
//											WF_DATA_SIZE = 57008;	//max audio lenght 	
//											}								
											
										for(j=0;j<WF_DATA_SIZE;j++)			//Fill dynamic waveform
											{
											//WFORMDYNAMIC[dkA][j] = WFORMDYNAMIC[dkA][j+SPP];	
											//optimization with color coefficient:
											a = WFORMDYNAMIC[dkA][j+START_POS_WF]&0x1F;
											a*= 153;
											a>>=8;											
											WFORMDYNAMIC[dkA][j] = WFORMDYNAMIC[dkA][j+START_POS_WF]&0xE0;
											WFORMDYNAMIC[dkA][j]|=a; 	
											}
											
//										for(j=0;j<(WF_DATA_SIZE>>1);j++)			//Fill dynamic waveform x2
//											{
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[dkA][wfmx2+(j/2)] = (WFORMDYNAMIC[dkA][j]&0xF0);	
//												}
//											else
//												{
//												WFORMDYNAMIC[dkA][wfmx2+(j/2)]+= WFORMDYNAMIC[dkA][j]>>4;	
//												}											
//											}	
//										for(j=0;j<(WF_DATA_SIZE>>2);j++)			//Fill dynamic waveform x4
//											{
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[dkA][wfmx4+(j/2)] = (WFORMDYNAMIC[dkA][2*j]&0xF0);
//												}
//											else
//												{
//												WFORMDYNAMIC[dkA][wfmx4+(j/2)]+= WFORMDYNAMIC[dkA][2*j]>>4;		
//												}											
//											}		
//										for(j=0;j<(WF_DATA_SIZE>>3);j++)			//Fill dynamic waveform x8
//											{
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[dkA][wfmx8+(j/2)] = (WFORMDYNAMIC[dkA][4*j]&0xF0);	
//												}
//											else
//												{
//												WFORMDYNAMIC[dkA][wfmx8+(j/2)]+= WFORMDYNAMIC[dkA][4*j]>>4;	
//												}											
//											}
//										for(j=0;j<(WF_DATA_SIZE>>4);j++)			//Fill dynamic waveform x16
//											{
//											wpos = 8;	
//											for(E=0;E<8;E++)			
//												{		
//												wpos+=WFORMDYNAMIC[dkA][8*j+E]&0x0F;
//												wpos+=WFORMDYNAMIC[dkA][8*j+E]>>4;	
//												}
//											wpos>>=4;	
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[dkA][wfmx16+(j/2)] = (wpos<<4);	
//												}
//											else
//												{
//												WFORMDYNAMIC[dkA][wfmx16+(j/2)]+= wpos&0x0F;	
//												}													
//											}		
//										for(j=0;j<(WF_DATA_SIZE>>5);j++)			//Fill dynamic waveform x32
//											{
//											wpos = 16;	
//											for(E=0;E<16;E++)			
//												{		
//												wpos+=WFORMDYNAMIC[dkA][16*j+E]&0x0F;
//												wpos+=WFORMDYNAMIC[dkA][16*j+E]>>4;	
//												}
//											wpos>>=5;	
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[dkA][wfmx32+(j/2)] = (wpos<<4);	
//												}
//											else
//												{
//												WFORMDYNAMIC[dkA][wfmx32+(j/2)]+= wpos&0x0F;	
//												}											
//											}			
											
										for(E=0;E<number_of_memory_cue_points[dkA];E++)											//Draw CUES on Display
											{
											if(MEMORY_adr[dkA][0][E] != 0xFFFF)
												{
												#if defined(DEBUG_UART_EN)		
												sprintf((char*)U_TX_DATA, "MEMORY in %06lu ms\n\r", MEMORY_adr[dkA][0][E]);											
												UART_TX(&huart4, U_TX_DATA, 21, 15);	
												#endif		
												mem_pos	= 609*MEMORY_adr[dkA][0][E];
												mem_pos/= (20*all_long[dkA]);
												DrawMemoryMarker(dkA, mem_pos, MEMORY_MARK, LCD_COLOR_RED);
												MEMORY_adr[dkA][0][E] = (MEMORY_adr[dkA][0][E]*3)/20;				//translate ms to 1/150s frames
												}	
											}	

										////////////////////////////////////sorting algoritm	
										uint8_t MINM, ii;
															
										for(E=0;E<number_of_memory_cue_points[dkA]-1;E++)	
											{
											MINM = E;
											for(ii=(E+1);ii<number_of_memory_cue_points[dkA];ii++)
												{
												if(MEMORY_adr[dkA][0][ii]<MEMORY_adr[dkA][0][MINM])
													{
													j = MEMORY_adr[dkA][0][MINM];
													MEMORY_adr[dkA][0][MINM] = MEMORY_adr[dkA][0][ii];
													MEMORY_adr[dkA][0][ii] = j;
													j = MEMORY_adr[dkA][1][MINM];
													MEMORY_adr[dkA][1][MINM] = MEMORY_adr[dkA][1][ii];
													MEMORY_adr[dkA][1][ii] = j;												
													j = MEMORY_type[dkA][MINM];
													MEMORY_type[dkA][MINM] = MEMORY_type[dkA][ii];
													MEMORY_type[dkA][ii] = j;	
													}
												}
											}

										uint8_t	r, g, b;	
										for(E=0;E<8;E++)											//Draw MEMORY on Display
											{
											if(HCUE_adr[dkA][0][E] != 0xFFFF)
												{
												HCUEPCOLOR[dkA][E][0] = ((HCUE32COLOR[dkA][E]&0x00FFFFFF)>>16)/3;	
												HCUEPCOLOR[dkA][E][1] =	((HCUE32COLOR[dkA][E]&0x0000FFFF)>>8)/3;
												HCUEPCOLOR[dkA][E][2] = (HCUE32COLOR[dkA][E]&0x000000FF)/3;
												r = (HCUE32COLOR[dkA][E]&0x00FFFFFF)>>19;	
												g = (HCUE32COLOR[dkA][E]&0x0000FFFF)>>11;
												b = (HCUE32COLOR[dkA][E]&0x000000FF)>>3;	
												HCUE16COLOR[dkA][E] = 0x8000+(0x400*r)+(0x20*g)+b; 
												mem_pos	= 609*HCUE_adr[dkA][0][E];
												mem_pos/= (20*all_long[dkA]);
												DrawMemoryMarker(dkA, mem_pos, 2+E, HCUE16COLOR[dkA][E]);				//square
												#if defined(DEBUG_UART_EN)		
												sprintf((char*)U_TX_DATA, "H.CUE in %06lu ms  ", HCUE_adr[dkA][0][E]);											
												UART_TX(&huart4, U_TX_DATA, 20, 15);												
												#endif		
													
												HCUE_adr[dkA][0][E] = (HCUE_adr[dkA][0][E]*3)/20;				//translate ms to 1/150s frames										
												if(E==0)							//HCUE_A
													{
													if(HCUE_type[dkA][E]&0x1)
														{
														///Tbuffer[18] |= 0x1;	
														///Tbuffer[17] &= 0x7F;	
														}
													else
														{
														///Tbuffer[17] |= 0x80;
														///Tbuffer[18] &= 0x7E;	
														}
													}
												else if(E==1)					//HCUE_B
													{
													if(HCUE_type[dkA][E]&0x1)
														{
														///Tbuffer[18] |= 0x8;	
														///Tbuffer[18] &= 0x7B;	
														}
													else
														{	
														///Tbuffer[18] |= 0x4;
														///Tbuffer[18] &= 0x77;	
														}
													}	
												else if(E==2)					//HCUE_C
													{
													if(HCUE_type[dkA][E]&0x1)
														{
														///Tbuffer[18] |= 0x40;		
														///Tbuffer[18] &= 0x5F;	
														}
													else
														{	
														///Tbuffer[18] |= 0x20;
														///Tbuffer[18] &= 0x3F;	
														}
													}	
												}
											else				//turn off leds
												{
												HCUEPCOLOR[dkA][E][0] = HCUEdisableCOLOR[0];	
												HCUEPCOLOR[dkA][E][1] =	HCUEdisableCOLOR[1];
												HCUEPCOLOR[dkA][E][2] = HCUEdisableCOLOR[2];
												}									
											}
										}									
									}									
								}
							}
						}
					}			
				}	
			}

		res = f_open(&file, path_AUDIOTRACK, FA_READ);				//Open audio file
		if (res != FR_OK)
			{
			return 13;	//cannot open AUDIOTRACK
			}
		#if defined(DEBUG_UART_EN)	
		sprintf((char*)U_TX_DATA, "\n\r");	
		UART_TX(&huart4, U_TX_DATA, 2, 5);				
		UART_TX(&huart4, path_AUDIOTRACK, 150, 45);	
		UART_TX(&huart4, U_TX_DATA, 2, 5);	
		#endif		
		res = f_read(&file, PCM, 512, &nbytes);
		if(res != FR_OK)
			{
			ERROR = 14;	//cannot read AUDIOTRACK
 			}
		#if defined(DEBUG_UART_EN)	
		sprintf((char*)U_TX_DATA, "\n\r");	
		UART_TX(&huart4, U_TX_DATA, 2, 5);				
		UART_TX(&huart4, (uint8_t *)PCM, 50, 45);	
		UART_TX(&huart4, U_TX_DATA, 2, 5);	
		#endif		
			
		if(PCM[dkA][0][5][0] != 1 || PCM[dkA][0][5][1] != 2 || PCM[dkA][0][6][0] != 44100 || PCM[dkA][0][8][1] != 16)	//Check audio format
			{
			ERROR = 15;	//unsupported audio format	
			}
		f_lseek(&file, 44);
		if(ERROR==0)
			{
			playlist[TRACK_NUMBER-1][54] |= 0x01;				//write history mem
			}
		curr_trck_color[dkA] = (rating[TRACK_NUMBER-1]&0x0F);	
		return ERROR;	
		};
		
////////////////////////////////////////////////////////////////////////
//	open export.pdb file, extract path for ANLZXXXX.DAT file
//	open ANLZXXXX.DAT file, extract path for audio file
//	extract static waveform data, bpm	
//	open ANLZXXXX.EXT file, extract dynamic waveform data, all_long[dkB] data
//	open audio file
//
//	output: error code
////////////////////////////////////////////////////////////////////////	
uint16_t LOAD_TRACKB(uint16_t TRACK_NUMBER)
		{
		if(TRACK_NUMBER==0)
			{
			return 1;				//invalid track number
			}	
		uint16_t ERROR = 0;
		uint16_t E=0;
		uint32_t mem_pos;	
		res = f_open(&fileb, path_export, FA_READ);
		if (res != FR_OK)
			{
			return 2;	//cannot open database
			}
		f_lseek(&fileb, (parcser_adress[TRACK_NUMBER-1]-42));			//find 0.DAT in file		
		char path_ANLZ[46];
		res = f_read(&fileb, path_ANLZ, sizeof(path_ANLZ), &nbytesb);
		if (res != FR_OK)
			{
			return 3;		//data not read
			}					
		f_close(&fileb);					//Close file Export.pdb
		path_ANLZ[0] = 48;
		path_ANLZ[1] = 58;
		path_ANLZ[45] = 0;
			
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "\n\r");	
		UART_TX(&huart4, U_TX_DATA, 2, 5);		
		UART_TX(&huart4, path_ANLZ, 45, 15);	
		UART_TX(&huart4, U_TX_DATA, 2, 5);		
		#endif		
			
		res = f_open(&fileb, path_ANLZ, FA_READ);
		if (res != FR_OK)
			{	
			return 4;	//cannot open ANLZXXXX.DAT file
			}
		uint32_t FILSIZE = f_size(&fileb);
		if(FILSIZE>WFD_SIZE)
			{
			return 26;	
			}
		res = f_read(&fileb, WFORMDYNAMIC[dkB], FILSIZE, &nbytesb);
		if (res != FR_OK)
			{
			return 5;	//cannot read ANLZXXXX.DAT file		
			}
		f_close(&fileb);					//Close file ANLZXXXX.DAT		
		uint32_t fsz;	
		fsz = WFORMDYNAMIC[dkB][8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkB][9];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][11];
		if(fsz != FILSIZE)
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[dkB][4];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][5];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][6];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][7];
		uint32_t StPosHead = fsz;		
		if(WFORMDYNAMIC[dkB][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkB][StPosHead+1] != 80 ||
			 WFORMDYNAMIC[dkB][StPosHead+2] != 84 || 
			 WFORMDYNAMIC[dkB][StPosHead+3] != 72)		//Check PPHT position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[dkB][StPosHead+4];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+5];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+6];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+7];								//head size
		uint32_t SPP = fsz+StPosHead+1;	
		fsz = WFORMDYNAMIC[dkB][StPosHead+12];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkB][StPosHead+13];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+14];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+15];							//Path Size
		char path_AUDIOTRACK[(fsz/2)+2];			//Create a Path for audiotrack
		while(E<(fsz+4))
			{
			path_AUDIOTRACK[(E/2)+2] = WFORMDYNAMIC[dkB][SPP+E];			//Fill path
			E=E+2;	
			}
		path_AUDIOTRACK[0] = 48;
		path_AUDIOTRACK[1] = 58;	
		fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+9];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+10];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+11];	
		StPosHead = StPosHead+fsz;	//PVBR position
		if(WFORMDYNAMIC[dkB][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkB][StPosHead+1] != 86 ||
			 WFORMDYNAMIC[dkB][StPosHead+2] != 66 || 
			 WFORMDYNAMIC[dkB][StPosHead+3] != 82)	//Check PVBR position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkB][StPosHead+9];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+11];		
		StPosHead = StPosHead+fsz;	//PQTZ position
		if(WFORMDYNAMIC[dkB][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkB][StPosHead+1] != 81 ||
			 WFORMDYNAMIC[dkB][StPosHead+2] != 84 || 
			 WFORMDYNAMIC[dkB][StPosHead+3] != 90)	//Check PQTZ position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[dkB][StPosHead+4];				
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkB][StPosHead+5];		
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+6];		
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+7];				//Head size
		uint32_t number_of_entries;
		number_of_entries = WFORMDYNAMIC[dkB][StPosHead+20];				
		number_of_entries<<=8;	
		number_of_entries+=WFORMDYNAMIC[dkB][StPosHead+21];		
		number_of_entries<<=8;
		number_of_entries+=WFORMDYNAMIC[dkB][StPosHead+22];		
		number_of_entries<<=8;
		number_of_entries+=WFORMDYNAMIC[dkB][StPosHead+23];				//calculate number_of_entries
		if(number_of_entries>2048)
			{
			number_of_entries = 2048;	
			}
		SPP = StPosHead + fsz + 2;						//start first BPM data.
		E = 0;
		GRID_OFFSET[dkB] = WFORMDYNAMIC[dkB][SPP-1];							//find first beat 1...4
		while(E<number_of_entries)
			{
			BPMGRID[dkB][E] = WFORMDYNAMIC[dkB][SPP+(E*8)];	
			BPMGRID[dkB][E]<<=8;
			BPMGRID[dkB][E]+= WFORMDYNAMIC[dkB][SPP+1+(E*8)];		
			BEATGRID[dkB][E] = WFORMDYNAMIC[dkB][SPP+2+(E*8)];	
			BEATGRID[dkB][E]<<=8;
			BEATGRID[dkB][E]+= WFORMDYNAMIC[dkB][SPP+3+(E*8)];	
			BEATGRID[dkB][E]<<=8;
			BEATGRID[dkB][E]+= WFORMDYNAMIC[dkB][SPP+4+(E*8)];	
			BEATGRID[dkB][E]<<=8;
			BEATGRID[dkB][E]+= WFORMDYNAMIC[dkB][SPP+5+(E*8)];	
			E++;	
			}
		if(E==2048)
			{
			BEATGRID[dkB][2047] = 0xFFFFFFFF;	
			}
		else
			{
			BEATGRID[dkB][E] = 0xFFFFFFFF;	
			BPMGRID[dkB][E] = BPMGRID[dkB][E-1];	
			}
		originalBPM[dkB] = BPMGRID[dkB][0];			//SEND ORIGINAL BPM		
		fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkB][StPosHead+9];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+11];	
		StPosHead = StPosHead+fsz;	//PWAV position	
		if(WFORMDYNAMIC[dkB][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkB][StPosHead+1] != 87 ||
			 WFORMDYNAMIC[dkB][StPosHead+2] != 65 || 
			 WFORMDYNAMIC[dkB][StPosHead+3] != 86)		//Check PWAV position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[dkB][StPosHead+4];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+5];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+6];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+7];				//PWAV Header size
		fsz+=StPosHead;			
		uint16_t ampl;
		uint32_t y;	

		for(E=0;E<203;E++)					//Fill Static Waveform 400->203
			{
			y = 506*E;			//506
			y>>=8;	
			ampl = (WFORMDYNAMIC[dkB][fsz+y]&0x1F)*192;	 //convert amplitude 24->18
			ampl>>=8;
			if(ampl>18)
				{
				ampl = 18;	
				}
			WFORMSTATIC[dkB][E] = WFORMDYNAMIC[dkB][fsz+y]&0x80;	//color	
			WFORMSTATIC[dkB][E]|= ampl;	
			}

		fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkB][StPosHead+9];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+11];				//PWAV TAG size
		StPosHead+=fsz;													//PWV2 start adress
		if(WFORMDYNAMIC[dkB][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkB][StPosHead+1] != 87 ||
			 WFORMDYNAMIC[dkB][StPosHead+2] != 86 || 
			 WFORMDYNAMIC[dkB][StPosHead+3] != 50)		//Check PWV2 position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[dkB][StPosHead+9];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[dkB][StPosHead+11];				//PWV2 Tag size
		StPosHead = StPosHead+fsz;	//PCOB position				
		if(WFORMDYNAMIC[dkB][StPosHead] != 80 ||
			 WFORMDYNAMIC[dkB][StPosHead+1] != 67 ||
			 WFORMDYNAMIC[dkB][StPosHead+2] != 79 || 
			 WFORMDYNAMIC[dkB][StPosHead+3] != 66)		//Check PCOB position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}		
			
		DrawMemoryMarker(dkB, 0xFF, NONE_MARK, 0x8000);
	
		uint32_t PCOB2_adr;
			
		//////////////////prepare HOT CUES	
		for(E=0;E<8;E++)									//Clear ícue massive
			{
			HCUE_type[dkB][E] = 0xFF;	 							//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 		
			HCUE_adr[dkB][0][E] = 0xFFFF;		
			HCUE_adr[dkB][1][E] = 0xFFFF;	
			HCUE32COLOR[dkB][E] = 0xFFFF;	
			}
		
		if(WFORMDYNAMIC[dkB][StPosHead+15]==1)    //check type PCOB - for HOT CUE points
			{
			number_of_hot_cue_points[dkB] = WFORMDYNAMIC[dkB][StPosHead+19]&0xF;				//number of hotcue points		
//			if(number_of_hot_cue_points[dkB]>0)
//				{
//				MemoryCuePyramid_ENABLE = 3;	
//				}
			fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
			fsz<<=8;	
			fsz+=WFORMDYNAMIC[dkB][StPosHead+9];
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkB][StPosHead+11];				//PCOB TAG size
			PCOB2_adr = fsz+StPosHead;							//start adress PCOB2
			fsz = WFORMDYNAMIC[dkB][StPosHead+4];			
			fsz<<=8;	
			fsz+=WFORMDYNAMIC[dkB][StPosHead+5];
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkB][StPosHead+6];	
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkB][StPosHead+7];				//PCOB head size
			StPosHead+= fsz;		
			uint8_t HCUE_NAME = 0;
			E = 0;	
			while(E<number_of_hot_cue_points[dkB])	
				{	
				fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[dkB][StPosHead+9];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkB][StPosHead+11];				//PCPT TAG size
				SPP = StPosHead+fsz;										//Next PCPT adress

				HCUE_NAME = WFORMDYNAMIC[dkB][StPosHead+15];
				if(WFORMDYNAMIC[dkB][StPosHead+19]!=0)				//when hcue active
					{
					if((HCUE_NAME<4) && (HCUE_NAME>0))
						{
						HCUE_type[dkB][HCUE_NAME-1] = 0x02;			//write 
						}
					}
				fsz = WFORMDYNAMIC[dkB][StPosHead+4];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[dkB][StPosHead+5];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkB][StPosHead+6];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkB][StPosHead+7];				//PCPT Head size
				StPosHead+= fsz;		
		
				if((HCUE_NAME<4) && (HCUE_NAME>0))
					{
					HCUE_NAME--;
					HCUE_adr[dkB][0][HCUE_NAME] = WFORMDYNAMIC[dkB][StPosHead+4];
					HCUE_adr[dkB][0][HCUE_NAME] = HCUE_adr[dkB][0][HCUE_NAME]<<8;
					HCUE_adr[dkB][0][HCUE_NAME]+= WFORMDYNAMIC[dkB][StPosHead+5];
					HCUE_adr[dkB][0][HCUE_NAME] = HCUE_adr[dkB][0][HCUE_NAME]<<8;
					HCUE_adr[dkB][0][HCUE_NAME]+= WFORMDYNAMIC[dkB][StPosHead+6];
					HCUE_adr[dkB][0][HCUE_NAME] = HCUE_adr[dkB][0][HCUE_NAME]<<8;	
					HCUE_adr[dkB][0][HCUE_NAME]+= WFORMDYNAMIC[dkB][StPosHead+7];
					if(WFORMDYNAMIC[dkB][StPosHead]==2)						//when hot cue type=loop
						{
						HCUE_type[dkB][HCUE_NAME]&= 0x03;	
						HCUE_type[dkB][HCUE_NAME] |= 0x01;		
						HCUE_adr[dkB][1][HCUE_NAME] = WFORMDYNAMIC[dkB][StPosHead+8];
						HCUE_adr[dkB][1][HCUE_NAME] = HCUE_adr[dkB][1][HCUE_NAME]<<8;
						HCUE_adr[dkB][1][HCUE_NAME]+= WFORMDYNAMIC[dkB][StPosHead+9];
						HCUE_adr[dkB][1][HCUE_NAME] = HCUE_adr[dkB][1][HCUE_NAME]<<8;
						HCUE_adr[dkB][1][HCUE_NAME]+= WFORMDYNAMIC[dkB][StPosHead+10];
						HCUE_adr[dkB][1][HCUE_NAME] = HCUE_adr[dkB][1][HCUE_NAME]<<8;	
						HCUE_adr[dkB][1][HCUE_NAME]+= WFORMDYNAMIC[dkB][StPosHead+11];
						HCUE_adr[dkB][1][HCUE_NAME] = (HCUE_adr[dkB][1][HCUE_NAME]*3)/20; 	//translate ms to 1/150s frames
						}
					}
				StPosHead = SPP;	
				E++;	
				}
			}
						
		//////////////////prepare MEMORY CUES
		for(E=0;E<10;E++)									//Clear memory cue massive
			{
			MEMORY_type[dkB][E] = 0;						//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 		
			MEMORY_adr[dkB][0][E] = 0xFFFF;
			MEMORY_adr[dkB][1][E] = 0xFFFF;	
			}
		StPosHead = PCOB2_adr;	
		if(WFORMDYNAMIC[dkB][StPosHead+15]==0)    //check type PCOB - for MEMORY CUE points
			{
			number_of_memory_cue_points[dkB] = WFORMDYNAMIC[dkB][StPosHead+19]&0xF;				//number of MEMORY points		
//			if(number_of_memory_cue_points[dkB]>0)
//				{
//				MemoryCuePyramid_ENABLE = 3;	
//				}
			fsz = WFORMDYNAMIC[dkB][StPosHead+4];			
			fsz<<=8;	
			fsz+=WFORMDYNAMIC[dkB][StPosHead+5];
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkB][StPosHead+6];	
			fsz<<=8;
			fsz+=WFORMDYNAMIC[dkB][StPosHead+7];				//PCOB head size
			StPosHead = StPosHead+fsz;		

			E = 0;	
			while(E<number_of_memory_cue_points[dkB])	
				{	
				fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkB][StPosHead+9];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkB][StPosHead+11];				//PCPT TAG size
				SPP = StPosHead+fsz;										//Next PCPT adress

				if(WFORMDYNAMIC[dkB][StPosHead+19]!=0)				//when hcue active
					{
					MEMORY_type[dkB][E] = 2;			//write 
					}	
				fsz = WFORMDYNAMIC[dkB][StPosHead+4];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[dkB][StPosHead+5];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkB][StPosHead+6];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkB][StPosHead+7];				//PCPT Head size
				StPosHead = StPosHead+fsz;		
		
				MEMORY_adr[dkB][0][E] = WFORMDYNAMIC[dkB][StPosHead+4];
				MEMORY_adr[dkB][0][E] = MEMORY_adr[dkB][0][E]<<8;
				MEMORY_adr[dkB][0][E]+= WFORMDYNAMIC[dkB][StPosHead+5];
				MEMORY_adr[dkB][0][E] = MEMORY_adr[dkB][0][E]<<8;
				MEMORY_adr[dkB][0][E]+= WFORMDYNAMIC[dkB][StPosHead+6];
				MEMORY_adr[dkB][0][E] = MEMORY_adr[dkB][0][E]<<8;	
				MEMORY_adr[dkB][0][E]+= WFORMDYNAMIC[dkB][StPosHead+7];	
				if(WFORMDYNAMIC[dkB][StPosHead]==2)						//when hot cue type=loop
					{
					MEMORY_type[dkB][E] |= 0x1;		
					MEMORY_adr[dkB][1][E] = WFORMDYNAMIC[dkB][StPosHead+8];
					MEMORY_adr[dkB][1][E] = MEMORY_adr[dkB][1][E]<<8;
					MEMORY_adr[dkB][1][E]+= WFORMDYNAMIC[dkB][StPosHead+9];
					MEMORY_adr[dkB][1][E] = MEMORY_adr[dkB][1][E]<<8;
					MEMORY_adr[dkB][1][E]+= WFORMDYNAMIC[dkB][StPosHead+10];
					MEMORY_adr[dkB][1][E] = MEMORY_adr[dkB][1][E]<<8;	
					MEMORY_adr[dkB][1][E]+= WFORMDYNAMIC[dkB][StPosHead+11];	
					MEMORY_adr[dkB][1][E] = (MEMORY_adr[dkB][1][E]*3)/20; 	//translate ms to 1/150s frames
					}
				StPosHead = SPP;	
				E++;	
				}
			}
		path_ANLZ[42] = 69;								//	
		path_ANLZ[43] = 88;								//	Replace *.DAT to *.EXT
		path_ANLZ[44] = 84;								//	for open EXT file with dynamic waveform	
		path_ANLZ[45] = 0;
			
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "\n\r");	
		UART_TX(&huart4, U_TX_DATA, 2, 5);		
		UART_TX(&huart4, path_ANLZ, 45, 15);	
		UART_TX(&huart4, U_TX_DATA, 2, 5);		
		#endif		
	
		res = f_open(&fileb, path_ANLZ, FA_READ);	
		if (res != FR_OK)
			{
			ERROR = 7;	//cannot open ANLZXXXX.EXT file
			}
		else
			{
			FILSIZE = f_size(&fileb);
			fsz = FILSIZE;
			if(fsz>WFD_SIZE)
				{
				fsz = WFD_SIZE;	
				}
			res = f_read(&fileb, WFORMDYNAMIC[dkB], fsz, &nbytesb);
			if(res != FR_OK)
				{
				ERROR = 8;	//ANLZXXXX.EXT file is damadge
				f_close(&fileb);					//Close file ANLZXXXX.EXT							
				}
			else
				{
				f_close(&fileb);					//Close file ANLZXXXX.EXT					
				fsz = WFORMDYNAMIC[dkB][8];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[dkB][9];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkB][10];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[dkB][11];	//file size		
				if(fsz != FILSIZE)
					{
					ERROR = 9;	//ANLZXXXX.EXT file is damadge
					}
				else		
					{	
					fsz = WFORMDYNAMIC[dkB][4];			
					fsz<<=8;	
					fsz+=WFORMDYNAMIC[dkB][5];	
					fsz<<=8;
					fsz+=WFORMDYNAMIC[dkB][6];	
					fsz<<=8;
					fsz+=WFORMDYNAMIC[dkB][7];	//Header size
					StPosHead = fsz;	
					if(WFORMDYNAMIC[dkB][StPosHead] != 80 | 
						 WFORMDYNAMIC[dkB][StPosHead+1] != 80 | 
					   WFORMDYNAMIC[dkB][StPosHead+2] != 84 | 
					   WFORMDYNAMIC[dkB][StPosHead+3] != 72)		//Check PPTH position in file
						{
						ERROR = 10;	//ANLZXXXX.EXT file is damadge				
						}
					else
						{		
						fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
						fsz<<=8;	
						fsz+=WFORMDYNAMIC[dkB][StPosHead+9];	
						fsz<<=8;
						fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
						fsz<<=8;
						fsz+=WFORMDYNAMIC[dkB][StPosHead+11];	//Tag size
						StPosHead += fsz;		
						if(WFORMDYNAMIC[dkB][StPosHead] != 80 | 
							 WFORMDYNAMIC[dkB][StPosHead+1] != 87 | 
						   WFORMDYNAMIC[dkB][StPosHead+2] != 86 | 
						   WFORMDYNAMIC[dkB][StPosHead+3] != 51)		//Check PWV3 position in file
							{
							return 11;	//ANLZXXXX.EXT file is damadge			
							}
						else
							{	
							fsz = WFORMDYNAMIC[dkB][StPosHead+4];			
							fsz<<=8;	
							fsz+=WFORMDYNAMIC[dkB][StPosHead+5];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[dkB][StPosHead+6];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[dkB][StPosHead+7];	//Header size
							uint32_t START_POS_WF = StPosHead + fsz; //Start position waveform	 //old SPP
									
							fsz = WFORMDYNAMIC[dkB][StPosHead+16];			
							fsz<<=8;	
							fsz+=WFORMDYNAMIC[dkB][StPosHead+17];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[dkB][StPosHead+18];	
							fsz<<=8;
							uint32_t WF_DATA_SIZE = fsz+WFORMDYNAMIC[dkB][StPosHead+19];		//waveform data size	//old fsz
							uint32_t j;							//Data shift	
								
							fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
							fsz<<=8;	
							fsz+=WFORMDYNAMIC[dkB][StPosHead+9];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[dkB][StPosHead+11];	//Tag size
							StPosHead += fsz;		
							if(WFORMDYNAMIC[dkB][StPosHead] != 80 | 
								 WFORMDYNAMIC[dkB][StPosHead+1] != 67 | 
								 WFORMDYNAMIC[dkB][StPosHead+2] != 79 | 
								 WFORMDYNAMIC[dkB][StPosHead+3] != 66)		//Check PCOB position in file
								{
								return 11;	//ANLZXXXX.EXT file is damadge			
								}
							else
								{
								fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
								fsz<<=8;	
								fsz+=WFORMDYNAMIC[dkB][StPosHead+9];	
								fsz<<=8;
								fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
								fsz<<=8;
								fsz+=WFORMDYNAMIC[dkB][StPosHead+11];	//Tag size
								StPosHead += fsz;		
								if(WFORMDYNAMIC[dkB][StPosHead] != 80 | 
								 WFORMDYNAMIC[dkB][StPosHead+1] != 67 | 
								 WFORMDYNAMIC[dkB][StPosHead+2] != 79 | 
								 WFORMDYNAMIC[dkB][StPosHead+3] != 66)		//Check PCO2 position in file
									{
									return 11;	//ANLZXXXX.EXT file is damadge			
									}	
								else
									{
									fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
									fsz<<=8;	
									fsz+=WFORMDYNAMIC[dkB][StPosHead+9];	
									fsz<<=8;
									fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
									fsz<<=8;
									fsz+=WFORMDYNAMIC[dkB][StPosHead+11];	//Tag size
									StPosHead += fsz;		
									if(WFORMDYNAMIC[dkB][StPosHead] != 80 | 
									 WFORMDYNAMIC[dkB][StPosHead+1] != 67 | 
									 WFORMDYNAMIC[dkB][StPosHead+2] != 79 | 
									 WFORMDYNAMIC[dkB][StPosHead+3] != 50)		//Check PCO2 position in file
										{
										return 11;	//ANLZXXXX.EXT file is damadge			
										}	
									else
										{
										#if defined(DEBUG_UART_EN)		
										sprintf((char*)U_TX_DATA, "Enter PCO2\n\r");	
										UART_TX(&huart4, U_TX_DATA, 12, 5);	
										#endif		
											
										if(WFORMDYNAMIC[dkB][StPosHead+15]==1)				//HOT CUE TYPE
											{
											number_of_hot_cue_points[dkB] = WFORMDYNAMIC[dkB][StPosHead+17];
											fsz = WFORMDYNAMIC[dkB][StPosHead+4];			
											fsz<<=8;	
											fsz+=WFORMDYNAMIC[dkB][StPosHead+5];	
											fsz<<=8;
											fsz+=WFORMDYNAMIC[dkB][StPosHead+6];	
											fsz<<=8;
											fsz+=WFORMDYNAMIC[dkB][StPosHead+7];	//header size
											StPosHead += fsz;	
											
											for(j=0;j<number_of_hot_cue_points[dkB];j++)			
												{	
												if(WFORMDYNAMIC[dkB][StPosHead] != 80 | 
													 WFORMDYNAMIC[dkB][StPosHead+1] != 67 | 
													 WFORMDYNAMIC[dkB][StPosHead+2] != 80 | 
													 WFORMDYNAMIC[dkB][StPosHead+3] != 50)		//Check PCP2 position in file
														{
														j = 100;	
														return 11;	//ANLZXXXX.EXT file is damadge			
														}
												else
													{
													#if defined(DEBUG_UART_EN)		
													sprintf((char*)U_TX_DATA, "Extract HOT CUE\n\r");	
													UART_TX(&huart4, U_TX_DATA, 17, 5);	
													#endif		
													if(WFORMDYNAMIC[dkB][StPosHead+15]>0)
														{
														HCUE_adr[dkB][0][WFORMDYNAMIC[dkB][StPosHead+15]-1] = WFORMDYNAMIC[dkB][StPosHead+20]; 		
														HCUE_adr[dkB][0][WFORMDYNAMIC[dkB][StPosHead+15]-1]<<=8;	
														HCUE_adr[dkB][0][WFORMDYNAMIC[dkB][StPosHead+15]-1]+=WFORMDYNAMIC[dkB][StPosHead+21];
														HCUE_adr[dkB][0][WFORMDYNAMIC[dkB][StPosHead+15]-1]<<=8;	
														HCUE_adr[dkB][0][WFORMDYNAMIC[dkB][StPosHead+15]-1]+=WFORMDYNAMIC[dkB][StPosHead+22];	
														HCUE_adr[dkB][0][WFORMDYNAMIC[dkB][StPosHead+15]-1]<<=8;	
														HCUE_adr[dkB][0][WFORMDYNAMIC[dkB][StPosHead+15]-1]+=WFORMDYNAMIC[dkB][StPosHead+23];
														if(WFORMDYNAMIC[dkB][StPosHead+16]==2)						//when hot cue type=loop
															{
															HCUE_adr[dkB][1][WFORMDYNAMIC[dkB][StPosHead+15]-1] = WFORMDYNAMIC[dkB][StPosHead+24]; 		
															HCUE_adr[dkB][1][WFORMDYNAMIC[dkB][StPosHead+15]-1]<<=8;	
															HCUE_adr[dkB][1][WFORMDYNAMIC[dkB][StPosHead+15]-1]+=WFORMDYNAMIC[dkB][StPosHead+25];
															HCUE_adr[dkB][1][WFORMDYNAMIC[dkB][StPosHead+15]-1]<<=8;	
															HCUE_adr[dkB][1][WFORMDYNAMIC[dkB][StPosHead+15]-1]+=WFORMDYNAMIC[dkB][StPosHead+26];	
															HCUE_adr[dkB][1][WFORMDYNAMIC[dkB][StPosHead+15]-1]<<=8;	
															HCUE_adr[dkB][1][WFORMDYNAMIC[dkB][StPosHead+15]-1]+=WFORMDYNAMIC[dkB][StPosHead+27];	
															}
														E=WFORMDYNAMIC[dkB][StPosHead+42];	
														E<<=8;
														E+=WFORMDYNAMIC[dkB][StPosHead+43];	//len_comment	
														HCUE32COLOR[dkB][WFORMDYNAMIC[dkB][StPosHead+15]-1] = WFORMDYNAMIC[dkB][StPosHead+45+E];																
														HCUE32COLOR[dkB][WFORMDYNAMIC[dkB][StPosHead+15]-1]<<=8;	
														HCUE32COLOR[dkB][WFORMDYNAMIC[dkB][StPosHead+15]-1]+= WFORMDYNAMIC[dkB][StPosHead+46+E];		
														HCUE32COLOR[dkB][WFORMDYNAMIC[dkB][StPosHead+15]-1]<<=8;		
														HCUE32COLOR[dkB][WFORMDYNAMIC[dkB][StPosHead+15]-1]+= WFORMDYNAMIC[dkB][StPosHead+47+E];	
														}														
													fsz = WFORMDYNAMIC[dkB][StPosHead+8];			
													fsz<<=8;	
													fsz+=WFORMDYNAMIC[dkB][StPosHead+9];	
													fsz<<=8;
													fsz+=WFORMDYNAMIC[dkB][StPosHead+10];	
													fsz<<=8;
													fsz+=WFORMDYNAMIC[dkB][StPosHead+11];	//len_entry
													StPosHead += fsz;		
													}													
												}												
											}
											
					///////add memory cues reading
											
									if(WF_DATA_SIZE>1)				//delete 2 end frames (exclude noise) 
											{
											WF_DATA_SIZE-=2;	
											}
										else
											{
											WF_DATA_SIZE = 0;	
											}									
										all_long[dkB] = WF_DATA_SIZE;
//										if(all_long[dkB]>57009)		//crop all long data for sldz
//											{
//											all_long[dkB] = 57009;	
//											}
											
										if(WF_DATA_SIZE>(WFD_SIZE-START_POS_WF))
											{
											WF_DATA_SIZE = WFD_SIZE-START_POS_WF;	
											}

//										if(WF_DATA_SIZE>57008)		//crop data for sldz		
//											{
//											WF_DATA_SIZE = 57008;	//max audio lenght 	
//											}								
											
										for(j=0;j<WF_DATA_SIZE;j++)			//Fill dynamic waveform
											{
											//WFORMDYNAMIC[dkB][j] = WFORMDYNAMIC[dkB][j+SPP];	
											//optimization with color coefficient:
											a = WFORMDYNAMIC[dkB][j+START_POS_WF]&0x1F;
											a*=153;
											a>>=8;											
											WFORMDYNAMIC[dkB][j] = WFORMDYNAMIC[dkB][j+START_POS_WF]&0xE0;
											WFORMDYNAMIC[dkB][j]|=a; 	
											}
											
//										for(j=0;j<(WF_DATA_SIZE>>1);j++)			//Fill dynamic waveform x2
//											{
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[dkB][wfmx2+(j/2)] = (WFORMDYNAMIC[dkB][j]&0xF0);	
//												}
//											else
//												{
//												WFORMDYNAMIC[dkB][wfmx2+(j/2)]+= WFORMDYNAMIC[dkB][j]>>4;	
//												}											
//											}	
//										for(j=0;j<(WF_DATA_SIZE>>2);j++)			//Fill dynamic waveform x4
//											{
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[dkB][wfmx4+(j/2)] = (WFORMDYNAMIC[dkB][2*j]&0xF0);
//												}
//											else
//												{
//												WFORMDYNAMIC[dkB][wfmx4+(j/2)]+= WFORMDYNAMIC[dkB][2*j]>>4;		
//												}											
//											}		
//										for(j=0;j<(WF_DATA_SIZE>>3);j++)			//Fill dynamic waveform x8
//											{
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[dkB][wfmx8+(j/2)] = (WFORMDYNAMIC[dkB][4*j]&0xF0);	
//												}
//											else
//												{
//												WFORMDYNAMIC[dkB][wfmx8+(j/2)]+= WFORMDYNAMIC[dkB][4*j]>>4;	
//												}											
//											}
//										for(j=0;j<(WF_DATA_SIZE>>4);j++)			//Fill dynamic waveform x16
//											{
//											wpos = 8;	
//											for(E=0;E<8;E++)			
//												{		
//												wpos+=WFORMDYNAMIC[dkB][8*j+E]&0x0F;
//												wpos+=WFORMDYNAMIC[dkB][8*j+E]>>4;	
//												}
//											wpos>>=4;	
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[dkB][wfmx16+(j/2)] = (wpos<<4);	
//												}
//											else
//												{
//												WFORMDYNAMIC[dkB][wfmx16+(j/2)]+= wpos&0x0F;	
//												}													
//											}		
//										for(j=0;j<(WF_DATA_SIZE>>5);j++)			//Fill dynamic waveform x32
//											{
//											wpos = 16;	
//											for(E=0;E<16;E++)			
//												{		
//												wpos+=WFORMDYNAMIC[dkB][16*j+E]&0x0F;
//												wpos+=WFORMDYNAMIC[dkB][16*j+E]>>4;	
//												}
//											wpos>>=5;	
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[dkB][wfmx32+(j/2)] = (wpos<<4);	
//												}
//											else
//												{
//												WFORMDYNAMIC[dkB][wfmx32+(j/2)]+= wpos&0x0F;	
//												}											
//											}			
											
										for(E=0;E<number_of_memory_cue_points[dkB];E++)											//Draw CUES on Display
											{
											if(MEMORY_adr[dkB][0][E] != 0xFFFF)
												{
												#if defined(DEBUG_UART_EN)		
												sprintf((char*)U_TX_DATA, "MEMORY in %06lu ms\n\r", MEMORY_adr[dkB][0][E]);											
												UART_TX(&huart4, U_TX_DATA, 21, 15);	
												#endif		
												mem_pos	= 609*MEMORY_adr[dkB][0][E];
												mem_pos/= (20*all_long[dkB]);
												DrawMemoryMarker(dkB, mem_pos, MEMORY_MARK, LCD_COLOR_RED);
												MEMORY_adr[dkB][0][E] = (MEMORY_adr[dkB][0][E]*3)/20;				//translate ms to 1/150s frames
												}	
											}	

										////////////////////////////////////sorting algoritm	
										uint8_t MINM, ii;
															
										for(E=0;E<number_of_memory_cue_points[dkB]-1;E++)	
											{
											MINM = E;
											for(ii=(E+1);ii<number_of_memory_cue_points[dkB];ii++)
												{
												if(MEMORY_adr[dkB][0][ii]<MEMORY_adr[dkB][0][MINM])
													{
													j = MEMORY_adr[dkB][0][MINM];
													MEMORY_adr[dkB][0][MINM] = MEMORY_adr[dkB][0][ii];
													MEMORY_adr[dkB][0][ii] = j;
													j = MEMORY_adr[dkB][1][MINM];
													MEMORY_adr[dkB][1][MINM] = MEMORY_adr[dkB][1][ii];
													MEMORY_adr[dkB][1][ii] = j;												
													j = MEMORY_type[dkB][MINM];
													MEMORY_type[dkB][MINM] = MEMORY_type[dkB][ii];
													MEMORY_type[dkB][ii] = j;	
													}
												}
											}

										uint8_t	r, g, b;	
										for(E=0;E<8;E++)											//Draw MEMORY on Display
											{
											if(HCUE_adr[dkB][0][E] != 0xFFFF)
												{		
												HCUEPCOLOR[dkB][E][0] = ((HCUE32COLOR[dkB][E]&0x00FFFFFF)>>16)/3;	
												HCUEPCOLOR[dkB][E][1] =	((HCUE32COLOR[dkB][E]&0x0000FFFF)>>8)/3;
												HCUEPCOLOR[dkB][E][2] = (HCUE32COLOR[dkB][E]&0x000000FF)/3;
												r = (HCUE32COLOR[dkB][E]&0x00FFFFFF)>>19;	
												g = (HCUE32COLOR[dkB][E]&0x0000FFFF)>>11;
												b = (HCUE32COLOR[dkB][E]&0x000000FF)>>3;	
												HCUE16COLOR[dkB][E] = 0x8000+(0x400*r)+(0x20*g)+b; 
												mem_pos	= 609*HCUE_adr[dkB][0][E];
												mem_pos/= (20*all_long[dkB]);
												DrawMemoryMarker(dkB, mem_pos, 2+E, HCUE16COLOR[dkB][E]);				//square
												#if defined(DEBUG_UART_EN)		
												sprintf((char*)U_TX_DATA, "H.CUE in %06lu ms  ", HCUE_adr[dkB][0][E]);											
												UART_TX(&huart4, U_TX_DATA, 20, 15);												
												#endif		
													
												HCUE_adr[dkB][0][E] = (HCUE_adr[dkB][0][E]*3)/20;				//translate ms to 1/150s frames										
												if(E==0)							//HCUE_A
													{
													if(HCUE_type[dkB][E]&0x1)
														{
														///Tbuffer[18] |= 0x1;	
														///Tbuffer[17] &= 0x7F;	
														}
													else
														{
														///Tbuffer[17] |= 0x80;
														///Tbuffer[18] &= 0x7E;	
														}
													}
												else if(E==1)					//HCUE_B
													{
													if(HCUE_type[dkB][E]&0x1)
														{
														///Tbuffer[18] |= 0x8;	
														///Tbuffer[18] &= 0x7B;	
														}
													else
														{	
														///Tbuffer[18] |= 0x4;
														///Tbuffer[18] &= 0x77;	
														}
													}	
												else if(E==2)					//HCUE_C
													{
													if(HCUE_type[dkB][E]&0x1)
														{
														///Tbuffer[18] |= 0x40;		
														///Tbuffer[18] &= 0x5F;	
														}
													else
														{	
														///Tbuffer[18] |= 0x20;
														///Tbuffer[18] &= 0x3F;	
														}
													}	
												}
											else				//turn off leds
												{
												HCUEPCOLOR[dkB][E][0] = HCUEdisableCOLOR[0];	
												HCUEPCOLOR[dkB][E][1] =	HCUEdisableCOLOR[1];
												HCUEPCOLOR[dkB][E][2] = HCUEdisableCOLOR[2];
												}									
											}
										}									
									}									
								}
							}
						}
					}			
				}	
			}

		res = f_open(&fileb, path_AUDIOTRACK, FA_READ);				//Open audio file
		if (res != FR_OK)
			{
			return 13;	//cannot open AUDIOTRACK
			}
		#if defined(DEBUG_UART_EN)	
		sprintf((char*)U_TX_DATA, "\n\r");	
		UART_TX(&huart4, U_TX_DATA, 2, 5);				
		UART_TX(&huart4, path_AUDIOTRACK, 150, 45);	
		UART_TX(&huart4, U_TX_DATA, 2, 5);	
		#endif		
		res = f_read(&fileb, PCM[dkB], 512, &nbytesb);
		if(res != FR_OK)
			{
			ERROR = 14;	//cannot read AUDIOTRACK
 			}
		#if defined(DEBUG_UART_EN)	
		sprintf((char*)U_TX_DATA, "\n\r");	
		UART_TX(&huart4, U_TX_DATA, 2, 5);				
		UART_TX(&huart4, (uint8_t *)PCM, 50, 45);	
		UART_TX(&huart4, U_TX_DATA, 2, 5);	
		#endif		
			
		if(PCM[dkB][0][5][0] != 1 || PCM[dkB][0][5][1] != 2 || PCM[dkB][0][6][0] != 44100 || PCM[dkB][0][8][1] != 16)	//Check audio format
			{
			ERROR = 15;	//unsupported audio format	
			}
		f_lseek(&fileb, 44);
		if(ERROR==0)
			{
			playlist[TRACK_NUMBER-1][54] |= 0x01;				//write history mem
			}
		curr_trck_color[dkB] = (rating[TRACK_NUMBER-1]&0x0F);	
		return ERROR;	
		};	
		
			
///////////////////////////////////////////		
//preparing for loading next track
//
//
void PREPARE_LOAD_TRACK(uint8_t dk, uint16_t TRACK_NUMBER, uint16_t TRACK_IN_PLAYLIST)
	{	
	lock_control[dk] = 1;	
	uint16_t ERR = 0;
	uint16_t i;	
	if(dk==dkA)
		{
		f_close(&file);		
		for(i=0;i<8;i++)
			{
			deckTbuf[i][1] = HCUEdisableCOLOR[0];
			deckTbuf[i][2] = HCUEdisableCOLOR[1];
			deckTbuf[i][3] = HCUEdisableCOLOR[2];
			}		
		}
	else
		{
		f_close(&fileb);	
		for(i=0;i<8;i++)
			{
			deckTbuf[i][9] = HCUEdisableCOLOR[0];
			deckTbuf[i][10] = HCUEdisableCOLOR[1];
			deckTbuf[i][11] = HCUEdisableCOLOR[2];
			}		
		}
	pitch[dk] = 0;	
	play_enable[dk] = 0;
	if(SLIPEN[dk])
		{
		SLIPEN[dk] = 0;
		if(dk==dkA)
			{
			deckTbuf[1][0]|=0x10;		//led on	
			}
		else
			{
			deckTbuf[1][8]|=0x10;		//led on		
			}
		}		
	for(i=0;i<2048;i++)
		{
		BEATGRID[dk][i] = 0xFFFFFFFF;
		BPMGRID[dk][i] = 0xFFFF;	
		}		
	play_adr[dk] = 0;	
	all_long[dk] = 0;		
	end_adata[dk] = 0xFFFF;
	start_adata[dk] = 0;	
	slip_pl_adr[dk] = 0;
	loop_active[dk] = 0;
	loop_act_gui[dk] = 0;			
	loop_lenght[dk] = 0xFF;
	loopmode_need_update[dk] = 1;	
	LOOP_OUT[dk] = 0;
	curr_trck_color[dk] = 0;		
	if(dk==dkA)
		{
		if(play_enable[dkB] && masterdeck!=dkB)
			{
			masterdeck = dkB;
			tempo_need_update[dkA] = 2;
			tempo_need_update[dkB] = 2;	
			}		
		ERR = LOAD_TRACKA(TRACK_NUMBER);
		}
	else
		{
		if(play_enable[dkA] && masterdeck!=dkA)
			{
			masterdeck = dkA;
			tempo_need_update[dkA] = 2;
			tempo_need_update[dkB] = 2;	
			}		
		ERR = LOAD_TRACKB(TRACK_NUMBER);
		}	
	if(ERR==0)								//the sequence of functions in this place is very important!
		{				
		track_play_now[dk] = TRACK_NUMBER;				
		tempo_need_update[dk] = 1;	
		mixerBPM = (originalBPM[dk]*potenciometr_tempo[dk])/10000;			//send bpm to mixer assy
		U_TX_DATA[0] = 0x96;																		//
		U_TX_DATA[1] = mixerBPM>>8;															//
		U_TX_DATA[2] = mixerBPM%256;														//
		UART_TX(&huart1, U_TX_DATA, 3, 5);											//
		end_adata[dk] = 0;
		//TIM3->CCR1 = ROUND_PWM_VALUE;					//round PWM	
		//REDRAW_TIMESCROLL(0);
		//DrawNewEmpty_TIMESCROLL();
		TRACK_n_PLAY[dk] = TRACK_IN_PLAYLIST;	
		trackname_parser(dk, track_play_now[dk]-1);	
			
		if(UT_SET[ACUE]==9)					//first beat
			{
			CUE_ADR[dk] = (BEATGRID[dk][0]*3)/20;		
			play_adr[dk] = CUE_ADR[dk]*294;	
			slip_pl_adr[dk] = play_adr[dk];
			}
		else if(UT_SET[ACUE]==8 && number_of_memory_cue_points[dk]>0)					//memory
			{	
			SET_MEMORY_CUE_1(dk, MEMORY_adr[dk][0][0]);
			CUE_OPERATION[dk] = MEMORY_NEED_SET_PART2;
			CUE_ADR[dk] =	MEMORY_adr[dk][0][0];
			}	
		else
			{
			uint8_t have_a_cue = 0;
			uint16_t c = 0;
			uint16_t M = 0;	
			while(have_a_cue==0 && all_long[dk]>28*c)
				{
				if(dk==dkA)
					{
					f_read(&file, PCM[dk][0][0], 32768, &nbytes);
					}
				else
					{
					f_read(&fileb, PCM[dk][0][0], 32768, &nbytes);	
					}						
				for(M=0;M<8192;M++)
					{
					if(PCM[dk][0][M][0]&0x8000)		//negative 65535...32768
						{
						PCM[dk][0][M][0] = 0xFFFF - PCM[dk][0][M][0];	
						}
					if(((PCM[dk][0][M][0]>>(9-UT_SET[ACUE]))>0) && have_a_cue==0)
						{
						have_a_cue = 1;
						CUE_ADR[dk] = (c*8192+M)/294;
						play_adr[dk] = CUE_ADR[dk]*294;
						slip_pl_adr[dk] = play_adr[dk];
						M = 0xFFFF;
						}
					if(PCM[dk][0][M][1]&0x8000)		//negative 65535...32768
						{
						PCM[dk][0][M][1] = 0xFFFF - PCM[dk][0][M][1];	
						}
					if(((PCM[dk][0][M][1]>>(9-UT_SET[ACUE]))>0) && have_a_cue==0)
						{
						have_a_cue = 1;
						CUE_ADR[dk] = (c*8192+M)/294;
						play_adr[dk] = CUE_ADR[dk]*294;
						slip_pl_adr[dk] = play_adr[dk];
						M = 0xFFFF;	
						}
					}	
				}
			if(dk==dkA)
				{
				f_lseek(&file, 44);						//return to start track	
				}
			else
				{
				f_lseek(&fileb, 44);						//return to start track		
				}			
			if(have_a_cue==0)
				{
				play_adr[dk] = 0;	
				slip_pl_adr[dk] = 0;	
				CUE_ADR[dk] = 0;		
				}		
			}			
		DrawStaticWFM(dk, DRAW_NEW_STATIC_WAVEFORM);				//Draw New waveform		
			
		if(UT_SET[ACUE]==9)					//first beat
			{
			RedrawWaveforms(dk, (BEATGRID[dk][0]*3)/20);
			}	
		else if(UT_SET[ACUE]==8 && number_of_memory_cue_points[dk]>0)					//memory
			{	
			RedrawWaveforms(dk, MEMORY_adr[dk][0][0]);	
			}			
		else		
			{
			RedrawWaveforms(dk, CUE_ADR[dk]);	
			}
			
		SwitchInformationLayer(WAVEFORM);	
		if(UT_SET[ACUE]==9)					//first beat
			{	
			SET_CUE(dk, (BEATGRID[dk][0]*3)/20);						//AUTO CUE set
			}
		else if(UT_SET[ACUE]==8 && number_of_memory_cue_points[dk]>0)					//memory
			{	
			SET_CUE(dk, MEMORY_adr[dk][0][0]);
			}			
		else
			{
			SET_CUE(dk, CUE_ADR[dk]);						//AUTO CUE set	
			}	
		DrawMinuteMarkers(dk);
		DrawKey(dk, key_id[track_play_now[dk]-1], dk);	
			
		for(i=0;i<8;i++)
			{	
			if(dk==dkA)
				{
				deckTbuf[i][1] = HCUEPCOLOR[dk][i][0];
				deckTbuf[i][2] = HCUEPCOLOR[dk][i][1];
				deckTbuf[i][3] = HCUEPCOLOR[dk][i][2];	
				}
			else
				{
				deckTbuf[i][9] = HCUEPCOLOR[dk][i][0];
				deckTbuf[i][10] = HCUEPCOLOR[dk][i][1];
				deckTbuf[i][11] = HCUEPCOLOR[dk][i][2];	
				}		
			}		
		}
	else
		{
		track_play_now[dk] = 0;
		if(dk==dkA)
			{
			f_close(&file);
			}
		else
			{
			f_close(&fileb);	
			}		
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Track not load. Error %03lu\n\r", ERR);	
		UART_TX(&huart4, U_TX_DATA, 27, 5);	
		#endif	
		all_long[dk] = 0;		
		originalBPM[dk] = 0xFFFF;
		SwitchInformationLayer(WAVEFORM);	
		ShowBPM(dk, originalBPM[dk]);
		TRACK_n_PLAY[dk] = 0;
		DrawStaticWFM(dk, CLEAR_WAVEFORM_ARRAY);	
		DrawStaticWFM(dk, MS_ERROR+ERR);		
		RED_CRCL_CUE_ADR[dk] = 85;
		}
	if(ERR==0)								//the sequence of functions in this place is very important!
		{		
		lock_control[dk] = 0;
		}
	return;	
	};		
	
		
///////////////////////////////////////////		
//
//	search current bar number	
//	
uint16_t FindCurrBar(uint8_t dk, uint32_t pos)
	{
	uint16_t u, mn;	
	uint8_t q;	
	u = 1024;
	mn = 512;
	while(mn>2)
		{	
		if(BEATGRID[dk][u]<pos)
			{
			if(mn>4)
				{					
				u+=mn;
				}
			else
				{					
				q = 1;
				}						
			}
		else
			{
			if(mn>4)
				{			
				u-=mn;
				}
			else
				{					
				q = 0;
				}						
			}
		mn/=2;	
		}
	if(q==1)
		{
		while(BEATGRID[dk][u]<pos)
			{
			u++;	
			}		
		if(u>0)
			{
			u--;	
			}
		}			
	else
		{
		while(BEATGRID[dk][u]>pos)
			{
			if(u==0)
				{
				break;	
				}				
			u--;	
			}
		}
	return u;	
	};
	
															
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
