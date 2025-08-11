// ==========================================================================================
// 
// File Name: camera.h
// Date: 2025/08/08
// Author: Gu Anyi
// Description: Camera class header file
// 
// ==========================================================================================

#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

struct ViewRect
{
	DirectX::XMFLOAT2 rectPosition;
	float rectWidth;
	float rectHeight;
};

class Camera
{
private:
	// Camera top-left in map space
	DirectX::XMFLOAT2 position;
	float screenWidth;
	float screenHeight;
	float mapWidth;
	float mapHeight;

public:
	Camera(); // コンストラクター

	void Initialize(float sw, float sh, float mw, float mh);
	void Finalize();
	void Update(const DirectX::XMFLOAT2& target);

	float GetX() const { return position.x; }
	float GetY() const { return position.y; }
	float GetScreenWidth() const { return screenWidth; }
	float GetScreenHeight() const { return screenHeight; }

	ViewRect GetViewRect() const;
};

#endif // CAMERA_H
