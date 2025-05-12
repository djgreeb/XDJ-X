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
//		file main.c:
//				//  /* Enables the MPU */
//					//HAL_MPU_Enable(MPU_HFNMI_PRIVDEF);
//
//
//					MPU_InitStruct.Enable = MPU_REGION_ENABLE;
//					MPU_InitStruct.Number = MPU_REGION_NUMBER1;
//					MPU_InitStruct.BaseAddress = 0x90000000;
//					MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
//					MPU_InitStruct.SubRegionDisable = 0x0;
//					MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
//					MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//					MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
//					MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
//					MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
//					MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
//
//					HAL_MPU_ConfigRegion(&MPU_InitStruct);
//
//					/** Initializes and configures the Region and the memory to be protected
//					*/
//					HAL_MPU_ConfigRegion(&MPU_InitStruct);
//					/* Enables the MPU */
//					HAL_MPU_Enable(MPU_HFNMI_PRIVDEF);
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
//	ver. 0.49
//		- fixed SPI DMA: deckRbuf and deckTbuf added to D-cache exception (enabled mpu address area)  
//	ver. 0.50
//		- expand tx buffer to 160 bytes
//	ver. 0.51
//		-	SPI DMA package changed from 20 bytes to 18 bytes
//	ver. 0.52
//		-	SPI DMA fixed
//	ver. 0.53
//		-	debugged SAI exchange 128 bits per sample (32 bits per channel)
//	ver. 0.54
//		- added test sine 882Hz
//	ver. 0.55
//		- added shor audio
//		- added double libruary QSPI MemMap need debug QSPI: HardFault in stm32h7xx_it.c
//	ver. 0.61
//		-	debugged QSPI MemMap
//		- added 2 audio tracks to QSPI and routed to SAI outputs
//	ver. 0.62
//		-	optimized libs
//	ver. 0.63
//		-	added text
//	ver. 0.67
//		- fixed QSPI reading bug where data was shifted by 2 bytes. sCommand.DummyCycles = 6;
//		- added rekordbox.h
//	ver. 0.69
//		-	bug fixed: path_ANLZ expanded to 46 bytes, for final 0 
//	ver. 0.70
//		-	added static wf and hcue, memory markers
//	ver. 0.72
//		- added hcue icons
//		-	added hcue icons and mem markers on dyn.wf
//	ver. 0.73
//		- gui optimization
//	ver. 0.74
//		- added audio playback in deck A
//	ver. 0.75
//		- changed packet length to 8 bytes (7+CRC)
//	ver. 0.76
//		-	added debug jog cnt
//	ver. 0.77
//		-	added FIR in audio handler
//		-	SAI transmission has been switched to floating point data
//	ver. 0.78
//		-	added PLAY, CUE, JOG code
//	ver. 0.79
//		- bug fixed in debug mode
//	ver. 0.81
//		-	added SLIP, TEMPO, jog display code
//	ver. 0.82
//		-	minor fixes
//	ver. 0.83
//	-	added rev butt code
//	ver. 0.85
//	-	added pitch pot control deck A
//	ver. 0.87
//	- added jog ring
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
char FIRMWARE_VERSION[] = "0.87";	
#define DEBUG_UART_EN				//sending work status to uart
#include "global_variables.h"
#include "audio.h"
#include "CWX3970.h"
#include "qspi_const.h"
#include "gui.h"
#include "audio_handler.h"
#include "deck_transfer.h"
#include "rekordbox.h"


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


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
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

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();
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

	CSP_QUADSPI_Init();
	CSP_QSPI_EnableMemoryMappedMode();


	BSP_SD_Init();
	USART1->CR1 |= USART_CR1_RXNEIE_RXFNEIE; //interrupt ON for a RX enable	
	USART1->CR1 |= USART_CR1_PEIE;
	UART4->CR1 |= USART_CR1_RXNEIE_RXFNEIE; //interrupt ON for a RX enable	
	UART4->CR1 |= USART_CR1_PEIE;
	UART7->CR1 |= USART_CR1_RXNEIE_RXFNEIE; //interrupt ON for a RX enable	
	UART7->CR1 |= USART_CR1_PEIE;	
	
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);	

	HAL_SAI_Transmit_IT(&hsai_BlockA1, (uint8_t*)&deckAout[0], 2);
	HAL_SAI_Transmit_IT(&hsai_BlockB1, (uint8_t*)&deckAout[0], 2);

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
	BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER_2);
	
	#if defined(DEBUG_UART_EN)	
	sprintf((char*)U_TX_DATA, "%01lu error\n\r", i);	
	HAL_UART_Transmit(&huart4, U_TX_DATA, 9, 5);	
	#endif
	
	i = 0;
	
	#if defined(DEBUG_UART_EN)	
	sprintf((char*)U_TX_DATA, "All init\n\r");	
	HAL_UART_Transmit(&huart4, U_TX_DATA, 10, 5);
	#endif

	BSP_LCD_SetTransparency(0, 255);
	BSP_LCD_SetTransparency(1, 255);
	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_BLACK);
			
	for(j=0;j<8;j++)
		{
		deckTbuf[j][0] = 24+j;	
		deckTbuf[j][1] = 0x20;
		deckTbuf[j][2] = 0x30;
		deckTbuf[j][3] = 0x40;
		deckTbuf[j][4] = 135;	//play pos
		deckTbuf[j][5] = 85;	
		deckTbuf[j][6] = 85;
		deckTbuf[j][7] = 0xFF;
		deckTbuf[j][8] = 24+j;	
		deckTbuf[j][9] = 0x20;
		deckTbuf[j][10] = 0x30;
		deckTbuf[j][11] = 0x40;
		deckTbuf[j][12] = 135;	//play pos	
		deckTbuf[j][13] = 85;	
		deckTbuf[j][14] = 85;
		deckTbuf[j][15] = 0xFF;	
		}
	deckTbuf[3][0]&= 0xEF;	
	deckTbuf[3][8]&= 0xEF;
	deckTbuf[5][0]&= 0xEF;	
	deckTbuf[5][8]&= 0xEF;

	SET_JOG_COLOR(0, JOG_WHITE);	
	SET_JOG_COLOR(1, JOG_WHITE);	
		
	HAL_TIM_Base_Start_IT(&htim2);									//start deck transfer timer
		
	HAL_Delay(2000);						//need decrees delay in deck assy. spi dma not starting?
	TIM8->CCR1 = 450;
	HAL_GPIO_WritePin(BACKLIGHT_EN_GPIO_Port, BACKLIGHT_EN_Pin, GPIO_PIN_SET);	
	DrawLOGO();	
		
	#if defined(DEBUG_UART_EN)	
	sprintf((char*)U_TX_DATA, "Main assy: ");	
	HAL_UART_Transmit(&huart4, U_TX_DATA, 11, 5);	
	HAL_UART_Transmit(&huart4, FIRMWARE_VERSION, 4, 5);
	sprintf((char*)U_TX_DATA, "\n\r");	
	HAL_UART_Transmit(&huart4, U_TX_DATA, 2, 5);
	sprintf((char*)U_TX_DATA, "Deck 1 UCOM: ");	
	HAL_UART_Transmit(&huart4, U_TX_DATA, 13, 5);
	sprintf((char*)U_TX_DATA, "%01lu.%02lu\n\r", (deckRbuf[4]&0x7F)/100, (deckRbuf[4]&0x7F)%100);
	HAL_UART_Transmit(&huart4, U_TX_DATA, 6, 5);
	sprintf((char*)U_TX_DATA, "Deck 2 UCOM: ");	
	HAL_UART_Transmit(&huart4, U_TX_DATA, 13, 5);
	sprintf((char*)U_TX_DATA, "%01lu.%02lu\n\r", (deckRbuf[12]&0x7F)/100, (deckRbuf[12]&0x7F)%100);
	HAL_UART_Transmit(&huart4, U_TX_DATA, 6, 5);	
	#endif	
			
	res = f_mount(&FAT, "0", 1);
	if (res!=FR_OK)
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "%01lu error\n\r", res);	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 9, 5);		
		sprintf((char*)U_TX_DATA, "SD not mounted\n\r");	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 16, 5);
		#endif	
		}
	else
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Drive mounted\n\r");	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 15, 5);	
		#endif		
		res = f_open(&file, path_pic, FA_READ);
		if (res != FR_OK)
			{
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "Picture not opened\n\r");	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 20, 5);	
			#endif	
			}		
		else
			{
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "Picture file open\n\r", i);	
			HAL_UART_Transmit(&huart4, U_TX_DATA, 19, 5);	
			#endif		
			HAL_Delay(500);
			f_lseek(&file, 54);			
			f_read(&file, fbuf, 223680, &nbytes);	
							
			for(i=0;i<111840;i++)	
				{	
				fbuf[i]|=0x8000;
				}		
			f_close(&file);	
			}	
		}
			
		
	TOTAL_TRACKS = DATABASE_PARSER();
	if(TOTAL_TRACKS==0xFFFE)
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Rekordbox database not found!\n\r");	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 31, 5);	
		#endif		
		SD_STATUS = SD_NEED_REPLACE;
		}
	else if(TOTAL_TRACKS==0xFFFF)
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Rekordbox database has more than 512 tracks.\n\r");	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 46, 5);	
		#endif	
		SD_STATUS = SD_NEED_REPLACE;	
		}
	else if(TOTAL_TRACKS==0xFFF1)
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Playlists structure error.\n\r");	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 28, 5);	
		#endif	
		SD_STATUS = SD_NEED_REPLACE;	
		}	
	else if(TOTAL_TRACKS==0)
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Rekordbox database dont't have a tracks.\n\r");	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 42, 5);	
		#endif	
		SD_STATUS = SD_NEED_REPLACE;	
		}	
	else
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Rekordbox database have a %03lu tracks\n\r", TOTAL_TRACKS);	
		HAL_UART_Transmit(&huart4, U_TX_DATA, 38, 5);	
		#endif
		SD_STATUS = SD_MOUNTED;	
		}		


		
	BSP_LCD_SelectLayer(0);
	
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	sprintf((char*)Buf, "%s", "HUGEL, Topic, Arash");
	BSP_LCD_DisplayStringAt(5, 124, Buf, TRANSPARENT_MODE);	
	sprintf((char*)Buf, "%s", "I Adore You (Extended");	
	BSP_LCD_DisplayStringAt(5, 141, Buf, TRANSPARENT_MODE);	
	sprintf((char*)Buf, "%s", "Zerb x Sofiya Nzau");		
	BSP_LCD_DisplayStringAt(245, 124, Buf, TRANSPARENT_MODE);
	sprintf((char*)Buf, "%s", "Mwaki (Franky Wah)");	
	BSP_LCD_DisplayStringAt(300, 141, Buf, TRANSPARENT_MODE);			
	j = 0;
			
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	RedrawWaveforms(play_adr/294);

	if(end_adr_valid_data<128)
		{
		f_read(&file, PCM[end_adr_valid_data][0], 32768, &nbytes);
		//DrawCueMarker(((end_adr_valid_data*11145)/all_long));				////-----------------
		end_adr_valid_data++;	
		}
	else if((end_adr_valid_data<((play_adr>>13)+42)) && (filling_step==0 || filling_step==6))									//filling the buffer forward
		{
		if(filling_step==6)
			{
			f_lseek(&file, ((32768*end_adr_valid_data)+44));	
			filling_step = 0;	
			}
		f_read(&file, PCM[end_adr_valid_data&0x7F][0], 32768, &nbytes);
		//DrawCueMarker(((end_adr_valid_data*11145)/all_long));			////-----------------
		end_adr_valid_data++;
		if((end_adr_valid_data-start_adr_valid_data)>128)
			{
			start_adr_valid_data = end_adr_valid_data-128;	
			}
		}
	else if(((end_adr_valid_data>((play_adr>>13)+86) || ((end_adr_valid_data-start_adr_valid_data)<124)) && start_adr_valid_data>3) || (filling_step!=0 && filling_step!=6))					//filling the buffer back
		{
		if(filling_step==0 || filling_step==6)
			{
			if((end_adr_valid_data-start_adr_valid_data)>127)	
				{
				end_adr_valid_data = start_adr_valid_data+124;	
				}	
			start_adr_valid_data-= 4;	
			f_lseek(&file, ((32768*(start_adr_valid_data))+44));
			filling_step = 1;	
			}
		else if(filling_step==1)
			{
			f_read(&file, PCM[start_adr_valid_data&0x7F][0], 32768, &nbytes);
			filling_step = 2;	
			}
		else if(filling_step==2)
			{
			f_read(&file, PCM[(start_adr_valid_data+1)&0x7F][0], 32768, &nbytes);
			filling_step = 3;	
			}
		else if(filling_step==3)
			{
			f_read(&file, PCM[(start_adr_valid_data+2)&0x7F][0], 32768, &nbytes);
			filling_step = 4;	
			}
		else if(filling_step==4)
			{
			f_read(&file, PCM[(start_adr_valid_data+3)&0x7F][0], 32768, &nbytes);
			filling_step = 5;	
			}
		else if(filling_step==5)
			{
			//DrawCueMarker(((start_adr_valid_data*11145)/all_long));											////-----------------
			filling_step = 6;		
			}
		}	
		
		
		
	if(track_need_load!=0)
		{
		if(track_need_load==1)				//load next track
			{				
			if(track_play_now==TOTAL_TRACKS)
				{
				track_play_now = 1;	
				}
			else
				{
				track_play_now++;	
				}	
			}
		PREPARE_LOAD_TRACK(track_play_now, track_play_now);	
		track_need_load = 0;	
		}	
		
	if(CUE_OPERATION==CUE_NEED_SET)
		{
//		if(QUANTIZE && dSHOW==WAVEFORM)				//add calculate bars in background process  /uncomment when bars calculated
//			{
//			if(((play_adr/294)>(BEATGRID[bars-1]+((BEATGRID[bars] - BEATGRID[bars-1])/2))) || bars==0)	
//				{
//				SET_CUE(BEATGRID[bars]);	
//				}
//			else
//				{
//				SET_CUE(BEATGRID[bars-1]);		
//				}				
//			}
//		else
			{
			SET_CUE(play_adr/294);	
			}
		CUE_OPERATION = 0;	
		}
	else if(CUE_OPERATION==CUE_NEED_CALL)
		{
		CALL_CUE();
		CUE_OPERATION = 0;			
		}
	else if(CUE_OPERATION==MEMORY_NEED_NEXT_SET)
		{
		if(number_of_memory_cue_points>0)
			{
			JJ=0;
			while(MEMORY_adr[0][JJ]<=(play_adr/294) && (JJ<number_of_memory_cue_points-1))
				{
				JJ++;	
				}
			if((play_adr/294)<MEMORY_adr[0][JJ])
				{
				if(loop_active)				//deactivate loop
					{
					loop_active = 0;
					LOOP_OUT = 0;			
					}
				SET_MEMORY_CUE_1(MEMORY_adr[0][JJ]);
				CUE_OPERATION = MEMORY_NEED_SET_PART2;	
				}
			else
				{
				CUE_OPERATION = 0;	
				}
			}
		else
			{
			CUE_OPERATION = 0;	
			}
		}	
	else if(CUE_OPERATION==MEMORY_NEED_PREVIOUS_SET)
		{
		if(number_of_memory_cue_points>0)
			{
			JJ = number_of_memory_cue_points-1;
			while(MEMORY_adr[0][JJ]>=(play_adr/294) && (JJ>0))
				{
				JJ--;	
				}
			if((play_adr/294)>MEMORY_adr[0][JJ])
				{
				if(loop_active)				//deactivate loop
					{
					loop_active = 0;
					LOOP_OUT = 0;			
					}	
				SET_MEMORY_CUE_1(MEMORY_adr[0][JJ]);
				CUE_OPERATION = MEMORY_NEED_SET_PART2;	
				}
			else
				{
				CUE_OPERATION = 0;	
				}
			}
		else
			{
			CUE_OPERATION = 0;	
			}			
		}	
	else if(CUE_OPERATION==MEMORY_NEED_SET_PART2 && ((end_adr_valid_data-start_adr_valid_data)>64))	
		{
		SET_MEMORY_CUE_2();
		offset_adress = 0;		
		CUE_OPERATION = 0;
		}	
		
		
	///from (PART_CODE==1)
	if(tempo_need_update>0)
		{
//		if(tempo_need_update==1)
//			{
//			ShowTempo(potenciometer_tempo);	
//			}	
//		if(originalBPM!=0xFFFF)
//			{
//			ShowBPM(((originalBPM+5)*potenciometer_tempo)/100000);	
//			}
		tempo_need_update = 0;
		}	
		
		
		
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


	

/* USER CODE END 4 */
	

	
/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x30000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  //HAL_MPU_Enable(MPU_HFNMI_PRIVDEF);


  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_HFNMI_PRIVDEF);


}

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
