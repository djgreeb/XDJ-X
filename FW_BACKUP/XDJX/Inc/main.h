/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#define BT_INFO_Pin GPIO_PIN_6
#define BT_INFO_GPIO_Port GPIOI
#define BT_TAGLIST_Pin GPIO_PIN_4
#define BT_TAGLIST_GPIO_Port GPIOI
#define SD_DETECT_Pin GPIO_PIN_5
#define SD_DETECT_GPIO_Port GPIOD
#define BT_MENU_Pin GPIO_PIN_7
#define BT_MENU_GPIO_Port GPIOI
#define BT_SHIFT_Pin GPIO_PIN_9
#define BT_SHIFT_GPIO_Port GPIOB
#define BT_TAG_Pin GPIO_PIN_2
#define BT_TAG_GPIO_Port GPIOH
#define LED_LOAD1_Pin GPIO_PIN_2
#define LED_LOAD1_GPIO_Port GPIOA
#define LED_LOAD0_Pin GPIO_PIN_1
#define LED_LOAD0_GPIO_Port GPIOA
#define BT_BACK_Pin GPIO_PIN_3
#define BT_BACK_GPIO_Port GPIOH
#define BUTTON_Pin GPIO_PIN_4
#define BUTTON_GPIO_Port GPIOH
#define BACKLIGHT_EN_Pin GPIO_PIN_15
#define BACKLIGHT_EN_GPIO_Port GPIOI
#define LED_MENU_Pin GPIO_PIN_10
#define LED_MENU_GPIO_Port GPIOH
#define BT_ENC_Pin GPIO_PIN_2
#define BT_ENC_GPIO_Port GPIOB
#define LED_INFO_Pin GPIO_PIN_9
#define LED_INFO_GPIO_Port GPIOH
#define BT_LOAD1_Pin GPIO_PIN_1
#define BT_LOAD1_GPIO_Port GPIOB
#define P8EN_Pin GPIO_PIN_8
#define P8EN_GPIO_Port GPIOH
#define BT_BROWSE_Pin GPIO_PIN_12
#define BT_BROWSE_GPIO_Port GPIOB
#define LED_ENC_Pin GPIO_PIN_3
#define LED_ENC_GPIO_Port GPIOA
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define BT_LOAD0_Pin GPIO_PIN_0
#define BT_LOAD0_GPIO_Port GPIOB
#define LED_TAGLIST_Pin GPIO_PIN_7
#define LED_TAGLIST_GPIO_Port GPIOH
#define LED_BROWSE_Pin GPIO_PIN_13
#define LED_BROWSE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
