// ==========================================================================================
// 
// File Name: texture.cpp
// Date: 2025/08/16
// Author: Gu Anyi
// Description: Manage texture initialization and release
// 
// ==========================================================================================
#include "texture.h"
#include "direct3d.h"
#include "DirectXTex.h"

#include <string>

using namespace DirectX;


bool Texture::Initialize(ID3D11Device* device, const wchar_t* pFilename)
{
	Release();

	TexMetadata metadata;
	ScratchImage image;

	HRESULT hr = LoadFromWICFile(pFilename, WIC_FLAGS_NONE, &metadata, image);
	if (FAILED(hr))
	{
		return false;
	}

	width = (unsigned int)metadata.width;
	height = (unsigned int)metadata.height;

	hr = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metadata, &pTexture);

	return SUCCEEDED(hr);
}

void Texture::Finalize()
{
	Release();
}

void Texture::Release()
{
	pTexture.Reset();
	width = 0;
	height = 0;
}

void Texture::SetTexture(ID3D11DeviceContext* context) const
{
	ID3D11ShaderResourceView* tex = pTexture.Get();
	context->PSSetShaderResources(0, 1, &tex);
}

unsigned int Texture::GetWidth() const
{
	return width;
}

unsigned int Texture::GetHeight() const
{
	return height;
}
