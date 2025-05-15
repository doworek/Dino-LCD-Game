/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define Led_Pin GPIO_PIN_13
#define Led_GPIO_Port GPIOC
#define Button_left_Pin GPIO_PIN_0
#define Button_left_GPIO_Port GPIOA
#define Button_left_EXTI_IRQn EXTI0_IRQn
#define Button_jump_Pin GPIO_PIN_1
#define Button_jump_GPIO_Port GPIOA
#define Button_jump_EXTI_IRQn EXTI1_IRQn
#define Button_right_Pin GPIO_PIN_2
#define Button_right_GPIO_Port GPIOA
#define Button_right_EXTI_IRQn EXTI2_IRQn

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
