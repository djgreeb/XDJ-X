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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//		Generated with: STM32CubeMX 6.10.0
//		Firmware Package Name and Version STM32Cube FW_G4 V1.5.1
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//	ver. 0.17
//		- added all gpio
//		- added button and leds handler
//		-	added UART handler (need add UART interrupt and regenerate code)
//		-	added ADC process
//	ver. 0.18
//		- added gpio direct registers calling
//		-	improved USART1_IRQHandler
//	ver. 0.21
//		- fix quick blink period
//		- fix turn off after turn on bug
//	ver. 0.23
//		-	regenerate code
//		-	added UART IRQHandler
//
//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////


#include "global_variables.h"

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(GPIOB, LED_WHITE_Pin|LED_GREEN_Pin, GPIO_PIN_SET);
	USART1->CR1 |= USART_CR1_RXNEIE; //interrupt for a RX enable
	HAL_Delay(300);
	HAL_ADC_Start(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	HAL_Delay(20);			//create 50Hz period			
		
	if(HAL_GPIO_ReadPin(BUTTON_IN_GPIO_Port, BUTTON_IN_Pin)==0 && BUTTON_pressed==0)
		{		
		LED_W_OFF;
		LED_G_OFF;		
		tim_button_press = 0;	
		TX_DATA[1] = B_PRESS;	
		HAL_UART_Transmit(&huart1, TX_DATA, 2, 5);		
		BUTTON_pressed = 1;	
		}	
	else if(HAL_GPIO_ReadPin(BUTTON_IN_GPIO_Port, BUTTON_IN_Pin)==0 && BUTTON_pressed==1)
		{
		if(tim_button_press<0xFFFF)
			{
			tim_button_press++;
			if(XDJ_POWER_ON==0)
				{
				if(tim_button_press==150)	//3 sec pressed
					{	
					HAL_GPIO_WritePin(PON_GPIO_Port, PON_Pin, GPIO_PIN_SET);	
					LED_W_OFF;
					LED_G_ON;	
					need_change_status_power = 1;	
					}					
				}
			else
				{
				if(tim_button_press==250)	//5 sec pressed (send turn off command to main assy, waitig answer)
					{	
					TX_DATA[1] = T_OFF;	
					HAL_UART_Transmit(&huart1, TX_DATA, 2, 5);						
					LED_W_OFF;
					LED_G_OFF;
					quick_blink = 1;	
					timeout_power = 0;	
					}		
				}				
			}
		}		
	else if(HAL_GPIO_ReadPin(BUTTON_IN_GPIO_Port, BUTTON_IN_Pin)==1 && BUTTON_pressed==1)
		{
		timeout_unpress++;	
		if(timeout_unpress>3)
			{
			if(need_change_status_power>0)
				{
				XDJ_POWER_ON = 1;		
				need_change_status_power = 0;	
				}
			if(tim_button_press<50)  //short press
				{
				TX_DATA[1] = B_UNPRESS;	
				HAL_UART_Transmit(&huart1, TX_DATA, 2, 5);		
				}			
			tim_button_press = 0xFFFF;	
			timeout_unpress = 0;	
			BUTTON_pressed = 0;	
			}				
		}		
	
	if(quick_blink==0)
		{
		if(XDJ_POWER_ON==0 && BUTTON_pressed==0)
			{
			if(HAL_GPIO_ReadPin(GPIOB, ACIN_EN_Pin)==1)
				{
				if(HAL_GPIO_ReadPin(GPIOB, CHARGE_EN_Pin)==0)			//CHARGE enable	
					{
					TX_DATA[0] = AC_CHARGE;	
					blink_tim++;							
					if(blink_tim<64 || (blink_tim>128 && blink_tim<192))	
						{
						LED_W_OFF;
						LED_G_OFF;		
						}
					else
						{	
						LED_W_ON;
						LED_G_OFF;		
						}					
					}
				else			//charge disable - FULL
					{
					TX_DATA[0] = AC_FULL;	
					LED_W_ON;
					LED_G_OFF;	
					}
				}
			else
				{
				TX_DATA[0] = AC_DSBL;	
				LED_W_OFF;
				LED_G_OFF;	
				}			
			}
		else if(XDJ_POWER_ON==1 && BUTTON_pressed==0)
			{
			if(HAL_GPIO_ReadPin(GPIOB, ACIN_EN_Pin)==1)
				{
				if(HAL_GPIO_ReadPin(GPIOB, CHARGE_EN_Pin)==0)			//CHARGE enable	
					{
					TX_DATA[0] = AC_CHARGE;	
					blink_tim++;	
					if(blink_tim<64 || (blink_tim>128 && blink_tim<192))	
						{
						LED_W_OFF;
						LED_G_ON;		
						}
					else
						{	
						LED_W_ON;
						LED_G_OFF;		
						}					
					}
				else			//charge disable - FULL
					{
					TX_DATA[0] = AC_FULL;	
					LED_W_OFF;
					LED_G_ON;	
					}
				}
			else
				{
				TX_DATA[0] = AC_DSBL;	
				LED_W_OFF;
				LED_G_ON;		
				}		
			}	
		if(previous_status!=TX_DATA[0])
			{
			if(XDJ_POWER_ON==1)
				{
				TX_DATA[1] = batt_prcntg;		//battery precentage	
				HAL_UART_Transmit(&huart1, TX_DATA, 2, 5);		
				}	
			previous_status = TX_DATA[0];	
			}
		}
	else				//turn off sequence (waitig answer form main assy) 
		{
		if(timeout_power<1000)
			{
			timeout_power++;
			if(timeout_power==999)			//20 sec pressed (hardware turn off)
				{
				quick_blink = 0;		
				HAL_GPIO_WritePin(PON_GPIO_Port, PON_Pin, GPIO_PIN_RESET);	
				LED_W_OFF;
				LED_G_OFF;
				XDJ_POWER_ON = 0;		
				timeout_power = 0xFFFF;	
				}				
			}			
		blink_tim++;
		LED_W_OFF;	
		if((blink_tim%8)==0)	
			{
			LED_G_ON;		
			}
		else if((blink_tim%8)==4)	
			{
			LED_G_OFF;		
			}
		}		

	if(usart_new_data)									////////////////////////usart data handler
		{
		HAL_Delay(2);	
		if((U_RX_DATA[0]==AC_DSBL) && (U_RX_DATA[1]==T_OFF))				//turn off command
			{							
			quick_blink = 0;
			HAL_GPIO_WritePin(PON_GPIO_Port, PON_Pin, GPIO_PIN_RESET);	
			LED_W_OFF;
			LED_G_OFF;
			XDJ_POWER_ON = 0;			
			}
		else if((U_RX_DATA[0]==AC_DSBL) && (U_RX_DATA[1]==BATT_RQ))				//ADC batt request
			{
			TX_DATA[1] = batt_prcntg;		//battery precentage	
			HAL_UART_Transmit(&huart1, TX_DATA, 2, 5);	
			}
		usart_new_data = 0;		
		}
		
	/////////////////////////////ADC process
	ADC_TEMP = HAL_ADC_GetValue(&hadc1);
	DATAV_TEMP*=3;	
	DATAV_TEMP+=ADC_TEMP;	
	DATAV_TEMP+=2;	
	DATAV_TEMP>>=2;	
		
	batt_prcntg = DATAV_TEMP/27;		//neeed table V to %	
	if(batt_prcntg>100)
		{
		batt_prcntg = 100;	
		}		
	
	if(previous_batt_prcntg!=batt_prcntg)			//forceble tx batt prcntg to main assy
		{	
		if((HAL_GetTick() - adc_timeout)>1000)
			{
			if(XDJ_POWER_ON==1)
				{
				TX_DATA[1] = batt_prcntg;		//battery precentage	
				HAL_UART_Transmit(&huart1, TX_DATA, 2, 5);				
				}				
			adc_timeout = HAL_GetTick();	
			previous_batt_prcntg = batt_prcntg;	
			}				
		}
	HAL_ADC_Start(&hadc1);
		
		
		
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


/////////////////////////////////////////////////////
//
//		UART handler
//
//
void USART1_IRQHandler(void)
	{
	uint8_t USART_RX_B;	
	if(USART1->ISR & USART_ISR_RXNE)		//have a new data
		{
		USART_RX_B = (USART1->RDR);	
		if(((HAL_GetTick() - usart_timeout)>200))
			{	
			if(USART_RX_B==AC_DSBL)
				{
				usart_data_cnt = 0;
				U_RX_DATA[0] = USART_RX_B;					
				}
			else
				{
				usart_data_cnt = 0xFF;	
				}
			}
		else
			{
			if(usart_data_cnt<4)	
				{
				usart_data_cnt++;	
				U_RX_DATA[usart_data_cnt] = USART_RX_B;
				usart_new_data = 1;	
				}
			}
		usart_timeout = HAL_GetTick(); 			
		}
  HAL_UART_IRQHandler(&huart1);
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
