/*==============================================================================

�@ �L�[�{�[�h���͂̋@�\[key_logger.cpp]
														 Author : Youhei Sato
														 Date   : 2025/06/27
--------------------------------------------------------------------------------

==============================================================================*/
#include "key_logger.h"

static Keyboard_State g_PrevState{};
static Keyboard_State g_TriggerState{};
static Keyboard_State g_ReleaseState{};


void KeyLogger_Initialize()
{
	Keyboard_Initialize();
}

void KeyLogger_Update()
{
	const Keyboard_State* pState = Keyboard_GetState(); // ���݂̃L�[���͏�Ԃ��擾�i������Ă���L�[���擾�j
	LPBYTE pn = (LPBYTE)pState;             // ���݂̃L�[��Ԃ��o�C�g�z��Ƃ��Ĉ����i��r�̂��߁j
	LPBYTE pp = (LPBYTE)&g_PrevState;       // �O�̃t���[���̃L�[��ԁi�����E����̔���Ɏg���j
	LPBYTE pt = (LPBYTE)&g_TriggerState;    // ����̃t���[���Łu�V���ɉ����ꂽ�v�L�[���L�^����z��
	LPBYTE pr = (LPBYTE)&g_ReleaseState;    // ����̃t���[���Łu�����ꂽ�i�����Ă��������͗����ꂽ�j�v�L�[���L�^����z��

	for (int i = 0; i < sizeof(Keyboard_State); i++)
	{
		// 0�������Ă�A1�������ĂȂ�
		// 0 1 -> 1
		// 1 0 -> 0
		// 1 1 -> 0
		// 0 0 -> 0
		pt[i] = (pp[i] ^ pn[i]) & pn[i];
		// 0 1 -> 0
		// 1 0 -> 1
		// 1 1 -> 0
		// 0 0 -> 0
		pr[i] = (pp[i] ^ pn[i]) & pn[i];
	}

	g_PrevState = *pState; // �L�[��ԍX�V
}

bool KeyLogger_IsPressed(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key);
}

bool KeyLogger_IsTrigger(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key, &g_TriggerState);
}

bool KeyLogger_IsRelease(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key, &g_ReleaseState);
}
