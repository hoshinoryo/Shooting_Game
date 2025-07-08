/*==============================================================================

�@ �Q�[���{��[game.h]
                                                         Author : Youhei Sato
                                                         Date   : 2025/06/27
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef GAME_H
#define GAME_H


void Game_Initialize();
void Game_Finalize();

// �Q�[�����̂͐�ΕK�v�̃A�b�v�f�[�g�ƃh���[(�Œ��)
void Game_Update(double elapsed_time);
void Game_Draw();

void hitJudgementBulletVSEnemy();
void hitJudgementPlayerVSEnemy();


#endif // GAME_H
