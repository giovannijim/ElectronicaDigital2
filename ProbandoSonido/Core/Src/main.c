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
#define TIMER_CLK 84000000  // 84 MHz clock
#define PRESCALER 127
#define COUNTER_PERIOD 20
// Define notes for Star Wars melody
// Formula to calculate ARR for a given note frequency
#define NOTE_FREQ_TO_ARR(freq) ((TIMER_CLK) / ((PRESCALER + 1) * (freq)))
#define NOTE_A4  NOTE_FREQ_TO_ARR(440)    // A4, 440 Hz
#define NOTE_A5  NOTE_FREQ_TO_ARR(440)    // Transpose A5 to A4
#define NOTE_F4  NOTE_FREQ_TO_ARR(349)    // F4, 349 Hz
#define NOTE_C5  NOTE_FREQ_TO_ARR(523)    // C5, 523 Hz
#define NOTE_E5  NOTE_FREQ_TO_ARR(659)    // E5, 659 Hz
#define NOTE_GS4 NOTE_FREQ_TO_ARR(415)    // G#4, 415 Hz
#define NOTE_F5  NOTE_FREQ_TO_ARR(349)    // Transpose F5 to F4
#define NOTE_GS5 NOTE_FREQ_TO_ARR(415)    // Transpose G#5 to G#4
#define NOTE_G5  NOTE_FREQ_TO_ARR(392)    // Transpose G5 to G4
#define NOTE_FS5 NOTE_FREQ_TO_ARR(370)    // Transpose F#5 to F#4
#define NOTE_AS5 NOTE_FREQ_TO_ARR(466)    // A#5, 466 Hz
#define NOTE_DS5 NOTE_FREQ_TO_ARR(622)    // D#5, 622 Hz
#define NOTE_D5  NOTE_FREQ_TO_ARR(587)    // D5, 587 Hz
#define NOTE_CS5 NOTE_FREQ_TO_ARR(554)    // C#5, 554 Hz
#define NOTE_AS4 NOTE_FREQ_TO_ARR(466)    // A#4, 466 Hz

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void beep(uint16_t ARR_value, uint16_t duration_ms) {
    // Set the auto-reload register (ARR) value for the desired frequency
    __HAL_TIM_SET_AUTORELOAD(&htim2, ARR_value);

    // Set the duty cycle to 50% (for square wave)
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, ARR_value / 2);

    // Start PWM
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    // Play the sound for the specified duration
    HAL_Delay(duration_ms);

    // Stop PWM (silence the buzzer)
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_4);
}


void PlayMelody() {
	// First section
	    beep(NOTE_A4, 500);
	    beep(NOTE_A4, 500);
	    beep(NOTE_A4, 500);
	    beep(NOTE_F4, 350);
	    beep(NOTE_C5, 150);
	    beep(NOTE_A4, 500);
	    beep(NOTE_F4, 350);
	    beep(NOTE_C5, 150);
	    beep(NOTE_A4, 650);
	    HAL_Delay(500);

	    beep(NOTE_E5, 500);
	    beep(NOTE_E5, 500);
	    beep(NOTE_E5, 500);
	    beep(NOTE_F5, 350);
	    beep(NOTE_C5, 150);
	    beep(NOTE_GS4, 500);
	    beep(NOTE_F4, 350);
	    beep(NOTE_C5, 150);
	    beep(NOTE_A4, 650);
	    HAL_Delay(500);

	    // Second section
	    beep(NOTE_A5, 500);
	    beep(NOTE_A4, 300);
	    beep(NOTE_A4, 150);
	    beep(NOTE_A5, 500);
	    beep(NOTE_GS5, 325);
	    beep(NOTE_G5, 175);
	    beep(NOTE_FS5, 125);
	    beep(NOTE_F5, 125);
	    beep(NOTE_FS5, 250);
	    HAL_Delay(325);

	    beep(NOTE_AS5, 250);
	    beep(NOTE_DS5, 500);
	    beep(NOTE_D5, 325);
	    beep(NOTE_CS5, 175);
	    beep(NOTE_C5, 125);
	    beep(NOTE_AS4, 125);
	    beep(NOTE_C5, 250);
	    HAL_Delay(350);

	    // Variant 1
	    beep(NOTE_F4, 250);
	    beep(NOTE_GS4, 500);
	    beep(NOTE_F4, 350);
	    beep(NOTE_A4, 125);
	    beep(NOTE_C5, 500);
	    beep(NOTE_A4, 375);
	    beep(NOTE_C5, 125);
	    beep(NOTE_E5, 650);
	    HAL_Delay(500);

	    // Second section again
	    beep(NOTE_A5, 500);
	    beep(NOTE_A4, 300);
	    beep(NOTE_A4, 150);
	    beep(NOTE_A5, 500);
	    beep(NOTE_GS5, 325);
	    beep(NOTE_G5, 175);
	    beep(NOTE_FS5, 125);
	    beep(NOTE_F5, 125);
	    beep(NOTE_FS5, 250);
	    HAL_Delay(325);

	    beep(NOTE_AS5, 250);
	    beep(NOTE_DS5, 500);
	    beep(NOTE_D5, 325);
	    beep(NOTE_CS5, 175);
	    beep(NOTE_C5, 125);
	    beep(NOTE_AS4, 125);
	    beep(NOTE_C5, 250);
	    HAL_Delay(350);

	    // Variant 2
	    beep(NOTE_F4, 250);
	    beep(NOTE_GS4, 500);
	    beep(NOTE_F4, 375);
	    beep(NOTE_C5, 125);
	    beep(NOTE_A4, 500);
	    beep(NOTE_F4, 375);
	    beep(NOTE_C5, 125);
	    beep(NOTE_A4, 650);
	    HAL_Delay(650);
}

