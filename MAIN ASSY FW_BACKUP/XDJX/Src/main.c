/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Product: XDJ-X main assy
//
//		Generated with: STM32CubeMX 6.10.0
//		Firmware Package Name and Version STM32Cube FW_H7 V1.11.1
//
//		After regenerate code:
//
//		replace file fmc.c
//		replace file \Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_hal_dma2d.c
//		replace file \Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_hal_dma2d.h
//		replace file \Src\fatfs_platform.c
//		replace file \Drivers\STM32H7xx_HAL_Driver\Src\stm32h7xx_hal_dma.c  !!!modify file
//		replace file \Src\ltdc.c
//
//
//
//
//
//
//		TIM8 - PWM display backlight
//		UART4 - debug uart
//		TIM2 - SPI DMA timer 400Hz
//		SPI1 - SPI EEPROM onboard
//		SPI2 - decks DMA data transfer
//		USART1 - mixer assy sync transfer UART
//		TIM3 - encoder
//		UART7 - power manager usart
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	ver. 0.1
//		- start ltdc
//	ver. 0.15
//		- added display 233*480
//	ver. 0.17
//	ver. 0.19
//		- work with CWX3970
//	ver. 0.23
//		- improved lcd lib to 233 pixels
//	ver. 0.25
//		- improved lcd lib
//	ver. 0.27
//		-	added SDIO and SAI1
//	ver. 0.29
//		- improved lcd lib (inverted signal pixel clock)
//		- added pwm backlight control
//	ver. 0.30
//		-	SAI pins remaped
//		-	add test dynamic waveform
//	ver. 0.33
//		-	improved DMA2D functions and productivity
//		-	added memcpy for waveform layer frame
//		-	Changed file stm32h7xx_hal_dma2d.c
//	ver. 0.35
//		-	added DMA framebuffer tranfer
//		-	added 14D font
//	ver. 0.36
//		- changed dynamic wf to 18-1-18 amplitude
//	ver. 0.37
//		-	added global_variables.h
//		-	added audio.h
//		-	added audio_handler.h
//	ver. 0.40
//		- regenerated code
//		- SAI switched to slave mode, 2 lines data
//		- added buttons and leds init
//		- added encoder timer init
//		-	added usart for power manager
//		- added SPI DMA init
//		- added SPI timer 400hz
//		- added SPI for EEPROM
//		- added QSPI
//	ver. 0.44
//		-	audio transfer starting
//		-	DMA deck transfer staring
//	ver. 0.45
//		-	added Power manager uart handler 
//		-	added Debug uart handler 
//		-	added Mixer uart handler
//	ver. 0.46
//		-	added SPI DMA packages
//	ver. 0.47
//		-	SPI DMA changes
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	
	
	
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "dma2d.h"
#include "fatfs.h"
#include "ltdc.h"
#include "quadspi.h"
#include "sai.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "global_variables.h"
#include "audio.h"
#include "audio_handler.h"
#include "deck_transfer.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include "stm32746g_discovery_lcd.h"

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */
void RedrawWaveforms(uint32_t position);
void intDRAW_WAVEFORM_FRAME(uint32_t position);						//internal function for redraw waveform



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
  MX_DMA_Init();
  MX_UART4_Init();
  MX_DMA2D_Init();
  MX_LTDC_Init();
  MX_SAI1_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_FMC_Init();
  MX_TIM8_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_QUADSPI_Init();
  MX_SPI2_Init();
  MX_UART7_Init();
  /* USER CODE BEGIN 2 */
	BSP_SD_Init();
	USART1->CR1 |= USART_CR1_RXNEIE_RXFNEIE; //interrupt ON for a RX enable	
	USART1->CR1 |= USART_CR1_PEIE;
	UART4->CR1 |= USART_CR1_RXNEIE_RXFNEIE; //interrupt ON for a RX enable	
	UART4->CR1 |= USART_CR1_PEIE;
	UART7->CR1 |= USART_CR1_RXNEIE_RXFNEIE; //interrupt ON for a RX enable	
	UART7->CR1 |= USART_CR1_PEIE;	
	
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);	

	HAL_SAI_Transmit_IT(&hsai_BlockA1, SAMPLE, 2);
	HAL_SAI_Transmit_IT(&hsai_BlockB1, SAMPLE, 2);

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
	BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER_2);

	sprintf((char*)U_TX_DATA, "%01lu error\n\r", i);	
	HAL_UART_Transmit(&huart4, U_TX_DATA, 9, 5);	

	i = 0;
	sprintf((char*)U_TX_DATA, "All init\n\r");	
	HAL_UART_Transmit(&huart4, U_TX_DATA, 10, 5);	

	BSP_LCD_SetTransparency(0, 255);
	BSP_LCD_SetTransparency(1, 255);
	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	
	HAL_Delay(3000);
	TIM8->CCR1 = 450;
	HAL_GPIO_WritePin(BACKLIGHT_EN_GPIO_Port, BACKLIGHT_EN_Pin, GPIO_PIN_SET);
	
	HAL_TIM_Base_Start_IT(&htim2);									//start deck transfer timer

	res = f_mount(&FAT, "0", 1);
	if (res!=FR_OK)
		{
		sprintf((char*)U_TX_DATA, "%01lu error\n\r", res);	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 9, 5);	
			
		sprintf((char*)U_TX_DATA, "SD not mounted\n\r");	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 16, 5);		
		}
	else
		{
		res = f_open(&file, path_pic, FA_READ);
		if (res != FR_OK)
			{
			sprintf((char*)U_TX_DATA, "Picture not opened\n\r");	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 20, 5);		
			}		
		else
			{
			sprintf((char*)U_TX_DATA, "Picture file open\n\r", i);	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 17, 5);		
			HAL_Delay(500);
			f_lseek(&file, 54);			
			f_read(&file, parcser_adress, 223680, &nbytes);	
							
			for(i=0;i<111840;i++)	
				{	
				parcser_adress[i]|=0x8000;
				}		
			f_close(&file);	
			}	
		}
		
	for(i=0;i<24980;i++)			//31->18
		{	
		a = WFORMDYNAMIC[i]&0x1F;
		a*= 153;
		a>>=8;
		WFORMDYNAMIC[i]&=0xE0;
		WFORMDYNAMIC[i]|=a; 	
		}
	
			
			
	BSP_LCD_SelectLayer(0);
	
	j = 0;
			

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	RedrawWaveforms(play_adr/294);	

		
		
