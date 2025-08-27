// ==========================================================================================
// 
// File Name: bullet.h
// Date: 2025/08/19
// Author: Gu Anyi
// Description: Bullet header file
// 
// ==========================================================================================

#ifndef BULLET_H
#define BULLET_H

#include "collision.h"
#include "camera.h"

#include <DirectXMath.h>


static constexpr int BULLETS_MAX = 1024;

void Bullet_Initialize();
void Bullet_Finalize();

void Bullet_Update(double elapsed_time);
void Bullet_Draw(const ViewRect& viewRect);

void Bullet_Create(const DirectX::XMFLOAT2& position,const DirectX::XMFLOAT2 dir, bool isFlipX = false); // バレットを打つ

bool Bullet_IsEnable(int index); // 弾の有効無効
Circle Bullet_GetCollision(int index);
void Bullet_Destroy(int index);

void Bullet_SetScreenPosition(const ViewRect& viewRect);

#endif // BULLET_H
