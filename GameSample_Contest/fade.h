// ==========================================================================================
// 
// File Name: fade.h
// Date: 2025/08/22
// Author: Gu Anyi
// Description: Fade header file
// 
// ==========================================================================================
#ifndef FADE_H
#define FADE_H

#include <DirectXMath.h>

enum FadeState
{
    FADE_STATE_NONE,
    FADE_STATE_IN,
    FADE_STATE_OUT,
    FADE_STATE_FINISHED_IN,
    FADE_STATE_FINISHED_OUT,
    FADE_STATE_MAX
};

void Fade_Initialize();
void Fade_Finalize();
void Fade_Update(double elapsed_time);
void Fade_Draw();

void Fade_Start(double time, bool isFadeOut, DirectX::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f });

FadeState Fade_GetState();

#endif // FADE_H