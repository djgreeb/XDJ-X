/////////////////////////////////////////////////////////////////////////////////////////////////
// 	#include MAX7219 - leds driver for level meter and backlight buttons
//	product: XDJ-X mixer assy
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void MAX7219_UPDATE(uint8_t num);
void MAX7219_INIT(void);
void LEVEL_METER(uint8_t ch, uint8_t lvl, uint8_t pklvl);

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void MAX7219_UPDATE(uint8_t num)
	{		
	uint8_t DT[2];	
	GPIOE->BSRR = 0x40000000;	
	DT[0] = MAX7219_DATA[num];
	DT[1] = num+1;
	HAL_SPI_Transmit(&hspi2, DT, 1, 10);			//size = 1, becouse 16bits mode spi
	GPIOE->BSRR = 0x00004000;
	return;	
	};
	
	
/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void MAX7219_INIT(void)
	{
	//((&hspi2)->Instance->CR1 |=  SPI_CR1_SPE);
	uint8_t DT[2];
	MAX7219_DATA[0] = 0x00;	
	MAX7219_DATA[1] = 0x00;
	MAX7219_DATA[2] = 0x00;
	MAX7219_DATA[3] = 0x00;
	MAX7219_DATA[4] = 0x00;
	MAX7219_DATA[5] = 0x00;
	MAX7219_DATA[6] = 0x00;
	MAX7219_DATA[7] = 0x00;	
	MAX7219_UPDATE(0);
	MAX7219_UPDATE(1);
	MAX7219_UPDATE(2);
	MAX7219_UPDATE(3);
	MAX7219_UPDATE(4);
	MAX7219_UPDATE(5);
	MAX7219_UPDATE(6);
	MAX7219_UPDATE(7);		
	HAL_GPIO_WritePin(GPIOE, MAX_LOAD_Pin, GPIO_PIN_RESET);
	DT[0] = 0xFF;										//power on
	DT[1] = 0x0C;
	HAL_SPI_Transmit(&hspi2, DT, 1, 10);
	HAL_GPIO_WritePin(GPIOE, MAX_LOAD_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, MAX_LOAD_Pin, GPIO_PIN_RESET);
	DT[0] = 0x07;										//scan limit 8 rows
	DT[1] = 0x0B;
	HAL_SPI_Transmit(&hspi2, DT, 1, 10);
	HAL_GPIO_WritePin(GPIOE, MAX_LOAD_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, MAX_LOAD_Pin, GPIO_PIN_RESET);
	DT[0] = 0x05;										//intensity
	DT[1] = 0x0A;
	HAL_SPI_Transmit(&hspi2, DT, 1, 10);
	HAL_GPIO_WritePin(GPIOE, MAX_LOAD_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, MAX_LOAD_Pin, GPIO_PIN_RESET);
	DT[0] = 0x00;							//no decode
	DT[1] = 0x09;
	HAL_SPI_Transmit(&hspi2, DT, 1, 10);
	HAL_GPIO_WritePin(GPIOE, MAX_LOAD_Pin, GPIO_PIN_SET);
	};	


	
/////////////////////////////////////////////////////////////////////////////////////////////////
//
//		lvl = 0..12
//		pklvl = 0..12
//		ch = 0..3
//	
void LEVEL_METER(uint8_t ch, uint8_t lvl, uint8_t pklvl)
	{
	if(ch==0)
		{
		MAX7219_DATA[2]&=0x0F;	
		if(lvl<9)
			{
			MAX7219_DATA[0] = 0xFF>>(8-lvl);	
			}			
		else
			{
			MAX7219_DATA[0] = 0xFF;
			MAX7219_DATA[2]|=(0xFF>>(4-(lvl-8)))&0xF0;
			}
		if(pklvl!=0)
			{
			if(pklvl<9)
				{
				MAX7219_DATA[0] = 0x01<<(pklvl-1);	
				}			
			else
				{
				MAX7219_DATA[2]|= 0x10<<(pklvl-9);
				}	
			}			
		MAX7219_need_update|= 0x05;	
		}		
else if(ch==1)
		{
		MAX7219_DATA[2]&=0xF0;	
		if(lvl<9)
			{
			MAX7219_DATA[1] = 0xFF>>(8-lvl);	
			}			
		else
			{
			MAX7219_DATA[1] = 0xFF;
			MAX7219_DATA[2]|=(0x0F>>(4-(lvl-8)));	
			}
		if(pklvl!=0)
			{
			if(pklvl<9)
				{
				MAX7219_DATA[1] = 0x01<<(pklvl-1);	
				}			
			else
				{
				MAX7219_DATA[2]|= 0x01<<(pklvl-9);
				}	
			}		
		MAX7219_need_update|= 0x06;	
		}
else if(ch==2)
		{
		MAX7219_DATA[5]&=0x0F;	
		if(lvl<9)
			{
			MAX7219_DATA[3] = 0xFF>>(8-lvl);	
			}			
		else
			{
			MAX7219_DATA[3] = 0xFF;
			MAX7219_DATA[5]|=(0xFF>>(4-(lvl-8)))&0xF0;
			}
		if(pklvl!=0)
			{
			if(pklvl<9)
				{
				MAX7219_DATA[3] = 0x01<<(pklvl-1);	
				}			
			else
				{
				MAX7219_DATA[5]|= 0x10<<(pklvl-9);
				}	
			}		
		MAX7219_need_update|= 0x28;	
		}		
	else
		{
		MAX7219_DATA[5]&=0xF0;	
		if(lvl<9)
			{
			MAX7219_DATA[4] = 0xFF>>(8-lvl);	
			}			
		else
			{
			MAX7219_DATA[4] = 0xFF;
			MAX7219_DATA[5]|=(0x0F>>(4-(lvl-8)));	
			}
		if(pklvl!=0)
			{
			if(pklvl<9)
				{
				MAX7219_DATA[4] = 0x01<<(pklvl-1);	
				}			
			else
				{
				MAX7219_DATA[5]|= 0x01<<(pklvl-9);
				}	
			}			
		MAX7219_need_update|= 0x30;	
		}	
	return;	
	}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
																																									