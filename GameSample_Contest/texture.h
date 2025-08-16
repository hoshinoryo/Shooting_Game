// ==========================================================================================
// 
// File Name: texture.h
// Date: 2025/08/16
// Author: Gu Anyi
// Description: Texture class header file
// 
// ==========================================================================================
#ifndef TEXTURE_H
#define TEXTURE_H

#include <wrl/client.h>
#include <d3d11.h>

class Texture
{
private:

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;
	unsigned int width;
	unsigned int height;

public:

	Texture() : pTexture(nullptr), width(0), height(0) {}

	bool Initialize(ID3D11Device* device, const wchar_t *pFilename);
	void Finalize();

	void Release();
	void SetTexture(ID3D11DeviceContext* context) const;

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
};

#endif //TEXTURE_H