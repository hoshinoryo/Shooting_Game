// ==========================================================================================
// 
// File Name: enemy.cpp
// Date: 2025/08/27
// Author: Gu Anyi
// Description: Enemy class describtion
// 
// ==========================================================================================

#include "enemy.h"
#include "texture.h"
#include "sprite.h"
#include "sprite_anim.h"
#include "direct3d.h"
#include "effect.h"
#include "check_collision.h"

#include <DirectXMath.h>

using namespace DirectX;


static constexpr float ENEMY_SPEED = 100.0f;

Enemy g_Enemies[ENEMIES_MAX] = {};

struct EnemyConfig // 敵の配置
{
    const wchar_t* texPath;
    int hpMax;
    XMFLOAT2 size;
};

static EnemyConfig g_EnemyConfigs[ENEMY_TYPE_MAX] = {
    { L"resources/Enemy_01.png", 3, { 128.0f, 128.0f } },
    { L"resources/Enemy_02.png", 4, { 128.0f, 128.0f } }
};


Enemy::Enemy()
{
    typeId = ENEMY_TYPE_MAX;
    enemyWorldPosition = {};
    enemyScreenPosition = {};
    enemyVelocity = {};
    enemySize = {};
    lifeTime = 0.0;
    hp = 1;
    enemyAnimPlayId = -1;
    enemyCircleCollision = { { 64.0f, 64.0f }, 28.0f };
    enemyBoxCollision = { { 64.0f, 92.0f }, 24.0f, 10.0f }; // test collision box
    isEnable = false;
    isDamaged = false;
    isFlipX = false;
}

void Enemy::Initialize(EnemyTypeID id, const XMFLOAT2& pos)
{
    typeId = id;
    enemyWorldPosition = pos;
    enemyScreenPosition = {};

    // from EnemyConfig
    hp = g_EnemyConfigs[typeId].hpMax;
    enemySize = g_EnemyConfigs[typeId].size;
    enemyTex.Initialize(Direct3D_GetDevice(), g_EnemyConfigs[typeId].texPath);
    enemyAnimPlayId = SpriteAnim_CreatePlayer(
        SpriteAnim_RegisterPattern(enemyTex, 6, 6, 0.1f, { enemySize.x, enemySize.y },
            { 0.0f, 128.0f }, true)
    );

    lifeTime = 0.0;
    isEnable = true;
    isDamaged = false;
}

void Enemy::Finalize()
{
    enemyTex.Finalize();
}

void Enemy::Update(double elapsed_time, const XMFLOAT2& playerWorldPos, Collision_Map& map)
{
    if (!isEnable) return;

    lifeTime += elapsed_time;

    if (lifeTime >= 30.0f)
    {
        Destroy();
        return;
    }

    // direction vector towards the player
    float dx = playerWorldPos.x - enemyWorldPosition.x;
    float dy = playerWorldPos.y - enemyWorldPosition.y;
    float len = sqrtf(dx * dx + dy * dy);
    if (len > 0.0001f)
    {
        dx /= len;
        dy /= len;
    }

    // trying to move
    XMFLOAT2 tryVel = { dx * ENEMY_SPEED, dy * ENEMY_SPEED };

    XMFLOAT2 oldPos = enemyWorldPosition;
    XMFLOAT2 newPos = oldPos;
    newPos.x += tryVel.x * elapsed_time;
    newPos.y += tryVel.y * elapsed_time;

    Box tryBox = GetBoxCollision();
    tryBox.center.x += tryVel.x * elapsed_time;
    tryBox.center.y += tryVel.y * elapsed_time;

    if (!CheckCollision_BoxVSMap(tryBox, map))
    {
        enemyWorldPosition = newPos;
        enemyVelocity = tryVel;
    }
    else // have collision, first try x axis then try y axis
    {
        XMFLOAT2 velx = { tryVel.x, 0.0f };
        Box boxX = GetBoxCollision();
        boxX.center.x += velx.x * elapsed_time;
        if (!CheckCollision_BoxVSMap(boxX, map))
        {
            enemyWorldPosition.x += velx.x * elapsed_time;
            enemyVelocity = velx;
        }
        else
        {
            XMFLOAT2 vely = { 0.0f, tryVel.y };
            Box boxY = GetBoxCollision();
            boxY.center.y += vely.y * elapsed_time;
            if (!CheckCollision_BoxVSMap(boxY, map))
            {
                enemyWorldPosition.y += vely.y * elapsed_time;
                enemyVelocity = vely;
            }
            else // totally stuck
            {
                enemyVelocity = { 0.0f, 0.0f };
            }
        }
    }

    isFlipX = (dx > 0.0f);

    //SetScreenPosition(viewRect);

    /*
    // 画面の外出ると消える
    if (enemyScreenPosition.x + enemySize.x * 0.5f < 0.0f || enemyScreenPosition.x + enemySize.x * 0.5f > 1600.0f ||
        enemyScreenPosition.y < -enemySize.y || enemyScreenPosition.y > 900.0f)
    {
        isEnable = false;
    }
    */
}

