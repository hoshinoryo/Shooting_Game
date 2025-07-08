/*==============================================================================

   �X�v���C�g�\�� [sprite.h]
														 Author : Youhei Sato
														 Date   : 2025/06/12
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef SPRITE_H
#define SPRITE_H

#include <d3d11.h>
#include <DirectXMath.h>

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Sprite_Finalize(void);

// 2D�p�J�����̐ݒ�
void Sprite_Begin();

// �f�t�H���g�w�i�F�͔��i����XMFLOAT4 color�j
// �e�N�X�`���S�\��
void Sprite_Draw(int texid, float dx, float dy,
	bool isFlipX = false, const DirectX::XMFLOAT4& color = {1.0f, 1.0f, 1.0f, 1.0f});

// �e�N�X�`���S�\���i�\���T�C�Y�ύX�j
void Sprite_Draw(int texid, float dx, float dy, float dw, float dh,
	bool isFlipX = false, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

// UV�J�b�g
void Sprite_Draw(int texid, float dx, float dy, int px, int py, int pw, int ph,
	bool isFlipX = false, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

// UV�J�b�g�i�\���T�C�Y�ύX�j
void Sprite_Draw(int texid, float dx, float dy, float dw, float dh, int px, int py, int pw, int ph,
	bool isFlipX = false, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

// ��̃o�b�t�@�A��]���ł���
void Sprite_Draw(int texid, float dx, float dy, float dw, float dh, int px, int py, int pw, int ph, float angle,
	const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

#endif // SPRITE_H
