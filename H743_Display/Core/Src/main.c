/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TFT_SSD1963.h"
#include "TFT_Buttons.h"
#include "Eeprom.h"
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

SPI_HandleTypeDef hspi3;

uint16_t BeepTimer = 0;
uint8_t	TouchCounter = 0;
uint16_t TouchX = 0;
uint16_t TouchY = 0;
uint32_t TouchXadd = 0;
uint32_t TouchYadd = 0;
uint16_t TouchXresult = 0;
uint16_t TouchYresult = 0;
uint8_t TouchResult = 0;
uint8_t TouchPointer = 0;
uint16_t TouchTimer = 0;
uint8_t KeyNumber[6];
uint8_t KeyCounter = 0;

uint8_t Thermo[4];
uint32_t ThermoIn = 0;
uint32_t ThermoOut = 0;

uint8_t BufClock[12];
uint8_t i = 0;
uint8_t ret = 0;
uint16_t Pokus = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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

  TouchCounter = 0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  HAL_SYSTICK_Config( 480000 );
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FMC_Init();
  MX_SPI4_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  LL_SPI_Enable(SPI4);
  LL_SPI_StartMasterTransfer(SPI4);		// for touch pad

  LL_SPI_Enable(SPI1);
  LL_SPI_StartMasterTransfer(SPI1);		// for connector

  TFT_Init();
  TFT_WindowFull( ColRed );
  HAL_Delay( 500 );
  TFT_WindowFull( ColGreen );
  HAL_Delay( 500 );
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
  TFT_WindowFull( ColBlue );
  Beep( 2000, 20);
  HAL_Delay( 500 );
  TFT_WindowFull( ColYellow );
  Beep( 1500, 20);
  HAL_Delay( 500 );
  TFT_WindowFull( ColBlack );
  Beep( 1000, 20);
  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  TFT_WindowFull( ColBlack );
	  TFT_ButtonString("MAX31855", ColWhite, ColBlue, ColBlack,   0, 400);
	  TFT_ButtonString("M95040", ColWhite, ColBlue, ColBlack, 160, 400);
	  TFT_ButtonString("Test 3", ColWhite, ColBlue, ColBlack, 320, 400);
	  TFT_ButtonString("Test 4", ColWhite, ColBlue, ColBlack, 480, 400);
	  TFT_ButtonString("Test 5", ColWhite, ColBlue, ColBlack, 640, 400);
	  TFT_ButtonClock(1, ColYellow, ColBlue, ColNavy, 4, 4);

	  while (1) {
		  if( TFT_ButtonStringCheck( 0, 400 ) == 1 ) {
				BufClock[0] = 0;
				BufClock[1] = 0;
				BufClock[2] = 0;
				BufClock[4] = 0;
			  MENU_Test1();
			  break;
		  }
		  if( TFT_ButtonStringCheck( 160, 400 ) == 1 ) {
			  MENU_Test2();
			  break;
		  }
		  if( TFT_ButtonStringCheck( 320, 400 ) == 1 ) {
			  MENU_Test3();
			  break;
		  }
		  if( TFT_ButtonStringCheck( 480, 400 ) == 1 ) {
			  MENU_Test4();
			  break;
		  }
		  if( TFT_ButtonStringCheck( 640, 400 ) == 1 ) {
			  MENU_Test5();
			  break;
		  }

		  TFT_ButtonClock(0, ColYellow, ColBlue, ColNavy, 4, 4);				// napise kolik je hodin
		  HAL_Delay(100);
		  if (TFT_ButtonClockCheck(4, 4) == 1) {
			  TFT_KeypadNumber("Year", ColCyan, ColRed, ColDarkCyan, 184, 4);
			  BufClock[7] = ClockNumer( TFT_KeypadNumberCheck(2, ColCyan, ColRed, ColDarkCyan, 184, 4));

			  TFT_KeypadNumber("Month", ColCyan, ColRed, ColDarkCyan, 184, 4);
			  BufClock[6] = ClockNumer( TFT_KeypadNumberCheck(2, ColCyan, ColRed, ColDarkCyan, 184, 4));

			  TFT_KeypadNumber("Date", ColCyan, ColRed, ColDarkCyan, 184, 4);
			  BufClock[5] = ClockNumer( TFT_KeypadNumberCheck(2, ColCyan, ColRed, ColDarkCyan, 184, 4));

			  TFT_KeypadNumber("Day", ColCyan, ColRed, ColDarkCyan, 184, 4);
			  BufClock[4] = ClockNumer( TFT_KeypadNumberCheck(2, ColCyan, ColRed, ColDarkCyan, 184, 4));

			  TFT_KeypadNumber("Hours", ColCyan, ColRed, ColDarkCyan, 184, 4);
			  BufClock[3] = ClockNumer( TFT_KeypadNumberCheck(2, ColCyan, ColRed, ColDarkCyan, 184, 4));

			  TFT_KeypadNumber("Minutes", ColCyan, ColRed, ColDarkCyan, 184, 4);
			  BufClock[2] = ClockNumer( TFT_KeypadNumberCheck(2, ColCyan, ColRed, ColDarkCyan, 184, 4));

			  TFT_KeypadNumber("Seconds", ColCyan, ColRed, ColDarkCyan, 184, 4);
			  BufClock[1] = ClockNumer( TFT_KeypadNumberCheck(2, ColCyan, ColRed, ColDarkCyan, 184, 4));

			  BufClock[0] = 0x00;
			  HAL_I2C_Master_Transmit(&hi2c1, 0xD0, BufClock, 7, HAL_MAX_DELAY);
			  break;
		  }

	//	  Thermo = LL_SPI_ReceiveData32(SPI2);
		  HAL_Delay(100);

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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_4)
  {
  }
  LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE0);
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1Q_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(1);
  LL_RCC_PLL1_SetN(120);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(5);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

   /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
   LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {

  }
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_2);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);
  LL_SetSystemCoreClock(480000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void MENU_Test1( void ) {						// MAX31855 temperature sensor reading
	TFT_WindowFull( ColDarkGreen );
	TFT_ButtonString("M95040", ColWhite, ColBlue, ColBlack,   0, 400);
	TFT_ButtonString("Test 3", ColWhite, ColBlue, ColBlack, 160, 400);
	TFT_ButtonString("Test 4", ColWhite, ColBlue, ColBlack, 320, 400);
	TFT_ButtonString("Test 5", ColWhite, ColBlue, ColBlack, 480, 400);
	TFT_ButtonString("EXIT", ColWhite, ColBlue, ColBlack, 640, 400);
	while(1) {

		HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Receive(&hspi2, Thermo, 4, 1000);
		HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);
		HAL_Delay(10);

		TFT_PrintString(0, "MAX31855 data array", ColRed, ColDarkGreen, 0, 10);
		TFT_PrintNumber(0, Thermo[0], 3, 0, ColYellow, ColDarkGreen, 200, 10);
		TFT_PrintNumber(0, Thermo[1], 3, 0, ColYellow, ColDarkGreen, 280, 10);
		TFT_PrintNumber(0, Thermo[2], 3, 0, ColYellow, ColDarkGreen, 350, 10);
		TFT_PrintNumber(0, Thermo[3], 3, 0, ColYellow, ColDarkGreen, 420, 10);

		ThermoIn = (Thermo[3] >> 4) | (Thermo[2] << 4);
		ThermoIn = ThermoIn * 625;

		ThermoOut = (Thermo[1] >> 2) | (Thermo[0] << 6);
		ThermoOut &= 0x03FFF;
		ThermoOut = ThermoOut * 25;

		TFT_PrintString(2, "Error", ColYellow, ColDarkGreen, 10, 60);
		TFT_PrintNumber(2, (Thermo[3] & 0x0F) , 2, 0, ColYellow, ColDarkGreen, 200, 60);
		TFT_PrintString(2, "ThermoIn", ColYellow, ColDarkGreen, 10, 120);
		TFT_PrintNumber(2, ThermoIn, 10, 4, ColYellow, ColDarkGreen, 200, 120);
		TFT_PrintString(2, "ThermoOut", ColYellow, ColDarkGreen, 10, 180);
		TFT_PrintNumber(2, ThermoOut, 10, 2, ColYellow, ColDarkGreen, 200, 180);

		if( TFT_ButtonStringCheck( 0, 400 ) == 1 ) {
			MENU_Test2();
			break;
		}
		if( TFT_ButtonStringCheck( 160, 400 ) == 1 ) {
			MENU_Test3();
			break;
		}
		if( TFT_ButtonStringCheck( 320, 400 ) == 1 ) {
		  	MENU_Test4();
		  	break;
		}
		if( TFT_ButtonStringCheck( 480, 400 ) == 1 ) {
			MENU_Test5();
			break;
		}
		if( TFT_ButtonStringCheck( 640, 400 ) == 1 ) break;
	}
}

