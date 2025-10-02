/////////////////////////////////////////////////////////////////////////////////////////////////
// 	timer handler
//	product: XDJ-X mixer assy
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdint.h"

uint8_t pot_convln(uint16_t dt);					//conversion without center detent
uint8_t pot_convcd(uint16_t dt);					//conversion with center detent
uint8_t LVLtoDB(uint16_t lvl);						//convert PCM level to DB scale
	
	
///////////////////////////////////////////////
//
//			TIMER 2400Hz
//				
//			load: 1-3us per cycle
//
void TIM2_IRQHandler(void)
	{
	//BT_LED_ON;
	if(tim_dv<15)			//0...14
		{
		if(tim_dv==0)		//////////////////////////////////////////			0			//////////////////////////////////////////
			{
			HAL_ADC_Start(&hadc1);
			HAL_ADC_Start(&hadc2);	
			if((GPIOE->IDR & 0x00000008) != 0x00U)			//Key in 0	(CFX)
				{
				if(CFX8_prsd==0)
					{
					if(CFXON==8)
						{
						MAX7219_DATA[6] = 0xFF;
						CFXON = 0;			
						}	
					else
						{	
						MAX7219_DATA[6] = CFXmask[CFXON-1];	
						CFXON = 8;	
						}
					MAX7219_need_update|=0x40;
					CFX8_prsd = 1;	
					}
				}
			else
				{
				if(CFX8_prsd==1)
					{
					CFX8_prsd = 0;	
					}	
				}					
			if((GPIOD->IDR & 0x00000008) != 0x00U)			//Key in 2 (BFX)
				{
				BFXn = 7;
				if(BFXn_previous!=BFXn)
					{
					need_draw_fxsel = 1;	
					BFXn_previous = BFXn;	
					}					
				}		
			if((GPIOB->IDR & 0x00000080) != 0x00U)			//Key in 3 (other keys)
				{
				if(CUE2_prsd==0)
					{
					if(CUE2ON==0)
						{
						MAX7219_DATA[7]|=	0x0C;
						CUE2ON = 1;	
						}						
					else
						{
						MAX7219_DATA[7]&=	0xF3;	
						CUE2ON = 0;	
						}
					MAX7219_need_update|=0x80;		
					CUE2_prsd = 1;	
					}
				}
			else
				{
				if(CUE2_prsd==1)
					{
					CUE2_prsd = 0;	
					}	
				}				
			MUX_ADDR0;	
			}			
		else if(tim_dv==1)		//////////////////////////////////////////			1			//////////////////////////////////////////
			{
			pot_SUM[tim_dv-1]-=pot_ADC[tim_dv-1][cnt_ad];	
			pot_ADC[tim_dv-1][cnt_ad] = HAL_ADC_GetValue(&hadc1);
			pot_SUM[tim_dv-1]+=pot_ADC[tim_dv-1][cnt_ad];		
			pot_SUM[tim_dv]-=pot_ADC[tim_dv][cnt_ad];		
			pot_ADC[tim_dv][cnt_ad] = HAL_ADC_GetValue(&hadc2);	
			pot_SUM[tim_dv]+=pot_ADC[tim_dv][cnt_ad];	
			ADC_TMP = (pot_SUM[tim_dv]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv]))	////////// HPHN
				{
				if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv]))	
					{
					pot_out[tim_dv] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv] = (pot_out[tim_dv]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv] = pot_convln(pot_out[tim_dv]);	
				HPHN_ATT = VR_LOG[pot_8b[tim_dv]];	
				}				
			ADC_TMP = (pot_SUM[tim_dv-1]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv-1]))	////////// PRMT	
				{
				if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv-1]))	
					{
					pot_out[tim_dv-1] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv-1] = (pot_out[tim_dv-1]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv-1] = pot_convcd(pot_out[tim_dv-1]);	
				}	
			}	
		else if(tim_dv==2)		//////////////////////////////////////////			2			//////////////////////////////////////////
			{	
			HAL_ADC_Start(&hadc1);
			HAL_ADC_Start(&hadc2);	
			if((GPIOE->IDR & 0x00000008) != 0x00U)			//Key in 0	(CFX)
				{
				if(CFX1_prsd==0)
					{
					if(CFXON==1)
						{
						MAX7219_DATA[6] = 0xFF;
						CFXON = 0;			
						}	
					else
						{	
						MAX7219_DATA[6] = CFXmask[CFXON-1];	
						CFXON = 1;	
						}
					MAX7219_need_update|=0x40;		
					CFX1_prsd = 1;	
					}
				}
			else
				{
				if(CFX1_prsd==1)
					{
					CFX1_prsd = 0;	
					}	
				}				
			if((GPIOA->IDR & 0x00000080) != 0x00U)			//Key in 1 (other keys)
				{
				CHSEL = 0;
				if(CHSEL_previous!=CHSEL)
					{
					need_draw_chsel = 1;	
					CHSEL_previous = CHSEL;	
					}	
				}		
			if((GPIOD->IDR & 0x00000008) != 0x00U)			//Key in 2 (BFX)
				{
				BFXn = 0;
				if(BFXn_previous!=BFXn)
					{
					need_draw_fxsel = 1;	
					BFXn_previous = BFXn;	
					}					
				}	
			MUX_ADDR1;	
			}			
		else if(tim_dv==3)		//////////////////////////////////////////			3			//////////////////////////////////////////
			{
			pot_SUM[tim_dv-1]-=pot_ADC[tim_dv-1][cnt_ad];	
			pot_ADC[tim_dv-1][cnt_ad] = HAL_ADC_GetValue(&hadc1);
			pot_SUM[tim_dv-1]+=pot_ADC[tim_dv-1][cnt_ad];		
			pot_SUM[tim_dv]-=pot_ADC[tim_dv][cnt_ad];		
			pot_ADC[tim_dv][cnt_ad] = HAL_ADC_GetValue(&hadc2);	
			pot_SUM[tim_dv]+=pot_ADC[tim_dv][cnt_ad];	
			ADC_TMP = (pot_SUM[tim_dv]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv]))	////////// HI0	
				{
				if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv]))	
					{
					pot_out[tim_dv] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv] = (pot_out[tim_dv]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv] = pot_convcd(pot_out[tim_dv]);	
				eql0.hg	= EQ_ISO[pot_8b[tim_dv]];	
				eqr0.hg	= EQ_ISO[pot_8b[tim_dv]];	
				}				
			ADC_TMP = (pot_SUM[tim_dv-1]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv-1]))	////////// HI1	
				{
				if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv-1]))	
					{
					pot_out[tim_dv-1] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv-1] = (pot_out[tim_dv-1]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv-1] = pot_convcd(pot_out[tim_dv-1]);	
				eql1.hg	= EQ_ISO[pot_8b[tim_dv-1]];	
				eqr1.hg	= EQ_ISO[pot_8b[tim_dv-1]];					
				}
			}	
		else if(tim_dv==4)		//////////////////////////////////////////			4			//////////////////////////////////////////
			{	
			HAL_ADC_Start(&hadc1);
			HAL_ADC_Start(&hadc2);				
			if((GPIOE->IDR & 0x00000008) != 0x00U)			//Key in 0	(CFX)
				{
				if(CFX2_prsd==0)
					{
					if(CFXON==2)
						{
						MAX7219_DATA[6] = 0xFF;
						CFXON = 0;			
						}	
					else
						{	
						MAX7219_DATA[6] = CFXmask[CFXON-1];	
						CFXON = 2;	
						}
					MAX7219_need_update|=0x40;		
					CFX2_prsd = 1;	
					}
				}
			else
				{
				if(CFX2_prsd==1)
					{
					CFX2_prsd = 0;	
					}	
				}				
			if((GPIOA->IDR & 0x00000080) != 0x00U)			//Key in 1 (other keys)
				{
				CHSEL = 1;
				need_draw_chsel = 1;	
				if(CHSEL_previous!=CHSEL)
					{
					need_draw_chsel = 1;	
					CHSEL_previous = CHSEL;	
					}	
				}		
			if((GPIOD->IDR & 0x00000008) != 0x00U)			//Key in 2 (BFX)
				{
				BFXn = 1;
				if(BFXn_previous!=BFXn)
					{
					need_draw_fxsel = 1;	
					BFXn_previous = BFXn;	
					}					
				}	
			if((GPIOB->IDR & 0x00000080) != 0x00U)			//Key in 3 (other keys)
				{
				if(BEATm_prsd==0)
					{
					if(B>0)
						{
						B--;	
						}						
					BEATm_prsd = 1;	
					}
				}
			else
				{
				if(BEATm_prsd==1)
					{
					BEATm_prsd = 0;	
					}	
				}		
			MUX_ADDR2;	
			}					
		else if(tim_dv==5)		//////////////////////////////////////////			5			//////////////////////////////////////////
			{
			pot_SUM[tim_dv-1]-=pot_ADC[tim_dv-1][cnt_ad];	
			pot_ADC[tim_dv-1][cnt_ad] = HAL_ADC_GetValue(&hadc1);
			pot_SUM[tim_dv-1]+=pot_ADC[tim_dv-1][cnt_ad];		
			pot_SUM[tim_dv]-=pot_ADC[tim_dv][cnt_ad];		
			pot_ADC[tim_dv][cnt_ad] = HAL_ADC_GetValue(&hadc2);	
			pot_SUM[tim_dv]+=pot_ADC[tim_dv][cnt_ad];	
			ADC_TMP = (pot_SUM[tim_dv]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv]))	////////// CFX0	
				{
				if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv]))	
					{
					pot_out[tim_dv] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv] = (pot_out[tim_dv]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv] = pot_convcd(pot_out[tim_dv]);	
				}				
			ADC_TMP = (pot_SUM[tim_dv-1]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv-1]))	////////// CFX1	
				{
				if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv-1]))	
					{
					pot_out[tim_dv-1] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv-1] = (pot_out[tim_dv-1]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv-1] = pot_convcd(pot_out[tim_dv-1]);	
				}	
			////////added calc code	 (leds blinking)
			if(tim_dv2<18)			//0...17
				{	
				if(tim_dv2==0)
					{
					if(tim_dv3==0)				//DIV 2
						{
						tim_dv3 = 1;	
						}						
					else
						{
						tim_dv3 = 0;
						if(blink_sync==0)			//DIV 2
							{
							blink_sync = 1;	
							}							
						else
							{
							blink_sync = 0;	
							}
						
						if(BFXON==1)
							{
							if(blink_sync==0)
								{
								MAX7219_DATA[7]|=0x10;
								}						
							else
								{
								MAX7219_DATA[7]&=0xEF;
								}				
							MAX7219_need_update|=0x80;	
							}
						if(CFXON!=0)
							{
							if(blink_sync==0)
								{
								MAX7219_DATA[6] = 0xFF;
								}						
							else
								{
								MAX7219_DATA[6] = CFXmask[CFXON-1];
								}				
							MAX7219_need_update|=0x40;
							}
						}		
					}					
				tim_dv2++;
				}
			else
				{
				if(BT_ACTIVATION_PRC>0)
					{
					if(BT_ACTIVATION_PRC%2==0)
						{
						BT_LED_ON;	
						}						
					else
						{
						BT_LED_OFF;	
						}						
					if(BT_ACTIVATION_PRC==1)
						{
						BT_ENABLE = 1;
						need_draw_bticon = 1;	
						BT_POW_ON;							
						}
					BT_ACTIVATION_PRC--;
					}
				tim_dv2 = 0;	
				}
			////////added calc code (level meter)	
			if(tim_dv2%2==0)		// 75hz refresh rate
				{		
				lvlc = LVLtoDB((uint16_t)lvl_ch0);		/////// CH0
				lvl_ch0 = 0;
				if(lvlc>=prev_lvl_ch0)
					{
					prev_lvl_ch0 = lvlc;
					tim_ch0 = 0;	
					}					
				else if(tim_ch0>dwnlvl)
					{
					prev_lvl_ch0-=1;
					tim_ch0 = 0;
					}
				if((prev_lvl_ch0>=prev_pk_ch0) || (timpk_ch0>dwnpk))
					{
					prev_pk_ch0 = prev_lvl_ch0;	
					timpk_ch0 = 0;	
					}									
				LEVEL_METER(0, prev_lvl_ch0, prev_pk_ch0);
				lvlc = LVLtoDB((uint16_t)lvl_ch1);		/////// CH1
				lvl_ch1 = 0;
				if(lvlc>=prev_lvl_ch1)
					{
					prev_lvl_ch1 = lvlc;
					tim_ch1 = 0;	
					}					
				else if(tim_ch1>dwnlvl)
					{
					prev_lvl_ch1-=1;
					tim_ch1 = 0;
					}
				if((prev_lvl_ch1>=prev_pk_ch1) || (timpk_ch1>dwnpk))
					{
					prev_pk_ch1 = prev_lvl_ch1;	
					timpk_ch1 = 0;	
					}		
				LEVEL_METER(1, prev_lvl_ch1, prev_pk_ch1);				
				lvlc = LVLtoDB((uint16_t)lvl_ml);		/////// MASTER L
				lvl_ml = 0;
				if(lvlc>=prev_lvl_ml)
					{
					prev_lvl_ml = lvlc;
					tim_ml = 0;	
					}					
				else if(tim_ml>dwnlvl)
					{
					prev_lvl_ml-=1;
					tim_ml = 0;
					}
				if((prev_lvl_ml>=prev_pk_ml) || (timpk_ml>dwnpk))
					{
					prev_pk_ml = prev_lvl_ml;	
					timpk_ml = 0;	
					}		
				LEVEL_METER(2, prev_lvl_ml, prev_pk_ml);			
				lvlc = LVLtoDB((uint16_t)lvl_mr);		/////// MASTER R
				lvl_mr = 0;
				if(lvlc>=prev_lvl_mr)
					{
					prev_lvl_mr = lvlc;
					tim_mr = 0;	
					}					
				else if(tim_mr>dwnlvl)
					{
					prev_lvl_mr-=1;
					tim_mr = 0;
					}
				if((prev_lvl_mr>=prev_pk_mr) || (timpk_mr>dwnpk))
					{
					prev_pk_mr = prev_lvl_mr;	
					timpk_mr = 0;	
					}					
				LEVEL_METER(3, prev_lvl_mr, prev_pk_mr);
				tim_ch0++;	
				tim_ch1++;	
				tim_ml++;	
				tim_mr++;	
				timpk_ch0++;	
				timpk_ch1++;	
				timpk_ml++;	
				timpk_mr++;
				}
			}	
		else if(tim_dv==6)		//////////////////////////////////////////			6			//////////////////////////////////////////
			{
			HAL_ADC_Start(&hadc1);
			HAL_ADC_Start(&hadc2);	
			if((GPIOE->IDR & 0x00000008) != 0x00U)			//Key in 0	(CFX)
				{
				if(CFX3_prsd==0)
					{
					if(CFXON==3)
						{
						MAX7219_DATA[6] = 0xFF;
						CFXON = 0;			
						}	
					else
						{	
						MAX7219_DATA[6] = CFXmask[CFXON-1];	
						CFXON = 3;	
						}
					MAX7219_need_update|=0x40;		
					CFX3_prsd = 1;	
					}
				}
			else
				{
				if(CFX3_prsd==1)
					{
					CFX3_prsd = 0;	
					}	
				}				
			if((GPIOA->IDR & 0x00000080) != 0x00U)			//Key in 1 (other keys)
				{
				CHSEL = 2;	
				if(CHSEL_previous!=CHSEL)
					{
					need_draw_chsel = 1;	
					CHSEL_previous = CHSEL;	
					}	
				}		
			if((GPIOD->IDR & 0x00000008) != 0x00U)			//Key in 2 (BFX)
				{
				BFXn = 2;
				if(BFXn_previous!=BFXn)
					{
					need_draw_fxsel = 1;	
					BFXn_previous = BFXn;	
					}					
				}		
			MUX_ADDR3;	
			}			
		else if(tim_dv==7)		//////////////////////////////////////////			7			//////////////////////////////////////////
			{
			pot_SUM[tim_dv-1]-=pot_ADC[tim_dv-1][cnt_ad];	
			pot_ADC[tim_dv-1][cnt_ad] = HAL_ADC_GetValue(&hadc1);
			pot_SUM[tim_dv-1]+=pot_ADC[tim_dv-1][cnt_ad];		
			pot_SUM[tim_dv]-=pot_ADC[tim_dv][cnt_ad];		
			pot_ADC[tim_dv][cnt_ad] = HAL_ADC_GetValue(&hadc2);	
			pot_SUM[tim_dv]+=pot_ADC[tim_dv][cnt_ad];	
			ADC_TMP = (pot_SUM[tim_dv]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv]))	////////// LOW0	
				{
				if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv]))	
					{
					pot_out[tim_dv] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv] = (pot_out[tim_dv]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv] = pot_convcd(pot_out[tim_dv]);	
				eql0.lg	= EQ_ISO[pot_8b[tim_dv]];	
				eqr0.lg	= EQ_ISO[pot_8b[tim_dv]];			
				}				
			ADC_TMP = (pot_SUM[tim_dv-1]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv-1]))	////////// LOW1		
				{
				if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv-1]))	
					{
					pot_out[tim_dv-1] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv-1] = (pot_out[tim_dv-1]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv-1] = pot_convcd(pot_out[tim_dv-1]);	
				eql1.lg	= EQ_ISO[pot_8b[tim_dv-1]];	
				eqr1.lg	= EQ_ISO[pot_8b[tim_dv-1]];			
				}	
			////////added calc code				
			if((GPIOB->IDR & 0x00000004)==0x00U)			//Bluetooth button
				{
				if(BT_bBUTT_prsd==0)
					{
					if(BT_ENABLE==0)
						{
						BT_ACTIVATION_PRC = 24;	
						}						
					else if(BT_ENABLE==1)
						{
						if((GPIOB->IDR & 0x00000800)==0x00U)		//SHIFT BUTTON
							{
							BT_ENABLE = 0;	
							BT_POW_OFF;
							BT_LED_ON;								
							need_draw_bticon = 1;	
							}
						else
							{
							BT_PRESS;
							}							
						}						
					BT_bBUTT_prsd = 1;	
					}					
				}
			else
				{
				if(BT_bBUTT_prsd==1)
					{
					if(BT_ENABLE==1)
						{
						if((GPIOB->IDR & 0x00000800)!=0x00U)		//SHIFT BUTTON
							{
							BT_UNPRESS;
							}
						}
					BT_bBUTT_prsd = 0;	
					}						
				}
			if(BT_ENABLE==1)		//check LED in real bluetooth	
				{
				if((GPIOA->IDR & 0x00000040)==0x00U)		//BT_LED		
					{
					BT_LED_ON;	
					}
				else
					{
					BT_LED_OFF;	
					}
				}	
			}	
		else if(tim_dv==8)		//////////////////////////////////////////			8			//////////////////////////////////////////
			{
			HAL_ADC_Start(&hadc1);
			HAL_ADC_Start(&hadc2);	
			if((GPIOE->IDR & 0x00000008) != 0x00U)			//Key in 0	(CFX)
				{
				if(CFX4_prsd==0)
					{
					if(CFXON==4)
						{
						MAX7219_DATA[6] = 0xFF;
						CFXON = 0;			
						}	
					else
						{	
						MAX7219_DATA[6] = CFXmask[CFXON-1];	
						CFXON = 4;	
						}
					MAX7219_need_update|=0x40;		
					CFX4_prsd = 1;	
					}
				}
			else
				{
				if(CFX4_prsd==1)
					{
					CFX4_prsd = 0;	
					}	
				}				
			if((GPIOA->IDR & 0x00000080) != 0x00U)			//Key in 1 (other keys)
				{
				if(EQCURVE_prsd==0)
					{
					EQCURVE_prsd = 1;	
					}
				}		
			else
				{
				if(EQCURVE_prsd==1)
					{
					EQCURVE_prsd = 0;	
					}	
				}				
			if((GPIOD->IDR & 0x00000008) != 0x00U)			//Key in 2 (BFX)
				{
				BFXn = 3;
				if(BFXn_previous!=BFXn)
					{
					need_draw_fxsel = 1;	
					BFXn_previous = BFXn;	
					}					
				}	
			if((GPIOB->IDR & 0x00000080) != 0x00U)			//Key in 3 (other keys)
				{
				if(BEATp_prsd==0)
					{
					if(B<15)
						{
						B++;	
						}	
					BEATp_prsd = 1;	
					}
				}
			else
				{
				if(BEATp_prsd==1)
					{
					BEATp_prsd = 0;	
					}	
				}	
			MUX_ADDR4;	
			}	
		else if(tim_dv==9)		//////////////////////////////////////////			9			//////////////////////////////////////////
			{
			pot_SUM[tim_dv-1]-=pot_ADC[tim_dv-1][cnt_ad];	
			pot_ADC[tim_dv-1][cnt_ad] = HAL_ADC_GetValue(&hadc1);
			pot_SUM[tim_dv-1]+=pot_ADC[tim_dv-1][cnt_ad];		
			pot_SUM[tim_dv]-=pot_ADC[tim_dv][cnt_ad];		
			pot_ADC[tim_dv][cnt_ad] = HAL_ADC_GetValue(&hadc2);	
			pot_SUM[tim_dv]+=pot_ADC[tim_dv][cnt_ad];	
			ADC_TMP = (pot_SUM[tim_dv]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv]))	////////// MIXN	
				{
				if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv]))	
					{
					pot_out[tim_dv] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv] = (pot_out[tim_dv]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv] = pot_convcd(pot_out[tim_dv]);	
				MIXN0_ATT = MXNG[255-pot_8b[tim_dv]];		
				MIXN1_ATT = MXNG[pot_8b[tim_dv]];		
				}				
			ADC_TMP = (pot_SUM[tim_dv-1]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv-1]))	////////// DEPT	
				{
				if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv-1]))	
					{
					pot_out[tim_dv-1] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv-1] = (pot_out[tim_dv-1]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv-1] = pot_convcd(pot_out[tim_dv-1]);	
				}				
			}	
		else if(tim_dv==10)		//////////////////////////////////////////			10			//////////////////////////////////////////
			{
			HAL_ADC_Start(&hadc1);
			HAL_ADC_Start(&hadc2);	
			if((GPIOE->IDR & 0x00000008) != 0x00U)			//Key in 0	(CFX)
				{
				if(CFX5_prsd==0)
					{
					if(CFXON==5)
						{
						MAX7219_DATA[6] = 0xFF;
						CFXON = 0;			
						}	
					else
						{	
						MAX7219_DATA[6] = CFXmask[CFXON-1];	
						CFXON = 5;	
						}
					MAX7219_need_update|=0x40;						
					CFX5_prsd = 1;	
					}
				}
			else
				{
				if(CFX5_prsd==1)
					{
					CFX5_prsd = 0;	
					}	
				}				
			if((GPIOA->IDR & 0x00000080) != 0x00U)			//Key in 1 (other keys)
				{
				CRSFCURVE_scan|= 0x01;
				}		
			else
				{
				CRSFCURVE_scan&= 0xFE;	
				}				
			if((GPIOD->IDR & 0x00000008) != 0x00U)			//Key in 2 (BFX)
				{
				BFXn = 4;
				if(BFXn_previous!=BFXn)
					{
					need_draw_fxsel = 1;	
					BFXn_previous = BFXn;	
					}					
				}	
			MUX_ADDR5;	
			}			
		else if(tim_dv==11)		//////////////////////////////////////////			11			//////////////////////////////////////////
			{
			pot_SUM[tim_dv-1]-=pot_ADC[tim_dv-1][cnt_ad];	
			pot_ADC[tim_dv-1][cnt_ad] = HAL_ADC_GetValue(&hadc1);
			pot_SUM[tim_dv-1]+=pot_ADC[tim_dv-1][cnt_ad];		
			pot_SUM[tim_dv]-=pot_ADC[tim_dv][cnt_ad];		
			pot_ADC[tim_dv][cnt_ad] = HAL_ADC_GetValue(&hadc2);	
			pot_SUM[tim_dv]+=pot_ADC[tim_dv][cnt_ad];	
			ADC_TMP = (pot_SUM[tim_dv]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv]))	////////// MID0	
				{
				if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv]))	
					{
					pot_out[tim_dv] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv] = (pot_out[tim_dv]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv] = pot_convcd(pot_out[tim_dv]);	
				eql0.mg	= EQ_ISO[pot_8b[tim_dv]];	
				eqr0.mg	= EQ_ISO[pot_8b[tim_dv]];
				}				
			ADC_TMP = (pot_SUM[tim_dv-1]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv-1]))	////////// MID1	
				{
				if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv-1]))	
					{
					pot_out[tim_dv-1] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv-1] = (pot_out[tim_dv-1]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv-1] = pot_convcd(pot_out[tim_dv-1]);	
				eql1.mg	= EQ_ISO[pot_8b[tim_dv-1]];	
				eqr1.mg	= EQ_ISO[pot_8b[tim_dv-1]];		
				}	
			}	
		else if(tim_dv==12)		//////////////////////////////////////////			12			//////////////////////////////////////////
			{
			HAL_ADC_Start(&hadc1);
			HAL_ADC_Start(&hadc2);	
			if((GPIOE->IDR & 0x00000008) != 0x00U)			//Key in 0	(CFX)
				{
				if(CFX6_prsd==0)
					{
					if(CFXON==6)
						{
						MAX7219_DATA[6] = 0xFF;
						CFXON = 0;			
						}	
					else
						{	
						MAX7219_DATA[6] = CFXmask[CFXON-1];	
						CFXON = 6;	
						}
					MAX7219_need_update|=0x40;
					CFX6_prsd = 1;	
					}
				}
			else
				{
				if(CFX6_prsd==1)
					{
					CFX6_prsd = 0;	
					}	
				}				
			if((GPIOA->IDR & 0x00000080) != 0x00U)			//Key in 1 (other keys)
				{
				CRSFCURVE_scan|= 0x02;
				}
			else
				{
				CRSFCURVE_scan&= 0xFD;	
				}				
			if((GPIOD->IDR & 0x00000008) != 0x00U)			//Key in 2 (BFX)
				{
				BFXn = 5;
				if(BFXn_previous!=BFXn)
					{
					need_draw_fxsel = 1;	
					BFXn_previous = BFXn;	
					}					
				}	
			if((GPIOB->IDR & 0x00000080) != 0x00U)			//Key in 3 (other keys)
				{
				if(BFXON_prsd==0)
					{
					if(BFXON==0)
						{
						MAX7219_DATA[7]&=	0xEF;
						BFXON = 1;	
						}						
					else
						{
						MAX7219_DATA[7]|=	0x10;
						BFXON = 0;	
						}
					MAX7219_need_update|=0x80;
					BFXON_prsd = 1;
					}
				}
			else
				{
				if(BFXON_prsd==1)
					{
					BFXON_prsd = 0;	
					}	
				}		
			MUX_ADDR6;	
			}					
		else if(tim_dv==13)		//////////////////////////////////////////			13			//////////////////////////////////////////
			{
			pot_SUM[tim_dv-1]-=pot_ADC[tim_dv-1][cnt_ad];	
			pot_ADC[tim_dv-1][cnt_ad] = HAL_ADC_GetValue(&hadc1);
			pot_SUM[tim_dv-1]+=pot_ADC[tim_dv-1][cnt_ad];		
			pot_SUM[tim_dv]-=pot_ADC[tim_dv][cnt_ad];		
			pot_ADC[tim_dv][cnt_ad] = HAL_ADC_GetValue(&hadc2);	
			pot_SUM[tim_dv]+=pot_ADC[tim_dv][cnt_ad];	
			ADC_TMP = (pot_SUM[tim_dv]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv]))	////////// FDR0	
				{
				if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv]))	
					{
					pot_out[tim_dv] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv] = (pot_out[tim_dv]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv] = pot_convln(pot_out[tim_dv]);	
				FDR0_ATT = VR_LOG[pot_8b[tim_dv]];	
				}				
			ADC_TMP = (pot_SUM[tim_dv-1]+4)>>3;	
			if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv-1]))	////////// FDR1	
				{
				if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv-1]))	
					{
					pot_out[tim_dv-1] = ADC_TMP;
					}
				else
					{
					pot_out[tim_dv-1] = (pot_out[tim_dv-1]+ADC_TMP+1)>>1;	
					}
				pot_8b[tim_dv-1] = pot_convln(pot_out[tim_dv-1]);
				FDR1_ATT = VR_LOG[pot_8b[tim_dv-1]];	
				}						
			}	
		else if(tim_dv==14)		//////////////////////////////////////////			14			//////////////////////////////////////////
			{
			HAL_ADC_Start(&hadc1);
			HAL_ADC_Start(&hadc2);	
			if((GPIOE->IDR & 0x00000008) != 0x00U)			//Key in 0	(CFX)
				{
				if(CFX7_prsd==0)
					{
					if(CFXON==7)
						{
						MAX7219_DATA[6] = 0xFF;
						CFXON = 0;			
						}	
					else
						{	
						MAX7219_DATA[6] = CFXmask[CFXON-1];	
						CFXON = 7;	
						}
					MAX7219_need_update|=0x40;
					CFX7_prsd = 1;	
					}
				}
			else
				{
				if(CFX7_prsd==1)
					{
					CFX7_prsd = 0;	
					}	
				}				
			if((GPIOD->IDR & 0x00000008) != 0x00U)			//Key in 2 (BFX)
				{
				BFXn = 6;
				if(BFXn_previous!=BFXn)
					{
					need_draw_fxsel = 1;	
					BFXn_previous = BFXn;	
					}					
				}	
			if((GPIOB->IDR & 0x00000080) != 0x00U)			//Key in 3 (other keys)
				{
				if(CUE1_prsd==0)
					{
					if(CUE1ON==0)
						{
						MAX7219_DATA[7]|=	0x03;
						CUE1ON = 1;	
						}						
					else
						{
						MAX7219_DATA[7]&=	0xFC;	
						CUE1ON = 0;	
						}
					MAX7219_need_update|=0x80;	
					CUE1_prsd = 1;	
					}
				}
			else
				{
				if(CUE1_prsd==1)
					{
					CUE1_prsd = 0;	
					}	
				}	
			MUX_ADDR7;	
			}			
		tim_dv++;	
		}
	else					//////////////////////////////////////////			15			//////////////////////////////////////////
		{
		pot_SUM[tim_dv-1]-=pot_ADC[tim_dv-1][cnt_ad];	
		pot_ADC[tim_dv-1][cnt_ad] = HAL_ADC_GetValue(&hadc1);
		pot_SUM[tim_dv-1]+=pot_ADC[tim_dv-1][cnt_ad];		
		pot_SUM[tim_dv]-=pot_ADC[tim_dv][cnt_ad];		
		pot_ADC[tim_dv][cnt_ad] = HAL_ADC_GetValue(&hadc2);	
		pot_SUM[tim_dv]+=pot_ADC[tim_dv][cnt_ad];	
		ADC_TMP = (pot_SUM[tim_dv]+4)>>3;	
		if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv]))	////////// TRM0	
			{
			if((ADC_TMP>(pot_out[tim_dv]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv]))	
				{
				pot_out[tim_dv] = ADC_TMP;
				}
			else
				{
				pot_out[tim_dv] = (pot_out[tim_dv]+ADC_TMP+1)>>1;	
				}
			pot_8b[tim_dv] = pot_convln(pot_out[tim_dv]);	
			TRM0_ATT = TRM[pot_8b[tim_dv]];	
			}				
		ADC_TMP = (pot_SUM[tim_dv-1]+4)>>3;	
		if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis))||((ADC_TMP+adc_hysteresis)<pot_out[tim_dv-1]))	////////// TRM1	
			{
			if((ADC_TMP>(pot_out[tim_dv-1]+adc_hysteresis2))||((ADC_TMP+adc_hysteresis2)<pot_out[tim_dv-1]))	
				{
				pot_out[tim_dv-1] = ADC_TMP;
				}
			else
				{
				pot_out[tim_dv-1] = (pot_out[tim_dv-1]+ADC_TMP+1)>>1;	
				}
			pot_8b[tim_dv-1] = pot_convln(pot_out[tim_dv-1]);	
			TRM1_ATT = TRM[pot_8b[tim_dv-1]];	
			}	
		////////added calc code	
		if(CRSFCURVE_scan==0)
			{
			CRSFCURVE = 0;	
			}			
		else if(CRSFCURVE_scan==1)
			{
			CRSFCURVE = 1;	
			}	
		else
			{
			CRSFCURVE = 2;	
			}	
		CRSFCURVE_scan = 0;
		if(cnt_ad<7)
			{
			cnt_ad++;	
			}			
		else
			{
			cnt_ad = 0;	
			}	
		tim_dv = 0;	
		}
  HAL_TIM_IRQHandler(&htim2);
	//BT_LED_OFF;	
	}

	
	
	
