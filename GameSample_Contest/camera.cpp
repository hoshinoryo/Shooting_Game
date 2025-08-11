// ==========================================================================================
// 
// File Name: camera.cpp
// Date: 2025/08/08
// Author: Gu Anyi
// Description: Calculates the visible area in the map based on the target position and screen size
// 
// ==========================================================================================

#include "camera.h"
#include "shader.h"

#include <DirectXMath.h>

using namespace DirectX;

Camera::Camera()
{
	position = { 0.0f, 0.0f };
	screenWidth = -1;
	screenHeight = -1;
	mapWidth = -1;
	mapHeight = -1;
}

void Camera::Initialize(float sw, float sh, float mw, float mh)
{
	screenWidth = sw;
	screenHeight = sh;
	mapWidth = mw;
	mapHeight = mh;
}

void Camera::Finalize()
{
}

void Camera::Update(const XMFLOAT2& target)
{
	// Calculate the new camera position based on the target position
	position.x = target.x - screenWidth / 2.0f;
	position.y = target.y - screenHeight / 2.0f;

	// Clamp the camera position to ensure it doesn't go out of bounds of the map
	if (position.x < 0) position.x = 0;
	if (position.y < 0) position.y = 0;
	if (position.x + screenWidth > mapWidth) position.x = mapWidth - screenWidth;
	if (position.y + screenHeight > mapHeight) position.y = mapHeight - screenHeight;
}

// 
ViewRect Camera::GetViewRect() const
{
	return ViewRect{ position, screenWidth, screenHeight };
}
