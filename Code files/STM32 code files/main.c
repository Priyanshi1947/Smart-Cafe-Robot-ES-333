/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Complete Line Follower with Table Selection & PWM Motors
******************************************************************************
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdlib.h> // Required for abs()
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
#define MAX_SPEED 800
#define SLOW_SPEED 600
#define CROSS_DELAY 400
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN 0 */
/**
*Controls L298N Motors.
* Left: PA4/PA5 (Dir), PA0 (PWM) | Right: PB8/PB9 (Dir), PA3 (PWM)
*/
void set_motors(int left_speed, int right_speed) {
  // Left Motor (PA4, PA5, PWM on TIM2 CH1/PA0)
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (left_speed >= 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, (left_speed >= 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
  // Right Motor (PB8, PB9, PWM on TIM2 CH4/PA3)
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, (right_speed >= 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, (right_speed >= 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, abs(left_speed));
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, abs(right_speed));
}
/* USER CODE END 0 */
/**
* @brief  The application entry point.
*/
int main(void)
{
	HAL_Init();
SystemClock_Config();
MX_GPIO_Init();
MX_TIM2_Init();
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
uint8_t target_table = 0;
uint8_t intersection_count = 0;
uint8_t mission_active =0;
uint8_t on_intersection = 0;
/* USER CODE END 2 */
while (1)
{
    // 1. EMERGENCY STOP (Highest Priority)
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == GPIO_PIN_SET)
    {
        set_motors(0, 0);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        HAL_Delay(200);
        continue;
    }
    // STARTING-pick the table
    if (mission_active == 0)
    {
        set_motors(0, 0);
        GPIO_PinState b0 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
        GPIO_PinState b1 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8);
        GPIO_PinState b2 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9);
        target_table = (b2 << 2) | (b1 << 1) | b0;
        // Display target on LEDs in Binary
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, b0);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, b1);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, b2);
        if (target_table > 0) {
            HAL_Delay(500); // Small debounce
            mission_active = 1;
            intersection_count = 0;
        }
        continue;
    }
    // IR SENSOR READING
    uint8_t R = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);//Right
    uint8_t C = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2);//Center
    uint8_t L = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3);//Left
    // INTERSECTION & PC11 LOGIC
    if (L && C && R)
    {
        if (!on_intersection)
        {
            intersection_count++;
            on_intersection = 1;
            // Only trigger STOP if we haven't reached the target yet

            if (target_table != 0 && intersection_count == target_table)
            {
                set_motors(0, 0); // STOP
                //INTERRUPT: WAIT FOR PC11 PUSH BUTTON For receiver OK confirmation
                while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == GPIO_PIN_RESET)
                {
                    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_14);
                    HAL_Delay(100); // Visual: Waiting for "OK"

                    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == GPIO_PIN_SET) break;
                }
                // RETURN
                // Set target to 0 so future intersections are ignored
                target_table = 0;
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_14, GPIO_PIN_RESET);
                HAL_Delay(1000); // 1s buffer
            }
            // DRIVE PAST INTERSECTION
            set_motors(MAX_SPEED, MAX_SPEED);
            HAL_Delay(CROSS_DELAY);
        }
    }
    else if (!L && !R)
      {
        on_intersection = 0;
      }
      // 5. NAVIGATION LOGIC for following the black path
      if (C && !L && !R) {
        set_motors(MAX_SPEED, MAX_SPEED); // Straight
      }
      else if (L && !R) {
        set_motors(0, MAX_SPEED); // Hard Left
      }
      else if (R && !L) {
        set_motors(MAX_SPEED, 0); // Hard Right
      }
      else if (!L && !C && !R) {
        set_motors(SLOW_SPEED, SLOW_SPEED); // Slow crawl if line lost
      }
}
}
/**
* @brief System Clock Configuration
*/
void SystemClock_Config(void)
{
RCC_OscInitTypeDef RCC_OscInitStruct = {0};
RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
__HAL_RCC_PWR_CLK_ENABLE();
__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
RCC_OscInitStruct.HSIState = RCC_HSI_ON;
RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
HAL_RCC_OscConfig(&RCC_OscInitStruct);
RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}
/**
* @brief TIM2 Initialization Function
*/
static void MX_TIM2_Init(void)
{
TIM_OC_InitTypeDef sConfigOC = {0};
__HAL_RCC_TIM2_CLK_ENABLE(); // Explicitly enable timer clock
htim2.Instance = TIM2;
htim2.Init.Prescaler = 16-1; // 16MHz clock down to 1MHz
htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
htim2.Init.Period = 1000-1;  // 0-1000 PWM range
htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
HAL_TIM_PWM_Init(&htim2);
sConfigOC.OCMode = TIM_OCMODE_PWM1;
sConfigOC.Pulse = 0;
sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4);
}
/**
* @brief GPIO Initialization Function
*/
static void MX_GPIO_Init(void)
{
GPIO_InitTypeDef GPIO_InitStruct = {0};
__HAL_RCC_GPIOA_CLK_ENABLE();
__HAL_RCC_GPIOB_CLK_ENABLE();
__HAL_RCC_GPIOC_CLK_ENABLE();
/* Motor Direction Pins */
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);
GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);
GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
/* PWM Pins (PA0 and PA3) */
GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3;
GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
/* Onboard LEDs */
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_14|GPIO_PIN_7, GPIO_PIN_RESET);
GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_14|GPIO_PIN_7;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
/* Inputs (Sensors & Communication) */
GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_8|
                      GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLDOWN;
HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
void Error_Handler(void)
{
__disable_irq();
while (1) {}
}

