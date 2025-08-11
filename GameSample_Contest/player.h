/*==============================================================================

　 プレーヤー制御[player.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/06/27
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef PLAYER_H
#define PLAYER_H

#include "collision.h"
#include "camera.h"

#include <DirectXMath.h>

void Player_Initialize(const DirectX::XMFLOAT2& position);
void Player_Finalize();

void Player_Update(double elapsed_time);
void Player_Draw(const ViewRect& viewRect);

void Player_Load();

bool Player_IsEnable(); // 弾の有効無効
Circle Player_GetCollision();
DirectX::XMFLOAT2 Player_GetPosition();
void Player_Destroy();

#endif // PLAYER_H
