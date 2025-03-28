/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_IN0_Pin GPIO_PIN_3
#define KEY_IN0_GPIO_Port GPIOE
#define PAEN_Pin GPIO_PIN_13
#define PAEN_GPIO_Port GPIOC
#define ADDR0_Pin GPIO_PIN_0
#define ADDR0_GPIO_Port GPIOC
#define ADDR1_Pin GPIO_PIN_1
#define ADDR1_GPIO_Port GPIOC
#define ADDR2_Pin GPIO_PIN_2
#define ADDR2_GPIO_Port GPIOC
#define BT_BUTT_Pin GPIO_PIN_0
#define BT_BUTT_GPIO_Port GPIOA
#define BT_LED_Pin GPIO_PIN_6
#define BT_LED_GPIO_Port GPIOA
#define KEY_IN1_Pin GPIO_PIN_7
#define KEY_IN1_GPIO_Port GPIOA
#define BT_bBUTT_Pin GPIO_PIN_2
#define BT_bBUTT_GPIO_Port GPIOB
#define BT_bLED_Pin GPIO_PIN_8
#define BT_bLED_GPIO_Port GPIOE
#define BT_PON_Pin GPIO_PIN_12
#define BT_PON_GPIO_Port GPIOE
#define MAX_LOAD_Pin GPIO_PIN_14
#define MAX_LOAD_GPIO_Port GPIOE
#define SHIFT_B_Pin GPIO_PIN_11
#define SHIFT_B_GPIO_Port GPIOB
#define KEY_IN2_Pin GPIO_PIN_3
#define KEY_IN2_GPIO_Port GPIOD
#define TFT_DC_Pin GPIO_PIN_5
#define TFT_DC_GPIO_Port GPIOD
#define BSELECT_Pin GPIO_PIN_6
#define BSELECT_GPIO_Port GPIOD
#define TFT_RST_Pin GPIO_PIN_4
#define TFT_RST_GPIO_Port GPIOB
#define TFT_CS_Pin GPIO_PIN_6
#define TFT_CS_GPIO_Port GPIOB
#define KEY_IN3_Pin GPIO_PIN_7
#define KEY_IN3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
