/*==============================================================================

Å@ ìGÇÃî≠ê∂êßå‰ [enemy_spawner.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/07/02
--------------------------------------------------------------------------------

==============================================================================*/

// Enemyî≠ê∂äÌÅAÇæÇ¢ÇΩÇ¢âÊñ ÇÃäOÇ≈î≠ê∂

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