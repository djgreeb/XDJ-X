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
uint16_t LOAD_TRACK(uint8_t dk, uint16_t TRACK_NUMBER);
void PREPARE_LOAD_TRACK(uint8_t dk, uint16_t TRACK_NUMBER, uint16_t TRACK_IN_PLAYLIST);
uint16_t FindCurrBar(uint8_t dk, uint32_t pos);		//search current bar number


////////////////////////////////////////////////////////////////////////
//
//
//Rekordbox database parser ver. 0.55
//
//
//Functions:
//Open and read file export.pdb
//find all tracks
//write trackname in massive
//write adress position datatrack in file export.pdb
//return number of tracks or 0 if error enable 
//extended to 8 hot cues reading from *.EXT files
//bug fixed: path_ANLZ expanded to 46 bytes, for final 0 
//BEATGRID and BPMGRID changed 4096->2048
//Rekordbox database parser ver. 0.44
//Cyrillic symbols added
//Rekordbox database parser ver. 0.45
//KEYSmatchtable bug fixed
//Rekordbox database parser ver. 0.47
//Expanded to 1024 tracks, 2048 mentions and 40 playlists
//Rekordbox database parser ver. 0.51
//Playlists>20 bug fixed
//Rekordbox database parser ver. 0.53
//Added support for reading large audio files when the *.EXT file exceeded the WFORMDYNAMIC buffer size
//Rekordbox database parser ver. 0.55
//2-layers static waveform added
//adaptive code from Pioneer waveform changer utility	added
//
//
////////////////////////////////////////////////////////////////////////
uint16_t DATABASE_PARSER(void)
	{	
	#if defined(DEBUG_UART_EN)		
	sprintf((char*)U_TX_DATA, "Start Rekordbox parser ver. 0.55\n\r");	
	UART_TX(&huart4, U_TX_DATA, 34, 55);	
	#endif		
	res = f_open(&file[0], path_export, FA_READ);
	if (res != FR_OK)
		{
		return 0xFFFE;	
		}		
	uint32_t FILSIZE = f_size(&file[0]);
	#if defined(DEBUG_UART_EN)		
	sprintf((char*)U_TX_DATA, "FILSIZE: %08lu\n\r", FILSIZE);	
	UART_TX(&huart4, U_TX_DATA, 19, 55);	
	#endif
	char str[4] = ".DAT";
	uint32_t crsr = 0;
	uint32_t BPM_CNT = 0;
	uint8_t	cycle_en;
	uint16_t all_trks = 0;			//dbmax max
	uint16_t T_ID = 0;
	uint16_t i  = 0;	
	uint16_t wi;	
	uint32_t NEXT_PAGE_0, LAST_PAGE_0, P_PAGE_0;	
	uint32_t NEXT_PAGE_5, LAST_PAGE_5; //page for KEY
	uint32_t NEXT_PAGE_7, LAST_PAGE_7; //page for PLAYLIST tree
	uint32_t NEXT_PAGE_8, LAST_PAGE_8; //page for PLAYLIST entries		
	uint32_t NEXT_PAGE_19, LAST_PAGE_19; //page for history, flash name, date
	for(T_ID=0;T_ID<(55*dbmax);T_ID++)					//clear arrays
		{	
		playlist[T_ID/55][T_ID%55] = 0;
		}		
	for(T_ID=0;T_ID<dbmax;T_ID++)					//clear arrays
		{	
		parcser_adress[T_ID] = 0;
		original_tempo[T_ID] = 0;	
		rating[T_ID] = 0;	
		duration[T_ID] = 0;	
		key_id[T_ID] = 0;		
		}		
	for(T_ID=0;T_ID<(2*dbmax);T_ID++)
		{
		WFORMDYNAMIC[dkA][4096+T_ID] = 0xFF;				//give out buffer for track ID in WFORMDYNAMIC[dkA]	offset 8192
		}
	T_ID = 0;	
		
	res = f_read(&file[0], WFORMDYNAMIC[dkA], 4096, &nbytes[0]);
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
		
	f_lseek(&file[0], 4096*NEXT_PAGE_0); 				//first page with track metadata: title, artist, genre, artwork ID, playing time, etc
	
	while(NEXT_PAGE_0<(FILSIZE>>12) && LAST_PAGE_0>=NEXT_PAGE_0)	
		{
		res = f_read(&file[0], WFORMDYNAMIC[dkA], 4096, &nbytes[0]);
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
					if(T_ID>dbmax)
						{	
						f_close(&file[0]);	
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
					crsr+=34;			//offset 14 File path of track analysis
					parcser_adress[T_ID-1] = (4096*P_PAGE_0)+(((crsr + (WFORMDYNAMIC[dkA][crsr] + 256*WFORMDYNAMIC[dkA][crsr+1])) - 122) + 41);	//save position ".[D]AT"
					crsr+=6;	//offset 17 Track title
					crsr+=WFORMDYNAMIC[dkA][crsr] + 256*WFORMDYNAMIC[dkA][crsr+1];	
					crsr-=128;	

					if(WFORMDYNAMIC[dkA][crsr]==0x90)		//Unicode	
						{
						wi = ((WFORMDYNAMIC[dkA][crsr+1] + 256*WFORMDYNAMIC[dkA][crsr+2])/2)-2;	
						for(i=0;i<wi;i++)
							{
							if(WFORMDYNAMIC[dkA][crsr+5+i*2]==0)
								{
								playlist[T_ID-1][i] =	WFORMDYNAMIC[dkA][crsr+4+i*2];	
								}								
							else
								{
								playlist[T_ID-1][i] =	CYRtoCP866[WFORMDYNAMIC[dkA][crsr+4+i*2]];	
								}								
							}	
						}
					else			//ASCII	
						{
						wi = ((WFORMDYNAMIC[dkA][crsr]-1)/2)-1;
						for(i=0;i<wi;i++)
							{
							playlist[T_ID-1][i] =	WFORMDYNAMIC[dkA][crsr+1+i];		
							}
						}
					while(i<54)																				//Fill zeros
						{
						playlist[T_ID-1][i] = 0;
						i++;		
						}									
					playlist[T_ID-1][53] = 0;
					playlist[T_ID-1][54] = 0;								
					}
				crsr++;	
				}				
			}
		f_lseek(&file[0], 4096*NEXT_PAGE_0);
		}	
	f_lseek(&file[0], 4096*NEXT_PAGE_5); 				//first page with musical keys, for reference by tracks, searching, and key matching
	cycle_en = 1;		
	while(NEXT_PAGE_5<(FILSIZE>>12) && cycle_en)	
		{
		if(LAST_PAGE_5==NEXT_PAGE_5)			//scan last page
			{
			cycle_en = 0;	
			}
		res = f_read(&file[0], WFORMDYNAMIC[dkA], 4096, &nbytes[0]);
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
						
				if(KEYS[WFORMDYNAMIC[dkA][crsr]-1][3]<33 || KEYS[WFORMDYNAMIC[dkA][crsr]-1][3]>125)				//cut string
					{
					KEYS[WFORMDYNAMIC[dkA][crsr]-1][3] = 0;	
					}
				for(i=0;i<24;i++)
					{
					if((KEYS[WFORMDYNAMIC[dkA][crsr]-1][0]==KEY_MATCH[i][0]) && 
						 (KEYS[WFORMDYNAMIC[dkA][crsr]-1][1]==KEY_MATCH[i][1]) && 
						 (KEYS[WFORMDYNAMIC[dkA][crsr]-1][2]==KEY_MATCH[i][2]))	
						{
						KEYSmatchtable[WFORMDYNAMIC[dkA][crsr]-1] = i;
						i = 96;	
						}	
					}	
				if(i<90)
					{
					KEYSmatchtable[WFORMDYNAMIC[dkA][crsr]-1] = 25;		//this string key not found in table	
					}					
				i = 0;		
					
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
		f_lseek(&file[0], 4096*NEXT_PAGE_5);	
		}	
	f_lseek(&file[0], 4096*NEXT_PAGE_7); 				//first page for PLAYLIST tree
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
		res = f_read(&file[0], WFORMDYNAMIC[dkA], 4096, &nbytes[0]);
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
			P_PAGE_0+=1;		
			while(P_PAGE_0>0)	
				{	
				BPM_CNT = (((WFORMDYNAMIC[dkA][crsr+20]-1)/2)-1);		//playlist lenght name	
				if(WFORMDYNAMIC[dkA][crsr+16]==0 && WFORMDYNAMIC[dkA][crsr+17]==0 && WFORMDYNAMIC[dkA][crsr+18]==0 && WFORMDYNAMIC[dkA][crsr+19]==0)			//row is playlist
					{						
					for(i=0;((i<BPM_CNT) && (i<20)); i++)
						{
						WFORMDYNAMIC[dkA][8192+21*(WFORMDYNAMIC[dkA][crsr+12]-1)+i] = WFORMDYNAMIC[dkA][crsr+21+i];								
						}
					WFORMDYNAMIC[dkA][8192+21*(WFORMDYNAMIC[dkA][crsr+12]-1)+20] = WFORMDYNAMIC[dkA][crsr+12];			//playlist ID	 	
					}					
				crsr+= 24 + 4*(BPM_CNT/4);
				P_PAGE_0--;	
				}
			}			
		f_lseek(&file[0], 4096*NEXT_PAGE_7);	
		}
	TOTAL_TRACKLISTS = 0;
	uint8_t inp = 0;
	uint8_t cnt;	
	for(BPM_CNT=1;BPM_CNT<255;BPM_CNT++)	//force playlists ID
		{
		for(i=0;i<200;i++)
			{
			if(WFORMDYNAMIC[dkA][8192+20+21*i]==BPM_CNT)				//have a playlist
				{					
				for(cnt=0;cnt<21;cnt++)			//copy playlist name
					{
					TRACKLIST_NAME[TOTAL_TRACKLISTS][cnt] = WFORMDYNAMIC[dkA][8192+cnt+21*i];				
					}
				inp = 1;	
				}
			}
		if(inp)	
			{
			if(TOTAL_TRACKLISTS<plmax)
				{
				TOTAL_TRACKLISTS++;
				}
			inp = 0;	
			}	
		}	
					
	f_lseek(&file[0], 4096*NEXT_PAGE_8); 				//first page for PLAYLIST tree
	
	cycle_en = 1;	
	for(i=0;i<(1024*plmax);i++)					//prepare 0 buffer
		{
		WFORMDYNAMIC[dkA][8192+i] = 0xFF;	
		}
	while(LAST_PAGE_8<(FILSIZE>>12) && cycle_en)	
		{
		if(LAST_PAGE_8==NEXT_PAGE_8)			//scan last page
			{
			cycle_en = 0;	
			}
		res = f_read(&file[0], WFORMDYNAMIC[dkA], 4096, &nbytes[0]);
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
			P_PAGE_0+=1;	
			while(P_PAGE_0>0)	
				{
				i = PlaylistID_to_Pos(WFORMDYNAMIC[dkA][crsr+8]);
				if(i==plmax)
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
		f_lseek(&file[0], 4096*NEXT_PAGE_8);	
		}
				
	TRACKLIST_OFFSET[0] = 0;
	crsr = 0;	
	for(i=0;i<TOTAL_TRACKLISTS;i++)
		{
		TRACKLIST_NAME[i][20] = 0;
		cycle_en = 1;	
			
		for(BPM_CNT=0;BPM_CNT<512;BPM_CNT++)
			{
			T_ID = (i*1024)+(2*BPM_CNT);
			if(WFORMDYNAMIC[dkA][8192+T_ID]==0xFF && WFORMDYNAMIC[dkA][8193+T_ID]==0xFF)
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
		
	f_lseek(&file[0], 4096*NEXT_PAGE_19); 				//first page with history, flash name, date
	cycle_en = 1;		
	while(NEXT_PAGE_19<(FILSIZE>>12) && cycle_en)	
		{
		if(LAST_PAGE_19==NEXT_PAGE_19)			//scan last page
			{
			cycle_en = 0;	
			}
			
		res = f_read(&file[0], WFORMDYNAMIC[dkA], 4096, &nbytes[0]);
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
					crsr+=T_ID + 3;	
						
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
					crsr+=T_ID+6;	
					BPM_CNT = 4-((crsr - BPM_CNT)%4);
					crsr+=BPM_CNT;
					P_PAGE_0--;			
					}
				else
					{
					P_PAGE_0 = 0;	
					}
				}
			}
		f_lseek(&file[0], 4096*NEXT_PAGE_19);	
		}
	f_close(&file[0]);					//Close file Export.pdb
	return all_trks;
	};



