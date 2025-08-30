// ==========================================================================================
// 
// File Name: guide.cpp
// Date: 2025/08/24
// Author: Gu Anyi
// Description: Manage and draw the guide scene
// 
// ==========================================================================================

#include "guide.h"
#include "texture.h"
#include "sprite.h"
#include "sprite_anim.h"
#include "direct3d.h"
#include "background.h"
#include "key_logger.h"
#include "scene.h"


static constexpr float PLAY_SPEED = 0.25f;
static constexpr float STANDARD_POINT_X = 1000.0f;
static constexpr float STANDARD_POINT_Y = 135.0f;

// Key texture
static Texture WKeyTex;
static Texture AKeyTex;
static Texture SKeyTex;
static Texture DKeyTex;
static Texture SpaceKeyTex;
static Texture EnterKeyTex;

static Texture GuideBGTex;
static Texture MoveTex;
static Texture ShootTex;
static Texture PressTex;
static Texture RetureTex;

// Key animation
static int WKeyAnimId = -1;
static int AKeyAnimId = -1;
static int SKeyAnimId = -1;
static int DKeyAnimId = -1;
static int SpaceKeyAnimId = -1;
static int EnterKeyAnimId = -1;

void Guide_Initialize()
{
	BG_Initialize();
	WKeyTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_W.png");
	AKeyTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_A.png");
	SKeyTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_S.png");
	DKeyTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_D.png");
	SpaceKeyTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_Space.png");
	EnterKeyTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_Enter.png");

	GuideBGTex.Initialize(Direct3D_GetDevice(), L"resources/Guide_Background.png");
	MoveTex.Initialize(Direct3D_GetDevice(), L"resources/Move.png");
	ShootTex.Initialize(Direct3D_GetDevice(), L"resources/Shoot.png");
	PressTex.Initialize(Direct3D_GetDevice(), L"resources/Press.png");
	RetureTex.Initialize(Direct3D_GetDevice(), L"resources/to_return.png");

	WKeyAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(WKeyTex, 3, 3, PLAY_SPEED, { 70.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
	AKeyAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(AKeyTex, 3, 3, PLAY_SPEED, { 70.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
	SKeyAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(SKeyTex, 3, 3, PLAY_SPEED, { 70.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
	DKeyAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(DKeyTex, 3, 3, PLAY_SPEED, { 70.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
	SpaceKeyAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(SpaceKeyTex, 3, 3, PLAY_SPEED, { 240.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
	EnterKeyAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(EnterKeyTex, 3, 3, PLAY_SPEED, { 240.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
}

void Guide_Finalize()
{
	RetureTex.Finalize();
	PressTex.Finalize();
	ShootTex.Finalize();
	MoveTex.Finalize();
	GuideBGTex.Finalize();
	EnterKeyTex.Finalize();
	SpaceKeyTex.Finalize();
	DKeyTex.Finalize();
	SKeyTex.Finalize();
	AKeyTex.Finalize();
	WKeyTex.Finalize();
	BG_Finalize();
}

void Guide_Update(double elapsed_time)
{
	BG_Update(elapsed_time);

	if (KeyLogger_IsTrigger(KK_ENTER))
	{
		Scene_Change(SCENE_TITLE);
	}
}

void Guide_Draw()
{
	// Background drawing
	BG_Draw();
	Sprite_Draw(GuideBGTex, 0.0f, 0.0f);

	Sprite_Draw(MoveTex, STANDARD_POINT_X - 260.0f, STANDARD_POINT_Y + 135.0f);
	Sprite_Draw(ShootTex, STANDARD_POINT_X - 260.0f, STANDARD_POINT_Y + 285.0f);

	// Key animation
	SpriteAnim_Draw(WKeyAnimId, STANDARD_POINT_X + 120.0f, STANDARD_POINT_Y, 100.0f, 100.0f, false);
	SpriteAnim_Draw(AKeyAnimId, STANDARD_POINT_X, STANDARD_POINT_Y + 100.0f, 100.0f, 100.0f, false);
	SpriteAnim_Draw(SKeyAnimId, STANDARD_POINT_X + 120.0f, STANDARD_POINT_Y + 100.0f, 100.0f, 100.0f, false);
	SpriteAnim_Draw(DKeyAnimId, STANDARD_POINT_X + 240.0f, STANDARD_POINT_Y + 100.0f, 100.0f, 100.0f, false);

	SpriteAnim_Draw(SpaceKeyAnimId, STANDARD_POINT_X, STANDARD_POINT_Y + 250.0f, 336.0f, 98.0f, false);

	Sprite_Draw(PressTex, 700.0f, 720.0f);
	SpriteAnim_Draw(EnterKeyAnimId, 860.0f, 710.0f, 150.0f, 43.75f, false);
	Sprite_Draw(RetureTex, 1020.0f, 720.0f);
}
