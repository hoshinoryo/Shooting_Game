// ==========================================================================================
// 
// File Name: camera.h
// Date: 2025/08/08
// Author: Gu Anyi
// Description: Calculates the visible area in the map based on the target position and screen size
// 
// ==========================================================================================

#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

void Camera_Initialize();
void Camera_Finalize();

void Camera_Update(const DirectX::XMFLOAT2& target, float screenWidth, float screenHeight, float mapWidth, float mapHeight);
void Camera_Set(); // View Matrix
DirectX::XMFLOAT2 Camera_GetPosition();


#endif // CAMERA_H
