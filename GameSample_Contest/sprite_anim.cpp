// ==========================================================================================
// 
// File Name: sprite_anim.cpp
// Date: 2025/08/17
// Author: Gu Anyi
// Description: Draw the sprite animation and manage animation playing data
// 
// ==========================================================================================

#include "sprite_anim.h"
#include "sprite.h"

#include <DirectXMath.h>

#include "debug_text.h"
#include "debug_ostream.h"

using namespace DirectX;


struct AnimPatternData
{
	//int m_TextureId = -1;    // テクスチャID
	int m_PatternMax = 0;    // アニメのパターン数
	int m_HPatternMax = 0;    // アニメの横方向のパターン数
	XMFLOAT2 m_StartPosition = { 0, 0 }; // アニメーションのスタート座標
	XMFLOAT2 m_PatternSize = { 0.0f, 0.0f };   // 1パターンのサイズ
	double m_seconds_per_pattern = 0.1; // 1パターンの再生時間
	bool m_IsLooped = true;  // ループするか
	Texture m_Texture;

	void Reset()
	{
		m_Texture.Release();
		m_PatternMax = 0;
		m_HPatternMax = 0;
		m_StartPosition = { 0, 0 };
		m_PatternSize = { 0, 0 };
		m_seconds_per_pattern = 0.1;
		m_IsLooped = true;
	}
};

struct AnimPlayData
{
	int m_PatternId = -1;    // アニメーションパターンID、使うパターンはどっち
	int m_PatternNum = 0;    // 現在再生中のパターン番号、常に変わっている
	double m_Accumulated_time = 0.0; // 累積時間
	bool m_IsStopped = false;
};

static constexpr int ANIM_PATTERN_MAX = 128;
static AnimPatternData g_AnimPattern[ANIM_PATTERN_MAX];
static constexpr int ANIM_PLAY_MAX = 256;
static AnimPlayData g_AnimPlayData[ANIM_PLAY_MAX];

void SpriteAnim_Initialize()
{
	for (AnimPatternData& data : g_AnimPattern)
	{
		data.m_PatternMax = 0;
		data.m_HPatternMax = 0;
		data.m_StartPosition = { 0, 0 };
		data.m_PatternSize = { 0.0f, 0.0f };
		data.m_seconds_per_pattern = 0.1;
		data.m_IsLooped = true;
	}

	for (AnimPlayData& data : g_AnimPlayData)
	{
		data.m_PatternId = -1;
		data.m_IsStopped = false;
		data.m_Accumulated_time = 0.0;
		data.m_IsStopped = false;
	}
}

void SpriteAnim_Finalize()
{
	for (AnimPatternData& data : g_AnimPattern)
	{
		data.m_Texture.Release();
	}
}

void SpriteAnim_Update(double elapsed_time)
{
	// 0.1秒以上たったらアニメーションを進める
	for (int i = 0; i < ANIM_PLAY_MAX; i++)
	{
		if (g_AnimPlayData[i].m_PatternId < 0) continue;

		AnimPatternData* pAnimPatternData = &g_AnimPattern[g_AnimPlayData[i].m_PatternId];

		if (g_AnimPlayData[i].m_Accumulated_time >= pAnimPatternData->m_seconds_per_pattern)
		{
			g_AnimPlayData[i].m_PatternNum++;

			if (g_AnimPlayData[i].m_PatternNum >= pAnimPatternData->m_PatternMax)
			{
				if (pAnimPatternData->m_IsLooped)
				{
					g_AnimPlayData[i].m_PatternNum = 0;
				}
				else
				{
					g_AnimPlayData[i].m_PatternNum = pAnimPatternData->m_PatternMax - 1;
					g_AnimPlayData[i].m_IsStopped = true;
				}
			}
			
			g_AnimPlayData[i].m_Accumulated_time -= pAnimPatternData->m_seconds_per_pattern;
		}

		g_AnimPlayData[i].m_Accumulated_time += elapsed_time; // 経過時間を加算する
	}
}

void SpriteAnim_Draw(int playid, float dx, float dy, float dw, float dh, bool isFlipX)
{
	int anim_pattern_id = g_AnimPlayData[playid].m_PatternId;
	if (anim_pattern_id < 0) return;

	AnimPatternData* pAnimPatternData = &g_AnimPattern[anim_pattern_id];
	Texture* tex = &pAnimPatternData->m_Texture;

	float px = pAnimPatternData->m_StartPosition.x +
		pAnimPatternData->m_PatternSize.x *
		(g_AnimPlayData[playid].m_PatternNum % pAnimPatternData->m_HPatternMax);
	float py = pAnimPatternData->m_StartPosition.y +
		pAnimPatternData->m_PatternSize.y *
		(g_AnimPlayData[playid].m_PatternNum / pAnimPatternData->m_HPatternMax);
	float pw = pAnimPatternData->m_PatternSize.x;
	float ph = pAnimPatternData->m_PatternSize.y;

	/*Sprite_Draw(pAnimPatternData->m_Texture,
		dx, dy, dw, dh,
		pAnimPatternData->m_StartPosition.x + pAnimPatternData->m_PatternSize.x * (g_AnimPlayData[playid].m_PatternNum % pAnimPatternData->m_HPatternMax),
		pAnimPatternData->m_StartPosition.y + pAnimPatternData->m_PatternSize.y * (g_AnimPlayData[playid].m_PatternNum / pAnimPatternData->m_HPatternMax),
		pAnimPatternData->m_PatternSize.x,
		pAnimPatternData->m_PatternSize.y,
		isFlipX
	);*/

	Sprite_Draw(*tex, dx, dy, dw, dh, (int)px, (int)py, (int)pw, (int)ph, isFlipX);
}

int SpriteAnim_RegisterPattern(const Texture& tex, int patternMax, int h_pattern_max, double seconds_per_pattern,
	const DirectX::XMFLOAT2& pattern_size,
	const DirectX::XMFLOAT2& start_position, bool is_looped)
{
	for (int i = 0; i < ANIM_PATTERN_MAX; i++)
	{
		if (g_AnimPattern[i].m_PatternMax != 0) continue;

		AnimPatternData& data = g_AnimPattern[i];
		data.Reset();
		data.m_Texture = tex;
		data.m_PatternMax = patternMax;
		data.m_HPatternMax = h_pattern_max;
		data.m_seconds_per_pattern = seconds_per_pattern;
		data.m_PatternSize = pattern_size;
		data.m_StartPosition = start_position;
		data.m_IsLooped = is_looped;

		return i;
	}

	return -1;
}

int SpriteAnim_CreatePlayer(int anim_pattern_id)
{
	for (int i = 0; i < ANIM_PLAY_MAX; i++)
	{
		if (g_AnimPlayData[i].m_PatternId >= 0) continue;
		
		g_AnimPlayData[i].m_PatternId = anim_pattern_id;
		g_AnimPlayData[i].m_Accumulated_time = 0.0;
		g_AnimPlayData[i].m_PatternNum = 0;
		g_AnimPlayData[i].m_IsStopped = false;

		return i;
	}

	return -1;
}

bool SpriteAnim_IsStopped(int index)
{
	return g_AnimPlayData[index].m_IsStopped;
}

void SpriteAnim_DestroyPlayer(int index)
{
	g_AnimPlayData[index].m_PatternId = -1;
}


