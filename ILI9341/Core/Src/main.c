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
#include "sprites.h"
#include "ili9341.h"
#include "fatfs_sd.h"
#include "string.h"
#include "stdio.h"
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
    unsigned int limitWidth_i;
    unsigned int colision;
    unsigned int playerDown;
    unsigned int playerUp;
    unsigned int playerLeft;
    unsigned int playerRight;
    unsigned int animationWalkLeft;
    unsigned int animationWalkRight;
    unsigned int animationWalkUp;
    unsigned int animationWalkDown;
    unsigned int IsAttack;
    float animationAttack;
	unsigned int IsDamage;
	float animationDamage;
	unsigned int IsDie;
	float animationDie;
	unsigned int playerLevel;
	unsigned int PlayerNum;
} player;

//Estructura enemigo 3
typedef struct {
    unsigned int x;         // Coordenada X del enemigo
    unsigned int y;         // Coordenada Y del enemigo
    unsigned int y_eye;
    unsigned int width;     // Ancho de la hitbox del enemigo
    unsigned int height;    // Alto de la hitbox del enemigo
    int isAlive;            // Estado del enemigo (1 = vivo, 0 = muerto)
    unsigned int e1Left;
    unsigned int e1Right;
    unsigned int e1Up;
    unsigned int e1Down;
    float animationFire;
    float animationEye;
    unsigned int speed;
    unsigned int limitwidth;
    unsigned int limitwidth_i;
    unsigned int limitheight;
    unsigned int limitheight_i;
    int health;
    unsigned int isMove;
    unsigned int Place;
    unsigned int delay;
    unsigned int delay_init;
} enemy_type3;

//Estructura enemigo 2
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
    float animationV;
    unsigned int isHurt;
    float animationDV;
    float animationDie;
    unsigned int IsMove;
    unsigned int move;
    float delay;
} enemy_type2;

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
    float animationV;
    float animationDV;
    float animationDie;
} enemy_type1;

typedef enum {
	MENU, SOLO, DUO, PAUSA, WIN1, WIN2, GAMEOVER } EstadoJuego ;

typedef enum {
	NIVEL1, NIVEL2, NIVEL3
} LevelPlaying;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
SPI_HandleTypeDef hspi1;
FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
//char archivo[100];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t fragmento_imagen[19200];
extern uint8_t play_duo_selected[];
extern uint8_t play_duo[];
extern uint8_t play_solo_selected[];
extern uint8_t play_solo[];

uint8_t buffer[10];

// variables de TYPEDEF
player p1,p2;
enemy_type1 e1_1, e1_2, e1_3,e1_4,e1_5,e1_6;
enemy_type2 e2_1,e2_2;
enemy_type3 e3_1, e3_2;
uint8_t modo, fase_p1, fase_p2;
/*  modo=1 1 jugadores
 *  modo=2 2 jugadores
 *  fase_p1 La fase en la que está el jugador 1
 *  fase_p2 La fase en la que está el jugador 2
*/
uint8_t DrawHitbox=0;
volatile EstadoJuego estadoActual = SOLO;
volatile EstadoJuego estadoAnterior = SOLO;
volatile EstadoJuego estadoFuturo;
volatile LevelPlaying nivelActual1;
volatile LevelPlaying nivelActual2;
volatile int IniciarLevel=0;
volatile int IniciarLevel2=0;
uint8_t repintarFondo = 0;
uint8_t pintarFondoPausa = 0;
uint8_t IniciarP1,IniciarP2;
unsigned int bandera1=0;
unsigned int bandera2=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_UART5_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void transmit_uart(char *string){
  uint8_t len=strlen(string);
  HAL_UART_Transmit(&huart2, (uint8_t*)string, len, 200);
}

// Función para leer una imagen desde la SD
int CargarBitmaps_SD(const char* filename) {
    FIL fil;
    UINT bytes_read;
    FRESULT fres;
    // Montar SD
    fres = f_mount(&fs, "/", 0);
    if (fres == FR_OK){
    	transmit_uart("SD MONTADA\n");
    }

    // Abrir el archivo desde la SD
    fres = f_open(&fil, filename, FA_READ);
    if (fres != FR_OK) {
       transmit_uart("Error al abrir el archivo en la SD\n");
        return 0;  // Error al abrir el archivo
    }
    transmit_uart("Se abrio el archivo\n");
    // Leer los datos del archivo y almacenarlos en la variable play
    fres = f_read(&fil, fragmento_imagen, 19200, &bytes_read);
    if (fres != FR_OK || bytes_read == 0) {
       transmit_uart("Error al leer la imagen desde la SD\n");
        f_close(&fil);
        return 0;  // Error al leer el archivo
    }

    // Cerrar el archivo
    f_close(&fil);
    transmit_uart("TERMINO DE CARGAR IMAGEN\n");
   // transmit_uart("Imagen cargada correctamente desde la SD a la variable play\n");
    return 1;  // Éxito
}

/*---------Sonido---------------------*/

// Función para cambiar la frecuencia del PWM (ajusta el periodo)
void setPWM_Frequency(uint32_t frequency) {
	if (estadoActual==DUO || estadoActual==SOLO){
		uint32_t timer_clock = 1000000; // 1 MHz (depende de tu configuración)
		uint32_t period = (timer_clock / frequency) - 1;

		// Cambiar el periodo (Auto-Reload Register)
		__HAL_TIM_SET_AUTORELOAD(&htim2, period);

		// Reiniciar el timer para aplicar el cambio de frecuencia
		__HAL_TIM_SET_COUNTER(&htim2, 0);
	}
}

// Función para cambiar el duty cycle
void setPWM_DutyCycle(uint16_t dutyCycle) {
	if (estadoActual==DUO || estadoActual==SOLO){
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, dutyCycle);
	}
}

/* Funciones Enemigo tipo 1 ---------------------------------------------------*/
void animation_e1_control(enemy_type1* enemy){
	int variableAnimationDV = enemy->animationDV;
	int variableAnimationV= enemy->animationV;
	if (enemy->isAlive==1){
		if (variableAnimationDV>4){
			if (variableAnimationV<16){
				enemy->animationV+=0.1;
			} else{
				enemy->animationV=0;
			}
		} else{
			enemy->animationDV+=0.1;
		}
	}
}


void animation_e1(enemy_type1* enemy){
	int variableAnimationV=enemy->animationV;
	int variableAnimationDV = enemy->animationDV;
	if (enemy->isAlive==1){
		if (variableAnimationDV>4){
			LCD_Sprite(enemy->x - (16 / 2)+2, enemy->y - (19 / 2), 16, 19, E1_Ide256x19_16, 16, variableAnimationV, 0, 0);
		    if (DrawHitbox==1){
		    	Rect(enemy->x - (enemy->width / 2), enemy->y - (enemy->height / 2), enemy->width, enemy->height, 0x0000);
		    }
		    if (DrawHitbox==2){
		    	FillRect(enemy->e1Left , enemy->y, 1, 1, 0xd685); //Izquierda
		    	FillRect(enemy->e1Right , enemy->y, 1, 1, 0xd685); //Derecha
		    	FillRect(enemy->x , enemy->e1Up, 1, 1, 0xd685); //Arriba
		    	FillRect(enemy->x , enemy->e1Down, 1, 1, 0xd685); //Abajo
		    }
		    if (DrawHitbox==3){
		    	Rect(enemy->x - (enemy->width / 2), enemy->y - (enemy->height / 2), enemy->width, enemy->height, 0x0000);
				FillRect(enemy->e1Left , enemy->y, 1, 1, 0xd685); //Izquierda
				FillRect(enemy->e1Right , enemy->y, 1, 1, 0xd685); //Derecha
				FillRect(enemy->x , enemy->e1Up, 1, 1, 0xd685); //Arriba
				FillRect(enemy->x , enemy->e1Down, 1, 1, 0xd685); //Abajo
		        }
			//FillRect(enemy->x - (enemy->width / 2), enemy->y - (enemy->height / 2), enemy->width, enemy->height, 0xFF0000);  // Color rojo
		}
	}
}

void animation_e1_die(enemy_type1* enemy){
	int variableAnimationDie=enemy->animationDie;
		if (enemy->isAlive==0){
			if (variableAnimationDie<11){
				LCD_Sprite(enemy->x - (14 / 2)+1, enemy->y - (25 / 2), 14,25, E1_Die14x25_11, 11, variableAnimationDie, 0, 0);
				enemy->animationDie+=0.2;
				}else{
					enemy->x=5000;
					enemy->y=5000;
				}
			}
}

void animation_e1_dieS(enemy_type1* enemy){
	int variableAnimationDieS=enemy->animationDie;
		if (enemy->isAlive==0){
			if (variableAnimationDieS<11){
				switch (variableAnimationDieS){
				case 0:
					setPWM_Frequency(8500); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);
					break;
				case 1:
					setPWM_Frequency(6500); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);
					break;
				case 2:
					setPWM_Frequency(8500); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);
					break;
				case 3:
					setPWM_Frequency(6500); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);
					break;
				case 4:
					setPWM_Frequency(6500); // Frecuencia de 1 kHz
					setPWM_DutyCycle(0);
					break;
				}
			}
		}
}

