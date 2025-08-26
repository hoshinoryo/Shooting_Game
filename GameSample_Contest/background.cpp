// ==========================================================================================
// 
// File Name: background.h
// Date: 2025/07/16
// Author: Gu Anyi
// Description: Manage and draw the background for title and guide scenes
// 
// ==========================================================================================

#include "background.h"
#include "texture.h"
#include "sprite.h"
#include "sprite_anim.h"
#include "direct3d.h"

#include <DirectXMath.h>

using namespace DirectX;


static constexpr float TITLE_PLAYER_SPEED = 1.0f;

static constexpr float SCROLL_SPEED_01 = 200.0f;
static float g_Offset01X = 0.0f;

// Background
static Texture g_BackgroundTex;
static Texture g_ChimneyTex;
static Texture g_SnowmanTex;
static int ChimneyAnimId = -1;
static int SnowmanAnimId = -1;

// Player animation
static Texture g_PlayerTex;
static int PlayerAnimId = -1;
static XMFLOAT2 PlayerAnimPos = { 0.0f, 0.0f };
static bool PlayerFlipX = false;
static bool PlayerRightMov = true;

void BG_Initialize()
{
    g_BackgroundTex.Initialize(Direct3D_GetDevice(), L"resources/Title_Background_2.png");
    g_ChimneyTex.Initialize(Direct3D_GetDevice(), L"resources/Chocolate_Chimney.png");
    g_PlayerTex.Initialize(Direct3D_GetDevice(), L"resources/Santa_Claus.png");
    g_SnowmanTex.Initialize(Direct3D_GetDevice(), L"resources/Snowman_1_Anim.png");

    ChimneyAnimId = SpriteAnim_CreatePlayer(
        SpriteAnim_RegisterPattern(g_ChimneyTex, 5, 5, 0.15, { 128.0f, 128.0f }, { 0.0f, 0.0f }, true)
    );
    SnowmanAnimId = SpriteAnim_CreatePlayer(
        SpriteAnim_RegisterPattern(g_SnowmanTex, 8, 8, 0.15, { 128.0f, 128.0f }, { 0.0f, 0.0f }, true)
    );

    PlayerAnimPos = { 100.0f, 760.0f }; // starting point
    PlayerAnimId = SpriteAnim_CreatePlayer(
        SpriteAnim_RegisterPattern(g_PlayerTex, 6, 6, 0.12, { 128.0f, 128.0f }, { 0.0f, 128.0f }, true)
    );
}

void BG_Finalize()
{
    g_SnowmanTex.Finalize();
    g_PlayerTex.Finalize();
    g_ChimneyTex.Finalize();
    g_BackgroundTex.Finalize();
}

void BG_Update(double elapsed_time)
{
    if (PlayerRightMov)
    {
        PlayerAnimPos.x += TITLE_PLAYER_SPEED;
        PlayerFlipX = false;
        if (PlayerAnimPos.x >= 500.0f)
        {
            PlayerAnimPos.x = 500.0f;
            PlayerRightMov = false;
        }
    }
    else
    {
        PlayerAnimPos.x -= TITLE_PLAYER_SPEED;
        PlayerFlipX = true;
        if (PlayerAnimPos.x <= 100.0f)
        {
            PlayerAnimPos.x = 100.f;
            PlayerRightMov = true;
        }
    }
}

void BG_Draw()
{
	// screen size: 1600x900

    Sprite_Draw(g_BackgroundTex, 0.0f, 0.0f);
    SpriteAnim_Draw(ChimneyAnimId, 160.0f, 300.0f, 128.0f, 128.0f, false);
    SpriteAnim_Draw(SnowmanAnimId, 1100.0f, 550.0f, 128.0f, 128.0f, false);

    SpriteAnim_Draw(PlayerAnimId, PlayerAnimPos.x, PlayerAnimPos.y, 128.0f, 128.0f, PlayerFlipX);
}