//	if(i<479)
//		{
//		i++;	
//		}			
//	else
//		{
//		i = 0;	
//		}			
//	BSP_LCD_SetTextColor(LCD_COLOR_TRANSPARENT);
//	BSP_LCD_DrawLine(i, 0, i, 232);		
//	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
//	BSP_LCD_DrawLine((i+140)%480, 0, (i+140)%480, 232);		
//	HAL_Delay(3);
		
		
	if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin)==0 && button_pressed==0)
		{
		if(path_pic[6]<57)
			{
			path_pic[6]++;	
			}
		else
			{
			path_pic[6] = 48;
			}				
		res = f_open(&file, path_pic, FA_READ);
		if (res != FR_OK)
			{
			sprintf((char*)U_TX_DATA, "Picture not opened\n\r");	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 20, 5);		
			}		
		else
			{
			sprintf((char*)U_TX_DATA, "Picture file open\n\r");	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);		
			f_lseek(&file, 54);			
			f_read(&file, parcser_adress, 223680, &nbytes);	
			for(i=0;i<111840;i++)	
				{	
				parcser_adress[i]|=0x8000;
				}
			f_close(&file);		
			}		
			
//			if(j<14)
//				{
//				j++;	
//				}
//			else
//				{
//				j = 0;	
//				}
//			for(i=0;i<111840;i++)	
//				{	
//				parcser_adress[i] = 0x0001<<j;
//				}
//			for(i=0;i<111840;i++)	
//				{	
//				parcser_adress[i]|=0x8000;
//				}
		button_pressed = 1;	
		}
	else if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin)==1 && button_pressed==1)
		{
		button_pressed = 0;	
		}		
		
		

//	sprintf((char*)U_TX_DATA, "Start fill 0xAAAA\n\r");	
//	HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);		
//	for(i=0;i<0x1000000;i++)	
//		{
//		parcser_adress[i] = 0xAAAA;	
//		}		
//	sprintf((char*)U_TX_DATA, "Check fill 0xAAAA\n\r");	
//	HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);		
//	for(i=0;i<0x1000000;i++)	
//		{
//		if(parcser_adress[i]!=0xAAAA)
//			{
//			sprintf((char*)U_TX_DATA, "Error data\n\r");	
//			HAL_UART_Transmit(&huart4, U_TX_DATA, 12, 5);					
//			i = 0xFFFFFFF0;	
//			}			
//		}		

