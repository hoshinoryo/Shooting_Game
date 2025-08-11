/*==============================================================================

　 ゲーム本体[game.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/06/27
--------------------------------------------------------------------------------

==============================================================================*/
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

#include "debug_text.h"
#include "debug_ostream.h"

#include <iostream>

Map testMap;
Camera testCam;


void Game_Initialize()
{
    testMap.Initialize();
    testCam.Initialize(Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
        (float)testMap.GetMapWidth(), (float)testMap.GetMapHeight());

    Player_Initialize({ Direct3D_GetBackBufferWidth() * 0.5f, Direct3D_GetBackBufferHeight() * 0.5f});
    Bullet_Initialize();
    //Enemy_Initialize();
    //EnemySpawner_Initialize();
    Effect_Initialize();

    // Enemy生成器
    EnemySpawner_Create({ (float)Direct3D_GetBackBufferWidth(), 200.0f }, ENEMY_TYPE_01, 4.0f, 2.0, 8);
    EnemySpawner_Create({                                 0.0f, 400.0f }, ENEMY_TYPE_02, 3.0f, 1.0, 4);

	BG_Initialize();
}

void Game_Finalize()
{
	BG_Finalize();
    Effect_Finalize();
    //EnemySpawner_Finalize();
    //Enemy_Finalize();
    Bullet_Finalize();
    Player_Finalize();

    testCam.Finalize();
    testMap.Finalize();
}

void Game_Update(double elapsed_time)
{
    BG_Update(elapsed_time);
    //EnemySpawner_Update(elapsed_time);
    Bullet_Update(elapsed_time);
    //Enemy_Update(elapsed_time);

    Player_Update(elapsed_time);
    testCam.Update(Player_GetPosition());

    //hal::dout << "Camera Position: (" << testCam.GetX() << ", " << testCam.GetY() << ")" << std::endl;
    
    hitJudgementBulletVSEnemy();
    hitJudgementPlayerVSEnemy();

    Effect_Update(elapsed_time);
}

void Game_Draw()
{
	BG_Draw();
    testMap.Draw(testCam.GetViewRect());

    Bullet_Draw();
    Player_Draw();
    //Enemy_Draw();
    Effect_Draw();
}

void hitJudgementBulletVSEnemy()
{
    for (int bi = 0; bi < BULLETS_MAX; bi++)
    {
        if (!Bullet_IsEnable(bi)) continue;
        
        for (int ei = 0; ei < ENEMIES_MAX; ei++)
        {
            if (!Enemy_IsEnable(ei)) continue;
            if (Collision_OverlapCircle(
                Bullet_GetCollision(bi),
                Enemy_GetCollision(ei)
            ))
            {
                // ヒットされたら
                Bullet_Destroy(bi);
                Enemy_Damage(ei);
            } 
        }
    }
}

void hitJudgementPlayerVSEnemy()
{
    if (!Player_IsEnable()) return;

    for (int ei = 0; ei < ENEMIES_MAX; ei++)
    {
        if (!Enemy_IsEnable(ei)) continue;

        if (Collision_OverlapCircle(
            Player_GetCollision(),
            Enemy_GetCollision(ei)
        )) // ヒットされたら
        {
            Player_Destroy();
            Enemy_Destroy(ei);
        }
    }
}


