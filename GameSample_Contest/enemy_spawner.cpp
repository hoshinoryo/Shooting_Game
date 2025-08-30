// ==========================================================================================
// 
// File Name: enemy_spawner.cpp
// Date: 2025/08/27
// Author: Gu Anyi
// Description: Initialize the enemy
// 
// ==========================================================================================

#include "enemy_spawner.h"

#include <DirectXMath.h>

using namespace DirectX;


struct EnemySpawn
{
    XMFLOAT2 positionMin;
    XMFLOAT2 positionMax;
    EnemyTypeID id;
    int count;
    double time; // î≠ê∂äÌé¿çséûä‘
    double rate; // ê∂ê¨ä‘äu
    double spawn_time; // éüÇ…ê∂ê¨Ç∑ÇÈó\íËÇÃéûä‘
    int spawn_count;
    bool isFinished;
};

static constexpr unsigned int ENEMY_SPAWNER_MAX = 50;
static EnemySpawn g_EnemySpawners[ENEMY_SPAWNER_MAX] = {};
static int g_SpawnerCount = 0;
static double g_Time = 0.0f;

void EnemySpawner_Initialize()
{
    g_SpawnerCount = 0;
    g_Time = 0.0;
}

void EnemySpawner_Finalize()
{
    for (int i = 0; i < ENEMIES_MAX; i++)
    {
        g_Enemies[i].Finalize();
    }

    g_SpawnerCount = 0;
    g_Time = 0.0;
}

void EnemySpawner_Update(double elapsed_time)
{
    g_Time += elapsed_time;

    for (int i = 0; i < g_SpawnerCount; i++)
    {
        if (g_EnemySpawners[i].isFinished) continue;

        if (g_EnemySpawners[i].time > g_Time) break;

        if (g_EnemySpawners[i].spawn_count == 0)
        {
            g_EnemySpawners[i].spawn_time = g_Time - g_EnemySpawners[i].rate - 0.00001;
        }

        if ((g_Time - g_EnemySpawners[i].spawn_time) >= g_EnemySpawners[i].rate)
        {
            float x = g_EnemySpawners[i].positionMin.x +
                static_cast<float>(rand()) / RAND_MAX * (g_EnemySpawners[i].positionMax.x - g_EnemySpawners[i].positionMax.x);
            float y = g_EnemySpawners[i].positionMin.y +
                static_cast<float>(rand()) / RAND_MAX * (g_EnemySpawners[i].positionMax.y - g_EnemySpawners[i].positionMax.y);

            Enemy_Create(g_EnemySpawners[i].id, { x, y });
            g_EnemySpawners[i].spawn_count++;

            if (g_EnemySpawners[i].spawn_count >= g_EnemySpawners[i].count)
            {
                g_EnemySpawners[i].isFinished = true;
            }
            g_EnemySpawners[i].spawn_time = g_Time;
        }
    }
}

void EnemySpawner_Create(const XMFLOAT2& posMin, const XMFLOAT2& posMax,
    EnemyTypeID id, double spawn_time, double spawn_rate, int spawn_count)
{
    if (g_SpawnerCount >= ENEMY_SPAWNER_MAX) return;

    EnemySpawn* pEs = &g_EnemySpawners[g_SpawnerCount];
    pEs->positionMin = posMin;
    pEs->positionMax = posMax;
    pEs->id = id;
    pEs->time = spawn_time;
    pEs->rate = spawn_rate;
    pEs->count = spawn_count;
    pEs->isFinished = false;
    pEs->spawn_count = 0;
    pEs->spawn_time = 0.0;
    g_SpawnerCount++;
}

bool EnemySpawner_IsFinishedAll()
{
    for (int i = 0; i < g_SpawnerCount; i++)
    {
        if (!g_EnemySpawners[i].isFinished)
        {
            return false;
        }
    }
    return true;
}
