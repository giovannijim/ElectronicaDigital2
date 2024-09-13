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
#include "stm32f4xx_hal.h"

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
#define BTN_Pin GPIO_PIN_13
#define BTN_GPIO_Port GPIOC
#define BTN2_Pin GPIO_PIN_0
#define BTN2_GPIO_Port GPIOC
#define BTN3_Pin GPIO_PIN_1
#define BTN3_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_5
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_6
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_7
#define LED3_GPIO_Port GPIOA
#define P28_Pin GPIO_PIN_4
#define P28_GPIO_Port GPIOC
#define P24_Pin GPIO_PIN_1
#define P24_GPIO_Port GPIOB
#define P23_Pin GPIO_PIN_2
#define P23_GPIO_Port GPIOB
#define P15_Pin GPIO_PIN_10
#define P15_GPIO_Port GPIOB
#define P22_Pin GPIO_PIN_12
#define P22_GPIO_Port GPIOB
#define P27_Pin GPIO_PIN_13
#define P27_GPIO_Port GPIOB
#define P26_Pin GPIO_PIN_14
#define P26_GPIO_Port GPIOB
#define P25_Pin GPIO_PIN_15
#define P25_GPIO_Port GPIOB
#define P18_Pin GPIO_PIN_7
#define P18_GPIO_Port GPIOC
#define P16_Pin GPIO_PIN_8
#define P16_GPIO_Port GPIOA
#define P17_Pin GPIO_PIN_9
#define P17_GPIO_Port GPIOA
#define P11_Pin GPIO_PIN_10
#define P11_GPIO_Port GPIOA
#define P21_Pin GPIO_PIN_11
#define P21_GPIO_Port GPIOA
#define P20_Pin GPIO_PIN_12
#define P20_GPIO_Port GPIOA
#define P12_Pin GPIO_PIN_3
#define P12_GPIO_Port GPIOB
#define P14_Pin GPIO_PIN_4
#define P14_GPIO_Port GPIOB
#define P13_Pin GPIO_PIN_5
#define P13_GPIO_Port GPIOB
#define P10_Pin GPIO_PIN_6
#define P10_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
