#include "dino_game.h"
	
void Menu_Player_Show(){
	LCD_Clear();
	LCD_Send_Str_Pos("Choose a player",0,0);
	LCD_Send_Str_Pos("Man",0,1);
	LCD_Send_Str_Pos("Llama",0,2);
	LCD_Send_Str_Pos("Dino",0,3);
}

void Menu_Obstacle_Show(){
	LCD_Clear();
	LCD_Send_Str_Pos("Choose an obstacle",0,0);
	LCD_Send_Str_Pos("Cacti",0,1);
	LCD_Send_Str_Pos("Tulip",0,2);
	LCD_Send_Str_Pos("Building",0,3);
}

void Game_Start(){
	LCD_Clear();					
	LCD_Set_Position(player_x, 3);
	LCD_Send_Char(player);
	LCD_Send_Str_Pos("SCORE:",5,0);
}

