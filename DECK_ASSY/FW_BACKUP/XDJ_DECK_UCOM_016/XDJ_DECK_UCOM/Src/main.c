
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2025 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#include "VFL_data.h"



////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//		Generated with: STM32CubeMX 4.25.1
//		Firmware Package Name and Version STM32Cube FW_F4 V1.21.0
//
//	
//
//
//
//
//	TIM1 LEDS PWM
//	TIM3 transformer PWM
//	TIM4 JOG ENCODER TIMER
//	TIM5 1315*4Hz for VFD
//	SPI1 DMA for transfer between main assy and deck ucom
//	SPI2 DMA for PADS RGB leds 
//	SPI3 DMA for VFD 
//	
//
//
//
////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//	ver. 0.03
//		- added all gpio
//		- added sharp fade transformer pwm
//	ver. 0.11 
//		- added button and leds handler
//		- added difine separated code for DECK_1 and DECK_2
//		-	added spi pads function
//	ver. 0.12 
//		-	changed starting delays
//	ver. 0.15
//		-	regenerated code		
//		-	add SPI DMA RX, TX buffer
//	ver. 0.16
//		- fix spi1 bug, change gpio speed to low 
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////

///////DECK CONFIGURATION//////

//#define DECK_1							//cooment this line, when deck 2 used

//#ifdef DECK_1

//#endif

//#ifndef DECK_1


//#endif	

