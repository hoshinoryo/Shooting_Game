/*==============================================================================

Å@ ìGÇÃêßå‰ [enemy.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/07/02
--------------------------------------------------------------------------------

==============================================================================*/

#ifndef ENEMY_H
#define ENEMY_H
#include "collision.h"

#include <DirectXMath.h>

static constexpr int ENEMIES_MAX = 256;

void Enemy_Initialize();
void Enemy_Finalize();

void Enemy_Update(double elapsed_time);
void Enemy_Draw();

// âΩéÌóﬁÇ™Ç†ÇÈÇ‚ÇËï˚
enum EnemyTypeID : int
{
    ENEMY_TYPE_01,
    ENEMY_TYPE_02,
};

void Enemy_Create(EnemyTypeID enemyType, const DirectX::XMFLOAT2& position);

bool Enemy_IsEnable(int index);

Circle Enemy_GetCollision(int index);
void Enemy_Destroy(int index); // è¡ñ≈

void Enemy_Damage(int index);

#endif // ENEMY_H