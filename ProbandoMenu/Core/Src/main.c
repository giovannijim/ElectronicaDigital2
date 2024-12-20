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
#include "ili9341.h"
#include "fatfs_sd.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

//Estructura del jugador
typedef struct{
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
	unsigned int v;
	unsigned int eliminado;
	unsigned int vel;
	unsigned int vista;
} player;

//Estructura del primer tipo de enemigo
typedef struct {
    unsigned int x;    // Coordenada en X
    unsigned int y;    // Coordenada en Y
    unsigned int v;    // Vida
    unsigned int eliminado; //Enemigo eliminado o no
    unsigned int w;    // Ancho de la hitbox
    unsigned int h;    // Altura de la hitbox
} enemigo_c1;

//
typedef struct {
    unsigned int x;         // Coordenada en X
    unsigned int y;         // Coordenada en Y
    unsigned int w;         // Anchura de la hitbox
    unsigned int h;         // Altura de la hitbox
    unsigned int v;         // Vida actual
    unsigned int vida_inicial; // Vida inicial
    unsigned int vivo;      // 1 si está vivo, 0 si está muerto
    unsigned int direccion; // Dirección de movimiento (0: derecha, 1: abajo, 2: izquierda, 3: arriba)
    unsigned int velocidad; // Velocidad de movimiento
} enemigo_c2;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Definir una región de memoria flash específica para almacenar la imagen
#define MOVIMIENTO_DERECHA  0
#define MOVIMIENTO_ABAJO    1
#define MOVIMIENTO_IZQUIERDA 2
#define MOVIMIENTO_ARRIBA   3

SPI_HandleTypeDef hspi1;
FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
char archivo[100];
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
extern uint8_t fondo[];
uint8_t buffer[10];
uint16_t contador=0;
enemigo_c1 e1,e2,e3;
enemigo_c2 e4,e5,e6;
player p1;

char position_p1 [3] = {40,40,8};
uint8_t image[30720];
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

void transmit_uart(char *string){
  uint8_t len=strlen(string);
  HAL_UART_Transmit(&huart2, (uint8_t*)string, len, 200);
}


// Función para leer una imagen desde la SD y almacenarla en la variable play
int load_image_from_sd_to_play(const char* filename) {
    FIL fil;
    UINT bytes_read;
    FRESULT fres;

    fres = f_mount(&fs, "/", 0);
	/*if (fres != FR_OK) {
		transmit_uart("Error al montar la SD\n");
		//free(image_buffer);
		return;
	}*/
    // Abrir el archivo desde la SD
    fres = f_open(&fil, filename, FA_READ);
    if (fres != FR_OK) {
       transmit_uart("Error al abrir el archivo en la SD\n");
        return 0;  // Error al abrir el archivo
    }

    // Leer los datos del archivo y almacenarlos en la variable play
    fres = f_read(&fil, image, 30720, &bytes_read);
    if (fres != FR_OK || bytes_read == 0) {
       transmit_uart("Error al leer la imagen desde la SD\n");
        f_close(&fil);
        return 0;  // Error al leer el archivo
    }

    // Cerrar el archivo
    f_close(&fil);
   // transmit_uart("Imagen cargada correctamente desde la SD a la variable play\n");
    return 1;  // Éxito
}

//Funciones Player
void init_player(player *p, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int vel, unsigned int color, unsigned int vista) {
    p->x = x;               // Posición inicial en X
    p->y = y;               // Posición inicial en Y
    p->w = w;               // Ancho del jugador
    p->h = h;               // Altura del jugador
    p->v = 0;               // Velocidad vertical inicial (si es necesario)
    p->eliminado = 0;       // El jugador está activo
    p->vel = vel;           // Velocidad del jugador
    p->vista = vista;
    FillRect(p->x, p->y, p->w, p->h, color);  // Dibuja el jugador en la pantalla usando el color especificado
}

