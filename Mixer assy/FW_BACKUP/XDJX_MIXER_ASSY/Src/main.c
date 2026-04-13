/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "sai.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include "arm_math.h"




////////////////////////////////////////////////////////////////////////////////////////
//
//		Product: XDJ-X mixer assy
//
//
//		Generated with: STM32CubeMX 6.10.0
//		Firmware Package Name and Version STM32Cube FW_H7 V1.11.1
//
//
//		add file arm_math.h
//		SPI2:
//	  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM; (SPI2)
//		SPI1:
//	  hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
//
//    PB3 (JTDO/TRACESWO)     ------> SPI1_SCK
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_7;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
//    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_3;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//
//
//		replace code in: sai.c		
//
//  		hsai_BlockA2.Instance = SAI2_Block_A;
//			hsai_BlockA2.Init.Protocol = SAI_FREE_PROTOCOL;
//			hsai_BlockA2.Init.AudioMode = SAI_MODESLAVE_TX;
//			hsai_BlockA2.Init.DataSize = SAI_DATASIZE_16;
//			hsai_BlockA2.Init.FirstBit = SAI_FIRSTBIT_MSB;
//			hsai_BlockA2.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
//			hsai_BlockA2.Init.Synchro = SAI_SYNCHRONOUS_EXT_SAI1;
//			hsai_BlockA2.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
//			hsai_BlockA2.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
//			hsai_BlockA2.Init.MonoStereoMode = SAI_STEREOMODE;
//			hsai_BlockA2.Init.CompandingMode = SAI_NOCOMPANDING;
//			hsai_BlockA2.Init.TriState = SAI_OUTPUT_NOTRELEASED;
//			hsai_BlockA2.Init.PdmInit.Activation = DISABLE;
//			hsai_BlockA2.Init.PdmInit.MicPairsNbr = 1;
//			hsai_BlockA2.Init.PdmInit.ClockEnable = SAI_PDM_CLOCK1_ENABLE;
//			hsai_BlockA2.FrameInit.FrameLength = 64;
//			hsai_BlockA2.FrameInit.ActiveFrameLength = 32;
//			hsai_BlockA2.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
//			hsai_BlockA2.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
//			hsai_BlockA2.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;
//			hsai_BlockA2.SlotInit.FirstBitOffset = 0;
//			hsai_BlockA2.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
//			hsai_BlockA2.SlotInit.SlotNumber = 2;
//			hsai_BlockA2.SlotInit.SlotActive = 0x00000003;
//			if (HAL_SAI_Init(&hsai_BlockA2) != HAL_OK)
//			{
//				Error_Handler();
//			}
//
//
//
//	TIM2 		interrupt for cycle request ADCs and buttons
//	TIM3		PWM for LCD backlight
//	TIM8		encoder timer
//	SPI1		TFT ST7735
//	SPI2		MAX7219
//	USART1	crossfader RX only BaudRate = 31000
//	UART5		for bridge main assy <=> mixer assy	
//
//
//
////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//	ver. 0.15
//	-	filling peripheral functions
//	ver. 0.16
//	-	added tft blink test
//	ver. 0.19
//	-	added tft_gui.h
//	-	improved TFT library (accelerated pixel output, changed arguments in functions to uint8_t)
//	-	add FontVFD font
//	-	multiplexer scanning sequence error fixed
//	ver. 0.21
//	-	added ADCs
//	-	added level meters func
//	ver. 0.22
//	-	added hysteresis filter for adcs
//	ver. 0.23
//	-	added USART1 crossfader recieve data
//	ver. 0.24
//	- added Bluetooth power, button, led managment
//	ver. 0.25
//	- DACs with sine wave output started
//	ver. 0.26
//	-	added blinking CFX, BFX
//	ver. 0.27
//	- added link uart5 handler
//	ver. 0.28
//	- fix BFXON led invert
//	ver. 0.29
//	-	debugged SAI exchange 128 bits per sample (32 bits per channel)
//	ver. 0.31
//	-	added potenciometers data conversion to 8 bits data
//	-	added firs audio processing with pot control
//	ver. 0.33
//	-	added TRIM, CRSF, FDR, MIXN processes
//	ver. 0.35
//	-	SAI reception has been switched to floating point data
//	-	added a new arrays for audio processing
//	ver. 0.37
//	-	added EQ processeng
//	-	added level meters connecting to audio
//	ver. 0.38
//	-	added LVLtoDB func.
//	- optimized LEVEL_METER func.
//	ver. 0.39
//	- leveler integrator code changed
//	ver. 0.40
//	- improved leveler integrator coeff
//	-	LEVEL_METER func bug fix
//	ver. 0.41
//	-	link uart up to 500000 baud
//	ver. 0.42
//	-	added inair sending
//	ver. 0.43
//	- ON AIR bug fixed
//	ver. 0.44
//	-	The frequency of checking and sending the in-air status has been changed
//	-	crossfader curves added
//	- EQ ISO curve added
//	ver. 0.51
//	-	LEDs CFX blink buf fixed
//	- EQ -26dB curve added (pio curve dump)
//	-	CRSFCURVE realtime update added
//	-	CFX FITER added
//	ver. 0.53
//	- BFX ECHO, ringbuffer added
//	ver. 0.57
//	- NOISE CFX added
//	- CFX FITER curve changed
//	ver. 0.59
//	- CFX FITER: drop distorsion fixed
//	- BFX TFT_DRAW_RNGDIV added
//	- CHANGE_RNGSIZE func added
//	ver. 0.61
//	- EFFATT, ORIGATT added 
//	- Delay BFX added
//	ver. 0.63
//	-	GUI fix
//	-	BFX FLANGER first code
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint16_t FW_VER = 63;

