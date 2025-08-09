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

#include <iostream>
#include <DirectXMath.h>

using namespace DirectX;

static XMFLOAT2 g_CameraPosition = { 0.0f, 0.0f };

void Camera_Initialize()
{
	g_CameraPosition = { 0.0f, 0.0f };
}

void Camera_Finalize()
{
}

void Camera_Update(const XMFLOAT2& target, float screenWidth, float screenHeight, float mapWidth, float mapHeight)
{
    g_CameraPosition.x = target.x - screenWidth * 0.5f;
    g_CameraPosition.y = target.y - screenHeight * 0.5f;

    if (g_CameraPosition.x < 0.0f)
        g_CameraPosition.x = 0.0f;
    if (g_CameraPosition.y < 0.0f)
        g_CameraPosition.y = 0.0f;
    if (g_CameraPosition.x > mapWidth - screenWidth)
        g_CameraPosition.x = mapWidth - screenWidth;
    if (g_CameraPosition.y > mapHeight - screenHeight)
        g_CameraPosition.y = mapHeight - screenHeight;

	std::cout << "Camera Position: (" << g_CameraPosition.x << ", " << g_CameraPosition.y << ")" << std::endl;
}

void Camera_Set()
{
    XMMATRIX view = XMMatrixTranslation(-g_CameraPosition.x, -g_CameraPosition.y, 0.0f);

    Shader_SetProjectionMatrix(view);
}

XMFLOAT2 Camera_GetPosition()
{
    return g_CameraPosition;
}
