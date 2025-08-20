// ==========================================================================================
// 
// File Name: bullet.cpp
// Date: 2025/08/19
// Author: Gu Anyi
// Description: Manage the bullet generation and control
// 
// ==========================================================================================
#include "bullet.h"
#include "player.h"
#include "direct3d.h"
#include "sprite.h"
#include "texture.h"
#include "key_logger.h"
#include <DirectXMath.h>

using namespace DirectX;

static constexpr float BULLET_SPEED = 500.0f;

struct Bullet
{
    XMFLOAT2 position;
    XMFLOAT2 velocity;
    XMFLOAT2 size;
    double life_time;
    bool isEnable;
    Circle collision;
};

static Bullet g_Bullets[BULLETS_MAX] = {};
static Texture bulletTex;

void Bullet_Initialize()
{
    bulletTex.Initialize(Direct3D_GetDevice(), L"resources/present.png");

    for (Bullet& b : g_Bullets)
    {
        b.isEnable = false;
    }
}

void Bullet_Finalize()
{
    bulletTex.Finalize();
}

void Bullet_Update(double elapsed_time)
{
    for (Bullet& b : g_Bullets)
    {
        if (!b.isEnable) continue;

        XMVECTOR position = XMLoadFloat2(&b.position);
        XMVECTOR velocity = XMLoadFloat2(&b.velocity);

        position += velocity * elapsed_time;

        XMStoreFloat2(&b.position, position);
        XMStoreFloat2(&b.velocity, velocity);

        b.life_time += elapsed_time;
        b.collision.center.x = b.position.x + b.size.x * 0.5f;
        b.collision.center.y = b.position.y + b.size.y * 0.5f;

        // Žõ–½‚ª’´‚¦‚½‚çÁ‚¦‚é
        if (b.life_time > 5.0f)
        {
            b.isEnable = false;
        }

        // ‰æ–Ê‚ÌŠOo‚é‚Ì‚àÁ‚¦‚é
        if (b.position.x > Direct3D_GetBackBufferWidth())
        {
            b.isEnable = false;
        }
    }
}

void Bullet_Draw()
{
    for (Bullet& b : g_Bullets)
    {
        if (!b.isEnable) continue;

        Sprite_Draw(bulletTex, b.position.x, b.position.y, b.size.x, b.size.y);

#if defined(DEBUG) || defined(_DEBUG)

        Collision_DebugDraw(b.collision, {1.0f, 0.0f, 0.0f, 1.0f});

#endif
    }
}

void Bullet_Create(const XMFLOAT2& position, const XMFLOAT2 dir, bool isFlipX)
{
    for (Bullet& b : g_Bullets)
    {
        if (b.isEnable) continue;

        b.isEnable  = true;
        b.life_time = 0.0;
        b.size.x = bulletTex.GetWidth();
        b.size.y = bulletTex.GetHeight();
        b.position.x = position.x - b.size.x * 0.5f;
        b.position.y = position.y - b.size.y * 0.5f;
        b.velocity  = { BULLET_SPEED * dir.x , BULLET_SPEED * dir.y};
        b.collision = { { b.position.x + b.size.x * 0.5f, b.position.y + b.size.y * 0.5f }, 14.0f };

        break;
    }
}

bool Bullet_IsEnable(int index)
{
    return g_Bullets[index].isEnable;
}

Circle Bullet_GetCollision(int index)
{
    float cx = g_Bullets[index].collision.center.x + g_Bullets[index].position.x;
    float cy = g_Bullets[index].collision.center.y + g_Bullets[index].position.y;
    return { {cx, cy}, g_Bullets[index].collision.radius };
}

void Bullet_Destroy(int index)
{
    g_Bullets[index].isEnable = false;
}
