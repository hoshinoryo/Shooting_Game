// ==========================================================================================
// 
// File Name: result.cpp
// Date: 2025/08/29
// Author: Gu Anyi
// Description: Manage the result scene
// 
// ==========================================================================================

#include "result.h"
#include "texture.h"
#include "sprite.h"
#include "sprite_anim.h"
#include "direct3d.h"
#include "scene.h"
#include "key_logger.h"

#include <DirectXMath.h>

using namespace DirectX;


static constexpr float FONT_W = 64.0f;
static constexpr float FONT_H = 47.0f;
static constexpr float PLAY_SPEED = 0.25f;

static Texture ScoreBGTex;
static Texture FinalScoreWordTex;
static Texture ScoreTex;
static Texture PressTex;
static Texture ReturnTex;

static Texture ReindeerTex;
static Texture EnterKeyTex;

static Texture OverTex;
static Texture ClearTex;

static Texture GingerHouseTex;
static Texture AbandonedHouseTex;

static int ReindeerAnimId = -1;
static int EnterKeyAnimId = -1;

static unsigned int g_ResultScore = 0;
static int g_digit = 0;
static Game_Result g_gameResult = GAME_RESULT_MAX;

void Result_Initialize()
{
	ScoreBGTex.Initialize(Direct3D_GetDevice(), L"resources/Result_Background.png");
	FinalScoreWordTex.Initialize(Direct3D_GetDevice(), L"resources/Final_score.png");
	ScoreTex.Initialize(Direct3D_GetDevice(), L"resources/Final_score_number.png");
	PressTex.Initialize(Direct3D_GetDevice(), L"resources/Press.png");
	ReturnTex.Initialize(Direct3D_GetDevice(), L"resources/return_to_main_menu.png");
	ReindeerTex.Initialize(Direct3D_GetDevice(), L"resources/Reindeer.png");
	EnterKeyTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_Enter.png");
	OverTex.Initialize(Direct3D_GetDevice(), L"resources/Game_over.png");
	ClearTex.Initialize(Direct3D_GetDevice(), L"resources/Game_clear.png");
	GingerHouseTex.Initialize(Direct3D_GetDevice(), L"resources/Gingerbread_House.png");
	AbandonedHouseTex.Initialize(Direct3D_GetDevice(), L"resources/Gingerbread_House_Abandoned.png");

	ReindeerAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(ReindeerTex, 2, 2, PLAY_SPEED, { 128.0f, 128.0f }, { 0.0f, 0.0f }, true)
	);
	EnterKeyAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(EnterKeyTex, 3, 3, PLAY_SPEED, { 240.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
}

void Result_Finalize()
{
	SpriteAnim_DestroyPlayer(EnterKeyAnimId);
	SpriteAnim_DestroyPlayer(ReindeerAnimId);

	AbandonedHouseTex.Finalize();
	GingerHouseTex.Finalize();
	ClearTex.Finalize();
	OverTex.Finalize();
	EnterKeyTex.Finalize();
	ReindeerTex.Finalize();
	ReturnTex.Finalize();
	PressTex.Finalize();
	ScoreTex.Finalize();
	FinalScoreWordTex.Finalize();
	ScoreBGTex.Finalize();
}

void Result_Update(double elapsed_time)
{
	if (KeyLogger_IsTrigger(KK_ENTER))
	{
		Scene_Change(SCENE_TITLE);
	}
}

void Result_Draw()
{
	Sprite_Draw(ScoreBGTex, 608.0f, 218.0f);
	Sprite_Draw(FinalScoreWordTex, 680.0f, 290.0f);
	Result_DrawScore({ 840.0f, 400.0f });
	SpriteAnim_Draw(ReindeerAnimId, 1250.0f, 550.0f, 128.0f, 128.0f, false);

	Result_DrawResult(Result_GetGameResult(), {860.0f, 520.0f});
	Result_DrawHouse(Result_GetGameResult());

	Sprite_Draw(PressTex, 460.0f, 720.0f);
	SpriteAnim_Draw(EnterKeyAnimId, 625.0f, 700.0f, 192.0f, 56.0f, false);
	Sprite_Draw(ReturnTex, 830.0f, 720.0f);
}

void Result_SetScoreAndDigit(const unsigned int score, const int digit)
{
	g_ResultScore = score;
	g_digit = digit;
}

void Result_DrawNumber(XMFLOAT2 pos, int number)
{
	Sprite_Draw(ScoreTex, pos.x, pos.y, FONT_W * number, 0, FONT_W, FONT_H);
}

void Result_DrawScore(XMFLOAT2 screenPosition)
{
	unsigned int temp = g_ResultScore;

	for (int i = 0; i < g_digit; i++)
	{
		int n = temp % 10;
		float x = screenPosition.x + (g_digit - i - 1) * FONT_W;

		Result_DrawNumber({ x, screenPosition.y }, n);

		temp /= 10;
	}
}

void Result_DrawResult(Game_Result result, XMFLOAT2 screenPosition)
{
	switch (result)
	{
	case GAME_OVER:
		Sprite_Draw(OverTex, screenPosition.x, screenPosition.y);
		break;
	case GAME_CLEAR:
		Sprite_Draw(ClearTex, screenPosition.x, screenPosition.y);
		break;
	}
}

void Result_DrawHouse(Game_Result result)
{
	switch (result)
	{
	case GAME_OVER:
		Sprite_Draw(AbandonedHouseTex, 130.0f, 100.0f);
		break;
	case GAME_CLEAR:
		Sprite_Draw(GingerHouseTex, 130.0f, 100.0f);
		break;
	}
}

void Result_SetGameResult(Game_Result result)
{
	g_gameResult = result;
}

Game_Result Result_GetGameResult()
{
	return g_gameResult;
}




