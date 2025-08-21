// ==========================================================================================
// 
// File Name: title.cpp
// Date: 2025/08/21
// Author: Gu Anyi
// Description: Manage and draw the title scene
// 
// ==========================================================================================

#include "title.h"
//#include "fade.h"
#include "scene.h"
#include "texture.h"
#include "sprite.h"
#include "key_logger.h"
#include "direct3d.h"


enum TitleState
{
    TITLE_STATE_FADE_IN,
    TITLE_STATE_FIGHTER_SHOW,
};

static TitleState g_State = {}; // タイトル演出制御、classが使うならもっと簡単に使える

static Texture g_TitleBgTex;

void Title_Initialize()
{
    g_TitleBgTex.Initialize(Direct3D_GetDevice(), L"resources/test_title.png");
}

void Title_Finalize()
{
    //Texture_AllRelease();
}

void Title_Update(double elapsed_time)
{
    /*
    if (KeyLogger_IsTrigger(KK_ENTER))
    {
        Fade_Start(1.0, true);
    }

    if (Fade_GetState() == FADE_STATE_FINISHED_OUT)
    {
        Scene_Change(SCENE_GAME);
    }
    */
}

void Title_Draw()
{
    Sprite_Draw(g_TitleBgTex, 300.0f, 100.0f);
}

