// ==========================================================================================
// 
// File Name: ui_element.h
// Date: 2025/08/28
// Author: Gu Anyi
// Description: UI element system header file, with ScoreUI class
// 
// ==========================================================================================

#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include "texture.h"
#include "player.h"

#include <DirectXMath.h>
#include <vector>

class UIElement
{
protected:

	DirectX::XMFLOAT2 screenPosition;

public:

	virtual ~UIElement() {}
	virtual void Update(double elapsed_time) = 0;
	virtual void Draw() = 0;
};


class ScoreUI : public UIElement
{
private:

	unsigned int realScore;
	unsigned int viewScore;
	unsigned int counterStop;
	int digit;
	Texture scoreTex;

public:

	ScoreUI();

	void Initialize(DirectX::XMFLOAT2 pos, int digit);
	void Finalize();
	void Update(double elapsed_time) override;
	void Draw() override;

	unsigned int GetScore();
	void AddScore(int score);
	void ResetScore();

	void DrawNumber(DirectX::XMFLOAT2 pos, int number);
};

class StateUI : public UIElement
{
private:

	Player* player;

public:

	StateUI();

	void Initialize(DirectX::XMFLOAT2 pos);
	void Finailize();
	void Update(double elapsed_time) override;
	void Draw() override;

	void BindPlayer(Player* p);
	float GetHP();
};

//---------------------------------
// UI Manager
//---------------------------------

class UIManager
{
private:

	std::vector<UIElement*> elements;

public:

	void Add(UIElement* element) { elements.push_back(element); }
	void Update(double elapsed_time);
	void Draw();
};


#endif // UI_ELEMENT_H