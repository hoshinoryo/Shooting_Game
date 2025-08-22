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
#include "background.h"
#include "camera.h"
#include "map.h"
#include "check_collision.h"
#include "fade.h"

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

Player testPlayer;
Camera testCam;

static bool g_GameStart = false;


void Game_Initialize()
{   
    // Map Initialization
    testMapMg.Initialize(mg_filePath);
    testMapFg.Initialize(fg_filePath);
    testCollision.Initialize(col_filePath);

    // Camera Initialization
    testCam.Initialize(
        Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
        (float)testMapMg.GetMapWidth(), (float)testMapMg.GetMapHeight()
    );

    testPlayer.Initialize({ Direct3D_GetBackBufferWidth() * 0.5f, Direct3D_GetBackBufferHeight() * 0.5f});
    Bullet_Initialize();
    //Enemy_Initialize();
    //EnemySpawner_Initialize();
    //Effect_Initialize();

    // Enemyê∂ê¨äÌ
    //EnemySpawner_Create({ (float)Direct3D_GetBackBufferWidth(), 200.0f }, ENEMY_TYPE_01, 4.0f, 2.0, 8);
    //EnemySpawner_Create({                                 0.0f, 400.0f }, ENEMY_TYPE_02, 3.0f, 1.0, 4);

	BG_Initialize();

    Fade_Start(0.8f, false);
    g_GameStart = false;
}

void Game_Finalize()
{
	BG_Finalize();
    //Effect_Finalize();
    //EnemySpawner_Finalize();
    //Enemy_Finalize();
    Bullet_Finalize();
    testPlayer.Finalize();

    testCam.Finalize();
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

    BG_Update(elapsed_time);
    //EnemySpawner_Update(elapsed_time);
    //Enemy_Update(elapsed_time);

    testPlayer.Update(elapsed_time, testCollision, testCam.GetViewRect());
    Bullet_Update(elapsed_time);

    testCam.Update(testPlayer.GetWorldPosition());

    CheckCollision_BulletVSEnemy();
    CheckCollision_PlayerVSEnemy(testPlayer);

    //Effect_Update(elapsed_time);

//#if defined(DEBUG) || defined(_DEBUG)
//
//    hal::dout << "Player position: " << testPlayer.GetPosition().x << ", " << testPlayer.GetPosition().y << std::endl;
//    hal::dout << "Camera Position: " << testCam.GetX() << ", " << testCam.GetY() << std::endl;
//    hal::dout << "Delta: " << (testPlayer.GetPosition().x - testCam.GetX() - Direct3D_GetBackBufferWidth() * 0.5f) << std::endl;
//
//#endif
}

void Game_Draw()
{
	BG_Draw();

    // Map Draw
    testMapMg.Draw(testCam.GetViewRect());
    testMapFg.Draw(testCam.GetViewRect());
    //testCollision.Draw(testCam.GetViewRect());

    Bullet_Draw();
    testPlayer.Draw();

    //Enemy_Draw();
    //Effect_Draw();
}