void Enemy::Draw(const ViewRect& viewRect)
{
    if (!isEnable) return;

    SetScreenPosition(viewRect);

    SpriteAnim_Draw(
        enemyAnimPlayId, enemyScreenPosition.x, enemyScreenPosition.y,
        enemySize.x, enemySize.y, isFlipX
    );

    isDamaged = false;

#if defined(DEBUG) || defined(_DEBUG)

    Circle c = GetCircleCollision();
    c.center.x -= viewRect.rectPosition.x;
    c.center.y -= viewRect.rectPosition.y;
    Collision_DebugDraw(c);

    Box b = GetBoxCollision();
    b.center.x -= viewRect.rectPosition.x;
    b.center.y -= viewRect.rectPosition.y;
    Collision_DebugDraw(b);

#endif
}


void Enemy::SetIsEnable(bool enable)
{
    isEnable = enable;
}

bool Enemy::GetIsEnable()
{
    return isEnable;
}

Circle Enemy::GetCircleCollision()
{
    float cx = enemyWorldPosition.x + enemyCircleCollision.center.x;
    float cy = enemyWorldPosition.y + enemyCircleCollision.center.y;

    return { { cx, cy }, enemyCircleCollision.radius };
}

Box Enemy::GetBoxCollision()
{
    return { { enemyWorldPosition.x + enemyBoxCollision.center.x, enemyWorldPosition.y + enemyBoxCollision.center.y },
    enemyBoxCollision.half_width, enemyBoxCollision.half_height };
}

XMFLOAT2 Enemy::GetWorldPosition()
{
    return enemyWorldPosition;
}

XMFLOAT2 Enemy::GetScreenPosition()
{
    return enemyScreenPosition;
}

void Enemy::SetScreenPosition(const ViewRect& viewRect)
{
    enemyScreenPosition.x = enemyWorldPosition.x - viewRect.rectPosition.x;
    enemyScreenPosition.y = enemyWorldPosition.y - viewRect.rectPosition.y;
}

int Enemy::GetHp()
{
    return hp;
}

void Enemy::Damage()
{
    if (lifeTime < 1.0) return;

    isDamaged = true;
    hp--;

    if (hp <= 0)
    {
        Destroy();
    }
}

void Enemy::Destroy()
{
    /*
    Effect_Create({
        enemyWorldPosition.x + enemySize.x * 0.5f,
        enemyWorldPosition.y + enemySize.y * 0.5f
        });
    */
    isEnable = false;
}

void Enemy_Create(EnemyTypeID typeId, const DirectX::XMFLOAT2& position)
{
    for (int i = 0; i < ENEMIES_MAX; i++)
    {
        if (!g_Enemies[i].GetIsEnable())
        {
            g_Enemies[i].Initialize(typeId, position);
            break;
        }
    }
}

void Enemy_UpdateAll(double elapsed_time, const XMFLOAT2& playerPos, Collision_Map& map)
{
    for (int i = 0; i < ENEMIES_MAX; i++)
    {
        if (g_Enemies[i].GetIsEnable())
        {
            g_Enemies[i].Update(elapsed_time, playerPos, map);
        }
    }
}

void Enemy_DrawAll(const ViewRect& viewRect)
{
    for (int i = 0; i < ENEMIES_MAX; i++)
    {
        if (g_Enemies[i].GetIsEnable())
        {
            g_Enemies[i].Draw(viewRect);
        }
    }
}
