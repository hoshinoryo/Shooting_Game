// ==========================================================================================
// 
// File Name: game.h
// Date: 2025/08/19
// Author: Gu Anyi
// Description: Game header file
// 
// ==========================================================================================
#ifndef GAME_H
#define GAME_H


void Game_Initialize();
void Game_Finalize();

void Game_Update(double elapsed_time);
void Game_Draw();

void Game_InstructionDraw();

#endif // GAME_H
