/////////////////////////////////////////////////////////////////////////////////////////////////
// 	Screenshot creating on sd card
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t CREATE_SCREEN(void);




///////////////////////////////////////////////////////////////	
//	univrsal function for creating current screenshots		
//	
uint16_t CREATE_SCREEN(void)
	{
	////////////////////////////////////////////////////	
	//	UNIVERSAL CODE FOR SCREENSHOT
	//	set in file sdmmc.c
	//	hsd1.Init.ClockDiv = 12;					//1
	//	
	char path_rec[]="0:/SCREEN_001.bmp";
		
	static const uint8_t bmp_header[70] = {
	0x42, 0x4D, 0x08, 0x6A, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x38, 0x00, 
	0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0xE9, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x03, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0xE0, 0x03, 0x00, 0x00, 0x1F, 0x00, 
	0x00, 0x00, 0x00, 0x80, 0x00, 0x00};

	uint32_t RND = HAL_GetTick();
	path_rec[10] = (RND%1000)/100 + 48;
	path_rec[11] = (RND%100)/10 + 48;
	path_rec[12] = RND%10 + 48; 				
		
	res = f_open(&file, path_rec, FA_CREATE_NEW | FA_WRITE);
			
	if(res==FR_OK)
		{
		f_write(&file, bmp_header, 70, &nbytes);	//write header
		HAL_Delay(10);
		uint16_t F;
		uint16_t IMG_BFR[32];	
		uint16_t px;	
		uint32_t dpxh1, dpxh0;
		uint8_t q;	
		for(F=0;F<3495;F++)
			{
			for(q=0;q<16;q++)
				{
				dpxh1 = *((uint32_t*)LCD_FRAME_BUFFER_2+F*16+q);					//high layer with transparents
				dpxh0 = *((uint32_t*)LCD_FRAME_BUFFER+F*16+q);				//low (background) layer		
				px = dpxh1>>16;	
				if((px>>15)==0)
					{
					IMG_BFR[2*q+1] = dpxh0>>16;			
					}						
				else
					{
					IMG_BFR[2*q+1] = px;
					}
				px = dpxh1&0x0000FFFF;	
				if((px>>15)==0)
					{
					IMG_BFR[2*q] = dpxh0&0x0000FFFF;		
					}						
				else
					{
					IMG_BFR[2*q] = px;
					}
				}
			res = f_write(&file, IMG_BFR, 64, &nbytes);	
			HAL_Delay(8);		
			}
		if(res==FR_OK)				//write end header	
			{
			f_write(&file, &bmp_header[68], 2, &nbytes);
			}			
		if(res==FR_OK)
			{
			res = f_close(&file);	
			if(res==FR_OK)
				{
				return RND%1000;		
				}
			else
				{
				return 0xFFFF;		
				}
			}
		else
			{
			f_close(&file);
			return 0xFFFF;
			}	
		}	
	else
		{	
		return 0xFFFF;	
		}		
	};


	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



