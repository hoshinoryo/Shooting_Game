// ==========================================================================================
// 
// File Name: enemy_spawner.h
// Date: 2025/08/27
// Author: Gu Anyi
// Description: Enemy spawner header file
// 
// ==========================================================================================

#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "enemy.h"
#include <DirectXMath.h>

void EnemySpawner_Initialize();
void EnemySpawner_Finalize();

void EnemySpawner_Update(double elapsed_time);

void EnemySpawner_Create(const DirectX::XMFLOAT2& posMin, const DirectX::XMFLOAT2& posMax,
    EnemyTypeID id, double spawn_time, double spawn_rate, int spawn_count);

bool EnemySpawner_IsFinishedAll();

#endif ENEMY_SPAWNER_H