void MENU_Test2( void ) {							// EEPROM M95040 - testing
	TFT_WindowFull( ColDarkBlue );
	TFT_ButtonString("MAX31855", ColWhite, ColBlue, ColBlack,   0, 400);
	TFT_ButtonString("Test 3", ColWhite, ColBlue, ColBlack, 160, 400);
	TFT_ButtonString("Test 4", ColWhite, ColBlue, ColBlack, 320, 400);
	TFT_ButtonString("Test 5", ColWhite, ColBlue, ColBlack, 480, 400);
	TFT_ButtonString("EXIT", ColWhite, ColBlue, ColBlack, 640, 400);

	TFT_ButtonString("Write 1", ColWhite, ColGreen, ColDarkBlue, 0, 0);
	TFT_ButtonString("Write 2", ColWhite, ColGreen, ColDarkBlue, 160, 0);
	TFT_ButtonString("Write 3", ColWhite, ColGreen, ColDarkBlue, 320, 0);
	TFT_ButtonString("Write 4", ColWhite, ColGreen, ColDarkBlue, 480, 0);
	TFT_ButtonString("Read", ColWhite, ColGreen, ColDarkBlue, 640, 0);



	while(1) {

		if( TFT_ButtonStringCheck( 0, 0 ) == 1 ) {
			BufClock[0] = 10;
			BufClock[1] = 120;
			BufClock[2] = 235;
			BufClock[3] = 88;
			BufClock[4] = 200;
			BufClock[5] = 114;
			BufClock[6] = 30;
			BufClock[7] = 255;
			BufClock[8] = 78;
			PrintArray(8, 0, 120 );
			EEPROM_SPI_INIT(&hspi3);

			EEPROM_SPI_WriteBuffer(BufClock, (uint16_t)0x01, (uint16_t)9 );

			Pokus = BufClock[1] * 256 + BufClock[0];
			TFT_PrintNumber(0, Pokus, 5, 0, ColYellow, ColDarkBlue, 0, 200);

			BufClock[0] = (uint8_t) Pokus >> 8;
			BufClock[1] = (uint8_t) Pokus;

			Pokus = BufClock[2] * 256 + BufClock[3];
			TFT_PrintNumber(0, Pokus, 5, 0, ColYellow, ColDarkBlue, 100, 200);

			BufClock[3] = (uint8_t) Pokus >> 8;
			BufClock[2] = (uint8_t) (Pokus);

			Pokus = BufClock[5] * 256 + BufClock[4];
			TFT_PrintNumber(0, Pokus, 5, 0, ColYellow, ColDarkBlue, 200, 200);

			BufClock[5] = Pokus >> 8;
			BufClock[4] = (uint8_t) Pokus;

			Pokus = BufClock[7] * 256 + BufClock[8];
			TFT_PrintNumber(0, Pokus, 5, 0, ColYellow, ColDarkBlue, 400, 200);

			BufClock[7] = Pokus >> 8;
			BufClock[8] = (uint8_t) (Pokus);

			PrintArray(8, 0, 240 );

		}
		if( TFT_ButtonStringCheck( 160, 0 ) == 1 ) {
			BufClock[0] = 0;
			BufClock[1] = 0;
			BufClock[2] = 0;
			BufClock[3] = 0;
			BufClock[4] = 0;
			BufClock[5] = 0;
			BufClock[6] = 0;
			BufClock[7] = 0;
			BufClock[8] = 0;
			PrintArray(8, 0, 120 );
			EEPROM_SPI_INIT(&hspi3);

			EEPROM_SPI_WriteBuffer(BufClock, (uint16_t)0x01, (uint16_t)9 );
		}
		if( TFT_ButtonStringCheck( 320, 0 ) == 1 ) {
			BufClock[0] = 100;
			BufClock[1] = 90;
			BufClock[2] = 80;
			BufClock[3] = 70;
			BufClock[4] = 60;
			BufClock[5] = 50;
			BufClock[6] = 40;
			BufClock[7] = 30;
			BufClock[8] = 20;
			PrintArray(8, 0, 120 );
			EEPROM_SPI_INIT(&hspi3);

			EEPROM_SPI_WriteBuffer(BufClock, (uint16_t)0x01, (uint16_t)9 );
		}
		if( TFT_ButtonStringCheck( 480, 0 ) == 1 ) {
			BufClock[0] = 55;
			BufClock[1] = 75;
			BufClock[2] = 95;
			BufClock[3] = 115;
			BufClock[4] = 135;
			BufClock[5] = 155;
			BufClock[6] = 175;
			BufClock[7] = 195;
			BufClock[8] = 215;
			PrintArray(8, 0, 120 );
			EEPROM_SPI_INIT(&hspi3);

			EEPROM_SPI_WriteBuffer(BufClock, (uint16_t)0x01, (uint16_t)9 );
		}
		if( TFT_ButtonStringCheck( 640, 0 ) == 1 ) {
			EEPROM_SPI_INIT(&hspi3);
			EEPROM_SPI_ReadBuffer(BufClock, (uint16_t)0x01, (uint16_t)9 );
			PrintArray(8, 0, 150 );
		}

		if( TFT_ButtonStringCheck( 0, 400 ) == 1 ) {
			MENU_Test1();
			break;
		}
		if( TFT_ButtonStringCheck( 160, 400 ) == 1 ) {
			MENU_Test3();
			break;
		}
		if( TFT_ButtonStringCheck( 320, 400 ) == 1 ) {
		  	MENU_Test4();
		  	break;
		}
		if( TFT_ButtonStringCheck( 480, 400 ) == 1 ) {
			MENU_Test5();
			break;
		}
		if( TFT_ButtonStringCheck( 640, 400 ) == 1 ) break;
	}
}

