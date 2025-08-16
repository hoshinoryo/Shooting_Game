// ==========================================================================================
// 
// File Name: sprite.h
// Date: 2025/08/17
// Author: Gu Anyi
// Description: Sprite header file
// 
// ==========================================================================================
#ifndef SPRITE_H
#define SPRITE_H

#include "texture.h"

#include <d3d11.h>
#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
};

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Sprite_Finalize(void);

// 2D用カメラの設定
void Sprite_Begin();

// デフォルト背景色は白（引数XMFLOAT4 color）
// テクスチャ全表示
void Sprite_Draw(const Texture& tex, float dx, float dy,
	bool isFlipX = false, const DirectX::XMFLOAT4& color = {1.0f, 1.0f, 1.0f, 1.0f});

// テクスチャ全表示（表示サイズ変更）
void Sprite_Draw(const Texture& tex, float dx, float dy, float dw, float dh,
	bool isFlipX = false, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

// UVカット
void Sprite_Draw(const Texture& tex, float dx, float dy, int px, int py, int pw, int ph,
	bool isFlipX = false, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

// UVカット（表示サイズ変更）
void Sprite_Draw(const Texture& tex, float dx, float dy, float dw, float dh, int px, int py, int pw, int ph,
	bool isFlipX = false, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

// 二つのバッファ、回転もできる
void Sprite_Draw(const Texture& tex, float dx, float dy, float dw, float dh, int px, int py, int pw, int ph, float angle,
	const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

void Sprite_UVFlip(Vertex* v, float u0, float u1, float v0, float v1, bool isFlipX);

#endif // SPRITE_H