#include "global_variables.h"

#include "spi_st7735.h"
#include "audio.h"
#include "audio_handler.h"
#include "tft_gui.h"
#include "MAX7219.h"

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
/* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART5_Init();
  MX_SAI1_Init();
  MX_SAI2_Init();
  MX_SAI3_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM8_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	MAX7219_INIT();
	HAL_Delay(2900);
	
	init_3band_state(&eql0, 850, 3500, 44100);		//850 3500 pio standart
	init_3band_state(&eqr0, 850, 3500, 44100);		//
	init_3band_state(&eql1, 850, 3500, 44100);		//
	init_3band_state(&eqr1, 850, 3500, 44100);		//
	
	
	HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_RESET);
	
	TFT_ini(128, 160);
	TFT_SetRotation();
  TFT_FillScreen(TFT_BLACK);			
	TFT_DRAW_STATIC();
		
	TFT_SetBackColor(TFT_BLACK);	
	TFT_SetTextColor(TFT_WHITE);
	
	HAL_SAI_Transmit_IT(&hsai_BlockA1, MASTER_BUS, 2);	
	HAL_SAI_Transmit_IT(&hsai_BlockA2, HP_BUS, 2);
	HAL_SAI_Receive_IT(&hsai_BlockA3, CH0IN, 2);
	HAL_SAI_Receive_IT(&hsai_BlockB3, CH1IN, 2);

	HAL_TIM_Base_Start_IT(&htim2);									//start main timer
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);				//start lcd pwm	
	TIM3->CCR2 = 750;																//set lcd pwm
		
	MAX7219_DATA[0] = 0xFF;	
	MAX7219_DATA[1] = 0xFF;
	MAX7219_DATA[2] = 0xFF;
	MAX7219_DATA[3] = 0xFF;
	MAX7219_DATA[4] = 0xFF;
	MAX7219_DATA[5] = 0xFF;
	MAX7219_DATA[6] = 0xFF;
	MAX7219_DATA[7] = 0xFF;
	
	MAX7219_UPDATE(0);
	MAX7219_UPDATE(1);
	MAX7219_UPDATE(2);
	MAX7219_UPDATE(3);
	MAX7219_UPDATE(4);
	MAX7219_UPDATE(5);
	MAX7219_UPDATE(6);
	MAX7219_UPDATE(7);		
				
	BT_LED_ON;			
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOC, PAEN_Pin, GPIO_PIN_SET);			//analog power on
	USART1->CR1 |= USART_CR1_RXNEIE_RXFNEIE; //interrupt ON for a RX enable	
	USART1->CR1 |= USART_CR1_PEIE;
	UART5->CR1 |= USART_CR1_RXNEIE_RXFNEIE; //interrupt ON for a RX enable	
	UART5->CR1 |= USART_CR1_PEIE;
	
	MAX7219_DATA[7] = 0xF0;
	MAX7219_UPDATE(7);
	
	prev_pot_8b_DEPT = pot_8b[DEPT]-5;			//force update 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	/////////////////TEMPORARY			
	if((HAL_GetTick()-temp_time)>25)
		{
		#include "INAIR.h"	

		if(BFXn==4)	//FLANGER
			{
			if(rngsize<135)
				{
				rngsize++;	
				}				
			else
				{
				rngsize = 30;	
				}	
			}		
		
		if(divdr<9)
			{
			divdr++;	
			}			
		else
			{
			if(last_bit_time<20)
				{
				last_bit_time++;	
				}
			
			if(lay==0)
				{
				TFT_SetTextColor(TFT_WHITE);
				lay = 1;	
				}
			else
				{	
				if(last_bit_time>19)
					{				
					TFT_SetTextColor(TFT_BLACK);
					}	
				else
					{
					TFT_SetTextColor(TFT_WHITE);	
					}				
				lay = 0;		
				}
				
			sprintf((char*)STR_BUFF, "%3lu", (BPM_link+50)/100);	
			TFT_SetFont(&FontVFD);					
			TFT_String(38, 77, STR_BUFF);		
						
			sprintf((char*)&STR_BUFF[48], "%02lu", B);
			TFT_SetTextColor(TFT_BLUE);
			TFT_SetFont(&Font8);		
			TFT_String(95, 110, &STR_BUFF[48]);				
			divdr = 0;	
			}	
		temp_time = HAL_GetTick();	
		}
	
	if(ADC_prev!=pot_8b[B])
		{			
		sprintf((char*)&STR_BUFF[32], "%03lu", pot_8b[B]);
		TFT_SetTextColor(TFT_WHITE);
		TFT_SetFont(&Font8);	
		TFT_String(95, 102, &STR_BUFF[32]);
		//LEVEL_METER(3, pot_8b[B]/21, 0);
		ADC_prev = pot_8b[B];	
		}			


	if(MAX7219_need_update!=0)
		{	
		if(MAX7219_need_update&0x01)
			{
			MAX7219_UPDATE(0);	
			MAX7219_need_update&=0xFE;	
			}
		if(MAX7219_need_update&0x02)
			{
			MAX7219_UPDATE(1);	
			MAX7219_need_update&=0xFD;	
			}	
		if(MAX7219_need_update&0x04)
			{
			MAX7219_UPDATE(2);	
			MAX7219_need_update&=0xFB;	
			}	
		if(MAX7219_need_update&0x08)
			{
			MAX7219_UPDATE(3);	
			MAX7219_need_update&=0xF7;	
			}				
		if(MAX7219_need_update&0x10)
			{
			MAX7219_UPDATE(4);	
			MAX7219_need_update&=0xEF;	
			}			
		if(MAX7219_need_update&0x20)
			{
			MAX7219_UPDATE(5);	
			MAX7219_need_update&=0xDF;	
			}				
		if(MAX7219_need_update&0x40)
			{
			MAX7219_UPDATE(6);	
			MAX7219_need_update&=0xBF;	
			}				
		if(MAX7219_need_update&0x80)
			{
			MAX7219_UPDATE(7);	
			MAX7219_need_update&=0x7F;	
			}		
		}	
		
	if(need_draw_fxsel>0)
		{		
		TFT_FXSELECTOR(BFXn);
		CHANGE_RNGSIZE();	
		TFT_DRAW_RNGDIV(rngdiv[BFXn]);		
		need_draw_fxsel = 0;	
		}
	if(need_draw_chsel>0)
		{		
		TFT_CHSELECTOR(CHSEL);
		need_draw_chsel = 0;	
		}
	if(need_draw_bticon>0)
		{
		TFT_DRAW_BT(BT_ENABLE);		
		need_draw_bticon = 0;	
		}			
	if(need_beat_change!=0)
		{
		if(need_beat_change==1)		//++
			{
			if(BFXn<8)
				{
				if(rngdiv[BFXn]<7)
					{					
					rngdiv[BFXn]++;
					}						
				}
			}
		else					//--
			{	
			if(BFXn<8)
				{
				if(rngdiv[BFXn]>0)
					{					
					rngdiv[BFXn]--;
					}						
				}
			}
		CHANGE_RNGSIZE();	
		TFT_DRAW_RNGDIV(rngdiv[BFXn]);	
		need_beat_change = 0;	
		}			
	if(need_draw_ms)
		{
		TFT_SetTextColor(TFT_WHITE);
		TFT_SetFont(&FontVFD);	
		sprintf((char*)STR_BUFF, "%4lu", (10*rngsize)/441);	
		TFT_String(19, 106, STR_BUFF);
		need_draw_ms = 0;	
		}	
		
	if(link_new_data)				//UART5 link handler
		{	
		if(link_urx_buf[0]==0x95 ||  link_urx_buf[0]==0x96)	//sending bpm at beats (BPM*100), 0x95-0x96 – first beat 1/4 
			{
			BPM_link = 0x100*link_urx_buf[1]+link_urx_buf[2];
			if(prev_BPM_link!=BPM_link)
				{
				CHANGE_RNGSIZE();
				prev_BPM_link = BPM_link;		
				}
			last_bit_time = 0;	
			}
		else if(link_urx_buf[0]==0x91 && link_urx_buf[1]==0x60 && link_urx_buf[2]==0x31)		//firmware version request
			{
			link_utx_buf[0] = 0x91;
			link_utx_buf[1] = FW_VER>>8;
			link_utx_buf[2] =	FW_VER%256;
			HAL_UART_Transmit(&huart5, link_utx_buf, 3, 5);	
			}
		else if(link_urx_buf[0]==0x92)		//setting table values of equalizer cutoff frequencies
			{
//				init_3band_state(&eql0, 880, 4000, 44100);
//				init_3band_state(&eqr0, 880, 4000, 44100);
//				init_3band_state(&eql1, 880, 4000, 44100);
//				init_3band_state(&eqr1, 880, 4000, 44100);
			}	
		else if(link_urx_buf[0]==0x93 && link_urx_buf[1]==0xB1)		//setting fader curve type
			{
				
			}	
		else if(link_urx_buf[0]==0x94 && link_urx_buf[1]==0xA3)		//setting headphones cue type (pre EQ or post EQ)
			{
				
			}	
		else if(link_urx_buf[0]==0x97 && link_urx_buf[1]==0xFC)		//shutdown command)
			{
			TIM3->CCR2 = 0;	
			HAL_GPIO_WritePin(GPIOC, PAEN_Pin, GPIO_PIN_RESET);			//analog power off	
			BT_LED_OFF;	
			CFXON = 0;	
			BFXON = 0;			
			CUE1ON = 0;
			CUE2ON = 0;
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
			}		
		link_new_data = 0;	
		}
		
				
	if(prev_pot_8b_CFX0!=pot_8b[CFX0])		///FILTER CH1
		{
		prev_pot_8b_CFX0 = pot_8b[CFX0];
		if(CFXON==8)
			{
			CALC_CUTF_0();		
			}	
		else if(CFXON==6)
			{
			CALC_CUTF_0();		
			}				
		}
	if(prev_pot_8b_CFX1!=pot_8b[CFX1])		///FILTER CH2
		{
		prev_pot_8b_CFX1 = pot_8b[CFX1];	
		if(CFXON==8)
			{
			CALC_CUTF_1();
			}			
		else if(CFXON==6)
			{
			CALC_CUTF_1();
			}				
		}
	if(prev_pot_8b_PRMT!=pot_8b[PRMT])		/// QFAC
		{
		prev_pot_8b_PRMT = pot_8b[PRMT];
		if(CFXON==8)
			{			
			CALC_CUTF_0();		
			CALC_CUTF_1();
			curr_TRIM_FILTER = TRIM_FILTER[pot_8b[PRMT]];	
			}
		else if(CFXON==6)
			{
			curr_TRIM_FILTER = TRIM_FILTER[128];	
			}			
		}	
	if(prev_pot_8b_DEPT!=pot_8b[DEPT])		/// BEAT FX
		{	
		prev_pot_8b_DEPT = pot_8b[DEPT];
		ORIGATT = ORIG_ATT[pot_8b[DEPT]];	
		EFFATT = EFF_ATT[pot_8b[DEPT]];		
		}

	if(prev_CFXON!=CFXON)			//calculate parameters for a new effect
		{
		prev_CFXON = CFXON;
		if(CFXON==8)
			{			
			CALC_CUTF_0();		
			CALC_CUTF_1();
			curr_TRIM_FILTER = TRIM_FILTER[pot_8b[PRMT]];		
			}	
		else if(CFXON==6)
			{			
			CALC_CUTF_0();		
			CALC_CUTF_1();
			curr_TRIM_FILTER = TRIM_FILTER[128];		
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_SAI1
                              |RCC_PERIPHCLK_SAI2|RCC_PERIPHCLK_SAI3;
  PeriphClkInitStruct.PLL2.PLL2M = 10;
  PeriphClkInitStruct.PLL2.PLL2N = 144;
  PeriphClkInitStruct.PLL2.PLL2P = 6;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_1;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 451;
  PeriphClkInitStruct.PLL3.PLL3P = 4;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 2;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL3;
  PeriphClkInitStruct.Sai23ClockSelection = RCC_SAI23CLKSOURCE_PLL3;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

#include "uarts_handler.h"
#include "timer_handler.h"
#include "encoder_handler.h"


	

	
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