void MENU_Test3( void ) {
	TFT_WindowFull( ColDarkGrey );
	TFT_ButtonString("MAX31855", ColWhite, ColBlue, ColBlack,   0, 400);
	TFT_ButtonString("M95040", ColWhite, ColBlue, ColBlack, 160, 400);
	TFT_ButtonString("Test 4", ColWhite, ColBlue, ColBlack, 320, 400);
	TFT_ButtonString("Test 5", ColWhite, ColBlue, ColBlack, 480, 400);
	TFT_ButtonString("EXIT", ColWhite, ColBlue, ColBlack, 640, 400);
	while(1) {
		if( TFT_ButtonStringCheck( 0, 400 ) == 1 ) {
			MENU_Test1();
			break;
		}
		if( TFT_ButtonStringCheck( 160, 400 ) == 1 ) {
			MENU_Test2();
			break;
		}
		if( TFT_ButtonStringCheck( 320, 400 ) == 1 ) {
		  	MENU_Test4();
		  	break;
		}
		if( TFT_ButtonStringCheck( 480, 400 ) == 1 ) {
			MENU_Test5();
			break;
		}
		if( TFT_ButtonStringCheck( 640, 400 ) == 1 ) break;
	}
}

void MENU_Test4( void ) {
	TFT_WindowFull( ColDarkCyan );
	TFT_ButtonString("MAX31855", ColWhite, ColBlue, ColBlack,   0, 400);
	TFT_ButtonString("M95040", ColWhite, ColBlue, ColBlack, 160, 400);
	TFT_ButtonString("Test 3", ColWhite, ColBlue, ColBlack, 320, 400);
	TFT_ButtonString("Test 5", ColWhite, ColBlue, ColBlack, 480, 400);
	TFT_ButtonString("EXIT", ColWhite, ColBlue, ColBlack, 640, 400);
	while(1) {
		if( TFT_ButtonStringCheck( 0, 400 ) == 1 ) {
			MENU_Test1();
			break;
		}
		if( TFT_ButtonStringCheck( 160, 400 ) == 1 ) {
			MENU_Test2();
			break;
		}
		if( TFT_ButtonStringCheck( 320, 400 ) == 1 ) {
		  	MENU_Test3();
		  	break;
		}
		if( TFT_ButtonStringCheck( 480, 400 ) == 1 ) {
			MENU_Test5();
			break;
		}
		if( TFT_ButtonStringCheck( 640, 400 ) == 1 ) break;
	}
}

