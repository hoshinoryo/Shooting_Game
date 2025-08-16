// ==========================================================================================
// 
// File Name: sprite.cpp
// Date: 2025/08/17
// Author: Gu Anyi
// Description: Draw the sprite
// 
// ==========================================================================================
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "direct3d.h"
#include "shader.h"
#include "debug_ostream.h"
#include "sprite.h"
#include "texture.h"

#pragma comment(lib, "d3d11.lib")


static constexpr int NUM_VERTEX = 4;

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ
static ID3D11ShaderResourceView* g_pTexture = nullptr; // テクスチャ

// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;


// 頂点構造体（GPUに渡す頂点データの定義）
//struct Vertex
//{
//	XMFLOAT3 position; // 頂点座標
//	XMFLOAT4 color;    // 色（r, g, b, a）→ ピクセルシェーダーにそのまま渡される
//	XMFLOAT2 uv;       // UV
//};


void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// デバイスとデバイスコンテキストのチェック
	if (!pDevice || !pContext) {
		hal::dout << "Sprite_Initialize() : 与えられたデバイスかコンテキストが不正です" << std::endl;
		return;
	}

	// デバイスとデバイスコンテキストの保存
	g_pDevice = pDevice;
	g_pContext = pContext;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	g_pDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);
}

void Sprite_Finalize(void)
{
	SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pVertexBuffer);
}

void Sprite_Begin()
{
	// 頂点情報を書き込み
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	// 頂点シェーダーに変換行列を設定
	Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));
}

void Sprite_Draw(const Texture& tex, float dx, float dy, bool isFlipX, const XMFLOAT4& color)
{
	Shader_Begin();

	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	Vertex* v = (Vertex*)msr.pData;

	unsigned int dw = tex.GetWidth();
	unsigned int dh = tex.GetHeight();

	v[0].position = { dx,      dy,     0.0f };
	v[1].position = { dx + dw, dy,     0.0f };
	v[2].position = { dx ,     dy + dh, 0.0f };
	v[3].position = { dx + dw, dy + dh, 0.0f };

	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	Sprite_UVFlip(v, 0.0f, 1.0f, 0.0f, 1.0f, isFlipX);

	g_pContext->Unmap(g_pVertexBuffer, 0);

	Shader_SetWorldMatrix(XMMatrixIdentity());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	tex.SetTexture(g_pContext);

	g_pContext->Draw(NUM_VERTEX, 0);

}

// UV mapping is not required
void Sprite_Draw(const Texture& tex, float dx, float dy, float dw, float dh,
	bool isFlipX, const XMFLOAT4& color)
{
	Shader_Begin();

	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	Vertex* v = (Vertex*)msr.pData;

	v[0].position = { dx,      dy,     0.0f };
	v[1].position = { dx + dw, dy,     0.0f };
	v[2].position = { dx ,     dy + dh, 0.0f };
	v[3].position = { dx + dw, dy + dh, 0.0f };

	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	Sprite_UVFlip(v, 0.0f, 1.0f, 0.0f, 1.0f, isFlipX);

	g_pContext->Unmap(g_pVertexBuffer, 0);

	Shader_SetWorldMatrix(XMMatrixIdentity());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	tex.SetTexture(g_pContext);

	g_pContext->Draw(NUM_VERTEX, 0);
}

// UV mapping is required
void Sprite_Draw(const Texture& tex, float dx, float dy, int px, int py, int pw, int ph,
	bool isFlipX, const XMFLOAT4& color)
{
	Shader_Begin();

	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* v = (Vertex*)msr.pData;

	v[0].position = { dx,      dy,      0.0f };
	v[1].position = { dx + pw, dy,      0.0f };
	v[2].position = { dx ,     dy + ph, 0.0f };
	v[3].position = { dx + pw, dy + ph, 0.0f };

	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	float tw = (float)tex.GetWidth();
	float th = (float)tex.GetHeight();

	float u0 = px / tw;
	float v0 = py / th;
	float u1 = (px + pw) / tw;
	float v1 = (py + ph) / th;

	Sprite_UVFlip(v, u0, u1, v0, v1, isFlipX);

	g_pContext->Unmap(g_pVertexBuffer, 0);

	Shader_SetWorldMatrix(XMMatrixIdentity());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	tex.SetTexture(g_pContext);

	g_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite_Draw(const Texture& tex, float dx, float dy, float dw, float dh, int px, int py, int pw, int ph,
	bool isFlipX, const XMFLOAT4& color)
{
	Shader_Begin();

	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* v = (Vertex*)msr.pData;

	v[0].position = { dx,      dy,     0.0f };
	v[1].position = { dx + dw, dy,     0.0f };
	v[2].position = { dx ,     dy + dh, 0.0f };
	v[3].position = { dx + dw, dy + dh, 0.0f };

	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	float tw = (float)tex.GetWidth();
	float th = (float)tex.GetHeight();

	float u0 = px / tw; // uvカット
	float v0 = py / th;
	float u1 = (px + pw) / tw;
	float v1 = (py + ph) / th;

	Sprite_UVFlip(v, u0, u1, v0, v1, isFlipX);

	g_pContext->Unmap(g_pVertexBuffer, 0);

	Shader_SetWorldMatrix(XMMatrixIdentity());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	tex.SetTexture(g_pContext);

	g_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite_Draw(const Texture& tex, float dx, float dy, float dw, float dh,
	int px, int py, int pw, int ph,
	float angle, const DirectX::XMFLOAT4& color)
{
	Shader_Begin();

	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* v = (Vertex*)msr.pData;

	v[0].position = { -0.5f, -0.5f, 0.0f };
	v[1].position = { +0.5f, -0.5f, 0.0f };
	v[2].position = { -0.5f, +0.5f, 0.0f };
	v[3].position = { +0.5f, +0.5f, 0.0f };

	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	float tw = (float)tex.GetWidth();
	float th = (float)tex.GetHeight();

	float u0 = px / tw;
	float v0 = py / th;
	float u1 = (px + pw) / tw;
	float v1 = (py + ph) / th;

	v[0].uv = { u0, v0 };
	v[1].uv = { u1, v0 };
	v[2].uv = { u0, v1 };
	v[3].uv = { u1, v1 };

	g_pContext->Unmap(g_pVertexBuffer, 0);

	Shader_SetWorldMatrix(XMMatrixIdentity());

	// 変換行列をシェーダーに設定
	XMMATRIX scale = XMMatrixScaling(dw, dh, 1.0f);           // 拡大縮小
	XMMATRIX rotation = XMMatrixRotationZ(angle);             // 回転行列
	XMMATRIX translation = XMMatrixTranslation(dx, dy, 0.0f); // 平行移動行列
	// 行列の合成
	Shader_SetWorldMatrix(scale * rotation * translation);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	tex.SetTexture(g_pContext);

	g_pContext->Draw(NUM_VERTEX, 0);
}

void Sprite_UVFlip(Vertex* v, float u0, float u1, float v0, float v1, bool isFlipX)
{
	if (!isFlipX)
	{
		v[0].uv = { u0, v0 };
		v[1].uv = { u1, v0 };
		v[2].uv = { u0, v1 };
		v[3].uv = { u1, v1 };
	}
	else // 左右反転
	{
		v[0].uv = { u1, v0 };
		v[1].uv = { u0, v0 };
		v[2].uv = { u1, v1 };
		v[3].uv = { u0, v1 };
	}
}