void initEnemy1(enemy_type1* enemy, unsigned int startX, unsigned int startY, unsigned int width, unsigned int height, int health) {
    // Inicializar las coordenadas y dimensiones
    enemy->x = startX;
    enemy->y = startY;
    enemy->width = width;
    enemy->height = height;

    // Inicializar la vida y el estado
    enemy->health = health;
    enemy->isAlive = 1;  // El enemigo comienza vivo

    //Animación
    enemy->animationV = 0;
    enemy->animationDV = 5;
    enemy->animationDie=12;

    //HITBOX DEBUG
	enemy->e1Left = enemy->x - (enemy->width / 2);
	enemy->e1Right = (enemy->x + enemy->width / 2); //+1
	enemy->e1Up = enemy->y - (enemy->height / 2);
	enemy->e1Down= (enemy->y + enemy->height / 2); //+1

    // Dibujar el enemigo en pantalla
    LCD_Sprite(enemy->x - (16 / 2), enemy->y - (19 / 2), 16, 19, E1_Ide256x19_16, 16, 0, 0, 0);

    if (DrawHitbox==1){
    	Rect(enemy->x - (enemy->width / 2), enemy->y - (enemy->height / 2), enemy->width, enemy->height, 0x0000);
    }
    if (DrawHitbox==2){
    	FillRect(enemy->e1Left , enemy->y, 1, 1, 0xd685); //Izquierda
    	FillRect(enemy->e1Right , enemy->y, 1, 1, 0xd685); //Derecha
    	FillRect(enemy->x , enemy->e1Up, 1, 1, 0xd685); //Arriba
    	FillRect(enemy->x , enemy->e1Down, 1, 1, 0xd685); //Abajo
    }
    if (DrawHitbox==3){
    	Rect(enemy->x - (enemy->width / 2), enemy->y - (enemy->height / 2), enemy->width, enemy->height, 0x0000);
		FillRect(enemy->e1Left , enemy->y, 1, 1, 0xd685); //Izquierda
		FillRect(enemy->e1Right , enemy->y, 1, 1, 0xd685); //Derecha
		FillRect(enemy->x , enemy->e1Up, 1, 1, 0xd685); //Arriba
		FillRect(enemy->x , enemy->e1Down, 1, 1, 0xd685); //Abajo
        }
}

