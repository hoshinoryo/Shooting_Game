// ==========================================================================================
// 
// File Name: collision.cpp
// Date: 2025/08/13
// Author: Gu Anyi
// Description: Manage collision check and the debug mode 
// 
// ==========================================================================================
#include "collision.h"
#include "direct3d.h"
#include "texture.h"
#include "shader.h"

#include <DirectXMath.h>

using namespace DirectX;

// Debug用変数
static constexpr int NUM_VERTEX = 5000; // 最大頂点数、半径1600の円の周長
static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

static Texture g_WhiteTex;

// 頂点構造体
struct Vertex
{
    XMFLOAT3 position; // 頂点座標
    XMFLOAT4 color;    // 色
    XMFLOAT2 uv;       // UV
};


bool Collision_CheckCircle(const Circle& a, const Circle& b)
{
    float x1 = b.center.x - a.center.x;
    float y1 = b.center.y - a.center.y;

    return (x1 * x1 + y1 * y1) < (a.radius + b.radius) * (a.radius + b.radius);
}

bool Collision_CheckBox(const Box& a, const Box& b)
{
    float at = a.center.y - a.half_height;
    float ab = a.center.y + a.half_height;
    float al = a.center.x - a.half_width;
    float ar = a.center.x + a.half_width;
    float bt = b.center.y - b.half_height;
    float bb = b.center.y + b.half_height;
    float bl = b.center.x - b.half_width;
    float br = b.center.x + b.half_width;
    return al < br && ar > bl && at < bb && ab > bt;
}

void Collision_DebugInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    g_pDevice = pDevice;
    g_pContext = pContext;

	g_WhiteTex.Initialize(g_pDevice, L"resources/white.png");

    // 頂点バッファ
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Vertex vertices[NUM_VERTEX] = {};
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	HRESULT hr = g_pDevice->CreateBuffer(&bd, &initData, &g_pVertexBuffer);
	if (FAILED(hr)) {
		g_pVertexBuffer = nullptr;
		return;
	}
}

void Collision_DebugFinalize()
{
    SAFE_RELEASE(g_pVertexBuffer);
	g_WhiteTex.Finalize();
}

void Collision_DebugDraw(const Circle& circle, const XMFLOAT4& color)
{
	//g_WhiteTex.Initialize(g_pDevice, L"resources/white.png");

	int NumVertex = (int)circle.radius * 2.0f * XM_PI + 1;

	Shader_Begin();

	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* v = (Vertex*)msr.pData;

	// 円の描画
	const float rad = XM_2PI / NumVertex;
	for (int i = 0; i < NumVertex; i++)
	{
		v[i].position.x = cosf(rad * i) * circle.radius + circle.center.x;
		v[i].position.y = sinf(rad * i) * circle.radius + circle.center.y;
		v[i].position.z = 0.0f;
		v[i].color = color;
		v[i].uv = XMFLOAT2{ 0.0, 0.0f };
	}

	g_pContext->Unmap(g_pVertexBuffer, 0);

	Shader_SetWorldMatrix(XMMatrixIdentity());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	g_WhiteTex.SetTexture(g_pContext);

	g_pContext->Draw(NumVertex, 0);
}

void Collision_DebugDraw(const Box& box, const XMFLOAT4& color)
{
	//g_WhiteTex.Initialize(g_pDevice, L"resources/white.png");

	int NumVertex = 5;

	Shader_Begin();

	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	Vertex* v = (Vertex*)msr.pData;

	v[0].position = { box.center.x - box.half_width, box.center.y - box.half_height, 0.0f };
	v[1].position = { box.center.x + box.half_width, box.center.y - box.half_height, 0.0f };
	v[2].position = { box.center.x + box.half_width, box.center.y + box.half_height, 0.0f };
	v[3].position = { box.center.x - box.half_width, box.center.y + box.half_height, 0.0f };
	v[4].position = { box.center.x - box.half_width, box.center.y - box.half_height, 0.0f };

	for (int i = 0; i < NumVertex; i++)
	{
		v[i].color = color;
		v[i].uv = XMFLOAT2{ 0.0, 0.0f };
	}

	g_pContext->Unmap(g_pVertexBuffer, 0);

	Shader_SetWorldMatrix(XMMatrixIdentity());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	g_WhiteTex.SetTexture(g_pContext);

	g_pContext->Draw(NumVertex, 0);
}
