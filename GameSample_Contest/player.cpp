/*==============================================================================

�@ �v���[���[����[player.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/06/27
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
// �G�\���������Ȃ炱�̓��
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
static bool g_PlayerFacingLeft; // ���Ɍ�����
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

void Player_Finalize() // ��������Ȃ�
{
}

void Player_Update(double elapsed_time)
{
    if (!g_PlayerEnable) return; // �v���[���[�����񂾂�߂�

    XMVECTOR position = XMLoadFloat2(&g_PlayerPosition);
    XMVECTOR velocity = XMLoadFloat2(&g_PlayerVelocity);

    XMVECTOR direction = {};

    if (KeyLogger_IsPressed(KK_W)) // �S��if���g���A�������f
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
    // ������A��D�������Ɠ����Ȃ�

    direction = XMVector2Normalize(direction); // �P�ʃx�N�g���ɂȂ�

    // position += direction * 3.0f; // �����̌��ʂ��Ȃ�����

    /*
    // �Œ�t���[���̂���
    velocity += direction * 1.0f;
    position += velocity;
    velocity *= 0.9f;
    */

    // �����V�~�����[�V�����̂����A�ǂ�ȃt���[���ł��������x���o��
    velocity += direction * 6000000.0f / 2500.0f * elapsed_time; // ��(�P�ʁF�j���[�g��)�A600���j���[�g���̗� / 2500�̏d���i�P�ʁF�L���j
    position += velocity * elapsed_time; // ���Z�ߒ��Aelapsed_time���g���Đϕ�����
    velocity += -velocity * 4.0f * elapsed_time; // �~�܂����

    XMStoreFloat2(&g_PlayerPosition, position); // ���Z���ʖ߂�
    XMStoreFloat2(&g_PlayerVelocity, velocity);

    /*
    // ��ʂ͈̔͂ň͂܂�Ă�
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

    // �e�𔭎˂���
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
