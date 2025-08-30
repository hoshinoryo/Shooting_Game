// ==========================================================================================
// 
// File Name: scene.cpp
// Date: 2025/08/21
// Author: Gu Anyi
// Description: Manage the scene
// 
// ==========================================================================================

#include "scene.h"
#include "game.h"
#include "title.h"
#include "guide.h"
#include "result.h"

static Scene g_Scene = SCENE_TITLE;
//static Scene g_Scene = SCENE_RESULT; // debug
static Scene g_SceneNext = g_Scene;

void Scene_Initialize()
{
    switch (g_Scene)
    {
    case SCENE_TITLE:
        Title_Initialize();
        break;
    case SCENE_GUIDE:
        Guide_Initialize();
        break;
    case SCENE_GAME:
        Game_Initialize();
        break;
    case SCENE_RESULT:
        Result_Initialize();
        break;
    }
}

void Scene_Finalize()
{
    switch (g_Scene)
    {
    case SCENE_TITLE:
        Title_Finalize();
        break;
    case SCENE_GUIDE:
        Guide_Finalize();
        break;
    case SCENE_GAME:
        Game_Finalize();
        break;
    case SCENE_RESULT:
        Result_Finalize();
        break;
    }
}

void Scene_Update(double elapsed_time)
{
    switch (g_Scene)
    {
    case SCENE_TITLE:
        Title_Update(elapsed_time);
        break;
    case SCENE_GUIDE:
        Guide_Update(elapsed_time);
        break;
    case SCENE_GAME:
        Game_Update(elapsed_time);
        break;
    case SCENE_RESULT:
        Result_Update(elapsed_time);
        break;
    }
}

void Scene_Draw()
{
    switch (g_Scene)
    {
    case SCENE_TITLE:
        Title_Draw();
        break;
    case SCENE_GUIDE:
        Guide_Draw();
        break;
    case SCENE_GAME:
        Game_Draw();
        break;
    case SCENE_RESULT:
        Result_Draw();
        break;
    }
}

void Scene_Refresh() // シーン更新
{
    if (g_Scene != g_SceneNext)
    {
        Scene_Finalize();

        g_Scene = g_SceneNext;

        Scene_Initialize();
    }
}

void Scene_Change(Scene scene)
{
    g_SceneNext = scene;
}
