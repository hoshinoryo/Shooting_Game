/*==============================================================================

�@ �G�̔������� [enemy_spawner.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2025/07/02
--------------------------------------------------------------------------------

==============================================================================*/
#include "enemy_spawner.h"
#include "debug_ostream.h"

#include <DirectXMath.h>
using namespace DirectX;

struct EnemySpawn
{
    XMFLOAT2 position;
    EnemyTypeID id;
    int count; // �����ő吔
    double time; // ��������s����
    double rate; // �����Ԋu�i1�̂��Ƃɉ��b�����ďo�����j
    double spawn_time; // ���ɐ�������\��̎��ԁi���^�C���X�^���v�j
    int spawn_count; // ���݂܂łɏo���������G�̐�
    bool isFinished; // �ő吔�܂ŏo���I������
};

static constexpr unsigned int ENEMY_SPAWNER_MAX = 100;
static EnemySpawn g_EnemySpawners[ENEMY_SPAWNER_MAX] = {};
static int g_SpawnerCount = 0; // �o�^��
static double g_Time = 0.0f; // �O���[�o���ϐ�

void EnemySpawner_Initialize()
{
    g_SpawnerCount = 0;
    g_Time = 0.0;
}

void EnemySpawner_Finalize()
{
}

void EnemySpawner_Update(double elapsed_time)
{
    g_Time += elapsed_time; // ���݂̌o�ߎ��ԁi�b�j��\���S�̃^�C�}�[

    for (int i = 0; i < g_SpawnerCount; i++)
    {
        if (g_EnemySpawners[i].isFinished) continue; // �I��������͐G��Ȃ�
        if (g_EnemySpawners[i].time > g_Time) break;

        if (g_EnemySpawners[i].spawn_count == 0)
        {
            g_EnemySpawners[i].spawn_time = g_Time - g_EnemySpawners[i].rate - 0.00001; // double�̌덷
        }

        if ((g_Time - g_EnemySpawners[i].spawn_time) >= g_EnemySpawners[i].rate)
        {
            Enemy_Create(g_EnemySpawners[i].id, g_EnemySpawners[i].position);
            g_EnemySpawners[i].spawn_count++;

            if (g_EnemySpawners[i].spawn_count >= g_EnemySpawners[i].count)
            {
                g_EnemySpawners[i].isFinished = true;
            }
            g_EnemySpawners[i].spawn_time = g_Time;
        }
    }
}

void EnemySpawner_Create(const XMFLOAT2& position, EnemyTypeID id, double spawn_time, double spawn_rate, int spawn_count)
{
    if (g_SpawnerCount >= ENEMY_SPAWNER_MAX) return;

    EnemySpawn* pEs = &g_EnemySpawners[g_SpawnerCount];
    pEs->position = position;
    pEs->id = id;
    pEs->time = spawn_time;
    pEs->rate = spawn_rate;
    pEs->count = spawn_count;
    pEs->isFinished = false;
    pEs->spawn_count = 0;
    pEs->spawn_time = 0.0;
    g_SpawnerCount++;
    hal::dout << g_SpawnerCount << std::endl;
}