#include "global_variables.h"
extern DMA_HandleTypeDef hdma_spi1_tx;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void draw_playing_sector(uint8_t sc);
void COLOR_SET(uint32_t color, uint8_t LD);												
												
												
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
	
	HAL_TIM_Base_Start_IT(&htim5);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);			//transformer pwm
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);			//leds pwm	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);			//leds pwm
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);			//leds pwm
	
	HAL_Delay(3100);
	HAL_SPI_TransmitReceive_DMA(&hspi1, deckTbuf, deckRbuf, 20);
	
	
	
	for(i=0;i<transformer_pwm;i++)			//soft start pwm
		{
		TIM3->CCR1 = i;
		HAL_Delay(25);	
		}
	TIM3->CCR1 = transformer_pwm;
		
	HAL_Delay(300);		
	COLOR_SET(PADS_COLOR[5], 0);
	COLOR_SET(PADS_COLOR[5], 1);
	COLOR_SET(PADS_COLOR[5], 2);
	COLOR_SET(PADS_COLOR[5], 3);
	COLOR_SET(PADS_COLOR[5], 4);
	COLOR_SET(PADS_COLOR[5], 5);
	COLOR_SET(PADS_COLOR[5], 6);
	COLOR_SET(PADS_COLOR[5], 7);	
	HAL_SPI_Transmit_DMA(&hspi2, PAD_BUF, 100);		
		
	HAL_GPIO_WritePin(GPIOC, LED_PLAY_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, LED_REV_Pin|LED_CUE_Pin|LED_MT_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, LED_VINYL_Pin|LED_SLIP_Pin|LED_RELOOP_Pin|LED_8L_Pin 
                          |LED_4L_Pin, GPIO_PIN_SET);	
		
	VFL_DATA[0]|=BITON[4];				//vinyl label ON
	VFL_DATA[0]|=BITON[6];				//S ring
		
	#ifdef DECK_1
	spd = 13;	
	VFL_DATA[27]|=BITON[0];				// cue marker (C74)
	VFL_DATA[27]|=BITON[2];				// cue marker	(C75)	
	#endif

	#ifndef DECK_1
	spd = 12;
	VFL_DATA[41]|=BITON[0];				// cue marker (C19)
	VFL_DATA[41]|=BITON[2];				// cue marker	(C18)
	#endif		
		
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	HAL_Delay(spd);

	if(sector<134)
		{
		sector++;	
		}
	else
		{
		sector = 0;	
		}
	draw_playing_sector(sector);				

	/* BUTTON PLAY ---------------------------------------------------------*/		
	if(HAL_GPIO_ReadPin(GPIOC, BUTTON_PLAY_Pin)==0 && BUTTON_PLAY_pressed==0)
		{
		if(LED_PLAY_ON)
			{
			LED_PLAY_L;	
			LED_PLAY_ON = 0;	
			}
		else
			{
			LED_PLAY_H;	
			LED_PLAY_ON = 1;	
			}
		BUTTON_PLAY_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOC, BUTTON_PLAY_Pin)==1 && BUTTON_PLAY_pressed==1)
		{
		BUTTON_PLAY_pressed = 0;		
		}	
	/* BUTTON CUE ---------------------------------------------------------*/	
	if(HAL_GPIO_ReadPin(GPIOC, BUTTON_CUE_Pin)==0 && BUTTON_CUE_pressed==0)
		{
		if(LED_CUE_ON)
			{
			LED_CUE_L;	
			LED_CUE_ON = 0;	
			}
		else
			{
			LED_CUE_H;	
			LED_CUE_ON = 1;	
			}
		BUTTON_CUE_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOC, BUTTON_CUE_Pin)==1 && BUTTON_CUE_pressed==1)
		{
		BUTTON_CUE_pressed = 0;		
		}	
	/* BUTTON REV ---------------------------------------------------------*/	
	if(HAL_GPIO_ReadPin(GPIOC, BUTTON_REV_Pin)==0 && BUTTON_REV_pressed==0)
		{
		if(LED_REV_ON)
			{
			LED_REV_L;
			LED_REV_ON = 0;	
			}
		else
			{
			LED_REV_H;
			LED_REV_ON = 1;	
			}
		BUTTON_REV_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOC, BUTTON_REV_Pin)==1 && BUTTON_REV_pressed==1)
		{
		BUTTON_REV_pressed = 0;		
		}	
	/* BUTTON MT ---------------------------------------------------------*/
	if(HAL_GPIO_ReadPin(GPIOC, BUTTON_MT_Pin)==0 && BUTTON_MT_pressed==0)
		{
		if(LED_MT_ON)
			{
			LED_MT_L;	
			LED_MT_ON = 0;	
			}
		else
			{
			LED_MT_H;	
			LED_MT_ON = 1;	
			}
		BUTTON_MT_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOC, BUTTON_MT_Pin)==1 && BUTTON_MT_pressed==1)
		{
		BUTTON_MT_pressed = 0;		
		}	
	/* BUTTON NEXT ---------------------------------------------------------*/	
	if(HAL_GPIO_ReadPin(GPIOC, BUTTON_NEXT_Pin)==0 && BUTTON_NEXT_pressed==0)
		{
		if(spd<50)
			{
			spd++;	
			}	
		BUTTON_NEXT_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOC, BUTTON_NEXT_Pin)==1 && BUTTON_NEXT_pressed==1)
		{
		BUTTON_NEXT_pressed = 0;		
		}	
	/* BUTTON JOG ---------------------------------------------------------*/	
	if(HAL_GPIO_ReadPin(GPIOB, BUTTON_JOG_Pin)==0 && BUTTON_JOG_pressed==0)
		{
		VFL_DATA[0]|=BITON[5];				//RING sensor ON	
		BUTTON_JOG_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOB, BUTTON_JOG_Pin)==1 && BUTTON_JOG_pressed==1)
		{
		VFL_DATA[0]&=BITOFF[5];				//RING sensor OFF	
		BUTTON_JOG_pressed = 0;		
		}	
	/* BUTTON 8L ---------------------------------------------------------*/			
	if(HAL_GPIO_ReadPin(GPIOB, BUTTON_8L_Pin)==0 && BUTTON_8L_pressed==0)
		{
		if(LED_8L_ON)
			{
			LED_8L_L;	
			LED_8L_ON = 0;	
			}
		else
			{
			LED_8L_H;	
			LED_8L_ON = 1;	
			}
		BUTTON_8L_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOB, BUTTON_8L_Pin)==1 && BUTTON_8L_pressed==1)
		{
		BUTTON_8L_pressed = 0;		
		}	
	/* BUTTON RELOOP ---------------------------------------------------------*/	
	if(HAL_GPIO_ReadPin(GPIOB, BUTTON_RELOOP_Pin)==0 && BUTTON_RELOOP_pressed==0)
		{
		if(LED_RELOOP_ON)
			{
			LED_RELOOP_L;	
			LED_RELOOP_ON = 0;	
			}
		else
			{
			LED_RELOOP_H;	
			LED_RELOOP_ON = 1;	
			}
		BUTTON_RELOOP_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOB, BUTTON_RELOOP_Pin)==1 && BUTTON_RELOOP_pressed==1)
		{
		BUTTON_RELOOP_pressed = 0;		
		}	
	/* BUTTON PREV ---------------------------------------------------------*/	
	if(HAL_GPIO_ReadPin(GPIOA, BUTTON_PREV_Pin)==0 && BUTTON_PREV_pressed==0)
		{
		if(spd>1)
			{
			spd--;	
			}
		BUTTON_PREV_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOA, BUTTON_PREV_Pin)==1 && BUTTON_PREV_pressed==1)
		{
		BUTTON_PREV_pressed = 0;		
		}		
	/* BUTTON 4L ---------------------------------------------------------*/		
	if(HAL_GPIO_ReadPin(GPIOA, BUTTON_4L_Pin)==0 && BUTTON_4L_pressed==0)
		{
		if(LED_4L_ON)
			{
			LED_4L_L;	
			LED_4L_ON = 0;	
			}
		else
			{
			LED_4L_H;	
			LED_4L_ON = 1;	
			}
		BUTTON_4L_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOA, BUTTON_4L_Pin)==1 && BUTTON_4L_pressed==1)
		{
		BUTTON_4L_pressed = 0;		
		}		
	/* BUTTON SLIP ---------------------------------------------------------*/
	if(HAL_GPIO_ReadPin(GPIOA, BUTTON_SLIP_Pin)==0 && BUTTON_SLIP_pressed==0)
		{
		if(LED_SLIP_ON)
			{
			LED_SLIP_L;	
			LED_SLIP_ON = 0;	
			}
		else
			{
			LED_SLIP_H;	
			LED_SLIP_ON = 1;	
			}
		BUTTON_SLIP_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOA, BUTTON_SLIP_Pin)==1 && BUTTON_SLIP_pressed==1)
		{
		BUTTON_SLIP_pressed = 0;		
		}		
	/* BUTTON VINYL ---------------------------------------------------------*/	
	if(HAL_GPIO_ReadPin(GPIOA, BUTTON_VINYL_Pin)==0 && BUTTON_VINYL_pressed==0)
		{
		if(LED_VINYL_ON)
			{
			LED_VINYL_L;	
			VFL_DATA[0]&=BITOFF[4];				//vinyl label OFF
			LED_VINYL_ON = 0;	
			}
		else
			{
			LED_VINYL_H;	
			VFL_DATA[0]|=BITON[4];				//vinyl label ON	
			LED_VINYL_ON = 1;	
			}
		BUTTON_VINYL_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(GPIOA, BUTTON_VINYL_Pin)==1 && BUTTON_VINYL_pressed==1)
		{
		BUTTON_VINYL_pressed = 0;		
		}			
		
		

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */



