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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fatfs_sd.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 128
SPI_HandleTypeDef hspi1;
FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
char archivo[100];
char buffer[BUFFER_SIZE];  // Buffer para almacenar los datos leídos
UINT bytesRead;
uint8_t respuesta[1];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
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
  //sending to UART
  void transmit_uart(char *string){
	  uint8_t len=strlen(string);
	  HAL_UART_Transmit(&huart2, (uint8_t*)string, len, 200);
  }

  void read_file_and_send_uart(const char *filename) {
      FIL fil;          // Estructura para el archivo
      FATFS fs;         // Estructura para el sistema de archivos
      FRESULT fres;     // Resultado de las operaciones FatFS
      UINT bytesRead;   // Número de bytes leídos
      char buffer[BUFFER_SIZE];  // Buffer para almacenar los datos leídos

      // 1. Montar la SD
      fres = f_mount(&fs, "", 0);
      if (fres == FR_OK) {
          transmit_uart("Micro SD card is mounted successfully!\n");
      } else {
          transmit_uart("Micro SD card mount error!\n");
          return;  // Si no se puede montar la SD, salir de la función
      }

      // 2. Abrir el archivo pasado como parámetro
      fres = f_open(&fil, filename, FA_READ);
      if (fres == FR_OK) {
          transmit_uart("File opened for reading.\n");
      } else {
          transmit_uart("File was not opened for reading!\n");
          f_mount(NULL, "", 0);  // Desmontar la SD antes de salir
          return;
      }

      // 3. Leer el archivo y enviar el contenido por UART
      do {
          fres = f_read(&fil, buffer, sizeof(buffer) - 1, &bytesRead);
          if (fres == FR_OK) {
              buffer[bytesRead] = '\0';  // Terminar el buffer con null
              transmit_uart(buffer);     // Enviar el contenido por UART
          } else {
              transmit_uart("Error reading file.\n");
              break;
          }
      } while (bytesRead > 0);  // Continuar leyendo hasta que no haya más datos

      // 4. Cerrar el archivo
      fres = f_close(&fil);
      if (fres == FR_OK) {
          transmit_uart("The file is closed.\n");
      } else {
          transmit_uart("The file was not closed.\n");
      }

      // 5. Desmontar la SD
      fres = f_mount(NULL, "", 0);
      if (fres == FR_OK) {
          transmit_uart("SD card unmounted successfully!\n");
      } else {
          transmit_uart("SD card unmount error!\n");
      }
  }

  void list_files_and_send_uart(void) {
      FATFS fs;            // Estructura para el sistema de archivos
      FILINFO fno;         // Información del archivo
      DIR dir;             // Estructura para el directorio
      FRESULT fres;        // Resultado de las operaciones FatFS
      char buffer[BUFFER_SIZE];  // Buffer para almacenar los nombres de archivos

      // 1. Montar la SD
      fres = f_mount(&fs, "", 0);
      if (fres == FR_OK) {
          transmit_uart("Micro SD card is mounted successfully!\n");
      } else {
          transmit_uart("Micro SD card mount error!\n");
          return;  // Si no se puede montar la SD, salir de la función
      }

      // 2. Abrir el directorio raíz
      fres = f_opendir(&dir, "/");
      if (fres == FR_OK) {
          transmit_uart("Root directory opened.\n");
      } else {
          transmit_uart("Error opening root directory.\n");
          f_mount(NULL, "", 0);  // Desmontar la SD antes de salir
          return;
      }

      // 3. Leer los archivos y enviar sus nombres por UART
      while (1) {
          fres = f_readdir(&dir, &fno);  // Leer el siguiente archivo
          if (fres != FR_OK || fno.fname[0] == 0) {
              break;  // Salir del bucle si ocurre un error o si no hay más archivos
          }

          // Verificar si es un archivo o un directorio
          if (fno.fattrib & AM_DIR) {
              snprintf(buffer, BUFFER_SIZE, "Directory: %s\n", fno.fname);
          } else {
              snprintf(buffer, BUFFER_SIZE, "File: %s\n", fno.fname);
          }

          // Enviar el nombre del archivo o directorio por UART
          transmit_uart(buffer);
      }

      // 4. Cerrar el directorio
      fres = f_closedir(&dir);
      if (fres == FR_OK) {
          transmit_uart("Directory closed.\n");
      } else {
          transmit_uart("Directory was not closed.\n");
      }

      // 5. Desmontar la SD
      fres = f_mount(NULL, "", 0);
      if (fres == FR_OK) {
          transmit_uart("SD card unmounted successfully!\n");
      } else {
          transmit_uart("SD card unmount error!\n");
      }
  }

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
   /*fres = f_mount(&fs, "/", 0);
   if(fres == FR_OK){
 	  transmit_uart("Micro SD card is mounted successfully!\n");
   }
   else if (fres != FR_OK){
 	  transmit_uart("Micro SD card mount error!\n");
   }*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */
	  transmit_uart("Ingrese un numero:\n1:Ver archivos\n2:Seleccionar archivo\n");
	  HAL_UART_Receive(&huart2, &respuesta, 1,10000);
	  if (respuesta[0]=='1'){
		  transmit_uart("Lista de archivos: \n");
		  list_files_and_send_uart();
	  }
	  if (respuesta[0]=='2'){
		  transmit_uart("Seleccione un archivo: \n");
		  HAL_UART_Receive(&huart2, &respuesta, 1,10000);
		  if (respuesta[0]=='1'){
			  read_file_and_send_uart("ascii-Heart.txt");
			  transmit_uart("\n");
		  }
		  if (respuesta[0]=='2'){
			  read_file_and_send_uart("ascii-penguin.txt");
			  transmit_uart("\n");
		  }
		  if (respuesta[0]=='3'){
			  read_file_and_send_uart("ascii-creeper.txt");
			  transmit_uart("\n");
		  }
	  }
	  respuesta[0]=='0';

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_SS_GPIO_Port, SD_SS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SD_SS_Pin */
  GPIO_InitStruct.Pin = SD_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SD_SS_GPIO_Port, &GPIO_InitStruct);

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