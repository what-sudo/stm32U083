/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#define BUFFER_SIZE  500

extern volatile uint8_t rx1_len;
extern volatile uint8_t rec1_end_flag;
extern uint8_t rx1_buffer[BUFFER_SIZE];

int DMA_Usart1_Send(uint8_t *buf,uint8_t len);
void Usart1_Handle(void);
void Usart1_IDLE(void);
/* USER CODE END Includes */

extern UART_HandleTypeDef hlpuart1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_LPUART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

