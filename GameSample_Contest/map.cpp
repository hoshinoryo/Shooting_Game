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
	std::fill(std::begin(mapArray), std::end(mapArray), -1); // default are all -1
	mapWidth = MAPCHIP_WIDTH * MAP_H_COUNT;
	mapHeight = MAPCHIP_HEIGHT * MAP_V_COUNT;
}

Map::~Map()
{
}

void Map::Initialize(const std::string& filePath)
{
	mapTexId = Texture_Load(L"resources/Christmass_Grass.png");

	if (!LoadMapFromCSV(filePath))
	{
		MessageBox(nullptr, "マップファイルの読み込みに失敗しました", "Error", MB_OK);
	}
}

void Map::Finalize()
{
}

bool Map::LoadMapFromCSV(const std::string& filePath)
{
	std::ifstream map_file(filePath);
	if (!map_file)
	{
		return false;
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

	return true;
}

void Map::Update(float elapsed_time)
{
}

void Map::Draw(const ViewRect& viewRect)
{
	// Starting map chip index
	float offsetX = viewRect.rectPosition.x / MAPCHIP_WIDTH;
	float offsetY = viewRect.rectPosition.y / MAPCHIP_HEIGHT;

	int tileOffsetX = static_cast<int>(offsetX);
	int tileOffsetY = static_cast<int>(offsetY);

	// Local offset from starting map chip
	float localOffsetX = -(viewRect.rectPosition.x - tileOffsetX * MAPCHIP_WIDTH);
	float localOffsetY = -(viewRect.rectPosition.y - tileOffsetY * MAPCHIP_HEIGHT);

	int horizontalCount = static_cast<int>(viewRect.rectWidth / MAPCHIP_WIDTH) + 2;
	int verticalCount = static_cast<int>(viewRect.rectHeight / MAPCHIP_HEIGHT) + 2;

	// Map chip drawing
	for (int y = 0; y < verticalCount; y++)
	{
		for (int x = 0; x < horizontalCount; x++)
		{
			int mapX = offsetX + x;
			int mapY = offsetY + y;

			if (mapX < 0 || mapX >= MAP_H_COUNT || mapY < 0 || mapY >= MAP_V_COUNT) continue;

			int chipId = mapArray[mapY * MAP_H_COUNT + mapX];

			if (chipId == -1) continue;

			// Map chip position in screen space
			float chipPosX = (float)(x * MAPCHIP_WIDTH) + localOffsetX;
			float chipPosY = (float)(y * MAPCHIP_HEIGHT) + localOffsetY;

			int chipIndexX = chipId % 8;
			int chipIndexY = chipId / 8;

			Sprite_Draw(mapTexId, chipPosX, chipPosY, MAPCHIP_WIDTH, MAPCHIP_HEIGHT, 64 * chipIndexX, 64 * chipIndexY, 64.0f, 64.0f);
		}
	}
}