void MENU_Test5( void ) {
	TFT_WindowFull( ColDarkRed );
	TFT_ButtonString("MAX31855", ColWhite, ColBlue, ColBlack,   0, 400);
	TFT_ButtonString("M95040", ColWhite, ColBlue, ColBlack, 160, 400);
	TFT_ButtonString("Test 3", ColWhite, ColBlue, ColBlack, 320, 400);
	TFT_ButtonString("Test 4", ColWhite, ColBlue, ColBlack, 480, 400);
	TFT_ButtonString("EXIT", ColWhite, ColBlue, ColBlack, 640, 400);
	while(1) {
		if( TFT_ButtonStringCheck( 0, 400 ) == 1 ) {
			MENU_Test1();
		}
		if( TFT_ButtonStringCheck( 160, 400 ) == 1 ) {
			MENU_Test2();
		}
		if( TFT_ButtonStringCheck( 320, 400 ) == 1 ) {
		  	MENU_Test3();
		}
		if( TFT_ButtonStringCheck( 480, 400 ) == 1 ) {
			MENU_Test4();
		}
		if( TFT_ButtonStringCheck( 640, 400 ) == 1 ) break;
	}
}

void Beep( uint32_t Tone, uint16_t Time) {
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  TIM_InitStruct.Prescaler = 480;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH4);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_ENABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH4, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM2, LL_TIM_CHANNEL_CH4);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);

  Tone = 1000000 / Tone;
  if(!LL_TIM_IsEnabledCounter(TIM2)) {
	LL_TIM_EnableCounter(TIM2);
	LL_TIM_EnableAllOutputs(TIM2);
  }
  LL_TIM_SetAutoReload(TIM2, Tone);
  Tone = Tone >> 1;
  LL_TIM_OC_SetCompareCH4(TIM2, Tone);
  BeepTimer = Time;
}

void PrintArray( uint8_t imax, uint16_t x, uint16_t y) {
	uint8_t i;
	for( i = 0; i < imax; i++) {
		TFT_PrintNumber(0, BufClock[i], 3, 0, ColYellow, ColDarkBlue, x + i * 80, y);
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
