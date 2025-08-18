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
#include "check_collision.h"
#include "direct3d.h"

#include "debug_text.h"

#include <DirectXMath.h>

using namespace DirectX;

static constexpr float PLAYER_ANIM_PLAY_RATE = 0.08f;
static constexpr float PLAYER_SPEED = 7.0f;


Player::Player()
{
    playerWorldPosition = {};
    playerScreenPosition = {};
    playerVelocity = {};
    playerSize = {};
    playerFlip = false;
    playerAnimPlayId = -1;
    playerCircleCollision = { { 64.0f, 92.0f }, 20.0f };
    playerBoxCollision = { { 64.0f, 92.0f }, 24.0f, 14.0f }; // test collision box
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

    playerTex.Initialize(Direct3D_GetDevice(), L"resources/Santa_Claus.png");
}

void Player::Finalize()
{
    playerTex.Finalize();
}

void Player::UpdatePosition(double elapsed_time, Collision_Map& map, const ViewRect& viewRect)
{
    XMFLOAT2 oldPos = playerWorldPosition;
    XMFLOAT2 newPos = oldPos;

    XMVECTOR direction = {};

    if (KeyLogger_IsPressed(KK_W))
    {
        direction += {0.0f, -1.0f};
        playerFlip = false;
    }
    if (KeyLogger_IsPressed(KK_A))
    {
        direction += {-1.0f, 0.0f};
        playerFlip = true;
    }
    if (KeyLogger_IsPressed(KK_S))
    {
        direction += {0.0f, 1.0f};
        playerFlip = false;
    }
    if (KeyLogger_IsPressed(KK_D))
    {
        direction += {1.0f, 0.0f};
        playerFlip = false;
    }

    if (XMVector2Equal(direction, XMVectorZero())) return;

    direction = XMVector2Normalize(direction);

    XMVECTOR moveVec = XMVectorScale(direction, PLAYER_SPEED);
    XMFLOAT2 move;
    XMStoreFloat2(&move, moveVec);

    // check collision with seperated x axis and y axis
    // x axis
    newPos.x += move.x;
    Box playerBoxX = GetBoxCollision();
    playerBoxX.center.x += move.x;
    if (CheckCollision_PlayerBoxVSMap(playerBoxX, map, viewRect))
    {
        newPos.x = oldPos.x;
    }

    // y axis
    newPos.y += move.y;
    Box playerBoxY = GetBoxCollision();
    playerBoxY.center.y += move.y;
    if (CheckCollision_PlayerBoxVSMap(playerBoxY, map, viewRect))
    {
        newPos.y = oldPos.y;
    }

    playerWorldPosition = newPos;

    // マップの範囲：1920 * 3200
    if (playerWorldPosition.x <= 0.0f) playerWorldPosition.x = 0.0f;
    if (playerWorldPosition.x >= (3200.0f - playerSize.x)) playerWorldPosition.x = 3200.0f - playerSize.x;
    if (playerWorldPosition.y <= 0.0f) playerWorldPosition.y = 0.0f;
    if (playerWorldPosition.y >= (1920.0f - playerSize.y)) playerWorldPosition.y = 1920.0f - playerSize.y;
}

void Player::UpdateStatus()
{
    Status newPlayerStatus = stand;

    if (KeyLogger_IsPressed(KK_W)) newPlayerStatus = walkBack;
    if (KeyLogger_IsPressed(KK_A)) newPlayerStatus = walkRight;
    if (KeyLogger_IsPressed(KK_S)) newPlayerStatus = walkFront;
    if (KeyLogger_IsPressed(KK_D)) newPlayerStatus = walkLeft;

    ChangeStatus(newPlayerStatus);
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
                playerTex, 1, 1,
                PLAYER_ANIM_PLAY_RATE, { playerSize.x, playerSize.y },
                { 256.0f, 896.0f }, true
            )
        );
        break;

    case walkFront:
        playerAnimPlayId = SpriteAnim_CreatePlayer(
            SpriteAnim_RegisterPattern(
                playerTex, 6, 6,
                PLAYER_ANIM_PLAY_RATE, { playerSize.x, playerSize.y },
                { 0.0f, 0.0f }, true
            )
        );
        break;

    case walkLeft:
        playerAnimPlayId = SpriteAnim_CreatePlayer(
            SpriteAnim_RegisterPattern(
                playerTex, 6, 6,
                PLAYER_ANIM_PLAY_RATE, { playerSize.x, playerSize.y },
                { 0.0f, 128.0f }, true
            )
        );
        break;

    case walkBack:
        playerAnimPlayId = SpriteAnim_CreatePlayer(
            SpriteAnim_RegisterPattern(
                playerTex, 6, 6,
                PLAYER_ANIM_PLAY_RATE, { playerSize.x, playerSize.y },
                { 0.0f, 256.0f }, true
            )
        );
        break;

    case walkRight:
        playerAnimPlayId = SpriteAnim_CreatePlayer(
            SpriteAnim_RegisterPattern(
                playerTex, 6, 6,
                PLAYER_ANIM_PLAY_RATE, { playerSize.x, playerSize.y },
                { 0.0f, 128.0f }, true
            )
        );
        break;
    }
}

/*
void Player::Update(double elapsed_time, const ViewRect& viewRect)
{
    if (!playerEnable) return;

    UpdatePosition(elapsed_time);
    UpdateStatus();

    SetScreenPosition(viewRect);

    // 弾を発射する
    if (KeyLogger_IsTrigger(KK_SPACE))
    {
        Bullet_Create({ playerWorldPosition.x + playerSize.x * 0.5f, playerWorldPosition.y + playerSize.y * 0.5f }, playerFlip);
    }
}
*/


// NOTICE: IN THE SCREEN SPACE!!!

void Player::Draw()
{
    if (!playerEnable) return;

    SpriteAnim_Draw(
        playerAnimPlayId, playerScreenPosition.x, playerScreenPosition.y,
        playerSize.x, playerSize.y, playerFlip
    );

#if defined(DEBUG) || defined(_DEBUG)

    //Collision_DebugDraw(playerTex, GetCircleCollision());
    Collision_DebugDraw(GetBoxCollision());

#endif
}

bool Player::GetIsEnable()
{
    return playerEnable;
}

void Player::SetWorldPosition(DirectX::XMFLOAT2& position)
{
    playerWorldPosition.x = position.x;
    playerWorldPosition.y = position.y;
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
    //float half_width = playerSize.x * 0.5f;
    //float half_height = playerSize.y * 0.5f;
    //return { {playerScreenPosition.x + half_width, playerScreenPosition.y + half_height}, half_width, half_height };
    return { { playerScreenPosition.x + playerBoxCollision.center.x, playerScreenPosition.y + playerBoxCollision.center.y },
        playerBoxCollision.half_width, playerBoxCollision.half_height };
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
