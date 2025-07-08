/*==============================================================================

　 プレーヤー制御[player.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/06/27
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
// 絵表示したいならこの二つ
#include "sprite.h"
#include "texture.h"
#include "key_logger.h"
#include "bullet.h"
#include "direct3d.h"

#include <DirectXMath.h>
using namespace DirectX;

static constexpr float PLAYER_WIDTH  = 120.0f;
static constexpr float PLAYER_HEIGHT = 132.0f;

static XMFLOAT2 g_PlayerPosition = {};
static XMFLOAT2 g_PlayerVelocity = {};
static XMFLOAT2 g_PlayerSize = {};
static bool g_PlayerFacingLeft; // 左に向かう
static int g_PlayerTexid = -1;
static Circle g_PlayerCollision = { { 60.0f, 66.0f }, 60.0f };
static bool g_PlayerEnable = true;

void Player_Initialize(const XMFLOAT2& position)
{
    g_PlayerPosition = position;
    g_PlayerVelocity = { 0.0f, 0.0f };
    g_PlayerSize = { PLAYER_WIDTH, PLAYER_HEIGHT };
    g_PlayerFacingLeft = true;
    g_PlayerEnable = true;

    g_PlayerTexid = Texture_Load(L"resources/player.png");
}

void Player_Finalize() // 多分いらない
{
}

void Player_Update(double elapsed_time)
{
    if (!g_PlayerEnable) return; // プレーヤーが死んだら戻す

    XMVECTOR position = XMLoadFloat2(&g_PlayerPosition);
    XMVECTOR velocity = XMLoadFloat2(&g_PlayerVelocity);

    XMVECTOR direction = {};

    if (KeyLogger_IsPressed(KK_W)) // 全部ifを使う、方向判断
    {
        direction += {0.0f, -1.0f};
    }
    if (KeyLogger_IsPressed(KK_A))
    {
        direction += {-1.0f, 0.0f};
        g_PlayerFacingLeft = false;
    }
    if (KeyLogger_IsPressed(KK_S))
    {
        direction += {0.0f, 1.0f};
    }
    if (KeyLogger_IsPressed(KK_D))
    {
        direction += {1.0f, 0.0f};
        g_PlayerFacingLeft = true;
    }
    // 同時にAとDを押すと動かない

    direction = XMVector2Normalize(direction); // 単位ベクトルになる

    // position += direction * 3.0f; // 減速の効果がないやり方

    /*
    // 固定フレームのやり方
    velocity += direction * 1.0f;
    position += velocity;
    velocity *= 0.9f;
    */

    // 物理シミュレーションのやり方、どんなフレームでも同じ速度が出る
    velocity += direction * 6000000.0f / 2500.0f * elapsed_time; // 力(単位：ニュートン)、600万ニュートンの力 / 2500の重さ（単位：キロ）
    position += velocity * elapsed_time; // 演算過程、elapsed_timeを使って積分する
    velocity += -velocity * 4.0f * elapsed_time; // 止まる効果

    XMStoreFloat2(&g_PlayerPosition, position); // 演算結果戻す
    XMStoreFloat2(&g_PlayerVelocity, velocity);

    /*
    // 画面の範囲で囲まれてる
    if (g_PlayerPosition.x <= 0.0f)
    {
        g_PlayerPosition.x = 0.0f;
    }
    if (g_PlayerPosition.x >= (Direct3D_GetBackBufferWidth() - g_PlayerSize.x))
    {
        g_PlayerPosition.x = Direct3D_GetBackBufferWidth() - g_PlayerSize.x;
    }
    if (g_PlayerPosition.y <= 0.0f)
    {
        g_PlayerPosition.y = 0.0f;
    }
    if (g_PlayerPosition.y >= (Direct3D_GetBackBufferHeight() - g_PlayerSize.y))
    {
        g_PlayerPosition.y = Direct3D_GetBackBufferHeight() - g_PlayerSize.y;
    }
    */

    // 弾を発射する
    if (KeyLogger_IsTrigger(KK_SPACE))
    {
        Bullet_Create({ g_PlayerPosition.x + g_PlayerSize.x * 0.5f, g_PlayerPosition.y + g_PlayerSize.y * 0.5f }, g_PlayerFacingLeft);
    }
}

void Player_Draw()
{
    if (!g_PlayerEnable) return;

    Sprite_Draw(g_PlayerTexid, g_PlayerPosition.x, g_PlayerPosition.y, g_PlayerSize.x, g_PlayerSize.y, g_PlayerFacingLeft);
}

void Player_Load()
{
}

bool Player_IsEnable()
{
    return g_PlayerEnable;
}

Circle Player_GetCollision()
{
    float cx = g_PlayerPosition.x + g_PlayerCollision.center.x;
    float cy = g_PlayerPosition.y + g_PlayerCollision.center.y;

    return { { cx, cy }, g_PlayerCollision.radius };
}

XMFLOAT2 Player_GetPosition()
{
    return g_PlayerPosition;
}

void Player_Destroy()
{
    g_PlayerEnable = false;
}