///////////////////////////////////////////////
//
//	out 0...255		
//				
//			
uint8_t pot_convln(uint16_t dt)
	{
	uint32_t res;	
	if(dt<POTL)
		{
		return 0;	
		}		
	else
		{
		if(dt>POTH)
			{
			dt = POTH;	
			}
		res = dt-POTL;	//3967
		res*=POTxLN;
		return res>>12;
		}		
	};
	
	
///////////////////////////////////////////////
//
//	out 0...255, 128 - center				
//				
//		
uint8_t pot_convcd(uint16_t dt)
	{
	uint32_t res;	
	if(dt<POTCL)
		{
		if(dt<POTL)
			{
			return 0;	
			}		
		else	//0...127
			{	
			res = dt-POTL;		//1898
			res*=POTxCD1;
			return res>>12;
			}
		}		
	else if(dt<=POTCH)
		{
		return 128;	
		}	
	else		//128...255
		{
		if(dt>POTH)
			{
			dt = POTH;	
			}	
		res = dt-POTCH;		//1868
		res*=POTxCD2;
		return (res>>12)+128;
		}		
	};	


/////////////////////////////////////////
//convert PCM level to DB scale
//
//	input: PCM level 0...32767
//
//	output DB scale level 0...12
//	
uint8_t LVLtoDB(uint16_t lvl)
	{	
	if(lvl>dBConv[6])
		{
		if(lvl>dBConv[3])
			{
			if(lvl>dBConv[1])
				{
				if(lvl>dBConv[0])
					{
					return 12;
					}
				else
					{
					return 11;	
					}					
				}
			else
				{
				if(lvl>dBConv[2])
					{
					return 10;
					}
				else
					{
					return 9;	
					}
				}				
			}				
		else
			{
			if(lvl>dBConv[4])
				{
				return 8;
				}
			else if(lvl>dBConv[5])
				{
				return 7;	
				}
			else
				{
				return 6;	
				}
			}	
		}
	else
		{
		if(lvl>dBConv[9])
			{	
			if(lvl>dBConv[7])
				{				
				return 5;
				}
			else if(lvl>dBConv[8])
				{
				return 4;	
				}
			else
				{
				return 3;	
				}				
			}
		else
			{
			if(lvl>dBConv[10])
				{				
				return 2;
				}
			else if(lvl>dBConv[11])
				{
				return 1;	
				}			
			else
				{
				return 0;	
				}	
			}
		}
	};
	

	
	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



