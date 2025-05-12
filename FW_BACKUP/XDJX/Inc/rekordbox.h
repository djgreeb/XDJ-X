/////////////////////////////////////////////////////////////////////////////////////////////////
//  Rekordbox database parser based on CDJ-1000mk3 new life project
//	product: Technics SL-DZ1200 new life
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t DATABASE_PARSER(void);
uint8_t PlaylistID_to_Pos(uint8_t ID);										//convert playlist ID to position in Tracklist name
uint16_t LOAD_TRACK(uint16_t TRACK_NUMBER);
void PREPARE_LOAD_TRACK(uint16_t TRACK_NUMBER, uint16_t TRACK_IN_PLAYLIST);



////////////////////////////////////////////////////////////////////////
//
//
//Rekordbox database parser ver. 0.42
//Functions:
//Open and read file export.pdb
//find all tracks
//write trackname in massive
//write adress position datatrack in file export.pdb
//return number of tracks or 0 if error enable 
//extended to 8 hot cues reading from *.EXT files
//bug fixed: path_ANLZ expanded to 46 bytes, for final 0 
//
//
////////////////////////////////////////////////////////////////////////
uint16_t DATABASE_PARSER(void)
	{	
	#if defined(DEBUG_UART_EN)		
	sprintf((char*)U_TX_DATA, "Start Rekordbox parser ver. 0.42\n\r");	
	HAL_UART_Transmit(&huart4, U_TX_DATA, 34, 55);	
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
		WFORMDYNAMIC[4096+T_ID] = 0xFF;				//give out buffer for track ID in WFORMDYNAMIC	offset 8192
		}
	T_ID = 0;	
		
	res = f_read(&file, WFORMDYNAMIC, 4096, &nbytes);
	if (res != FR_OK)
		{
		return 0;	
		}	
	crsr = 28;		
	while(crsr<(28+16*(WFORMDYNAMIC[8] + 256*WFORMDYNAMIC[9] + 65536*WFORMDYNAMIC[10] + 16777216*WFORMDYNAMIC[11])))	//scan first page and search link for all type pages with tables 
		{
		if(WFORMDYNAMIC[crsr]==0 && 
			 WFORMDYNAMIC[crsr+1]==0 && 
			 WFORMDYNAMIC[crsr+2]==0 && 
			 WFORMDYNAMIC[crsr+3]==0)	//Track metadata: title, artist, genre, artwork ID, playing time, etc
			{
			NEXT_PAGE_0 = WFORMDYNAMIC[crsr+8] + 256*WFORMDYNAMIC[crsr+9] + 65536*WFORMDYNAMIC[crsr+10] + 16777216*WFORMDYNAMIC[crsr+11];
			LAST_PAGE_0 = WFORMDYNAMIC[crsr+12] + 256*WFORMDYNAMIC[crsr+13] + 65536*WFORMDYNAMIC[crsr+14] + 16777216*WFORMDYNAMIC[crsr+15];
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "LP0 %08lu\n\r", LAST_PAGE_0);	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 14, 55);	
			#endif		
			}
		else if(WFORMDYNAMIC[crsr]==5 && 
						WFORMDYNAMIC[crsr+1]==0 && 
						WFORMDYNAMIC[crsr+2]==0 && 
						WFORMDYNAMIC[crsr+3]==0)	//Musical keys, for reference by tracks, searching, and key matching
			{
			NEXT_PAGE_5 = WFORMDYNAMIC[crsr+8] + 256*WFORMDYNAMIC[crsr+9] + 65536*WFORMDYNAMIC[crsr+10] + 16777216*WFORMDYNAMIC[crsr+11];	
			LAST_PAGE_5 = WFORMDYNAMIC[crsr+12] + 256*WFORMDYNAMIC[crsr+13] + 65536*WFORMDYNAMIC[crsr+14] + 16777216*WFORMDYNAMIC[crsr+15];	
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "LP5 %08lu\n\r", LAST_PAGE_5);	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 14, 55);	
			#endif	
			}	
		else if(WFORMDYNAMIC[crsr]==7 && 
						WFORMDYNAMIC[crsr+1]==0 && 
						WFORMDYNAMIC[crsr+2]==0 && 
						WFORMDYNAMIC[crsr+3]==0)	//Describes the hierarchical tree structure of available playlists and folders grouping them
			{
			NEXT_PAGE_7 = WFORMDYNAMIC[crsr+8] + 256*WFORMDYNAMIC[crsr+9] + 65536*WFORMDYNAMIC[crsr+10] + 16777216*WFORMDYNAMIC[crsr+11];	
			LAST_PAGE_7 = WFORMDYNAMIC[crsr+12] + 256*WFORMDYNAMIC[crsr+13] + 65536*WFORMDYNAMIC[crsr+14] + 16777216*WFORMDYNAMIC[crsr+15];
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "LP7 %08lu\n\r", LAST_PAGE_7);	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 14, 55);	
			#endif					
			}		
		else if(WFORMDYNAMIC[crsr]==8 && 
						WFORMDYNAMIC[crsr+1]==0 && 
						WFORMDYNAMIC[crsr+2]==0 && 
						WFORMDYNAMIC[crsr+3]==0)	//Links tracks to playlists, in the right order
			{
			NEXT_PAGE_8 = WFORMDYNAMIC[crsr+8] + 256*WFORMDYNAMIC[crsr+9] + 65536*WFORMDYNAMIC[crsr+10] + 16777216*WFORMDYNAMIC[crsr+11];	
			LAST_PAGE_8 = WFORMDYNAMIC[crsr+12] + 256*WFORMDYNAMIC[crsr+13] + 65536*WFORMDYNAMIC[crsr+14] + 16777216*WFORMDYNAMIC[crsr+15];	
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "LP8 %08lu\n\r", LAST_PAGE_8);	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 14, 55);	
			#endif				
			}	
		else if(WFORMDYNAMIC[crsr]==19 && 
						WFORMDYNAMIC[crsr+1]==0 && 
						WFORMDYNAMIC[crsr+2]==0 && 
						WFORMDYNAMIC[crsr+3]==0)	//Links tracks to playlists, in the right order
			{
			NEXT_PAGE_19 = WFORMDYNAMIC[crsr+8] + 256*WFORMDYNAMIC[crsr+9] + 65536*WFORMDYNAMIC[crsr+10] + 16777216*WFORMDYNAMIC[crsr+11];	
			LAST_PAGE_19 = WFORMDYNAMIC[crsr+12] + 256*WFORMDYNAMIC[crsr+13] + 65536*WFORMDYNAMIC[crsr+14] + 16777216*WFORMDYNAMIC[crsr+15];
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "LP19 %08lu\n\r", LAST_PAGE_19);	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 15, 55);	
			#endif					
			}		
		crsr+=16;		
		}
	crsr = 0;
		
	f_lseek(&file, 4096*NEXT_PAGE_0); 				//first page with track metadata: title, artist, genre, artwork ID, playing time, etc
	
	while(NEXT_PAGE_0<(FILSIZE>>12) && LAST_PAGE_0>=NEXT_PAGE_0)	
		{
		res = f_read(&file, WFORMDYNAMIC, 4096, &nbytes);
		if (res != FR_OK)
			{
			return 0;	
			}
		crsr = 0;
		P_PAGE_0 = NEXT_PAGE_0;
		NEXT_PAGE_0 = WFORMDYNAMIC[12] + 256*WFORMDYNAMIC[13] + 65536*WFORMDYNAMIC[14] + 16777216*WFORMDYNAMIC[15];		
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "NP0 %08lu\n\r", NEXT_PAGE_0);	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 14, 55);	
		#endif			
		if(WFORMDYNAMIC[8]==0 && WFORMDYNAMIC[9]==0 && WFORMDYNAMIC[10]==0 && WFORMDYNAMIC[11]==0 && (WFORMDYNAMIC[27]&0x40)==0 && WFORMDYNAMIC[36]==0 && WFORMDYNAMIC[37]==0)		//check page type
			{			
			while(crsr<4095)
				{
				if(WFORMDYNAMIC[crsr]==0x24 && WFORMDYNAMIC[crsr+1]==0x00 && WFORMDYNAMIC[crsr+8]==0x44 && WFORMDYNAMIC[crsr+9]==0xAC 
					&& WFORMDYNAMIC[crsr+48]==0x83 && WFORMDYNAMIC[crsr+49]==0x05 && WFORMDYNAMIC[crsr+82]==0x10)		//check 0x2400 and sample rate 44100 bitrate 1411kbps 16Bits
					{	
					T_ID = WFORMDYNAMIC[crsr+72] + 256*WFORMDYNAMIC[crsr+73];		
					if(T_ID>512)
						{	
						f_close(&file);	
						return 0xFFFF;	
						}
						
					if(WFORMDYNAMIC[4096+2*(T_ID-1)]==0xFF && WFORMDYNAMIC[4097+2*(T_ID-1)]==0xFF)		//check track new or 
						{
						all_trks++;	
						}	
					WFORMDYNAMIC[4096+2*(T_ID-1)] = WFORMDYNAMIC[crsr+72];	
					WFORMDYNAMIC[4097+2*(T_ID-1)] = WFORMDYNAMIC[crsr+73];		
					crsr+= 32;	
					key_id[T_ID-1] = WFORMDYNAMIC[crsr];
					crsr+= 24;					
					BPM_CNT = WFORMDYNAMIC[crsr] + 256*WFORMDYNAMIC[crsr+1] + 65535*WFORMDYNAMIC[crsr+2];	
					BPM_CNT = BPM_CNT/10;	
					original_tempo[T_ID-1] = BPM_CNT;
					crsr+= 28;	
					duration[T_ID-1] = WFORMDYNAMIC[crsr] + 256*WFORMDYNAMIC[crsr+1];	
					crsr+= 4;
					rating[T_ID-1] = WFORMDYNAMIC[crsr] + 256*WFORMDYNAMIC[crsr+1];
					crsr+= 4;						
					cycle_en = 1;							
					while(crsr<4095 && cycle_en)
						{	
						if(WFORMDYNAMIC[crsr]==str[1])
							{
							if(WFORMDYNAMIC[crsr-1]==str[0] && 
								 WFORMDYNAMIC[crsr+1]==str[2] && 
								 WFORMDYNAMIC[crsr+2]==str[3])				//".DAT" Finded!
								{
								parcser_adress[T_ID-1] = (4096*P_PAGE_0)+crsr;										//save position ".[D]AT"	
								crsr = crsr+16;									
								for(i=0;((i<54) && (WFORMDYNAMIC[crsr+i]!=3));i++)						//copy track name	
									{		
									if((WFORMDYNAMIC[crsr+i]<32) || (WFORMDYNAMIC[crsr+i]>126))				//filtering UNICODE symbols
										{
										playlist[T_ID-1][i] = 32;	
										}										
									else
										{										
										playlist[T_ID-1][i] = WFORMDYNAMIC[crsr+i];
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
		res = f_read(&file, WFORMDYNAMIC, 4096, &nbytes);
		if (res != FR_OK)
			{
			return 0;	
			}
		crsr = 0;
		NEXT_PAGE_5 = WFORMDYNAMIC[12] + 256*WFORMDYNAMIC[13] + 65536*WFORMDYNAMIC[14] + 16777216*WFORMDYNAMIC[15];						
		if(WFORMDYNAMIC[8]==5 && WFORMDYNAMIC[9]==0 && WFORMDYNAMIC[10]==0 && WFORMDYNAMIC[11]==0 && (WFORMDYNAMIC[27]&0x40)==0 && WFORMDYNAMIC[36]==0 && WFORMDYNAMIC[37]==0 && WFORMDYNAMIC[24]!=0)		//check page type
			{	
			P_PAGE_0 = WFORMDYNAMIC[24];	
			crsr = 40;	
			while(P_PAGE_0>0)	
				{					
				KEYS[WFORMDYNAMIC[crsr]-1][0] = WFORMDYNAMIC[crsr+9];
				KEYS[WFORMDYNAMIC[crsr]-1][1] = WFORMDYNAMIC[crsr+10];	
				KEYS[WFORMDYNAMIC[crsr]-1][2] = WFORMDYNAMIC[crsr+11];
				KEYS[WFORMDYNAMIC[crsr]-1][3] = WFORMDYNAMIC[crsr+12];
				if(KEYS[WFORMDYNAMIC[crsr]-1][3]<33 || KEYS[WFORMDYNAMIC[crsr]-1][3]>125)
					{
					KEYS[WFORMDYNAMIC[crsr]-1][3] = 0;	
					}
						
				if(WFORMDYNAMIC[crsr+8]==5)
					{
					KEYS[WFORMDYNAMIC[crsr]-1][1] = 0;
					crsr+=12;	
					}
				else if(WFORMDYNAMIC[crsr+8]==7)
					{
					KEYS[WFORMDYNAMIC[crsr]-1][2] = 0;
					crsr+=12;	
					}	
				else if(WFORMDYNAMIC[crsr+8]==9)
					{
					KEYS[WFORMDYNAMIC[crsr]-1][3] = 0;
					crsr+=12;	
					}
				else
					{	
					crsr = crsr + 12 + 4*((((WFORMDYNAMIC[crsr+8]-1)/2)-1)/4);		
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
		WFORMDYNAMIC[8192+i] = 0;	
		}		
	while(LAST_PAGE_7<(FILSIZE>>12) && cycle_en)	
		{
		if(LAST_PAGE_7==NEXT_PAGE_7)			//scan last page
			{
			cycle_en = 0;	
			}
		res = f_read(&file, WFORMDYNAMIC, 4096, &nbytes);
		if (res != FR_OK)
			{
			return 0;	
			}
		crsr = 0;
		NEXT_PAGE_7 = WFORMDYNAMIC[12] + 256*WFORMDYNAMIC[13] + 65536*WFORMDYNAMIC[14] + 16777216*WFORMDYNAMIC[15];			
		if(WFORMDYNAMIC[8]==7 && WFORMDYNAMIC[9]==0 && WFORMDYNAMIC[10]==0 && WFORMDYNAMIC[11]==0 && (WFORMDYNAMIC[27]&0x40)==0 && WFORMDYNAMIC[36]==0 && WFORMDYNAMIC[37]==0 && WFORMDYNAMIC[24]!=0)		//check page type
			{	
			P_PAGE_0 = WFORMDYNAMIC[24];
			crsr = 40;	
			P_PAGE_0 = P_PAGE_0 + 1;		
			while(P_PAGE_0>0)	
				{		
				BPM_CNT = (((WFORMDYNAMIC[crsr+20]-1)/2)-1);		//playlist lenght name	
				if(WFORMDYNAMIC[crsr+16]==0 && WFORMDYNAMIC[crsr+17]==0 && WFORMDYNAMIC[crsr+18]==0 && WFORMDYNAMIC[crsr+19]==0)			//row is playlist
					{
					//WFORMDYNAMIC[crsr+12]			//playlist ID		
					for(i=0;(i<BPM_CNT && i<20); i++)
						{
						WFORMDYNAMIC[8192+21*(WFORMDYNAMIC[crsr+12]-1)+i] = WFORMDYNAMIC[crsr+21+i];	
						}
					WFORMDYNAMIC[8192+21*(WFORMDYNAMIC[crsr+12]-1)+20] = WFORMDYNAMIC[crsr+12];			//playlist ID	 	
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
			if(WFORMDYNAMIC[8192+20+21*i]==BPM_CNT)				//have a playlist
				{
				for(cnt=0;cnt<21;cnt++)
					{
					TRACKLIST_NAME[TOTAL_TRACKLISTS][cnt] = WFORMDYNAMIC[8192+cnt+21*i];
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
		WFORMDYNAMIC[8192+i] = 0xFF;	
		}		
		
	while(LAST_PAGE_8<(FILSIZE>>12) && cycle_en)	
		{
		if(LAST_PAGE_8==NEXT_PAGE_8)			//scan last page
			{
			cycle_en = 0;	
			}
		res = f_read(&file, WFORMDYNAMIC, 4096, &nbytes);
		if (res != FR_OK)
			{
			return 0;	
			}
		crsr = 0;
		NEXT_PAGE_8 = WFORMDYNAMIC[12] + 256*WFORMDYNAMIC[13] + 65536*WFORMDYNAMIC[14] + 16777216*WFORMDYNAMIC[15];			
		if(WFORMDYNAMIC[8]==8 && WFORMDYNAMIC[9]==0 && WFORMDYNAMIC[10]==0 && WFORMDYNAMIC[11]==0 && (WFORMDYNAMIC[27]&0x40)==0 && WFORMDYNAMIC[36]==0 && WFORMDYNAMIC[37]==0 && (WFORMDYNAMIC[24]!=0 || WFORMDYNAMIC[25]!=0))		//check page type
			{	
			P_PAGE_0 = WFORMDYNAMIC[34] + 256*WFORMDYNAMIC[35];
			crsr = 40;	
			P_PAGE_0 = P_PAGE_0 + 1;	
			while(P_PAGE_0>0)	
				{
				i = PlaylistID_to_Pos(WFORMDYNAMIC[crsr+8]);
				if(i==20)
					{
					return 0xFFF1;				//send to error
					}
				T_ID = i*1024+(2*(WFORMDYNAMIC[crsr]+256*WFORMDYNAMIC[crsr+1]-1));
				WFORMDYNAMIC[8192+T_ID] = WFORMDYNAMIC[crsr+4];			
				WFORMDYNAMIC[8193+T_ID] = WFORMDYNAMIC[crsr+5];	
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
			if((WFORMDYNAMIC[8192+T_ID] + 256*WFORMDYNAMIC[8193+T_ID])==0xFFFF)
				{
				cycle_en = 0;
				}
			else
				{
				TRACKS_DATABASE[TRACKLIST_OFFSET[i]+BPM_CNT] = WFORMDYNAMIC[8192+T_ID] + 256*WFORMDYNAMIC[8193+T_ID]; 	
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
			
		res = f_read(&file, WFORMDYNAMIC, 4096, &nbytes);
		if (res != FR_OK)
			{
			return 0;	
			}
		crsr = 0;
		NEXT_PAGE_19 = WFORMDYNAMIC[12] + 256*WFORMDYNAMIC[13] + 65536*WFORMDYNAMIC[14] + 16777216*WFORMDYNAMIC[15];						
		if(WFORMDYNAMIC[8]==19 && WFORMDYNAMIC[9]==0 && WFORMDYNAMIC[10]==0 && WFORMDYNAMIC[11]==0 && (WFORMDYNAMIC[27]&0x40)==0 && WFORMDYNAMIC[36]==0 && WFORMDYNAMIC[37]==0 && WFORMDYNAMIC[24]!=0)		//check page type
			{	
			P_PAGE_0 = WFORMDYNAMIC[24];	
			crsr = 40;
			while(P_PAGE_0>0)	
				{
				if(WFORMDYNAMIC[crsr]==0x80 && WFORMDYNAMIC[crsr+1]==0x02)
					{
					BPM_CNT = crsr;	
					crsr+=12;
						
					T_ID = WFORMDYNAMIC[crsr]-1;			//date lenght sting
					T_ID = T_ID/2;
					T_ID-=1;						
					
					for(i=0;(i<T_ID && i<13);i++)
						{
						SD_DATE[i] = WFORMDYNAMIC[crsr+i+1];	
						}
					SD_DATE[i] = 0;		//end string	
					crsr = crsr + T_ID + 3;	
						
					T_ID = WFORMDYNAMIC[crsr]-1; 			//date lenght sting	
					T_ID = T_ID/2;
					T_ID-=1;		
					crsr = crsr + T_ID + 1;
					T_ID = WFORMDYNAMIC[crsr]-1; 			//date lenght sting	
					T_ID = T_ID/2;
					T_ID-=1;
							
					for(i=0;(i<T_ID && i<18);i++)	
						{
						SDCARD_NAME[i] = WFORMDYNAMIC[crsr+i+1];	
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
//	open ANLZXXXX.EXT file, extract dynamic waveform data, all_long data
//	open audio file
//
//	output: error code
////////////////////////////////////////////////////////////////////////	
uint16_t LOAD_TRACK(uint16_t TRACK_NUMBER)
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
		HAL_UART_Transmit(&huart4, U_TX_DATA, 2, 5);		
		HAL_UART_Transmit(&huart4, path_ANLZ, 45, 15);	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 2, 5);		
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
		res = f_read(&file, WFORMDYNAMIC, FILSIZE, &nbytes);
		if (res != FR_OK)
			{
			return 5;	//cannot read ANLZXXXX.DAT file		
			}
		f_close(&file);					//Close file ANLZXXXX.DAT		
		uint32_t fsz;	
		fsz = WFORMDYNAMIC[8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[9];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[11];
		if(fsz != FILSIZE)
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[4];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[5];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[6];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[7];
		uint32_t StPosHead = fsz;		
		if(WFORMDYNAMIC[StPosHead] != 80 ||
			 WFORMDYNAMIC[StPosHead+1] != 80 ||
			 WFORMDYNAMIC[StPosHead+2] != 84 || 
			 WFORMDYNAMIC[StPosHead+3] != 72)		//Check PPHT position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[StPosHead+4];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+5];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+6];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+7];								//head size
		uint32_t SPP = fsz+StPosHead+1;	
		fsz = WFORMDYNAMIC[StPosHead+12];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[StPosHead+13];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+14];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+15];							//Path Size
		char path_AUDIOTRACK[(fsz/2)+2];			//Create a Path for audiotrack
		while(E<(fsz+4))
			{
			path_AUDIOTRACK[(E/2)+2] = WFORMDYNAMIC[SPP+E];			//Fill path
			E=E+2;	
			}
		path_AUDIOTRACK[0] = 48;
		path_AUDIOTRACK[1] = 58;	
		fsz = WFORMDYNAMIC[StPosHead+8];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+9];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+10];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+11];	
		StPosHead = StPosHead+fsz;	//PVBR position
		if(WFORMDYNAMIC[StPosHead] != 80 ||
			 WFORMDYNAMIC[StPosHead+1] != 86 ||
			 WFORMDYNAMIC[StPosHead+2] != 66 || 
			 WFORMDYNAMIC[StPosHead+3] != 82)	//Check PVBR position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[StPosHead+9];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+11];		
		StPosHead = StPosHead+fsz;	//PQTZ position
		if(WFORMDYNAMIC[StPosHead] != 80 ||
			 WFORMDYNAMIC[StPosHead+1] != 81 ||
			 WFORMDYNAMIC[StPosHead+2] != 84 || 
			 WFORMDYNAMIC[StPosHead+3] != 90)	//Check PQTZ position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}	
		fsz = WFORMDYNAMIC[StPosHead+4];				
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[StPosHead+5];		
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+6];		
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+7];				//Head size
		uint32_t number_of_entries;
		number_of_entries = WFORMDYNAMIC[StPosHead+20];				
		number_of_entries<<=8;	
		number_of_entries+=WFORMDYNAMIC[StPosHead+21];		
		number_of_entries<<=8;
		number_of_entries+=WFORMDYNAMIC[StPosHead+22];		
		number_of_entries<<=8;
		number_of_entries+=WFORMDYNAMIC[StPosHead+23];				//calculate number_of_entries
		if(number_of_entries>4096)
			{
			number_of_entries = 4096;	
			}
		SPP = StPosHead + fsz + 2;						//start first BPM data.
		E = 0;
		GRID_OFFSET = WFORMDYNAMIC[SPP-1];							//find first beat 1...4
		while(E<number_of_entries)
			{
			BPMGRID[E] = WFORMDYNAMIC[SPP+(E*8)];	
			BPMGRID[E]<<=8;
			BPMGRID[E]+= WFORMDYNAMIC[SPP+1+(E*8)];		
			BEATGRID[E] = WFORMDYNAMIC[SPP+2+(E*8)];	
			BEATGRID[E]<<=8;
			BEATGRID[E]+= WFORMDYNAMIC[SPP+3+(E*8)];	
			BEATGRID[E]<<=8;
			BEATGRID[E]+= WFORMDYNAMIC[SPP+4+(E*8)];	
			BEATGRID[E]<<=8;
			BEATGRID[E]+= WFORMDYNAMIC[SPP+5+(E*8)];	
			BEATGRID[E] = (BEATGRID[E]*3)/20; 	//translate ms to 1/150s frames
			E++;	
			}
		if(E==4096)
			{
			BEATGRID[4095] = 0xFFFF;	
			}
		else
			{
			BEATGRID[E] = 0xFFFF;	
			BPMGRID[E] = BPMGRID[E-1];	
			}
		originalBPM = BPMGRID[0];			//SEND ORIGINAL BPM		
		fsz = WFORMDYNAMIC[StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[StPosHead+9];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+11];	
		StPosHead = StPosHead+fsz;	//PWAV position	
		if(WFORMDYNAMIC[StPosHead] != 80 ||
			 WFORMDYNAMIC[StPosHead+1] != 87 ||
			 WFORMDYNAMIC[StPosHead+2] != 65 || 
			 WFORMDYNAMIC[StPosHead+3] != 86)		//Check PWAV position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[StPosHead+4];			
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+5];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+6];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+7];				//PWAV Header size
		fsz+=StPosHead;			
		uint16_t ampl;
		uint32_t y;	

		for(E=0;E<203;E++)					//Fill Static Waveform 400->203
			{
			y = 506*E;			//506
			y>>=8;	
			ampl = (WFORMDYNAMIC[fsz+y]&0x1F)*192;	 //convert amplitude 24->18
			ampl>>=8;
			if(ampl>18)
				{
				ampl = 18;	
				}
			WFORMSTATIC[E] = WFORMDYNAMIC[fsz+y]&0x80;	//color	
			WFORMSTATIC[E]|= ampl;	
			}

		fsz = WFORMDYNAMIC[StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[StPosHead+9];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+11];				//PWAV TAG size
		StPosHead+=fsz;													//PWV2 start adress
		if(WFORMDYNAMIC[StPosHead] != 80 ||
			 WFORMDYNAMIC[StPosHead+1] != 87 ||
			 WFORMDYNAMIC[StPosHead+2] != 86 || 
			 WFORMDYNAMIC[StPosHead+3] != 50)		//Check PWV2 position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}
		fsz = WFORMDYNAMIC[StPosHead+8];			
		fsz<<=8;	
		fsz+=WFORMDYNAMIC[StPosHead+9];
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+10];	
		fsz<<=8;
		fsz+=WFORMDYNAMIC[StPosHead+11];				//PWV2 Tag size
		StPosHead = StPosHead+fsz;	//PCOB position				
		if(WFORMDYNAMIC[StPosHead] != 80 ||
			 WFORMDYNAMIC[StPosHead+1] != 67 ||
			 WFORMDYNAMIC[StPosHead+2] != 79 || 
			 WFORMDYNAMIC[StPosHead+3] != 66)		//Check PCOB position in file
			{
			return 6;   //file ANLZXXXX.DAT is damadge!		
			}		
		DrawMemoryMarker(0xFF, NONE_MARK, 0x8000);
		uint32_t PCOB2_adr;
			
		//////////////////prepare HOT CUES	
		for(E=0;E<8;E++)									//Clear ícue massive
			{
			HCUE_type[E] = 0xFF;	 							//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 		
			HCUE_adr[0][E] = 0xFFFF;		
			HCUE_adr[1][E] = 0xFFFF;	
			HCUE32COLOR[E] = 0xFFFF;	
			}
		
		if(WFORMDYNAMIC[StPosHead+15]==1)    //check type PCOB - for HOT CUE points
			{
			number_of_hot_cue_points = WFORMDYNAMIC[StPosHead+19]&0xF;				//number of hotcue points		
//			if(number_of_hot_cue_points>0)
//				{
//				MemoryCuePyramid_ENABLE = 3;	
//				}
			fsz = WFORMDYNAMIC[StPosHead+8];			
			fsz<<=8;	
			fsz+=WFORMDYNAMIC[StPosHead+9];
			fsz<<=8;
			fsz+=WFORMDYNAMIC[StPosHead+10];	
			fsz<<=8;
			fsz+=WFORMDYNAMIC[StPosHead+11];				//PCOB TAG size
			PCOB2_adr = fsz+StPosHead;							//start adress PCOB2
			fsz = WFORMDYNAMIC[StPosHead+4];			
			fsz<<=8;	
			fsz+=WFORMDYNAMIC[StPosHead+5];
			fsz<<=8;
			fsz+=WFORMDYNAMIC[StPosHead+6];	
			fsz<<=8;
			fsz+=WFORMDYNAMIC[StPosHead+7];				//PCOB head size
			StPosHead+= fsz;		
			uint8_t HCUE_NAME = 0;
			E = 0;	
			while(E<number_of_hot_cue_points)	
				{	
				fsz = WFORMDYNAMIC[StPosHead+8];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[StPosHead+9];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[StPosHead+10];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[StPosHead+11];				//PCPT TAG size
				SPP = StPosHead+fsz;										//Next PCPT adress

				HCUE_NAME = WFORMDYNAMIC[StPosHead+15];
				if(WFORMDYNAMIC[StPosHead+19]!=0)				//when hcue active
					{
					if((HCUE_NAME<4) && (HCUE_NAME>0))
						{
						HCUE_type[HCUE_NAME-1] = 0x02;			//write 
						}
					}
				fsz = WFORMDYNAMIC[StPosHead+4];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[StPosHead+5];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[StPosHead+6];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[StPosHead+7];				//PCPT Head size
				StPosHead+= fsz;		
		
				if((HCUE_NAME<4) && (HCUE_NAME>0))
					{
					HCUE_NAME--;
					HCUE_adr[0][HCUE_NAME] = WFORMDYNAMIC[StPosHead+4];
					HCUE_adr[0][HCUE_NAME] = HCUE_adr[0][HCUE_NAME]<<8;
					HCUE_adr[0][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+5];
					HCUE_adr[0][HCUE_NAME] = HCUE_adr[0][HCUE_NAME]<<8;
					HCUE_adr[0][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+6];
					HCUE_adr[0][HCUE_NAME] = HCUE_adr[0][HCUE_NAME]<<8;	
					HCUE_adr[0][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+7];
					if(WFORMDYNAMIC[StPosHead]==2)						//when hot cue type=loop
						{
						HCUE_type[HCUE_NAME]&= 0x03;	
						HCUE_type[HCUE_NAME] |= 0x01;		
						HCUE_adr[1][HCUE_NAME] = WFORMDYNAMIC[StPosHead+8];
						HCUE_adr[1][HCUE_NAME] = HCUE_adr[1][HCUE_NAME]<<8;
						HCUE_adr[1][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+9];
						HCUE_adr[1][HCUE_NAME] = HCUE_adr[1][HCUE_NAME]<<8;
						HCUE_adr[1][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+10];
						HCUE_adr[1][HCUE_NAME] = HCUE_adr[1][HCUE_NAME]<<8;	
						HCUE_adr[1][HCUE_NAME]+= WFORMDYNAMIC[StPosHead+11];
						HCUE_adr[1][HCUE_NAME] = (HCUE_adr[1][HCUE_NAME]*3)/20; 	//translate ms to 1/150s frames
						}
					}
				StPosHead = SPP;	
				E++;	
				}
			}
						
		//////////////////prepare MEMORY CUES
		for(E=0;E<10;E++)									//Clear memory cue massive
			{
			MEMORY_type[E] = 0;						//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 		
			MEMORY_adr[0][E] = 0xFFFF;
			MEMORY_adr[1][E] = 0xFFFF;	
			}
		StPosHead = PCOB2_adr;	
		if(WFORMDYNAMIC[StPosHead+15]==0)    //check type PCOB - for MEMORY CUE points
			{
			number_of_memory_cue_points = WFORMDYNAMIC[StPosHead+19]&0xF;				//number of MEMORY points		
//			if(number_of_memory_cue_points>0)
//				{
//				MemoryCuePyramid_ENABLE = 3;	
//				}
			fsz = WFORMDYNAMIC[StPosHead+4];			
			fsz<<=8;	
			fsz+=WFORMDYNAMIC[StPosHead+5];
			fsz<<=8;
			fsz+=WFORMDYNAMIC[StPosHead+6];	
			fsz<<=8;
			fsz+=WFORMDYNAMIC[StPosHead+7];				//PCOB head size
			StPosHead = StPosHead+fsz;		

			E = 0;	
			while(E<number_of_memory_cue_points)	
				{	
				fsz = WFORMDYNAMIC[StPosHead+8];			
				fsz<<=8;
				fsz+=WFORMDYNAMIC[StPosHead+9];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[StPosHead+10];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[StPosHead+11];				//PCPT TAG size
				SPP = StPosHead+fsz;										//Next PCPT adress

				if(WFORMDYNAMIC[StPosHead+19]!=0)				//when hcue active
					{
					MEMORY_type[E] = 2;			//write 
					}	
				fsz = WFORMDYNAMIC[StPosHead+4];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[StPosHead+5];
				fsz<<=8;
				fsz+=WFORMDYNAMIC[StPosHead+6];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[StPosHead+7];				//PCPT Head size
				StPosHead = StPosHead+fsz;		
		
				MEMORY_adr[0][E] = WFORMDYNAMIC[StPosHead+4];
				MEMORY_adr[0][E] = MEMORY_adr[0][E]<<8;
				MEMORY_adr[0][E]+= WFORMDYNAMIC[StPosHead+5];
				MEMORY_adr[0][E] = MEMORY_adr[0][E]<<8;
				MEMORY_adr[0][E]+= WFORMDYNAMIC[StPosHead+6];
				MEMORY_adr[0][E] = MEMORY_adr[0][E]<<8;	
				MEMORY_adr[0][E]+= WFORMDYNAMIC[StPosHead+7];	
				if(WFORMDYNAMIC[StPosHead]==2)						//when hot cue type=loop
					{
					MEMORY_type[E] |= 0x1;		
					MEMORY_adr[1][E] = WFORMDYNAMIC[StPosHead+8];
					MEMORY_adr[1][E] = MEMORY_adr[1][E]<<8;
					MEMORY_adr[1][E]+= WFORMDYNAMIC[StPosHead+9];
					MEMORY_adr[1][E] = MEMORY_adr[1][E]<<8;
					MEMORY_adr[1][E]+= WFORMDYNAMIC[StPosHead+10];
					MEMORY_adr[1][E] = MEMORY_adr[1][E]<<8;	
					MEMORY_adr[1][E]+= WFORMDYNAMIC[StPosHead+11];	
					MEMORY_adr[1][E] = (MEMORY_adr[1][E]*3)/20; 	//translate ms to 1/150s frames
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
		HAL_UART_Transmit(&huart4, U_TX_DATA, 2, 5);		
		HAL_UART_Transmit(&huart4, path_ANLZ, 45, 15);	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 2, 5);		
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
			res = f_read(&file, WFORMDYNAMIC, fsz, &nbytes);
			if(res != FR_OK)
				{
				ERROR = 8;	//ANLZXXXX.EXT file is damadge
				f_close(&file);					//Close file ANLZXXXX.EXT							
				}
			else
				{
				f_close(&file);					//Close file ANLZXXXX.EXT					
				fsz = WFORMDYNAMIC[8];			
				fsz<<=8;	
				fsz+=WFORMDYNAMIC[9];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[10];	
				fsz<<=8;
				fsz+=WFORMDYNAMIC[11];	//file size		
				if(fsz != FILSIZE)
					{
					ERROR = 9;	//ANLZXXXX.EXT file is damadge
					}
				else		
					{	
					fsz = WFORMDYNAMIC[4];			
					fsz<<=8;	
					fsz+=WFORMDYNAMIC[5];	
					fsz<<=8;
					fsz+=WFORMDYNAMIC[6];	
					fsz<<=8;
					fsz+=WFORMDYNAMIC[7];	//Header size
					StPosHead = fsz;	
					if(WFORMDYNAMIC[StPosHead] != 80 | 
						 WFORMDYNAMIC[StPosHead+1] != 80 | 
					   WFORMDYNAMIC[StPosHead+2] != 84 | 
					   WFORMDYNAMIC[StPosHead+3] != 72)		//Check PPTH position in file
						{
						ERROR = 10;	//ANLZXXXX.EXT file is damadge				
						}
					else
						{		
						fsz = WFORMDYNAMIC[StPosHead+8];			
						fsz<<=8;	
						fsz+=WFORMDYNAMIC[StPosHead+9];	
						fsz<<=8;
						fsz+=WFORMDYNAMIC[StPosHead+10];	
						fsz<<=8;
						fsz+=WFORMDYNAMIC[StPosHead+11];	//Tag size
						StPosHead += fsz;		
						if(WFORMDYNAMIC[StPosHead] != 80 | 
							 WFORMDYNAMIC[StPosHead+1] != 87 | 
						   WFORMDYNAMIC[StPosHead+2] != 86 | 
						   WFORMDYNAMIC[StPosHead+3] != 51)		//Check PWV3 position in file
							{
							return 11;	//ANLZXXXX.EXT file is damadge			
							}
						else
							{	
							fsz = WFORMDYNAMIC[StPosHead+4];			
							fsz<<=8;	
							fsz+=WFORMDYNAMIC[StPosHead+5];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[StPosHead+6];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[StPosHead+7];	//Header size
							uint32_t START_POS_WF = StPosHead + fsz; //Start position waveform	 //old SPP
									
							fsz = WFORMDYNAMIC[StPosHead+16];			
							fsz<<=8;	
							fsz+=WFORMDYNAMIC[StPosHead+17];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[StPosHead+18];	
							fsz<<=8;
							uint32_t WF_DATA_SIZE = fsz+WFORMDYNAMIC[StPosHead+19];		//waveform data size	//old fsz
							uint32_t j;							//Data shift	
								
							fsz = WFORMDYNAMIC[StPosHead+8];			
							fsz<<=8;	
							fsz+=WFORMDYNAMIC[StPosHead+9];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[StPosHead+10];	
							fsz<<=8;
							fsz+=WFORMDYNAMIC[StPosHead+11];	//Tag size
							StPosHead += fsz;		
							if(WFORMDYNAMIC[StPosHead] != 80 | 
								 WFORMDYNAMIC[StPosHead+1] != 67 | 
								 WFORMDYNAMIC[StPosHead+2] != 79 | 
								 WFORMDYNAMIC[StPosHead+3] != 66)		//Check PCOB position in file
								{
								return 11;	//ANLZXXXX.EXT file is damadge			
								}
							else
								{
								fsz = WFORMDYNAMIC[StPosHead+8];			
								fsz<<=8;	
								fsz+=WFORMDYNAMIC[StPosHead+9];	
								fsz<<=8;
								fsz+=WFORMDYNAMIC[StPosHead+10];	
								fsz<<=8;
								fsz+=WFORMDYNAMIC[StPosHead+11];	//Tag size
								StPosHead += fsz;		
								if(WFORMDYNAMIC[StPosHead] != 80 | 
								 WFORMDYNAMIC[StPosHead+1] != 67 | 
								 WFORMDYNAMIC[StPosHead+2] != 79 | 
								 WFORMDYNAMIC[StPosHead+3] != 66)		//Check PCO2 position in file
									{
									return 11;	//ANLZXXXX.EXT file is damadge			
									}	
								else
									{
									fsz = WFORMDYNAMIC[StPosHead+8];			
									fsz<<=8;	
									fsz+=WFORMDYNAMIC[StPosHead+9];	
									fsz<<=8;
									fsz+=WFORMDYNAMIC[StPosHead+10];	
									fsz<<=8;
									fsz+=WFORMDYNAMIC[StPosHead+11];	//Tag size
									StPosHead += fsz;		
									if(WFORMDYNAMIC[StPosHead] != 80 | 
									 WFORMDYNAMIC[StPosHead+1] != 67 | 
									 WFORMDYNAMIC[StPosHead+2] != 79 | 
									 WFORMDYNAMIC[StPosHead+3] != 50)		//Check PCO2 position in file
										{
										return 11;	//ANLZXXXX.EXT file is damadge			
										}	
									else
										{
										#if defined(DEBUG_UART_EN)		
										sprintf((char*)U_TX_DATA, "Enter PCO2\n\r");	
										HAL_UART_Transmit(&huart4, U_TX_DATA, 12, 5);	
										#endif		
											
											
										if(WFORMDYNAMIC[StPosHead+15]==1)				//HOT CUE TYPE
											{
											number_of_hot_cue_points = WFORMDYNAMIC[StPosHead+17];
											fsz = WFORMDYNAMIC[StPosHead+4];			
											fsz<<=8;	
											fsz+=WFORMDYNAMIC[StPosHead+5];	
											fsz<<=8;
											fsz+=WFORMDYNAMIC[StPosHead+6];	
											fsz<<=8;
											fsz+=WFORMDYNAMIC[StPosHead+7];	//header size
											StPosHead += fsz;	
											
											for(j=0;j<number_of_hot_cue_points;j++)			
												{	
												if(WFORMDYNAMIC[StPosHead] != 80 | 
													 WFORMDYNAMIC[StPosHead+1] != 67 | 
													 WFORMDYNAMIC[StPosHead+2] != 80 | 
													 WFORMDYNAMIC[StPosHead+3] != 50)		//Check PCP2 position in file
														{
														j = 100;	
														return 11;	//ANLZXXXX.EXT file is damadge			
														}
												else
													{
													#if defined(DEBUG_UART_EN)		
													sprintf((char*)U_TX_DATA, "Extract HOT CUE\n\r");	
													HAL_UART_Transmit(&huart4, U_TX_DATA, 17, 5);	
													#endif		
													if(WFORMDYNAMIC[StPosHead+15]>0)
														{
														HCUE_adr[0][WFORMDYNAMIC[StPosHead+15]-1] = WFORMDYNAMIC[StPosHead+20]; 		
														HCUE_adr[0][WFORMDYNAMIC[StPosHead+15]-1]<<=8;	
														HCUE_adr[0][WFORMDYNAMIC[StPosHead+15]-1]+=WFORMDYNAMIC[StPosHead+21];
														HCUE_adr[0][WFORMDYNAMIC[StPosHead+15]-1]<<=8;	
														HCUE_adr[0][WFORMDYNAMIC[StPosHead+15]-1]+=WFORMDYNAMIC[StPosHead+22];	
														HCUE_adr[0][WFORMDYNAMIC[StPosHead+15]-1]<<=8;	
														HCUE_adr[0][WFORMDYNAMIC[StPosHead+15]-1]+=WFORMDYNAMIC[StPosHead+23];
														if(WFORMDYNAMIC[StPosHead+16]==2)						//when hot cue type=loop
															{
															HCUE_adr[1][WFORMDYNAMIC[StPosHead+15]-1] = WFORMDYNAMIC[StPosHead+24]; 		
															HCUE_adr[1][WFORMDYNAMIC[StPosHead+15]-1]<<=8;	
															HCUE_adr[1][WFORMDYNAMIC[StPosHead+15]-1]+=WFORMDYNAMIC[StPosHead+25];
															HCUE_adr[1][WFORMDYNAMIC[StPosHead+15]-1]<<=8;	
															HCUE_adr[1][WFORMDYNAMIC[StPosHead+15]-1]+=WFORMDYNAMIC[StPosHead+26];	
															HCUE_adr[1][WFORMDYNAMIC[StPosHead+15]-1]<<=8;	
															HCUE_adr[1][WFORMDYNAMIC[StPosHead+15]-1]+=WFORMDYNAMIC[StPosHead+27];	
															}
														E=WFORMDYNAMIC[StPosHead+42];	
														E<<=8;
														E+=WFORMDYNAMIC[StPosHead+43];	//len_comment	
														HCUE32COLOR[WFORMDYNAMIC[StPosHead+15]-1] = WFORMDYNAMIC[StPosHead+45+E];																
														HCUE32COLOR[WFORMDYNAMIC[StPosHead+15]-1]<<=8;	
														HCUE32COLOR[WFORMDYNAMIC[StPosHead+15]-1]+= WFORMDYNAMIC[StPosHead+46+E];		
														HCUE32COLOR[WFORMDYNAMIC[StPosHead+15]-1]<<=8;		
														HCUE32COLOR[WFORMDYNAMIC[StPosHead+15]-1]+= WFORMDYNAMIC[StPosHead+47+E];	
														}														
													fsz = WFORMDYNAMIC[StPosHead+8];			
													fsz<<=8;	
													fsz+=WFORMDYNAMIC[StPosHead+9];	
													fsz<<=8;
													fsz+=WFORMDYNAMIC[StPosHead+10];	
													fsz<<=8;
													fsz+=WFORMDYNAMIC[StPosHead+11];	//len_entry
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
										all_long = WF_DATA_SIZE;
//										if(all_long>57009)		//crop all long data for sldz
//											{
//											all_long = 57009;	
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
											//WFORMDYNAMIC[j] = WFORMDYNAMIC[j+SPP];	
											//optimization with color coefficient:
											a = WFORMDYNAMIC[j+START_POS_WF]&0x1F;
											a*= 153;
											a>>=8;											
											WFORMDYNAMIC[j] = WFORMDYNAMIC[j+START_POS_WF]&0xE0;
											WFORMDYNAMIC[j]|=a; 	
											}
											
//										for(j=0;j<(WF_DATA_SIZE>>1);j++)			//Fill dynamic waveform x2
//											{
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[wfmx2+(j/2)] = (WFORMDYNAMIC[j]&0xF0);	
//												}
//											else
//												{
//												WFORMDYNAMIC[wfmx2+(j/2)]+= WFORMDYNAMIC[j]>>4;	
//												}											
//											}	
//										for(j=0;j<(WF_DATA_SIZE>>2);j++)			//Fill dynamic waveform x4
//											{
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[wfmx4+(j/2)] = (WFORMDYNAMIC[2*j]&0xF0);
//												}
//											else
//												{
//												WFORMDYNAMIC[wfmx4+(j/2)]+= WFORMDYNAMIC[2*j]>>4;		
//												}											
//											}		
//										for(j=0;j<(WF_DATA_SIZE>>3);j++)			//Fill dynamic waveform x8
//											{
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[wfmx8+(j/2)] = (WFORMDYNAMIC[4*j]&0xF0);	
//												}
//											else
//												{
//												WFORMDYNAMIC[wfmx8+(j/2)]+= WFORMDYNAMIC[4*j]>>4;	
//												}											
//											}
//										for(j=0;j<(WF_DATA_SIZE>>4);j++)			//Fill dynamic waveform x16
//											{
//											wpos = 8;	
//											for(E=0;E<8;E++)			
//												{		
//												wpos+=WFORMDYNAMIC[8*j+E]&0x0F;
//												wpos+=WFORMDYNAMIC[8*j+E]>>4;	
//												}
//											wpos>>=4;	
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[wfmx16+(j/2)] = (wpos<<4);	
//												}
//											else
//												{
//												WFORMDYNAMIC[wfmx16+(j/2)]+= wpos&0x0F;	
//												}													
//											}		
//										for(j=0;j<(WF_DATA_SIZE>>5);j++)			//Fill dynamic waveform x32
//											{
//											wpos = 16;	
//											for(E=0;E<16;E++)			
//												{		
//												wpos+=WFORMDYNAMIC[16*j+E]&0x0F;
//												wpos+=WFORMDYNAMIC[16*j+E]>>4;	
//												}
//											wpos>>=5;	
//											if(j%2==0)
//												{
//												WFORMDYNAMIC[wfmx32+(j/2)] = (wpos<<4);	
//												}
//											else
//												{
//												WFORMDYNAMIC[wfmx32+(j/2)]+= wpos&0x0F;	
//												}											
//											}			
											
										for(E=0;E<number_of_memory_cue_points;E++)											//Draw CUES on Display
											{
											if(MEMORY_adr[0][E] != 0xFFFF)
												{
												#if defined(DEBUG_UART_EN)		
												sprintf((char*)U_TX_DATA, "MEMORY in %06lu ms\n\r", MEMORY_adr[0][E]);											
												HAL_UART_Transmit(&huart4, U_TX_DATA, 21, 15);	
												#endif		
												mem_pos	= 609*MEMORY_adr[0][E];
												mem_pos/= (20*all_long);
												DrawMemoryMarker(mem_pos, MEMORY_MARK, LCD_COLOR_RED);
												MEMORY_adr[0][E] = (MEMORY_adr[0][E]*3)/20;				//translate ms to 1/150s frames
												}	
											}	

										////////////////////////////////////sorting algoritm	
										uint8_t MINM, ii;
															
										for(E=0;E<number_of_memory_cue_points-1;E++)	
											{
											MINM = E;
											for(ii=(E+1);ii<number_of_memory_cue_points;ii++)
												{
												if(MEMORY_adr[0][ii]<MEMORY_adr[0][MINM])
													{
													j = MEMORY_adr[0][MINM];
													MEMORY_adr[0][MINM] = MEMORY_adr[0][ii];
													MEMORY_adr[0][ii] = j;
													j = MEMORY_adr[1][MINM];
													MEMORY_adr[1][MINM] = MEMORY_adr[1][ii];
													MEMORY_adr[1][ii] = j;												
													j = MEMORY_type[MINM];
													MEMORY_type[MINM] = MEMORY_type[ii];
													MEMORY_type[ii] = j;	
													}
												}
											}

										uint8_t	r, g, b;	
										for(E=0;E<8;E++)											//Draw MEMORY on Display
											{
											if(HCUE_adr[0][E] != 0xFFFF)
												{		
												r = (HCUE32COLOR[E]&0x00FFFFFF)>>19;	
												g = (HCUE32COLOR[E]&0x0000FFFF)>>11;
												b = (HCUE32COLOR[E]&0x000000FF)>>3;	
												HCUE16COLOR[E] = 0x8000+(0x400*r)+(0x20*g)+b; 
												mem_pos	= 609*HCUE_adr[0][E];
												mem_pos/= (20*all_long);
												DrawMemoryMarker(mem_pos, 2+E, HCUE16COLOR[E]);				//square
												#if defined(DEBUG_UART_EN)		
												sprintf((char*)U_TX_DATA, "H.CUE in %06lu ms  ", HCUE_adr[0][E]);											
												HAL_UART_Transmit(&huart4, U_TX_DATA, 20, 15);												
												#endif		
													
												HCUE_adr[0][E] = (HCUE_adr[0][E]*3)/20;				//translate ms to 1/150s frames										
												if(E==0)							//HCUE_A
													{
													if(HCUE_type[E]&0x1)
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
													if(HCUE_type[E]&0x1)
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
													if(HCUE_type[E]&0x1)
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
												if(E==0)							//HCUE_A
													{
													///Tbuffer[17] &= 0x7F;
													///Tbuffer[18] &= 0x7E;	
													}
												else if(E==1)					//HCUE_B
													{
													///Tbuffer[18] &= 0x73;		 
													}	
												else if(E==2)					//HCUE_C
													{	
													///Tbuffer[18] &= 0x1F;		
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
			}

		res = f_open(&file, path_AUDIOTRACK, FA_READ);				//Open audio file
		if (res != FR_OK)
			{
			return 13;	//cannot open AUDIOTRACK
			}
		#if defined(DEBUG_UART_EN)	
		sprintf((char*)U_TX_DATA, "\n\r");	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 2, 5);				
		HAL_UART_Transmit(&huart4, path_AUDIOTRACK, 150, 45);	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 2, 5);	
		#endif		
		res = f_read(&file, PCM, 512, &nbytes);
		if(res != FR_OK)
			{
			ERROR = 14;	//cannot read AUDIOTRACK
 			}
		#if defined(DEBUG_UART_EN)	
		sprintf((char*)U_TX_DATA, "\n\r");	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 2, 5);				
		HAL_UART_Transmit(&huart4, (uint8_t *)PCM, 50, 45);	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 2, 5);	
		#endif		
			
		if(PCM[0][5][0] != 1 || PCM[0][5][1] != 2 || PCM[0][6][0] != 44100 || PCM[0][8][1] != 16)	//Check audio format
			{
			ERROR = 15;	//unsupported audio format	
			}
		f_lseek(&file, 44);
		if(ERROR==0)
			{
			playlist[TRACK_NUMBER-1][54] |= 0x01;				//write history mem
			}
		return ERROR;	
		}

		
///////////////////////////////////////////		
//preparing for loading next track
//
//
void PREPARE_LOAD_TRACK(uint16_t TRACK_NUMBER, uint16_t TRACK_IN_PLAYLIST)
	{	
	lock_control = 1;	
	uint16_t ERR = 0;	
	f_close(&file);	
	pitch = 0;	
	play_enable = 0;
	if(SLIPEN)
		{
		SLIPEN = 0;
		deckTbuf[1][0]|=0x10;		//led on	
		}		
	play_adr = 0;	
	all_long = 0;		
	end_adr_valid_data = 0xFFFF;
	start_adr_valid_data = 0;	
	slip_play_adr = 0;
	loop_active = 0;
	LOOP_OUT = 0;		
	ERR = LOAD_TRACK(TRACK_NUMBER);		
	if(ERR==0)								//the sequence of functions in this place is very important!
		{				
		track_play_now = TRACK_NUMBER;	
			
//		tempo_need_update = 1;		
		end_adr_valid_data = 0;
		//TIM3->CCR1 = ROUND_PWM_VALUE;					//round PWM		
//////	if(QUANTIZE)
//////		{
//////		ShowQUANTIZE(2);	
//////		}	
		//ShowTrackNumber(TRACK_IN_PLAYLIST);
		//REDRAW_TIMESCROLL(0);
		//DrawNewEmpty_TIMESCROLL();
			
//////		if(UTILITY_SETTINGS[2]==9)					//first beat
//////			{
			play_adr = BEATGRID[0]*294;	
			slip_play_adr = play_adr;
			CUE_ADR = BEATGRID[0];	
//////			}
//////		else if(UTILITY_SETTINGS[2]==8 && number_of_memory_cue_points>0)					//memory
//////			{	
//////			SET_MEMORY_CUE_1(MEMORY_adr[0][0]);
//////			CUE_OPERATION = MEMORY_NEED_SET_PART2;
//////			CUE_ADR =	MEMORY_adr[0][0];
//////			}	
//////		else
//////			{
//////			uint8_t have_a_cue = 0;
//////			uint16_t c = 0;
//////			uint16_t M = 0;	
//////			while(have_a_cue==0 && all_long>28*c)
//////				{
//////				f_read(&file, PCM[0][0], 32768, &nbytes);	
//////				for(M=0;M<8192;M++)
//////					{
//////					if(PCM[0][M][0]&0x8000)		//negative 65535...32768
//////						{
//////						PCM[0][M][0] = 0xFFFF - PCM[0][M][0];	
//////						}
//////					if(((PCM[0][M][0]>>(9-UTILITY_SETTINGS[2]))>0) && have_a_cue==0)
//////						{
//////						have_a_cue = 1;
//////						CUE_ADR = (c*8192+M)/294;
//////						play_adr = CUE_ADR*294;
//////						slip_play_adr = play_adr;
//////						M = 0xFFFF;	
//////						}
//////					if(PCM[0][M][1]&0x8000)		//negative 65535...32768
//////						{
//////						PCM[0][M][1] = 0xFFFF - PCM[0][M][1];	
//////						}
//////					if(((PCM[0][M][1]>>(9-UTILITY_SETTINGS[2]))>0) && have_a_cue==0)
//////						{
//////						have_a_cue = 1;
//////						CUE_ADR = (c*8192+M)/294;
//////						play_adr = CUE_ADR*294;
//////						slip_play_adr = play_adr;
//////						M = 0xFFFF;	
//////						}
//////					}	
//////				}
//////			f_lseek(&file, 44);						//return to start track	
//////				
//////			if(have_a_cue==0)
//////				{
//////				play_adr = 0;	
//////				slip_play_adr = 0;	
//////				CUE_ADR = 0;		
//////				}		
//////			}			
			
		DrawStaticWFM(DRAW_NEW_STATIC_WAVEFORM);				//Draw New waveform		
	
//////		if(UTILITY_SETTINGS[2]==9)					//first beat
//////			{
			RedrawWaveforms(BEATGRID[0]);
//////			}	
//////		else if(UTILITY_SETTINGS[2]==8 && number_of_memory_cue_points>0)					//memory
//////			{	
//////			RedrawWaveforms(MEMORY_adr[0][0]);	
//////			}			
//////		else		
//////			{
//////			RedrawWaveforms(CUE_ADR);	
//////			}
		
		///SwitchInformationLayer(WAVEFORM);	

//////		if(UTILITY_SETTINGS[2]==9)					//first beat
//////			{	
			SET_CUE(BEATGRID[0]);						//AUTO CUE set
//////			}
//////		else if(UTILITY_SETTINGS[2]==8 && number_of_memory_cue_points>0)					//memory
//////			{	
//////			SET_CUE(MEMORY_adr[0][0]);
//////			}			
//////		else
//////			{
//////			SET_CUE(CUE_ADR);						//AUTO CUE set	
//////			}	
//		previous_wfmpos = 0xFFFFFFFF;	
		DrawMinuteMarkers();	


		}
	else
		{
		track_play_now = 0;	
			
		f_close(&file);		
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Track not load. Error %03lu\n\r", ERR);	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 27, 5);	
		#endif	
		all_long = 0;		
		originalBPM = 0xFFFF;	
		//ShowBPM(originalBPM);
//////		if(QUANTIZE)
//////			{
//////			ShowQUANTIZE(1);	
//////			}	
//////		else
//////			{
//////			ShowQUANTIZE(0);		
//////			}
		//ShowTrackNumber(0);
		///MemoryCuePyramid_ENABLE = 0;	
		///SwitchInformationLayer(WAVEFORM);	
		DrawStaticWFM(CLEAR_WAVEFORM_ARRAY);	
		DrawStaticWFM(MS_ERROR+ERR);	
		RED_CIRCLE_CUE_ADR = 85;	
//		OLED_DrawLine(0, 63, 127, 63, OLED_BLACK);
//		need_update_oled1|=0x80;		//(7)
		}
	if(ERR==0)								//the sequence of functions in this place is very important!
		{		
		lock_control = 0;
		}
	return;	
	}	





		
														
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
