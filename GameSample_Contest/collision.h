/*==============================================================================

　 コリジョン判定 [collision.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/07/03
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef COLLISION_H
#define COLLISION_H

#include <DirectXMath.h>

struct Circle // 丸の定義
{
    DirectX::XMFLOAT2 center; // 中心座標
    float radius; // 半径
};

struct Box // 四角の定義
{
    DirectX::XMFLOAT2 center;
    float half_width;
    float half_height;
};

bool Collision_OverlapCircle(const Circle& a, const Circle& b);

bool Collision_OverlapBox(const Box& a, const Box& b);


#endif // COLLISION_H

