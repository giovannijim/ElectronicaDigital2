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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

//Estructura del jugador
typedef struct {
    unsigned int x;         // Coordenada x del jugador
    unsigned int y;         // Coordenada y del jugador
    unsigned int width;     // Ancho del hitbox (y sprite)
    unsigned int height;    // Largo del hitbox (y sprite)
    unsigned int speed;     // Velocidad de movimiento del jugador
    unsigned int life;      // Vida del jugador
    unsigned int score;     // Puntuación actual del jugador
    unsigned int isAlive;   // Estado del jugador (1: vivo, 0: muerto)
    unsigned int direction; // Dirección de movimiento (0: arriba, 1: derecha, 2: abajo, 3: izquierda)
    unsigned int limitWidth; // Ancho de los límites de desplazamiento
    unsigned int limitHeight; // Largo de los límites de desplazamiento
    unsigned int colision;
    unsigned int playerLeft;
    unsigned int playerRight;
    unsigned int playerUp;
    unsigned int playerDown;
} player;

//Estructura enemigo 1
typedef struct {
    unsigned int x;         // Coordenada X del enemigo
    unsigned int y;         // Coordenada Y del enemigo
    unsigned int width;     // Ancho de la hitbox del enemigo
    unsigned int height;    // Alto de la hitbox del enemigo
    int health;             // Vida del enemigo
    int isAlive;            // Estado del enemigo (1 = vivo, 0 = muerto)
    unsigned int e1Left;
    unsigned int e1Right;
    unsigned int e1Up;
    unsigned int e1Down;

} enemy_type1;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
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
player p1,p2;
enemy_type1 e1_1, e1_2, e1_3;
int i;
uint8_t modo, fase_p1, fase_p2;
/*  modo=1 1 jugadores
 *  modo=2 2 jugadores
 *  fase_p1 La fase en la que está el jugador 1
 *  fase_p2 La fase en la que está el jugador 2
*/

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

//Funciones Enemigo tipo 1
void initEnemy1(enemy_type1* enemy, unsigned int startX, unsigned int startY, unsigned int width, unsigned int height, int health) {
    // Inicializar las coordenadas y dimensiones
    enemy->x = startX;
    enemy->y = startY;
    enemy->width = width;
    enemy->height = height;

    // Inicializar la vida y el estado
    enemy->health = health;
    enemy->isAlive = 1;  // El enemigo comienza vivo

    // Dibujar el enemigo en pantalla
    FillRect(enemy->x - (enemy->width / 2), enemy->y - (enemy->height / 2), enemy->width, enemy->height, 0xFF0000);  // Color rojo
    FillRect(enemy->x , enemy->y, 1, 1, 0xFFFFFF);

    //HITBOX DEBUG
	enemy->e1Left = enemy->x - (enemy->width / 2);
	enemy->e1Right = (enemy->x + (enemy->width / 2)-1);
	enemy->e1Up = enemy->y - (enemy->height / 2);
	enemy->e1Down= (enemy->y + (enemy->height / 2)-1);
	FillRect(enemy->e1Left , enemy->y, 1, 1, 0xFFFFFF); //Izquierda
	FillRect(enemy->e1Right , enemy->y, 1, 1, 0x0420); //Derecha
	FillRect(enemy->x , enemy->e1Up, 1, 1, 0xFFFFFF); //Arriba
	FillRect(enemy->x , enemy->e1Down, 1, 1, 0x0420); //Abajo
}

int ColisionPlayer_e1(enemy_type1* enemy, player* player,int direction, int futureX, int futureY){
	// Verificar colisión con el enemigo basado en la dirección de movimiento
	    switch (direction) {
	        case 0: // Movimiento hacia abajo
	        	if (futureY >= enemy->e1Up && (player->playerRight<=enemy->e1Right && player->playerLeft>=enemy->e1Left)){
	        		player->y -= player->speed;
	        		return 0;  // Colisión con el enemigo
	        	}
	        	 break;
	        return 1;

/*
	            if (player->y - player->speed < enemyBottom &&
	                player->x > enemyLeft &&
	                player->x < enemyRight) {
	            	player->y -= player->speed;
	                return 1;  // Colisión con el enemigo
	            }
	            break;
	        case 1: // Movimiento hacia dercha
	            if (player->y + player->speed > enemyTop &&
	                player->x > enemyLeft &&
	                player->x < enemyRight) {
	            	player->y += player->speed;
	                return 1;  // Colisión con el enemigo
	            }
	            break;
	        case 2: // Movimiento hacia arriba
	            if (player->x - player->speed < enemyRight &&
	                player->y > enemyTop &&
	                player->y < enemyBottom) {
	            	player->x += player->speed;
	                return 1;  // Colisión con el enemigo
	            }
	            break;
	        case 3: // Movimiento hacia izquierda
	            if (player->x + player->speed > enemyLeft &&
	                player->y > enemyTop &&
	                player->y < enemyBottom) {
	            	player->x -= player->speed;
	                return 1;  // Colisión con el enemigo
	            }
	            break;
	    }
*/
}
}

