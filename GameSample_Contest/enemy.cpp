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
#include "ui_element.h"
#include "render_queue.h"
#include "scene.h"
#include "result.h"

#include <DirectXMath.h>

using namespace DirectX;


static constexpr float ENEMY_SPEED = 150.0f;
static constexpr float HP_SLIDER_HEIGHT = 16.0f;
static constexpr float HP_BAR_WIDTH = 56.0f;
static constexpr float HP_BAR_HEIGHT = 16.0f;
static constexpr XMFLOAT2 HP_SLIDER_OFFSET = { 32.0f, -24.0f };

static Texture HpSliderBGTex;
static Texture HpSliderFGTex;

Enemy g_Enemies[ENEMIES_MAX] = {};
extern ScoreUI scoreUI;

// “G‚Ì”z’u
struct EnemyConfig
{
    const wchar_t* texPath;
    int hpMax;
    XMFLOAT2 size;
    EnemyTargetType targetType;
    XMFLOAT2 targetPos;
};

static EnemyConfig g_EnemyConfigs[ENEMY_TYPE_MAX] = {
    { L"resources/Enemy_01.png", 4, { 128.0f, 128.0f }, FixedPoint, { 1152.0f, 1088.0f } },
    { L"resources/Enemy_02.png", 5, { 128.0f, 128.0f }, PlayerPosition, { 1152.0f, 1088.0f } },
    { L"resources/Enemy_03.png", 3, { 128.0f, 128.0f }, PlayerPosition, { 1152.0f, 1088.0f } }
};


Enemy::Enemy()
{
    typeId = ENEMY_TYPE_MAX;
    enemyWorldPosition = {};
    enemyScreenPosition = {};
    enemyVelocity = {};
    enemySize = {};
    lifeTime = 0.0;
    enemyHp = 1;
    enemyAnimPlayId = -1;
    enemyCircleCollision = { { 64.0f, 64.0f }, 28.0f };
    enemyBoxCollision = { { 64.0f, 92.0f }, 16.0f, 8.0f };
    isEnable = false;
    isDamaged = false;
    isFlipX = false;
    damagedTimer = 0.0f;

    hpSliderWidth = 0.0f;
    targetType = TARGET_NONE;
    targetPos = { 0.0f, 0.0f };
}

void Enemy::Initialize(EnemyTypeID id, const XMFLOAT2& pos)
{
    typeId = id;
    enemyWorldPosition = pos;
    enemyScreenPosition = {};

    // from EnemyConfig
    enemyHp = g_EnemyConfigs[typeId].hpMax;
    enemySize = g_EnemyConfigs[typeId].size;
    enemyTex.Initialize(Direct3D_GetDevice(), g_EnemyConfigs[typeId].texPath);
    enemyAnimPlayId = SpriteAnim_CreatePlayer(
        SpriteAnim_RegisterPattern(enemyTex, 6, 6, 0.1f, { enemySize.x, enemySize.y },
            { 0.0f, 128.0f }, true)
    );
    targetType = g_EnemyConfigs[typeId].targetType;
    targetPos = g_EnemyConfigs[typeId].targetPos;

    lifeTime = 0.0;
    isEnable = true;
    isDamaged = false;
    damagedTimer = 0.0f;

    hpSliderWidth = 64.0f;
    HpSliderBGTex.Initialize(Direct3D_GetDevice(), L"resources/EnemyHP_SliderBG.png");
    HpSliderFGTex.Initialize(Direct3D_GetDevice(), L"resources/EnemyHP_SliderFull.png");
}

void Enemy::Finalize()
{
    SpriteAnim_DestroyPlayer(enemyAnimPlayId);
    enemyAnimPlayId = -1;

    typeId = ENEMY_TYPE_MAX;
    enemyWorldPosition = { 0.0f, 0.0f };
    enemyScreenPosition = { 0.0f, 0.0f };
    enemyHp = 0;
    enemySize = { 0.0f, 0.0f };
    targetType = TARGET_NONE;
    targetPos = { 0.0f, 0.0f };

    lifeTime = 0.0;
    isEnable = false;
    isDamaged = false;
    damagedTimer = 0.0f;

    hpSliderWidth = 0.0f;

    HpSliderFGTex.Finalize();
    HpSliderBGTex.Finalize();
    enemyTex.Finalize();
}

