/*==============================================================================

　 敵の制御 [enemy.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/07/02
--------------------------------------------------------------------------------

==============================================================================*/
#include "enemy.h"
#include "texture.h"
#include "sprite.h"
#include "direct3d.h"
#include "effect.h"
#include <DirectXMath.h>

using namespace DirectX;

// EnemyTypeは種類の設計図で、Enemyは実際に生成された敵
// EnemyはtypeIdを使ってEnemyTypeと紐づき、種類ごとのテクスチャや当たり判定などを取得する

struct EnemyType // 敵の種類
{
    int texId;
    bool isFlipX;

    XMFLOAT2 velocity; // 速度
    Circle collision;  // 敵のコリジョン
    int hp;            // 最大hp
};

struct Enemy // 敵個体
{
    int typeId;      // EnemyTypeとつながる引数
    XMFLOAT2 position;
    float offsetY;
    double lifeTime; // 生きてる時間の計算
    int hp;          // hp状態
    bool isEnable;
    bool isDamaged;
    bool isFacingLeft;
};

static constexpr float ENEMY_WIDTH = 96.0f;
static constexpr float ENEMY_HEIGHT = 132.0f;

static Enemy g_Enemies[ENEMIES_MAX] = {};
static int g_EnemyTexid = -1;

static EnemyType g_EnemyType[] = { 
    {-1, 1, { -200.0f, 0.0f }, { { ENEMY_WIDTH * 0.5f, ENEMY_HEIGHT * 0.5f }, ENEMY_WIDTH * 0.5f }, 2 },
    {-1, 0, {  200.0f, 0.0f }, { { ENEMY_WIDTH * 0.5f, ENEMY_HEIGHT * 0.5f }, ENEMY_WIDTH * 0.5f }, 2 }
}; // enemyタイプごとにコリジョンのサイズ


void Enemy_Initialize()
{
    for (Enemy& e : g_Enemies)
    {
        e.isEnable = false;
    }

    g_EnemyType[0].texId = Texture_Load(L"resources/enemy_01.png");
    g_EnemyType[1].texId = Texture_Load(L"resources/enemy_02.png");
}

void Enemy_Finalize()
{
}

void Enemy_Update(double elapsed_time)
{
    for (Enemy& e : g_Enemies) // bulletの生成とそっくり
    {
        if (!e.isEnable) continue;

        switch (e.typeId)
        {
        case ENEMY_TYPE_01:
            XMVECTOR position = XMLoadFloat2(&e.position);
            XMVECTOR velocity = XMLoadFloat2(&g_EnemyType[e.typeId].velocity);

            position += velocity * elapsed_time;

            XMStoreFloat2(&e.position, position);
            XMStoreFloat2(&g_EnemyType[e.typeId].velocity, velocity);
            break;

        case ENEMY_TYPE_02:
            e.position.x += g_EnemyType[e.typeId].velocity.x * elapsed_time;
            e.position.y = e.offsetY + sinf(e.lifeTime * 2) * 200.0f;
            break;
        }

        e.lifeTime += elapsed_time;

        // 画面の外出ると消える
        if (e.position.x +  ENEMY_WIDTH < 0.0f)
        {
            e.isEnable = false;
        }
    }
}

void Enemy_Draw()
{
    for (Enemy& e : g_Enemies)
    {
        if (!e.isEnable) continue;

        Sprite_Draw(g_EnemyType[e.typeId].texId, e.position.x, e.position.y, ENEMY_WIDTH, ENEMY_HEIGHT, e.isFacingLeft,
            e.isDamaged ? XMFLOAT4{1.0f, 1.0f, 0.0f, 1.0f} : XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f});
        e.isDamaged = false;
    }
}

void Enemy_Create(EnemyTypeID enemyType, const DirectX::XMFLOAT2& position)
{
    for (Enemy& e : g_Enemies)
    {
        if (e.isEnable) continue;

        // 空き領域発見
        e.isEnable  = true;
        e.isDamaged = false;
        e.typeId = enemyType;
        e.offsetY = position.y;
        e.position = position;
        e.lifeTime = 0.0;
        e.hp = g_EnemyType[e.typeId].hp;
        e.isFacingLeft = g_EnemyType[e.typeId].isFlipX;

        break;
    }
}

bool Enemy_IsEnable(int index)
{
    return g_Enemies[index].isEnable;
}

Circle Enemy_GetCollision(int index)
{
    int id = g_Enemies[index].typeId;
    float cx = g_Enemies[index].position.x + g_EnemyType[id].collision.center.x;
    float cy = g_Enemies[index].position.y + g_EnemyType[id].collision.center.y;
    return { {cx, cy }, g_EnemyType[id].collision.radius };
}


void Enemy_Destroy(int index)
{
    Effect_Create({
        g_Enemies[index].position.x + ENEMY_WIDTH  * 0.5f - 70.0f,
        g_Enemies[index].position.y + ENEMY_HEIGHT * 0.5f - 70.0f
        });
    g_Enemies[index].isEnable = false;
}

void Enemy_Damage(int index)
{
    g_Enemies[index].isDamaged = true;
    g_Enemies[index].hp--;

    if (g_Enemies[index].hp <= 0)
    {
        Enemy_Destroy(index);
    }
}
