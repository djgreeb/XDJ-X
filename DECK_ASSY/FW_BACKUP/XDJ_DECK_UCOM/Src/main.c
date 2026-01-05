
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
//	TIM2 time counter for jog encoder
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
//		- fix spi bug, change gpio speed to low 
//	ver. 0.17
//		- added button and leds first handler in spi irq
//	ver. 0.19
//		-	improved SPI transfer
//	ver. 0.20
//		-	SPI transfer fixed
//	ver. 0.21
//		- changed packet length to 8 bytes (7+CRC)
//	ver. 0.23
//		- added cue on vfd
//		- added slip mode on vfd
//	ver. 0.25
//		-	regenerated code
//		-	added TIM2
//	ver. 0.26
//		-	improved jog cnt
//	ver. 0.27
//		- fixed bug, data offsets on 2 deck due to packet reduction by 16 bytes
//	ver. 0.29
//		- slip mode round clear fixed
//		- adc pitch gysteresis added 
//	ver. 0.31
//		- PWM ring have been changed
//	ver. 0.34
//		- load animation added
//		- power animation added
//		- spi transfer up to 2X
//	ver. 0.36
//		- improved jog encoder code
//	ver. 0.39
//		-	The WH2812B data transfer lib has been modified due to the SPI frequency not being suitable for 4-bit data transfer. The buffer has been changed from 100=>75 bytes. 4=>3 bit data transfer.
//	ver. 0.41
//		-	draw_cue_sector (prev_cue_sect<86) bug fixed
//		- startup pads color have been changed
//		- pitch ADC (pot_convcd, POTCL, POTCH, POTxCD1, POTxCD2) have been changed
//
//
//
//
//
//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////

///////DECK CONFIGURATION//////

#define DECK_1							//cooment this line, when deck 2 used

//#ifdef DECK_1

//#endif

//#ifndef DECK_1


//#endif	

#include "global_variables.h"
#include "CRC.h"
extern DMA_HandleTypeDef hdma_spi1_tx;

