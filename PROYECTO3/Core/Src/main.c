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
#include "bitmaps.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ESP32_ADDRESS 0x10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

uint8_t TX_Buffer [] = "S";
uint8_t RX_Buffer [1];

uint8_t option[1];
uint8_t option1[1];
uint8_t option2[1];
uint8_t tx_buffer[20];

uint8_t carroD1 = 0;
uint8_t carroD2 = 0;
uint8_t carroD3 = 0;
uint8_t carroD4 = 0;
uint8_t carroD5 = 0;
uint8_t carroD6 = 0;
uint8_t carroD7 = 0;
uint8_t carroD8 = 0;

uint8_t parqueo5 = 0;
uint8_t parqueo6 = 0;
uint8_t parqueo7 = 0;
uint8_t parqueo8 = 0;

int parqueos = 8;
uint8_t change = 0;

int y1 = 35;
int y2 = 35;
int y3 = 35;
int y4 = 35;
int y5 = 160;
int y6 = 160;
int y7 = 160;
int y8 = 160;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_UART5_Init(void);
/* USER CODE BEGIN PFP */
void send_i2c_command(char* command);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void transmit_uart(char *message);
void transmit_uart3(char *message);
void transmit_uart5(char *message);
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Receive_IT(&huart2, option, 1);  // EMPEZAR RECEPCIÓN
  HAL_UART_Receive_IT(&huart3, option, 1);  // EMPEZAR RECEPCIÓN
  HAL_UART_Receive_IT(&huart5, option2, 1);
	LCD_Init();

	LCD_Clear(0x736E);
	//FillRect(0, 0, 319, 239, 0xDD47);
	FillRect(93, 19, 203, 6, 0xDD47);
	FillRect(90, 19, 6, 57, 0xDD47);
	FillRect(140, 19, 6, 57, 0xDD47);
	FillRect(190, 19, 6, 57, 0xDD47);
	FillRect(240, 19, 6, 57, 0xDD47);
	FillRect(290, 19, 6, 57, 0xDD47);

	FillRect(93, 221, 203, 6, 0xDD47);
	FillRect(90, 170, 6, 57, 0xDD47);
	FillRect(140, 170, 6, 57, 0xDD47);
	FillRect(190, 170, 6, 57, 0xDD47);
	FillRect(240, 170, 6, 57, 0xDD47);
	FillRect(290, 170, 6, 57, 0xDD47);

	LCD_Sprite(20, 107, 30, 33, numbers, 10, 8, 0, 0);

	//SEMAFOROS

	LCD_Sprite(101, 85, 35, 18, semaforo, 2, 0, 0, 0);	//SEMAFORO1
	LCD_Sprite(151, 85, 35, 18, semaforo, 2, 0, 0, 0);	//SEMAFORO2
	LCD_Sprite(201, 85, 35, 18, semaforo, 2, 0, 0, 0);	//SEMAFORO3
	LCD_Sprite(251, 85, 35, 18, semaforo, 2, 0, 0, 0);	//SEMAFORO4

	LCD_Sprite(101, 140, 35, 18, semaforo, 2, 0, 0, 0);	//SEMAFORO5
	LCD_Sprite(151, 140, 35, 18, semaforo, 2, 0, 0, 0);	//SEMAFORO6
	LCD_Sprite(201, 140, 35, 18, semaforo, 2, 0, 0, 0);	//SEMAFORO7
	LCD_Sprite(251, 140, 35, 18, semaforo, 2, 0, 0, 0);	//SEMAFORO8

		//LCD_Bitmap(0, 0, 320, 240, fondo);
	 //FillRect(0, 0, 319, 206, 0x1911);

	  //LCD_Print("Hola Mundo", 20, 100, 1, 0x001F, 0xCAB9);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {

		//CARRO 1
		if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == GPIO_PIN_RESET){
			if (carroD1 == 0){
				LCD_Sprite(100, y1, 36, 48, cars, 8, 0, 0, 0);
				if (y1 >= 26){
					y1--;
				}
				if (y1 == 25){
					carroD1 = 1;
					parqueos--;
					change = 1;
					LCD_Sprite(101, 85, 35, 18, semaforo, 2, 1, 0, 0);
					HAL_UART_Transmit(&huart5, "a", 1, 10);
					send_i2c_command("1");
				}
			}

		}else{
			if (carroD1 == 1){
				FillRect(100, 25, 36, 50, 0x736E);
				carroD1 = 0;
				parqueos++;
				change = 1;
				LCD_Sprite(101, 85, 35, 18, semaforo, 2, 0, 0, 0);
				HAL_UART_Transmit(&huart5, "i", 1, 10);
				send_i2c_command("a");
			}
			y1 = 35;
		}

		//CARRO 2
		if (HAL_GPIO_ReadPin(BUTTON2_GPIO_Port, BUTTON2_Pin) == GPIO_PIN_RESET){
			if (carroD2 == 0){
				LCD_Sprite(150, y2, 36, 48, cars, 8, 1, 0, 0);
				if (y2 >= 26){
					y2--;
				}
				if (y2 == 25){
					carroD2 = 1;
					parqueos--;
					change = 1;
					LCD_Sprite(151, 85, 35, 18, semaforo, 2, 1, 0, 0);
					HAL_UART_Transmit(&huart5, "b", 1, 10);
					send_i2c_command("2");
				}
			}
		}else{
			if (carroD2 == 1){
				FillRect(150, 25, 36, 50, 0x736E);
				carroD2 = 0;
				parqueos++;
				change = 1;
				LCD_Sprite(151, 85, 35, 18, semaforo, 2, 0, 0, 0);
				HAL_UART_Transmit(&huart5, "j", 1, 10);
				send_i2c_command("b");
			}
			y2 = 35;
		}

		//CARRO 3
		if (HAL_GPIO_ReadPin(BUTTON3_GPIO_Port, BUTTON3_Pin) == GPIO_PIN_RESET){
			if (carroD3 == 0){
				LCD_Sprite(200, y3, 36, 48, cars, 8, 2, 0, 0);
				if (y3 >= 26){
					y3--;
				}
				if (y3 == 25){
					carroD3 = 1;
					parqueos--;
					change = 1;
					LCD_Sprite(201, 85, 35, 18, semaforo, 2, 1, 0, 0);
					HAL_UART_Transmit(&huart5, "c", 1, 10);
					send_i2c_command("3");
				}
			}
		}else{
			if (carroD3 == 1){
				FillRect(200, 25, 36, 50, 0x736E);
				carroD3 = 0;
				parqueos++;
				change = 1;
				LCD_Sprite(201, 85, 35, 18, semaforo, 2, 0, 0, 0);
				HAL_UART_Transmit(&huart5, "k", 1, 10);
				send_i2c_command("c");
			}
			y3 = 35;
		}

		//CARRO 4
		if (HAL_GPIO_ReadPin(BUTTON4_GPIO_Port, BUTTON4_Pin) == GPIO_PIN_RESET){
			if (carroD4 == 0){
				LCD_Sprite(250, y4, 36, 48, cars, 8, 3, 0, 0);
				if (y4 >= 26){
					y4--;
				}
				if (y4 == 25){
					carroD4 = 1;
					parqueos--;
					change = 1;
					LCD_Sprite(251, 85, 35, 18, semaforo, 2, 1, 0, 0);
					HAL_UART_Transmit(&huart5, "d", 1, 10);
					send_i2c_command("4");
				}
			}
		}else{
			if (carroD4 == 1){
				FillRect(250, 25, 36, 50, 0x736E);
				carroD4 = 0;
				parqueos++;
				change = 1;
				LCD_Sprite(251, 85, 35, 18, semaforo, 2, 0, 0, 0);
				HAL_UART_Transmit(&huart5, "l", 1, 10);
				send_i2c_command("d");
			}
			y4 = 35;
		}

		//CARRO 5
		if (parqueo5 == 1){	//SI SE RECIBE UN DATO PROVENIENTE DE LA UART
			if (carroD5 == 0){
				LCD_Sprite(100, y5, 36, 48, cars, 8, 4, 0, 0);
				if (y5 <= 169){
					y5++;
				}
				if (y5 == 170){
					carroD5 = 1;
					parqueos--;
					change = 1;
					LCD_Sprite(101, 140, 35, 18, semaforo, 2, 1, 0, 0);
					HAL_UART_Transmit(&huart5, "e", 1, 10);
					send_i2c_command("5");
					//parqueo5 = 0;
				}
			}
		}else if (parqueo5 == 0){
			if (carroD5 == 1){
				FillRect(100, 170, 36, 50, 0x736E);
				carroD5 = 0;
				parqueos++;
				change = 1;
				LCD_Sprite(101, 140, 35, 18, semaforo, 2, 0, 0, 0);
				HAL_UART_Transmit(&huart5, "m", 1, 10);
				send_i2c_command("e");
				//parqueo5 = 0;
			}
			y5 = 160;
		}

		//CARRO 6
		if (parqueo6 == 1){	//SI SE RECIBE UN DATO PROVENIENTE DE LA UART
			if (carroD6 == 0){
				LCD_Sprite(150, y6, 36, 48, cars, 8, 5, 0, 0);
				if (y6 <= 169){
					y6++;
				}
				if (y6 == 170){
					carroD6 = 1;
					parqueos--;
					change = 1;
					LCD_Sprite(151, 140, 35, 18, semaforo, 2, 1, 0, 0);
					HAL_UART_Transmit(&huart5, "f", 1, 10);
					send_i2c_command("6");
					//parqueo5 = 0;
				}
			}
		}else if (parqueo6 == 0){
			if (carroD6 == 1){
				FillRect(150, 170, 36, 50, 0x736E);
				carroD6 = 0;
				parqueos++;
				change = 1;
				LCD_Sprite(151, 140, 35, 18, semaforo, 2, 0, 0, 0);
				HAL_UART_Transmit(&huart5, "n", 1, 10);
				send_i2c_command("f");
				//parqueo5 = 0;
			}
			y6 = 160;
		}
		//CARRO 7
		if (parqueo7 == 1){	//SI SE RECIBE UN DATO PROVENIENTE DE LA UART
			if (carroD7 == 0){
				LCD_Sprite(200, y7, 36, 48, cars, 8, 6, 0, 0);
				if (y7 <= 169){
					y7++;
				}
				if (y7 == 170){
					carroD7 = 1;
					parqueos--;
					change = 1;
					LCD_Sprite(201, 140, 35, 18, semaforo, 2, 1, 0, 0);
					HAL_UART_Transmit(&huart5, "g", 1, 10);
					send_i2c_command("7");
					//parqueo5 = 0;
				}
			}
		}else if (parqueo7 == 0){
			if (carroD7 == 1){
				FillRect(200, 170, 36, 50, 0x736E);
				carroD7 = 0;
				parqueos++;
				change = 1;
				LCD_Sprite(201, 140, 35, 18, semaforo, 2, 0, 0, 0);
				HAL_UART_Transmit(&huart5, "o", 1, 10);
				send_i2c_command("g");
				//parqueo5 = 0;
			}
			y7 = 160;
		}
		//CARRO 8
		if (parqueo8 == 1){	//SI SE RECIBE UN DATO PROVENIENTE DE LA UART
			if (carroD8 == 0){
				LCD_Sprite(250, y8, 36, 48, cars, 8, 7, 0, 0);
				if (y8 <= 169){
					y8++;
				}
				if (y8 == 170){
					carroD8 = 1;
					parqueos--;
					change = 1;
					LCD_Sprite(251, 140, 35, 18, semaforo, 2, 1, 0, 0);
					HAL_UART_Transmit(&huart5, "h", 1, 10);
					send_i2c_command("8");
					//parqueo5 = 0;
				}
			}
		}else if (parqueo8 == 0){
			if (carroD8 == 1){
				FillRect(250, 170, 36, 50, 0x736E);
				carroD8 = 0;
				parqueos++;
				change = 1;
				LCD_Sprite(251, 140, 35, 18, semaforo, 2, 0, 0, 0);
				HAL_UART_Transmit(&huart5, "p", 1, 10);
				send_i2c_command("h");
				//parqueo5 = 0;
			}
			y8 = 160;
		}

		//NUMEROS
		if (parqueos == 8 && change == 1){
			LCD_Sprite(20, 107, 30, 33, numbers, 10, 8, 0, 0);
			change = 0;
		}else if (parqueos == 7 && change == 1){
			LCD_Sprite(20, 107, 30, 33, numbers, 10, 7, 0, 0);
			change = 0;
		}else if (parqueos == 6 && change == 1){
			LCD_Sprite(20, 107, 30, 33, numbers, 10, 6, 0, 0);
			change = 0;
		}else if (parqueos == 5 && change == 1){
			LCD_Sprite(20, 107, 30, 33, numbers, 10, 5, 0, 0);
			change = 0;
		}else if (parqueos == 4 && change == 1){
			LCD_Sprite(20, 107, 30, 33, numbers, 10, 4, 0, 0);
			change = 0;
		}else if (parqueos == 3 && change == 1){
			LCD_Sprite(20, 107, 30, 33, numbers, 10, 3, 0, 0);
			change = 0;
		}else if (parqueos == 2 && change == 1){
			LCD_Sprite(20, 107, 30, 33, numbers, 10, 2, 0, 0);
			change = 0;
		}else if (parqueos == 1 && change == 1){
			LCD_Sprite(20, 107, 30, 33, numbers, 10, 1, 0, 0);
			change = 0;
		}else if (parqueos == 0 && change == 1){
			LCD_Sprite(20, 107, 30, 33, numbers, 10, 0, 0, 0);
			change = 0;
		}

		/*TX_Buffer[0] = '0';
		HAL_I2C_Master_Transmit(&hi2c1, (deviceAddress << 1)|0x00, TX_Buffer, 1, HAL_MAX_DELAY);
		HAL_Delay(1000);
		TX_Buffer[0] = 'S';
		HAL_I2C_Master_Transmit(&hi2c1, (deviceAddress << 1), TX_Buffer, 1, HAL_MAX_DELAY);
		HAL_Delay(1000);

		HAL_I2C_Master_Receive(&hi2c1, (deviceAddress << 1)|0x01, (uint8_t *)RX_Buffer, 1, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, RX_Buffer, 1, 1000);*/

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
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
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
  huart5.Init.BaudRate = 9600;
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
  huart2.Init.BaudRate = 9600;
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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
                          |LCD_D4_Pin|SD_SS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BUTTON2_Pin BUTTON3_Pin BUTTON4_Pin BUTTON1_Pin */
  GPIO_InitStruct.Pin = BUTTON2_Pin|BUTTON3_Pin|BUTTON4_Pin|BUTTON1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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

