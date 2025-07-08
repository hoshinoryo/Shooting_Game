/*==============================================================================

　 弾の制御 [bullet.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/07/01
--------------------------------------------------------------------------------

==============================================================================*/

#ifndef BULLET_H
#define BULLET_H

#include "collision.h"
#include <DirectXMath.h>


static constexpr int BULLETS_MAX = 1024;

void Bullet_Initialize();
void Bullet_Finalize();

void Bullet_Update(double elapsed_time);
void Bullet_Draw();

void Bullet_Create(const DirectX::XMFLOAT2& position, bool isFlipX = false); // バレットを打つ

bool Bullet_IsEnable(int index); // 弾の有効無効
Circle Bullet_GetCollision(int index);
void Bullet_Destroy(int index);

#endif // BULLET_H
