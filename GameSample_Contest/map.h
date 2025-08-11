// ==========================================================================================
// 
// File Name: map.h
// Date: 2025/08/09
// Author: Gu Anyi
// Description: Map class header file
// 
// ==========================================================================================

#ifndef MAP_H
#define MAP_H

#include "camera.h"

#include <DirectXMath.h>

using namespace DirectX;

class Map
{
private:

	static constexpr unsigned int MAPCHIP_WIDTH = 64;
	static constexpr unsigned int MAPCHIP_HEIGHT = 64;
	static constexpr unsigned int MAP_H_COUNT = 50;
	static constexpr unsigned int MAP_V_COUNT = 30;

	int mapTexId;
	int mapArray[MAP_H_COUNT * MAP_V_COUNT];

	int mapWidth;
	int mapHeight;

public:
	Map();
	~Map();

	void Initialize();
	void Finalize();

	void Update(float elapsed_time);
	void Draw(const ViewRect& viewRect);
	//void Draw();

	int GetMapWidth() const { return mapWidth; }
	int GetMapHeight() const { return mapHeight; }
};

#endif // MAP_H