////////////////////////////////////////////////////////////////////////
//convert playlist ID to position in Tracklist name
//
//
///////////////////////////////////////////////////////////////////////	
uint8_t PlaylistID_to_Pos(uint8_t ID)
	{
	uint8_t K;
	for(K=0;K<plmax;K++)
		{
		if(ID==TRACKLIST_NAME[K][20])
			{
			return K;	
			}
		}
	return plmax;
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
	uint16_t LOAD_TRACK(uint8_t dk, uint16_t TRACK_NUMBER)
		{
		if(TRACK_NUMBER==0)
			{
			return 1;				//invalid track number
			}	
		uint16_t ERROR = 0;
		uint16_t E=0;
		uint32_t mem_pos;	
		res = f_open(&file[dk], path_export, FA_READ);
		if (res != FR_OK)
			{
			return 2;	//cannot open database
			}
		f_lseek(&file[dk], (parcser_adress[TRACK_NUMBER-1]-42));			//find 0.DAT in file		
		char path_ANLZ[46];
		res = f_read(&file[dk], path_ANLZ, sizeof(path_ANLZ), &nbytes[dk]);
		if (res != FR_OK)
			{
			return 3;		//data not read
			}					
		f_close(&file[dk]);					//Close file Export.pdb
		path_ANLZ[0] = 48;
		path_ANLZ[1] = 58;
		path_ANLZ[45] = 0;
			
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "\n\r");	
		UART_TX(&huart4, U_TX_DATA, 2, 5);		
		UART_TX(&huart4, path_ANLZ, 45, 15);	
		UART_TX(&huart4, U_TX_DATA, 2, 5);		
		#endif		
			
		res = f_open(&file[dk], path_ANLZ, FA_READ);
		if (res != FR_OK)
			{	
			return 4;	//cannot open ANLZXXXX.DAT file
			}
		uint32_t FILSIZE = f_size(&file[dk]);
		if(FILSIZE>8912896)
			{
			return 26;	
			}
		res = f_read(&file[dk], sdram.bf[dk], FILSIZE, &nbytes[dk]);
		if (res != FR_OK)
			{
			return 5;	//cannot read ANLZXXXX.DAT file		
			}
		f_close(&file[dk]);					//Close file ANLZXXXX.DAT		
		uint32_t fsz;	
		fsz = sdram.bf[dk][8];			
		fsz<<=8;	
		fsz+=sdram.bf[dk][9];	
		fsz<<=8;
		fsz+=sdram.bf[dk][10];	
		fsz<<=8;
		fsz+=sdram.bf[dk][11];
		if(fsz != FILSIZE)
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = sdram.bf[dk][4];			
		fsz<<=8;
		fsz+=sdram.bf[dk][5];	
		fsz<<=8;
		fsz+=sdram.bf[dk][6];	
		fsz<<=8;
		fsz+=sdram.bf[dk][7];
		uint32_t StPosHead = fsz;		
		if(sdram.bf[dk][StPosHead] != 80 ||
			 sdram.bf[dk][StPosHead+1] != 80 ||
			 sdram.bf[dk][StPosHead+2] != 84 || 
			 sdram.bf[dk][StPosHead+3] != 72)		//Check PPHT position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = sdram.bf[dk][StPosHead+4];			
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+5];	
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+6];	
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+7];								//head size
		uint32_t SPP = fsz+StPosHead+1;	
		fsz = sdram.bf[dk][StPosHead+12];			
		fsz<<=8;	
		fsz+=sdram.bf[dk][StPosHead+13];	
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+14];	
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+15];							//Path Size
		char path_AUDIOTRACK[(fsz/2)+2];			//Create a Path for audiotrack
		E = 0;	
		while(E<fsz)
			{	
			if(sdram.bf[dk][SPP+E-1]==0)
				{
				path_AUDIOTRACK[(E/2)+2] = sdram.bf[dk][SPP+E];			//Fill path	
				}				
			else if(sdram.bf[dk][SPP+E-1]==0x04)				//Convert Unicode to CP866
				{	
				if(sdram.bf[dk][SPP+E]<96)
					{
					path_AUDIOTRACK[(E/2)+2] = CYRtoCP866[sdram.bf[dk][SPP+E]];		
					}									
				}
			E+=2;					
			}
		path_AUDIOTRACK[0] = 48;
		path_AUDIOTRACK[1] = 58;	
		fsz = sdram.bf[dk][StPosHead+8];			
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+9];	
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+10];
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+11];	
		StPosHead = StPosHead+fsz;	//PVBR position
		if(sdram.bf[dk][StPosHead] != 80 ||
			 sdram.bf[dk][StPosHead+1] != 86 ||
			 sdram.bf[dk][StPosHead+2] != 66 || 
			 sdram.bf[dk][StPosHead+3] != 82)	//Check PVBR position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = sdram.bf[dk][StPosHead+8];			
		fsz<<=8;	
		fsz+=sdram.bf[dk][StPosHead+9];	
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+10];	
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+11];		
		StPosHead = StPosHead+fsz;	//PQTZ position
		if(sdram.bf[dk][StPosHead] != 80 ||
			 sdram.bf[dk][StPosHead+1] != 81 ||
			 sdram.bf[dk][StPosHead+2] != 84 || 
			 sdram.bf[dk][StPosHead+3] != 90)	//Check PQTZ position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = sdram.bf[dk][StPosHead+4];				
		fsz<<=8;	
		fsz+=sdram.bf[dk][StPosHead+5];		
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+6];		
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+7];				//Head size
		uint32_t number_of_entries;
		number_of_entries = sdram.bf[dk][StPosHead+20];				
		number_of_entries<<=8;	
		number_of_entries+=sdram.bf[dk][StPosHead+21];		
		number_of_entries<<=8;
		number_of_entries+=sdram.bf[dk][StPosHead+22];		
		number_of_entries<<=8;
		number_of_entries+=sdram.bf[dk][StPosHead+23];				//calculate number_of_entries
		if(number_of_entries>2048)
			{
			number_of_entries = 2048;	
			}
		SPP = StPosHead + fsz + 2;						//start first BPM data.
		E = 0;
		GRID_OFFSET[dk] = sdram.bf[dk][SPP-1];							//find first beat 1...4
		while(E<number_of_entries)
			{
			BPMGRID[dk][E] = sdram.bf[dk][SPP+(E*8)];	
			BPMGRID[dk][E]<<=8;
			BPMGRID[dk][E]+= sdram.bf[dk][SPP+1+(E*8)];		
			BEATGRID[dk][E] = sdram.bf[dk][SPP+2+(E*8)];	
			BEATGRID[dk][E]<<=8;
			BEATGRID[dk][E]+= sdram.bf[dk][SPP+3+(E*8)];	
			BEATGRID[dk][E]<<=8;
			BEATGRID[dk][E]+= sdram.bf[dk][SPP+4+(E*8)];	
			BEATGRID[dk][E]<<=8;
			BEATGRID[dk][E]+= sdram.bf[dk][SPP+5+(E*8)];	
			E++;	
			}
		if(E==2048)
			{
			BEATGRID[dk][2047] = 0xFFFFFFFF;	
			}
		else
			{
			BEATGRID[dk][E] = 0xFFFFFFFF;	
			BPMGRID[dk][E] = BPMGRID[dk][E-1];	
			}
		originalBPM[dk] = BPMGRID[dk][0];			//SEND ORIGINAL BPM		
		fsz = sdram.bf[dk][StPosHead+8];			
		fsz<<=8;	
		fsz+=sdram.bf[dk][StPosHead+9];
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+10];	
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+11];	
		StPosHead = StPosHead+fsz;	//PWAV position	
		if(sdram.bf[dk][StPosHead] != 80   ||
			 sdram.bf[dk][StPosHead+1] != 87 ||
			 sdram.bf[dk][StPosHead+2] != 65 || 
			 sdram.bf[dk][StPosHead+3] != 86)		//Check PWAV position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = sdram.bf[dk][StPosHead+4];			
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+5];
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+6];	
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+7];				//PWAV Header size
		fsz+=StPosHead;			
		uint16_t ampl;
		uint32_t y;	

		for(E=0;E<202;E++)					//Fill Static Waveform 400->202
			{
			y = 509*E;
			y>>=8;	
			ampl = (sdram.bf[dk][fsz+y]&0x1F)*192;	 //convert amplitude 24->18
			ampl>>=8;
			if(ampl>18)
				{
				ampl = 18;	
				}
			WFORMSTATIC[dk][E] = sdram.bf[dk][fsz+y]&0x80;	//color	
			WFORMSTATIC[dk][E]|= ampl;	
			}

		fsz = sdram.bf[dk][StPosHead+8];			
		fsz<<=8;	
		fsz+=sdram.bf[dk][StPosHead+9];
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+10];	
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+11];				//PWAV TAG size
		StPosHead+=fsz;													//PWV2 start adress
		if(sdram.bf[dk][StPosHead] != 80   ||
			 sdram.bf[dk][StPosHead+1] != 87 ||
			 sdram.bf[dk][StPosHead+2] != 86 || 
			 sdram.bf[dk][StPosHead+3] != 50)		//Check PWV2 position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = sdram.bf[dk][StPosHead+8];			
		fsz<<=8;	
		fsz+=sdram.bf[dk][StPosHead+9];
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+10];	
		fsz<<=8;
		fsz+=sdram.bf[dk][StPosHead+11];				//PWV2 Tag size
		StPosHead = StPosHead+fsz;	//PCOB position				
		if(sdram.bf[dk][StPosHead] != 80 	 ||
			 sdram.bf[dk][StPosHead+1] != 67 ||
			 sdram.bf[dk][StPosHead+2] != 79 || 
			 sdram.bf[dk][StPosHead+3] != 66)		//Check PCOB position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}		
		DrawMemoryMarker(dk, 0xFF, NONE_MARK, 0x8000);
		uint32_t PCOB2_adr;
			
		//////////////////prepare HOT CUES	
		for(E=0;E<8;E++)									//Clear ícue massive
			{
			HCUE_type[dk][E] = 0xFF;	 							//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 		
			HCUE_adr[dk][0][E] = 0xFFFF;		
			HCUE_adr[dk][1][E] = 0xFFFF;	
			HCUE32COLOR[dk][E] = 0xFFFF;	
			}
		
		if(sdram.bf[dk][StPosHead+15]==1)    //check type PCOB - for HOT CUE points
			{
			number_of_hot_cue_points[dk] = sdram.bf[dk][StPosHead+19]&0xF;				//number of hotcue points		
//			if(number_of_hot_cue_points[dk]>0)
//				{
//				MemoryCuePyramid_ENABLE = 3;	
//				}
			fsz = sdram.bf[dk][StPosHead+8];			
			fsz<<=8;	
			fsz+=sdram.bf[dk][StPosHead+9];
			fsz<<=8;
			fsz+=sdram.bf[dk][StPosHead+10];	
			fsz<<=8;
			fsz+=sdram.bf[dk][StPosHead+11];				//PCOB TAG size
			PCOB2_adr = fsz+StPosHead;							//start adress PCOB2
			fsz = sdram.bf[dk][StPosHead+4];			
			fsz<<=8;	
			fsz+=sdram.bf[dk][StPosHead+5];
			fsz<<=8;
			fsz+=sdram.bf[dk][StPosHead+6];	
			fsz<<=8;
			fsz+=sdram.bf[dk][StPosHead+7];				//PCOB head size
			StPosHead+= fsz;		
			uint8_t HCUE_NAME = 0;
			E = 0;	
			while(E<number_of_hot_cue_points[dk])	
				{	
				fsz = sdram.bf[dk][StPosHead+8];			
				fsz<<=8;	
				fsz+=sdram.bf[dk][StPosHead+9];
				fsz<<=8;
				fsz+=sdram.bf[dk][StPosHead+10];	
				fsz<<=8;
				fsz+=sdram.bf[dk][StPosHead+11];				//PCPT TAG size
				SPP = StPosHead+fsz;										//Next PCPT adress

				HCUE_NAME = sdram.bf[dk][StPosHead+15];
				if(sdram.bf[dk][StPosHead+19]!=0)				//when hcue active
					{
					if((HCUE_NAME<4) && (HCUE_NAME>0))
						{
						HCUE_type[dk][HCUE_NAME-1] = 0x02;			//write 
						}
					}
				fsz = sdram.bf[dk][StPosHead+4];			
				fsz<<=8;	
				fsz+=sdram.bf[dk][StPosHead+5];
				fsz<<=8;
				fsz+=sdram.bf[dk][StPosHead+6];	
				fsz<<=8;
				fsz+=sdram.bf[dk][StPosHead+7];				//PCPT Head size
				StPosHead+= fsz;		
		
				if((HCUE_NAME<4) && (HCUE_NAME>0))
					{
					HCUE_NAME--;
					HCUE_adr[dk][0][HCUE_NAME] = sdram.bf[dk][StPosHead+4];
					HCUE_adr[dk][0][HCUE_NAME] = HCUE_adr[dk][0][HCUE_NAME]<<8;
					HCUE_adr[dk][0][HCUE_NAME]+= sdram.bf[dk][StPosHead+5];
					HCUE_adr[dk][0][HCUE_NAME] = HCUE_adr[dk][0][HCUE_NAME]<<8;
					HCUE_adr[dk][0][HCUE_NAME]+= sdram.bf[dk][StPosHead+6];
					HCUE_adr[dk][0][HCUE_NAME] = HCUE_adr[dk][0][HCUE_NAME]<<8;	
					HCUE_adr[dk][0][HCUE_NAME]+= sdram.bf[dk][StPosHead+7];
					if(sdram.bf[dk][StPosHead]==2)						//when hot cue type=loop
						{
						HCUE_type[dk][HCUE_NAME]&= 0x03;	
						HCUE_type[dk][HCUE_NAME] |= 0x01;		
						HCUE_adr[dk][1][HCUE_NAME] = sdram.bf[dk][StPosHead+8];
						HCUE_adr[dk][1][HCUE_NAME] = HCUE_adr[dk][1][HCUE_NAME]<<8;
						HCUE_adr[dk][1][HCUE_NAME]+= sdram.bf[dk][StPosHead+9];
						HCUE_adr[dk][1][HCUE_NAME] = HCUE_adr[dk][1][HCUE_NAME]<<8;
						HCUE_adr[dk][1][HCUE_NAME]+= sdram.bf[dk][StPosHead+10];
						HCUE_adr[dk][1][HCUE_NAME] = HCUE_adr[dk][1][HCUE_NAME]<<8;	
						HCUE_adr[dk][1][HCUE_NAME]+= sdram.bf[dk][StPosHead+11];
						HCUE_adr[dk][1][HCUE_NAME] = (HCUE_adr[dk][1][HCUE_NAME]*3)/20; 	//translate ms to 1/150s frames
						}
					}
				StPosHead = SPP;	
				E++;	
				}
			}
						
		//////////////////prepare MEMORY CUES
		for(E=0;E<10;E++)									//Clear memory cue massive
			{
			MEMORY_type[dk][E] = 0;						//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 		
			MEMORY_adr[dk][0][E] = 0xFFFF;
			MEMORY_adr[dk][1][E] = 0xFFFF;	
			}
		StPosHead = PCOB2_adr;	
		if(sdram.bf[dk][StPosHead+15]==0)    //check type PCOB - for MEMORY CUE points
			{
			number_of_memory_cue_points[dk] = sdram.bf[dk][StPosHead+19]&0xF;				//number of MEMORY points	
			fsz = sdram.bf[dk][StPosHead+4];			
			fsz<<=8;	
			fsz+=sdram.bf[dk][StPosHead+5];
			fsz<<=8;
			fsz+=sdram.bf[dk][StPosHead+6];	
			fsz<<=8;
			fsz+=sdram.bf[dk][StPosHead+7];				//PCOB head size
			StPosHead = StPosHead+fsz;		

			E = 0;	
			while(E<number_of_memory_cue_points[dk])	
				{	
				fsz = sdram.bf[dk][StPosHead+8];			
				fsz<<=8;
				fsz+=sdram.bf[dk][StPosHead+9];
				fsz<<=8;
				fsz+=sdram.bf[dk][StPosHead+10];	
				fsz<<=8;
				fsz+=sdram.bf[dk][StPosHead+11];				//PCPT TAG size
				SPP = StPosHead+fsz;										//Next PCPT adress

				if(sdram.bf[dk][StPosHead+19]!=0)				//when hcue active
					{
					MEMORY_type[dk][E] = 2;			//write 
					}	
				fsz = sdram.bf[dk][StPosHead+4];			
				fsz<<=8;	
				fsz+=sdram.bf[dk][StPosHead+5];
				fsz<<=8;
				fsz+=sdram.bf[dk][StPosHead+6];	
				fsz<<=8;
				fsz+=sdram.bf[dk][StPosHead+7];				//PCPT Head size
				StPosHead = StPosHead+fsz;		
		
				MEMORY_adr[dk][0][E] = sdram.bf[dk][StPosHead+4];
				MEMORY_adr[dk][0][E] = MEMORY_adr[dk][0][E]<<8;
				MEMORY_adr[dk][0][E]+= sdram.bf[dk][StPosHead+5];
				MEMORY_adr[dk][0][E] = MEMORY_adr[dk][0][E]<<8;
				MEMORY_adr[dk][0][E]+= sdram.bf[dk][StPosHead+6];
				MEMORY_adr[dk][0][E] = MEMORY_adr[dk][0][E]<<8;	
				MEMORY_adr[dk][0][E]+= sdram.bf[dk][StPosHead+7];	
				if(sdram.bf[dk][StPosHead]==2)						//when hot cue type=loop
					{
					MEMORY_type[dk][E] |= 0x1;		
					MEMORY_adr[dk][1][E] = sdram.bf[dk][StPosHead+8];
					MEMORY_adr[dk][1][E] = MEMORY_adr[dk][1][E]<<8;
					MEMORY_adr[dk][1][E]+= sdram.bf[dk][StPosHead+9];
					MEMORY_adr[dk][1][E] = MEMORY_adr[dk][1][E]<<8;
					MEMORY_adr[dk][1][E]+= sdram.bf[dk][StPosHead+10];
					MEMORY_adr[dk][1][E] = MEMORY_adr[dk][1][E]<<8;	
					MEMORY_adr[dk][1][E]+= sdram.bf[dk][StPosHead+11];	
					MEMORY_adr[dk][1][E] = (MEMORY_adr[dk][1][E]*3)/20; 	//translate ms to 1/150s frames
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
	
		res = f_open(&file[dk], path_ANLZ, FA_READ);	
		if (res != FR_OK)
			{
			ERROR = 7;	//cannot open ANLZXXXX.EXT file
			}
		else
			{
			FILSIZE = f_size(&file[dk]);
			fsz = FILSIZE;
			if(fsz>8912896)
				{
				fsz = 8912896;
				#if defined(DEBUG_UART_EN)		
				sprintf((char*)U_TX_DATA, "bigfile\n\r");	
				UART_TX(&huart4, U_TX_DATA, 9, 5);		
				#endif						
				}
			res = f_read(&file[dk], sdram.bf[dk], fsz, &nbytes[dk]);
			if(res != FR_OK)
				{
				ERROR = 8;	//ANLZXXXX.EXT file is damadge
				f_close(&file[dk]);					//Close file ANLZXXXX.EXT							
				}
			else
				{
				f_close(&file[dk]);					//Close file ANLZXXXX.EXT			
				fsz = sdram.bf[dk][8];			
				fsz<<=8;	
				fsz+=sdram.bf[dk][9];	
				fsz<<=8;
				fsz+=sdram.bf[dk][10];	
				fsz<<=8;
				fsz+=sdram.bf[dk][11];	//file size		
				if(fsz != FILSIZE)
					{
					ERROR = 9;	//ANLZXXXX.EXT file is damadge
					}
				else		
					{	
					fsz = sdram.bf[dk][4];			
					fsz<<=8;	
					fsz+=sdram.bf[dk][5];	
					fsz<<=8;
					fsz+=sdram.bf[dk][6];	
					fsz<<=8;
					fsz+=sdram.bf[dk][7];	//Header size
					StPosHead = fsz;	
					if(sdram.bf[dk][StPosHead] != 80 	 || 
						 sdram.bf[dk][StPosHead+1] != 80 || 
					   sdram.bf[dk][StPosHead+2] != 84 || 
					   sdram.bf[dk][StPosHead+3] != 72)		//Check PPTH position in file
						{
						ERROR = 10;	//ANLZXXXX.EXT file is damadge				
						}
					else
						{		
						fsz = sdram.bf[dk][StPosHead+8];			
						fsz<<=8;	
						fsz+=sdram.bf[dk][StPosHead+9];	
						fsz<<=8;
						fsz+=sdram.bf[dk][StPosHead+10];	
						fsz<<=8;
						fsz+=sdram.bf[dk][StPosHead+11];	//Tag size
						StPosHead += fsz;		
						uint32_t PWV4_pointer = StPosHead;			//pointer for next TAG - PWV4 
						if(sdram.bf[dk][StPosHead] != 80   || 
							 sdram.bf[dk][StPosHead+1] != 87 || 
						   sdram.bf[dk][StPosHead+2] != 86 || 
						   sdram.bf[dk][StPosHead+3] != 51)		//Check PWV3 position in file
							{
							#if defined(DEBUG_UART_EN)		
							sprintf((char*)U_TX_DATA, "Check PWV3 \n\r");	
							UART_TX(&huart4, U_TX_DATA, 13, 5);		
							#endif				
							return 11;	//ANLZXXXX.EXT file is damadge			
							}
						else
							{	
							fsz = sdram.bf[dk][StPosHead+4];			
							fsz<<=8;	
							fsz+=sdram.bf[dk][StPosHead+5];	
							fsz<<=8;
							fsz+=sdram.bf[dk][StPosHead+6];	
							fsz<<=8;
							fsz+=sdram.bf[dk][StPosHead+7];	//Header size
							uint32_t START_POS_WF = StPosHead + fsz; //Start position waveform	 //old SPP
									
							fsz = sdram.bf[dk][StPosHead+16];			
							fsz<<=8;	
							fsz+=sdram.bf[dk][StPosHead+17];	
							fsz<<=8;
							fsz+=sdram.bf[dk][StPosHead+18];	
							fsz<<=8;
							uint32_t WF_DATA_SIZE = fsz+sdram.bf[dk][StPosHead+19];		//waveform data size	//old fsz
							uint32_t j;							//Data shift	
								
							if(WF_DATA_SIZE>1)				//delete 2 end frames (exclude noise) 
								{
								WF_DATA_SIZE-=2;	
								}
							else
								{
								WF_DATA_SIZE = 0;	
								}									
							all_long[dk] = WF_DATA_SIZE;	
							if(WF_DATA_SIZE>90000)
								{
								WF_DATA_SIZE = 90000;	
								}	
							for(j=0;j<WF_DATA_SIZE;j++)			//Fill dynamic waveform
								{
								a = sdram.bf[dk][j+START_POS_WF]&0x1F;
								a*= 153;
								a>>=8;											
								WFORMDYNAMIC[dk][j] = sdram.bf[dk][j+START_POS_WF]&0xE0;
								WFORMDYNAMIC[dk][j]|=a; 	
								}
							for(j=0;j<(WFD_SIZE-90000);j++)			//clear next arrays
								{	
								WFORMDYNAMIC[dk][90000+j] = 0xE0;			//amplitude = 0, color = 7
								}	
							//////////////////////////////////// fill 2-4-8-16 waveforms
							uint8_t clr, ampl;
							for(j=0;j<(WF_DATA_SIZE>>1);j++)			//Fill dynamic waveform x2
								{
								clr = WFORMDYNAMIC[dk][2*j]>>5;	
								ampl = WFORMDYNAMIC[dk][2*j]&0x1F;	
								for(a=1;a<2;a++)			//2-4-8-16
									{
									if((WFORMDYNAMIC[dk][2*j+a]&0x1F)>ampl)
										{
										ampl = WFORMDYNAMIC[dk][2*j+a]&0x1F;	
										if(ampl>13)
											{
											clr = (WFORMDYNAMIC[dk][2*j+a]>>5);
											}
										}
									}		
								WFORMDYNAMIC[dk][90000+j] = (clr<<5) | ampl;
								}	
							for(j=0;j<(WF_DATA_SIZE>>2);j++)			//Fill dynamic waveform x4
								{
								clr = WFORMDYNAMIC[dk][4*j]>>5;	
								ampl = WFORMDYNAMIC[dk][4*j]&0x1F;	
								for(a=1;a<4;a++)			//2-4-8-16
									{
									if((WFORMDYNAMIC[dk][4*j+a]&0x1F)>ampl)
										{
										ampl	= WFORMDYNAMIC[dk][4*j+a]&0x1F;	
										if(ampl>13)
											{
											clr = (WFORMDYNAMIC[dk][4*j+a]>>5);
											}
										}
									}		
								WFORMDYNAMIC[dk][135000+j] = (clr<<5) | ampl;	
								}		
							for(j=0;j<(WF_DATA_SIZE>>3);j++)			//Fill dynamic waveform x8
								{
								clr = WFORMDYNAMIC[dk][8*j]>>5;	
								ampl = WFORMDYNAMIC[dk][8*j]&0x1F;	
								for(a=1;a<8;a++)			//2-4-8-16
									{
									if((WFORMDYNAMIC[dk][8*j+a]&0x1F)>ampl)
										{
										ampl	= WFORMDYNAMIC[dk][8*j+a]&0x1F;	
										if(ampl>13)
											{
											clr = (WFORMDYNAMIC[dk][8*j+a]>>5);
											}
										}
									}		
								WFORMDYNAMIC[dk][157500+j] = (clr<<5) | ampl;
								}
							for(j=0;j<(WF_DATA_SIZE>>4);j++)			//Fill dynamic waveform x16
								{
								clr = WFORMDYNAMIC[dk][16*j]>>5;	
								ampl = WFORMDYNAMIC[dk][16*j]&0x1F;	
								for(a=1;a<16;a++)			//2-4-8-16
									{
									if((WFORMDYNAMIC[dk][16*j+a]&0x1F)>ampl)
										{
										ampl	= WFORMDYNAMIC[dk][16*j+a]&0x1F;	
										if(ampl>13)
											{
											clr = (WFORMDYNAMIC[dk][16*j+a]>>5);
											}
										}
									}		
								WFORMDYNAMIC[dk][168750+j] = (clr<<5) | ampl;
								}		
							for(E=0;E<number_of_memory_cue_points[dk];E++)											//Draw CUES on Display
								{
								if(MEMORY_adr[dk][0][E] != 0xFFFF)
									{
									#if defined(DEBUG_UART_EN)		
									sprintf((char*)U_TX_DATA, "MEMORY in %06lu ms\n\r", MEMORY_adr[dk][0][E]);											
									UART_TX(&huart4, U_TX_DATA, 21, 15);	
									#endif		
									mem_pos	= 605*MEMORY_adr[dk][0][E];
									mem_pos/= (20*all_long[dk]);
									DrawMemoryMarker(dk, mem_pos, MEMORY_MARK, LCD_COLOR_RED);
									MEMORY_adr[dk][0][E] = (MEMORY_adr[dk][0][E]*3)/20;				//translate ms to 1/150s frames
									}	
								}		
																
							fsz = sdram.bf[dk][StPosHead+8];			
							fsz<<=8;	
							fsz+=sdram.bf[dk][StPosHead+9];	
							fsz<<=8;
							fsz+=sdram.bf[dk][StPosHead+10];	
							fsz<<=8;
							fsz+=sdram.bf[dk][StPosHead+11];	//Tag size
							StPosHead += fsz;		
							if(sdram.bf[dk][StPosHead] != 80 	 || 
								 sdram.bf[dk][StPosHead+1] != 67 || 
								 sdram.bf[dk][StPosHead+2] != 79 || 
								 sdram.bf[dk][StPosHead+3] != 66)		//Check PCOB position in file
								{
								#if defined(DEBUG_UART_EN)		
								sprintf((char*)U_TX_DATA, "Check PCOB \n\r");	
								UART_TX(&huart4, U_TX_DATA, 13, 5);		
								#endif		
								return 11;	//ANLZXXXX.EXT file is damadge			
								}
							else
								{
								fsz = sdram.bf[dk][StPosHead+8];			
								fsz<<=8;	
								fsz+=sdram.bf[dk][StPosHead+9];	
								fsz<<=8;
								fsz+=sdram.bf[dk][StPosHead+10];	
								fsz<<=8;
								fsz+=sdram.bf[dk][StPosHead+11];	//Tag size
								StPosHead += fsz;		
								if(sdram.bf[dk][StPosHead] != 80 || 
								 sdram.bf[dk][StPosHead+1] != 67 || 
								 sdram.bf[dk][StPosHead+2] != 79 || 
								 sdram.bf[dk][StPosHead+3] != 66)		//Check PCO2 position in file
									{
									return 11;	//ANLZXXXX.EXT file is damadge			
									}	
								else
									{
									fsz = sdram.bf[dk][StPosHead+8];			
									fsz<<=8;	
									fsz+=sdram.bf[dk][StPosHead+9];	
									fsz<<=8;
									fsz+=sdram.bf[dk][StPosHead+10];	
									fsz<<=8;
									fsz+=sdram.bf[dk][StPosHead+11];	//Tag size
									StPosHead += fsz;		
									if(sdram.bf[dk][StPosHead] != 80 || 
									 sdram.bf[dk][StPosHead+1] != 67 || 
									 sdram.bf[dk][StPosHead+2] != 79 || 
									 sdram.bf[dk][StPosHead+3] != 50)		//Check PCO2 position in file
										{
										return 11;	//ANLZXXXX.EXT file is damadge			
										}	
									else
										{
										#if defined(DEBUG_UART_EN)		
										sprintf((char*)U_TX_DATA, "Enter PCO2\n\r");	
										UART_TX(&huart4, U_TX_DATA, 12, 5);	
										#endif
										if(sdram.bf[dk][StPosHead+15]==1)				//HOT CUE TYPE
											{
											number_of_hot_cue_points[dk] = sdram.bf[dk][StPosHead+17];
											fsz = sdram.bf[dk][StPosHead+4];			
											fsz<<=8;	
											fsz+=sdram.bf[dk][StPosHead+5];	
											fsz<<=8;
											fsz+=sdram.bf[dk][StPosHead+6];	
											fsz<<=8;
											fsz+=sdram.bf[dk][StPosHead+7];	//header size
											StPosHead += fsz;	
											for(j=0;j<number_of_hot_cue_points[dk];j++)			
												{	
												if(sdram.bf[dk][StPosHead] != 80 || 
													 sdram.bf[dk][StPosHead+1] != 67 || 
													 sdram.bf[dk][StPosHead+2] != 80 || 
													 sdram.bf[dk][StPosHead+3] != 50)		//Check PCP2 position in file
													{
													j = 100;	
													#if defined(DEBUG_UART_EN)		
													sprintf((char*)U_TX_DATA, "J100\n\r");	
													UART_TX(&huart4, U_TX_DATA, 6, 5);		
													#endif			
													return 11;	//ANLZXXXX.EXT file is damadge			
													}
												else
													{
													#if defined(DEBUG_UART_EN)		
													sprintf((char*)U_TX_DATA, "Extract HOT CUE\n\r");	
													UART_TX(&huart4, U_TX_DATA, 17, 5);	
													#endif		
													if(sdram.bf[dk][StPosHead+15]>0)
														{
														HCUE_adr[dk][0][sdram.bf[dk][StPosHead+15]-1] = sdram.bf[dk][StPosHead+20]; 		
														HCUE_adr[dk][0][sdram.bf[dk][StPosHead+15]-1]<<=8;	
														HCUE_adr[dk][0][sdram.bf[dk][StPosHead+15]-1]+=sdram.bf[dk][StPosHead+21];
														HCUE_adr[dk][0][sdram.bf[dk][StPosHead+15]-1]<<=8;	
														HCUE_adr[dk][0][sdram.bf[dk][StPosHead+15]-1]+=sdram.bf[dk][StPosHead+22];	
														HCUE_adr[dk][0][sdram.bf[dk][StPosHead+15]-1]<<=8;	
														HCUE_adr[dk][0][sdram.bf[dk][StPosHead+15]-1]+=sdram.bf[dk][StPosHead+23];
														if(sdram.bf[dk][StPosHead+16]==2)						//when hot cue type=loop
															{
															HCUE_adr[dk][1][sdram.bf[dk][StPosHead+15]-1] = sdram.bf[dk][StPosHead+24]; 		
															HCUE_adr[dk][1][sdram.bf[dk][StPosHead+15]-1]<<=8;	
															HCUE_adr[dk][1][sdram.bf[dk][StPosHead+15]-1]+=sdram.bf[dk][StPosHead+25];
															HCUE_adr[dk][1][sdram.bf[dk][StPosHead+15]-1]<<=8;	
															HCUE_adr[dk][1][sdram.bf[dk][StPosHead+15]-1]+=sdram.bf[dk][StPosHead+26];	
															HCUE_adr[dk][1][sdram.bf[dk][StPosHead+15]-1]<<=8;	
															HCUE_adr[dk][1][sdram.bf[dk][StPosHead+15]-1]+=sdram.bf[dk][StPosHead+27];	
															}
														E=sdram.bf[dk][StPosHead+42];	
														E<<=8;
														E+=sdram.bf[dk][StPosHead+43];	//len_comment	
														HCUE32COLOR[dk][sdram.bf[dk][StPosHead+15]-1] = sdram.bf[dk][StPosHead+45+E];																
														HCUE32COLOR[dk][sdram.bf[dk][StPosHead+15]-1]<<=8;	
														HCUE32COLOR[dk][sdram.bf[dk][StPosHead+15]-1]+= sdram.bf[dk][StPosHead+46+E];		
														HCUE32COLOR[dk][sdram.bf[dk][StPosHead+15]-1]<<=8;		
														HCUE32COLOR[dk][sdram.bf[dk][StPosHead+15]-1]+= sdram.bf[dk][StPosHead+47+E];	
														}														
													fsz = sdram.bf[dk][StPosHead+8];			
													fsz<<=8;	
													fsz+=sdram.bf[dk][StPosHead+9];	
													fsz<<=8;
													fsz+=sdram.bf[dk][StPosHead+10];	
													fsz<<=8;
													fsz+=sdram.bf[dk][StPosHead+11];	//len_entry
													StPosHead+=fsz;		
													}													
												}												
											}
										///////add memory cues reading
											
										////////////////////////////////////sorting algoritm	
										uint8_t MINM, ii;
															
										for(E=0;E<number_of_memory_cue_points[dk]-1;E++)	
											{
											MINM = E;
											for(ii=(E+1);ii<number_of_memory_cue_points[dk];ii++)
												{
												if(MEMORY_adr[dk][0][ii]<MEMORY_adr[dk][0][MINM])
													{
													j = MEMORY_adr[dk][0][MINM];
													MEMORY_adr[dk][0][MINM] = MEMORY_adr[dk][0][ii];
													MEMORY_adr[dk][0][ii] = j;
													j = MEMORY_adr[dk][1][MINM];
													MEMORY_adr[dk][1][MINM] = MEMORY_adr[dk][1][ii];
													MEMORY_adr[dk][1][ii] = j;												
													j = MEMORY_type[dk][MINM];
													MEMORY_type[dk][MINM] = MEMORY_type[dk][ii];
													MEMORY_type[dk][ii] = j;	
													}
												}
											}

										uint8_t	r, g, b, b2;	
										for(E=0;E<8;E++)											//Draw MEMORY on Display
											{
											if(HCUE_adr[dk][0][E] != 0xFFFF)
												{
												HCUEPCOLOR[dk][E][0] = ((HCUE32COLOR[dk][E]&0x00FFFFFF)>>16)/3;	
												HCUEPCOLOR[dk][E][1] =	((HCUE32COLOR[dk][E]&0x0000FFFF)>>8)/3;
												HCUEPCOLOR[dk][E][2] = (HCUE32COLOR[dk][E]&0x000000FF)/3;
												r = (HCUE32COLOR[dk][E]&0x00FFFFFF)>>19;	
												g = (HCUE32COLOR[dk][E]&0x0000FFFF)>>11;
												b = (HCUE32COLOR[dk][E]&0x000000FF)>>3;	
												HCUE16COLOR[dk][E] = 0x8000+(0x400*r)+(0x20*g)+b; 
												mem_pos	= 605*HCUE_adr[dk][0][E];
												mem_pos/= (20*all_long[dk]);
												DrawMemoryMarker(dk, mem_pos, 2+E, HCUE16COLOR[dk][E]);				//square
												#if defined(DEBUG_UART_EN)		
												sprintf((char*)U_TX_DATA, "H.CUE in %06lu ms  ", HCUE_adr[dk][0][E]);											
												UART_TX(&huart4, U_TX_DATA, 20, 15);												
												#endif		
													
												HCUE_adr[dk][0][E] = (HCUE_adr[dk][0][E]*3)/20;				//translate ms to 1/150s frames										
												if(E==0)							//HCUE_A
													{
													if(HCUE_type[dk][E]&0x1)
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
													if(HCUE_type[dk][E]&0x1)
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
													if(HCUE_type[dk][E]&0x1)
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
												HCUEPCOLOR[dk][E][0] = HCUEdisableCOLOR[0];	
												HCUEPCOLOR[dk][E][1] =	HCUEdisableCOLOR[1];
												HCUEPCOLOR[dk][E][2] = HCUEdisableCOLOR[2];
												}
											}	

										StPosHead = PWV4_pointer;			//pointer for next TAG - PWV5 
										uint16_t als;	
										for(E=0;E<11;E++)											//Search PWV5 
											{	
											#if defined(DEBUG_UART_EN)		
											UART_TX(&huart4, &sdram.bf[dk][StPosHead], 4, 5);		
											sprintf((char*)U_TX_DATA, "\n\r");	
											UART_TX(&huart4, U_TX_DATA, 2, 5);
											#endif		
											if(sdram.bf[dk][StPosHead] != 80)
												{
												E = 100;	
												ERROR = 10;	//ANLZXXXX.EXT file is damadge		
												}
											else
												{
												if(sdram.bf[dk][StPosHead+1]==87 && 
													sdram.bf[dk][StPosHead+2]==86  && 
													sdram.bf[dk][StPosHead+3]== 52)		//Check PWV4 position in file
													{
													E = 50;	
													}
												else
													{
													fsz = sdram.bf[dk][StPosHead+8];			
													fsz<<=8;	
													fsz+=sdram.bf[dk][StPosHead+9];	
													fsz<<=8;
													fsz+=sdram.bf[dk][StPosHead+10];	
													fsz<<=8;
													fsz+=sdram.bf[dk][StPosHead+11];	//Tag size
													StPosHead += fsz;		
													}													
												}							
											}	
											
										fsz = sdram.bf[dk][StPosHead+4];	//pointer to start 7200 data		
										fsz<<=8;	
										fsz+=sdram.bf[dk][StPosHead+5];	
										fsz<<=8;
										fsz+=sdram.bf[dk][StPosHead+6];	
										fsz<<=8;
										fsz+=sdram.bf[dk][StPosHead+7];	//header size
										StPosHead += fsz;	
											
										for(E=0;E<202;E++)					//Fill Static Waveform 1200->202
											{
											y = 1521*E;
											y>>=8;					
											als = sdram.bf[dk][StPosHead+y*6+2] + 		//average for blue color map
												sdram.bf[dk][StPosHead+(y+1)*6+2] + 
												sdram.bf[dk][StPosHead+(y+2)*6+2] + 
												sdram.bf[dk][StPosHead+(y+3)*6+2] + 
												sdram.bf[dk][StPosHead+(y+4)*6+2] + 2; 
											sdram.bf[dk][StPosHead+y*6+2] = als/5;
												
											als = sdram.bf[dk][StPosHead+y*6+3] + 		//average for RGB colors and levels
												sdram.bf[dk][StPosHead+(y+1)*6+3] + 
												sdram.bf[dk][StPosHead+(y+2)*6+3] + 
												sdram.bf[dk][StPosHead+(y+3)*6+3] + 
												sdram.bf[dk][StPosHead+(y+4)*6+3] + 2; 
											r = als/5;
											als = sdram.bf[dk][StPosHead+y*6+4] + 
												sdram.bf[dk][StPosHead+(y+1)*6+4] + 
												sdram.bf[dk][StPosHead+(y+2)*6+4] + 
												sdram.bf[dk][StPosHead+(y+3)*6+4] + 
												sdram.bf[dk][StPosHead+(y+4)*6+4] + 2; 
											g = als/5;
											als = sdram.bf[dk][StPosHead+y*6+5] + 
												sdram.bf[dk][StPosHead+(y+1)*6+5] + 
												sdram.bf[dk][StPosHead+(y+2)*6+5] + 
												sdram.bf[dk][StPosHead+(y+3)*6+5] + 
												sdram.bf[dk][StPosHead+(y+4)*6+5] + 2; 
											b = als/5;
											if(r>127)
												{
												r = 0;	
												}												
											if(g>127)
												{
												g = 0;	
												}
											if(b>127)
												{
												b = 0;	
												}	
											//////////adaptive code from Pioneer waveform changer utility	
											if(sdram.bf[dk][StPosHead+y*6+2]>63 && r>64)		//find low freq
												{
												b2 = 127;	
												sdram.bf[dk][StPosHead+y*6+2]+=15;	
												if(sdram.bf[dk][StPosHead+y*6+2]>127)
													{
													sdram.bf[dk][StPosHead+y*6+2] = 127;	
													}				
												}
											else
												{
												b2 = 0;	
												if(sdram.bf[dk][StPosHead+y*6+2]<15)
													{
													sdram.bf[dk][StPosHead+y*6+2] = 0;	
													}
												else
													{
													sdram.bf[dk][StPosHead+y*6+2]-=15;	
													}													
												}											
											if(r<b)
												{
												if(g<b)
													{		 
													if(r>g) //b max  g min
														{
														b = r+1;	//down b
														}													
													else	//b max  r min
														{
														b = g; 	//down b	
														}													
													if(b2==0)
														{
														r-=r/3;
														g-=g/3;		
														}	
													else
														{
														b-=b/6;	
														g-=g/5;	
														}													
													}												
												else		//g max
													{
													if(b2==0)
														{	
														r-=r/3;
														b-=b/5;
														if(g>55)
															{
															g-=55;
															g/=3;
															g+=55;			
															}														
														}
													else
														{
														r+=r/4;		//up r
														if(r>g)
															{
															r = g;	
															}														
														b-=b/3;	
														}
													}												
												}	
											else
												{
												if(g<r)
													{			//red is max
													if(b2==0)
														{
														g-=g/4;
														b-=b/4;			
														}
													else
														{
														r+=r/5;
														if(r>127)
															{
															r = 127;	
															}														
														g-=g/4;	
														b-=b/3;	
														}													
													}												
												else
													{		//green is max and blue is min
													if(b2==0)
														{
														r-=r/5;
														b-=b/5;
														if(g>55)
															{
															g-=55;
															g/=3;
															g+=55;			
															}	
														}
													else
														{
														r+=r/3;
														if(r>g)
															{
															r = g;	
															}														
														b-=b/4;		
														}													
													}											
												}							
											WFST_AHB[dk][E] = r;			//Find Max color = amplitude
											if(WFST_AHB[dk][E]<g)
												{
												WFST_AHB[dk][E] = g;	
												}												
											if(WFST_AHB[dk][E]<b)
												{
												WFST_AHB[dk][E] = b;	
												}		

											WFST_AL[dk][E] = b;			//blue color = amplitude
											
											als = 127*r;						//colors normalization
											als/=WFST_AHB[dk][E];	
											r = als;
											als = 127*g;
											als/=WFST_AHB[dk][E];	
											g = als;
											als = 127*b;
											als/=WFST_AHB[dk][E];	
											b = als;
											WFST_RGB[dk][E] = 0x8000 | ((r>>2)<<10) | ((g>>2)<<5) | (b>>2); 		//sample color
											}
											
										r = 0;	
										for(E=0;E<202;E++)					//amplitude normalization (find max)
											{	
											if(r<WFST_AHB[dk][E])
												{
												r = WFST_AHB[dk][E];	
												}
											if(r<WFST_AL[dk][E])
												{
												r = WFST_AL[dk][E];	
												}
											}	

										for(E=0;E<202;E++)					//amplitude normalization 
											{
											y = 1521*E;
											y>>=8;
										  als = WFST_AHB[dk][E]*20;	//convert amplitude 127->18	
											als/=r;
											if(als>18)
												{
												als = 18;	
												}								
											WFST_AHB[dk][E] = als;	
											als = WFST_AL[dk][E]*20;	 //convert amplitude 127->18
											als/=r;
											if(als>18)
												{
												als = 18;	
												}								
											WFST_AL[dk][E] = als;			
											if(sdram.bf[dk][StPosHead+y*6+2]<16)
												{
												WFST_AHB[dk][E]|= 192;		//ampl+color map	
												}												
											else if(sdram.bf[dk][StPosHead+y*6+2]<48)
												{
												WFST_AHB[dk][E]|= 160;		//ampl+color map	
												}	
											else if(sdram.bf[dk][StPosHead+y*6+2]<64)
												{
												WFST_AHB[dk][E]|= 128;		//ampl+color map	
												}	
											else if(sdram.bf[dk][StPosHead+y*6+2]<80)
												{
												WFST_AHB[dk][E]|= 96;		//ampl+color map	
												}	
											else if(sdram.bf[dk][StPosHead+y*6+2]<96)
												{
												WFST_AHB[dk][E]|= 64;		//ampl+color map	
												}
											else if(sdram.bf[dk][StPosHead+y*6+2]<112)
												{
												WFST_AHB[dk][E]|= 32;		//ampl+color map	
												}
											else if(sdram.bf[dk][StPosHead+y*6+2]<128)
												{
												//ampl+color map (null)
												}
											else
												{
												WFST_AHB[dk][E]|= 192;		//ampl+color map													
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

		res = f_open(&file[dk], path_AUDIOTRACK, FA_READ);				//Open audio file
		#if defined(DEBUG_UART_EN)	
		sprintf((char*)U_TX_DATA, "\n\r");	
		UART_TX(&huart4, U_TX_DATA, 2, 5);				
		UART_TX(&huart4, path_AUDIOTRACK, 150, 45);	
		UART_TX(&huart4, U_TX_DATA, 2, 5);	
		#endif
		if (res != FR_OK)
			{
			return 13;	//cannot open AUDIOTRACK
			}
		res = f_read(&file[dk], sdram.pcm[dk], 512, &nbytes[dk]);
		if(res != FR_OK)
			{
			ERROR = 14;	//cannot read AUDIOTRACK
 			}
		#if defined(DEBUG_UART_EN)	
		sprintf((char*)U_TX_DATA, "\n\r");	
		UART_TX(&huart4, U_TX_DATA, 2, 5);				
		UART_TX(&huart4, (uint8_t *)sdram.pcm, 50, 45);	
		UART_TX(&huart4, U_TX_DATA, 2, 5);	
		#endif		
			
		if(sdram.pcm[dk][0][5][0] != 1 || sdram.pcm[dk][0][5][1] != 2 || sdram.pcm[dk][0][6][0] != 44100 || sdram.pcm[dk][0][8][1] != 16)	//Check audio format
			{
			ERROR = 15;	//unsupported audio format	
			}
		f_lseek(&file[dk], 44);
		if(ERROR==0)
			{
			playlist[TRACK_NUMBER-1][54] |= 0x01;				//write history mem
			}
		curr_trck_color[dk] = (rating[TRACK_NUMBER-1]&0x0F);	
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
	f_close(&file[dk]);		
		
	if(dk==dkA)
		{
		for(i=0;i<8;i++)
			{
			deckTbuf[i][1] = HCUEdisableCOLOR[0];
			deckTbuf[i][2] = HCUEdisableCOLOR[1];
			deckTbuf[i][3] = HCUEdisableCOLOR[2];
			}		
		}
	else
		{
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
	CURSOR_COLOR[dk] = LCD_COLOR_BLACK;	
	if(dk==dkA)
		{
		if(play_enable[dkB] && masterdeck!=dkB)
			{
			masterdeck = dkB;
			tempo_need_update[dkA] = 2;
			tempo_need_update[dkB] = 2;	
			}
		}
	else
		{		
		if(play_enable[dkA] && masterdeck!=dkA)
			{
			masterdeck = dkA;
			tempo_need_update[dkA] = 2;
			tempo_need_update[dkB] = 2;	
			}
		}	
	ERR = LOAD_TRACK(dk, TRACK_NUMBER);	
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
		CURSOR_COLOR[dk] = LCD_COLOR_WHITE;	
			
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
				f_read(&file[dk], sdram.pcm[dk][0][0], 32768, &nbytes[dk]);	
				for(M=0;M<8192;M++)
					{
					if(sdram.pcm[dk][0][M][0]&0x8000)		//negative 65535...32768
						{
						sdram.pcm[dk][0][M][0] = 0xFFFF - sdram.pcm[dk][0][M][0];	
						}
					if(((sdram.pcm[dk][0][M][0]>>(9-UT_SET[ACUE]))>0) && have_a_cue==0)
						{
						have_a_cue = 1;
						CUE_ADR[dk] = (c*8192+M)/294;
						play_adr[dk] = CUE_ADR[dk]*294;
						slip_pl_adr[dk] = play_adr[dk];
						M = 0xFFFF;
						}
					if(sdram.pcm[dk][0][M][1]&0x8000)		//negative 65535...32768
						{
						sdram.pcm[dk][0][M][1] = 0xFFFF - sdram.pcm[dk][0][M][1];	
						}
					if(((sdram.pcm[dk][0][M][1]>>(9-UT_SET[ACUE]))>0) && have_a_cue==0)
						{
						have_a_cue = 1;
						CUE_ADR[dk] = (c*8192+M)/294;
						play_adr[dk] = CUE_ADR[dk]*294;
						slip_pl_adr[dk] = play_adr[dk];
						M = 0xFFFF;	
						}
					}	
				}
			f_lseek(&file[dk], 44);						//return to start track		
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
		f_close(&file[dk]);
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
	if(prev_inair[dk]==0)		//inactive
		{
		if(UT_SET[11]<9)
			{
			jog_work_color[dk] = jog_color[UT_SET[12]][UT_SET[11]];	
			}					
		else
			{
			jog_work_color[dk] = jog_color[UT_SET[12]][curr_trck_color[dk]];		
			}
		}
	else		//in air
		{
		if(UT_SET[10]<9)
			{
			jog_work_color[dk] = jog_color[UT_SET[12]][UT_SET[10]];	
			}					
		else
			{
			jog_work_color[dk] = jog_color[UT_SET[12]][curr_trck_color[dk]];		
			}		
		}				
	SET_JOG_COLOR(dk, jog_work_color[dk]);	
	
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
