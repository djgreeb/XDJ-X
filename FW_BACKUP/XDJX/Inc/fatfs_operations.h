/////////////////////////////////////////////////////////////////////////////////////////////////
//  fatfs_operations
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////



if(dkN==0)
	{	
	dkN = 1;
	}
else
	{
	dkN = 0;		
	}	

if(end_adata[dkN]<128)
	{
	res = f_read(&file[dkN], PCM[dkN][end_adata[dkN]][0], 32768, &nbytes[dkN]);
	if(res==FR_OK)
		{				
		need_redraw_memline[dkN] = 1;	
		end_adata[dkN]++;
		}					
	}
else if((end_adata[dkN]<((play_adr[dkN]>>13)+42)) && (fill_stp[dkN]==0 || fill_stp[dkN]==6) && (end_adata[dkN]<=(((all_long[dkN]*294)>>13)+1)))									//filling >>
	{
	if(fill_stp[dkN]==6)		//after <<
		{
		res = f_lseek(&file[dkN], ((32768*end_adata[dkN])+44));
		if(res==FR_OK)
			{
			fill_stp[dkN] = 0;
			}						
		}		
	res = f_read(&file[dkN], PCM[dkN][end_adata[dkN]&0x7F][0], 32768, &nbytes[dkN]);	
	if(res==FR_OK)
		{				
		end_adata[dkN]++;
		if((end_adata[dkN]-start_adata[dkN])>128)
			{
			start_adata[dkN] = end_adata[dkN]-128;	
			}
		need_redraw_memline[dkN] = 1;	
		}
	}
else if(((end_adata[dkN]>((play_adr[dkN]>>13)+86) || ((end_adata[dkN]-start_adata[dkN])<124)) && start_adata[dkN]>3) || (fill_stp[dkN]!=0 && fill_stp[dkN]!=6))					//filling <<
	{
	if(fill_stp[dkN]==0 || fill_stp[dkN]==6)
		{
		if((end_adata[dkN]-start_adata[dkN])>127)	
			{
			end_adata[dkN] = start_adata[dkN]+124;	
			}	
		start_adata[dkN]-=4;	
		res = f_lseek(&file[dkN], ((32768*(start_adata[dkN]))+44));
		if(res==FR_OK)
			{
			fill_stp[dkN] = 1;
			}						
		}
	else if(fill_stp[dkN]==1)
		{
		res = f_read(&file[dkN], PCM[dkN][start_adata[dkN]&0x7F][0], 32768, &nbytes[dkN]);
		if(res==FR_OK)
			{	
			fill_stp[dkN] = 2;
			}						
		}
	else if(fill_stp[dkN]==2)
		{
		res = f_read(&file[dkN], PCM[dkN][(start_adata[dkN]+1)&0x7F][0], 32768, &nbytes[dkN]);
		if(res==FR_OK)
			{	
			fill_stp[dkN] = 3;
			}						
		}
	else if(fill_stp[dkN]==3)
		{
		res = f_read(&file[dkN], PCM[dkN][(start_adata[dkN]+2)&0x7F][0], 32768, &nbytes[dkN]);
		if(res==FR_OK)
			{
			fill_stp[dkN] = 4;
			}						
		}
	else if(fill_stp[dkN]==4)
		{
		res = f_read(&file[dkN], PCM[dkN][(start_adata[dkN]+3)&0x7F][0], 32768, &nbytes[dkN]);
		if(res==FR_OK)
			{	
			fill_stp[dkN] = 5;
			}						
		}
	else if(fill_stp[dkN]==5)
		{
		need_redraw_memline[dkN] = 1;		
		fill_stp[dkN] = 6;		
		}
	}		
	
				
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



