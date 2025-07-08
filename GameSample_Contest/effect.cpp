/*==============================================================================

�@ �G�t�F�N�g�̕`�� [effect.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/07/04
--------------------------------------------------------------------------------

==============================================================================*/
#include "effect.h"
#include "sprite_anim.h"
#include "texture.h"

#include <DirectXMath.h>
using namespace DirectX;

struct Effect
{
    XMFLOAT2 position;
    int sprite_anim_id;
    bool isEnable;
};

static constexpr unsigned int EFFECTS_MAX = 256;
static Effect g_Effects[EFFECTS_MAX] = {};
static int g_EffectTexId = -1;
static int g_AnimPatternId = -1;

void Effect_Initialize()
{
    for (Effect& e : g_Effects)
    {
        e.isEnable = false;
    }

    g_EffectTexId = Texture_Load(L"resources/explosion_2.png");
    g_AnimPatternId = SpriteAnim_RegisterPattern(g_EffectTexId, 16, 4, 0.08, { 200, 200 }, {0, 0}, false);
}

void Effect_Finalize()
{
}

void Effect_Update(double elapsed_time)
{
    for (Effect& e : g_Effects)
    {
        if (!e.isEnable) continue;

        if (SpriteAnim_IsStopped(e.sprite_anim_id))
        {
            SpriteAnim_DestroyPlayer(e.sprite_anim_id);
            e.isEnable = false;
        }
    }
}

void Effect_Draw()
{
    for (Effect& e : g_Effects)
    {
        if (!e.isEnable) continue;

        SpriteAnim_Draw(e.sprite_anim_id, e.position.x, e.position.y, 70.0f, 70.0f);
    }
}

void Effect_Create(const XMFLOAT2& position)
{
    for (Effect& e : g_Effects)
    {
        if (e.isEnable) continue; // �g�p���̂̓X�L�b�v

        // �󂫗̈攭��
        e.isEnable = true;
        e.position = position;
        e.sprite_anim_id = SpriteAnim_CreatePlayer(g_AnimPatternId);
        break;
    }
}
