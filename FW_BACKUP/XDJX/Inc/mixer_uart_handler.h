/////////////////////////////////////////////////////////////////////////////////////////////////
// 	Mixer uart handler 
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

	if(mxr_new_data)									////////////////////////usart data descriptor-handler
		{
		if((mxr_urx_buf[0]==0x97) && (UT_SET[9]==1))
			{
			if(prev_inair[dkA]!=mxr_urx_buf[1])
				{
				prev_inair[dkA] = mxr_urx_buf[1];	
				if(mxr_urx_buf[1]==0)		//inactive
					{
					if(UT_SET[11]<9)
						{
						jog_work_color[dkA] = jog_color[UT_SET[12]][UT_SET[11]];	
						}					
					else
						{
						jog_work_color[dkA] = jog_color[UT_SET[12]][curr_trck_color[dkA]];		
						}
					//change masterdeck	
					if(play_enable[dkB] && masterdeck!=dkB)
						{
						masterdeck = dkB;
						tempo_need_update[dkA] = 2;
						tempo_need_update[dkB] = 2;	
						}
					}
				else		//in air
					{
					if(UT_SET[10]<9)
						{
						jog_work_color[dkA] = jog_color[UT_SET[12]][UT_SET[10]];	
						}					
					else
						{
						jog_work_color[dkA] = jog_color[UT_SET[12]][curr_trck_color[dkA]];		
						}		
					}				
				SET_JOG_COLOR(dkA, jog_work_color[dkA]);
				}				
				
			if(prev_inair[dkB]!=mxr_urx_buf[2])
				{
				prev_inair[dkB] = mxr_urx_buf[2];	
				if(mxr_urx_buf[2]==0)		//inactive
					{
					if(UT_SET[11]<9)
						{
						jog_work_color[dkB] = jog_color[UT_SET[12]][UT_SET[11]];	
						}					
					else
						{
						jog_work_color[dkB] = jog_color[UT_SET[12]][curr_trck_color[dkB]];		
						}
					//change masterdeck	
					if(play_enable[dkA] && masterdeck!=dkA)
						{
						masterdeck = dkA;
						tempo_need_update[dkA] = 2;
						tempo_need_update[dkB] = 2;	
						}	
					}
				else		//in air
					{
					if(UT_SET[10]<9)
						{
						jog_work_color[dkB] = jog_color[UT_SET[12]][UT_SET[10]];	
						}					
					else
						{
						jog_work_color[dkB] = jog_color[UT_SET[12]][curr_trck_color[dkB]];		
						}		
					}				
				SET_JOG_COLOR(dkB, jog_work_color[dkB]);
				}				
			}
		mxr_new_data = 0;		
		};

	
		
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



