/*==============================================================================

�@ �G�̔������� [enemy_spawner.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/07/02
--------------------------------------------------------------------------------

==============================================================================*/

// Enemy������A����������ʂ̊O�Ŕ���

#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "enemy.h"
#include <DirectXMath.h>

void EnemySpawner_Initialize();
void EnemySpawner_Finalize();

void EnemySpawner_Update(double elapsed_time);

void EnemySpawner_Create(const DirectX::XMFLOAT2& position,
    EnemyTypeID id, double spawn_time, double spawn_rate, int spawn_count); // 

#endif ENEMY_SPAWNER_H