/////////////////////////////////////////////////////
//
//	Timer 1315*4Hz
//
//
void TIM5_IRQHandler(void)
	{
	if(tim5div<11)
		{
		tim5div++;	
			
		if(tim5div==1)
			{
			LAT_L;
			BK_L;	
			}
		else if(tim5div==2)
			{
			HAL_SPI_Transmit_DMA(&hspi3, &VFL_DATA[0], 16);	
			}			
		else if(tim5div==3)
			{
			BK_H;	
			}				
		else if(tim5div==4)
			{
			LAT_H;	
			}	
		else if(tim5div==5)
			{
			LAT_L;
			BK_L;		
			}	
		else if(tim5div==6)
			{
			HAL_SPI_Transmit_DMA(&hspi3, &VFL_DATA[16], 16);	
			}	
		else if(tim5div==7)
			{
			BK_H;	
			}	
		else if(tim5div==8)
			{
			LAT_H;	
			}	
		else if(tim5div==9)
			{
			LAT_L;
			BK_L;		
			}	
		else if(tim5div==10)
			{
			HAL_SPI_Transmit_DMA(&hspi3, &VFL_DATA[32], 16);	
			}	
		else if(tim5div==11)
			{
			BK_H;	
			}
		}
	else
		{	
		tim5div = 0;
		LAT_H;
		}
  HAL_TIM_IRQHandler(&htim5);
	}





