// ==========================================================================================
// 
// File Name: player.cpp
// Date: 2025/08/13
// Author: Gu Anyi
// Description: Player class header file
// 
// ==========================================================================================
#ifndef PLAYER_H
#define PLAYER_H

#include "collision.h"
#include "camera.h"

#include <DirectXMath.h>

enum Status {
	none, // for initialization
	stand,
	walkFront,
	walkLeft,
	walkBack,
	walkRight,
	STATUS_MAX
};

class Player
{
private:

	DirectX::XMFLOAT2 playerPosition;
	DirectX::XMFLOAT2 playerVelocity;
	DirectX::XMFLOAT2 playerSize;
	bool playerFlip;
	int playerTexId;
	int playerAnimPlayId;
	Circle playerCollision;
	bool playerEnable;
	Status playerStatus;

public:
	Player();

	void Initialize(const DirectX::XMFLOAT2& position);
	void Finalize();

	void Update(double elapsed_time);
	void Draw(const ViewRect& viewRect);
	void ChangeStatus(Status newPlayerStatus);

	bool IsEnable();
	Circle GetCollision();
	DirectX::XMFLOAT2 GetPosition();
	DirectX::XMFLOAT2 GetSize();
	void Destroy();
};

#endif // PLAYER_H
