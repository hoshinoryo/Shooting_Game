/*==============================================================================

   �e�N�X�`���Ǘ� [texture.cpp]
														 Author : Youhei Sato
														 Date   : 2025/06/13
--------------------------------------------------------------------------------

==============================================================================*/
#include "texture.h"
#include "direct3d.h"
#include "DirectXTex.h"
using namespace DirectX;
#include <string>

static constexpr int TEXTURE_MAX = 256; // �e�N�X�`���Ǘ��ő吔

struct Texture // �e�N�X�`���Ǘ�����p�̍\����
{
	std::wstring filename;
	ID3D11ShaderResourceView* pTexture;
	unsigned int width; // �e�N�X�`���̕�
	unsigned int height; // �e�N�X�`���̍���
};

static Texture g_Textures[TEXTURE_MAX] = {};
static int g_SetTextureIndex = -1; // �Ǘ��ԍ��A�f�t�H���g�� -1

// ���ӁI�������ŊO������ݒ肳�����́BRelease�s�v�B
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

void Texture_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	for (Texture& t : g_Textures)
	{
		t.pTexture = nullptr;
	}

	g_SetTextureIndex = -1;

	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̕ۑ�
	g_pDevice = pDevice;
	g_pContext = pContext;
}

void Texture_Finalize(void)
{
	Texture_AllRelease();
}

int Texture_Load(const wchar_t* pFilename)
{
	// ���łɓǂݍ���ł�����͓̂ǂݍ��܂Ȃ�
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Textures[i].filename == pFilename) // ���ǂݍ������Ƃ���t�@�C�����͓����ł���
		{
			return i; // �Ǘ��ԍ���Ԃ�
		}
	}

	// �󂢂Ă���Ǘ��̈��T��
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Textures[i].pTexture)
		{
			continue; // �g�p��
		}

		// �e�N�X�`���̓ǂݍ���

		TexMetadata metadata;
		// TexMetadata�F�e�N�X�`���̃��^���i���A�����A�t�H�[�}�b�g�Ȃǁj��ێ�����\����
		// �s�N�Z���f�[�^���̂͊܂܂ꂸ�A�摜�̊�{�����Ǘ�����
		ScratchImage image;

		HRESULT hr = LoadFromWICFile(pFilename, WIC_FLAGS_NONE, &metadata, image); // �t�@�C���ǂݍ���

		if (FAILED(hr))
		{
			MessageBoxW(nullptr, L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", pFilename, MB_OK | MB_ICONERROR);
			return -1;
		}

		g_Textures[i].filename = pFilename;
		g_Textures[i].width = (unsigned int)metadata.width;
		g_Textures[i].height = (unsigned int)metadata.height;

		hr = CreateShaderResourceView(g_pDevice, image.GetImages(),
			image.GetImageCount(), metadata, &g_Textures[i].pTexture);

		return i; // �Ǘ��ԍ���Ԃ�
	}
}

void Texture_AllRelease()
{
	for (Texture& t : g_Textures)
	{
		t.filename.clear(); // �t�@�C�����폜
		SAFE_RELEASE(t.pTexture);
	}
}

void Texture_SetTexture(int texid)
{
	// �e�N�X�`���ݒ�
	if (texid < 0)
	{
		return; // �Ǘ��ԍ���-1�̏ꍇ
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
