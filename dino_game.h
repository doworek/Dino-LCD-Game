/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DINO_GAME_H_
#define DINO_GAME_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "lcd_i2c_hd44780.h"
#include "stdlib.h"
#include <stdbool.h>

static int player;
static int obstacle;

static int player_x = 4; //current player x position
static int obstacle_x = 19; //current obstacle x position
static bool obstacle_flag; //is obstacle shown?
static bool state_menu_player = false;
static bool state_menu_obstacle = false;
static bool state_gameplay = false;
static bool button_pushed_left;
static bool button_pushed_right;
static bool button_pushed_jump;
static uint8_t y_draw = 2; //y position draw player
static uint8_t y_erase = 3; //y position erase player
static uint8_t k = 0;
static int score = 0;
static char score_ch[10];
static int map[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //map of obstacles
static const int generate_obstacles[100]={0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0};

void Game_Start(void);
void Menu_Player_Show(void);
void Menu_Obstacle_Show(void);
void Gameplay_Obstacles(void);
void Gameplay_Player(void);
void Show_Score(void);
void Game_Over(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
