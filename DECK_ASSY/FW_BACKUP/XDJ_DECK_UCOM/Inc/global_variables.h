/////////////////////////////////////////////////////////////////////////////////////////////////
// 	global variables
//	product: XDJ-X deck ucom
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

/* VFD ---------------------------------------------------------*/
uint8_t prev_pl_sect = 0xFF;	
uint8_t prev_cue_sect = 0xFF;	
uint8_t prev_slip_sect = 0xFF;

uint8_t VFL_DATA[48] = {0x60, 0x00, 0x00, 0x00,
												0x00, 0x00, 0x00, 0x00,
												0x00, 0x00, 0x00, 0x00,
												0x00, 0x00, 0x00, 0x00,
	
												0x20, 0x00, 0x00, 0x00,
												0x00, 0x00, 0x00, 0x00,
												0x00, 0x00, 0x00, 0x00,
												0x00, 0x00, 0x00, 0x00,

												0x40, 0x00, 0x00, 0x00,
												0x00, 0x00, 0x00, 0x00,
												0x00, 0x00, 0x00, 0x00,
												0x00, 0x00, 0x00, 0x00 };

																								
#define	LAT_L		GPIOC->BSRR = 0x08000000  //HAL_GPIO_WritePin(LAT_GPIO_Port, LAT_Pin, GPIO_PIN_RESET);
#define	LAT_H		GPIOC->BSRR = 0x00000800  //HAL_GPIO_WritePin(LAT_GPIO_Port, LAT_Pin, GPIO_PIN_SET);								
#define	BK_L		GPIOD->BSRR = 0x00040000  //HAL_GPIO_WritePin(BK_GPIO_Port, BK_Pin, GPIO_PIN_RESET);										
#define	BK_H		GPIOD->BSRR = 0x00000004  //HAL_GPIO_WritePin(BK_GPIO_Port, BK_Pin, GPIO_PIN_SET);
												
/* pwm transformer ---------------------------------------------------------*/																							
#define transformer_pwm 89
uint8_t i;												
					
/* timer ---------------------------------------------------------*/												
uint8_t tim5div = 0;
							
/* BUTTONS ---------------------------------------------------------*/
										

												
												
/* LEDS ---------------------------------------------------------*/
#define LED_PLAY_H		GPIOC->BSRR = 0x00000008
#define LED_PLAY_L		GPIOC->BSRR = 0x00080000
#define LED_CUE_H			GPIOA->BSRR = 0x00000008		
#define LED_CUE_L			GPIOA->BSRR = 0x00080000
#define LED_REV_H			GPIOA->BSRR = 0x00000004			
#define LED_REV_L			GPIOA->BSRR = 0x00040000
#define LED_MT_H			GPIOA->BSRR = 0x00008000			
#define LED_MT_L			GPIOA->BSRR = 0x80000000
#define LED_VINYL_H		GPIOB->BSRR = 0x00000002		
#define LED_VINYL_L		GPIOB->BSRR = 0x00020000	
#define LED_SLIP_H		GPIOB->BSRR = 0x00000004		
#define LED_SLIP_L		GPIOB->BSRR = 0x00040000
#define LED_RELOOP_H	GPIOB->BSRR = 0x00000010		
#define LED_RELOOP_L	GPIOB->BSRR = 0x00100000
#define LED_8L_H			GPIOB->BSRR = 0x00000100		
#define LED_8L_L			GPIOB->BSRR = 0x01000000
#define LED_4L_H			GPIOB->BSRR = 0x00000200		
#define LED_4L_L			GPIOB->BSRR = 0x02000000

/* PADS ---------------------------------------------------------*/
uint32_t PAD[8] = {0};
uint32_t pad_tmp;
uint8_t need_update_pads = 0;
uint8_t PAD_BUF[100] =  {0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88,
												0x88, 0x88, 0x88, 0x88, 
												0x00, 0x00, 0x00, 0x00};			

											

/* JOG RING ---------------------------------------------------------*/																				 
uint8_t R_rx, G_rx, B_rx;						

																			 

/* SPI DMA TRANSFER ---------------------------------------------------------*/																			 
		
uint8_t deckRbuf[20];
																			 
#ifdef DECK_1
uint8_t deckTbuf[18] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
												0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#endif

#ifndef DECK_1
uint8_t deckTbuf[18] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
												0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif			

																			 

												
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
																																									