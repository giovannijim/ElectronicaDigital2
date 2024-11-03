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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define LED_SENS3_G_Pin GPIO_PIN_0
#define LED_SENS3_G_GPIO_Port GPIOC
#define LED_SENS3_R_Pin GPIO_PIN_1
#define LED_SENS3_R_GPIO_Port GPIOC
#define LED_SENS4_R_Pin GPIO_PIN_2
#define LED_SENS4_R_GPIO_Port GPIOC
#define LED_SENS4_G_Pin GPIO_PIN_3
#define LED_SENS4_G_GPIO_Port GPIOC
#define LED_SENS1_R_Pin GPIO_PIN_0
#define LED_SENS1_R_GPIO_Port GPIOA
#define LED_SENS1_G_Pin GPIO_PIN_1
#define LED_SENS1_G_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define LED_SENS2_R_Pin GPIO_PIN_4
#define LED_SENS2_R_GPIO_Port GPIOA
#define BTN2_Pin GPIO_PIN_5
#define BTN2_GPIO_Port GPIOA
#define BTN2_EXTI_IRQn EXTI9_5_IRQn
#define BTN3_Pin GPIO_PIN_6
#define BTN3_GPIO_Port GPIOA
#define BTN3_EXTI_IRQn EXTI9_5_IRQn
#define BTN4_Pin GPIO_PIN_7
#define BTN4_GPIO_Port GPIOA
#define BTN4_EXTI_IRQn EXTI9_5_IRQn
#define SSD_D_Pin GPIO_PIN_5
#define SSD_D_GPIO_Port GPIOC
#define LED_SENS2_G_Pin GPIO_PIN_0
#define LED_SENS2_G_GPIO_Port GPIOB
#define SSD_C_Pin GPIO_PIN_2
#define SSD_C_GPIO_Port GPIOB
#define SSD_B_Pin GPIO_PIN_12
#define SSD_B_GPIO_Port GPIOB
#define SSD_E_Pin GPIO_PIN_6
#define SSD_E_GPIO_Port GPIOC
#define SSD_G_Pin GPIO_PIN_8
#define SSD_G_GPIO_Port GPIOC
#define SSD_A_Pin GPIO_PIN_11
#define SSD_A_GPIO_Port GPIOA
#define SSD_F_Pin GPIO_PIN_12
#define SSD_F_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LED_SENS1_RB7_Pin GPIO_PIN_7
#define LED_SENS1_RB7_GPIO_Port GPIOB
#define BTN1_Pin GPIO_PIN_8
#define BTN1_GPIO_Port GPIOB
#define BTN1_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
