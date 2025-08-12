// ==========================================================================================
// 
// File Name: player.cpp
// Date: 2025/08/13
// Author: Gu Anyi
// Description: Manage the player character
// 
// ==========================================================================================
#include "player.h"
#include "sprite.h"
#include "texture.h"
#include "key_logger.h"
#include "bullet.h"
#include "direct3d.h"
#include "debug_text.h"

#include <DirectXMath.h>


using namespace DirectX;

Player::Player()
{
    playerPosition = {};
    playerVelocity = {};
    playerSize = {};
    playerFlip = false;
    playerTexId = -1;
    playerCollision = { { 64.0f, 64.0f }, 64.0f };
    playerEnable = true;
}

void Player::Initialize(const XMFLOAT2& position)
{
    playerPosition = position;
    playerVelocity = { 0.0f, 0.0f };
    playerSize = { 128.0f, 128.0f };
    playerFlip = false;
    playerEnable = true;

    playerTexId = Texture_Load(L"resources/Santa_Claus.png");
}

void Player::Finalize()
{
}

void Player::Update(double elapsed_time)
{
    if (!playerEnable) return; // プレーヤーが死んだら戻す

    XMVECTOR position = XMLoadFloat2(&playerPosition);
    XMVECTOR velocity = XMLoadFloat2(&playerVelocity);

    XMVECTOR direction = {};

    if (KeyLogger_IsPressed(KK_W)) //方向判断
    {
        direction += {0.0f, -1.0f};
    }
    if (KeyLogger_IsPressed(KK_A))
    {
        direction += {-1.0f, 0.0f};
        playerFlip = true;
    }
    if (KeyLogger_IsPressed(KK_S))
    {
        direction += {0.0f, 1.0f};
    }
    if (KeyLogger_IsPressed(KK_D))
    {
        direction += {1.0f, 0.0f};
        playerFlip = false;
    }

    direction = XMVector2Normalize(direction); // 単位ベクトル

    velocity += direction * 6000000.0f / 2500.0f * elapsed_time; // 力(単位：ニュートン)、600万ニュートンの力 / 2500の重さ（単位：キロ）
    position += velocity * elapsed_time; // 演算過程、elapsed_timeを使って積分する
    velocity += -velocity * 4.0f * elapsed_time; // 止まる効果

    XMStoreFloat2(&playerPosition, position);
    XMStoreFloat2(&playerVelocity, velocity);

    // マップの範囲：1920 * 3200
    if (playerPosition.x <= 0.0f)
    {
        playerPosition.x = 0.0f;
    }
    if (playerPosition.x >= (3200.0f - playerSize.x))
    {
        playerPosition.x = 3200.0f - playerSize.x;
    }
    if (playerPosition.y <= 0.0f)
    {
        playerPosition.y = 0.0f;
    }
    if (playerPosition.y >= (1920.0f - playerSize.y))
    {
        playerPosition.y = 1920.0f - playerSize.y;
    }

    // 弾を発射する
    if (KeyLogger_IsTrigger(KK_SPACE))
    {
        Bullet_Create({ playerPosition.x + playerSize.x * 0.5f, playerPosition.y + playerSize.y * 0.5f }, playerFlip);
    }
}

void Player::Draw(const ViewRect& viewRect)
{
    if (!playerEnable) return;
    
    // NOTICE: IN THE SCREEN SPACE!!!
    Sprite_Draw(
        playerTexId,
        playerPosition.x - viewRect.rectPosition.x, playerPosition.y - viewRect.rectPosition.y,
        0.0f, 128.0f,
        playerSize.x, playerSize.y,
        playerFlip
    );
}

void Player::Load()
{
}

bool Player::IsEnable()
{
    return playerEnable;
}

Circle Player::GetCollision()
{
    float cx = playerPosition.x + playerCollision.center.x;
    float cy = playerPosition.y + playerCollision.center.y;

    return { { cx, cy }, playerCollision.radius };
}

XMFLOAT2 Player::GetPosition()
{
    return playerPosition;
}

DirectX::XMFLOAT2 Player::GetSize()
{
    return playerSize;
}

void Player::Destroy()
{
    playerEnable = false;
}
