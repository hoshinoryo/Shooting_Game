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

// Button texture
static Texture WButtonTex;
static Texture AButtonTex;
static Texture SButtonTex;
static Texture DButtonTex;
static Texture SpaceButtonTex;
static Texture EnterButtonTex;

static Texture GuideBGTex;
static Texture MoveTex;
static Texture ShootTex;
static Texture PressTex;
static Texture RetureTex;

// Button animation
static int WButtonAnimId = -1;
static int AButtonAnimId = -1;
static int SButtonAnimId = -1;
static int DButtonAnimId = -1;
static int SpaceButtonAnimId = -1;
static int EnterButtonAnimId = -1;

void Guide_Initialize()
{
	BG_Initialize();
	WButtonTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_W.png");
	AButtonTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_A.png");
	SButtonTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_S.png");
	DButtonTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_D.png");
	SpaceButtonTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_Space.png");
	EnterButtonTex.Initialize(Direct3D_GetDevice(), L"resources/UI_Buttons/Button_Enter.png");

	GuideBGTex.Initialize(Direct3D_GetDevice(), L"resources/Guide_Background.png");
	MoveTex.Initialize(Direct3D_GetDevice(), L"resources/Move.png");
	ShootTex.Initialize(Direct3D_GetDevice(), L"resources/Shoot.png");
	PressTex.Initialize(Direct3D_GetDevice(), L"resources/Press.png");
	RetureTex.Initialize(Direct3D_GetDevice(), L"resources/to_return.png");

	WButtonAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(WButtonTex, 3, 3, PLAY_SPEED, { 70.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
	AButtonAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(AButtonTex, 3, 3, PLAY_SPEED, { 70.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
	SButtonAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(SButtonTex, 3, 3, PLAY_SPEED, { 70.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
	DButtonAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(DButtonTex, 3, 3, PLAY_SPEED, { 70.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
	SpaceButtonAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(SpaceButtonTex, 3, 3, PLAY_SPEED, { 240.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
	EnterButtonAnimId = SpriteAnim_CreatePlayer(
		SpriteAnim_RegisterPattern(EnterButtonTex, 3, 3, PLAY_SPEED, { 240.0f, 70.0f }, { 0.0f, 0.0f }, true)
	);
}

void Guide_Finalize()
{
	RetureTex.Finalize();
	PressTex.Finalize();
	ShootTex.Finalize();
	MoveTex.Finalize();
	GuideBGTex.Finalize();
	EnterButtonTex.Finalize();
	SpaceButtonTex.Finalize();
	DButtonTex.Finalize();
	SButtonTex.Finalize();
	AButtonTex.Finalize();
	WButtonTex.Finalize();
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

	// Button animation
	SpriteAnim_Draw(WButtonAnimId, STANDARD_POINT_X + 120.0f, STANDARD_POINT_Y, 100.0f, 100.0f, false);
	SpriteAnim_Draw(AButtonAnimId, STANDARD_POINT_X, STANDARD_POINT_Y + 100.0f, 100.0f, 100.0f, false);
	SpriteAnim_Draw(SButtonAnimId, STANDARD_POINT_X + 120.0f, STANDARD_POINT_Y + 100.0f, 100.0f, 100.0f, false);
	SpriteAnim_Draw(DButtonAnimId, STANDARD_POINT_X + 240.0f, STANDARD_POINT_Y + 100.0f, 100.0f, 100.0f, false);

	SpriteAnim_Draw(SpaceButtonAnimId, STANDARD_POINT_X, STANDARD_POINT_Y + 250.0f, 336.0f, 98.0f, false);

	Sprite_Draw(PressTex, 700.0f, 720.0f);
	SpriteAnim_Draw(EnterButtonAnimId, 860.0f, 710.0f, 150.0f, 43.75f, false);
	Sprite_Draw(RetureTex, 1020.0f, 720.0f);
}
