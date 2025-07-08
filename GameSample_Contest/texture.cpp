/*==============================================================================

   テクスチャ管理 [texture.cpp]
														 Author : Youhei Sato
														 Date   : 2025/06/13
--------------------------------------------------------------------------------

==============================================================================*/
#include "texture.h"
#include "direct3d.h"
#include "DirectXTex.h"
using namespace DirectX;
#include <string>

static constexpr int TEXTURE_MAX = 256; // テクスチャ管理最大数

struct Texture // テクスチャ管理する用の構造体
{
	std::wstring filename;
	ID3D11ShaderResourceView* pTexture;
	unsigned int width; // テクスチャの幅
	unsigned int height; // テクスチャの高さ
};

static Texture g_Textures[TEXTURE_MAX] = {};
static int g_SetTextureIndex = -1; // 管理番号、デフォルトは -1

// 注意！初期化で外部から設定されるもの。Release不要。
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
	// すでに読み込んでいるものは読み込まない
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Textures[i].filename == pFilename) // 今読み込もうとするファイル名は同じですか
		{
			return i; // 管理番号を返す
		}
	}

	// 空いている管理領域を探す
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Textures[i].pTexture)
		{
			continue; // 使用中
		}

		// テクスチャの読み込み

		TexMetadata metadata;
		// TexMetadata：テクスチャのメタ情報（幅、高さ、フォーマットなど）を保持する構造体
		// ピクセルデータ自体は含まれず、画像の基本情報を管理する
		ScratchImage image;

		HRESULT hr = LoadFromWICFile(pFilename, WIC_FLAGS_NONE, &metadata, image); // ファイル読み込み

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
		t.filename.clear(); // ファイル名削除
		SAFE_RELEASE(t.pTexture);
	}
}

void Texture_SetTexture(int texid)
{
	// テクスチャ設定
	if (texid < 0)
	{
		return; // 管理番号は-1の場合
	}
	// if (g_SetTextureIndex == texid) return;

	g_SetTextureIndex = texid;

	g_pContext->PSSetShaderResources(0, 1, &g_Textures[texid].pTexture);
}

unsigned int Texture_Width(int texid)
{
	if (texid < 0)
	{
		return 0;
	}
	return g_Textures[texid].width;
}

unsigned int Texture_Height(int texid)
{
	if (texid < 0)
	{
		return 0;
	}
	return g_Textures[texid].height;
}
