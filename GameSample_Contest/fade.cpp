// ==========================================================================================
// 
// File Name: fade.cpp
// Date: 2025/08/22
// Author: Gu Anyi
// Description: Manage the scene fading effects
// 
// ==========================================================================================

#include "fade.h"
#include "texture.h"
#include "sprite.h"
#include "direct3d.h"

#include <algorithm>

using namespace DirectX;

static double g_FadeTime{ 0.0 };
static double g_FadeStartTime{ 0.0 };
static double g_AccumulatedTime{ 0.0 };
static XMFLOAT3 g_Color{ 0.0f, 0.0f, 0.0f };
static float g_Alpha = 0.0f;
static FadeState g_State = FADE_STATE_NONE;
static Texture g_FadeTex;


void Fade_Initialize()
{
    g_FadeTime = 0.0;
    g_FadeStartTime = 0.0;
    g_AccumulatedTime = 0.0;
    g_Color = { 0.0f, 0.0f, 0.0f };
    g_Alpha = 0.0f;
    g_State = FADE_STATE_NONE;

    g_FadeTex.Initialize(Direct3D_GetDevice(), L"resources/white.png");
}

void Fade_Finalize()
{
}

void Fade_Update(double elapsed_time)
{
    if (g_State == FADE_STATE_NONE || g_State >= FADE_STATE_FINISHED_IN) return;

    g_AccumulatedTime += elapsed_time;

    // フェード開始からの経過時間 ÷ フェードの総時間 で進行率を出し、1.0 を上限として制限する
    double ratio = std::min((g_AccumulatedTime - g_FadeStartTime) / g_FadeTime, 1.0);

    if (ratio >= 1.0)
    {
        g_State = (g_State == FADE_STATE_IN ? FADE_STATE_FINISHED_IN : FADE_STATE_FINISHED_OUT);
        g_Alpha = (g_State == FADE_STATE_FINISHED_IN) ? 0.0f : 1.0f;

        return;
    }

    g_Alpha = (float)(g_State == FADE_STATE_IN ? 1.0 - ratio : ratio);
}

void Fade_Draw()
{
    if (g_State == FADE_STATE_NONE) return;
    if (g_State == FADE_STATE_FINISHED_IN) return;

    XMFLOAT4 color{ g_Color.x, g_Color.y, g_Color.z, g_Alpha };

    Sprite_Draw(g_FadeTex, 0.0f, 0.0f,
        Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(), false, color);
}

void Fade_Start(double time, bool isFadeOut, DirectX::XMFLOAT3 color)
{
    g_FadeStartTime = g_AccumulatedTime;
    g_FadeTime = time;
    g_State = isFadeOut ? FADE_STATE_OUT : FADE_STATE_IN;
    g_Color = color;
    g_Alpha = isFadeOut ? 0.0f : 1.0f;
}

FadeState Fade_GetState()
{
    return g_State;
}
