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
#include "camera.h"

const float MAP_WIDTH = 2000.0f;
const float MAP_HEIGHT = 2000.0f;


void Game_Initialize()
{
    Player_Initialize({ Direct3D_GetBackBufferWidth() * 0.5f, Direct3D_GetBackBufferHeight() * 0.5f});
    Camera_Initialize();
    Bullet_Initialize();
    Enemy_Initialize();
    EnemySpawner_Initialize();
    Effect_Initialize();

    // Enemy生成器
    EnemySpawner_Create({ (float)Direct3D_GetBackBufferWidth(), 200.0f }, ENEMY_TYPE_01, 4.0f, 2.0, 8);
    EnemySpawner_Create({                                 0.0f, 400.0f }, ENEMY_TYPE_02, 3.0f, 1.0, 4);
}

void Game_Finalize()
{
    Effect_Finalize();
    EnemySpawner_Finalize();
    Enemy_Finalize();
    Bullet_Finalize();
    Camera_Finalize();
    Player_Finalize();
}

void Game_Update(double elapsed_time)
{
    Camera_Update(Player_GetPosition(), (float)Direct3D_GetBackBufferWidth(), (float)Direct3D_GetBackBufferHeight(), MAP_WIDTH, MAP_HEIGHT);
    EnemySpawner_Update(elapsed_time);

    Player_Update(elapsed_time);
    Bullet_Update(elapsed_time);
    Enemy_Update(elapsed_time);

    hitJudgementBulletVSEnemy();
    hitJudgementPlayerVSEnemy();

    Effect_Update(elapsed_time);
}

void Game_Draw()
{
    Camera_Set();
    Bullet_Draw();
    Player_Draw(); // 重なったらごちらから先に描く
    Enemy_Draw();
    Effect_Draw();
}

void hitJudgementBulletVSEnemy()
{
    for (int bi = 0; bi < BULLETS_MAX; bi++)
    {
        if (!Bullet_IsEnable(bi)) continue; //　もし弾は有効
        
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
    if (!Player_IsEnable()) return; //　もしプレーヤーは有効

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