void P1_erasePath(void){
	if (position_p1[2]==8){
		FillRect(position_p1[0]-20, position_p1[1], 20, 20, 0xFFFF);
	}
	if (position_p1[2]==2){
		FillRect(position_p1[0]+20, position_p1[1], 20, 20, 0xFFFF);
	}
	if (position_p1[2]==6){
		FillRect(position_p1[0], position_p1[1]-20, 20, 20, 0xFFFF);
	}
	if (position_p1[2]==4){
		FillRect(position_p1[0], position_p1[1]+20, 20, 20, 0xFFFF);
	}
}
//Enemigo tipo 1
	//Iniciar enemigo
	void init_enemigo(enemigo_c1 *enemigo, unsigned int x, unsigned int y, unsigned int vida) {
		enemigo->x = x;
		enemigo->y = y;
		enemigo->v = vida;
		enemigo->eliminado = 0; // Inicialmente no está eliminado
	}
	//Dibujar enemigo
	void dibujar_enemigo(enemigo_c1 *enemigo, unsigned int color) {
	    FillRect(enemigo->x, enemigo->y, 20, 20, color);
	}

	//Validar vida enemigo
	void verificar_vida(enemigo_c1 *enemigo, unsigned int color_fondo) {
	    if (enemigo->v == 0 && enemigo->eliminado == 0) {
	        // Dibuja el cuadrado del color de fondo solo una vez
	        dibujar_enemigo(enemigo, color_fondo);
	        enemigo->eliminado = 1;  // Marca al enemigo como eliminado
	    }
	}
	//Validar si se golpeo un enemigo del tipo 1
	void verificar_golpe(enemigo_c1 *enemigo, int *position_p1) {
	    // Jugador mirando hacia la derecha (p1[2] == 8)
	    if (position_p1[2] == 8) {
	        if (position_p1[0] + 20 >= enemigo->x && position_p1[0] <= enemigo->x + enemigo->w) {
	            if (position_p1[1] >= enemigo->y && position_p1[1] <= enemigo->y + enemigo->h) {
	                enemigo->v -= 1;
	            }
	        }
	    }

	    // Jugador mirando hacia la izquierda (p1[2] == 2)
	    if (position_p1[2] == 2) {
	        if (position_p1[0] - 20 <= enemigo->x + enemigo->w && position_p1[0] >= enemigo->x) {
	            if (position_p1[1] >= enemigo->y && position_p1[1] <= enemigo->y + enemigo->h) {
	                enemigo->v -= 1;
	            }
	        }
	    }

	    // Jugador mirando hacia abajo (p1[2] == 6)
	    if (position_p1[2] == 6) {
	        if (position_p1[1] + 20 >= enemigo->y && position_p1[1] <= enemigo->y + enemigo->h) {
	            if (position_p1[0] >= enemigo->x && position_p1[0] <= enemigo->x + enemigo->w) {
	                enemigo->v -= 1;
	            }
	        }
	    }

	    // Jugador mirando hacia arriba (p1[2] == 4)
	    if (position_p1[2] == 4) {
	        if (position_p1[1] - 20 <= enemigo->y + enemigo->h && position_p1[1] >= enemigo->y) {
	            if (position_p1[0] >= enemigo->x && position_p1[0] <= enemigo->x + enemigo->w) {
	                enemigo->v -= 1;
	            }
	        }
	    }
	}

//Enemigo tipo 2

