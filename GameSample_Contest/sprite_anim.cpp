/*==============================================================================

   �X�v���C�g�A�j���[�V�����`�� [sprite_anim.cpp]
														 Author : Youhei Sato
														 Date   : 2025/06/17
--------------------------------------------------------------------------------

==============================================================================*/
#include "sprite_anim.h"
#include "sprite.h"
#include "texture.h"
#include <DirectXMath.h>
using namespace DirectX;


struct AnimPatternData
{
	int m_TextureId = -1;    // �e�N�X�`��ID
	int m_PatternMax = 0;    // �A�j���̃p�^�[����
	int m_HPatternMax = 0;    // �A�j���̉������̃p�^�[����
	XMUINT2 m_StartPosition = { 0, 0 }; // �A�j���[�V�����̃X�^�[�g���W
	XMUINT2 m_PatternSize = { 0, 0 };   // 1�p�^�[���̃T�C�Y
	double m_seconds_per_pattern = 0.1; // 1�p�^�[���̍Đ�����
	bool m_IsLooped = true;  // ���[�v���邩
};

struct AnimPlayData
{
	int m_PatternId = -1;    // �A�j���[�V�����p�^�[��ID�A�g���p�^�[���͂ǂ���
	int m_PatternNum = 0;    // ���ݍĐ����̃p�^�[���ԍ��A��ɕς���Ă���
	double m_Accumulated_time = 0.0; // �ݐώ���
	bool m_IsStopped = false;
};

static constexpr int ANIM_PATTERN_MAX = 128;
static AnimPatternData g_AnimPattern[ANIM_PATTERN_MAX];
static constexpr int ANIM_PLAY_MAX = 256;
static AnimPlayData g_AnimPlayData[ANIM_PLAY_MAX];

void SpriteAnim_Initialize()
{
	// �A�j���[�V�����p�^�[���Ǘ������������i�S�ė��p���Ă��Ȃ��j�󋵂ɂ���

	for (AnimPatternData& data : g_AnimPattern)
	{
		data.m_TextureId = -1; // -1��������g���Ȃ�
	}

	for (AnimPlayData& data : g_AnimPlayData)
	{
		data.m_PatternId = -1; // -1��������g���Ȃ�
		data.m_IsStopped = false;
	}
}

void SpriteAnim_Finalize()
{
}

void SpriteAnim_Update(double elapsed_time)
{
	// 0.1�b�ȏソ������A�j���[�V������i�߂�
	for (int i = 0; i < ANIM_PLAY_MAX; i++)
	{
		if (g_AnimPlayData[i].m_PatternId < 0) continue; // �f�[�^���Ȃ��Ƃ���ōĐ��K�v���Ȃ�

		// m_PatternId���g���΁A�����A�j���𕡐��̍Đ��f�[�^�Ŏg���񂹂�
		// g_AnimPattern[i]�ɂ���ƌŒ肳��āA�_��ɐ؂�ւ����Ȃ��Ȃ�
		AnimPatternData* pAnimPatternData = &g_AnimPattern[g_AnimPlayData[i].m_PatternId]; // �������ďk�߂悤�ɂ���

		if (g_AnimPlayData[i].m_Accumulated_time >= pAnimPatternData->m_seconds_per_pattern) // �ݐώ��Ԃ��p�^�[���̍Đ����Ԃ𒴂�����
		{
			g_AnimPlayData[i].m_PatternNum++; // �p�^�[���������̃p�^�[���ɍX�V

			if (g_AnimPlayData[i].m_PatternNum >= pAnimPatternData->m_PatternMax) // �Ō�̃p�^�[���ɗ�����
			{
				if (pAnimPatternData->m_IsLooped)
				{
					g_AnimPlayData[i].m_PatternNum = 0; // ���[�v����Ȃ�ŏ��̃p�^�[���ԍ��ɖ߂�
				}
				else
				{
					g_AnimPlayData[i].m_PatternNum = pAnimPatternData->m_PatternMax - 1; // ���[�v���Ȃ��Ƃ��̂܂ܕۂ�
					g_AnimPlayData[i].m_IsStopped = true;
				}
			}
			
			g_AnimPlayData[i].m_Accumulated_time -= pAnimPatternData->m_seconds_per_pattern;
		}

		g_AnimPlayData[i].m_Accumulated_time += elapsed_time; // �o�ߎ��Ԃ����Z����
	}
}

void SpriteAnim_Draw(int playid, float dx, float dy, float dw, float dh)
{
	int anim_pattern_id = g_AnimPlayData[playid].m_PatternId;
	AnimPatternData* pAnimPatternData = &g_AnimPattern[anim_pattern_id];

	// ����s�̉摜���A�j���ɂ���Asprite.cpp�̊֐����g��
	Sprite_Draw(pAnimPatternData->m_TextureId,
		dx, dy, dw, dh,
		pAnimPatternData->m_StartPosition.x + pAnimPatternData->m_PatternSize.x * (g_AnimPlayData[playid].m_PatternNum % pAnimPatternData->m_HPatternMax),
		pAnimPatternData->m_StartPosition.y + pAnimPatternData->m_PatternSize.y * (g_AnimPlayData[playid].m_PatternNum / pAnimPatternData->m_HPatternMax),
		pAnimPatternData->m_PatternSize.x,
		pAnimPatternData->m_PatternSize.y
	);
}

int SpriteAnim_RegisterPattern(int texId, int patternMax, int h_pattern_max, double seconds_per_pattern,
	const DirectX::XMUINT2& pattern_size,
	const DirectX::XMUINT2& start_position, bool is_looped)
{
	// �O��Initialize()�̒��ɏ����Ă���R�[�h���ʂ̊֐��ɓƗ�����

	for (int i = 0; i < ANIM_PATTERN_MAX; i++)
	{
		// �󂢂Ă�ꏊ��T��
		if (g_AnimPattern[i].m_TextureId >= 0) continue;

		g_AnimPattern[i].m_TextureId = texId;
		g_AnimPattern[i].m_PatternMax = patternMax;
		g_AnimPattern[i].m_HPatternMax = h_pattern_max;
		g_AnimPattern[i].m_seconds_per_pattern = seconds_per_pattern;
		g_AnimPattern[i].m_PatternSize = pattern_size;
		g_AnimPattern[i].m_StartPosition = start_position;
		g_AnimPattern[i].m_IsLooped = is_looped;

		return i; // �Ǘ��ԍ���߂�
	}

	return -1;
}

int SpriteAnim_CreatePlayer(int anim_pattern_id)
{
	for (int i = 0; i < ANIM_PLAY_MAX; i++)
	{
		if (g_AnimPlayData[i].m_PatternId >= 0) continue; // �󂢂ĂȂ�
		
		g_AnimPlayData[i].m_PatternId = anim_pattern_id;
		g_AnimPlayData[i].m_Accumulated_time = 0.0;
		g_AnimPlayData[i].m_PatternNum = 0;

		// �A�j���Đ��f�[�^���ė��p����ꍇ�A�O��̒�~��Ԃ��c���Ă���\��������B
		// �V�����Đ����J�n���邽�߁A�����I�ɍĐ����ifalse�j�Ƀ��Z�b�g����K�v������B
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
