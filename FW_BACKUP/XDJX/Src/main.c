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
//	ver. 0.88
//	-	PRETRIM changed for disable DC in audio, when track stopped
//	-	optimize HAL_SAI_Transmit_IT
//	- bug fixed: PLAY LED sometimes did not on in play mode
//	-	added ShowTempo first
//	ver. 0.89
//	- added ShowBPM
//	ver. 0.90
//	- added loops
//	-	improved ShowBPM
//	ver. 0.92
//	- added DrawTempoRange func
//	-	added DrawLoopMode func 
//	ver. 0.93
//	- minor improvements
//	ver. 0.95
//	- optimizations
//	ver. 0.96
//	- spi eeprom added
//	ver. 0.97
//	-	added LOAD A, LOAD B, ENC buttons
//	ver. 0.98
//	-	added ShowQUANTIZE func
//	ver. 0.99
//	-	added ShowACUE func
//	-	added part code
//	ver. 1.01
//	-	added transmittion bpm to mixer assy
//	-	UART TX optimized
//	ver. 1.03
//	- uart mixer link up to 500000 baud
//	- uart debug up to 256000 baud
//	- uart PM up to 256000 baud
//	ver. 1.04
//	- jog ring colors added
//	-	encoder code added
//	ver. 1.06
//	-	encoder bug fixed
//	-	jog color const calibration
//	ver. 1.07
//	-	color cue changed
//	- batt icon added
//	- lcd brightness control added
//	ver. 1.08
//	- bug fixed
//	ver. 1.09
//	-	LTDC minor improvements
//	ver. 1.11
//	- SPI2 speed up to 2X
//	ver. 1.12
//	-	decka_handker, deckb_handker added
//	ver. 1.14
//	- optimize code audio buff and adress jumping
//	ver. 1.15
//	-	migrated to STM32H743XIH6
//	ver. 1.16
//	- StaticWFM shifted 1px to down
//	ver. 1.17
//	-	double variables for decks
//	ver. 1.20
//	- added double code for decks
//	-	deck B draw time added
//	ver. 1.21
//	- double dynamic waveform added
//	-	sdmmc divider 33
//	ver. 1.22
//	-	sdmmc divider 35 (33 - have error 2)
//	-	masterdeck added
//	ver. 1.23
//	-	improved ShowBPM, masterdeck algorithm
//	ver. 1.24
//	- USART1_IRQHandler MIXER HANDLER, timeout changed 100ms->20ms
//	ver. 1.25
//	- added 30 sec markers
//	- added waveform left posotion
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
char FIRMWARE_VERSION[] = "1.25";	
#define DEBUG_UART_EN				//sending work status to uart
#include "global_variables.h"
#include "audio.h"
#include "CWX3970.h"
#include "qspi_const.h"
#include "gui.h"
#include "audio_handler.h"
#include "deck_transfer.h"
#include "rekordbox.h"
#include "spi_mem.h"



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
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_LTDC_Init();
  MX_QUADSPI_Init();
  MX_SDMMC1_SD_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM8_Init();
  MX_UART4_Init();
  MX_UART7_Init();
  MX_USART1_UART_Init();
  MX_SAI1_Init();
  MX_FATFS_Init();
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

	HAL_SAI_Transmit_IT(&hsai_BlockA1, (uint8_t*)&deckout[dkA][0], 2);
	HAL_SAI_Transmit_IT(&hsai_BlockB1, (uint8_t*)&deckout[dkB][0], 2);

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
	BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER_2);
	
	#if defined(DEBUG_UART_EN)	
	sprintf((char*)U_TX_DATA, "%01lu error\n\r", i);	
	UART_TX(&huart4, U_TX_DATA, 9, 5);	
	#endif
	
	i = 0;
	
	#if defined(DEBUG_UART_EN)	
	sprintf((char*)U_TX_DATA, "All init\n\r");	
	UART_TX(&huart4, U_TX_DATA, 10, 5);
	#endif

	BSP_LCD_SetTransparency(0, 255);
	BSP_LCD_SetTransparency(1, 255);
	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_BLACK);	
	
	for(j=0;j<19;j++)		//extract eeprom data
		{		
		UT_SET[j] = RD_EEMEM(j);
		}	
		
	tempo_range[dkA] = UT_SET[4];		
		
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

	if(UT_SET[11]<9)
		{
		jog_work_color[dkA] = jog_color[UT_SET[12]][UT_SET[11]];
		jog_work_color[dkB] = jog_color[UT_SET[12]][UT_SET[11]];	
		}					
	else
		{
		jog_work_color[dkA] = jog_color[UT_SET[12]][curr_trck_color[dkA]];	
		jog_work_color[dkB] = jog_color[UT_SET[12]][curr_trck_color[dkB]];	
		}
	SET_JOG_COLOR(dkA, jog_work_color[dkA]);	
	SET_JOG_COLOR(dkB, jog_work_color[dkB]);	
		
	HAL_TIM_Base_Start_IT(&htim2);									//start deck transfer timer
	HAL_GPIO_WritePin(GPIOH, P8EN_Pin, GPIO_PIN_SET);	
	HAL_Delay(1300);						//need decrees delay in deck assy. spi dma not starting?
