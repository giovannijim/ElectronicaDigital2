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
#include "ili9341.h"
#include "sprites.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
    uint8_t estado;
    unsigned int isEntering;
    float animacionEntrada;
    unsigned int isOut;
    float animacionSalida;
    uint8_t num;
    unsigned int x;
    unsigned int y;
} EspacioParqueo;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
extern uint8_t fondo[];
uint8_t byteControl= 0x00;
uint8_t datos = 0x00;
EspacioParqueo p1,p2,p3,p4,p5,p6,p7,p8;
uint8_t Rx[1];
uint8_t Tx[1];

// Variables para antirrebote y conteo por botón
uint32_t previousMillis[4] = {0, 0, 0, 0};
uint32_t currentMillis = 0;
uint32_t counterOutside = 0;
uint32_t counterInside[4] = {0, 0, 0, 0};

//Variables I2C
uint8_t I2C_Rx[1];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
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

void initParqueo(EspacioParqueo* parqueo, uint8_t num, unsigned int x, unsigned int y){
	parqueo ->estado = 0; // 0 = libre, 1 = ocupado
	parqueo ->isEntering=0;
	parqueo ->animacionEntrada=5;
	parqueo ->isOut=0;
	parqueo ->animacionSalida=5;
	parqueo ->num=num;
	parqueo ->x=x;
	parqueo ->y=y;
}

void FanimationOut(EspacioParqueo* parqueo){
	int variableAnimationOut = parqueo ->  animacionSalida;
	if (variableAnimationOut<5){
		if (parqueo->num==0x10 || parqueo->num==0x20||parqueo->num==0x40 || parqueo->num==0x80){
			LCD_Sprite(parqueo ->x, parqueo->y, 40, 24, RedCar_40x24, 5, variableAnimationOut, 1, 0);
		} else{
			LCD_Sprite(parqueo ->x, parqueo->y, 40, 24, RedCar_40x24, 5, variableAnimationOut, 0, 0);
			  }
		}
	parqueo -> animacionSalida+=0.8;//0.2
}

void FanimationEnter(EspacioParqueo* parqueo){
	int variableAnimationEnter = parqueo ->  animacionEntrada;
	if (variableAnimationEnter<5){
		int frame=4-variableAnimationEnter;
		if (parqueo->num==0x10 || parqueo->num==0x20||parqueo->num==0x40 || parqueo->num==0x80){
		LCD_Sprite(parqueo ->x, parqueo->y, 40, 24, RedCar_40x24, 5, frame, 1, 0);
		} else{
			LCD_Sprite(parqueo ->x, parqueo->y, 40, 24, RedCar_40x24, 5, frame, 0, 0);
		}
	}
	parqueo -> animacionEntrada+=0.8;
}

void actualizarEstadoParqueos(EspacioParqueo* parqueo, uint8_t byteControl, int num) {
	if (byteControl & (1 << num)) {
	            // El bit está activo, el espacio está ocupado
	            parqueo ->estado = 1;
	            parqueo ->animacionEntrada=0;
	        } else {
	            // El bit está inactivo, el espacio está libre
	            parqueo -> estado = 0;
	            parqueo ->animacionSalida=0;
	        }
}

