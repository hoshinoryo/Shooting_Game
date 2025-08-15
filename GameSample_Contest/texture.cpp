/*==============================================================================

   テクスチャ管理 [texture.cpp]
														 Author : Youhei Sato
														 Date   : 2025/06/13
--------------------------------------------------------------------------------

==============================================================================*/
#include "texture.h"
#include "direct3d.h"
#include "DirectXTex.h"

#include <string>

using namespace DirectX;

static constexpr int TEXTURE_MAX = 256; // テクスチャ管理最大数

struct Texture // テクスチャ管理する用の構造体
{
	std::wstring filename;
	ID3D11ShaderResourceView* pTexture;
	unsigned int width;
	unsigned int height;
};

static Texture g_Textures[TEXTURE_MAX] = {};
static int g_SetTextureIndex = -1; // 管理番号、デフォルトは -1

static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	for (Texture& t : g_Textures)
	{
		t.pTexture = nullptr;
	}

	g_SetTextureIndex = -1;

	// デバイスとデバイスコンテキストの保存
	g_pDevice = pDevice;
	g_pContext = pContext;
}

void Texture_Finalize(void)
{
	Texture_AllRelease();
}

int Texture_Load(const wchar_t* pFilename)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Textures[i].filename == pFilename)
		{
			return i;
		}
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Textures[i].pTexture)
		{
			continue; // 使用中
		}

		// テクスチャの読み込み
		TexMetadata metadata;
		ScratchImage image;

		HRESULT hr = LoadFromWICFile(pFilename, WIC_FLAGS_NONE, &metadata, image);

		if (FAILED(hr))
		{
			MessageBoxW(nullptr, L"テクスチャの読み込みに失敗しました", pFilename, MB_OK | MB_ICONERROR);
			return -1;
		}

		g_Textures[i].filename = pFilename;
		g_Textures[i].width = (unsigned int)metadata.width;
		g_Textures[i].height = (unsigned int)metadata.height;

		hr = CreateShaderResourceView(g_pDevice, image.GetImages(),
			image.GetImageCount(), metadata, &g_Textures[i].pTexture);

		return i; // 管理番号を返す
	}
}

void Texture_AllRelease()
{
	for (Texture& t : g_Textures)
	{
		t.filename.clear();
		SAFE_RELEASE(t.pTexture);
	}
}

void Texture_SetTexture(int texid)
{
	if (texid < 0 || texid >= TEXTURE_MAX || !g_Textures[texid].pTexture)
	{ 
		return;
	}
	// if (g_SetTextureIndex == texid) return;

	g_SetTextureIndex = texid;
	g_pContext->PSSetShaderResources(0, 1, &g_Textures[texid].pTexture);
}

unsigned int Texture_Width(int texid)
{
	if (texid < 0 || texid >= TEXTURE_MAX || !g_Textures[texid].pTexture)
	{
		return 0;
	}
	return g_Textures[texid].width;
}

unsigned int Texture_Height(int texid)
{
	if (texid < 0 || texid >= TEXTURE_MAX || !g_Textures[texid].pTexture)
	{
		return 0;
	}
	return g_Textures[texid].height;
}
