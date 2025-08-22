// ==========================================================================================
// 
// File Name: title.cpp
// Date: 2025/08/21
// Author: Gu Anyi
// Description: Manage and draw the title scene
// 
// ==========================================================================================

#include "title.h"
#include "fade.h"
#include "scene.h"
#include "texture.h"
#include "sprite.h"
#include "sprite_anim.h"
#include "key_logger.h"
#include "direct3d.h"


static constexpr float BUTTON_CENTER = 800.0f;

enum TitleState
{
    TITLE_STATE_FADE_IN,
    TITLE_STATE_FIGHTER_SHOW,
};

enum MenuState
{
    MENU_PLAY,
    MENU_OPTIONS,
    MENU_QUIT,
    MENU_MAX
};

static TitleState g_State = {};
static int g_SelectedMenu = MENU_PLAY;
static bool g_isPressed = false;
static double g_pressedTime = 0.0;
static bool g_fadeStarted = false;

static Texture g_TitleTex;
static Texture g_PlayButtonTex;
static Texture g_GuideButtonTex;
static Texture g_QuitButtonTex;

// Background
static Texture g_BackgroundTex;
static Texture g_ChimneyTex;
static int ChimneyAnimId = -1;

void Title_Initialize()
{
    g_TitleTex.Initialize(Direct3D_GetDevice(), L"resources/test_title.png");
    g_PlayButtonTex.Initialize(Direct3D_GetDevice(), L"resources/Button_Play.png");
    g_GuideButtonTex.Initialize(Direct3D_GetDevice(), L"resources/Button_Guide.png");
    g_QuitButtonTex.Initialize(Direct3D_GetDevice(), L"resources/Button_Quit.png");
    g_BackgroundTex.Initialize(Direct3D_GetDevice(), L"resources/Title_Background_2.png");
    g_ChimneyTex.Initialize(Direct3D_GetDevice(), L"resources/Chocolate_Chimney.png");

    ChimneyAnimId = SpriteAnim_CreatePlayer(
        SpriteAnim_RegisterPattern(g_ChimneyTex, 5, 5, 0.15, { 128.0f, 128.0f }, { 0.0f, 0.0f }, true)
    );
}

void Title_Finalize()
{
    g_ChimneyTex.Finalize();
    g_BackgroundTex.Finalize();
    g_QuitButtonTex.Finalize();
    g_GuideButtonTex.Finalize();
    g_PlayButtonTex.Finalize();
    g_TitleTex.Finalize();
}

void Title_Update(double elapsed_time)
{
    if (!g_isPressed)
    {
        if (KeyLogger_IsTrigger(KK_DOWN))
        {
            g_SelectedMenu = (g_SelectedMenu + 1) % MENU_MAX;
        }
        if (KeyLogger_IsTrigger(KK_UP))
        {
            g_SelectedMenu = (g_SelectedMenu - 1 + MENU_MAX) % MENU_MAX;
        }

        if (KeyLogger_IsTrigger(KK_ENTER))
        {
            g_isPressed = true;
            g_pressedTime = 0.0;
            g_fadeStarted = false;
        }
    }
    else
    {
        g_pressedTime += elapsed_time;
        if (!g_fadeStarted && g_pressedTime >= 0.4)
        {
            if (g_SelectedMenu == MENU_PLAY)
            {
                Fade_Start(0.8f, true);
                g_fadeStarted = true;
            }
            else if (g_SelectedMenu == MENU_QUIT)
            {
                PostQuitMessage(0);
            }
        }

        if (g_SelectedMenu == MENU_PLAY && Fade_GetState() == FADE_STATE_FINISHED_OUT)
        {
            Scene_Change(SCENE_GAME);
        }
    }
}

void Title_Draw()
{
    Sprite_Draw(g_BackgroundTex, 0.0f, 0.0f);
    SpriteAnim_Draw(ChimneyAnimId, 160.0f, 300.0f, 128.0f, 128.0f, false);
    Sprite_Draw(g_TitleTex, 370.0f, 60.0f);

    int playState = 0;
    if (g_SelectedMenu == MENU_PLAY)
    {
        playState = g_isPressed ? 2 : 1;
    }
    Sprite_Draw(g_PlayButtonTex, BUTTON_CENTER, 530.0f, (g_PlayButtonTex.GetWidth() / 3) * playState, 0.0f,
        g_PlayButtonTex.GetWidth() / 3, g_PlayButtonTex.GetHeight());

    int optionsState = 0;
    if (g_SelectedMenu == MENU_OPTIONS)
    {
        optionsState = g_isPressed ? 2 : 1;
    }
    Sprite_Draw(g_GuideButtonTex, BUTTON_CENTER,630.0f, (g_GuideButtonTex.GetWidth() / 3) * optionsState, 0.0f,
        g_GuideButtonTex.GetWidth() / 3, g_GuideButtonTex.GetHeight());

    int quitState = 0;
    if (g_SelectedMenu == MENU_QUIT)
    {
        quitState = g_isPressed ? 2 : 1;
    }
    Sprite_Draw(g_QuitButtonTex, BUTTON_CENTER, 730.0f, (g_QuitButtonTex.GetWidth() / 3) * quitState, 0.0f,
        g_QuitButtonTex.GetWidth() / 3, g_QuitButtonTex.GetHeight());
}

