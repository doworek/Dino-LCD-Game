/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lcd_i2c_hd44780.h"
#include "dino_game.h"

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

		
void Delay_tm3(uint16_t delay)
{
	__HAL_TIM_SET_COUNTER(&htim3, 0);
  while(__HAL_TIM_GET_COUNTER(&htim3)<delay);
}
		
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == Button_left_Pin)
		{
			button_pushed_left = true;
		} else if(GPIO_Pin == Button_jump_Pin)
		{
			HAL_TIM_Base_Start_IT(&htim3);
			Delay_tm3(800);
			button_pushed_jump = true;
			y_draw=2; y_erase=3;
		} else if(GPIO_Pin == Button_right_Pin)
		{
			button_pushed_right = true;
		}
}	

void Gameplay_Obstacles()
{
	if(obstacle_x<0)
				{
					LCD_Set_Position(0,3);
					LCD_Send_Char(' ');
					obstacle_x=19;
				}
				
			LCD_Set_Position(obstacle_x,3);
				
			if(map[obstacle_x]==1)
			{
				LCD_Send_Char(obstacle);
				obstacle_flag=true;
			}else
			{ 
				obstacle_flag=false;
				LCD_Send_Char(' ');
			}
				obstacle_x--;
}

void Gameplay_Player()
{
	LCD_Set_Position(player_x, y_erase);
	if(map[player_x]==1)
		{
			LCD_Send_Char_Pos(obstacle,player_x,3);
		}
		LCD_Send_Char(' ');
		LCD_Set_Position(player_x, y_draw);
		LCD_Send_Char(player);
}


void Show_Score()
{
	if(obstacle_x+1 == player_x && y_draw == 2 && obstacle_flag)
		{
			score++;
		}
		sprintf(score_ch, "%d", score);
		LCD_Send_Str_Pos(score_ch,11,0);
}


void Game_Over()
{
	if(obstacle_x+1==player_x && y_draw==3 && obstacle_flag)
	{
		LCD_Clear(); 
		LCD_Send_Str_Pos("Game Over", 5,1);
		LCD_Send_Str_Pos("Your score: ", 3,2); 
		LCD_Send_Str_Pos(score_ch, 14,2); 
		HAL_Delay(3000);
		LCD_Clear();
		state_gameplay=false;
	}
}
int main(void)
{ 
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
	LCD_Init();
	
	//custom characters
static const unsigned char Man[] = { 0x0C, 0x0C, 0x00, 0x0F, 0x1C, 0x0C, 0x1A, 0x13};
static const unsigned char Llama[] = { 0x02, 0x02, 0x03, 0x03, 0x02, 0x1E, 0x0E, 0x0A };
static const unsigned char Dino[] = { 0x03, 0x03, 0x02, 0x07, 0x16, 0x1E, 0x04, 0x06 };
static const unsigned char Rabbit[] = { 0x02, 0x02, 0x03, 0x03, 0x0E, 0x1E, 0x0E, 0x0A };
static const unsigned char Cactus[] = { 0x04, 0x04, 0x05, 0x15, 0x15, 0x17, 0x1C, 0x04 };
static const unsigned char Tulip[] = { 0x15, 0x1F, 0x1F, 0x0E, 0x04, 0x06, 0x0C, 0x04 };
static const unsigned char Building[] = { 0x1F, 0x15, 0x1F, 0x15, 0x1F, 0x15, 0x1F, 0x1F };

	Create_Custom_Character(Man,1);
	Create_Custom_Character(Llama,2);
	Create_Custom_Character(Dino,3); 
	Create_Custom_Character(Rabbit,4); 
	Create_Custom_Character(Cactus,5); 
	Create_Custom_Character(Tulip,6); 
	Create_Custom_Character(Building,7);
	
	state_menu_player = true;

Menu_Player_Show();

  /* Infinite loop */
  while (1)
  {
		if(state_gameplay)
			{
				Gameplay_Obstacles();
				Gameplay_Player();
				Show_Score();
			}
		
			if(button_pushed_left)
				{
				if(state_menu_player)
					{
						player = 1;
            Menu_Obstacle_Show();
						state_menu_player = false;
						state_gameplay = false;
						state_menu_obstacle = true;
						button_pushed_left = false;
					}else if(state_menu_obstacle)
					{
						obstacle = 5;
            Game_Start();
						state_menu_player = false;
						HAL_TIM_Base_Start_IT(&htim2);
						state_menu_obstacle = false;
						state_gameplay = true;
						button_pushed_left = false;
					}else if(state_gameplay)
					{
						if(player_x>0 && y_draw==3)
							{
								LCD_Send_Char_Pos(' ' ,player_x, 3);
								player_x--;
								LCD_Send_Char_Pos(player, player_x, 3);
								button_pushed_left = false;
							}
					}
				}
				
			else if(button_pushed_right)
				{
					if(state_menu_player)
						{
							player = 3;
							Menu_Obstacle_Show();
							state_menu_player = false;
							state_gameplay = false;
							state_menu_obstacle = true;
							button_pushed_right = false;	
						}else if(state_menu_obstacle)
						{
							obstacle = 7;
            Game_Start();
						state_menu_player = false;
						state_menu_obstacle = false;
						HAL_TIM_Base_Start_IT(&htim2);
						state_gameplay = true;
							button_pushed_right = false;	
						} else if(state_gameplay)
						{
							if(player_x<19 && y_draw==3)
								{
									LCD_Send_Char_Pos(' ' ,player_x, 3);
									player_x++;
									LCD_Send_Char_Pos(player, player_x, 3);
									button_pushed_right = false;		
								}		
						}				 
				}
		else if(button_pushed_jump)
			{
				if(state_menu_player)
					{
						player = 2;
            Menu_Obstacle_Show();
						state_menu_player = false;
						state_gameplay = false;
						state_menu_obstacle = true;
						button_pushed_jump = false;
					}
					else if(state_menu_obstacle)
						{
							obstacle = 6;
            Game_Start();
						state_menu_player = false;
						state_menu_obstacle = false;
						HAL_TIM_Base_Start_IT(&htim2);
						state_gameplay = true;
							button_pushed_jump = false;
						}
						else if(state_gameplay)
							{
								button_pushed_jump = false;
							}
			}
			
			Game_Over();
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
	
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7200-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4000-1;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7200-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Led_Pin */
  GPIO_InitStruct.Pin = Led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Led_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Button_left_Pin Button_jump_Pin Button_right_Pin */
  GPIO_InitStruct.Pin = Button_left_Pin|Button_jump_Pin|Button_right_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
		 if(htim->Instance==TIM1)
		 {
		 }
		 
	 if(htim->Instance==TIM2)
		 {
	   if(k>100){ k=0;}
	 
	     map[19] = generate_obstacles[k];
	 
			 for(uint8_t i=1; i<=19; i++)
			 {
				 map[i-1] = map[i];
			 }
			 k++;
		 }
		 
     if(htim->Instance==TIM3)
		  {
			 HAL_GPIO_TogglePin(Led_GPIO_Port, Led_Pin);
			 y_draw=3;
			 y_erase=2;
			 HAL_TIM_Base_Stop(&htim3);
		  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