uint8_t FIRMWARE_VERSION = 41;			//127 max!

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
#include "VFL_GUI.h"
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);				//ENCODER TIME
	HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_ALL);				//ENCODER
	
	
	#ifdef DECK_1
	deckTbuf[4] = FIRMWARE_VERSION;
	#endif

	#ifndef DECK_1
	deckTbuf[12] = FIRMWARE_VERSION;
	#endif
	
	
	HAL_TIM_Base_Start_IT(&htim5);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);			//transformer pwm
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);			//leds pwm	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);			//leds pwm
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);			//leds pwm

	HAL_Delay(1500);
	for(i=0;i<transformer_pwm;i++)			//soft start pwm
		{
		TIM3->CCR1 = i;
		HAL_Delay(25);	
		}
	TIM3->CCR1 = transformer_pwm;
	HAL_Delay(1000);
		
	HAL_SPI_TransmitReceive_DMA(&hspi1, deckTbuf, deckRbuf, 16);

	HAL_Delay(300);				
	COLOR_SET(0xFF780030, 0);
	COLOR_SET(0xFF780030, 1);
	COLOR_SET(0xFF780030, 2);
	COLOR_SET(0xFF780030, 3);
	COLOR_SET(0xFF780030, 4);
	COLOR_SET(0xFF780030, 5);
	COLOR_SET(0xFF780030, 6);
	COLOR_SET(0xFF780030, 7);		
	HAL_SPI_Transmit_DMA(&hspi2, PAD_BUF, 75);		
		
	HAL_GPIO_WritePin(GPIOC, LED_PLAY_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, LED_REV_Pin|LED_CUE_Pin|LED_MT_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, LED_VINYL_Pin|LED_SLIP_Pin|LED_RELOOP_Pin|LED_8L_Pin 
                          |LED_4L_Pin, GPIO_PIN_SET);	
		
	VFL_DATA[0]|=BITON[4];				//vinyl label ON
	VFL_DATA[0]|=BITON[6];				//S ring
		
	TIM1->CCR1 = 255;
	TIM1->CCR2 = 255;
	TIM1->CCR3 = 255;	
		
		
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	if(need_update_pads==2)
		{		
		HAL_SPI_Transmit_DMA(&hspi2, PAD_BUF, 75);		
		need_update_pads = 0;
		}
		
	if(animation_enable==1)			//load animation
		{
		draw_load_animation(step_animation);	
		if(step_animation==0)
			{			
			draw_cue_sector(85);	
			prev_pl_sect = 0xFF;	
			prev_cue_sect = 0xFF;	
			prev_slip_sect = 0xFF;	
			HAL_Delay(200);		//stay on first step - empty circle	
			}
		HAL_Delay(9);
		step_animation++;
		if(step_animation==69)
			{
			HAL_Delay(400);	
			animation_enable = 0;
			step_animation = 0;	
			}	
		}		
	else if(animation_enable==3)
		{
		draw_power_animation(step_animation);	
		if(step_animation==0)
			{			
			draw_cue_sector(85);	
			prev_pl_sect = 0xFF;	
			prev_cue_sect = 0xFF;	
			prev_slip_sect = 0xFF;	
			HAL_Delay(100);		//stay on first step - empty circle	
			}
		HAL_Delay(9);
		step_animation++;
		if(step_animation==271)
			{
			HAL_Delay(50);	
			animation_enable = 0;
			step_animation = 0;	
			}	
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

///////////////////////////////////////////////////////////////
//
//		Jog encoder timer
//
void TIM4_IRQHandler(void)
	{
				
  HAL_TIM_IRQHandler(&htim4);	
	}

		
#include "spi_transfer.h"
	
	
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
	uint8_t j, g[3];	
		
	LD = 7-LD;	
	
	g[1] = (color>>16) & 0x000000FF;	
	g[0] = (color>>8) & 0x000000FF;	
	g[2] = color & 0x000000FF;
	for(j=0;j<3;j++)
		{
		if((g[j]&0x80)!=0)
			{
			PAD_BUF[(9*LD)+(3*j)]|= 0x40;
			}		
		else
			{
			PAD_BUF[(9*LD)+(3*j)]&= 0xBF;	
			}		
		if(((g[j]<<1)&0x80)!=0)
			{
			PAD_BUF[(9*LD)+(3*j)]|= 0x08;
			}		
		else
			{
			PAD_BUF[(9*LD)+(3*j)]&= 0xF7;	
			}	
		if(((g[j]<<2)&0x80)!=0)
			{
			PAD_BUF[(9*LD)+(3*j)]|= 0x01;
			}		
		else
			{
			PAD_BUF[(9*LD)+(3*j)]&= 0xFE;	
			}			
		if(((g[j]<<3)&0x80)!=0)
			{
			PAD_BUF[1+(9*LD)+(3*j)]|= 0x20;
			}		
		else
			{
			PAD_BUF[1+(9*LD)+(3*j)]&= 0xDF;	
			}				
		if(((g[j]<<4)&0x80)!=0)
			{
			PAD_BUF[1+(9*LD)+(3*j)]|= 0x04;
			}		
		else
			{
			PAD_BUF[1+(9*LD)+(3*j)]&= 0xFB;	
			}				
		if(((g[j]<<5)&0x80)!=0)
			{
			PAD_BUF[2+(9*LD)+(3*j)]|= 0x80;
			}		
		else
			{
			PAD_BUF[2+(9*LD)+(3*j)]&= 0x7F;	
			}				
		if(((g[j]<<6)&0x80)!=0)
			{
			PAD_BUF[2+(9*LD)+(3*j)]|= 0x10;
			}		
		else
			{
			PAD_BUF[2+(9*LD)+(3*j)]&= 0xEF;	
			}			
		if(((g[j]<<7)&0x80)!=0)
			{
			PAD_BUF[2+(9*LD)+(3*j)]|= 0x02;
			}		
		else
			{
			PAD_BUF[2+(9*LD)+(3*j)]&= 0xFD;	
			}		
		}
	return;	
	};		
	
	
	

	
	
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
