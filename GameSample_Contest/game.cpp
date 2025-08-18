// ==========================================================================================
// 
// File Name: game.cpp
// Date: 2025/08/19
// Author: Gu Anyi
// Description: Manage the game loop
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
    //Bullet_Initialize();
    //Enemy_Initialize();
    //EnemySpawner_Initialize();
    //Effect_Initialize();

    // Enemyê∂ê¨äÌ
    EnemySpawner_Create({ (float)Direct3D_GetBackBufferWidth(), 200.0f }, ENEMY_TYPE_01, 4.0f, 2.0, 8);
    EnemySpawner_Create({                                 0.0f, 400.0f }, ENEMY_TYPE_02, 3.0f, 1.0, 4);

	BG_Initialize();
}

void Game_Finalize()
{
	BG_Finalize();
    //Effect_Finalize();
    //EnemySpawner_Finalize();
    //Enemy_Finalize();
    //Bullet_Finalize();
    testPlayer.Finalize();

    testCam.Finalize();
    testCollision.Finalize();
    testMapFg.Finalize();
    testMapMg.Finalize();
}

void Game_Update(double elapsed_time)
{
    BG_Update(elapsed_time);
    //EnemySpawner_Update(elapsed_time);
    //Bullet_Update(elapsed_time);
    //Enemy_Update(elapsed_time);

    testPlayer.UpdatePosition(elapsed_time, testCollision, testCam.GetViewRect());
    testPlayer.UpdateStatus();
    testPlayer.SetScreenPosition(testCam.GetViewRect()); // to screen space

    testCam.Update(testPlayer.GetWorldPosition());

    hitJudgementBulletVSEnemy();
    hitJudgementPlayerVSEnemy();

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

    //Bullet_Draw();
    testPlayer.Draw();

    //Enemy_Draw();
    //Effect_Draw();
}

void hitJudgementBulletVSEnemy()
{
    for (int bi = 0; bi < BULLETS_MAX; bi++)
    {
        if (!Bullet_IsEnable(bi)) continue;
        
        for (int ei = 0; ei < ENEMIES_MAX; ei++)
        {
            if (!Enemy_IsEnable(ei)) continue;
            if (Collision_CheckCircle(
                Bullet_GetCollision(bi),
                Enemy_GetCollision(ei)
            ))
            {
                // ÉqÉbÉgÇ≥ÇÍÇΩÇÁ
                Bullet_Destroy(bi);
                Enemy_Damage(ei);
            } 
        }
    }
}

void hitJudgementPlayerVSEnemy()
{
    if (!testPlayer.GetIsEnable()) return;

    for (int ei = 0; ei < ENEMIES_MAX; ei++)
    {
        if (!Enemy_IsEnable(ei)) continue;

        if (Collision_CheckCircle(
            testPlayer.GetCircleCollision(),
            Enemy_GetCollision(ei)
        )) // ÉqÉbÉgÇ≥ÇÍÇΩÇÁ
        {
            testPlayer.Destroy();
            Enemy_Destroy(ei);
        }
    }
}


