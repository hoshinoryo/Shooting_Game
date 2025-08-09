// ==========================================================================================
// 
// File Name: background.h
// Date: 2025/07/16
// Author: Gu Anyi
// Description: Manage the background
// 
// ==========================================================================================

#include "background.h"
#include "texture.h"
#include "sprite.h"
#include "direct3d.h"

static int g_TestBgTexId = -1;
static constexpr float SCROLL_SPEED_01 = 200.0f;
static float g_Offset01X = 0.0f;

void BG_Initialize()
{
    g_TestBgTexId = Texture_Load(L"resources/test_background.png");
}

void BG_Finalize()
{
}

void BG_Update(double elapsed_time)
{
    //g_Offset01X += elapsed_time * SCROLL_SPEED_01;

    //if (g_Offset01X >= Direct3D_GetBackBufferWidth()) g_Offset01X -= Direct3D_GetBackBufferWidth();
}

void BG_Draw()
{
    // debug mode: test background
	// screen size: 1600x900
    Sprite_Draw(g_TestBgTexId, 0.0f, 0.0f, 1280.0f, 720.0f);
}
