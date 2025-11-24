/////////////////////////////////////////////////////////////////////////////////////////////////
//  Assist-ASA spi EEPROM mem lib M95160-WMN6TP (2KB)
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdint.h"

void WR_EEMEM(uint16_t add, uint8_t data);				//write spi eeprom memory
uint8_t RD_EEMEM(uint16_t add);										//read spi eeprom memory


	
//////////////////////////////////////////////////////////////////////////
//
//		write spi eeprom memory
//		execution time 7ms
void WR_EEMEM(uint16_t add, uint8_t data)	
	{		
	uint8_t dt[4];
	dt[0] = 0x06;				//write data array			
	dt[1] = (add>>8)&0x07;	
	dt[2] = add&0x00FF;	
	dt[3] = data; 		
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);	
	HAL_SPI_Transmit(&hspi1, dt, 1, 25);
	HAL_Delay(1);	
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);	
	HAL_Delay(1);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	dt[0] = 0x02;
	HAL_SPI_Transmit(&hspi1, dt, 4, 25);
	HAL_Delay(2);	
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
	HAL_Delay(1);		
	return;	
	}
	
	
	
//////////////////////////////////////////////////////////////////////////
//
//		read spi eeprom memory
//	
uint8_t RD_EEMEM(uint16_t add)
	{
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);		
	uint8_t dt[4], rxdt[4];
	dt[0] = 0x03;				//read data array
	dt[1] = (add>>8)&0x07;	
	dt[2] =  add&0x00FF;
	dt[3] =	0x00;
	HAL_SPI_Transmit(&hspi1, dt, 3, 25);	
	HAL_SPI_Receive(&hspi1, dt, 1, 10);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);		
	return dt[0];	
	}
	
	
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	