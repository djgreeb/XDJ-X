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
uint8_t sector = 0;	

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
uint8_t BUTTON_PLAY_pressed = 0;												
uint8_t BUTTON_CUE_pressed = 0;
uint8_t BUTTON_REV_pressed = 0;
uint8_t BUTTON_MT_pressed = 0;
uint8_t BUTTON_JOG_pressed = 0;
uint8_t BUTTON_4L_pressed = 0;
uint8_t BUTTON_8L_pressed = 0;
uint8_t BUTTON_RELOOP_pressed = 0;
uint8_t BUTTON_NEXT_pressed = 0;
uint8_t BUTTON_PREV_pressed = 0;
uint8_t BUTTON_SLIP_pressed = 0;
uint8_t BUTTON_VINYL_pressed = 0;												

												
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
			
const static uint32_t PADS_COLOR[6] = {0xFF2B0000,			//red
																			 0xFF002000,			//green
																			 0xFF190800,			//orange
																			 0xFF000035,			//blue
																			 0xFF302011,			//white
																			 0xFF010101				//black
																			 };				

/* temporary ---------------------------------------------------------*/
uint8_t spd = 13;
uint8_t LED_PLAY_ON = 1;		
uint8_t LED_CUE_ON = 1;				
uint8_t LED_REV_ON = 1;					
uint8_t LED_MT_ON = 1;						
uint8_t LED_VINYL_ON = 1;				
uint8_t LED_SLIP_ON = 1;				
uint8_t LED_RELOOP_ON = 1;			
uint8_t LED_8L_ON = 1;			
uint8_t LED_4L_ON = 1;

/* SPI DMA TRANSFER ---------------------------------------------------------*/																			 
		
uint8_t deckRbuf[20];
																			 
#ifdef DECK_1
uint8_t deckTbuf[20] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
												0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#endif

#ifndef DECK_1
uint8_t deckTbuf[20] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
												0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif			

																			 

												
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
																																									