//	U_TX_DATA[0] = 0x94;			//setting headphones cue type (pre EQ or post EQ)
//	U_TX_DATA[1] = 0xA3;
//	U_TX_DATA[2] =  UT_SET[18];
//	UART_TX(&huart1, U_TX_DATA, 3, 5);		
//	HAL_Delay(200);	
//	U_TX_DATA[0] = 0x93;			//setting fader curve type
//	U_TX_DATA[1] = 0xB1;
//	U_TX_DATA[2] =  UT_SET[15];
//	UART_TX(&huart1, U_TX_DATA, 3, 5);		
//	HAL_Delay(200);	
//	U_TX_DATA[0] = 0x92;			//mixer EQ type
//	U_TX_DATA[1] = UT_SET[16];
//	U_TX_DATA[2] =  UT_SET[17];
//	UART_TX(&huart1, U_TX_DATA, 3, 5);	
//	HAL_Delay(200);
	TIM8->CCR1 = LCDPWM_MAP[UT_SET[13]];			//LCD
	HAL_GPIO_WritePin(BACKLIGHT_EN_GPIO_Port, BACKLIGHT_EN_Pin, GPIO_PIN_SET);	
	DrawLOGO();	
	HAL_TIM_Base_Start_IT(&htim3);		

	res = f_mount(&FAT, "0", 1);
	if (res!=FR_OK)
		{
		BSP_LCD_SelectLayer(0);
		BSP_LCD_Clear(LCD_COLOR_BLACK);			
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "%01lu error\n\r", res);	
		UART_TX(&huart4, U_TX_DATA, 9, 5);		
		sprintf((char*)U_TX_DATA, "SD not mounted\n\r");	
		UART_TX(&huart4, U_TX_DATA, 16, 5);
		#endif	
		}
	else
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Drive mounted\n\r");	
		UART_TX(&huart4, U_TX_DATA, 15, 5);	
		#endif		
		res = f_open(&file, path_pic, FA_READ);
		if (res != FR_OK)
			{
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "Picture not opened\n\r");	
			UART_TX(&huart4, U_TX_DATA, 20, 5);	
			#endif	
			}		
		else
			{
			#if defined(DEBUG_UART_EN)		
			sprintf((char*)U_TX_DATA, "Picture file open\n\r", i);	
			UART_TX(&huart4, U_TX_DATA, 19, 5);	
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
		UART_TX(&huart4, U_TX_DATA, 31, 5);	
		#endif		
		SD_STATUS = SD_NEED_REPLACE;
		}
	else if(TOTAL_TRACKS==0xFFFF)
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Rekordbox database has more than 512 tracks.\n\r");	
		UART_TX(&huart4, U_TX_DATA, 46, 5);	
		#endif	
		SD_STATUS = SD_NEED_REPLACE;	
		}
	else if(TOTAL_TRACKS==0xFFF1)
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Playlists structure error.\n\r");	
		UART_TX(&huart4, U_TX_DATA, 28, 5);	
		#endif	
		SD_STATUS = SD_NEED_REPLACE;	
		}	
	else if(TOTAL_TRACKS==0)
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Rekordbox database dont't have a tracks.\n\r");	
		UART_TX(&huart4, U_TX_DATA, 42, 5);	
		#endif	
		SD_STATUS = SD_NEED_REPLACE;	
		}	
	else
		{
		#if defined(DEBUG_UART_EN)		
		sprintf((char*)U_TX_DATA, "Rekordbox database have a %03lu tracks\n\r", TOTAL_TRACKS);	
		UART_TX(&huart4, U_TX_DATA, 38, 5);	
		#endif
		SD_STATUS = SD_MOUNTED;	
		}		

	HAL_GPIO_WritePin(GPIOA, LED_LOAD1_Pin|LED_LOAD0_Pin|LED_ENC_Pin, GPIO_PIN_SET);	
				
	BSP_LCD_SelectLayer(0);		
	ShowTempo(dkA, 10000);
	ShowTempo(dkB, 10000);	
	ShowBPM(dkA, 0xFFFF);
	ShowBPM(dkB, 0xFFFF);	
	DrawLoopMode(dkA, loop_lenght[dkA], loop_act_gui[dkA]);	
	DrawLoopMode(dkB, loop_lenght[dkB], loop_act_gui[dkB]);		
	DrawTempoRange(dkA, tempo_range[dkA]);
	DrawTempoRange(dkB, tempo_range[dkB]);	
	if(UT_SET[ACUE]==8)
		{
		ShowACUE(2);	
		}
	else
		{
		ShowACUE(1);	
		}	
		
	BSP_LCD_SetFont(&Font15P);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	sprintf((char*)Buf, "%s", "03");
	BSP_LCD_DisplayStringAt(5, 124, Buf, TRANSPARENT_MODE);		
	BSP_LCD_SetTextColor(LCD_COLOR_CUE);
	sprintf((char*)Buf, "%s", "Bleu Clair & OOTORO");
	BSP_LCD_DisplayStringAt(29, 124, Buf, TRANSPARENT_MODE);	
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
	sprintf((char*)Buf, "%s", "Beat Like This");	
	BSP_LCD_DisplayStringAt(5, 141, Buf, TRANSPARENT_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
	sprintf((char*)Buf, "%s", "04");		
	BSP_LCD_DisplayStringAt(245, 124, Buf, TRANSPARENT_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);	
	sprintf((char*)Buf, "%s", "Just Kiddin");		
	BSP_LCD_DisplayStringAt(269, 124, Buf, TRANSPARENT_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);	
	sprintf((char*)Buf, "%s", "Out My Mind");	
	BSP_LCD_DisplayStringAt(300, 141, Buf, TRANSPARENT_MODE);			
	j = 0;
			
	
	#if defined(DEBUG_UART_EN)	
	sprintf((char*)U_TX_DATA, "Main assy: ");	
	UART_TX(&huart4, U_TX_DATA, 11, 5);	
	UART_TX(&huart4, FIRMWARE_VERSION, 4, 5);
	sprintf((char*)U_TX_DATA, "\n\r");	
	UART_TX(&huart4, U_TX_DATA, 2, 5);
	sprintf((char*)U_TX_DATA, "Deck 1 UCOM: ");	
	UART_TX(&huart4, U_TX_DATA, 13, 5);
	sprintf((char*)U_TX_DATA, "%01lu.%02lu\n\r", (deckRbuf[4]&0x7F)/100, (deckRbuf[4]&0x7F)%100);
	UART_TX(&huart4, U_TX_DATA, 6, 5);
	sprintf((char*)U_TX_DATA, "Deck 2 UCOM: ");	
	UART_TX(&huart4, U_TX_DATA, 13, 5);
	sprintf((char*)U_TX_DATA, "%01lu.%02lu\n\r", (deckRbuf[12]&0x7F)/100, (deckRbuf[12]&0x7F)%100);
	UART_TX(&huart4, U_TX_DATA, 6, 5);	
	#endif		
		
//	U_TX_DATA[0] = 0x91;			//firmware version request
//	U_TX_DATA[1] = 0x60;
//	U_TX_DATA[2] = 0x31;
//	UART_TX(&huart1, U_TX_DATA, 3, 5);	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	if((HAL_GetTick() - frametime)>19)	
		{
		frametime = HAL_GetTick();		
		RedrawWaveforms(dkA, play_adr[dkA]/294);
		RedrawWaveforms(dkB, play_adr[dkB]/294);	
			
		if(need_redraw_memline)
			{
			BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
			BSP_LCD_DrawHLine(3, 191, 203);						
			BSP_LCD_SetTextColor(LCD_COLOR_RED);
			BSP_LCD_DrawHLine(3+((start_adata[dkA]*5656)/all_long[dkA]), 191, ((end_adata[dkA]*5656)/all_long[dkA])-((start_adata[dkA]*5656)/all_long[dkA]));			
			need_redraw_memline = 0;	
			}			
		}
		
	if(fat_semaphore==0)
		{	
		if(end_adata[dkA]<128)
			{
			f_read(&file, PCM[dkA][end_adata[dkA]][0], 32768, &nbytes);
			need_redraw_memline = 1;	
			end_adata[dkA]++;	
			}
		else if((end_adata[dkA]<((play_adr[dkA]>>13)+42)) && (fill_stp[dkA]==0 || fill_stp[dkA]==6) && (end_adata[dkA]<=(((all_long[dkA]*294)>>13)+1)))									//filling the buffer forward
			{
			if(fill_stp[dkA]==6)
				{
				f_lseek(&file, ((32768*end_adata[dkA])+44));	
				fill_stp[dkA] = 0;	
				}		
			f_read(&file, PCM[dkA][end_adata[dkA]&0x7F][0], 32768, &nbytes);		
			end_adata[dkA]++;
			if((end_adata[dkA]-start_adata[dkA])>128)
				{
				start_adata[dkA] = end_adata[dkA]-128;	
				}
			need_redraw_memline = 1;	
			}
		else if(((end_adata[dkA]>((play_adr[dkA]>>13)+86) || ((end_adata[dkA]-start_adata[dkA])<124)) && start_adata[dkA]>3) || (fill_stp[dkA]!=0 && fill_stp[dkA]!=6))					//filling the buffer back
			{
			if(fill_stp[dkA]==0 || fill_stp[dkA]==6)
				{
				if((end_adata[dkA]-start_adata[dkA])>127)	
					{
					end_adata[dkA] = start_adata[dkA]+124;	
					}	
				start_adata[dkA]-=4;	
				f_lseek(&file, ((32768*(start_adata[dkA]))+44));
				fill_stp[dkA] = 1;	
				}
			else if(fill_stp[dkA]==1)
				{
				f_read(&file, PCM[dkA][start_adata[dkA]&0x7F][0], 32768, &nbytes);
				fill_stp[dkA] = 2;	
				}
			else if(fill_stp[dkA]==2)
				{
				f_read(&file, PCM[dkA][(start_adata[dkA]+1)&0x7F][0], 32768, &nbytes);
				fill_stp[dkA] = 3;	
				}
			else if(fill_stp[dkA]==3)
				{
				f_read(&file, PCM[dkA][(start_adata[dkA]+2)&0x7F][0], 32768, &nbytes);
				fill_stp[dkA] = 4;	
				}
			else if(fill_stp[dkA]==4)
				{
				f_read(&file, PCM[dkA][(start_adata[dkA]+3)&0x7F][0], 32768, &nbytes);
				fill_stp[dkA] = 5;	
				}
			else if(fill_stp[dkA]==5)
				{
				need_redraw_memline = 1;		
				fill_stp[dkA] = 6;		
				}
			}		
		fat_semaphore = 1;	
		}		
	else
		{
		if(end_adata[dkB]<128)
			{
			f_read(&fileb, PCM[dkB][end_adata[dkB]][0], 32768, &nbytesb);
			//need_redraw_memline = 1;	
			end_adata[dkB]++;	
			}
		else if((end_adata[dkB]<((play_adr[dkB]>>13)+42)) && (fill_stp[dkB]==0 || fill_stp[dkB]==6) && (end_adata[dkB]<=(((all_long[dkB]*294)>>13)+1)))									//filling the buffer forward
			{
			if(fill_stp[dkB]==6)
				{
				f_lseek(&fileb, ((32768*end_adata[dkB])+44));	
				fill_stp[dkB] = 0;	
				}		
			f_read(&fileb, PCM[dkB][end_adata[dkB]&0x7F][0], 32768, &nbytesb);		
			end_adata[dkB]++;
			if((end_adata[dkB]-start_adata[dkB])>128)
				{
				start_adata[dkB] = end_adata[dkB]-128;	
				}
			//need_redraw_memline = 1;	
			}
		else if(((end_adata[dkB]>((play_adr[dkB]>>13)+86) || ((end_adata[dkB]-start_adata[dkB])<124)) && start_adata[dkB]>3) || (fill_stp[dkB]!=0 && fill_stp[dkB]!=6))					//filling the buffer back
			{
			if(fill_stp[dkB]==0 || fill_stp[dkB]==6)
				{
				if((end_adata[dkB]-start_adata[dkB])>127)	
					{
					end_adata[dkB] = start_adata[dkB]+124;	
					}	
				start_adata[dkB]-=4;	
				f_lseek(&fileb, ((32768*(start_adata[dkB]))+44));
				fill_stp[dkB] = 1;	
				}
			else if(fill_stp[dkB]==1)
				{
				f_read(&fileb, PCM[dkB][start_adata[dkB]&0x7F][0], 32768, &nbytesb);
				fill_stp[dkB] = 2;	
				}
			else if(fill_stp[dkB]==2)
				{
				f_read(&fileb, PCM[dkB][(start_adata[dkB]+1)&0x7F][0], 32768, &nbytesb);
				fill_stp[dkB] = 3;	
				}
			else if(fill_stp[dkB]==3)
				{
				f_read(&fileb, PCM[dkB][(start_adata[dkB]+2)&0x7F][0], 32768, &nbytesb);
				fill_stp[dkB] = 4;	
				}
			else if(fill_stp[dkB]==4)
				{
				f_read(&fileb, PCM[dkB][(start_adata[dkB]+3)&0x7F][0], 32768, &nbytesb);
				fill_stp[dkB] = 5;	
				}
			else if(fill_stp[dkB]==5)
				{
				//need_redraw_memline = 1;		
				fill_stp[dkB] = 6;		
				}
			}	
		fat_semaphore = 0;	
		}	
	
		
		
	if(PART_CODE==0)
		{
		//TOUCH_SCREEN_HANDLER();
		if(batt_need_update>0)
			{
			if(batt_need_update&0x0F)
				{
				ShowBATT(batt_prcnt);	
				}
			if(batt_need_update&0xF0)
				{
				ShowAC((batt_status&0x03)-1);
				}				
			batt_need_update = 0;	
			}
		PART_CODE = 1;	
		}	
	else if(PART_CODE==1)
		{				
		//if(track_need_load[dkA]!=0 && TOTAL_TRACKS_IN_CURRENT_PLAYLIST!=0)
		if(track_need_load[dkA]!=0)	
			{
			if(UT_SET[1] && end_track[dkA]==0 && play_enable[dkA])						//lock load	
				{	
				//message about load lock		
				}
			else
				{
				ENC_LED_BLINK[dkA] = 0;
				load_animation_en[dkA] = 1;
				if(track_play_now[dkA]==TOTAL_TRACKS)
					{
					track_play_now[dkA] = 1;	
					}
				else
					{
					track_play_now[dkA]++;	
					}	
				PREPARE_LOAD_TRACK(dkA, track_play_now[dkA], track_play_now[dkA]);			
				}	
			track_need_load[dkA] = 0;	
			}
			
		//if(track_need_load[dkB]!=0 && TOTAL_TRACKS_IN_CURRENT_PLAYLIST!=0)
		if(track_need_load[dkB]!=0)	
			{
			if(UT_SET[1] && end_track[dkB]==0 && play_enable[dkB])						//lock load	
				{	
				//message about load lock		
				}
			else
				{
				ENC_LED_BLINK[dkB] = 0;
				load_animation_en[dkB] = 1;
				if(track_play_now[dkB]==TOTAL_TRACKS)
					{
					track_play_now[dkB] = 1;	
					}
				else
					{
					track_play_now[dkB]++;	
					}	
				PREPARE_LOAD_TRACK(dkB, track_play_now[dkB], track_play_now[dkB]);			
				}	
			track_need_load[dkB] = 0;	
			}		
					
		/////deck A	
		if(tempo_range_need_update[dkA])
			{	
			DrawTempoRange(dkA, tempo_range[dkA]);
			tempo_need_update[dkA] = 1;	
			tempo_range_need_update[dkA] = 0;	
			}	
		if(tempo_need_update[dkA]>0)
			{
			if(tempo_need_update[dkA]==1)
				{
				ShowTempo(dkA, potenciometr_tempo[dkA]);	
				}	
			if(originalBPM[dkA]!=0xFFFF)
				{
				ShowBPM(dkA, ((originalBPM[dkA]+5)*potenciometr_tempo[dkA])/100000);	
				}
			if(masterdeck==dkA)
				{		
				mixerBPM = (originalBPM[dkA]*potenciometr_tempo[dkA])/10000;
				}					
			tempo_need_update[dkA] = 0;
			}	
		if(loopmode_need_update[dkA])
			{	
			DrawLoopMode(dkA, loop_lenght[dkA], loop_act_gui[dkA]);
			loopmode_need_update[dkA] = 0;	
			}		
		/////deck B	
		if(tempo_range_need_update[dkB])
			{	
			DrawTempoRange(dkB, tempo_range[dkB]);
			tempo_need_update[dkB] = 1;	
			tempo_range_need_update[dkB] = 0;	
			}	
		if(tempo_need_update[dkB]>0)
			{
			if(tempo_need_update[dkB]==1)
				{
				ShowTempo(dkB, potenciometr_tempo[dkB]);	
				}	
			if(originalBPM[dkB]!=0xFFFF)
				{
				ShowBPM(dkB, ((originalBPM[dkB]+5)*potenciometr_tempo[dkB])/100000);	
				}
			if(masterdeck==dkB)
				{				
				mixerBPM = (originalBPM[dkB]*potenciometr_tempo[dkB])/10000;	
				}
			tempo_need_update[dkB] = 0;
			}	
		if(loopmode_need_update[dkB])
			{	
			DrawLoopMode(dkB, loop_lenght[dkB], loop_act_gui[dkB]);
			loopmode_need_update[dkB] = 0;	
			}
		/////deck A		
		if(time_mode_need_update[dkA])			
			{
			forcibly_redraw[dkA] = 1;	
			if(REMAIN_ENABLE[dkA])
				{
				DrawStaticWFM(dkA, REDRAW_IN_REMAIN_MODE);
				}
			else
				{
				DrawStaticWFM(dkA, REDRAW_IN_NREMAIN_MODE);	
				}
			time_mode_need_update[dkA] = 0;		
			}     /////deck B		
		else if(time_mode_need_update[dkB])			
			{
			forcibly_redraw[dkB] = 1;	
			if(REMAIN_ENABLE[dkB])
				{
				DrawStaticWFM(dkB, REDRAW_IN_REMAIN_MODE);
				}
			else
				{
				DrawStaticWFM(dkB, REDRAW_IN_NREMAIN_MODE);	
				}
			time_mode_need_update[dkB] = 0;		
			}			
		else if(need_DSW[dkA]>0)
			{
			if(need_DSW[dkA]==1)
				{
				DrawStaticWFM(dkA, REDRAW_IN_NREMAIN_MODE);				
				}
			else
				{
				DrawStaticWFM(dkA, REDRAW_IN_REMAIN_MODE);					
				}
			if(jog_light[dkA]==0)	
				{
				SET_JOG_COLOR(dkA, jog_work_color[dkA]);	
				jog_light[dkA] = 1;	
				}	
			else
				{
				SET_JOG_COLOR(dkA, 0);	
				jog_light[dkA] = 0;	
				}
			need_DSW[dkA] = 0;	
			}
		else if(need_DSW[dkB]>0)
			{
			if(need_DSW[dkB]==1)
				{
				DrawStaticWFM(dkB, REDRAW_IN_NREMAIN_MODE);				
				}
			else
				{
				DrawStaticWFM(dkB, REDRAW_IN_REMAIN_MODE);					
				}
			if(jog_light[dkB]==0)	
				{
				SET_JOG_COLOR(dkB, jog_work_color[dkB]);	
				jog_light[dkB] = 1;	
				}	
			else
				{
				SET_JOG_COLOR(dkB, 0);	
				jog_light[dkB] = 0;	
				}
			need_DSW[dkB] = 0;	
			}
		else if(quantize_mode_need_update)
			{
			if(UT_SET[QUANTIZE]==0)
				{
				ShowQUANTIZE(0);	
				}
			else
				{
				ShowQUANTIZE(1);
				}	
			quantize_mode_need_update = 0;	
			}		
		PART_CODE = 2;	
		}
	else if(PART_CODE==2)
		{
		//INFO BUTTON	
		//BROWSER-WAVEFORM BUTTON	
		//SD BUTTON	
		//MIDI BUTTON	
		//USB BUTTON	
		//TAG TRACK/REMOVE BUTTON	
		//TAG LIST BUTTON
		//MENU BUTTON
		//BACK BUTTON
		// (enc button)LOAD TRACK
			
		if(enc_need_up)							///////////ENCODER 	
			{
			if(dSHOW==WAVEFORM)
				{
				if(DynamicWaveformZOOM>1)
					{
					DynamicWaveformZOOM/=2;
					if(track_play_now[dkA]!=0)
						{
						forcibly_redraw[dkA] = 1;
						}						
					if(track_play_now[dkB]!=0)
						{	
						forcibly_redraw[dkB] = 1;
						}		
					}				
				}				
			enc_need_up = 0;	
			}
		else if(enc_need_down)							///////////ENCODER 
			{	
			if(dSHOW==WAVEFORM)
				{				
				if(DynamicWaveformZOOM<16)
					{
					DynamicWaveformZOOM*=2;
					if(track_play_now[dkA]!=0)
						{	
						forcibly_redraw[dkA] = 1;
						}
					if(track_play_now[dkB]!=0)
						{	
						forcibly_redraw[dkB] = 1;
						}			
					}
				}				
			enc_need_down = 0;	
			}	
			
		if(((GPIOB->IDR&0x00000004)==0) && ENC_BUTTON_pressed==0) 							///////////ENCODER BUTTON
			{

				
			ENC_BUTTON_pressed = 1;	
			}
		else if(((GPIOB->IDR&0x00000004)!=0) && ENC_BUTTON_pressed==1)	
			{
			ENC_BUTTON_pressed = 0;	
			}	
		else if(((GPIOB->IDR&0x00000001)==0) && LOAD_BUTTON_pressed[0]==0) 							///////////LOAD DECK A
			{	
				
			track_need_load[dkA] = 1;	
			LOAD_BUTTON_pressed[0] = 1;	
			}
		else if(((GPIOB->IDR&0x00000001)!=0) && LOAD_BUTTON_pressed[0]==1)	
			{
			LOAD_BUTTON_pressed[0] = 0;	
			}
		else if(((GPIOB->IDR&0x00000002)==0) && LOAD_BUTTON_pressed[1]==0) 							///////////LOAD DECK B
			{

			track_need_load[dkB] = 1;	
			LOAD_BUTTON_pressed[1] = 1;	
			}
		else if(((GPIOB->IDR&0x00000002)!=0) && LOAD_BUTTON_pressed[1]==1)	
			{
			LOAD_BUTTON_pressed[1] = 0;	
			}	
		PART_CODE = 0;	
		}
	
		
	///from main code	
	if(loop_active[dkA] && CUE_ADR[dkA]<LOOP_OUT[dkA] && (play_adr[dkA]/294)>=LOOP_OUT[dkA])					//return to cue for loop mode
		{	
		CALL_CUE(dkA);
		need_jump_addr[dkA] = 1;
		}
	else if(need_jump_addr[dkA]==1)
		{
		if((play_adr[dkA]/294)>(CUE_ADR[dkA]+175) || ((play_adr[dkA]/294)+175)<CUE_ADR[dkA])		//when play_adr leaves the CUE buff boundaries
			{
			offset_adress[dkA] = 0;
			offset_adressBIG[dkA] = 0;	
			need_jump_addr[dkA] = 0;	
			}	
		}
	if(loop_active[dkB] && CUE_ADR[dkB]<LOOP_OUT[dkB] && (play_adr[dkB]/294)>=LOOP_OUT[dkB])					//return to cue for loop mode
		{	
		CALL_CUE(dkB);
		need_jump_addr[dkB] = 1;
		}
	else if(need_jump_addr[dkB]==1)
		{
		if((play_adr[dkB]/294)>(CUE_ADR[dkB]+175) || ((play_adr[dkB]/294)+175)<CUE_ADR[dkB])		//when play_adr leaves the CUE buff boundaries
			{
			offset_adress[dkB] = 0;
			offset_adressBIG[dkB] = 0;	
			need_jump_addr[dkB] = 0;	
			}	
		}

		
	/////////////		deck A		///////////////////////		
	if(CUE_OPERATION[dkA]==CUE_NEED_SET)
		{
		if(UT_SET[QUANTIZE]==1 && dSHOW==WAVEFORM)				//add calculate bars[dkA] in background process  /uncomment when bars[dkA] calculated
			{
			if(((play_adr[dkA]/294)>(BEATGRID[dkA][bars[dkA]-1]+((BEATGRID[dkA][bars[dkA]] - BEATGRID[dkA][bars[dkA]-1])/2))) || bars[dkA]==0)	
				{
				SET_CUE(dkA, BEATGRID[dkA][bars[dkA]]);	
				}
			else
				{
				SET_CUE(dkA, BEATGRID[dkA][bars[dkA]-1]);		
				}				
			}
		else
			{
			SET_CUE(dkA, play_adr[dkA]/294);	
			}
		//change masterdeck	
		if(play_enable[dkB] && masterdeck!=dkB)
			{
			masterdeck = dkB;
			tempo_need_update[dkA] = 2;
			tempo_need_update[dkB] = 2;	
			}
		CUE_OPERATION[dkA] = 0;	
		}
	else if(CUE_OPERATION[dkA]==CUE_NEED_CALL)
		{
		CALL_CUE(dkA);
		//change masterdeck	
		if(play_enable[dkB] && masterdeck!=dkB)
			{
			masterdeck = dkB;
			tempo_need_update[dkA] = 2;
			tempo_need_update[dkB] = 2;	
			}
		CUE_OPERATION[dkA] = 0;			
		}
	else if(CUE_OPERATION[dkA]==MEMORY_NEED_NEXT_SET)
		{
		if(number_of_memory_cue_points[dkA]>0)
			{
			JJ=0;
			while(MEMORY_adr[dkA][0][JJ]<=(play_adr[dkA]/294) && (JJ<number_of_memory_cue_points[dkA]-1))
				{
				JJ++;	
				}
			if((play_adr[dkA]/294)<MEMORY_adr[dkA][0][JJ])
				{
				if(loop_active[dkA])				//deactivate loop
					{
					loop_act_gui[dkA] = 0;			
					loop_lenght[dkA] = 0xFF;
					loop_active[dkA] = 0;
					LOOP_OUT[dkA] = 0;			
					loopmode_need_update[dkA] = 1;	
					}
				SET_MEMORY_CUE_1(dkA, MEMORY_adr[dkA][0][JJ]);
				CUE_OPERATION[dkA] = MEMORY_NEED_SET_PART2;	
				//change masterdeck	
				if(play_enable[dkB] && masterdeck!=dkB)
					{
					masterdeck = dkB;
					tempo_need_update[dkA] = 2;
					tempo_need_update[dkB] = 2;	
					}	
				}
			else
				{
				CUE_OPERATION[dkA] = 0;	
				}
			}
		else
			{
			CUE_OPERATION[dkA] = 0;	
			}
		}	
	else if(CUE_OPERATION[dkA]==MEMORY_NEED_PREVIOUS_SET)
		{
		if(number_of_memory_cue_points[dkA]>0)
			{
			JJ = number_of_memory_cue_points[dkA]-1;
			while(MEMORY_adr[dkA][0][JJ]>=(play_adr[dkA]/294) && (JJ>0))
				{
				JJ--;	
				}
			if((play_adr[dkA]/294)>MEMORY_adr[dkA][0][JJ])
				{
				if(loop_active[dkA])				//deactivate loop
					{
					loop_act_gui[dkA] = 0;			
					loop_lenght[dkA] = 0xFF;			
					loop_active[dkA] = 0;
					LOOP_OUT[dkA] = 0;			
					loopmode_need_update[dkA] = 1;	
					}	
				SET_MEMORY_CUE_1(dkA, MEMORY_adr[dkA][0][JJ]);	
				CUE_OPERATION[dkA] = MEMORY_NEED_SET_PART2;	
				//change masterdeck	
				if(play_enable[dkB] && masterdeck!=dkB)
					{
					masterdeck = dkB;
					tempo_need_update[dkA] = 2;
					tempo_need_update[dkB] = 2;	
					}		
				}
			else
				{
				CUE_OPERATION[dkA] = 0;	
				}
			}
		else
			{
			CUE_OPERATION[dkA] = 0;	
			}			
		}	
	else if(CUE_OPERATION[dkA]==MEMORY_NEED_SET_PART2 && ((end_adata[dkA]-start_adata[dkA])>64))	
		{
		SET_MEMORY_CUE_2(dkA);
		offset_adress[dkA] = 0;	
		offset_adressBIG[dkA] = 0;		
		CUE_OPERATION[dkA] = 0;
		}	
		
	
	/////////////		deck B		///////////////////////		
	if(CUE_OPERATION[dkB]==CUE_NEED_SET)
		{
		if(UT_SET[QUANTIZE]==1 && dSHOW==WAVEFORM)				//add calculate bars[dkB] in background process  /uncomment when bars[dkB] calculated
			{
			if(((play_adr[dkB]/294)>(BEATGRID[dkB][bars[dkB]-1]+((BEATGRID[dkB][bars[dkB]] - BEATGRID[dkB][bars[dkB]-1])/2))) || bars[dkB]==0)	
				{
				SET_CUE(dkB, BEATGRID[dkB][bars[dkB]]);	
				}
			else
				{
				SET_CUE(dkB, BEATGRID[dkB][bars[dkB]-1]);		
				}				
			}
		else
			{
			SET_CUE(dkB, play_adr[dkB]/294);	
			}
		//change masterdeck	
		if(play_enable[dkA] && masterdeck!=dkA)
			{
			masterdeck = dkA;
			tempo_need_update[dkA] = 2;
			tempo_need_update[dkB] = 2;	
			}				
		CUE_OPERATION[dkB] = 0;	
		}
	else if(CUE_OPERATION[dkB]==CUE_NEED_CALL)
		{
		CALL_CUE(dkB);
		//change masterdeck	
		if(play_enable[dkA] && masterdeck!=dkA)
			{
			masterdeck = dkA;
			tempo_need_update[dkA] = 2;
			tempo_need_update[dkB] = 2;	
			}			
		CUE_OPERATION[dkB] = 0;			
		}
	else if(CUE_OPERATION[dkB]==MEMORY_NEED_NEXT_SET)
		{
		if(number_of_memory_cue_points[dkB]>0)
			{
			JJ=0;
			while(MEMORY_adr[dkB][0][JJ]<=(play_adr[dkB]/294) && (JJ<number_of_memory_cue_points[dkB]-1))
				{
				JJ++;	
				}
			if((play_adr[dkB]/294)<MEMORY_adr[dkB][0][JJ])
				{
				if(loop_active[dkB])				//deactivate loop
					{
					loop_act_gui[dkB] = 0;			
					loop_lenght[dkB] = 0xFF;
					loop_active[dkB] = 0;
					LOOP_OUT[dkB] = 0;			
					loopmode_need_update[dkB] = 1;	
					}
				SET_MEMORY_CUE_1(dkB, MEMORY_adr[dkB][0][JJ]);
				CUE_OPERATION[dkB] = MEMORY_NEED_SET_PART2;	
				//change masterdeck	
				if(play_enable[dkA] && masterdeck!=dkA)
					{
					masterdeck = dkA;
					tempo_need_update[dkA] = 2;
					tempo_need_update[dkB] = 2;	
					}		
				}
			else
				{
				CUE_OPERATION[dkB] = 0;	
				}
			}
		else
			{
			CUE_OPERATION[dkB] = 0;	
			}
		}	
	else if(CUE_OPERATION[dkB]==MEMORY_NEED_PREVIOUS_SET)
		{
		if(number_of_memory_cue_points[dkB]>0)
			{
			JJ = number_of_memory_cue_points[dkB]-1;
			while(MEMORY_adr[dkB][0][JJ]>=(play_adr[dkB]/294) && (JJ>0))
				{
				JJ--;	
				}
			if((play_adr[dkB]/294)>MEMORY_adr[dkB][0][JJ])
				{
				if(loop_active[dkB])				//deactivate loop
					{
					loop_act_gui[dkB] = 0;			
					loop_lenght[dkB] = 0xFF;			
					loop_active[dkB] = 0;
					LOOP_OUT[dkB] = 0;			
					loopmode_need_update[dkB] = 1;	
					}	
				SET_MEMORY_CUE_1(dkB, MEMORY_adr[dkB][0][JJ]);
				CUE_OPERATION[dkB] = MEMORY_NEED_SET_PART2;	
				//change masterdeck	
				if(play_enable[dkA] && masterdeck!=dkA)
					{
					masterdeck = dkA;
					tempo_need_update[dkA] = 2;
					tempo_need_update[dkB] = 2;	
					}		
				}
			else
				{
				CUE_OPERATION[dkB] = 0;	
				}
			}
		else
			{
			CUE_OPERATION[dkB] = 0;	
			}			
		}	
	else if(CUE_OPERATION[dkB]==MEMORY_NEED_SET_PART2 && ((end_adata[dkB]-start_adata[dkB])>64))	
		{
		SET_MEMORY_CUE_2(dkB);
		offset_adress[dkB] = 0;	
		offset_adressBIG[dkB] = 0;			
		CUE_OPERATION[dkB] = 0;
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
  PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;		//
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
#include "uarts_IRQs.h"
#include "encoder_handler.h"	

	

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

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;

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
