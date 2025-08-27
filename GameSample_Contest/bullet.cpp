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
    XMFLOAT2 worldPosition;
    XMFLOAT2 screenPosition;
    XMFLOAT2 velocity;
    XMFLOAT2 size;
    double life_time;
    bool isEnable;
    Circle collision; // world coordinate
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

        XMVECTOR position = XMLoadFloat2(&b.worldPosition);
        XMVECTOR velocity = XMLoadFloat2(&b.velocity);

        position += velocity * elapsed_time;

        XMStoreFloat2(&b.worldPosition, position);
        XMStoreFloat2(&b.velocity, velocity);

        b.life_time += elapsed_time;
        b.collision.center.x = b.size.x * 0.5f;
        b.collision.center.y = b.size.y * 0.5f;

        // Žõ–½‚ª’´‚¦‚½‚çÁ‚¦‚é
        if (b.life_time > 5.0f)
        {
            b.isEnable = false;
        }

        // ‰æ–Ê‚ÌŠOo‚é‚Ì‚àÁ‚¦‚é
        if (b.worldPosition.x > Direct3D_GetBackBufferWidth())
        {
            b.isEnable = false;
        }
    }
}

void Bullet_Draw(const ViewRect& viewRect)
{
    for (int i = 0; i < BULLETS_MAX; i++)
    {
        Bullet& b = g_Bullets[i];

        if (!b.isEnable) continue;

        Bullet_SetScreenPosition(viewRect);

        Sprite_Draw(bulletTex, b.screenPosition.x, b.screenPosition.y, b.size.x, b.size.y);

#if defined(DEBUG) || defined(_DEBUG)

        Circle c = Bullet_GetCollision(i);
        c.center.x -= viewRect.rectPosition.x;
        c.center.y -= viewRect.rectPosition.y;
        Collision_DebugDraw(c, {1.0f, 0.0f, 0.0f, 1.0f});

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
        b.worldPosition.x = position.x - b.size.x * 0.5f;
        b.worldPosition.y = position.y - b.size.y * 0.5f;
        b.velocity  = { BULLET_SPEED * dir.x , BULLET_SPEED * dir.y};
        b.collision = { { b.worldPosition.x + b.size.x * 0.5f, b.worldPosition.y + b.size.y * 0.5f }, 14.0f };

        break;
    }
}

bool Bullet_IsEnable(int index)
{
    return g_Bullets[index].isEnable;
}

Circle Bullet_GetCollision(int index)
{
    float cx = g_Bullets[index].collision.center.x + g_Bullets[index].worldPosition.x;
    float cy = g_Bullets[index].collision.center.y + g_Bullets[index].worldPosition.y;

    return { {cx, cy}, g_Bullets[index].collision.radius };
}

void Bullet_Destroy(int index)
{
    g_Bullets[index].isEnable = false;
}

void Bullet_SetScreenPosition(const ViewRect& viewRect)
{
    for (Bullet& b : g_Bullets)
    {
        b.screenPosition.x = b.worldPosition.x - viewRect.rectPosition.x;
        b.screenPosition.y = b.worldPosition.y - viewRect.rectPosition.y;
    }
}