void Enemy::Update(double elapsed_time, const XMFLOAT2& playerPos, Collision_Map& map, const ViewRect& viewRect)
{
    if (!isEnable) return;

    XMFLOAT2 target = GetTargetPosition(playerPos);

    // damaged status
    if (isDamaged)
    {
        damagedTimer -= static_cast<float>(elapsed_time);
        if (damagedTimer <= 0.0f)
        {
            isDamaged = false;
        }
    }

    // lift time
    lifeTime += elapsed_time;

    if (lifeTime >= 60.0f)
    {
        Destroy(ByTimeout);
        return;
    }

    // AI: direction vector towards the player
    float dx = target.x - enemyWorldPosition.x;
    float dy = target.y - enemyWorldPosition.y;
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

    // flip in x axis
    isFlipX = (dx > 0.0f);

    // if arrived at target position
    float reachDict = 5.0f;
    if (targetType == FixedPoint && len <= reachDict)
    {
        Destroy(ByArrivedTarget);
        return;
    }

    RenderQueue::Add(GetBoxCollision().center.y, [this, viewRect]()
        {
            this->Draw(viewRect);
        });
}

void Enemy::Draw(const ViewRect& viewRect)
{
    if (!isEnable) return;

    SetScreenPosition(viewRect);

    XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    if (isDamaged)
    {
        color = { 1.0f, 0.3f, 0.3f, 1.0f };
    }

    SpriteAnim_Draw(
        enemyAnimPlayId, enemyScreenPosition.x, enemyScreenPosition.y,
        enemySize.x, enemySize.y, isFlipX, color
    );

    isDamaged = false;

    // HP bar
    float hpPercent = (float)enemyHp / (float)g_EnemyConfigs[typeId].hpMax;
    if (hpPercent < 0.0f) hpPercent = 0.0f;

    float hpSliderPosX = enemyScreenPosition.x + HP_SLIDER_OFFSET.x;
    float hpSliderPosY = enemyScreenPosition.y + HP_SLIDER_OFFSET.y;

    Sprite_Draw(HpSliderBGTex, hpSliderPosX, hpSliderPosY);
    Sprite_Draw(HpSliderFGTex, hpSliderPosX + 4.0f, hpSliderPosY + 4.0f, HP_BAR_WIDTH * hpPercent, HP_BAR_HEIGHT,
        4.0f, 4.0f, HP_BAR_WIDTH * hpPercent, HP_BAR_HEIGHT);

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

void Enemy::Move(float dx, float dy)
{
    enemyWorldPosition.x += dx;
    enemyWorldPosition.y += dy;
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
    return enemyHp;
}

void Enemy::SetTargetPosition(const DirectX::XMFLOAT2& pos)
{
    targetPos = pos;
}

XMFLOAT2 Enemy::GetTargetPosition(const DirectX::XMFLOAT2& playerPos)
{
    if (targetType == EnemyTargetType::PlayerPosition)
    {
        return playerPos;
    }
    else
    {
        return targetPos;
    }   
}

void Enemy::Damage()
{
    if (lifeTime < 1.0) return;

    enemyHp--;
    isDamaged = true;
    damagedTimer = 0.1f;

    if (enemyHp <= 0)
    {
        Destroy(ByBullet);
    }
}

void Enemy::Destroy(EnemyDeathReason reason)
{
    /*
    Effect_Create({
        enemyWorldPosition.x + enemySize.x * 0.5f,
        enemyWorldPosition.y + enemySize.y * 0.5f
        });
    */
    isEnable = false;

    switch (reason)
    {
    case ByBullet:
        scoreUI.AddScore(100);
        break;

    case ByPlayerCollision:
        break;

    case ByTimeout:
        break;

    case ByArrivedTarget:
        Result_SetScoreAndDigit(scoreUI.GetScore(), scoreUI.GetDigit());
        Scene_Change(SCENE_RESULT);
        break;
    }
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

void Enemy_UpdateAll(double elapsed_time, const XMFLOAT2& targetPos, Collision_Map& map, const ViewRect& viewRect)
{
    for (int i = 0; i < ENEMIES_MAX; i++)
    {
        if (g_Enemies[i].GetIsEnable())
        {
            g_Enemies[i].Update(elapsed_time, targetPos, map, viewRect);
        }
    }

    CheckCollision_EnemyVSEnemy();
}

/*
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
*/

bool Enemy_AreAllCleared()
{
    for (int i = 0; i < ENEMIES_MAX; i++)
    {
        if (g_Enemies[i].GetIsEnable())
        {
            return false;
        }
    }
    return true;
}