/////////////////////////////////////////////////////
//
//	0...134
//
void draw_playing_sector(uint8_t sc)
	{
	uint8_t i;	
	for(i=0;i<135;i++)
		{
		if(i>=sc && i<(sc+5) || (sc>129 && i<((sc+140)%135)))
			{						//black
			VFL_DATA[PSEGM[i]>>3]&=BITOFF[PSEGM[i]%8];
			}
		else					//white
			{
			VFL_DATA[PSEGM[i]>>3]|=BITON[PSEGM[i]%8];	
			}
		}	
	};
	
	
/////////////////////////////////////////////////////
//	set color buffer for rgb leds
//	led_num 0...7
//
void COLOR_SET(uint32_t color, uint8_t LD)
	{
	if(LD>7)
		{
		return;	
		}		
	uint8_t j, r, g, b;	
		
	if(LD<4)
		{
		LD+= 4;	
		}
	else
		{
		LD = 7-LD;	
		}
		
	r = (color>>16) & 0x000000FF;	
	g = (color>>8) & 0x000000FF;	
	b = color & 0x000000FF; 
	for(j=0;j<4;j++)
		{
		if((g<<(2*j)&0x80)!=0)
			{
			PAD_BUF[j+(12*LD)] = 0xC0;	
			}
		else
			{
			PAD_BUF[j+(12*LD)] = 0x80;	
			}
		if(((g<<(2*j+1))&0x80)!=0)
			{
			PAD_BUF[j+(12*LD)]|=0x0C;	
			}
		else
			{
			PAD_BUF[j+(12*LD)]|=0x08;	
			}	
		}
	for(j=0;j<4;j++)
		{
		if((r<<(2*j)&0x80)!=0)
			{
			PAD_BUF[j+4+(12*LD)] = 0xC0;	
			}
		else
			{
			PAD_BUF[j+4+(12*LD)] = 0x80;	
			}
		if(((r<<(2*j+1))&0x80)!=0)
			{
			PAD_BUF[j+4+(12*LD)]|=0x0C;	
			}
		else
			{
			PAD_BUF[j+4+(12*LD)]|=0x08;	
			}	
		}	
	for(j=0;j<4;j++)
		{
		if((b<<(2*j)&0x80)!=0)
			{
			PAD_BUF[j+8+(12*LD)] = 0xC0;	
			}
		else
			{
			PAD_BUF[j+8+(12*LD)] = 0x80;	
			}
		if(((b<<(2*j+1))&0x80)!=0)
			{
			PAD_BUF[j+8+(12*LD)]|=0x0C;	
			}
		else
			{
			PAD_BUF[j+8+(12*LD)]|=0x08;	
			}	
		}	
	return;	
	};		
	
	
	
/////////////////////////////////////////////////////
//	
//	SPI DMA transfer handler
//	
void DMA2_Stream3_IRQHandler(void)
	{
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
		
	#ifdef DECK_1
	deckTbuf[0] = deckRbuf[0];	
	deckTbuf[1] = deckRbuf[1];	
	deckTbuf[2] = deckRbuf[2];	
	deckTbuf[3] = deckRbuf[3];
	deckTbuf[4] = deckRbuf[4];	
	deckTbuf[5] = deckRbuf[5];	
	deckTbuf[6] = deckRbuf[6];	
	deckTbuf[7] = deckRbuf[7];			
	deckTbuf[8] = deckRbuf[8];	
	deckTbuf[9] = deckRbuf[9];	
	#endif

	#ifndef DECK_1
	deckTbuf[10] = deckRbuf[10];	
	deckTbuf[11] = deckRbuf[11];	
	deckTbuf[12] = deckRbuf[12];	
	deckTbuf[13] = deckRbuf[13];
	deckTbuf[14] = deckRbuf[14];	
	deckTbuf[15] = deckRbuf[15];	
	deckTbuf[16] = deckRbuf[16];	
	deckTbuf[17] = deckRbuf[17];			
	deckTbuf[18] = deckRbuf[18];	
	deckTbuf[19] = deckRbuf[19];
	#endif			
		
//	if(CheckRXCRC()==1)
//		{
//			
//			
//		}
//	
//	CheckTXCRC();	
	}	
	
	
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