//	sprintf((char*)U_TX_DATA, "Start fill 0x5555\n\r");	
//	HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);		
//	for(i=0;i<0x1000000;i++)	
//		{
//		parcser_adress[i] = 0x5555;	
//		}		
//	sprintf((char*)U_TX_DATA, "Check fill 0x5555\n\r");	
//	HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);		
//	for(i=0;i<0x1000000;i++)	
//		{
//		if(parcser_adress[i]!=0x5555)
//			{
//			sprintf((char*)U_TX_DATA, "Error data\n\r");	
//			HAL_UART_Transmit(&huart4, U_TX_DATA, 12, 5);					
//			i = 0xFFFFFFF0;	
//			}			
//		}		
		
//	sprintf((char*)U_TX_DATA, "Start fill 0x0000\n\r");	
//	HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);		
//	for(i=0;i<0x1000000;i++)	
//		{
//		parcser_adress[i] = 0x0000;	
//		}		
//	sprintf((char*)U_TX_DATA, "Check fill 0x0000\n\r");	
//	HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);		
//	for(i=0;i<0x1000000;i++)	
//		{
//		if(parcser_adress[i]!=0x0000)
//			{
//			sprintf((char*)U_TX_DATA, "Error data\n\r");	
//			HAL_UART_Transmit(&huart4, U_TX_DATA, 12, 5);					
//			i = 0xFFFFFFF0;	
//			}			
//		}		
//		
//	sprintf((char*)U_TX_DATA, "Start fill 0xFFFF\n\r");	
//	HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);		
//	for(i=0;i<0x1000000;i++)	
//		{
//		parcser_adress[i] = 0xFFFF;	
//		}		
//	sprintf((char*)U_TX_DATA, "Check fill 0xFFFF\n\r");	
//	HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);		
//	for(i=0;i<0x1000000;i++)	
//		{
//		if(parcser_adress[i]!=0xFFFF)
//			{
//			sprintf((char*)U_TX_DATA, "Error data\n\r");	
//			HAL_UART_Transmit(&huart4, U_TX_DATA, 12, 5);					
//			i = 0xFFFFFFF0;	
//			}			
//		}	

//	errs = 0;
//	sprintf((char*)U_TX_DATA, "Start fill Addres\n\r");	
//	HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);		
//	for(i=0;i<0x1000000;i++)	
//		{
//		parcser_adress[i] = (i%55000);	
//		}		
//	sprintf((char*)U_TX_DATA, "Check fill Addres\n\r");	
//	HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);		
//	for(i=0;i<0x1000000;i++)	
//		{
//		if(parcser_adress[i]!=(i%55000))
//			{
//			errs++;	
//			}			
//		}	
//	if(errs!=0)
//		{
//		sprintf((char*)U_TX_DATA, "Errors %08lu\n\r", errs);	
//		HAL_UART_Transmit(&huart4, U_TX_DATA, 17, 5);
//		}
//	else
//		{
//		sprintf((char*)U_TX_DATA, "Test OK\n\r", errs);	
//		HAL_UART_Transmit(&huart4, U_TX_DATA, 9, 5);	
//		}
//		
//	while(1)
//		{
//			
//		}		
//		
//	HAL_Delay(500);			
		
		
	#include "pm_uart_handler.h"
	#include "mixer_uart_handler.h"
	#include "debug_uart_handler.h"
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
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC|RCC_PERIPHCLK_SAI1
                              |RCC_PERIPHCLK_SPI2|RCC_PERIPHCLK_SPI1;
  PeriphClkInitStruct.PLL2.PLL2M = 25;
  PeriphClkInitStruct.PLL2.PLL2N = 271;
  PeriphClkInitStruct.PLL2.PLL2P = 2;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
#include "uarts_IRQs.h"