void init_enemigo2(enemigo_c2 *enemigo, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int v, unsigned int direccion, unsigned int velocidad) {
	enemigo->x = x;               // Coordenada inicial en X
	enemigo->y = y;               // Coordenada inicial en Y
	enemigo->w = w;               // Ancho de la hitbox
	enemigo->h = h;               // Altura de la hitbox
	enemigo->v = v;               // Vida inicial
	enemigo->direccion = direccion; // Dirección inicial de movimiento
	enemigo->velocidad = velocidad; // Velocidad del movimiento
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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

	LCD_Init();
	//Fondo
	if (load_image_from_sd_to_play("fb1_h.bin")) {
		LCD_Bitmap(0, 0, 320, 48, image);

	}
	if (load_image_from_sd_to_play("fb2_h.bin")) {
		LCD_Bitmap(0, 48, 320, 48, image);
	}
	if (load_image_from_sd_to_play("fb3_h.bin")) {
		LCD_Bitmap(0, 96, 320, 48, image);

	}
	if (load_image_from_sd_to_play("fb4_h.bin")) {
		LCD_Bitmap(0, 144, 320, 48, image);
	}
	if (load_image_from_sd_to_play("fb5_h.bin")) {
		LCD_Bitmap(0, 192, 320, 48, image);
	}
	//Fondo
	//FillRect(0, 0, 319, 239, 0xFFFF);
	//Linea de en medio
	//V_line(160, 0, 240, 0x0000);
	//FillRect(50, 60, 20, 20, 0xF800);
	//FillRect(70, 60, 20, 20, 0x07E0);
	//FillRect(90, 60, 20, 20, 0x001F);

	//mostrar_imagen_sd("fondo_menu.txt",0,0,320,240);
	//LCD_Clear(0x00);
	//LCD_Bitmap(0, 0, 320, 240, fondo);
	//FillRect(0, 0, 319, 206, 0x1911);

	//LCD_Print("Hola Mundo", 20, 100, 1, 0x001F, 0xCAB9);
	/*
	  // Activar bandera interrupcion
	  HAL_UART_Receive_IT(&huart2, buffer, 1);

	  //EnemigoC1 1
	  init_enemigo(&e1, 100, 100, 3);
	  dibujar_enemigo(&e1, 0x07E0);

	  //EnemigoC1 2
	  init_enemigo(&e2, 100, 20, 3);
	  dibujar_enemigo(&e2, 0x0E0E);

	  //EnemigoC1 3
	  init_enemigo(&e3, 20, 100, 3);
	  dibujar_enemigo(&e3, 0x0FFF);
	  */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {

		//Jugador 1
		//FillRect(position_p1[0], position_p1[1], 20, 20, 0xF800);
		//P1_erasePath();

		//Enemigo 1
		//verificar_vida(&e1, 0xFFFF);
		//verificar_vida(&e2, 0xFFFF);
		//verificar_vida(&e3, 0xFFFF);
		//movimiento_e1();


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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
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
                          |LCD_D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_SS_GPIO_Port, SD_SS_Pin, GPIO_PIN_SET);

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
                           LCD_D4_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin
                          |LCD_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_SS_Pin */
  GPIO_InitStruct.Pin = SD_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(SD_SS_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(buffer[0] == 'u'){
		if (position_p1[0]<140){
		position_p1[0]+=20;
		position_p1[2]=8;}

		if(p1.x<140){
			p1.x=p1.x+p1.vel;
			p1.vista=8;
		}
	}
	if(buffer[0] == 'd'){
		if (position_p1[0]>0){
		position_p1[0]-=20;
		position_p1[2]=2;}

		if (p1.x>0){
			p1.x=p1.x-p1.vel;
			p1.vista=4;
		}
	}
	if(buffer[0] == 'r'){
		if (position_p1[1]<220){
		position_p1[1]+=20;
		position_p1[2]=6;}

		if (p1.y<220){
			p1.y=p1.y+p1.vel;
			p1.vista=6;
		}
	}
	if(buffer[0] == 'l'){
		if(position_p1[1]>0){
		position_p1[1]-=20;
		position_p1[2]=4;}

		if (p1.y>0){
			p1.y=p1.y-p1.vel;
			p1.vista=4;
		}
	}
	if (buffer[0]=='b'){
	    //verificar_golpe(&e1, position_p1);
	    //verificar_golpe(&e2, position_p1);
	    //verificar_golpe(&e3, position_p1);
	}
	// Vuelve a activar la recepción por interrupción
	HAL_UART_Receive_IT(&huart2, buffer, 1);
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
