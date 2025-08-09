// ==========================================================================================
// 
// File Name: chader.h
// Date: 2025/08/09
// Author: Gu Anyi
// Description: Initialize and manage shaders
// 
// ==========================================================================================

#ifndef SHADER_H
#define	SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>

bool Shader_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Shader_Finalize();

void Shader_SetWorldMatrix(const DirectX::XMMATRIX& matrix);
//void Shader_SetViewMatrix(const DirectX::XMMATRIX& matrix);
void Shader_SetProjectionMatrix(const DirectX::XMMATRIX& matrix);

void Shader_Begin();

#endif // SHADER_H