// Función para enviar un comando I2C
void send_i2c_command(char* command) {
    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(ESP32_ADDRESS << 1), (uint8_t*)command, strlen(command), HAL_MAX_DELAY);
}

void transmit_uart2(char *message) {
   HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
}
void transmit_uart3(char *message) {
   HAL_UART_Transmit(&huart3, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
}
void transmit_uart5(char *message) {
   HAL_UART_Transmit(&huart5, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART2){
		switch (option1[0]){
		case 'e':
			parqueo5 = 1;
			break;
		case 'f':
			parqueo6 = 1;
			break;
		case 'g':
			parqueo7 = 1;
			break;
		case 'h':
			parqueo8 = 1;
			break;
		default:
			break;
		}
		HAL_UART_Receive_IT(&huart2, option1, 1);  // EMPEZAR RECEPCIÓN
	}
	if(huart->Instance == USART3){
		switch (option1[0]){
			case 'e':
				parqueo5 = 1;
				break;
			case 'm':
				parqueo5 = 0;
				break;
			case 'f':
				parqueo6 = 1;
				break;
			case 'n':
				parqueo6 = 0;
				break;
			case 'g':
				parqueo7 = 1;
				break;
			case 'o':
				parqueo7 = 0;
				break;
			case 'h':
				parqueo8 = 1;
				break;
			case 'p':
				parqueo8 = 0;
				break;
			default:
				break;
			}
			HAL_UART_Receive_IT(&huart3, option1, 1);  // EMPEZAR RECEPCIÓN
		}
	if(huart->Instance == UART5){
			switch (option2[0]){
			case 'e':
				strcpy(tx_buffer, "e");
				break;
			case 'f':
				strcpy(tx_buffer, "f");
				break;
			case 'g':
				strcpy(tx_buffer, "g");
				break;
			case 'h':
				strcpy(tx_buffer, "h");
				break;
			default:
				break;
			}
			HAL_UART_Receive_IT(&huart5, option2, 1);  // EMPEZAR RECEPCIÓN
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
