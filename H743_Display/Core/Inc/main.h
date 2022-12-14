/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_crs.h"
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_system.h"
#include "stm32h7xx_ll_exti.h"
#include "stm32h7xx_ll_cortex.h"
#include "stm32h7xx_ll_utils.h"
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_spi.h"
#include "stm32h7xx_ll_tim.h"
#include "stm32h7xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define TouchError 100

extern uint16_t BeepTimer;
extern uint8_t	TouchCounter;
extern uint16_t TouchX;
extern uint16_t TouchY;
extern uint32_t TouchXadd;
extern uint32_t TouchYadd;
extern uint16_t TouchXresult;
extern uint16_t TouchYresult;
extern uint8_t TouchResult;
extern uint8_t TouchPointer;
extern uint16_t TouchTimer;
extern uint8_t KeyNumber[6];
extern uint8_t KeyCounter;

extern uint8_t Thermo[4];
extern uint32_t ThermoIn;
extern uint32_t ThermoOut;

extern uint8_t BufClock[12];
extern uint8_t i;
extern uint8_t ret;
extern uint16_t Pokus;

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
void MENU_Test1( void );
void MENU_Test2( void );
void MENU_Test3( void );
void MENU_Test4( void );
void MENU_Test5( void );

void Beep( uint32_t Tone, uint16_t Time);
void PrintArray( uint8_t imax, uint16_t x, uint16_t y);

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TFT_LED_Pin GPIO_PIN_3
#define TFT_LED_GPIO_Port GPIOE
#define SPI4_NSS_Pin GPIO_PIN_4
#define SPI4_NSS_GPIO_Port GPIOE
#define SPI3_H_Pin GPIO_PIN_13
#define SPI3_H_GPIO_Port GPIOC
#define SPI2_NSS_Pin GPIO_PIN_12
#define SPI2_NSS_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_10
#define LED3_GPIO_Port GPIOA
#define SPI3_NSS_Pin GPIO_PIN_15
#define SPI3_NSS_GPIO_Port GPIOA
#define SPI3_W_Pin GPIO_PIN_12
#define SPI3_W_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_6
#define LED4_GPIO_Port GPIOB
#define FMC_RES_Pin GPIO_PIN_9
#define FMC_RES_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
