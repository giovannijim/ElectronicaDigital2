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
uint8_t contador = 0;
uint8_t FlagInitGame = 0;
uint8_t P1_Points = 0;
uint8_t P2_Points = 0;
uint8_t Playable = 0;

uint8_t winner1 = 0;
uint8_t winner2 = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	// Si este boton es precionado, encender bandera de inicio de juego
	//  Apagar LEDs de los ganadores
	if(HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == 0){
		FlagInitGame = 1;
		HAL_GPIO_WritePin(P20_GPIO_Port, P20_Pin, 0);
		HAL_GPIO_WritePin(P10_GPIO_Port, P10_Pin, 0);
		/*
		HAL_GPIO_WritePin(P21_GPIO_Port, P21_Pin, 1);
		HAL_GPIO_WritePin(P22_GPIO_Port, P22_Pin, 1);
		HAL_GPIO_WritePin(P23_GPIO_Port, P23_Pin, 1);
		HAL_GPIO_WritePin(P24_GPIO_Port, P24_Pin, 1);
		HAL_GPIO_WritePin(P25_GPIO_Port, P25_Pin, 1);
		HAL_GPIO_WritePin(P26_GPIO_Port, P26_Pin, 1);
		HAL_GPIO_WritePin(P27_GPIO_Port, P27_Pin, 1);
		HAL_GPIO_WritePin(P28_GPIO_Port, P28_Pin, 1);*/
	}
	// Si la bandera esta encendida, proceder con la secuencia de inicio
	// La secuencia de incio es, prender los leds con un intervalo de tiempo
	// de por medio.
	if(FlagInitGame == 1){
		P1_Points = 0;
		P2_Points = 0;
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
		HAL_Delay(300);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
		HAL_Delay(300);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 1);
		HAL_Delay(300);
		FlagInitGame = 0;
		Playable = 1;
	}

	if (Playable == 1){
		// Apaga las leds de la secuencia de inicio
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 0);
		// Si se preciona el boton BTN2, verificar si contador < 8,
		// Si si lo es, sumar puntos, sino establecer banderas en 0
		// Establecer bandera ganador y apagar led de otro jugador
		if(HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == 0){
			if(P1_Points < 8){
			P1_Points++;
			HAL_Delay(150);
			} else{
				P1_Points = 0;
				winner1=1;
				Playable=0;
				HAL_GPIO_WritePin(P20_GPIO_Port, P20_Pin, 0);
			}
		}
		// Si se preciona el boton BTN3, verificar si contador < 8,
		// Si si lo es, sumar puntos, sino establecer banderas en 0
		// Establecer bandera ganador y apagar led de otro jugador
		if(HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) == 0){
			if(P2_Points < 8){
			P2_Points++;
			HAL_Delay(200);
			} else{
				P2_Points = 0;
				winner2=1;
				Playable=0;
				HAL_GPIO_WritePin(P10_GPIO_Port, P10_Pin, 0);
			}
		}

	}
	// Verificar que jugador gano y prender su respectivo LED
	if (winner1 == 1){
		HAL_GPIO_WritePin(P10_GPIO_Port, P10_Pin, 1);

	}
	if (winner2 == 1){
		HAL_GPIO_WritePin(P20_GPIO_Port, P20_Pin, 1);
		}
	// Verificar los puntos de cada jugador y en funcion del
	// puntaje, prender dicho led en una secuencia de forma
	// contador de decadas.
	if(P1_Points==0){
		HAL_GPIO_WritePin(P11_GPIO_Port, P11_Pin, 0);
		HAL_GPIO_WritePin(P12_GPIO_Port, P12_Pin, 0);
		HAL_GPIO_WritePin(P13_GPIO_Port, P13_Pin, 0);
		HAL_GPIO_WritePin(P14_GPIO_Port, P14_Pin, 0);
		HAL_GPIO_WritePin(P15_GPIO_Port, P15_Pin, 0);
		HAL_GPIO_WritePin(P16_GPIO_Port, P16_Pin, 0);
		HAL_GPIO_WritePin(P17_GPIO_Port, P17_Pin, 0);
		HAL_GPIO_WritePin(P18_GPIO_Port, P18_Pin, 0);
	}
	if(P1_Points==1){
		HAL_GPIO_WritePin(P11_GPIO_Port, P11_Pin, 1);
		HAL_GPIO_WritePin(P12_GPIO_Port, P12_Pin, 0);
		HAL_GPIO_WritePin(P13_GPIO_Port, P13_Pin, 0);
		HAL_GPIO_WritePin(P14_GPIO_Port, P14_Pin, 0);
		HAL_GPIO_WritePin(P15_GPIO_Port, P15_Pin, 0);
		HAL_GPIO_WritePin(P16_GPIO_Port, P16_Pin, 0);
		HAL_GPIO_WritePin(P17_GPIO_Port, P17_Pin, 0);
		HAL_GPIO_WritePin(P18_GPIO_Port, P18_Pin, 0);
	}
	if(P1_Points==2){
		HAL_GPIO_WritePin(P11_GPIO_Port, P11_Pin, 0);
		HAL_GPIO_WritePin(P12_GPIO_Port, P12_Pin, 1);
		HAL_GPIO_WritePin(P13_GPIO_Port, P13_Pin, 0);
		HAL_GPIO_WritePin(P14_GPIO_Port, P14_Pin, 0);
		HAL_GPIO_WritePin(P15_GPIO_Port, P15_Pin, 0);
		HAL_GPIO_WritePin(P16_GPIO_Port, P16_Pin, 0);
		HAL_GPIO_WritePin(P17_GPIO_Port, P17_Pin, 0);
		HAL_GPIO_WritePin(P18_GPIO_Port, P18_Pin, 0);
	}
	if(P1_Points==3){
		HAL_GPIO_WritePin(P11_GPIO_Port, P11_Pin, 0);
		HAL_GPIO_WritePin(P12_GPIO_Port, P12_Pin, 0);
		HAL_GPIO_WritePin(P13_GPIO_Port, P13_Pin, 1);
		HAL_GPIO_WritePin(P14_GPIO_Port, P14_Pin, 0);
		HAL_GPIO_WritePin(P15_GPIO_Port, P15_Pin, 0);
		HAL_GPIO_WritePin(P16_GPIO_Port, P16_Pin, 0);
		HAL_GPIO_WritePin(P17_GPIO_Port, P17_Pin, 0);
		HAL_GPIO_WritePin(P18_GPIO_Port, P18_Pin, 0);
	}
	if(P1_Points==4){
		HAL_GPIO_WritePin(P11_GPIO_Port, P11_Pin, 0);
		HAL_GPIO_WritePin(P12_GPIO_Port, P12_Pin, 0);
		HAL_GPIO_WritePin(P13_GPIO_Port, P13_Pin, 0);
		HAL_GPIO_WritePin(P14_GPIO_Port, P14_Pin, 1);
		HAL_GPIO_WritePin(P15_GPIO_Port, P15_Pin, 0);
		HAL_GPIO_WritePin(P16_GPIO_Port, P16_Pin, 0);
		HAL_GPIO_WritePin(P17_GPIO_Port, P17_Pin, 0);
		HAL_GPIO_WritePin(P18_GPIO_Port, P18_Pin, 0);
	}
	if(P1_Points==5){
		HAL_GPIO_WritePin(P11_GPIO_Port, P11_Pin, 0);
		HAL_GPIO_WritePin(P12_GPIO_Port, P12_Pin, 0);
		HAL_GPIO_WritePin(P13_GPIO_Port, P13_Pin, 0);
		HAL_GPIO_WritePin(P14_GPIO_Port, P14_Pin, 0);
		HAL_GPIO_WritePin(P15_GPIO_Port, P15_Pin, 1);
		HAL_GPIO_WritePin(P16_GPIO_Port, P16_Pin, 0);
		HAL_GPIO_WritePin(P17_GPIO_Port, P17_Pin, 0);
		HAL_GPIO_WritePin(P18_GPIO_Port, P18_Pin, 0);
	}
	if(P1_Points==6){
		HAL_GPIO_WritePin(P11_GPIO_Port, P11_Pin, 0);
		HAL_GPIO_WritePin(P12_GPIO_Port, P12_Pin, 0);
		HAL_GPIO_WritePin(P13_GPIO_Port, P13_Pin, 0);
		HAL_GPIO_WritePin(P14_GPIO_Port, P14_Pin, 0);
		HAL_GPIO_WritePin(P15_GPIO_Port, P15_Pin, 0);
		HAL_GPIO_WritePin(P16_GPIO_Port, P16_Pin, 1);
		HAL_GPIO_WritePin(P17_GPIO_Port, P17_Pin, 0);
		HAL_GPIO_WritePin(P18_GPIO_Port, P18_Pin, 0);
	}
	if(P1_Points==7){
		HAL_GPIO_WritePin(P11_GPIO_Port, P11_Pin, 0);
		HAL_GPIO_WritePin(P12_GPIO_Port, P12_Pin, 0);
		HAL_GPIO_WritePin(P13_GPIO_Port, P13_Pin, 0);
		HAL_GPIO_WritePin(P14_GPIO_Port, P14_Pin, 0);
		HAL_GPIO_WritePin(P15_GPIO_Port, P15_Pin, 0);
		HAL_GPIO_WritePin(P16_GPIO_Port, P16_Pin, 0);
		HAL_GPIO_WritePin(P17_GPIO_Port, P17_Pin, 1);
		HAL_GPIO_WritePin(P18_GPIO_Port, P18_Pin, 0);
	}
	if(P1_Points==8){
		HAL_GPIO_WritePin(P11_GPIO_Port, P11_Pin, 0);
		HAL_GPIO_WritePin(P12_GPIO_Port, P12_Pin, 0);
		HAL_GPIO_WritePin(P13_GPIO_Port, P13_Pin, 0);
		HAL_GPIO_WritePin(P14_GPIO_Port, P14_Pin, 0);
		HAL_GPIO_WritePin(P15_GPIO_Port, P15_Pin, 0);
		HAL_GPIO_WritePin(P16_GPIO_Port, P16_Pin, 0);
		HAL_GPIO_WritePin(P17_GPIO_Port, P17_Pin, 0);
		HAL_GPIO_WritePin(P18_GPIO_Port, P18_Pin, 1);
	}

	// PLAYER 2

	if(P2_Points==0){
		HAL_GPIO_WritePin(P21_GPIO_Port, P21_Pin, 0);
		HAL_GPIO_WritePin(P22_GPIO_Port, P22_Pin, 0);
		HAL_GPIO_WritePin(P23_GPIO_Port, P23_Pin, 0);
		HAL_GPIO_WritePin(P24_GPIO_Port, P24_Pin, 0);
		HAL_GPIO_WritePin(P25_GPIO_Port, P25_Pin, 0);
		HAL_GPIO_WritePin(P26_GPIO_Port, P26_Pin, 0);
		HAL_GPIO_WritePin(P27_GPIO_Port, P27_Pin, 0);
		HAL_GPIO_WritePin(P28_GPIO_Port, P28_Pin, 0);
	}
	if(P2_Points==1){
		HAL_GPIO_WritePin(P21_GPIO_Port, P21_Pin, 1);
		HAL_GPIO_WritePin(P22_GPIO_Port, P22_Pin, 0);
		HAL_GPIO_WritePin(P23_GPIO_Port, P23_Pin, 0);
		HAL_GPIO_WritePin(P24_GPIO_Port, P24_Pin, 0);
		HAL_GPIO_WritePin(P25_GPIO_Port, P25_Pin, 0);
		HAL_GPIO_WritePin(P26_GPIO_Port, P26_Pin, 0);
		HAL_GPIO_WritePin(P27_GPIO_Port, P27_Pin, 0);
		HAL_GPIO_WritePin(P28_GPIO_Port, P28_Pin, 0);
	}
	if(P2_Points==2){
		HAL_GPIO_WritePin(P21_GPIO_Port, P21_Pin, 0);
		HAL_GPIO_WritePin(P22_GPIO_Port, P22_Pin, 1);
		HAL_GPIO_WritePin(P23_GPIO_Port, P23_Pin, 0);
		HAL_GPIO_WritePin(P24_GPIO_Port, P24_Pin, 0);
		HAL_GPIO_WritePin(P25_GPIO_Port, P25_Pin, 0);
		HAL_GPIO_WritePin(P26_GPIO_Port, P26_Pin, 0);
		HAL_GPIO_WritePin(P27_GPIO_Port, P27_Pin, 0);
		HAL_GPIO_WritePin(P28_GPIO_Port, P28_Pin, 0);
	}
	if(P2_Points==3){
		HAL_GPIO_WritePin(P21_GPIO_Port, P21_Pin, 0);
		HAL_GPIO_WritePin(P22_GPIO_Port, P22_Pin, 0);
		HAL_GPIO_WritePin(P23_GPIO_Port, P23_Pin, 1);
		HAL_GPIO_WritePin(P24_GPIO_Port, P24_Pin, 0);
		HAL_GPIO_WritePin(P25_GPIO_Port, P25_Pin, 0);
		HAL_GPIO_WritePin(P26_GPIO_Port, P26_Pin, 0);
		HAL_GPIO_WritePin(P27_GPIO_Port, P27_Pin, 0);
		HAL_GPIO_WritePin(P28_GPIO_Port, P28_Pin, 0);
	}
	if(P2_Points==4){
		HAL_GPIO_WritePin(P21_GPIO_Port, P21_Pin, 0);
		HAL_GPIO_WritePin(P22_GPIO_Port, P22_Pin, 0);
		HAL_GPIO_WritePin(P23_GPIO_Port, P23_Pin, 0);
		HAL_GPIO_WritePin(P24_GPIO_Port, P24_Pin, 1);
		HAL_GPIO_WritePin(P25_GPIO_Port, P25_Pin, 0);
		HAL_GPIO_WritePin(P26_GPIO_Port, P26_Pin, 0);
		HAL_GPIO_WritePin(P27_GPIO_Port, P27_Pin, 0);
		HAL_GPIO_WritePin(P28_GPIO_Port, P28_Pin, 0);
	}
	if(P2_Points==5){
		HAL_GPIO_WritePin(P21_GPIO_Port, P21_Pin, 0);
		HAL_GPIO_WritePin(P22_GPIO_Port, P22_Pin, 0);
		HAL_GPIO_WritePin(P23_GPIO_Port, P23_Pin, 0);
		HAL_GPIO_WritePin(P24_GPIO_Port, P24_Pin, 0);
		HAL_GPIO_WritePin(P25_GPIO_Port, P25_Pin, 1);
		HAL_GPIO_WritePin(P26_GPIO_Port, P26_Pin, 0);
		HAL_GPIO_WritePin(P27_GPIO_Port, P27_Pin, 0);
		HAL_GPIO_WritePin(P28_GPIO_Port, P28_Pin, 0);
	}
	if(P2_Points==6){
		HAL_GPIO_WritePin(P21_GPIO_Port, P21_Pin, 0);
		HAL_GPIO_WritePin(P22_GPIO_Port, P22_Pin, 0);
		HAL_GPIO_WritePin(P23_GPIO_Port, P23_Pin, 0);
		HAL_GPIO_WritePin(P24_GPIO_Port, P24_Pin, 0);
		HAL_GPIO_WritePin(P25_GPIO_Port, P25_Pin, 0);
		HAL_GPIO_WritePin(P26_GPIO_Port, P26_Pin, 1);
		HAL_GPIO_WritePin(P27_GPIO_Port, P27_Pin, 0);
		HAL_GPIO_WritePin(P28_GPIO_Port, P28_Pin, 0);
	}
	if(P2_Points==7){
		HAL_GPIO_WritePin(P21_GPIO_Port, P21_Pin, 0);
		HAL_GPIO_WritePin(P22_GPIO_Port, P22_Pin, 0);
		HAL_GPIO_WritePin(P23_GPIO_Port, P23_Pin, 0);
		HAL_GPIO_WritePin(P24_GPIO_Port, P24_Pin, 0);
		HAL_GPIO_WritePin(P25_GPIO_Port, P25_Pin, 0);
		HAL_GPIO_WritePin(P26_GPIO_Port, P26_Pin, 0);
		HAL_GPIO_WritePin(P27_GPIO_Port, P27_Pin, 1);
		HAL_GPIO_WritePin(P28_GPIO_Port, P28_Pin, 0);
	}
	if(P2_Points==8){
		HAL_GPIO_WritePin(P21_GPIO_Port, P21_Pin, 0);
		HAL_GPIO_WritePin(P22_GPIO_Port, P22_Pin, 0);
		HAL_GPIO_WritePin(P23_GPIO_Port, P23_Pin, 0);
		HAL_GPIO_WritePin(P24_GPIO_Port, P24_Pin, 0);
		HAL_GPIO_WritePin(P25_GPIO_Port, P25_Pin, 0);
		HAL_GPIO_WritePin(P26_GPIO_Port, P26_Pin, 0);
		HAL_GPIO_WritePin(P27_GPIO_Port, P27_Pin, 0);
		HAL_GPIO_WritePin(P28_GPIO_Port, P28_Pin, 1);
	}

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|LED2_Pin|LED3_Pin|P16_Pin
                          |P17_Pin|P11_Pin|P21_Pin|P20_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, P28_Pin|P18_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, P24_Pin|P23_Pin|P15_Pin|P22_Pin
                          |P27_Pin|P26_Pin|P25_Pin|P12_Pin
                          |P14_Pin|P13_Pin|P10_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BTN_Pin BTN2_Pin BTN3_Pin */
  GPIO_InitStruct.Pin = BTN_Pin|BTN2_Pin|BTN3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin P16_Pin
                           P17_Pin P11_Pin P21_Pin P20_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|P16_Pin
                          |P17_Pin|P11_Pin|P21_Pin|P20_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : P28_Pin P18_Pin */
  GPIO_InitStruct.Pin = P28_Pin|P18_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : P24_Pin P23_Pin P15_Pin P22_Pin
                           P27_Pin P26_Pin P25_Pin P12_Pin
                           P14_Pin P13_Pin P10_Pin */
  GPIO_InitStruct.Pin = P24_Pin|P23_Pin|P15_Pin|P22_Pin
                          |P27_Pin|P26_Pin|P25_Pin|P12_Pin
                          |P14_Pin|P13_Pin|P10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