/*
   // Verificar colisiones con Enemy_type1
    if (player->x - (player->width / 2) < enemy->x + (enemy->width / 2) &&
            player->x + (player->width / 2) > enemy->x - (enemy->width / 2) &&
            player->y - (player->height / 2) < enemy->y + (enemy->height / 2) &&
            player->y + (player->height / 2) > enemy->y - (enemy->height / 2)){
    	player->colision=1;
    	switch (direction) {
    	        case 0:  // Abajo
    	            player->y -= player->speed;
    	            break;
    	        case 1:  // Derecha
    	        	player->x -= player->speed;
    	            break;
    	        case 2:  // Arriba
    	        	player->y += player->speed;
    	            break;
    	        case 3:  // Izquierda
    	        	player->x += player->speed;
    	            break;
    	    }
    	FillRect(enemy->x - (enemy->width / 2), enemy->y - (enemy->height / 2), enemy->width, enemy->height, 0xFF0000);  // Color rojo
    	FillRect(enemy->x , enemy->y, 1, 1, 0xFFFFFF);
    	player->life -=1;
    	return 1;

    }
 */

//Funciones Player
void initPlayer(player* player, unsigned int startX, unsigned int startY, unsigned int playerWidth, unsigned int playerHeight, unsigned int speed, unsigned int life, unsigned int limitWidth, unsigned int limitHeight) {
    // Inicializar las propiedades del jugador
    player->x = startX;
    player->y = startY;
    player->width = playerWidth;
    player->height = playerHeight;
    player->speed = speed;
    player->life = life;
    player->score = 0;          // Puntuación inicial en 0
    player->isAlive = 1;        // El jugador comienza vivo
    player->direction = 0;      // Dirección inicial (arriba)
    player->limitWidth = limitWidth;
    player->limitHeight = limitHeight;

    // Dibujar el jugador en pantalla
    FillRect(player->x - (player->width / 2), player->y - (player->height / 2), player->width, player->height, 0xFFFB00);
    FillRect(player->x , player->y, 1, 1, 0x000000);

    //HITBOX DEBUG
    player->playerLeft=player->x-(player->width / 2);
    player->playerRight=(player->x+(player->width / 2)-1);
    player->playerUp=player->y-(player->height / 2);
    player->playerDown=(player->y+(player->height / 2)-1);
    FillRect(player->playerLeft , player->y, 1, 1, 0xFFFFFF); //Izquierda
    FillRect(player->playerRight , player->y, 1, 1, 0x0420); //Derecha
    FillRect(player->x , player->playerUp, 1, 1, 0xFFFFFF); //Arriba
    FillRect(player->x , player->playerDown, 1, 1, 0x0420); //Abajo


}

int playerCanMove(player* player, unsigned int direction) {
    // Variables para calcular la posición futura del jugador según la dirección
    int futureX = player->x;
    int futureY = player->y;

    // Calcular las nuevas coordenadas dependiendo de la dirección
    switch (direction) {
        case 0:  // Abajo
            futureY += player->speed;
            break;
        case 1:  // Derecha
            futureX += player->speed;
            break;
        case 2:  // Arriba
            futureY -= player->speed;
            break;
        case 3:  // Izquierda
            futureX -= player->speed;
            break;
    }

    if (ColisionPlayer_e1(&e1_2, player, direction,futureX,futureY)==0){
    	return 0;
    }
    if (ColisionPlayer_e1(&e1_2, player, direction,futureX,futureY)==0){
        	return 0;
        }
    if (ColisionPlayer_e1(&e1_3, player, direction,futureX,futureY)==0){
        	return 0;
        }

    // Verificar colisiones con los bordes en base a la posición futura
    if (futureX <= 0) {
        return 0;  // Colisión con el borde izquierdo
    }
    if (futureX >= player->limitWidth) {
        return 0;  // Colisión con el borde derecho
    }
    if (futureY  <= 0) {
        return 0;  // Colisión con el borde superior
    }
    if (futureY>= player->limitHeight) {
        return 0;  // Colisión con el borde inferior
    }
    // No hay colisiones, se puede mover
    return 1;
}

