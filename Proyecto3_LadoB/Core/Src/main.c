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
typedef enum {
	NOT_AVAILABLE, AVAILABLE
} Disponibilidad;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Tabla de estados de pines (0 o 1) para cada valor de availableParkings (0 a 8)
#define PIN_STATES_SIZE 9
const uint8_t pinStates[PIN_STATES_SIZE] = {
    0b00111111,  // 0: Todos los segmentos menos G
    0b00000110,  // 1: B y C
    0b01011011,  // 2: A, B, G, E, D
    0b01001111,  // 3: A, B, G, C, D
    0b01100110,  // 4: F, G, B, C
    0b01101101,  // 5: A, F, G, C, D
    0b01111101,  // 6: A, F, G, E, D, C
    0b01000111,  // 7: A, B, C, G
    0b01111111   // 8: Todos los segmentos
};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t availableParkings = 0;
volatile Disponibilidad Sensor1 = AVAILABLE;
volatile Disponibilidad Sensor2 = AVAILABLE;
volatile Disponibilidad Sensor3 = AVAILABLE;
volatile Disponibilidad Sensor4 = AVAILABLE;
uint8_t dataReceived[10]; // Buffer para almacenar los datos recibidos
uint8_t DisponibilidadOtherSTM = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART5_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
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
  MX_UART5_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  void verificarAP(void) {
      static GPIO_TypeDef* const ports[] = {
          SSD_A_GPIO_Port, SSD_B_GPIO_Port, SSD_C_GPIO_Port,
          SSD_D_GPIO_Port, SSD_E_GPIO_Port, SSD_F_GPIO_Port,
          SSD_G_GPIO_Port
      };

      static const uint16_t pins[] = {
          SSD_A_Pin, SSD_B_Pin, SSD_C_Pin,
          SSD_D_Pin, SSD_E_Pin, SSD_F_Pin,
          SSD_G_Pin
      };

      // Lee el byte correspondiente al estado de los pines
      uint8_t estado = pinStates[availableParkings];

      // Itera sobre cada pin y configura su estado
      for (int i = 0; i < 7; i++) {
          HAL_GPIO_WritePin(ports[i], pins[i], (estado >> i) & 0x01);
      }
  }
  uint8_t contarSensoresDisponibles() {
      uint8_t contador = 0;
      contador = contador + DisponibilidadOtherSTM;
      if (Sensor1 == AVAILABLE) {
              contador++;
	  }
	  if (Sensor2 == AVAILABLE) {
		  contador++;
	  }
	  if (Sensor3 == AVAILABLE) {
		  contador++;
	  }
	  if (Sensor4 == AVAILABLE) {
		  contador++;
	  }
      return contador;
  }

  void verificarEstadoLEDS(void){
	  if (Sensor1 == AVAILABLE) {
		  HAL_GPIO_WritePin(LED_SENS1_G_GPIO_Port, LED_SENS1_G_Pin, 1);
		  HAL_GPIO_WritePin(LED_SENS1_R_GPIO_Port, LED_SENS1_R_Pin, 0);
	  }
	  if (Sensor1 == NOT_AVAILABLE) {
		  HAL_GPIO_WritePin(LED_SENS1_G_GPIO_Port, LED_SENS1_G_Pin, 0);
		  HAL_GPIO_WritePin(LED_SENS1_R_GPIO_Port, LED_SENS1_R_Pin, 1);
	  }
	  if (Sensor2 == AVAILABLE) {
		  HAL_GPIO_WritePin(LED_SENS2_G_GPIO_Port, LED_SENS2_G_Pin, 1);
		  HAL_GPIO_WritePin(LED_SENS2_R_GPIO_Port, LED_SENS2_R_Pin, 0);
	  }
	  if (Sensor2 == NOT_AVAILABLE) {
		  HAL_GPIO_WritePin(LED_SENS2_G_GPIO_Port, LED_SENS2_G_Pin, 0);
		  HAL_GPIO_WritePin(LED_SENS2_R_GPIO_Port, LED_SENS2_R_Pin, 1);
	  }
	  if (Sensor3 == AVAILABLE) {
		  HAL_GPIO_WritePin(LED_SENS3_G_GPIO_Port, LED_SENS3_G_Pin, 1);
		  HAL_GPIO_WritePin(LED_SENS3_R_GPIO_Port, LED_SENS3_R_Pin, 0);
	  }
	  if (Sensor3 == NOT_AVAILABLE) {
		  HAL_GPIO_WritePin(LED_SENS3_G_GPIO_Port, LED_SENS3_G_Pin, 0);
		  HAL_GPIO_WritePin(LED_SENS3_R_GPIO_Port, LED_SENS3_R_Pin, 1);
	  }
	  if (Sensor4 == AVAILABLE) {
		  HAL_GPIO_WritePin(LED_SENS4_G_GPIO_Port, LED_SENS4_G_Pin, 1);
		  HAL_GPIO_WritePin(LED_SENS4_R_GPIO_Port, LED_SENS4_R_Pin, 0);
	  }
	  if (Sensor4 == NOT_AVAILABLE) {
		  HAL_GPIO_WritePin(LED_SENS4_G_GPIO_Port, LED_SENS4_G_Pin, 0);
		  HAL_GPIO_WritePin(LED_SENS4_R_GPIO_Port, LED_SENS4_R_Pin, 1);
	  }
  }

  // Inicia la recepción en modo interrupción
  HAL_I2C_Slave_Receive_IT(&hi2c1, dataReceived, sizeof(dataReceived));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  availableParkings = contarSensoresDisponibles();
	  verificarAP();
	  verificarEstadoLEDS();
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 2;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_SENS3_G_Pin|LED_SENS3_R_Pin|LED_SENS4_R_Pin|LED_SENS4_G_Pin
                          |SSD_D_Pin|SSD_E_Pin|SSD_G_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_SENS1_R_Pin|LED_SENS1_G_Pin|LED_SENS2_R_Pin|SSD_A_Pin
                          |SSD_F_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_SENS2_G_Pin|SSD_C_Pin|SSD_B_Pin|LED_SENS1_RB7_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_SENS3_G_Pin LED_SENS3_R_Pin LED_SENS4_R_Pin LED_SENS4_G_Pin
                           SSD_D_Pin SSD_E_Pin SSD_G_Pin */
  GPIO_InitStruct.Pin = LED_SENS3_G_Pin|LED_SENS3_R_Pin|LED_SENS4_R_Pin|LED_SENS4_G_Pin
                          |SSD_D_Pin|SSD_E_Pin|SSD_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_SENS1_R_Pin LED_SENS1_G_Pin LED_SENS2_R_Pin SSD_A_Pin
                           SSD_F_Pin */
  GPIO_InitStruct.Pin = LED_SENS1_R_Pin|LED_SENS1_G_Pin|LED_SENS2_R_Pin|SSD_A_Pin
                          |SSD_F_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : USART_TX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(USART_TX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN2_Pin BTN3_Pin BTN4_Pin */
  GPIO_InitStruct.Pin = BTN2_Pin|BTN3_Pin|BTN4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_SENS2_G_Pin SSD_C_Pin SSD_B_Pin LED_SENS1_RB7_Pin */
  GPIO_InitStruct.Pin = LED_SENS2_G_Pin|SSD_C_Pin|SSD_B_Pin|LED_SENS1_RB7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BTN1_Pin */
  GPIO_InitStruct.Pin = BTN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BTN1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    //1
    if (GPIO_Pin == GPIO_PIN_8) {
    	switch (Sensor1) {
			case NOT_AVAILABLE:
				Sensor1 = AVAILABLE;
				break;
			case AVAILABLE:
				Sensor1 = NOT_AVAILABLE;
				break;
			default:
				break;
		}
    }
    if (GPIO_Pin == GPIO_PIN_5) {
	   switch (Sensor2) {
			case NOT_AVAILABLE:
				Sensor2 = AVAILABLE;
				break;
			case AVAILABLE:
				Sensor2 = NOT_AVAILABLE;
				break;
			default:
				break;
		}
    }
    if (GPIO_Pin == GPIO_PIN_6) {
    	switch (Sensor3) {
			case NOT_AVAILABLE:
				Sensor3 = AVAILABLE;
				break;
			case AVAILABLE:
				Sensor3 = NOT_AVAILABLE;
				break;
			default:
				break;
		}
    }
	if (GPIO_Pin == GPIO_PIN_7) {
		switch (Sensor4) {
			case NOT_AVAILABLE:
				availableParkings = availableParkings + 1;
				Sensor4 = AVAILABLE;
				break;
			case AVAILABLE:
				availableParkings = availableParkings - 1;
				Sensor4 = NOT_AVAILABLE;
				break;
			default:
				break;
		}
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C1) { // Asegúrarnos de que es el periférico I2C correcto

    	// AGREGAR LOGICA PARA RECIBIR CUANTOS HAY DISPONIBLES EN EL OTRO STM DESDE LA ESP32.

        // Volver a activar la bandera de recepcion
        HAL_I2C_Slave_Receive_IT(&hi2c1, dataReceived, sizeof(dataReceived));
    }
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
