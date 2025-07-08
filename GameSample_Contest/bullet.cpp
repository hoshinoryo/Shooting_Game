/*==============================================================================

�@ �e�̐��� [bullet.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/07/01
--------------------------------------------------------------------------------

==============================================================================*/
#include "bullet.h"
#include "player.h"
#include "direct3d.h" // ��ʃT�C�Y���擾
#include "sprite.h"
#include "texture.h"
#include <DirectXMath.h>

using namespace DirectX;

struct Bullet
{
    XMFLOAT2 position;
    XMFLOAT2 velocity;
    double life_time;
    bool isEnable;
    Circle collision; // collider
};
static Bullet g_Bullets[BULLETS_MAX] = {};
static int g_BulletTexid = -1;


void Bullet_Initialize()
{
    for (Bullet& b : g_Bullets)
    {
        b.isEnable = false;
    }

    g_BulletTexid = Texture_Load(L"resources/present.png");
}

void Bullet_Finalize()
{
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

        // �������������������
        if (b.life_time > 5.0f)
        {
            b.isEnable = false;
        }

        // ��ʂ̊O�o��̂�������
        if (b.position.x > Direct3D_GetBackBufferWidth()) // �e�̒��S���W�͂ǂ���������
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

        Sprite_Draw(g_BulletTexid, b.position.x, b.position.y, 42.0f, 48.0f);
    }
}

void Bullet_Create(const XMFLOAT2& position, bool isFlipX)
{
    for (Bullet& b : g_Bullets)
    {
        if (b.isEnable) continue; // �g�p���̒e�̓X�L�b�v

        // �󂫗̈攭��
        b.isEnable  = true;
        b.life_time = 0.0;
        b.position  = position;
        b.velocity  = { (isFlipX ? 500.0f : -500.0f), 0.0f}; // �΂߂ɂ��Ăق����Ȃ�Ay������������p�ӂ��Ă���������炤�A�����|�C���g
        b.collision = { { 21.0f, 24.0f }, 20.0f }; // �e��collider

        break; // ����Ȃ��ƒe�͏����Ȃ�
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
    return { {cx, cy}, g_Bullets[index].collision.radius }; // �e�̃R���W�����Z���^�[
}

void Bullet_Destroy(int index)
{
    g_Bullets[index].isEnable = false;
}