void nonBlockingDelay(uint32_t delay) {
    uint32_t start = HAL_GetTick();
    while ((HAL_GetTick() - start) < delay) {
        // Do nothing, just wait for the delay to pass
    }
}

void playMelody2() {
    static uint8_t noteIndex = 0;
    uint16_t notes[] = {NOTE_A4, NOTE_C5, NOTE_E5};
    uint16_t durations[] = {500, 500, 500};
    uint8_t numNotes = sizeof(notes) / sizeof(notes[0]);

    if (noteIndex < numNotes) {
        beep(notes[noteIndex], durations[noteIndex]);
        nonBlockingDelay(100);
        noteIndex++;
    } else {
        noteIndex = 0;  // Restart melody
    }
}
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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  beep(NOTE_A4, 500);
	  	    beep(NOTE_A4, 500);
	  	    beep(NOTE_A4, 500);
	  	    beep(NOTE_F4, 350);
	  	    beep(NOTE_C5, 150);
	  	    beep(NOTE_A4, 500);
	  	    beep(NOTE_F4, 350);
	  	    beep(NOTE_C5, 150);
	  	    beep(NOTE_A4, 650);
	  	    HAL_Delay(500);

	  	    beep(NOTE_E5, 500);
	  	    beep(NOTE_E5, 500);
	  	    beep(NOTE_E5, 500);
	  	    beep(NOTE_F5, 350);
	  	    beep(NOTE_C5, 150);
	  	    beep(NOTE_GS4, 500);
	  	    beep(NOTE_F4, 350);
	  	    beep(NOTE_C5, 150);
	  	    beep(NOTE_A4, 650);
	  	    HAL_Delay(500);

	  	    // Second section
	  	    beep(NOTE_A5, 500);
	  	    beep(NOTE_A4, 300);
	  	    beep(NOTE_A4, 150);
	  	    beep(NOTE_A5, 500);
	  	    beep(NOTE_GS5, 325);
	  	    beep(NOTE_G5, 175);
	  	    beep(NOTE_FS5, 125);
	  	    beep(NOTE_F5, 125);
	  	    beep(NOTE_FS5, 250);
	  	    HAL_Delay(325);

	  	    beep(NOTE_AS5, 250);
	  	    beep(NOTE_DS5, 500);
	  	    beep(NOTE_D5, 325);
	  	    beep(NOTE_CS5, 175);
	  	    beep(NOTE_C5, 125);
	  	    beep(NOTE_AS4, 125);
	  	    beep(NOTE_C5, 250);
	  	    HAL_Delay(350);

	  	    // Variant 1
	  	    beep(NOTE_F4, 250);
	  	    beep(NOTE_GS4, 500);
	  	    beep(NOTE_F4, 350);
	  	    beep(NOTE_A4, 125);
	  	    beep(NOTE_C5, 500);
	  	    beep(NOTE_A4, 375);
	  	    beep(NOTE_C5, 125);
	  	    beep(NOTE_E5, 650);
	  	    HAL_Delay(500);

	  	    // Second section again
	  	    beep(NOTE_A5, 500);
	  	    beep(NOTE_A4, 300);
	  	    beep(NOTE_A4, 150);
	  	    beep(NOTE_A5, 500);
	  	    beep(NOTE_GS5, 325);
	  	    beep(NOTE_G5, 175);
	  	    beep(NOTE_FS5, 125);
	  	    beep(NOTE_F5, 125);
	  	    beep(NOTE_FS5, 250);
	  	    HAL_Delay(325);

	  	    beep(NOTE_AS5, 250);
	  	    beep(NOTE_DS5, 500);
	  	    beep(NOTE_D5, 325);
	  	    beep(NOTE_CS5, 175);
	  	    beep(NOTE_C5, 125);
	  	    beep(NOTE_AS4, 125);
	  	    beep(NOTE_C5, 250);
	  	    HAL_Delay(350);

	  	    // Variant 2
	  	    beep(NOTE_F4, 250);
	  	    beep(NOTE_GS4, 500);
	  	    beep(NOTE_F4, 375);
	  	    beep(NOTE_C5, 125);
	  	    beep(NOTE_A4, 500);
	  	    beep(NOTE_F4, 375);
	  	    beep(NOTE_C5, 125);
	  	    beep(NOTE_A4, 650);
	  	    HAL_Delay(650);
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
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 127;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

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
