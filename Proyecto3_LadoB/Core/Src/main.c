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
#include "string.h"
#include "stdio.h"
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
#define STM32_SLAVE_ADDR 0x01  // Dirección del STM32 esclavo
#define ESP32_SLAVE_ADDR 0x02  // Dirección del ESP32 esclavo
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t availableParkings = 0;
volatile Disponibilidad Sensor1 = AVAILABLE;
volatile Disponibilidad Sensor2 = AVAILABLE;
volatile Disponibilidad Sensor3 = AVAILABLE;
volatile Disponibilidad Sensor4 = AVAILABLE;
uint8_t dataReceived; // Buffer para almacenar los datos recibidos
uint8_t DisponibilidadOtherSTM = 0;
uint8_t dataToSend = 0x55;
uint8_t receivedData[8] = {};
volatile uint8_t dataAvailable = 0;
uint8_t bufferRx[1] = {};
uint8_t responseData = 0xAA;  // Byte de respuesta de prueba
//Variables I2C
uint8_t I2C_Rx[1];
uint8_t Rx[1];
uint8_t Tx[1];
uint8_t byteControl= 0xF0;
// Variables para antirrebote y conteo por botón
uint32_t previousMillis[4] = {0, 0, 0, 0};
uint32_t currentMillis = 0;
uint32_t counterOutside = 0;
uint32_t counterInside[4] = {0, 0, 0, 0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
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
      if (byteControl & 0x01) {
              contador++;
	  }
	  if (byteControl & 0x02) {
		  contador++;
	  }
	  if (byteControl & 0x04) {
		  contador++;
	  }
	  if (byteControl & 0x08) {
		  contador++;
	  }
	  if (byteControl & 0x10) {
		  contador++;
	  }
	  if (byteControl & 0x20) {
		  contador++;
	  }
	  if (byteControl & 0x40) {
		  contador++;
	  }
	  if (byteControl & 0x80) {
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
  HAL_I2C_Slave_Receive_IT(&hi2c1, receivedData, 1);  // Prepararse para recibir otro byte
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
/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    //1


    if (GPIO_Pin == GPIO_PIN_8) {
    	switch (Sensor1) {
			case NOT_AVAILABLE:
				Sensor1 = AVAILABLE;
				byteControl^= 0b00010000;
				Tx[0]=byteControl;
				Rx[0]=byteControl;
				break;
			case AVAILABLE:
				Sensor1 = NOT_AVAILABLE;
				byteControl^= 0b00010000;
				Tx[0]=byteControl;
				Rx[0]=byteControl;
				break;
			default:
				break;
		}
    }
    if (GPIO_Pin == GPIO_PIN_5) {
	   switch (Sensor2) {
			case NOT_AVAILABLE:
				Sensor2 = AVAILABLE;
				byteControl^= 0b00100000;
				Tx[0]=byteControl;
				Rx[0]=byteControl;
				break;
			case AVAILABLE:
				Sensor2 = NOT_AVAILABLE;
				byteControl^= 0b00100000;
				Tx[0]=byteControl;
				Rx[0]=byteControl;
				break;
			default:
				break;
		}
    }
    if (GPIO_Pin == GPIO_PIN_6) {
    	switch (Sensor3) {
			case NOT_AVAILABLE:
				Sensor3 = AVAILABLE;
				byteControl^= 0b01000000;
				Tx[0]=byteControl;
				Rx[0]=byteControl;
				break;
			case AVAILABLE:
				Sensor3 = NOT_AVAILABLE;
				byteControl^= 0b01000000;
				Tx[0]=byteControl;
				Rx[0]=byteControl;
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
				byteControl^= 0b10000000;
				Tx[0]=byteControl;
				Rx[0]=byteControl;
				break;
			case AVAILABLE:
				availableParkings = availableParkings - 1;
				Sensor4 = NOT_AVAILABLE;
				byteControl^= 0b10000000;
				Tx[0]=byteControl;
				Rx[0]=byteControl;
				break;
			default:
				break;
		}
	}

}*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    counterOutside++; // Para pruebas
    currentMillis = HAL_GetTick();

    // Verificar qué botón fue presionado y aplicar el antirrebote
    if (GPIO_Pin == GPIO_PIN_8 && (currentMillis - previousMillis[0] > 250))
    {
        counterInside[0]++; // Para pruebas del botón 1
        previousMillis[0] = currentMillis;
        byteControl^= 0b00010000;
        Tx[0]=byteControl;
        Rx[0]=byteControl;
        //HAL_UART_Transmit_IT(&huart2, Tx, sizeof(Tx));
    }
    else if (GPIO_Pin == GPIO_PIN_5 && (currentMillis - previousMillis[1] > 250))
    {
        counterInside[1]++; // Para pruebas del botón 2
        previousMillis[1] = currentMillis;
        byteControl^= 0b00100000;
        Tx[0]=byteControl;
        Rx[0]=byteControl;
        //HAL_UART_Transmit_IT(&huart2, Tx, sizeof(Tx));

    }
    else if (GPIO_Pin == GPIO_PIN_6 && (currentMillis - previousMillis[2] > 250))
    {
        counterInside[2]++; // Para pruebas del botón 3
        previousMillis[2] = currentMillis;
        byteControl^= 0b01000000;
        Tx[0]=byteControl;
        Rx[0]=byteControl;
        //HAL_UART_Transmit_IT(&huart2, Tx, sizeof(Tx));
    }
    else if (GPIO_Pin == GPIO_PIN_7 && (currentMillis - previousMillis[3] > 250))
    {
        counterInside[3]++; // Para pruebas del botón 4

        previousMillis[3] = currentMillis;
        byteControl^= 0b10000000;
		Tx[0]=byteControl;
		Rx[0]=byteControl;
		//HAL_UART_Transmit_IT(&huart2, Tx, sizeof(Tx));
    }
}

//I2C
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c){
	//Pone el disposivo a escuchar
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode){
	//Cuando recibe algo verifica la dirección
	if (TransferDirection== I2C_DIRECTION_TRANSMIT){
		HAL_I2C_Slave_Seq_Receive_IT(hi2c, I2C_Rx, sizeof(I2C_Rx), I2C_FIRST_AND_LAST_FRAME);
	} else{
		Tx[0]=byteControl;
		HAL_I2C_Slave_Seq_Transmit_IT(hi2c, Tx, sizeof(Tx), I2C_FIRST_AND_LAST_FRAME);
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c){
	//Al terminar la recepción de datos envía el byteControl (Parqueos) y vuelve a escuchar
	byteControl=I2C_Rx[0];
	HAL_UART_Transmit_IT(&huart2, I2C_Rx, sizeof(Tx));
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c){
	//Al terminar el envío de datos se pone a escuchar
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){{
	//Si hay errores, se pone a escuchar
}
	HAL_I2C_EnableListen_IT(hi2c);
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