int ColisionPlayer_e1(enemy_type1* enemy, player* player,int direction, int x, int y){
	// Verificar colisión con el enemigo basado en la dirección de movimiento
	// Dirección de movimiento (0: arriba, 1: derecha, 2: abajo, 3: izquierda)
	if (enemy->isAlive==1){
		switch (player->direction) {
				case 0: // Movimiento hacia arriba
					int y_u= y-12;
					if (y_u<=enemy->e1Down && y_u>=enemy->e1Up){
						if(player->playerLeft>=enemy->e1Left && player->playerLeft<=enemy->e1Right){
							player->y=player->y+5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
						if(player->playerRight>=enemy->e1Left && player->playerRight<=enemy->e1Right){
							player->y=player->y+5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
						if (player->x>=enemy->e1Left && player->x<=enemy->e1Right){
							player->y=player->y+5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
					}
					break;
				case 1: //Movimiento hacia derecha
					int x_r=x+12;
					if (x_r>=enemy->e1Left && x_r<=enemy->e1Right){
						if (player->playerUp>=enemy->e1Up && player->playerUp<=enemy->e1Down){
							player->x=player->x-5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
						if (player->playerDown>=enemy->e1Up && player->playerUp<=enemy->e1Down){
							player->x=player->x-5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
						if (player->y>=enemy->e1Up && player->y<=enemy->e1Down){
							player->x=player->x-5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
					}

					break;
				case 2: //Movimiento hacia abajo
					int y_d= y+12;
					if (y_d>=enemy->e1Up && y_d<=enemy->e1Down){
						if(player->playerLeft>=enemy->e1Left && player->playerLeft<=enemy->e1Right){
							player->y=player->y-5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
						if(player->playerRight>=enemy->e1Left && player->playerRight<=enemy->e1Right){
							player->y=player->y-5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
						if (player->x>=enemy->e1Left && player->x<=enemy->e1Right){
							player->y=player->y-5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
					 }
					break;
				case 3: //Movimiento hacia izquierda
					int x_l=x-12;
					if (x_l<=enemy->e1Right && x_l>=enemy->e1Left){
						if (player->playerUp>=enemy->e1Up && player->playerUp<=enemy->e1Down){
							player->x=player->x+5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
						if (player->playerDown>=enemy->e1Up && player->playerUp<=enemy->e1Down){
							player->x=player->x+5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
						if (player->y>=enemy->e1Up && player->y<=enemy->e1Down){
							player->x=player->x+5;
							player->IsDamage=1;
							player->animationDamage=0;
							player->life-=1;
							if (player->life==0){
								player->isAlive=0;
								player->animationDie=0;
							}
							return 0;
						}
					}
					break;
			return 1;
			}
	}
	return 1;
}

/* Funciones Enemigo tipo 2 -------------------------------------------*/

void Hitbox_e2(enemy_type2* enemy){
    //HITBOX DEBUG
	enemy->e1Left = enemy->x - (enemy->width / 2);
	enemy->e1Right = (enemy->x + enemy->width / 2);
	enemy->e1Up = enemy->y - (enemy->height / 2);
	enemy->e1Down= (enemy->y + enemy->height / 2);

    // Dibujar el enemigo en pantalla
    LCD_Sprite(enemy->x - (16 / 2), enemy->y - (20/ 2), 16, 20, E2_Ide16x20_5, 5, 4, 0, 0);

    if (DrawHitbox==1){
    	Rect(enemy->x - (enemy->width / 2), enemy->y - (enemy->height / 2), enemy->width, enemy->height, 0x0000);
    }
    if (DrawHitbox==2){
    	FillRect(enemy->e1Left , enemy->y, 1, 1, 0xd685); //Izquierda
    	FillRect(enemy->e1Right , enemy->y, 1, 1, 0xd685); //Derecha
    	FillRect(enemy->x , enemy->e1Up, 1, 1, 0xd685); //Arriba
    	FillRect(enemy->x , enemy->e1Down, 1, 1, 0xd685); //Abajo
    }
    if (DrawHitbox==3){
    	Rect(enemy->x - (enemy->width / 2), enemy->y - (enemy->height / 2), enemy->width, enemy->height, 0x0000);
		FillRect(enemy->e1Left , enemy->y, 1, 1, 0xd685); //Izquierda
		FillRect(enemy->e1Right , enemy->y, 1, 1, 0xd685); //Derecha
		FillRect(enemy->x , enemy->e1Up, 1, 1, 0xd685); //Arriba
		FillRect(enemy->x , enemy->e1Down, 1, 1, 0xd685); //Abajo
        }
}

void initEnemy2(enemy_type2* enemy, unsigned int startX, unsigned int startY, unsigned int width, unsigned int height, int health) {
    // Inicializar las coordenadas y dimensiones
    enemy->x = startX;
    enemy->y = startY;
    enemy->width = width;
    enemy->height = height;

    // Inicializar la vida y el estado
    enemy->health = health;
    enemy->isAlive = 1;  // El enemigo comienza vivo
    enemy->move=0;

    //Animación
    enemy->animationV = 0;
    enemy->animationDV = 3;
    enemy->animationDie=6;

    //Movimiento
    enemy->IsMove=0;
    enemy->move=0;
    enemy->delay=0;


}

void moveE2(enemy_type2* enemy, player* player){
	if (enemy->isHurt==0){
		int delay_s=enemy->delay;
		if (delay_s==100000){ //
			switch(enemy->move){
			case 0: //Derecha
				int futureX_D=player->x+50;
				if (futureX_D>player->limitWidth-20){
					enemy->move+=1;
					moveE2(enemy,player);
					}else{
					FillRect(enemy->x - (16 / 2), enemy->y - (20/ 2), 16, 20,0xFE8B);
					enemy->x=futureX_D;
					enemy->y=player->y;
					enemy->animationV=0;
					enemy->move+=1;
					Hitbox_e2(enemy);
				}
				break;
			case 1: //Abajo
				int futureY_A=player->y+50;
				if (futureY_A>player->limitHeight-20){
					enemy->move+=1;
					moveE2(enemy,player);
				}else {
					FillRect(enemy->x - (16 / 2), enemy->y - (20/ 2), 16, 20,0xFE8B);
					enemy->y=futureY_A;
					enemy->x=player->x;
					enemy->animationV=0;
					enemy->move+=1;
					Hitbox_e2(enemy);
				}
				break;
			case 2: //Izquierda
				int futureX_I=player->x-50;
				if (futureX_I<player->limitWidth_i+20){
						enemy->move+=1;
						moveE2(enemy,player);
					}else{
					FillRect(enemy->x - (16 / 2), enemy->y - (20/ 2), 16, 20,0xFE8B);
					enemy->x=futureX_I;
					enemy->y=player->y;
					enemy->animationV=0;
					enemy->move+=1;
					Hitbox_e2(enemy);
				}
				break;
			case 3: //Arriba
				int futureY_U=player->y-50;
				if (futureY_U>player->limitHeight+20){
					enemy->move+=1;
					moveE2(enemy,player);
				}else {
					FillRect(enemy->x - (16 / 2), enemy->y - (20/ 2), 16, 20,0xFE8B);
					enemy->y=futureY_U;
					enemy->x=player->x;
					enemy->animationV=0;
					enemy->move=0;
					Hitbox_e2(enemy);
				}
				break;
			}
		}else{
			if (delay_s==200000){ //
				enemy->delay=0;
			}
		}
	}
}

void moveE2_2(enemy_type2* enemy, player* player){
	if (enemy->isHurt==0){
		int delay_s2=enemy->delay;
		if (delay_s2==100000){
			switch(enemy->move){
			case 0: //Derecha
				int futureX_D2=player->x+50;
				if (futureX_D2>player->limitWidth-20){
					enemy->move+=1;
					moveE2_2(enemy,player);
					}else{
					FillRect(enemy->x - (16 / 2), enemy->y - (20/ 2), 16, 20,0xFE8B);
					enemy->x=futureX_D2;
					enemy->y=player->y;
					enemy->animationV=0;
					enemy->move+=1;
					Hitbox_e2(enemy);
				}
				break;
			case 1: //Abajo
				int futureY_A2=player->y+50;
				if (futureY_A2>player->limitHeight-20){
					enemy->move+=1;
					moveE2_2(enemy,player);
				}else {
					FillRect(enemy->x - (16 / 2), enemy->y - (20/ 2), 16, 20,0xFE8B);
					enemy->y=futureY_A2;
					enemy->x=player->x;
					enemy->animationV=0;
					enemy->move+=1;
					Hitbox_e2(enemy);
				}
				break;
			case 2: //Izquierda
				int futureX_I2=player->x-50;
				if (futureX_I2<player->limitWidth_i+20){
						enemy->move+=1;
						moveE2_2(enemy,player);
					}else{
					FillRect(enemy->x - (16 / 2), enemy->y - (20/ 2), 16, 20,0xFE8B);
					enemy->x=futureX_I2;
					enemy->y=player->y;
					enemy->animationV=0;
					enemy->move+=1;
					Hitbox_e2(enemy);
				}
				break;
			case 3: //Arriba
				int futureY_U2=player->y-50;
				if (futureY_U2>player->limitHeight+20){
					enemy->move+=1;
					moveE2_2(enemy,player);
				}else {
					FillRect(enemy->x - (16 / 2), enemy->y - (20/ 2), 16, 20,0xFE8B);
					enemy->y=futureY_U2;
					enemy->x=player->x;
					enemy->animationV=0;
					enemy->move=0;
					Hitbox_e2(enemy);
				}
				break;
			}
		}else{
			if (delay_s2==200000){
				enemy->delay=0;
			}
		}
	}
}

void E2_Appear(enemy_type2* enemy){
	int variableAnimationAppear=enemy->animationV;
	if (enemy->isAlive==1){
		if (variableAnimationAppear<5){
			LCD_Sprite(enemy->x - (16 / 2), enemy->y - (20/ 2), 16, 20, E2_Ide16x20_5, 5, variableAnimationAppear, 0, 0);
			enemy->animationV+=0.05;
			}
		}
}

void E2_Hurt(enemy_type2* enemy){
	if (enemy->isHurt==1){
		int variableAnimationHurt=enemy->animationDV;
		if (enemy->isAlive==1){
			if (variableAnimationHurt<3){
				LCD_Sprite(enemy->x - (28 / 2), enemy->y - (25/ 2), 28, 25, E2_Damage28x25, 3, variableAnimationHurt, 0, 0);
				enemy->animationDV+=0.05;
				} else{
					enemy->isHurt=0;
				}
			}
	}
}

void E2_HurtS(enemy_type2* enemy){
	if (enemy->isHurt==1){
		int variableAnimationHurtS=enemy->animationDV;
		if (enemy->isAlive==1){
			if (variableAnimationHurtS<3){
				switch (variableAnimationHurtS){
				case 0:
					setPWM_Frequency(6500); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);
					break;
				case 1:
					setPWM_Frequency(4500); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);
					break;
				case 2:
					setPWM_Frequency(2500); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);
					break;
				}
			}
		}
	}
}

void E2_Die(enemy_type2* enemy){
	int variableAnimationDie=enemy->animationDie;
			if (enemy->isAlive==0){
				if (variableAnimationDie<6){
					LCD_Sprite(enemy->x - (26 / 2)+1, enemy->y - (23 / 2), 26,23, E2_Die26x23, 6, variableAnimationDie, 0, 0);
					enemy->animationDie+=0.2;
					}else{
						FillRect(enemy->x - (26 / 2)+1, enemy->y - (23 / 2), 26,23, 0xFE8B);
						enemy->x=5000;
						enemy->y=5000;
					}
				}
}

void E2_DieS(enemy_type2* enemy){
	int variableAnimationDieS=enemy->animationDie;
			if (enemy->isAlive==0){
				if (variableAnimationDieS<6){
					switch (variableAnimationDieS){
					case 0:
						setPWM_Frequency(15000); // Frecuencia de 1 kHz
						setPWM_DutyCycle(50);
						break;
					case 1:
						setPWM_Frequency(6000); // Frecuencia de 1 kHz
						setPWM_DutyCycle(50);
						break;
					case 2:
						setPWM_Frequency(10000); // Frecuencia de 1 kHz
						setPWM_DutyCycle(50);
						break;
					case 3:
						setPWM_Frequency(4500); // Frecuencia de 1 kHz
						setPWM_DutyCycle(50);
						break;
					case 4:
						setPWM_Frequency(4500); // Frecuencia de 1 kHz
						setPWM_DutyCycle(50);
						break;
					case 5:
						setPWM_Frequency(4500); // Frecuencia de 1 kHz
						setPWM_DutyCycle(0);
						break;
					}
				}
			}
}

void PlayerHit_E2(player* player, enemy_type2* enemy){
	if (enemy->isAlive == 1){
		// Verificar si le pego a un enemigo del tipo 1
		// Dirección de movimiento (0: arriba, 1: derecha, 2: abajo, 3: izquierda)
		switch (player->direction){
			case 0: { // Golpe hacia arriba
				int rangoY = player->y - 15;
				if (rangoY<=enemy->e1Down && rangoY>=enemy->e1Up){
					if (player->x>=(enemy->e1Left-5) && player->x<=(enemy->e1Right+5)){
						enemy->health -= 1;
						enemy->isHurt=1;
						enemy->animationDV=0;
						if(enemy->health==0){
							enemy->isAlive=0;
							enemy->animationDie=0;
						}
					}
				}
			break;
			}
			case 2:{ //Golpe hacia abajo
				int rangoY_d= player->y + 15;
				if (rangoY_d<=enemy->e1Down && rangoY_d>=enemy->e1Up){
					if (player->x>=(enemy->e1Left-5) && player->x<=(enemy->e1Right+5)){
						enemy->health -= 1;
						enemy->isHurt=1;
						enemy->animationDV=0;
						if(enemy->health==0){
							enemy->isAlive=0;
							enemy->animationDie=0;
						}
					}
				}
				break;
			}
			case 1:{ //Golpe hacia derecha
				int rangoX= player->x + 20;
				if (rangoX<=enemy->e1Right && rangoX>=enemy->e1Left){
					if (player->y>=(enemy->e1Down-15) && player->y<=(enemy->e1Up+20)){
						enemy->health -= 1;
						enemy->isHurt=1;
						enemy->animationDV=0;
						if(enemy->health==0){
							enemy->isAlive=0;
							enemy->animationDie=0;
						}
					}
				}
				break;
			}
			case 3:{ //Golpe hacia izquierda
							int rangoX= player->x - 20;
							if (rangoX<=enemy->e1Right && rangoX>=enemy->e1Left){
								if (player->y>=(enemy->e1Down-15) && player->y<=(enemy->e1Up+20)){
									enemy->health -= 1;
									enemy->isHurt=1;
									enemy->animationDV=0;
									if(enemy->health==0){
										enemy->isAlive=0;
										enemy->animationDie=0;
									}
								}
							}
							break;
						}
		}
	}
}

/* Funciones Enemigo 3 -------------------------------------------------*/
void initEnemy3(enemy_type3* enemy, unsigned int startX, unsigned int startY, unsigned int width, unsigned int height, int health, player* player) {
    // Inicializar las coordenadas y dimensiones
    enemy->x = startX;
    enemy->y_eye=startY;
    enemy->y = startY+20;
    enemy->width = width;
    enemy->height = height;
    enemy->speed=5;
    enemy->Place=0;
    enemy->delay=0;
    enemy->delay_init=0;

    // Inicializar la vida y el estado
    enemy->health = health;
    enemy->isAlive = 1;  // El enemigo comienza vivo

    //Animación
    enemy->animationFire=0;
    enemy->animationEye=0;
    enemy->isMove=0;

    //Limites
    enemy->limitwidth=player->limitWidth;
    enemy->limitwidth_i=player->limitWidth_i;
    enemy->limitheight_i=18;
    enemy->limitheight=220;
}

void E3_Eye(enemy_type3* enemy){
	if (enemy->health>0){
		if (enemy->isMove==0){
			enemy->y=enemy->y_eye+20;
			int variableEye=enemy->animationEye;
			if (variableEye<7){
				LCD_Sprite(enemy->x - (16 / 2), enemy->y_eye - (17 / 2), 16, 17, E3_Eye16x17_7, 7, variableEye, 0, 0);
				enemy->animationEye+=0.1;
			}
			else{
				FillRect(enemy->x - (16 / 2), enemy->y_eye - (17 / 2), 16, 17, 0xFE8B);
				enemy->isMove=1;
				enemy->animationFire=0;
			}
		}
	} else{
		FillRect(enemy->x - (16 / 2), enemy->y_eye - (17 / 2), 16, 17, 0xFE8B);
	}
}

void E3_EyeS(enemy_type3* enemy){
	if (enemy->health>0){
		if (enemy->isMove==0){
			int variableEyeS=enemy->animationEye;
			if (variableEyeS<7){
				switch (variableEyeS){
				case 1:
					setPWM_Frequency(6500); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);   // Duty cycle al 50%
					break;
				case 2:
					setPWM_Frequency(45000); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);   // Duty cycle al 50%
					break;
				case 3:
					setPWM_Frequency(3000); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);   // Duty cycle al 50%
					break;
				case 4:
					setPWM_Frequency(2000); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);   // Duty cycle al 50%
					break;
				case 5:
					setPWM_Frequency(1000); // Frecuencia de 1 kHz
					setPWM_DutyCycle(50);   // Duty cycle al 50%
					break;
				case 6:
					setPWM_Frequency(10000); // Frecuencia de 1 kHz
					setPWM_DutyCycle(0);   // Duty cycle al 50%
					break;
				}
			}
		}
	}
}

void E3_FireMove(enemy_type3* enemy,player* player){
	HitboxPlayer(player);
	if (enemy->health>0){
		if (enemy->isMove==1){
			int FutureY=enemy->y+enemy->speed;
			if (FutureY>=220){
				enemy->isMove=0;
				enemy->animationEye=0;
				enemy->health-=1;
				enemy->Place+=1;
				enemy->delay=0;
				FillRect(enemy->x - (16 / 2), enemy->y - (16 / 2), 16, 16, 0xFE8B); //Se puede eliminar
			} else{
				FillRect(enemy->x - (16 / 2), enemy->y - (16 / 2), 16, 16, 0xFE8B);
				int animationFire=enemy->animationFire;
				enemy->y=FutureY;
				LCD_Sprite(enemy->x - (16 / 2), enemy->y - (16 / 2), 16, 16, E3_Fire16x16_4, 4, animationFire, 0, 0);
				if (player->playerUp<=enemy->e1Down){ //player->playerUp>=enemy->e1Up &&
					if((enemy->x >= player->playerLeft) && (enemy->x <= player->playerRight)){
						player->y=player->y+5;
						player->direction=0;
						player->IsDamage=1;
						player->animationDamage=0;
						player->life-=1;
						FillRect(player->x - (player->width / 2)+1, player->y - (player->height / 2)+1, player->width+1, player->height+1, 0xFE8B);
						if (player->life==0){
							player->isAlive=0;
							player->animationDie=0;
						FillRect(player->x - (player->width / 2)+1, player->y - (player->height / 2)+1, player->width+1, player->height+1, 0xFE8B);
						}

						FillRect(enemy->x - (16 / 2), enemy->y - (16 / 2), 16, 16, 0xFE8B); //Se puede eliminar
						enemy->isMove=0;
						enemy->animationEye=0;
						enemy->Place+=1;
						enemy->delay=0;
			}
				}
			}
		}
	}
}

void E3_FireAnimation(enemy_type3* enemy){
	if (enemy->health>0 && enemy->isMove){
		enemy->animationFire+=0.2;
		if (enemy->animationFire>=4){
			enemy->animationFire=0;
		}
	}
}

void E3_Hitbox(enemy_type3* enemy){

    //HITBOX DEBUG
	enemy->e1Left = enemy->x - (enemy->width / 2);
	enemy->e1Right = (enemy->x + enemy->width / 2); //+1
	enemy->e1Up = enemy->y - (enemy->height / 2);
	enemy->e1Down= (enemy->y + enemy->height / 2); //+1

		if (DrawHitbox==1){
	    	Rect(enemy->x - (enemy->width / 2), enemy->y - (enemy->height / 2), enemy->width, enemy->height, 0x0000);
	    }
	    if (DrawHitbox==2){
	    	FillRect(enemy->e1Left , enemy->y, 1, 1, 0xd685); //Izquierda
	    	FillRect(enemy->e1Right , enemy->y, 1, 1, 0xd685); //Derecha
	    	FillRect(enemy->x , enemy->e1Up, 1, 1, 0xd685); //Arriba
	    	FillRect(enemy->x , enemy->e1Down, 1, 1, 0xd685); //Abajo
	    }
	    if (DrawHitbox==3){
	    	Rect(enemy->x - (enemy->width / 2), enemy->y - (enemy->height / 2), enemy->width, enemy->height, 0x0000);
			FillRect(enemy->e1Left , enemy->y, 1, 1, 0xd685); //Izquierda
			FillRect(enemy->e1Right , enemy->y, 1, 1, 0xd685); //Derecha
			FillRect(enemy->x , enemy->e1Up, 1, 1, 0xd685); //Arriba
			FillRect(enemy->x , enemy->e1Down, 1, 1, 0xd685); //Abajo
	        }
}

void E3_MoveX(enemy_type3* enemy){
	if (enemy->delay==0){
	int FutureX;
	enemy->delay=1;
	switch (enemy->Place){
		case 0:
			FutureX=enemy->x+40;
			if (FutureX>=enemy->limitwidth){
				enemy->x=enemy->limitwidth_i+35;

			} else{
				enemy->x=FutureX;
			}
			break;

		case 1:
			FutureX=enemy->x-20;
			if (FutureX<=enemy->limitwidth_i){
				enemy->x=enemy->limitwidth-35;

			} else{
				enemy->x=FutureX;
			}
			break;
		case 2:
			FutureX=enemy->x+50;
			if (FutureX>=enemy->limitwidth){
				enemy->x=enemy->limitwidth_i+50;

			} else{
				enemy->x=FutureX;
			}
			break;
		case 3:
					FutureX=enemy->x-50;
					if (FutureX<=enemy->limitwidth_i){
						enemy->x=enemy->limitwidth-35;

					} else{
						enemy->x=FutureX;
					}
					break;
		case 4:
			enemy->Place=0;
		}
	}
}

/* Funciones Jugador ---------------------------------------------------*/
 void initPlayer(player* player, unsigned int startX, unsigned int startY, unsigned int playerWidth, unsigned int playerHeight, unsigned int speed, unsigned int life, unsigned int limitWidth, unsigned int limitHeight,unsigned int limitWidth_i) {
    // Inicializar las propiedades del jugador
    player->x = startX;
    player->y = startY;
    player->width = playerWidth;
    player->height = playerHeight;
    player->speed = speed;
    player->life = life;
    player->score = 0;          // Puntuación inicial en 0
    player->isAlive = 1;        // El jugador comienza vivo
    player->direction = 2;      // Dirección inicial (abajo)
    player->limitWidth = limitWidth;
    player->limitWidth_i = limitWidth_i; //
    player->limitHeight = limitHeight;
    player->IsAttack = 0;
    player->animationAttack=7;
	player-> IsDamage=0;
	player-> animationDamage=3;
	player-> animationDie=4;
	player->  IsDie=0;
	player-> playerLevel=0;

    // Dibujar el jugador en pantalla
    LCD_Sprite(player->x - (18 / 2)+2, player->y - (23 / 2+4), 18, 23, LinkAttackDown_18x23_6, 6, 5, 0, 0);
    //FillRect(player->x , player->y, 1, 1, 0x000000);

    //HITBOX DEBUG
    player->playerLeft=player->x-(player->width / 2);
    player->playerRight=(player->x+(player->width / 2)-1);
    player->playerUp=player->y-(player->height / 2);
    player->playerDown=(player->y+(player->height / 2)-1);

}

int playerCanMove(player* player, unsigned int direction) {
	// Dirección de movimiento (0: arriba, 1: derecha, 2: abajo, 3: izquierda)
	switch(player->direction){
	case 0: //Colisiones HITBOX Superior
		int FutureplayerUp = (player->y - player->speed)-(player->height / 2);
		//Borde
		if (FutureplayerUp<=18){
			return 0;
		}
		break;
	case 1: //Colisiones HITBOX Derecha
		int FutureplayerRight = ((player->x+ player->speed)+(player->width / 2));
		//Borde
		if (FutureplayerRight>=player->limitWidth){
			return 0;
		}
		break;
	case 2: //Colisiones HITBOX Abajo
		int FutureplayerDown=((player->y+ player->speed)+(player->height / 2));
		//Borde
		if (FutureplayerDown>=player->limitHeight){
			return 0;
		}
		break;
	case 3: //Colisiones HITBOX Izquierda (Cambiar a Width_limite inferior)
		int FutureplayerLeft = ((player->x-player->speed)-(player->width / 2));
		//Borde
		if (FutureplayerLeft<=player->limitWidth_i){
			return 0;
		}
	}

    // Variables para calcular la posición futura del jugador según la dirección
    int futureX = player->x;
    int futureY = player->y;

    if (player->PlayerNum==1){
    //Colision con E1
    if (ColisionPlayer_e1(&e1_1, player, direction,futureX,futureY)==0){
    	return 0;
    }
    if (ColisionPlayer_e1(&e1_2, player, direction,futureX,futureY)==0){
        	return 0;
        }
    if (ColisionPlayer_e1(&e1_3, player, direction,futureX,futureY)==0){
        	return 0;
        }
    }

    if (player->PlayerNum==2){
       //Colision con E1
       if (ColisionPlayer_e1(&e1_4, player, direction,futureX,futureY)==0){
       	return 0;
       }
       if (ColisionPlayer_e1(&e1_5, player, direction,futureX,futureY)==0){
           	return 0;
           }
       if (ColisionPlayer_e1(&e1_6, player, direction,futureX,futureY)==0){
           	return 0;
           }
       }
    // No hay colisiones, se puede mover
    return 1;
}

void HitboxPlayer(player* player){

    //HITBOX DEBUG
    player->playerLeft=player->x-(player->width / 2);
    player->playerRight=(player->x+(player->width / 2)); //-1
    player->playerUp=player->y-(player->height / 2);
    player->playerDown=(player->y+(player->height / 2)); //-1

    if (DrawHitbox==1){
        	Rect(player->x - (player->width / 2), player->y - (player->height / 2), player->width, player->height, 0x0000);
        }
    if (DrawHitbox==2){
		FillRect(player->playerLeft , player->y, 1, 1, 0xd685); //Izquierda
		FillRect(player->playerRight , player->y, 1, 1, 0xd685); //Derecha
		FillRect(player->x , player->playerUp, 1, 1, 0xd685); //Arriba
		FillRect(player->x , player->playerDown, 1, 1, 0xd685); //Abajo}
    }
    if (DrawHitbox==3){
    	 Rect(player->x - (player->width / 2), player->y - (player->height / 2), player->width, player->height, 0x0000);
        FillRect(player->playerLeft , player->y, 1, 1, 0xd685); //Izquierda
        FillRect(player->playerRight , player->y, 1, 1, 0xd685); //Derecha
        FillRect(player->x , player->playerUp, 1, 1, 0xd685); //Arriba
        FillRect(player->x , player->playerDown, 1, 1, 0xd685); //Abajo}

        }
}

void PlayerHit(player* player, enemy_type1* enemy){
	if (enemy->isAlive == 1){
		// Verificar si le pego a un enemigo del tipo 1
		// Dirección de movimiento (0: arriba, 1: derecha, 2: abajo, 3: izquierda)
		switch (player->direction){
			case 0: { // Golpe hacia arriba
				int rangoY = player->y - 15;
				if (rangoY<=enemy->e1Down && rangoY>=enemy->e1Up){
					if (player->x>=(enemy->e1Left-5) && player->x<=(enemy->e1Right+5)){
						enemy->health -= 1;
						if(enemy->health==0){
							enemy->isAlive=0;
							enemy->animationDie=0;
						}
					}
				}
			break;
			}
			case 2:{ //Golpe hacia abajo
				int rangoY_d= player->y + 15;
				if (rangoY_d<=enemy->e1Down && rangoY_d>=enemy->e1Up){
					if (player->x>=(enemy->e1Left-5) && player->x<=(enemy->e1Right+5)){
						enemy->health -= 1;
						if(enemy->health==0){
							enemy->isAlive=0;
							enemy->animationDie=0;
						}
					}
				}
				break;
			}
			case 1:{ //Golpe hacia derecha
				int rangoX= player->x + 20;
				if (rangoX<=enemy->e1Right && rangoX>=enemy->e1Left){
					if (player->y>=(enemy->e1Down-15) && player->y<=(enemy->e1Up+20)){
						enemy->health -= 1;
						if(enemy->health==0){
							enemy->isAlive=0;
							enemy->animationDie=0;
						}
					}
				}
				break;
			}
			case 3:{ //Golpe hacia izquierda
							int rangoX= player->x - 20;
							if (rangoX<=enemy->e1Right && rangoX>=enemy->e1Left){
								if (player->y>=(enemy->e1Down-15) && player->y<=(enemy->e1Up+20)){
									enemy->health -= 1;
									if(enemy->health==0){
										enemy->isAlive=0;
										enemy->animationDie=0;
									}
								}
							}
							break;
						}
		}
	}
}

void PlayerAnimation(player* player){
	if (player->isAlive==1){
		//(0: arriba, 1: derecha, 2: abajo, 3: izquierda)
		if (player->direction==0){
			LCD_Sprite(player->x - (18 / 2)+1, player->y - (26 / 2), 18, 26, LinkUpWalk180x26_10, 10, player->animationWalkUp, 0, 0);
		}
		if (player->direction==1){
			LCD_Sprite(player->x - (22 / 2)+1, player->y - (23 / 2), 22, 23, LinkSideWalk220x23_10, 10, player->animationWalkRight, 1, 0);
		}
		if (player->direction==2){
			LCD_Sprite(player->x - (18 / 2)+1, player->y - (24 / 2)+1, 18, 24, LinkDownWalk180x24_10, 10, player->animationWalkDown, 0, 0);
			}
		if (player->direction==3){

			LCD_Sprite(player->x - (22 / 2)+1, player->y - (23 / 2), 22, 23, LinkSideWalk220x23_10, 10, player->animationWalkLeft, 0, 0);
			}
	}
}

void PlayerAttackAnimation(player* player){
	int variableAnimationAttack=player->animationAttack;
	if (player->isAlive==1){
		if (variableAnimationAttack<6){
			//(0: arriba, 1: derecha, 2: abajo, 3: izquierda)
			if (player->direction==0){

				LCD_Sprite(player->x - (18 / 2)+1, player->y - (22 / 2), 18, 22, LinkAttackUp_18x22_6, 6, variableAnimationAttack, 0, 0);
			}
			if (player->direction==1){

				LCD_Sprite(player->x - (20 / 2)+1, player->y - (22 / 2), 20, 22, LinkAttackSide_20x22_6, 6, variableAnimationAttack, 1, 0);			}
			if (player->direction==2){
				LCD_Sprite(player->x - (18 / 2)+1,player->y - (23 / 2), 18, 23, LinkAttackDown_18x23_6, 6, variableAnimationAttack, 0, 0);
			}
			if (player->direction==3){

				LCD_Sprite(player->x - (20 / 2)+1, player->y - (22 / 2), 20, 22, LinkAttackSide_20x22_6, 6, variableAnimationAttack, 0, 0);
				}
			player->animationAttack+=0.2;
			}else{
				player->IsAttack=0;
			}
		}
}

void PlayerAttackSound(player* player){
	int variableAnimationAttackS=player->animationAttack;
	if (player->isAlive==1){
		if (variableAnimationAttackS<6){
			switch (variableAnimationAttackS){
			case 0:
		        setPWM_Frequency(1000); // Frecuencia de 1 kHz
		        setPWM_DutyCycle(50);   // Duty cycle al 50%
				break;
			case 1:
		        setPWM_Frequency(2000); // Frecuencia de 1 kHz
		        setPWM_DutyCycle(50);   // Duty cycle al 50%
				break;
			case 2:
		        setPWM_Frequency(3000); // Frecuencia de 1 kHz
		        setPWM_DutyCycle(50);   // Duty cycle al 50%
				break;
			case 3:
		        setPWM_Frequency(4000); // Frecuencia de 1 kHz
		        setPWM_DutyCycle(50);   // Duty cycle al 50%
				break;
			case 4:
		        setPWM_Frequency(5000); // Frecuencia de 1 kHz
		        setPWM_DutyCycle(50);   // Duty cycle al 50%
				break;
			case 5:
		        setPWM_Frequency(6000); // Frecuencia de 1 kHz
		        setPWM_DutyCycle(0);   // Duty cycle al 50%
				break;
			}

			}
	}
}

void PlayerDamageAnimation(player* player){
	int variableAnimationDamage=player->animationDamage;
	if (player->isAlive==1){
		if (variableAnimationDamage<3){
			//(0: arriba, 1: derecha, 2: abajo, 3: izquierda)
			if (player->direction==0){

				LCD_Sprite(player->x - (18 / 2)+1, player->y - (20 / 2), 18, 20, LinkDamageUp_18x20_3, 3, variableAnimationDamage, 0, 0);
			}
			if (player->direction==1){

				LCD_Sprite(player->x - (20 / 2)+1, player->y - (21 / 2), 20, 21, LinkDamageSide_20x21_3, 3, variableAnimationDamage, 1, 0);
			}
			if (player->direction==2){

				LCD_Sprite(player->x - (20 / 2)+1, player->y - (21 / 2), 20, 21, LinkDamageDown_20x21_3, 3, variableAnimationDamage, 0, 0);
					}
			if (player->direction==3){

				LCD_Sprite(player->x - (20 / 2)+1, player->y - (21 / 2), 20, 21, LinkDamageSide_20x21_3, 3, variableAnimationDamage, 0, 0);
					}
			player->animationDamage+=0.2;
			}else{
				player->IsDamage=0;
			}
		}
}

void PlayerDamageSound(player* player){
	int variableAnimationDamageS=player->animationDamage;
	if (player->isAlive==1){
		if (variableAnimationDamageS<3){
			switch (variableAnimationDamageS){
			case 0:
				setPWM_Frequency(5000); // Frecuencia de 1 kHz
				setPWM_DutyCycle(50);   // Duty cycle al 50%
				break;
			case 1:
				setPWM_Frequency(8000); // Frecuencia de 1 kHz
				setPWM_DutyCycle(50);   // Duty cycle al 50%
				break;
			case 2:
				setPWM_Frequency(1000); // Frecuencia de 1 kHz
				setPWM_DutyCycle(0);   // Duty cycle al 50%
				break;
			}
			}
		}
}

void PlayerDieAnimation(player* player){
	int variableAnimationDie=player->animationDie;
	if (player->isAlive==0){
		if (variableAnimationDie<4){
			FillRect(player->x - (player->width / 2)+1, player->y - (player->height / 2), player->width+1, player->height+1, 0xFE8B);
			LCD_Sprite(player->x - (24 / 2)+1, player->y - (24 / 2), 24,24, LinkDie_24x24_4, 4, variableAnimationDie, 0, 0);
			player->animationDie+=0.2;
			}
		}
}

void PlayerDieSound(player* player){
	int variableAnimationDieS=player->animationDie;
	if (player->isAlive==0){
		if (variableAnimationDieS<4){
			switch (variableAnimationDieS){
			case 0:
				setPWM_Frequency(10000); // Frecuencia de 1 kHz
				setPWM_DutyCycle(50);   // Duty cycle al 50%
				break;
			case 1:
				setPWM_Frequency(5000); // Frecuencia de 1 kHz
				setPWM_DutyCycle(50);   // Duty cycle al 50%
				break;
			case 2:
				setPWM_Frequency(10000); // Frecuencia de 1 kHz
				setPWM_DutyCycle(50);   // Duty cycle al 50%
				break;
			case 3:
				setPWM_Frequency(1000); // Frecuencia de 1 kHz
				setPWM_DutyCycle(0);   // Duty cycle al 50%
				break;

			}
			}
		}
}

void initLevelSolo(void){
	FillRect(p1.x - (p1.width / 2)+1, p1.y - (p1.height / 2)+1, p1.width+1, p1.height+1, 0xFE8B);
	if (IniciarLevel==1){
		//Inicializar Jugador 1
		initPlayer(&p1, 160, 200, 22, 30, 5, 3, 300, 220,18);
		initPlayer(&p2, 160, 200, 22, 30, 5, 3, 300, 220,18);
		p1.PlayerNum=1;
		p2.PlayerNum=2;
		p2.isAlive=0;

		if (nivelActual1==NIVEL1){
			//Inicializar enemigo 1
			initEnemy1(&e1_1, 40, 80, 16, 19, 3);
			//Inicializar enemigo 2
			initEnemy1(&e1_2, 160, 80, 16, 19, 3);
			//Inicializar enemigo 3
			initEnemy1(&e1_3, 280, 80, 16, 19, 3);}

		  if (nivelActual1==NIVEL2){
			  initEnemy2(&e2_1, 160, 80, 16, 20, 3);
			  IniciarLevel=0;
			  }

		  if (nivelActual1==NIVEL3){
			  initEnemy3(&e3_1, 80, 30, 15, 15, 15, &p1);
			  IniciarLevel=0;
		  	  }
		  }
}

void initLevelP1(void){
	FillRect(p1.x - (p1.width / 2)+1, p1.y - (p1.height / 2)+1, p1.width+1, p1.height+1, 0xFE8B);
	if (IniciarLevel==1){
		//Linea de en medio
		V_line(160, 0, 240, 0x0000);
		initPlayer(&p1, 80, 200, 22, 30, 5, 3, 160, 220,18);
		p1.PlayerNum=1;
		p2.PlayerNum=2;

		if (nivelActual1==NIVEL1){
			//Inicializar enemigo 1
			initEnemy1(&e1_1, 80, 80, 16, 19, 3);
			IniciarLevel=0;}

		  if (nivelActual1==NIVEL2){
			  initEnemy2(&e2_1, 80, 80, 16, 20, 3);
			  IniciarLevel=0;
			  }

		  if (nivelActual1==NIVEL3){
			  initEnemy3(&e3_1, 80, 30, 15, 15, 3, &p1);
			  IniciarLevel=0;
		  	  }
		  }

}

void initLevelP2(void){
	FillRect(p2.x - (p2.width / 2)+1, p2.y - (p2.height / 2)+1, p2.width+1, p2.height+1, 0xFE8B);
	if (IniciarLevel2==1){
		//Inicializar Jugador 1
		//Linea de en medio
		V_line(160, 0, 240, 0x0000);
		initPlayer(&p2, 240, 200, 22, 30, 5, 3, 300, 220,160);
		p1.PlayerNum=1;
		p2.PlayerNum=2;

		 if (nivelActual2==NIVEL1){
			//Inicializar enemigo 1
			initEnemy1(&e1_4, 240, 120, 16, 19, 3);
			IniciarLevel2=0;
			}

	     if (nivelActual2==NIVEL2){
			  initEnemy2(&e2_2, 240, 80, 16, 20, 3);
			IniciarLevel2=0;
		  }

	     if (nivelActual2==NIVEL3){
		  initEnemy3(&e3_2, 240, 30, 15, 15, 3, &p2);
		  IniciarLevel=0;
		  }
	  }

}

void CargarMultiplesBitmaps(char *baseName) {
    char fileName[20];
    int y_offset = 0;

    for (int i = 1; i <= 8; i++) {
        // Formateamos el nombre del archivo, agregando el número y "_h.bin"
        sprintf(fileName, "%s%d_h.bin", baseName, i);

        // Intentamos cargar el bitmap desde la tarjeta SD
        if (CargarBitmaps_SD(fileName)) {
            // Mostramos el bitmap en la posición correspondiente
            LCD_Bitmap(0, y_offset, 320, 30, fragmento_imagen);
        }

        // Aumentamos el desplazamiento vertical para el siguiente bitmap
        y_offset += 30;
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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_UART5_Init();
  MX_FATFS_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

	LCD_Init();

	//Cargar el fondo en 8 fragmentaciones
	CargarMultiplesBitmaps("mp");


	// Activar bandera interrupcion
	HAL_UART_Receive_IT(&huart5, buffer, 1);

	// ESTADO DE INICIO DEL JUEGO
	estadoActual = MENU;
	nivelActual1 = NIVEL1;
	nivelActual2 = NIVEL1;
	modo = 0;
    fase_p1=1;
    fase_p2=1;


  	if (estadoActual == SOLO){
	//Inicializar Jugador 1
	initPlayer(&p1, 160, 200, 22, 30, 5, 3, 300, 220,18);
	initPlayer(&p2, 160, 200, 22, 30, 5, 3, 300, 220,18);
	p1.PlayerNum=1;
	p2.PlayerNum=2;
	p2.isAlive=0;

	if (nivelActual1==NIVEL1){
		//Inicializar enemigo 1
		initEnemy1(&e1_1, 40, 80, 16, 19, 3);
		//Inicializar enemigo 2
		initEnemy1(&e1_2, 160, 80, 16, 19, 3);
		//Inicializar enemigo 3
		initEnemy1(&e1_3, 280, 80, 16, 19, 3);}

	  if (nivelActual1==NIVEL2){
		  initEnemy2(&e2_1, 160, 80, 16, 20, 3);
		  }

	  if (nivelActual1==NIVEL3){
		  initEnemy3(&e3_1, 80, 30, 15, 15, 15, &p1);
	  	  }
}
  if (estadoActual==DUO){
	//Linea de en medio
	V_line(160, 0, 240, 0x0000);
	initPlayer(&p1, 80, 200, 22, 30, 5, 3, 160, 220,18); //80
	initPlayer(&p2, 240, 200, 22, 30, 5, 3, 300, 220,160); //240
	p1.PlayerNum=1;
	p2.PlayerNum=2;

	if (nivelActual1==NIVEL1){
		//Inicializar enemigo 1
		initEnemy1(&e1_1, 80, 80, 16, 19, 3);}
	 if (nivelActual1==NIVEL2){
		  initEnemy2(&e2_1, 90, 80, 16, 20, 3);
		  }
	  if (nivelActual1==NIVEL3){
		  initEnemy3(&e3_1, 80, 30, 15, 15, 15, &p1);}
	  if (nivelActual2==NIVEL1){
		//Inicializar enemigo 1
		initEnemy1(&e1_4, 240, 120, 16, 19, 3);
	    }
	  if (nivelActual2==NIVEL2){
	  	  initEnemy2(&e2_2, 230, 80, 16, 20, 3);
	  }
	  if (nivelActual2==NIVEL3){
		  initEnemy3(&e3_2, 240, 30, 15, 15, 15, &p2);}
	  HitboxPlayer(&p1);
	  HitboxPlayer(&p2);
	}

  // Iniciar PWM
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		if (repintarFondo) {
		switch (estadoActual) {
			case MENU:
				CargarMultiplesBitmaps("mp");
				break;
			case SOLO:
				CargarMultiplesBitmaps("fb");
				LCD_Sprite(p1.x - (20 / 2)+1, p1.y - (21 / 2), 20, 21, LinkDamageSide_20x21_3, 3, 0, 0, 0);
				if (IniciarP1==1){
				initLevelSolo();}
				break;
			case DUO:
				CargarMultiplesBitmaps("fb");
				V_line(160, 0, 240, 0x0000);
				LCD_Sprite(p1.x - (20 / 2)+1, p1.y - (21 / 2), 20, 21, LinkDamageSide_20x21_3, 3, 0, 0, 0);
				LCD_Sprite(p2.x - (20 / 2)+1, p2.y - (21 / 2), 20, 21, LinkDamageSide_20x21_3, 3, 0, 0, 0);
				if (IniciarP1==1){
					initLevelP1();}
				if (IniciarP2==1){
					initLevelP2();}
				break;
			case PAUSA:
				CargarMultiplesBitmaps("pausa");
				break;
			case WIN1:
				break;
			case WIN2:
				break;
			case GAMEOVER:
				break;
		}
		repintarFondo = 0; // Fondo pintado, no es necesario repintar.
	}
	if (pintarFondoPausa == 1) {
		// Pinta el fondo de PAUSA solo una vez
		CargarMultiplesBitmaps("pausa");  // Función que pinta el fondo de PAUSA
		pintarFondoPausa = 0;  // Desactiva la bandera
	}
	switch(estadoActual){
	case MENU:
		// En funcion del estadoFuturo, se seleccionara que mostrar en la pantlla
		// (Para mostrar que opcion selecciono en le menuPrincipal)
		switch(estadoFuturo){
		case SOLO:
			LCD_Bitmap(20, 180, 110, 30, play_solo_selected);
			LCD_Bitmap(190, 180, 110, 30, play_duo);
			break;
		case DUO:
			LCD_Bitmap(190, 180, 110, 30, play_duo_selected);
			LCD_Bitmap(20, 180, 110, 30, play_solo);
			break;
		default:
			LCD_Bitmap(20, 180, 110, 30, play_solo);
			LCD_Bitmap(190, 180, 110, 30, play_duo);
			break;
		}
		break;
	case SOLO:{
		if (nivelActual1==NIVEL1){
				if(e1_1.isAlive==1){
					animation_e1(&e1_1);
					animation_e1_control(&e1_1);}

				if(e1_2.isAlive==1){
					animation_e1(&e1_2);
					animation_e1_control(&e1_2);}

				if(e1_3.isAlive==1){
					animation_e1(&e1_3);
					animation_e1_control(&e1_3);}

				animation_e1_die(&e1_1);
				animation_e1_dieS(&e1_1);
				animation_e1_die(&e1_2);
				animation_e1_dieS(&e1_2);
				animation_e1_die(&e1_3);
				animation_e1_dieS(&e1_3);
				if (e1_1.isAlive==0&&e1_2.isAlive==0&&e1_3.isAlive==0 && e1_1.animationDie>=11 && e1_2.animationDie>=11 && e1_3.animationDie>=11){
					nivelActual1=NIVEL2;
					IniciarLevel=1;
					initLevelSolo();
				}
			}

		if (nivelActual1==NIVEL2){
			if (e2_1.isAlive==1){
				moveE2(&e2_1, &p1);
				e2_1.delay+=1; //1
				E2_Appear(&e2_1);
				E2_Hurt(&e2_1);
				E2_HurtS(&e2_1);

			}
			E2_Die(&e2_1);
			if (e2_1.isAlive==0&&e2_1.animationDie>=6){
				nivelActual1=NIVEL3;
				IniciarLevel=1;
				initLevelSolo();
			}
		}

		if (nivelActual1==NIVEL3){
			if (e3_1.isAlive==1){
			E3_MoveX(&e3_1);
			E3_Eye(&e3_1);
			E3_EyeS(&e3_1);
			E3_FireMove(&e3_1,&p1);
			E3_Hitbox(&e3_1);
			E3_FireAnimation(&e3_1);

			}
			if (e3_1.health==0){
				estadoActual = WIN1;
			}
		}

		PlayerAttackAnimation(&p1);
		PlayerAttackSound(&p1);
		PlayerDamageAnimation(&p1);
		PlayerDamageSound(&p1);
		PlayerDieAnimation(&p1);
		PlayerDieSound(&p1);

		if(p1.isAlive == 0){
			estadoActual = GAMEOVER;
		}

		break;}

	case DUO:{
		if (nivelActual1==NIVEL1){
			if(e1_1.isAlive==1){
				animation_e1(&e1_1);
				animation_e1_control(&e1_1);}
				animation_e1_die(&e1_1);
				animation_e1_dieS(&e1_1);

			if ( e1_1.isAlive==0&& e1_1.animationDie>=11 ){
				nivelActual1=NIVEL2;
				IniciarLevel=1;
				initLevelP1();
			}
		}

		if (nivelActual1==NIVEL2){
			if (e2_1.isAlive==1){
				moveE2(&e2_1, &p1);
				if (nivelActual2!=NIVEL2){
				e2_1.delay+=1000;}//0.5
				else{
					e2_1.delay+=1;
				}
				E2_Appear(&e2_1);
				E2_Hurt(&e2_1);
				E2_HurtS(&e2_1);
			}
			E2_Die(&e2_1);
			if (e2_1.isAlive==0&&e2_1.animationDie>=6){
				e3_1.isAlive=1;
				nivelActual1=NIVEL3;
				IniciarLevel=1;
				initLevelP1();
			}
		}
		if (nivelActual1==NIVEL3){
			if (e3_1.isAlive==1){
				E3_MoveX(&e3_1);
				E3_Eye(&e3_1);
				E3_EyeS(&e3_1);
				E3_FireMove(&e3_1,&p1);
				E3_Hitbox(&e3_1);
				E3_FireAnimation(&e3_1);
			}
			if (e3_1.health==0){
				estadoActual = WIN1;
			}
		}

		if (nivelActual2==NIVEL1){
			if(e1_4.isAlive==1){
				animation_e1(&e1_4);
				animation_e1_control(&e1_4);}
				animation_e1_die(&e1_4);
				animation_e1_dieS(&e1_4);

			if (e1_4.isAlive==0 && e1_4.animationDie>=11){
				nivelActual2=NIVEL2;
				IniciarLevel2=1;
				initLevelP2();
			}
		}

		if (nivelActual2==NIVEL2){
			if (e2_2.isAlive==1){
				moveE2(&e2_2, &p2);
				if (nivelActual1!=NIVEL2){
					e2_2.delay+=1000;
				}else{
				e2_2.delay+=1;}
				E2_Appear(&e2_2);
				E2_Hurt(&e2_2);
				E2_HurtS(&e2_2);
			}
			E2_Die(&e2_2);
			if (e2_2.isAlive==0&&e2_2.animationDie>=6){
				e3_2.isAlive=1;
				nivelActual2=NIVEL3;
				IniciarLevel2=1;
				initLevelP2();
			}
		}

		if (nivelActual2==NIVEL3){
			if (e3_2.isAlive==1){
				E3_MoveX(&e3_2);
				E3_Eye(&e3_2);
				E3_EyeS(&e3_2);
				E3_FireMove(&e3_2,&p2);
				E3_Hitbox(&e3_2);
				E3_FireAnimation(&e3_2);
			}
			if (e3_2.health==0){
				estadoActual = WIN2;
			}
		}


		PlayerAttackAnimation(&p1);
		PlayerAttackSound(&p1);
		PlayerDamageAnimation(&p1);
		PlayerDamageSound(&p1);
		PlayerDieAnimation(&p1);
		PlayerDieSound(&p1);

		PlayerAttackAnimation(&p2);
		PlayerAttackSound(&p2);
		PlayerDamageAnimation(&p2);
		PlayerDamageSound(&p2);
		PlayerDieAnimation(&p2);
		PlayerDieSound(&p2);

		if(p1.isAlive == 0 && p2.isAlive == 0){
			estadoActual = GAMEOVER;
		}

		break;}
	case PAUSA:
		//LCD_Bitmap(0, 0, 320, 240, pausa_menu);
		break;
	case WIN1:
		CargarMultiplesBitmaps("p1w");
		break;
	case WIN2:
		CargarMultiplesBitmaps("p2w");
		break;
	case GAMEOVER:
		CargarMultiplesBitmaps("ES");
		break;
	default:
		CargarMultiplesBitmaps("mp");
		break;
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
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 80-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

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
  htim2.Init.Prescaler = 80-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000000;
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
  __HAL_RCC_GPIOD_CLK_ENABLE();

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
	if (modo==1){
	HitboxPlayer(&p1);}
	if (modo==2){
		HitboxPlayer(&p1);
		HitboxPlayer(&p2);
	}

	if(buffer[0] == 'd'){
		if (p1.IsAttack==0 && p1.IsDamage==0 &&p1.isAlive==1){
			FillRect(p1.x - (p1.width / 2)+1, p1.y - (p1.height / 2)+1, p1.width+1, p1.height+1, 0xFE8B);
			p1.direction=2;
		if (playerCanMove(&p1, 0)) {
		    p1.y=p1.y+p1.speed;
		    if (p1.animationWalkDown<10){
		    		p1.animationWalkDown+=1;
		    			} else{
		    				p1.animationWalkDown=0;
		    			}
		    PlayerAnimation(&p1);
		} else {
			FillRect(p1.x - (p1.width / 2)+1, p1.y - (p1.height / 2)+1, p1.width+1, p1.height+1,0xFE8B);
			LCD_Sprite(p1.x - (18 / 2)+1, p1.y - (26 / 2)+1, 18, 26, LinkDownWalk180x24_10, 10, 0, 0, 0);
		}
	}
}
	if(buffer[0] == 'u'){
		if (p1.IsAttack==0 && p1.IsDamage==0 &&p1.isAlive==1){
			FillRect(p1.x - (p1.width / 2)+1, p1.y - (p1.height / 2)+1, p1.width+1, p1.height+1, 0xFE8B);
			p1.direction=0;
			if (playerCanMove(&p1, 2)) {
				p1.y=p1.y-p1.speed;
				if (p1.animationWalkUp<10){
						p1.animationWalkUp+=1;
						} else{
						  p1.animationWalkUp=0;
						  }
			PlayerAnimation(&p1);
			} else {
				LCD_Sprite(p1.x - (18 / 2)+1, p1.y - (26 / 2), 18, 26, LinkUpWalk180x26_10, 10, 0, 0, 0);
				//FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
			}
		}
}
	if(buffer[0] == 'r'){
		if (p1.IsAttack==0 && p1.IsDamage==0 &&p1.isAlive==1){
			FillRect(p1.x - (p1.width / 2)+1, p1.y - (p1.height / 2)+1, p1.width+1, p1.height+1, 0xFE8B);
			p1.direction=1;
			if (playerCanMove(&p1, 1)) {
				p1.x=p1.x+p1.speed;
				if (p1.animationWalkRight<10){
						p1.animationWalkRight+=1;
						} else{
						  p1.animationWalkRight=0;
						  }
			PlayerAnimation(&p1);
						  //FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
			} else {
				LCD_Sprite(p1.x - (22 / 2)+1, p1.y - (23 / 2), 22, 23, LinkSideWalk220x23_10, 10, 0, 1, 0);
				//FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
			}
	}
}
	if(buffer[0] == 'l'){
		if (p1.IsAttack==0 && p1.IsDamage==0 &&p1.isAlive==1){
			FillRect(p1.x - (p1.width / 2)+1, p1.y - (p1.height / 2)+1, p1.width+1, p1.height+1, 0xFE8B);
			p1.direction=3;
			if (playerCanMove(&p1, 3)) {
				p1.x=p1.x-p1.speed;
				if (p1.animationWalkLeft<10){
						p1.animationWalkLeft+=1;
						} else{
						  p1.animationWalkLeft=0;
						  }
			 PlayerAnimation(&p1);
						  //FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);

			} else {
				LCD_Sprite(p1.x - (22 / 2)+1, p1.y - (23 / 2), 22, 23, LinkSideWalk220x23_10, 10, 0, 0, 0);
				//FillRect(p1.x - (p1.width / 2), p1.y - (p1.height / 2), p1.width, p1.height, 0xFFFB00);
			}
	}
}
	if (buffer[0]=='b'){
		 if (estadoActual == MENU ) {
			 // IMPRIMIR SPRITE DE UN JUGADOR
			 estadoFuturo = SOLO;
			 IniciarLevel=1;
			 IniciarP1=1;
			IniciarLevel2=1;
			IniciarP2=1;
		 }
		 if (p1.IsAttack==0 && p1.IsDamage==0 &&p1.isAlive==1 && (estadoActual==SOLO||estadoActual==DUO)){
					p1.IsAttack=1;
					p1.animationAttack=0;
					PlayerHit(&p1, &e1_1);
					PlayerHit(&p1, &e1_2);
					PlayerHit(&p1, &e1_3);
					PlayerHit_E2(&p1, &e2_1);}
	}

	//Jugador 2
	if(buffer[0] == '1'){ //Arriba
			if (p2.IsAttack==0 && p2.IsDamage==0 &&p2.isAlive==1){
				FillRect(p2.x - (p2.width / 2)+1, p2.y - (p2.height / 2)+1, p2.width+1, p2.height+1, 0xFE8B);
				p2.direction=0;
				if (playerCanMove(&p2, 2)) {
					p2.y=p2.y-p2.speed;
					if (p2.animationWalkUp<10){
							p2.animationWalkUp+=1;
							} else{
							  p2.animationWalkUp=0;
							  }
				PlayerAnimation(&p2);
				} else {
					LCD_Sprite(p2.x - (18 / 2)+1, p2.y - (26 / 2), 18, 26, LinkUpWalk180x26_10, 10, 0, 0, 0);
					//FillRect(p2.x - (p2.width / 2), p2.y - (p2.height / 2), p2.width, p2.height, 0xFFFB00);
				}
			}
	}

	if(buffer[0] == '2'){ //Derecha
		if (p2.IsAttack==0 && p2.IsDamage==0 &&p2.isAlive==1){
			FillRect(p2.x - (p2.width / 2)+1, p2.y - (p2.height / 2)+1, p2.width+1, p2.height+1, 0xFE8B);
			p2.direction=1;
			if (playerCanMove(&p2, 1)) {
				p2.x=p2.x+p2.speed;
				if (p2.animationWalkRight<10){
						p2.animationWalkRight+=1;
						} else{
						  p2.animationWalkRight=0;
						  }
			PlayerAnimation(&p2);
						  //FillRect(p2.x - (p2.width / 2), p2.y - (p2.height / 2), p2.width, p2.height, 0xFFFB00);
			} else {
				LCD_Sprite(p2.x - (22 / 2)+1, p2.y - (23 / 2), 22, 23, LinkSideWalk220x23_10, 10, 0, 1, 0);
				//FillRect(p2.x - (p2.width / 2), p2.y - (p2.height / 2), p2.width, p2.height, 0xFFFB00);
			}
	}
}

	if(buffer[0] == '3'){ //Abajo
		if (p2.IsAttack==0 && p2.IsDamage==0 &&p2.isAlive==1){
			FillRect(p2.x - (p2.width / 2)+1, p2.y - (p2.height / 2)+1, p2.width+1, p2.height+1, 0xFE8B);
			p2.direction=2;
		if (playerCanMove(&p2, 0)) {
		    p2.y=p2.y+p2.speed;
		    if (p2.animationWalkDown<10){
		    		p2.animationWalkDown+=1;
		    			} else{
		    				p2.animationWalkDown=0;
		    			}
		    PlayerAnimation(&p2);
		} else {
			FillRect(p2.x - (p2.width / 2)+1, p2.y - (p2.height / 2)+1, p2.width+1, p2.height+1, 0xFE8B);
			LCD_Sprite(p2.x - (18 / 2)+1, p2.y - (26 / 2)+1, 18, 26, LinkDownWalk180x24_10, 10, 0, 0, 0);
		}
	}
}

	if(buffer[0] == '4'){ //Izquierda
		if (p2.IsAttack==0 && p2.IsDamage==0 &&p2.isAlive==1){
			FillRect(p2.x - (p2.width / 2)+1, p2.y - (p2.height / 2)+1, p2.width+1, p2.height+1, 0xFE8B);
			p2.direction=3;
			if (playerCanMove(&p2, 3)) {
				p2.x=p2.x-p2.speed;
				if (p2.animationWalkLeft<10){
						p2.animationWalkLeft+=1;
						} else{
						  p2.animationWalkLeft=0;
						  }
			 PlayerAnimation(&p2);
						  //FillRect(p2.x - (p2.width / 2), p2.y - (p2.height / 2), p2.width, p2.height, 0xFFFB00);

			} else {
				LCD_Sprite(p2.x - (22 / 2)+1, p2.y - (23 / 2), 22, 23, LinkSideWalk220x23_10, 10, 0, 0, 0);
				//FillRect(p2.x - (p2.width / 2), p2.y - (p2.height / 2), p2.width, p2.height, 0xFFFB00);
			}
	}
}

	if (buffer[0]=='5'){
		if (estadoActual == MENU ) {
					 // IMPRIMIR SPRITE DE DOS JUGADORES
			estadoFuturo = DUO;
			IniciarLevel=1;
			IniciarLevel2=1;
			IniciarP1=1;
			IniciarP2=1;
		}

		if (p2.IsAttack==0 && p2.IsDamage==0 &&p2.isAlive==1 && estadoActual==DUO){
			p2.IsAttack=1;
			p2.animationAttack=0;
			PlayerHit(&p2, &e1_4);
			PlayerHit(&p2, &e1_5);
			PlayerHit(&p2, &e1_6);
			PlayerHit_E2(&p2, &e2_2);}

	}

	if (buffer[0] == '6') {
		if (estadoActual == MENU) {
			// Al presionar 6 desde el menú, confirma el cambio al estado seleccionado (SOLO o DUO)
			estadoActual = estadoFuturo;
			repintarFondo = 1;
		} else if (estadoActual == SOLO || estadoActual == DUO) {
			// Si ya estamos en SOLO o DUO, cambiamos a PAUSA
			if (estadoActual != PAUSA) {
				estadoAnterior = estadoActual;  // Guarda el estado actual (SOLO o DUO)
				pintarFondoPausa = 1;
				estadoActual = PAUSA;
				IniciarP1=0;
				IniciarP2=0;// Cambia a PAUSA
			}
		} else if (estadoActual == PAUSA) {
			// Si estamos en PAUSA, volvemos al estado anterior
			estadoActual = estadoAnterior;     // Restauramos el estado SOLO o DUO
			repintarFondo = 1;                 // Indicamos que el fondo necesita repintarse
		}
		// Si alguno de los dos jugadores gano, y se presiona este boton
		// Regresar al menu
		else if (estadoActual == WIN1 || estadoActual ==WIN2){
			estadoActual = MENU;
			repintarFondo = 1;
		}
		// Si se encuentra en la GAMEOVER, y se presiona este boton
		// Regresar al menu
		else if (estadoActual == GAMEOVER){
			estadoActual = MENU;
			repintarFondo = 1;
		}
	}

	// Vuelve a activar la recepción por interrupción
	HAL_UART_Receive_IT(&huart5, buffer, 1);
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
