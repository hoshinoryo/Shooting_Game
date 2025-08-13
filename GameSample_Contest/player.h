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

	DirectX::XMFLOAT2 playerWorldPosition;
	DirectX::XMFLOAT2 playerScreenPosition;
	DirectX::XMFLOAT2 playerVelocity;
	DirectX::XMFLOAT2 playerSize;
	bool playerFlip;
	int playerTexId;
	int playerAnimPlayId;
	Circle playerCircleCollision;
	bool playerEnable;
	Status playerStatus;

public:
	Player();

	void Initialize(const DirectX::XMFLOAT2& position);
	void Finalize();

	void Update(double elapsed_time, const ViewRect& viewRect);
	void Draw();
	void ChangeStatus(Status newPlayerStatus);

	bool IsEnable();

	void SetScreenPosition(const ViewRect& viewRect);

	Circle GetCircleCollision();
	Box GetBoxCollision();

	DirectX::XMFLOAT2 GetWorldPosition();
	DirectX::XMFLOAT2 GetScreenPosition();
	DirectX::XMFLOAT2 GetSize();

	void Destroy();
};

#endif // PLAYER_H
