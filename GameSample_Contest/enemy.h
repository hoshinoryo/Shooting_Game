// ==========================================================================================
// 
// File Name: enemy.h
// Date: 2025/08/27
// Author: Gu Anyi
// Description: Enemy header file
// 
// ==========================================================================================

#ifndef ENEMY_H
#define ENEMY_H

#include "collision.h"
#include "map.h"
#include "camera.h"

#include <DirectXMath.h>

constexpr int ENEMIES_MAX = 256;

enum EnemyTypeID : int
{
    ENEMY_TYPE_01,
    ENEMY_TYPE_02,
    ENEMY_TYPE_MAX
};

class Enemy
{
private:

    EnemyTypeID typeId;
    DirectX::XMFLOAT2 enemyWorldPosition;
    DirectX::XMFLOAT2 enemyScreenPosition;
    DirectX::XMFLOAT2 enemyVelocity;
    DirectX::XMFLOAT2 enemySize;
    double lifeTime;
    int enemyHp;
    Texture enemyTex;
    int enemyAnimPlayId;
    Circle enemyCircleCollision;
    Box enemyBoxCollision;
    bool isEnable;
    bool isDamaged;
    bool isFlipX;
    float damagedTimer;

public:

    Enemy();
    void Initialize(EnemyTypeID id, const DirectX::XMFLOAT2& pos);
    void Finalize();

    void Update(double elapsed_time, const DirectX::XMFLOAT2& playerPos, Collision_Map& map);
    void Draw(const ViewRect& viewRect);
    void Damage();
    void Destroy();

    void SetIsEnable(bool enable);
    bool GetIsEnable();

    Circle GetCircleCollision();
    Box GetBoxCollision();

    DirectX::XMFLOAT2 GetWorldPosition();
    DirectX::XMFLOAT2 GetScreenPosition();
    void SetScreenPosition(const ViewRect& viewRect);

    int GetHp();
};


extern Enemy g_Enemies[ENEMIES_MAX]; // Global enemy array

// Global methods
void Enemy_Create(EnemyTypeID typeId, const DirectX::XMFLOAT2& position);
void Enemy_UpdateAll(double elapsed_time, const DirectX::XMFLOAT2& playerPos, Collision_Map& map);
void Enemy_DrawAll(const ViewRect& viewRect);

#endif // ENEMY_H