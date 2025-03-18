/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : SAI.c
  * Description        : This file provides code for the configuration
  *                      of the SAI instances.
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
#include "sai.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SAI_HandleTypeDef hsai_BlockA1;
SAI_HandleTypeDef hsai_BlockA2;
SAI_HandleTypeDef hsai_BlockA3;
SAI_HandleTypeDef hsai_BlockB3;

/* SAI1 init function */
void MX_SAI1_Init(void)
{

  /* USER CODE BEGIN SAI1_Init 0 */

  /* USER CODE END SAI1_Init 0 */

  /* USER CODE BEGIN SAI1_Init 1 */

  /* USER CODE END SAI1_Init 1 */

  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.Protocol = SAI_FREE_PROTOCOL;
  hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_TX;
  hsai_BlockA1.Init.DataSize = SAI_DATASIZE_16;
  hsai_BlockA1.Init.FirstBit = SAI_FIRSTBIT_MSB;
  hsai_BlockA1.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
  hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_44K;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_OUTBLOCKA_ENABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  hsai_BlockA1.Init.PdmInit.Activation = DISABLE;
  hsai_BlockA1.Init.PdmInit.MicPairsNbr = 1;
  hsai_BlockA1.Init.PdmInit.ClockEnable = SAI_PDM_CLOCK1_ENABLE;
  hsai_BlockA1.FrameInit.FrameLength = 64;
  hsai_BlockA1.FrameInit.ActiveFrameLength = 32;
  hsai_BlockA1.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
  hsai_BlockA1.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
  hsai_BlockA1.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;
  hsai_BlockA1.SlotInit.FirstBitOffset = 0;
  hsai_BlockA1.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
  hsai_BlockA1.SlotInit.SlotNumber = 2;
  hsai_BlockA1.SlotInit.SlotActive = 0x00000003;
  if (HAL_SAI_Init(&hsai_BlockA1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SAI1_Init 2 */

  /* USER CODE END SAI1_Init 2 */

}
/* SAI2 init function */
void MX_SAI2_Init(void)
{

  /* USER CODE BEGIN SAI2_Init 0 */

  /* USER CODE END SAI2_Init 0 */

  /* USER CODE BEGIN SAI2_Init 1 */

  /* USER CODE END SAI2_Init 1 */

  hsai_BlockA2.Instance = SAI2_Block_A;
  hsai_BlockA2.Init.Protocol = SAI_FREE_PROTOCOL;
  hsai_BlockA2.Init.AudioMode = SAI_MODESLAVE_TX;
  hsai_BlockA2.Init.DataSize = SAI_DATASIZE_16;
  hsai_BlockA2.Init.FirstBit = SAI_FIRSTBIT_MSB;
  hsai_BlockA2.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
  hsai_BlockA2.Init.Synchro = SAI_SYNCHRONOUS_EXT_SAI1;
  hsai_BlockA2.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
  hsai_BlockA2.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA2.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockA2.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA2.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  hsai_BlockA2.Init.PdmInit.Activation = DISABLE;
  hsai_BlockA2.Init.PdmInit.MicPairsNbr = 1;
  hsai_BlockA2.Init.PdmInit.ClockEnable = SAI_PDM_CLOCK1_ENABLE;
  hsai_BlockA2.FrameInit.FrameLength = 64;
  hsai_BlockA2.FrameInit.ActiveFrameLength = 32;
  hsai_BlockA2.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
  hsai_BlockA2.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
  hsai_BlockA2.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;
  hsai_BlockA2.SlotInit.FirstBitOffset = 0;
  hsai_BlockA2.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
  hsai_BlockA2.SlotInit.SlotNumber = 2;
  hsai_BlockA2.SlotInit.SlotActive = 0x00000003;
  if (HAL_SAI_Init(&hsai_BlockA2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SAI2_Init 2 */

  /* USER CODE END SAI2_Init 2 */

}
/* SAI3 init function */
void MX_SAI3_Init(void)
{

  /* USER CODE BEGIN SAI3_Init 0 */

  /* USER CODE END SAI3_Init 0 */

  /* USER CODE BEGIN SAI3_Init 1 */

  /* USER CODE END SAI3_Init 1 */

  /* USER CODE BEGIN SAI3_Init 1 */

  /* USER CODE END SAI3_Init 1 */

  hsai_BlockA3.Instance = SAI3_Block_A;
  hsai_BlockA3.Init.Protocol = SAI_FREE_PROTOCOL;
  hsai_BlockA3.Init.AudioMode = SAI_MODESLAVE_RX;
  hsai_BlockA3.Init.DataSize = SAI_DATASIZE_16;
  hsai_BlockA3.Init.FirstBit = SAI_FIRSTBIT_MSB;
  hsai_BlockA3.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
  hsai_BlockA3.Init.Synchro = SAI_SYNCHRONOUS_EXT_SAI1;
  hsai_BlockA3.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
  hsai_BlockA3.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA3.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockA3.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA3.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  hsai_BlockA3.Init.PdmInit.Activation = DISABLE;
  hsai_BlockA3.Init.PdmInit.MicPairsNbr = 1;
  hsai_BlockA3.Init.PdmInit.ClockEnable = SAI_PDM_CLOCK1_ENABLE;
  hsai_BlockA3.FrameInit.FrameLength = 64;
  hsai_BlockA3.FrameInit.ActiveFrameLength = 32;
  hsai_BlockA3.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
  hsai_BlockA3.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
  hsai_BlockA3.FrameInit.FSOffset = SAI_FS_FIRSTBIT;
  hsai_BlockA3.SlotInit.FirstBitOffset = 0;
  hsai_BlockA3.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
  hsai_BlockA3.SlotInit.SlotNumber = 2;
  hsai_BlockA3.SlotInit.SlotActive = 0x00000003;
  if (HAL_SAI_Init(&hsai_BlockA3) != HAL_OK)
  {
    Error_Handler();
  }
  hsai_BlockB3.Instance = SAI3_Block_B;
  hsai_BlockB3.Init.Protocol = SAI_FREE_PROTOCOL;
  hsai_BlockB3.Init.AudioMode = SAI_MODESLAVE_RX;
  hsai_BlockB3.Init.DataSize = SAI_DATASIZE_16;
  hsai_BlockB3.Init.FirstBit = SAI_FIRSTBIT_MSB;
  hsai_BlockB3.Init.ClockStrobing = SAI_CLOCKSTROBING_RISINGEDGE;
  hsai_BlockB3.Init.Synchro = SAI_SYNCHRONOUS;
  hsai_BlockB3.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
  hsai_BlockB3.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockB3.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockB3.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockB3.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  hsai_BlockB3.Init.PdmInit.Activation = DISABLE;
  hsai_BlockB3.Init.PdmInit.MicPairsNbr = 1;
  hsai_BlockB3.Init.PdmInit.ClockEnable = SAI_PDM_CLOCK1_ENABLE;
  hsai_BlockB3.FrameInit.FrameLength = 64;
  hsai_BlockB3.FrameInit.ActiveFrameLength = 32;
  hsai_BlockB3.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
  hsai_BlockB3.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
  hsai_BlockB3.FrameInit.FSOffset = SAI_FS_FIRSTBIT;
  hsai_BlockB3.SlotInit.FirstBitOffset = 0;
  hsai_BlockB3.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
  hsai_BlockB3.SlotInit.SlotNumber = 2;
  hsai_BlockB3.SlotInit.SlotActive = 0x00000003;
  if (HAL_SAI_Init(&hsai_BlockB3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SAI3_Init 2 */

  /* USER CODE END SAI3_Init 2 */

}
static uint32_t SAI1_client =0;
static uint32_t SAI2_client =0;
static uint32_t SAI3_client =0;

void HAL_SAI_MspInit(SAI_HandleTypeDef* saiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
/* SAI1 */
    if(saiHandle->Instance==SAI1_Block_A)
    {
    /* SAI1 clock enable */
    if (SAI1_client == 0)
    {
       __HAL_RCC_SAI1_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(SAI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SAI1_IRQn);
    }
    SAI1_client ++;

    /**SAI1_A_Block_A GPIO Configuration
    PE2     ------> SAI1_MCLK_A
    PE4     ------> SAI1_FS_A
    PE5     ------> SAI1_SCK_A
    PE6     ------> SAI1_SD_A
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    }
/* SAI2 */
    if(saiHandle->Instance==SAI2_Block_A)
    {
    /* SAI2 clock enable */
    if (SAI2_client == 0)
    {
       __HAL_RCC_SAI2_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(SAI2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(SAI2_IRQn);
    }
    SAI2_client ++;

    /**SAI2_A_Block_A GPIO Configuration
    PD11     ------> SAI2_SD_A
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF10_SAI2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    }
/* SAI3 */
    if(saiHandle->Instance==SAI3_Block_A)
    {
    /* SAI3 clock enable */
    if (SAI3_client == 0)
    {
       __HAL_RCC_SAI3_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(SAI3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(SAI3_IRQn);
    }
    SAI3_client ++;

    /**SAI3_A_Block_A GPIO Configuration
    PD1     ------> SAI3_SD_A
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SAI3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    }
    if(saiHandle->Instance==SAI3_Block_B)
    {
      /* SAI3 clock enable */
      if (SAI3_client == 0)
      {
       __HAL_RCC_SAI3_CLK_ENABLE();

      /* Peripheral interrupt init*/
      HAL_NVIC_SetPriority(SAI3_IRQn, 2, 0);
      HAL_NVIC_EnableIRQ(SAI3_IRQn);
      }
    SAI3_client ++;

    /**SAI3_B_Block_B GPIO Configuration
    PD9     ------> SAI3_SD_B
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SAI3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    }
}

void HAL_SAI_MspDeInit(SAI_HandleTypeDef* saiHandle)
{

/* SAI1 */
    if(saiHandle->Instance==SAI1_Block_A)
    {
    SAI1_client --;
    if (SAI1_client == 0)
      {
      /* Peripheral clock disable */
       __HAL_RCC_SAI1_CLK_DISABLE();
      HAL_NVIC_DisableIRQ(SAI1_IRQn);
      }

    /**SAI1_A_Block_A GPIO Configuration
    PE2     ------> SAI1_MCLK_A
    PE4     ------> SAI1_FS_A
    PE5     ------> SAI1_SCK_A
    PE6     ------> SAI1_SD_A
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

    }
/* SAI2 */
    if(saiHandle->Instance==SAI2_Block_A)
    {
    SAI2_client --;
    if (SAI2_client == 0)
      {
      /* Peripheral clock disable */
       __HAL_RCC_SAI2_CLK_DISABLE();
      HAL_NVIC_DisableIRQ(SAI2_IRQn);
      }

    /**SAI2_A_Block_A GPIO Configuration
    PD11     ------> SAI2_SD_A
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_11);

    }
/* SAI3 */
    if(saiHandle->Instance==SAI3_Block_A)
    {
    SAI3_client --;
    if (SAI3_client == 0)
      {
      /* Peripheral clock disable */
       __HAL_RCC_SAI3_CLK_DISABLE();
      HAL_NVIC_DisableIRQ(SAI3_IRQn);
      }

    /**SAI3_A_Block_A GPIO Configuration
    PD1     ------> SAI3_SD_A
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_1);

    }
    if(saiHandle->Instance==SAI3_Block_B)
    {
    SAI3_client --;
      if (SAI3_client == 0)
      {
      /* Peripheral clock disable */
      __HAL_RCC_SAI3_CLK_DISABLE();
      HAL_NVIC_DisableIRQ(SAI3_IRQn);
      }

    /**SAI3_B_Block_B GPIO Configuration
    PD9     ------> SAI3_SD_B
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_9);

    }
}

/**
  * @}
  */

/**
  * @}
  */