////////////////////////////////////////////////////////////////////////
//
//Function redraw bar on static waveform and redraw dynamic waveform
//position = 1/150 sec
void RedrawWaveforms(uint32_t position)
	{
	if(position>all_long)
		{
		return;	
		}
	uint32_t clock_pos;	

	if(REMAIN_ENABLE)
		{
		clock_pos = all_long - position;	
		}	
	else
		{
		clock_pos	= position;
		}
		
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
	BSP_LCD_SetFont(&Font18D);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

//	if(forcibly_redraw==1)
//		{
//		Prev10m = 0xFF;
//		Prev1m = 0xFF;
//		Prev10s = 0xFF;
//		Prev1s = 0xFF;
//		Prev10f = 0xFF;
//		Prev1f = 0xFF;
//		PrevHf = 0xFF;				
//		}

	if(Prev10m != (clock_pos/90000)%10)			
		{
		Prev10m = (clock_pos/90000)%10;	
		sprintf((char *)Buf , "%0lu", Prev10m);				//10 Min	
		BSP_LCD_DisplayStringAt(306, 168, Buf, LEFT_MODE);	
		}
	if(Prev1m != (clock_pos/9000)%10)			
		{
		Prev1m = (clock_pos/9000)%10;	
		sprintf((char *)Buf , "%0lu", Prev1m);				//1 Min	
		BSP_LCD_DisplayStringAt(322, 168, Buf, LEFT_MODE);				
		}
	if(Prev10s != (clock_pos/1500)%6)
		{
		Prev10s = (clock_pos/1500)%6;
		sprintf((char *)Buf , "%0lu", Prev10s);				//10 Sec	
		BSP_LCD_DisplayStringAt(352, 168, Buf, LEFT_MODE);		
		}
	if(Prev1s != (clock_pos/150)%10)
		{
		Prev1s = (clock_pos/150)%10;
		sprintf((char *)Buf , "%0lu", Prev1s);				//1 Sec	
		BSP_LCD_DisplayStringAt(368, 168, Buf, LEFT_MODE);
		}		
	if(Prev10f != ((clock_pos/2)%75)/10)
		{
		Prev10f = ((clock_pos/2)%75)/10;
		BSP_LCD_SetFont(&Font14D);	
		sprintf((char *)Buf , "%0lu", Prev10f);				//10 F	
		BSP_LCD_DisplayStringAt(394, 172, Buf, LEFT_MODE);
		}		
	if(Prev1f != ((clock_pos/2)%75)%10)
		{
		Prev1f = ((clock_pos/2)%75)%10;
		BSP_LCD_SetFont(&Font14D);	
		sprintf((char *)Buf , "%0lu", Prev1f);				//1 F	
		BSP_LCD_DisplayStringAt(407, 172, Buf, LEFT_MODE);	
		}
	if(PrevHf != clock_pos%2)
		{
		PrevHf = clock_pos%2;
		BSP_LCD_SetFont(&Font14D);		
		if(PrevHf%2==1)
			{
			sprintf((char*)Buf, "%s", "5");	
			}
		else
			{
			sprintf((char*)Buf, "%s", "0");	
			}
		BSP_LCD_DisplayStringAt(424, 172, Buf, LEFT_MODE);					
		}		
		
	//DrawStaticWFM(position*399/all_long);	
		
	position = position/DynamicWaveformZOOM;					//zoom correction	
		
	if(position!=PreviousPositionDW)
		{
		PreviousPositionDW = position;
		if(VisibleLayer==0)
			{
			BSP_LCD_SelectLayer(1);	
			intDRAW_WAVEFORM_FRAME(position);		
			//memcpy(&parcser_adress[114240], &parcser_adress[144960], 48960);							
			HAL_DMA_GO(&hdma_memtomem_dma1_stream0, 0xC0049200, 0xC003D680, 8880);

				
			HAL_DMA_Poll(&hdma_memtomem_dma1_stream0, HAL_DMA_FULL_TRANSFER, 8);	
			BSP_LCD_SetTransparency(1, 255);		//верхний слой виден
			BSP_LCD_SelectLayer(0);
			VisibleLayer = 1;
			}
		else
			{
			BSP_LCD_SelectLayer(0);
			intDRAW_WAVEFORM_FRAME(position);	
			//memcpy(&parcser_adress[2400], &parcser_adress[33120], 48960);
			HAL_DMA_GO(&hdma_memtomem_dma1_stream0, 0xC0012840, 0xC0006CC0, 8880);	
			HAL_DMA_Poll(&hdma_memtomem_dma1_stream0, HAL_DMA_FULL_TRANSFER, 8);
			BSP_LCD_SetTransparency(1, 0);		//верхний не слой виден
			VisibleLayer = 0;	
			}
		}
	return;	
	}


