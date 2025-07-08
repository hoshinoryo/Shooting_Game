/*==============================================================================

�@ �e�̐��� [bullet.h]
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

void Bullet_Create(const DirectX::XMFLOAT2& position, bool isFlipX = false); // �o���b�g��ł�

bool Bullet_IsEnable(int index); // �e�̗L������
Circle Bullet_GetCollision(int index);
void Bullet_Destroy(int index);

#endif // BULLET_H
