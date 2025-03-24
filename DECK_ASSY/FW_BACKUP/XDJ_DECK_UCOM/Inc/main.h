/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define BUTTON_PAD_1_Pin GPIO_PIN_13
#define BUTTON_PAD_1_GPIO_Port GPIOC
#define BUTTON_PAD_0_Pin GPIO_PIN_14
#define BUTTON_PAD_0_GPIO_Port GPIOC
#define BUTTON_REV_Pin GPIO_PIN_15
#define BUTTON_REV_GPIO_Port GPIOC
#define BUTTON_CUE_Pin GPIO_PIN_0
#define BUTTON_CUE_GPIO_Port GPIOC
#define WS2812B_Pin GPIO_PIN_1
#define WS2812B_GPIO_Port GPIOC
#define BUTTON_PLAY_Pin GPIO_PIN_2
#define BUTTON_PLAY_GPIO_Port GPIOC
#define LED_PLAY_Pin GPIO_PIN_3
#define LED_PLAY_GPIO_Port GPIOC
#define LED_REV_Pin GPIO_PIN_2
#define LED_REV_GPIO_Port GPIOA
#define LED_CUE_Pin GPIO_PIN_3
#define LED_CUE_GPIO_Port GPIOA
#define BUTTON_PAD_2_Pin GPIO_PIN_4
#define BUTTON_PAD_2_GPIO_Port GPIOC
#define BUTTON_PAD_3_Pin GPIO_PIN_5
#define BUTTON_PAD_3_GPIO_Port GPIOC
#define BUTTON_JOG_Pin GPIO_PIN_0
#define BUTTON_JOG_GPIO_Port GPIOB
#define LED_VINYL_Pin GPIO_PIN_1
#define LED_VINYL_GPIO_Port GPIOB
#define LED_SLIP_Pin GPIO_PIN_2
#define LED_SLIP_GPIO_Port GPIOB
#define not_used_Pin GPIO_PIN_10
#define not_used_GPIO_Port GPIOB
#define BUTTON_PAD_4_Pin GPIO_PIN_12
#define BUTTON_PAD_4_GPIO_Port GPIOB
#define BUTTON_PAD_5_Pin GPIO_PIN_13
#define BUTTON_PAD_5_GPIO_Port GPIOB
#define BUTTON_PAD_6_Pin GPIO_PIN_14
#define BUTTON_PAD_6_GPIO_Port GPIOB
#define BUTTON_PAD_7_Pin GPIO_PIN_15
#define BUTTON_PAD_7_GPIO_Port GPIOB
#define TRANSFORMER_PWM_Pin GPIO_PIN_6
#define TRANSFORMER_PWM_GPIO_Port GPIOC
#define BUTTON_MT_Pin GPIO_PIN_7
#define BUTTON_MT_GPIO_Port GPIOC
#define BUTTON_NEXT_Pin GPIO_PIN_9
#define BUTTON_NEXT_GPIO_Port GPIOC
#define BUTTON_PREV_Pin GPIO_PIN_11
#define BUTTON_PREV_GPIO_Port GPIOA
#define BUTTON_4L_Pin GPIO_PIN_12
#define BUTTON_4L_GPIO_Port GPIOA
#define BUTTON_SLIP_Pin GPIO_PIN_13
#define BUTTON_SLIP_GPIO_Port GPIOA
#define BUTTON_VINYL_Pin GPIO_PIN_14
#define BUTTON_VINYL_GPIO_Port GPIOA
#define LED_MT_Pin GPIO_PIN_15
#define LED_MT_GPIO_Port GPIOA
#define SCK_Pin GPIO_PIN_10
#define SCK_GPIO_Port GPIOC
#define LAT_Pin GPIO_PIN_11
#define LAT_GPIO_Port GPIOC
#define SI_Pin GPIO_PIN_12
#define SI_GPIO_Port GPIOC
#define BK_Pin GPIO_PIN_2
#define BK_GPIO_Port GPIOD
#define BUTTON_8L_Pin GPIO_PIN_3
#define BUTTON_8L_GPIO_Port GPIOB
#define LED_RELOOP_Pin GPIO_PIN_4
#define LED_RELOOP_GPIO_Port GPIOB
#define BUTTON_RELOOP_Pin GPIO_PIN_5
#define BUTTON_RELOOP_GPIO_Port GPIOB
#define LED_8L_Pin GPIO_PIN_8
#define LED_8L_GPIO_Port GPIOB
#define LED_4L_Pin GPIO_PIN_9
#define LED_4L_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
