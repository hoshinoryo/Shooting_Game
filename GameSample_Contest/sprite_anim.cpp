// ==========================================================================================
// 
// File Name: sprite_anim.cpp
// Date: 2025/08/17
// Author: Gu Anyi
// Description: Manages sprite animations dynamically using vectors and shared_ptr for flexible, 
//				automatic resource handling
// 
// ==========================================================================================

#include "sprite_anim.h"
#include "sprite.h"

#include <DirectXMath.h>
#include <vector>
#include <memory>

#include "debug_text.h"
#include "debug_ostream.h"

using namespace DirectX;


struct AnimPatternData
{
	int m_PatternMax = 0;
	int m_HPatternMax = 0;
	XMFLOAT2 m_StartPosition = { 0, 0 };
	XMFLOAT2 m_PatternSize = { 0.0f, 0.0f };
	double m_seconds_per_pattern = 0.1;
	bool m_IsLooped = true;
	std::shared_ptr<Texture> m_Texture;

	void Reset()
	{
		m_Texture.reset();
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
	int m_PatternId = -1;
	int m_PatternNum = 0;
	double m_Accumulated_time = 0.0; // ó›êœéûä‘
	bool m_IsStopped = false;
};

static std::vector<std::shared_ptr<AnimPatternData>> g_AnimPattern;
static constexpr int ANIM_PLAY_MAX = 256;
static AnimPlayData g_AnimPlayData[ANIM_PLAY_MAX];

void SpriteAnim_Initialize()
{
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
	g_AnimPattern.clear();
}

void SpriteAnim_Update(double elapsed_time)
{
	for (int i = 0; i < ANIM_PLAY_MAX; i++)
	{
		if (g_AnimPlayData[i].m_PatternId < 0) continue;
		int pattern_id = g_AnimPlayData[i].m_PatternId;
		if (pattern_id >= (int)g_AnimPattern.size()) continue;

		auto pattern = g_AnimPattern[pattern_id];
		if (!pattern) continue;

		if (g_AnimPlayData[i].m_Accumulated_time >= pattern->m_seconds_per_pattern)
		{
			g_AnimPlayData[i].m_PatternNum++;

			if (g_AnimPlayData[i].m_PatternNum >= pattern->m_PatternMax)
			{
				if (pattern->m_IsLooped)
				{
					g_AnimPlayData[i].m_PatternNum = 0;
				}
				else
				{
					g_AnimPlayData[i].m_PatternNum = pattern->m_PatternMax - 1;
					g_AnimPlayData[i].m_IsStopped = true;
				}
			}
			
			g_AnimPlayData[i].m_Accumulated_time -= pattern->m_seconds_per_pattern;
		}

		g_AnimPlayData[i].m_Accumulated_time += elapsed_time; // åoâﬂéûä‘Çâ¡éZÇ∑ÇÈ
	}
}

void SpriteAnim_Draw(int playid, float dx, float dy, float dw, float dh, bool isFlipX, const XMFLOAT4& color)
{
	if (playid < 0 || playid >= ANIM_PLAY_MAX) return;
	int anim_pattern_id = g_AnimPlayData[playid].m_PatternId;
	if (anim_pattern_id < 0 || anim_pattern_id >= (int)g_AnimPattern.size()) return;

	auto pAnimPatternData = g_AnimPattern[anim_pattern_id];
	if (!pAnimPatternData || !pAnimPatternData->m_Texture) return;

	float px = pAnimPatternData->m_StartPosition.x +
		pAnimPatternData->m_PatternSize.x *
		(g_AnimPlayData[playid].m_PatternNum % pAnimPatternData->m_HPatternMax);
	float py = pAnimPatternData->m_StartPosition.y +
		pAnimPatternData->m_PatternSize.y *
		(g_AnimPlayData[playid].m_PatternNum / pAnimPatternData->m_HPatternMax);
	float pw = pAnimPatternData->m_PatternSize.x;
	float ph = pAnimPatternData->m_PatternSize.y;

	Sprite_Draw(*pAnimPatternData->m_Texture, dx, dy, dw, dh, (int)px, (int)py, (int)pw, (int)ph, isFlipX, color);
}

int SpriteAnim_RegisterPattern(const Texture& tex, int patternMax, int h_pattern_max, double seconds_per_pattern,
	const DirectX::XMFLOAT2& pattern_size,
	const DirectX::XMFLOAT2& start_position, bool is_looped)
{
	auto pattern = std::make_shared<AnimPatternData>();
	pattern->m_Texture = std::make_shared<Texture>(tex);
	pattern->m_PatternMax = patternMax;
	pattern->m_HPatternMax = h_pattern_max;
	pattern->m_seconds_per_pattern = seconds_per_pattern;
	pattern->m_PatternSize = pattern_size;
	pattern->m_StartPosition = start_position;
	pattern->m_IsLooped = is_looped;

	g_AnimPattern.push_back(pattern);

	return (int)(g_AnimPattern.size() - 1);
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
	if (index < 0 || index >= ANIM_PLAY_MAX) return true;
	return g_AnimPlayData[index].m_IsStopped;
}

void SpriteAnim_DestroyPlayer(int index)
{
	if (index < 0 || index >= ANIM_PLAY_MAX) return;
	g_AnimPlayData[index].m_PatternId = -1;
}


