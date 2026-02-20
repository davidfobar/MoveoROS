/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
#define DRIVER6_STEP_Pin GPIO_PIN_2
#define DRIVER6_STEP_GPIO_Port GPIOE
#define DRIVER6_DIR_Pin GPIO_PIN_3
#define DRIVER6_DIR_GPIO_Port GPIOE
#define DRIVER5_UART_Pin GPIO_PIN_4
#define DRIVER5_UART_GPIO_Port GPIOE
#define DRIVER7_STEP_Pin GPIO_PIN_6
#define DRIVER7_STEP_GPIO_Port GPIOE
#define DRIVER5_STEP_Pin GPIO_PIN_13
#define DRIVER5_STEP_GPIO_Port GPIOC
#define DRIVER5_DIR_Pin GPIO_PIN_0
#define DRIVER5_DIR_GPIO_Port GPIOF
#define DRIVER5_EN_Pin GPIO_PIN_1
#define DRIVER5_EN_GPIO_Port GPIOF
#define DRIVER4_UART_Pin GPIO_PIN_2
#define DRIVER4_UART_GPIO_Port GPIOF
#define DRIVER4_STEP_Pin GPIO_PIN_9
#define DRIVER4_STEP_GPIO_Port GPIOF
#define DRIVER4_DIR_Pin GPIO_PIN_10
#define DRIVER4_DIR_GPIO_Port GPIOF
#define DRIVER3_DIR_Pin GPIO_PIN_1
#define DRIVER3_DIR_GPIO_Port GPIOC
#define DRIVER3_EN_Pin GPIO_PIN_0
#define DRIVER3_EN_GPIO_Port GPIOA
#define DRIVER0_UART_Pin GPIO_PIN_4
#define DRIVER0_UART_GPIO_Port GPIOC
#define DRIVER2_STEP_Pin GPIO_PIN_11
#define DRIVER2_STEP_GPIO_Port GPIOF
#define DRIVER0_DIR_Pin GPIO_PIN_12
#define DRIVER0_DIR_GPIO_Port GPIOF
#define DRIVER0_STEP_Pin GPIO_PIN_13
#define DRIVER0_STEP_GPIO_Port GPIOF
#define DRIVER0_EN_Pin GPIO_PIN_14
#define DRIVER0_EN_GPIO_Port GPIOF
#define DRIVER1_EN_Pin GPIO_PIN_15
#define DRIVER1_EN_GPIO_Port GPIOF
#define DRIVER1_STEP_Pin GPIO_PIN_0
#define DRIVER1_STEP_GPIO_Port GPIOG
#define DRIVER1_DIR_Pin GPIO_PIN_1
#define DRIVER1_DIR_GPIO_Port GPIOG
#define DRIVER1_UART_Pin GPIO_PIN_11
#define DRIVER1_UART_GPIO_Port GPIOD
#define DRIVER4_EN_Pin GPIO_PIN_2
#define DRIVER4_EN_GPIO_Port GPIOG
#define DRIVER2_DIR_Pin GPIO_PIN_3
#define DRIVER2_DIR_GPIO_Port GPIOG
#define DRIVER3_STEP_Pin GPIO_PIN_4
#define DRIVER3_STEP_GPIO_Port GPIOG
#define DRIVER2_EN_Pin GPIO_PIN_5
#define DRIVER2_EN_GPIO_Port GPIOG
#define DRIVER2_UART_Pin GPIO_PIN_6
#define DRIVER2_UART_GPIO_Port GPIOC
#define DRIVER3_UART_Pin GPIO_PIN_7
#define DRIVER3_UART_GPIO_Port GPIOC
#define DRIVER7_UART_Pin GPIO_PIN_3
#define DRIVER7_UART_GPIO_Port GPIOD
#define DRIVER6_EN_Pin GPIO_PIN_4
#define DRIVER6_EN_GPIO_Port GPIOD
#define DRIVER7_EN_Pin GPIO_PIN_0
#define DRIVER7_EN_GPIO_Port GPIOE
#define DRIVER6_UART_Pin GPIO_PIN_1
#define DRIVER6_UART_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
