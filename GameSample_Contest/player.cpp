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
#include "sprite_anim.h"
#include "texture.h"
#include "key_logger.h"
#include "bullet.h"
#include "direct3d.h"
#include "debug_text.h"

#include <DirectXMath.h>

using namespace DirectX;

static constexpr float PLAYER_ANIM_PLAY_RATE = 0.08f;


Player::Player()
{
    playerWorldPosition = {};
    playerScreenPosition = {};
    playerVelocity = {};
    playerSize = {};
    playerFlip = false;
    playerTexId = -1;
    playerAnimPlayId = -1;
    playerCircleCollision = { { 64.0f, 92.0f }, 20.0f };
    playerEnable = true;
    playerStatus = none;
}

void Player::Initialize(const XMFLOAT2& position)
{

    playerWorldPosition = position;
    playerScreenPosition = {};
    playerVelocity = { 0.0f, 0.0f };
    playerSize = { 128.0f, 128.0f };
    playerFlip = false;
    playerEnable = true;

    playerTexId = Texture_Load(L"resources/Santa_Claus.png");
}

void Player::Finalize()
{
}

void Player::Update(double elapsed_time, const ViewRect& viewRect)
{
    if (!playerEnable) return; // プレーヤーが死んだら戻す

    XMVECTOR position = XMLoadFloat2(&playerWorldPosition);
    XMVECTOR velocity = XMLoadFloat2(&playerVelocity);

    XMVECTOR direction = {};

    Status newPlayerStatus = stand;

    if (KeyLogger_IsPressed(KK_W)) //方向判断
    {
        direction += {0.0f, -1.0f};
        playerFlip = false;
        newPlayerStatus = walkBack;
    }
    if (KeyLogger_IsPressed(KK_A))
    {
        direction += {-1.0f, 0.0f};
        playerFlip = true;
        newPlayerStatus = walkRight;
    }
    if (KeyLogger_IsPressed(KK_S))
    {
        direction += {0.0f, 1.0f};
        playerFlip = false;
        newPlayerStatus = walkFront;
    }
    if (KeyLogger_IsPressed(KK_D))
    {
        direction += {1.0f, 0.0f};
        playerFlip = false;
        newPlayerStatus = walkLeft;
    }

    ChangeStatus(newPlayerStatus);

    direction = XMVector2Normalize(direction); // 単位ベクトル

    velocity += direction * 6000000.0f / 2500.0f * elapsed_time; // 力(単位：ニュートン)、600万ニュートンの力 / 2500の重さ（単位：キロ）
    position += velocity * elapsed_time;
    velocity += -velocity * 6.0f * elapsed_time;

    XMStoreFloat2(&playerWorldPosition, position);
    XMStoreFloat2(&playerVelocity, velocity);

    // マップの範囲：1920 * 3200
    if (playerWorldPosition.x <= 0.0f)
    {
        playerWorldPosition.x = 0.0f;
    }
    if (playerWorldPosition.x >= (3200.0f - playerSize.x))
    {
        playerWorldPosition.x = 3200.0f - playerSize.x;
    }
    if (playerWorldPosition.y <= 0.0f)
    {
        playerWorldPosition.y = 0.0f;
    }
    if (playerWorldPosition.y >= (1920.0f - playerSize.y))
    {
        playerWorldPosition.y = 1920.0f - playerSize.y;
    }

    SetScreenPosition(viewRect);

    // 弾を発射する
    if (KeyLogger_IsTrigger(KK_SPACE))
    {
        Bullet_Create({ playerWorldPosition.x + playerSize.x * 0.5f, playerWorldPosition.y + playerSize.y * 0.5f }, playerFlip);
    }
}

// NOTICE: IN THE SCREEN SPACE!!!

void Player::Draw()
{
    if (!playerEnable) return;

    SpriteAnim_Draw(
        playerAnimPlayId, playerScreenPosition.x, playerScreenPosition.y,
        playerSize.x, playerSize.y, playerFlip
    );

#if defined(DEBUG) || defined(_DEBUG)

    Collision_DebugDraw(GetCircleCollision());
    //Collision_DebugDraw(GetBoxCollision());

#endif
}

void Player::ChangeStatus(Status newPlayerStatus)
{
    if (newPlayerStatus == playerStatus) return; // if status haven't changed

    if (playerAnimPlayId >= 0)
    {
        SpriteAnim_DestroyPlayer(playerAnimPlayId);
        playerAnimPlayId = -1;
    }

    playerStatus = newPlayerStatus;

    // register new animation
    switch (playerStatus)
    {
    case stand:
        playerAnimPlayId = SpriteAnim_CreatePlayer(
            SpriteAnim_RegisterPattern(
                playerTexId, 1, 1,
                PLAYER_ANIM_PLAY_RATE, { playerSize.x, playerSize.y },
                { 256.0f, 896.0f }, true
            )
        );
        break;

    case walkFront:
        playerAnimPlayId = SpriteAnim_CreatePlayer(
            SpriteAnim_RegisterPattern(
                playerTexId, 6, 6,
                PLAYER_ANIM_PLAY_RATE, { playerSize.x, playerSize.y },
                { 0.0f, 0.0f }, true
            )
        );
        break;

    case walkLeft:
        playerAnimPlayId = SpriteAnim_CreatePlayer(
            SpriteAnim_RegisterPattern(
                playerTexId, 6, 6,
                PLAYER_ANIM_PLAY_RATE, { playerSize.x, playerSize.y },
                { 0.0f, 128.0f }, true
            )
        );
        break;

    case walkBack:
        playerAnimPlayId = SpriteAnim_CreatePlayer(
            SpriteAnim_RegisterPattern(
                playerTexId, 6, 6,
                PLAYER_ANIM_PLAY_RATE, { playerSize.x, playerSize.y },
                { 0.0f, 256.0f }, true
            )
        );
        break;

    case walkRight:
        playerAnimPlayId = SpriteAnim_CreatePlayer(
            SpriteAnim_RegisterPattern(
                playerTexId, 6, 6,
                PLAYER_ANIM_PLAY_RATE, { playerSize.x, playerSize.y },
                { 0.0f, 128.0f }, true
            )
        );
        break;

    }
}

bool Player::IsEnable()
{
    return playerEnable;
}

void Player::SetScreenPosition(const ViewRect& viewRect)
{
    playerScreenPosition.x = playerWorldPosition.x - viewRect.rectPosition.x;
    playerScreenPosition.y = playerWorldPosition.y - viewRect.rectPosition.y;
}

Circle Player::GetCircleCollision()
{
    float cx = playerScreenPosition.x + playerCircleCollision.center.x;
    float cy = playerScreenPosition.y + playerCircleCollision.center.y;

    return { { cx, cy }, playerCircleCollision.radius };
}

Box Player::GetBoxCollision()
{
    float half_width = playerSize.x * 0.5f;
    float half_height = playerSize.y * 0.5f;
    return { {playerScreenPosition.x + half_width, playerScreenPosition.y + half_height}, half_width, half_height };
}

XMFLOAT2 Player::GetWorldPosition()
{
    return playerWorldPosition;
}

XMFLOAT2 Player::GetScreenPosition()
{
    return playerScreenPosition;
}

XMFLOAT2 Player::GetSize()
{
    return playerSize;
}

void Player::Destroy()
{
    playerEnable = false;
}