void HitboxPlayer(player* player){
    //HITBOX DEBUG
    player->playerLeft=player->x-(player->width / 2);
    player->playerRight=(player->x+(player->width / 2)-1);
    player->playerUp=player->y-(player->height / 2);
    player->playerDown=(player->y+(player->height / 2)-1);
    FillRect(player->playerLeft , player->y, 1, 1, 0xFFFFFF); //Izquierda
    FillRect(player->playerRight , player->y, 1, 1, 0x0420); //Derecha
    FillRect(player->x , player->playerUp, 1, 1, 0xFFFFFF); //Arriba
    FillRect(player->x , player->playerDown, 1, 1, 0x0420); //Abajo
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
  /* USER CODE BEGIN 2 */

	LCD_Init();
	LCD_Clear(0x00);

	//Fondo
	FillRect(0, 0, 319, 239, 0xFFFF);

	//FillRect(50, 60, 20, 20, 0xF800);
	//FillRect(70, 60, 20, 20, 0x07E0);
	//FillRect(90, 60, 20, 20, 0x001F);

	//LCD_Bitmap(0, 0, 320, 240, fondo);
	//FillRect(0, 0, 319, 206, 0x1911);

	//LCD_Print("Hola Mundo", 20, 100, 1, 0x001F, 0xCAB9);

	  // Activar bandera interrupcion
	  HAL_UART_Receive_IT(&huart2, buffer, 1);
	  modo=1;
	  if (modo==1){
	    //Inicializar Jugador 1
		initPlayer(&p1, 160, 200, 20, 20, 20, 3, 320, 240);
		//Inicializar enemigo 1
		initEnemy1(&e1_1, 50, 80, 20, 20, 3);
		//Inicializar enemigo 2
		initEnemy1(&e1_2, 160, 80, 20, 20, 3);
		//Inicializar enemigo 3
		initEnemy1(&e1_3, 270, 80, 20, 20, 3);}
	  if (modo==2){
		//Linea de en medio
		V_line(160, 0, 240, 0x0000);
	  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
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

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(buffer[0] == 'd'){
		FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFFFF);
		if (playerCanMove(&p1, 0)) {
		    p1.y=p1.y+p1.speed;
		    FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
		    FillRect(p1.x , p1.y, 1, 1, 0x000000);
		} else {
			FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
			FillRect(p1.x , p1.y, 1, 1, 0x000000);
		}
	}
	if(buffer[0] == 'u'){
		FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFFFF);
		if (playerCanMove(&p1, 2)) {
			p1.y=p1.y-p1.speed;
			FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
			FillRect(p1.x , p1.y, 1, 1, 0x000000);
		} else {
			FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
			FillRect(p1.x , p1.y, 1, 1, 0x000000);
		}
	}
	if(buffer[0] == 'r'){
		FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFFFF);
		if (playerCanMove(&p1, 1)) {
			p1.x=p1.x+p1.speed;
			FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
			FillRect(p1.x , p1.y, 1, 1, 0x000000);
		} else {
			FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
			FillRect(p1.x , p1.y, 1, 1, 0x000000);
		}
	}
	if(buffer[0] == 'l'){
		FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFFFF);
		if (playerCanMove(&p1, 3)) {
			p1.x=p1.x-p1.speed;
			FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
			FillRect(p1.x , p1.y, 1, 1, 0x000000);
		} else {
			FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
			FillRect(p1.x , p1.y, 1, 1, 0x000000);
		}
	}
	if (buffer[0]=='b'){
		/*
	    verificar_golpe(&e1, position_p1);
	    verificar_golpe(&e2, position_p1);
	    verificar_golpe(&e3, position_p1);
	    */
	}
	HitboxPlayer(&p1);
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
