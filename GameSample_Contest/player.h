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
#include "map.h"

#include <DirectXMath.h>

enum Status {
	none, // for initialization
	walkFront,
	stopFront,
	walkLeft,
	stopLeft,
	walkBack,
	stopBack,
	walkRight,
	stopRight,
	STATUS_MAX
};

class Player
{
private:

	DirectX::XMFLOAT2 playerWorldPosition;
	DirectX::XMFLOAT2 playerScreenPosition;
	DirectX::XMFLOAT2 playerVelocity;
	DirectX::XMFLOAT2 playerSize;

	Texture playerTex;
	bool playerFlip;
	int playerAnimPlayId;
	Circle playerCircleCollision;
	Box playerBoxCollision;
	bool playerEnable;

	int playerHp;
	bool playerDamaged;
	float damagedTimer;
	bool damagedStatusApplied;

	Status playerStatus;
	Status lastMoveStatus;

public:

	Player();

	void Initialize(const DirectX::XMFLOAT2& position);
	void Finalize();

	void UpdatePosition(double elapsed_time, Collision_Map& map);
	void UpdateStatus();
	void ChangeStatus(Status newPlayerStatus, bool forceDamaged);
	void Update(double elapsed_time, Collision_Map& map, const ViewRect& viewRect);
	
	DirectX::XMFLOAT2 GetShootDirection();
	void Shoot(double elapsed_time);
	void Draw(const ViewRect& viewRect);

	bool GetIsEnable();
	int GetHp();

	Circle GetCircleCollision();
	Box GetBoxCollision();

	DirectX::XMFLOAT2 GetWorldPosition();
	DirectX::XMFLOAT2 GetScreenPosition();
	DirectX::XMFLOAT2 GetSize();
	
	void SetWorldPosition(DirectX::XMFLOAT2& position);
	void SetScreenPosition(const ViewRect& viewRect);

	void Damage();
	void Destroy();
};

#endif // PLAYER_H