void controlParqueo(void){

	actualizarEstadoParqueos(&p1,byteControl, 0);
	FanimationOut(&p1);
	FanimationEnter(&p1);

	actualizarEstadoParqueos(&p2,byteControl, 1);
	FanimationOut(&p2);
	FanimationEnter(&p2);

	actualizarEstadoParqueos(&p3,byteControl, 2);
	FanimationOut(&p3);
	FanimationEnter(&p3);

	actualizarEstadoParqueos(&p4,byteControl, 3);
	FanimationOut(&p4);
	FanimationEnter(&p4);

	actualizarEstadoParqueos(&p5,byteControl, 4);
	FanimationOut(&p5);
	FanimationEnter(&p5);

	actualizarEstadoParqueos(&p6,byteControl, 5);
	FanimationOut(&p6);
	FanimationEnter(&p6);

	actualizarEstadoParqueos(&p7,byteControl, 6);
	FanimationOut(&p7);
	FanimationEnter(&p7);

	actualizarEstadoParqueos(&p8,byteControl, 7);
	FanimationOut(&p8);
	FanimationEnter(&p8);

}



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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  //UART
  //HAL_UART_Receive_IT(&huart2, Rx, sizeof(Rx));

  //I2C
  if (HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK){
	  Error_Handler();
  }

  HAL_Delay(100);

	LCD_Init();
	LCD_Clear(0x00);

	LCD_Bitmap(0, 0, 320, 240, fondo);

	initParqueo(&p1, 0x01, 26, 191);
	initParqueo(&p2, 0x02, 26, 150);
	initParqueo(&p3, 0x04, 26, 108);
	initParqueo(&p4, 0x08, 26, 68);

	initParqueo(&p5, 0x10, 253, 68);
	initParqueo(&p6, 0x20, 253, 108);
	initParqueo(&p7, 0x40, 253, 150);
	initParqueo(&p8, 0x80, 253, 191);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		controlParqueo();
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
  hi2c1.Init.OwnAddress1 = 0;
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_RST_Pin|LCD_D1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin
                          |LCD_D4_Pin|SD_SS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_RST_Pin LCD_D1_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RD_Pin LCD_WR_Pin LCD_RS_Pin LCD_D7_Pin
                           LCD_D0_Pin LCD_D2_Pin */
  GPIO_InitStruct.Pin = LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_CS_Pin LCD_D6_Pin LCD_D3_Pin LCD_D5_Pin
                           LCD_D4_Pin SD_SS_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin
                          |LCD_D4_Pin|SD_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BTN1_Pin BTN2_Pin BTN3_Pin BTN4_Pin */
  GPIO_InitStruct.Pin = BTN1_Pin|BTN2_Pin|BTN3_Pin|BTN4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    counterOutside++; // Para pruebas
    currentMillis = HAL_GetTick();

    // Verificar qué botón fue presionado y aplicar el antirrebote
    if (GPIO_Pin == GPIO_PIN_12 && (currentMillis - previousMillis[0] > 150))
    {
        counterInside[0]++; // Para pruebas del botón 1
        previousMillis[0] = currentMillis;
        byteControl^= 0b00000001;
        Tx[0]=byteControl;
        Rx[0]=byteControl;
        //HAL_UART_Transmit_IT(&huart2, Tx, sizeof(Tx));
    }
    else if (GPIO_Pin == GPIO_PIN_13 && (currentMillis - previousMillis[1] > 150))
    {
        counterInside[1]++; // Para pruebas del botón 2
        previousMillis[1] = currentMillis;
        byteControl^= 0b00000010;
        Tx[0]=byteControl;
        Rx[0]=byteControl;
        //HAL_UART_Transmit_IT(&huart2, Tx, sizeof(Tx));

    }
    else if (GPIO_Pin == GPIO_PIN_14 && (currentMillis - previousMillis[2] > 150))
    {
        counterInside[2]++; // Para pruebas del botón 3
        previousMillis[2] = currentMillis;
        byteControl^= 0b00000100;
        Tx[0]=byteControl;
        Rx[0]=byteControl;
        //HAL_UART_Transmit_IT(&huart2, Tx, sizeof(Tx));
    }
    else if (GPIO_Pin == GPIO_PIN_15 && (currentMillis - previousMillis[3] > 150))
    {
        counterInside[3]++; // Para pruebas del botón 4

        previousMillis[3] = currentMillis;
        byteControl^= 0b00001000;
		Tx[0]=byteControl;
		Rx[0]=byteControl;
		//HAL_UART_Transmit_IT(&huart2, Tx, sizeof(Tx));
    }
}

//UART

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	byteControl=Rx[0];
	HAL_UART_Transmit_IT(huart, Tx, sizeof(Tx));
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	HAL_UART_Receive_IT(huart, Rx, sizeof(Rx));
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
	while (1) {
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
