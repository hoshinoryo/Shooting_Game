// ==========================================================================================
// 
// File Name: game.cpp
// Date: 2025/08/19
// Author: Gu Anyi
// Description: Manage the main game loop
// 
// ==========================================================================================

#include "game.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "enemy_spawner.h"
#include "collision.h"
#include "effect.h"
#include "direct3d.h"
#include "camera.h"
#include "map.h"
#include "check_collision.h"
#include "fade.h"
#include "ui_element.h"

// Debug output
#include "debug_text.h"
#include "debug_ostream.h"

#include <iostream>
#include <string>

Map testMapMg;
Map testMapFg;
Collision_Map testCollision;
std::string mg_filePath = "resources/Tiled_Project/output/test_map_mg.csv";
std::string fg_filePath = "resources/Tiled_Project/output/test_map_fg.csv";
std::string col_filePath = "resources/Tiled_Project/output/test_map_collisions.csv";

Player player;
Camera gameCam;
ScoreUI testScoreUI;
StateUI testStateUI;
UIManager testUIManger;

static bool g_GameStart = false;


void Game_Initialize()
{   
    // Map Initialization
    testMapMg.Initialize(mg_filePath);
    testMapFg.Initialize(fg_filePath);
    testCollision.Initialize(col_filePath);

    // Camera Initialization
    gameCam.Initialize(
        Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
        (float)testMapMg.GetMapWidth(), (float)testMapMg.GetMapHeight()
    );

    // Player Initialization
    player.Initialize({ Direct3D_GetBackBufferWidth() * 0.5f, Direct3D_GetBackBufferHeight() * 0.5f});
    Bullet_Initialize();

    // Enemy Spawner in world coordinate
    EnemySpawner_Initialize();
    EnemySpawner_Create({ 800.0f,   0.0f }, ENEMY_TYPE_01, 4.0f, 3.0, 8);
    EnemySpawner_Create({   0.0f, 700.0f }, ENEMY_TYPE_02, 3.0f, 5.0, 5);

    //Effect_Initialize();

    // Game UI
    testStateUI.Initialize({ 1100.0f, 40.0f });
    testScoreUI.Initialize({ 1350.0f, 120.0f }, 5);
    testUIManger.Add(&testStateUI);
    testUIManger.Add(&testScoreUI);

    testStateUI.BindPlayer(&player);

    Fade_Start(0.8f, false);
    g_GameStart = false;
}

void Game_Finalize()
{
    //Effect_Finalize();
    EnemySpawner_Finalize();

    Bullet_Finalize();
    player.Finalize();

    gameCam.Finalize();
    testCollision.Finalize();
    testMapFg.Finalize();
    testMapMg.Finalize();
}

void Game_Update(double elapsed_time)
{
    if (!g_GameStart && Fade_GetState() == FADE_STATE_FINISHED_IN)
    {
        g_GameStart = true;
    }

    EnemySpawner_Update(elapsed_time);
    Enemy_UpdateAll(elapsed_time, player.GetWorldPosition(), testCollision);

    player.Update(elapsed_time, testCollision);
    Bullet_Update(elapsed_time);

    gameCam.Update(player.GetWorldPosition());

    CheckCollision_BulletVSEnemy();
    CheckCollision_PlayerVSEnemy(player);

    //Effect_Update(elapsed_time);
    testUIManger.Update(elapsed_time);

//#if defined(DEBUG) || defined(_DEBUG)
//
//    hal::dout << "Player position: " << player.GetPosition().x << ", " << player.GetPosition().y << std::endl;
//    hal::dout << "Camera Position: " << gameCam.GetX() << ", " << gameCam.GetY() << std::endl;
//    hal::dout << "Delta: " << (player.GetPosition().x - gameCam.GetX() - Direct3D_GetBackBufferWidth() * 0.5f) << std::endl;
//
//#endif
}

void Game_Draw()
{
    // Map Drawing
    testMapMg.Draw(gameCam.GetViewRect());
    testMapFg.Draw(gameCam.GetViewRect());
    //testCollision.Draw(gameCam.GetViewRect());

    Bullet_Draw(gameCam.GetViewRect());
    player.Draw(gameCam.GetViewRect());

    Enemy_DrawAll(gameCam.GetViewRect());

    //Effect_Draw();
    testUIManger.Draw();
}

