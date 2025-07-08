/*==============================================================================

   ÉJÉÅÉâê›íË [camera.cpp]
														 Author : GU ANYI
														 Date   : 2025/07/08
--------------------------------------------------------------------------------

==============================================================================*/
#include "camera.h"
#include "shader.h"

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
}

void Camera_Set()
{
    XMMATRIX view = XMMatrixTranslation(-g_CameraPosition.x, -g_CameraPosition.y, 0.0f);
    Shader_SetWorldMatrix(view);
}

XMFLOAT2 Camera_GetPosition()
{
    return g_CameraPosition;
}
