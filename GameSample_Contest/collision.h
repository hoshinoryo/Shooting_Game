/*==============================================================================

�@ �R���W�������� [collision.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/07/03
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef COLLISION_H
#define COLLISION_H

#include <DirectXMath.h>

struct Circle // �ۂ̒�`
{
    DirectX::XMFLOAT2 center; // ���S���W
    float radius; // ���a
};

struct Box // �l�p�̒�`
{
    DirectX::XMFLOAT2 center;
    float half_width;
    float half_height;
};

bool Collision_OverlapCircle(const Circle& a, const Circle& b);

bool Collision_OverlapBox(const Box& a, const Box& b);


#endif // COLLISION_H

