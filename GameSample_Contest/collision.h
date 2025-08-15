// ==========================================================================================
// 
// File Name: collision.h
// Date: 2025/08/13
// Author: Gu Anyi
// Description: Collision header file
// 
// ==========================================================================================
#ifndef COLLISION_H
#define COLLISION_H

#include <d3d11.h>
#include <DirectXMath.h>

struct Circle
{
    DirectX::XMFLOAT2 center; // 中心座標
    float radius; // 半径
};

struct Box
{
    DirectX::XMFLOAT2 center;
    float half_width;
    float half_height;
};

bool Collision_CheckCircle(const Circle& a, const Circle& b);
bool Collision_CheckBox(const Box& a, const Box& b);

// debug drawing
void Collision_DebugInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Collision_DebugFinalize(); // 頂点バッファー回収
void Collision_DebugDraw(const Circle& circle, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
void Collision_DebugDraw(const Box& box, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });


#endif // COLLISION_H

