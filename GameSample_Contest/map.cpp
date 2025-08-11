// ==========================================================================================
// 
// File Name: map.h
// Date: 2025/08/09
// Author: Gu Anyi
// Description: Renders only the visible portion of the map based on the camera's position
// 
// ==========================================================================================

#include "map.h"
#include "sprite.h"
#include "texture.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


Map::Map()
{
	mapTexId = -1;
	std::fill(std::begin(mapArray), std::end(mapArray), 0);
	mapWidth = MAPCHIP_WIDTH * MAP_H_COUNT;
	mapHeight = MAPCHIP_HEIGHT * MAP_V_COUNT;
}

Map::~Map()
{
}

void Map::Initialize()
{
	mapTexId = Texture_Load(L"resources/Christmass_Grass.png");

	// Import tiled csv file
	std::ifstream map_file("resources/Tiled_Project/output/test_map_mg.csv");
	if (!map_file)
	{
		MessageBox(nullptr, "マップファイルの読み込みに失敗しました", "Error", MB_OK);
	}

	std::string line;
	int index = 0;

	while (std::getline(map_file, line))
	{
		std::stringstream ss(line);
		std::string cell;
		while (std::getline(ss, cell, ','))
		{
			mapArray[index++] = std::stoi(cell);
		}
	}
}

void Map::Finalize()
{
}

void Map::Update(float elapsed_time)
{
}

void Map::Draw(const ViewRect& viewRect)
//void Map::Draw()
{
	// Starting map chip index
	int offsetX = static_cast<int>(viewRect.rectPosition.x) / MAPCHIP_WIDTH;
	int offsetY = static_cast<int>(viewRect.rectPosition.y) / MAPCHIP_HEIGHT;

	// Local offset from starting map chip
	int localOffsetX = -(viewRect.rectPosition.x - offsetX * MAPCHIP_WIDTH);
	int localOffsetY = -(viewRect.rectPosition.y - offsetY * MAPCHIP_HEIGHT);

	int horizontalCount = static_cast<int>(viewRect.rectWidth / MAPCHIP_WIDTH) + 2;
	int verticalCount = static_cast<int>(viewRect.rectHeight / MAPCHIP_HEIGHT) + 2;

	for (int y = 0; y < verticalCount; y++)
	{
		for (int x = 0; x < horizontalCount; x++)
		{
			int mapX = offsetX + x;
			int mapY = offsetY + y;

			if (mapX < 0 || mapX >= MAP_H_COUNT || mapY < 0 || mapY >= MAP_V_COUNT) continue;

			int chipId = mapArray[mapY * MAP_H_COUNT + mapX];

			// Map chip position in screen space
			float chipPosX = (float)(x * MAPCHIP_WIDTH) + localOffsetX;
			float chipPosY = (float)(y * MAPCHIP_HEIGHT) + localOffsetY;

			int chipIndexX = chipId % 8;
			int chipIndexY = chipId / 8;

			Sprite_Draw(mapTexId, chipPosX, chipPosY, MAPCHIP_WIDTH, MAPCHIP_HEIGHT, 64 * chipIndexX, 64 * chipIndexY, 64.0f, 64.0f);
		}
	}
}

