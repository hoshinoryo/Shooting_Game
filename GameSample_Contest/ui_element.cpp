// ==========================================================================================
// 
// File Name: ui_element.cpp
// Date: 2025/08/28
// Author: Gu Anyi
// Description: ScoreUI describtion
// 
// ==========================================================================================

#include "ui_element.h"
#include "direct3d.h"
#include "sprite.h"

#include <algorithm>

using namespace DirectX;

static constexpr float FONT_W = 32.0f;
static constexpr float FONT_H = 32.0f;

//---------------------------------
// Score UI Methods
//---------------------------------

ScoreUI::ScoreUI()
{
	realScore = 0;
	viewScore = 0;
	counterStop = 1;
	digit = 0;
	screenPosition = {};
}

void ScoreUI::Initialize(XMFLOAT2 pos, int digit)
{
	realScore = 0;
	viewScore = 0;
	counterStop = 1;
	this->digit = digit;
	screenPosition = pos;

	for (int i = 0; i < digit; i++)
	{
		counterStop *= 10;
	}
	counterStop--;

	scoreTex.Initialize(Direct3D_GetDevice(), L"resources/number.png");
}

void ScoreUI::Finalize()
{
	scoreTex.Finalize();
}

void ScoreUI::Update(double elapsed_time)
{
	viewScore = std::min(viewScore + 1, realScore);
}

void ScoreUI::Draw()
{
	unsigned int temp = std::min(viewScore, counterStop);

	for (int i = 0; i < digit; i++)
	{
		int n = temp % 10;
		float x = screenPosition.x + (digit - i - 1) * FONT_W;

		DrawNumber({ x, screenPosition.y }, n);

		temp /= 10;
	}
}

unsigned int ScoreUI::GetScore()
{
	return realScore;
}

void ScoreUI::AddScore(int score)
{
	viewScore = realScore;
	realScore += score;
}

void ScoreUI::ResetScore()
{
	realScore = 0;
	viewScore = 0;
}

void ScoreUI::DrawNumber(XMFLOAT2 pos, int number)
{
	Sprite_Draw(scoreTex, pos.x, pos.y, FONT_W * number, 0, FONT_W, FONT_H);
}


//---------------------------------
// State UI Methods
//---------------------------------

static Texture ScoreWordTex;
static Texture HPWordTex;
static Texture HPSliderTex;
static Texture StateBGTex;

StateUI::StateUI()
{
	screenPosition = {};
}

void StateUI::Initialize(DirectX::XMFLOAT2 pos)
{
	screenPosition = pos;
	ScoreWordTex.Initialize(Direct3D_GetDevice(), L"resources/Score.png");
	HPWordTex.Initialize(Direct3D_GetDevice(), L"resources/HP.png");
	HPSliderTex.Initialize(Direct3D_GetDevice(), L"resources/HP_Slider.png");
	StateBGTex.Initialize(Direct3D_GetDevice(), L"resources/State_BG.png");
}

void StateUI::Finailize()
{
	StateBGTex.Finalize();
	HPSliderTex.Finalize();
	HPWordTex.Finalize();
	ScoreWordTex.Finalize();
}

void StateUI::Update(double elapsed_time)
{
}

void StateUI::Draw()
{
	Sprite_Draw(StateBGTex, screenPosition.x, screenPosition.y);
	Sprite_Draw(HPWordTex, screenPosition.x + 33.0f, screenPosition.y + 33.0f);

	// Player hp slider
	if (player)
	{
		float hp = player->GetHp();
		float posY = (5 - hp) * (HPSliderTex.GetHeight() / 6);
		Sprite_Draw(HPSliderTex, screenPosition.x + 170.0f, screenPosition.y + 20.0f,
			240.0f, 52.0f, 0.0f, posY, HPSliderTex.GetWidth(), HPSliderTex.GetHeight() / 6);
	}

	Sprite_Draw(ScoreWordTex, screenPosition.x + 33.0f, screenPosition.y + 83.0f);
}

void StateUI::BindPlayer(Player* p)
{
	player = p;
}

float StateUI::GetHP()
{
	return 0.0f;
}


//---------------------------------
// UI Manager Methods
//---------------------------------

void UIManager::Update(double elapsed_time)
{
	for (auto e : elements)
	{
		e->Update(elapsed_time);
	}
}

void UIManager::Draw()
{
	for (auto e : elements)
	{
		e->Draw();
	}
}