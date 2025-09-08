// ==========================================================================================
// 
// File Name: game.cpp
// Date: 2025/08/19
// Author: Gu Anyi
// Description: Manage the main game loop
// 
// ==========================================================================================

#include "Audio.h"
#include "bullet.h"
#include "camera.h"
#include "collision.h"
#include "check_collision.h"
#include "direct3d.h"
#include "enemy.h"
#include "enemy_spawner.h"
#include "fade.h"
#include "game.h"
#include "key_logger.h"
#include "map.h"
#include "player.h"
#include "result.h"
#include "render_queue.h"
#include "scene.h"
#include "sprite.h"
#include "ui_element.h"


// Debug output
#include "debug_text.h"
#include "debug_ostream.h"

#include <iostream>
#include <string>

Map mapMg;
Map mapFg;
Map mapDeco;
Collision_Map mapCollision;
std::string mg_filePath = "resources/Tiled_Project/output/test_map_mg.csv";
std::string fg_filePath = "resources/Tiled_Project/output/test_map_fg.csv";
std::string deco_filePath = "resources/Tiled_Project/output/test_map_deco.csv";
std::string col_filePath = "resources/Tiled_Project/output/test_map_collisions.csv";

Player player;
Camera gameCam;
ScoreUI scoreUI;
StateUI stateUI;
UIManager UIManger;


DirectX::XMFLOAT2 PLAYER_START_POSITION = { 950.0f, 850.0f };


static bool gameStart = false;
static Texture instructionTex;
static Texture whiteTex;
static int bgmId = -1;


void Game_Initialize()
{   
#if defined(DEBUG) || defined(_DEBUG)

    //hal::dout << "Game_Initialize called!" << std::endl;

#endif

    // Map Initialization
    mapMg.Initialize(mg_filePath, L"resources/Christmas_Grass.png", 64, 64);
    mapFg.Initialize(fg_filePath, L"resources/Christmas_Grass.png", 64, 64);
    mapDeco.Initialize(deco_filePath, L"resources/Deco_New.png", 64, 64);
    mapCollision.Initialize(col_filePath, 64, 64);

    instructionTex.Initialize(Direct3D_GetDevice(), L"resources/Instruction.png");
    whiteTex.Initialize(Direct3D_GetDevice(), L"resources/white.png");

    // Camera Initialization
    gameCam.Initialize(
        Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
        (float)mapMg.GetMapWidth(), (float)mapMg.GetMapHeight()
    );

    // Player Initialization
    player.Initialize(PLAYER_START_POSITION);
    Bullet_Initialize();
    
    // Enemy Spawner in world coordinate
    EnemySpawner_Initialize();
    EnemySpawner_Create({ 200.0f, 0.0f }, { 1000.0f, 0.0f }, ENEMY_TYPE_01, 2.0f, 2.0, 8);
    EnemySpawner_Create({ 0.0f, 300.0f }, { 0.0f, 900.0f }, ENEMY_TYPE_02, 8.0f, 4.0, 4);
    EnemySpawner_Create({ 3200.0f, 500.0f }, { 3200.0f, 1600.0f }, ENEMY_TYPE_03, 10.0f, 3.0, 6);

    bgmId = LoadAudio("resources/BGM/christmas_piano.wav");

    // Game UI
    stateUI.Initialize({ 1100.0f, 40.0f });
    scoreUI.Initialize({ 1350.0f, 120.0f }, 5);
    UIManger.Add(&stateUI);
    UIManger.Add(&scoreUI);

    stateUI.BindPlayer(&player);

    Fade_Start(0.8f, false);
    gameStart = false;
}

void Game_Finalize()
{
    gameStart = false;

    scoreUI.Finalize();
    stateUI.Finailize();

    UnloadAudio(bgmId);
    EnemySpawner_Finalize();

    Bullet_Finalize();
    player.Finalize();

    gameCam.Finalize();

    whiteTex.Finalize();
    instructionTex.Finalize();

    mapCollision.Finalize();
    mapDeco.Finalize();
    mapFg.Finalize();
    mapMg.Finalize();
}

void Game_Update(double elapsed_time)
{
    if (!gameStart)
    {
        if (KeyLogger_IsTrigger(KK_SPACE))
        {
            gameStart = true;
            PlayAudio(bgmId, true);
        }
        return;
    }

    EnemySpawner_Update(elapsed_time);

    // enemy update
    Enemy_UpdateAll(elapsed_time, player.GetWorldPosition(), mapCollision, gameCam.GetViewRect());

    // player update
    player.Update(elapsed_time, mapCollision, gameCam.GetViewRect());

    // bullet update
    Bullet_UpdateAll(elapsed_time, gameCam.GetViewRect());

    gameCam.Update(player.GetWorldPosition());

    // if player die
    if (!player.GetIsEnable())
    {
        Result_SetGameResult(GAME_OVER);
        Result_SetScoreAndDigit(scoreUI.GetScore(), scoreUI.GetDigit());
        Scene_Change(SCENE_RESULT);
        return;
    }

    CheckCollision_BulletVSEnemy();
    CheckCollision_PlayerVSEnemy(player);

    // UI update
    UIManger.Update(elapsed_time);

    // game clear
    if (Enemy_AreAllCleared() && EnemySpawner_IsFinishedAll())
    {
        Result_SetGameResult(GAME_CLEAR);
        Result_SetScoreAndDigit(scoreUI.GetScore(), scoreUI.GetDigit());
        Scene_Change(SCENE_RESULT);
    }
    
}

void Game_Draw()
{
    // Map Drawing
    mapMg.Draw(gameCam.GetViewRect());
    mapFg.Draw(gameCam.GetViewRect());
    //mapCollision.Draw(gameCam.GetViewRect());

    mapDeco.QueueDraw(gameCam.GetViewRect());

    RenderQueue::DrawAll();

    UIManger.Draw();

    if (!gameStart)
    {
        Game_InstructionDraw();
    }
}

void Game_InstructionDraw()
{
    Sprite_Draw(whiteTex, 0.0f, 0.0f, 1600.0f, 900.0f, false, { 1.0f, 1.0f, 1.0f, 0.8f });

    Sprite_Draw(instructionTex, 248.0f, 137.5f);
}