//////////////////////////////////////////////	
//	
//	internal function for redraw waveform	
//	
void intDRAW_WAVEFORM_FRAME(uint32_t position)
	{
	uint16_t i, adr, BG_COLOR;
	uint16_t u, x; 	
	uint8_t	j;
	x = 0;
	u = 0;	
				
//	if(position>=200)
//		{
//		while((BEATGRID[u]-(BEATGRID[u]%DynamicWaveformZOOM))<(DynamicWaveformZOOM*(position-200)))
//			{
//			u++;	
//			}	
//		}	
	
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);	
//	BSP_LCD_FillRect(36, 49, 408, 5);
//	BSP_LCD_FillRect(36, 136, 408, 5);				
//	BSP_LCD_FillRect(40, 54, 400, 5);
//	BSP_LCD_FillRect(40, 131, 400, 5);
			
	for(i=0;i<480;i++)
		{	
		adr = DynamicWaveformZOOM*(i+position-240);	
		BG_COLOR = LCD_COLOR_BLACK;	

//		if(adr<=all_long)
//			{
//			if((CUE_ADR-(CUE_ADR%DynamicWaveformZOOM))==adr) //Draw CUE triangle
//				{
//				BSP_LCD_SetTextColor(CUE_COLOR);
//				FillTriangle(i+36, i+44, i+40, 170, 170, 166);
//				}
				
//			if((BEATGRID[u+x]-(BEATGRID[u+x]%DynamicWaveformZOOM))==adr)	
//				{
//				if(((u+x)%4)==((1-GRID_OFFSET)&0x03))					//red grid
//					{
//					ForceDrawVLine(i+40, 84, 5, LCD_COLOR_RED);
//					ForceDrawVLine(i+40, 161, 5, LCD_COLOR_RED);
//					}
//				else if(DynamicWaveformZOOM<8)			//white grid
//					{	
//					ForceDrawVLine(i+40, 84, 5, LCD_COLOR_WHITE);
//					ForceDrawVLine(i+40, 161, 5, LCD_COLOR_WHITE);		
//					}
//				x++;	
//				}	
//			}

		if(i==239)																		///you can optimize 1 raz draw red line
			{
			//bars = u+x;		
			}
		else if(i==240)
			{
			}
		else
			{
			if(adr<=all_long)
				{
				if(DynamicWaveformZOOM==1)
					{	
					ForceDrawVLine(i, 97-(WFORMDYNAMIC[adr]&0x1F), 1+2*(WFORMDYNAMIC[adr]&0x1F), COLOR_MAP[0][WFORMDYNAMIC[adr]>>5]);		//124-125px center	
					ForceDrawVLine(i, 79, 18-(WFORMDYNAMIC[adr]&0x1F), BG_COLOR);								
					ForceDrawVLine(i, 98+(WFORMDYNAMIC[adr]&0x1F), 18-(WFORMDYNAMIC[adr]&0x1F), BG_COLOR);			
					}
				else 	
					{
					uint8_t amplitude = (WFORMDYNAMIC[adr]&0x1F);
					if(amplitude>18)			//may delete check, when [adr] not owerflow value from massive WFORMDYNAMIC
						{
						amplitude = 18;	
						}						
					uint8_t color = (WFORMDYNAMIC[adr]>>5);
					for(j=0;j<(DynamicWaveformZOOM-1);j++)
						{
						if((WFORMDYNAMIC[adr+j+1]&0x1F)>amplitude)
							{
							amplitude	= (WFORMDYNAMIC[adr+j+1]&0x1F);
							if(amplitude>13)
								{
								color = (WFORMDYNAMIC[adr+j+1]>>5);
								}
							}
						}		
					ForceDrawVLine(i, 97-amplitude, 1+2*amplitude, COLOR_MAP[0][color]);		//124-125px center
					ForceDrawVLine(i, 79, 18-amplitude, BG_COLOR);	
					ForceDrawVLine(i, 98+amplitude, 18-amplitude, BG_COLOR);		
					}
				}
			else
				{
				ForceDrawVLine(i, 79, 37, LCD_COLOR_BLACK);		
				}	
			}			
		}
	ForceDrawVLine(239, 17, 109, LCD_COLOR_WHITE);	
	ForceDrawVLine(240, 17, 109, LCD_COLOR_WHITE);		
	return;	
	}	
	
	

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
