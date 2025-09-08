// ==========================================================================================
// 
// File Name: result.h
// Date: 2025/08/29
// Author: Gu Anyi
// Description: Result header file
// 
// ==========================================================================================

#ifndef RESULT_H
#define RESULT_H

#include <DirectXMath.h>

enum Game_Result
{
	GAME_OVER,
	GAME_CLEAR,
	GAME_RESULT_MAX
};

void Result_Initialize();
void Result_Finalize();
void Result_Update(double elapsed_time);
void Result_Draw();

void Result_SetScoreAndDigit(const unsigned int score, const int digit);
void Result_DrawNumber(DirectX::XMFLOAT2 pos, int number);
void Result_DrawScore(DirectX::XMFLOAT2 screenPosition);

void Result_DrawResult(Game_Result result, DirectX::XMFLOAT2 screenPosition);
void Result_DrawHouse(Game_Result result);

void Result_SetGameResult(Game_Result result);
Game_Result Result_GetGameResult();

#endif // RESULT_H