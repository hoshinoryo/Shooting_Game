// ==========================================================================================
// 
// File Name: sprite_anim.h
// Date: 2025/08/17
// Author: Gu Anyi
// Description: Sprite animation header file
// 
// ==========================================================================================

#ifndef SPRITE_ANIM_H
#define SPRITE_ANIM_H

#include <DirectXMath.h>

#include "texture.h"


void SpriteAnim_Initialize();
void SpriteAnim_Finalize();

void SpriteAnim_Update(double elapsed_time);
void SpriteAnim_Draw(int playid, float dx, float dy, float dw, float dh, bool isFlipX);

// ÉpÉ^Å[Éììoò^óp
int SpriteAnim_RegisterPattern(
	const Texture& tex, int patternMax, int h_pattern_max, double seconds_per_pattern,
	const DirectX::XMFLOAT2& pattern_size, 
	const DirectX::XMFLOAT2& start_position, bool is_looped = true
);

int SpriteAnim_CreatePlayer(int anim_pattern_id);
bool SpriteAnim_IsStopped(int index);
void SpriteAnim_DestroyPlayer(int index);

#endif // SPRITE_ANIM_H

