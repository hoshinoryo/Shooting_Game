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
#include "direct3d.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


// Map class

Map::Map()
{
	//mapTexId = -1;
	std::fill(std::begin(mapArray), std::end(mapArray), -1); // default are all -1
	mapWidth = MAPCHIP_WIDTH * MAP_H_COUNT;
	mapHeight = MAPCHIP_HEIGHT * MAP_V_COUNT;
}

Map::~Map()
{
}

void Map::Initialize(const std::string& filePath)
{
	mapTex.Initialize(Direct3D_GetDevice(), L"resources/Christmass_Grass.png");

	if (!LoadMapFromCSV(filePath))
	{
		MessageBox(nullptr, "マップファイルの読み込みに失敗しました", "Error", MB_OK);
	}
}

void Map::Finalize()
{
	mapTex.Finalize();
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

			Sprite_Draw(mapTex, chipPosX, chipPosY, MAPCHIP_WIDTH, MAPCHIP_HEIGHT, 64 * chipIndexX, 64 * chipIndexY, 64.0f, 64.0f);
		}
	}
}


// Collision_Map class

Collision_Map::Collision_Map()
{
	//mapTexId = -1;
	std::fill(std::begin(mapArray), std::end(mapArray), -1);
	mapWidth = MAPCHIP_WIDTH * MAP_H_COUNT;
	mapHeight = MAPCHIP_HEIGHT * MAP_V_COUNT;

	chipBoxCollision.resize(MAP_H_COUNT * MAP_V_COUNT);
}


void Collision_Map::Initialize(const std::string& filePath)
{
	mapTex.Initialize(Direct3D_GetDevice(), L"resources/Collision_full.png");

	if (!LoadMapFromCSV(filePath))
	{
		MessageBox(nullptr, "マップファイルの読み込みに失敗しました", "Error", MB_OK);
	}

	for (int y = 0; y < MAP_V_COUNT; y++)
	{
		for (int x = 0; x < MAP_H_COUNT; x++)
		{
			int chipId = mapArray[y * MAP_H_COUNT + x];
			if (chipId == -1) continue;

			float chipPosX = (float)(x * MAPCHIP_WIDTH);
			float chipPosY = (float)(y * MAPCHIP_HEIGHT);

			chipBoxCollision[y * MAP_H_COUNT + x] = GenerateChipBox(chipPosX, chipPosY, chipId);
		}
	}

}

void Collision_Map::Draw(const ViewRect& viewRect)
{
	float offsetX = viewRect.rectPosition.x / MAPCHIP_WIDTH;
	float offsetY = viewRect.rectPosition.y / MAPCHIP_HEIGHT;

	int tileOffsetX = static_cast<int>(offsetX);
	int tileOffsetY = static_cast<int>(offsetY);

	float localOffsetX = -(viewRect.rectPosition.x - tileOffsetX * MAPCHIP_WIDTH);
	float localOffsetY = -(viewRect.rectPosition.y - tileOffsetY * MAPCHIP_HEIGHT);

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

			if (chipId == -1) continue;

			float chipPosX = (float)(x * MAPCHIP_WIDTH) + localOffsetX;
			float chipPosY = (float)(y * MAPCHIP_HEIGHT) + localOffsetY;

			int chipIndexX = chipId % 4;
			int chipIndexY = chipId / 4;

			Sprite_Draw(mapTex, chipPosX, chipPosY, MAPCHIP_WIDTH, MAPCHIP_HEIGHT, 64 * chipIndexX, 64 * chipIndexY, 64.0f, 64.0f);

#if defined(DEBUG) || defined(_DEBUG)
			for (auto& localBox : chipBoxCollision[mapY * MAP_H_COUNT + mapX])
			{
				Box worldBox = {
					{ localBox.center.x + chipPosX, localBox.center.y + chipPosY },
					localBox.half_width, localBox.half_height
				};
				Collision_DebugDraw(worldBox);
			}
#endif
		}
	}
}

int Collision_Map::GetWorldToMapX(float x)
{
	return (int)(x / MAPCHIP_WIDTH) + (x < 0 ? -1 : 0);
}

int Collision_Map::GetWorldToMapY(float y)
{
	return (int)(y / MAPCHIP_HEIGHT) + (y < 0 ? -1 : 0);
}

int Collision_Map::GetMapChip(int map_x, int map_y)
{
	if (map_x < 0) map_x = 0;
	if (map_x >= MAP_H_COUNT) map_x = MAP_H_COUNT - 1;
	if (map_y < 0) map_y = 0;
	if (map_y >= MAP_V_COUNT) map_y = MAP_V_COUNT - 1;

	return mapArray[map_x + map_y * MAP_H_COUNT];
}

std::vector<Box> Collision_Map::GetChipBoxCollision(float chipPosX, float chipPosY, float mapX, float mapY)
{
	std::vector<Box> worldBoxes;
	const std::vector<Box>& localBoxes = chipBoxCollision[mapY * MAP_H_COUNT + mapX];

	for (const auto& box : localBoxes)
	{
		Box worldBox = box;
		worldBox.center.x += chipPosX;
		worldBox.center.y += chipPosY;
		worldBoxes.push_back(worldBox);
	}

	return worldBoxes;
}

std::vector<Box> Collision_Map::GenerateChipBox(float chipPosX, float chipPosY, int chipId)
{
	std::vector<Box> boxes;

	switch (chipId)
	{
	case 0:
		boxes.push_back({ { 16.0f, 32.0f }, 16.0f, 32.0f });
		break;
	case 1:
		boxes.push_back({ { 48.0f, 32.0f }, 16.0f, 32.0f });
		break;
	case 2:
		boxes.push_back({ { 32.0f, 16.0f }, 32.0f, 16.0f });
		break;
	case 3:
		boxes.push_back({ { 32.0f, 48.0f }, 32.0f, 16.0f });
		break;
	case 4:
		boxes.push_back({ { 16.0f, 16.0f }, 16.0f, 16.0f });
		break;
	case 5:
		boxes.push_back({ { 48.0f, 16.0f }, 16.0f, 16.0f });
		break;
	case 6:
		boxes.push_back({ { 16.0f, 48.0f }, 16.0f, 16.0f });
		break;
	case 7:
		boxes.push_back({ { 48.0f, 48.0f }, 16.0f, 16.0f });
		break;
	case 8:
		boxes.push_back({ { 16.0f, 32.0f }, 16.0f, 32.0f });
		boxes.push_back({ { 48.0f, 48.0f }, 16.0f, 16.0f });
		break;
	case 9:
		boxes.push_back({ { 16.0f, 48.0f }, 16.0f, 16.0f });
		boxes.push_back({ { 48.0f, 32.0f }, 16.0f, 32.0f });
		break;
	case 10:
		boxes.push_back({ { 16.0f, 32.0f }, 16.0f, 32.0f });
		boxes.push_back({ { 48.0f, 16.0f }, 16.0f, 16.0f });
		break;
	case 11:
		boxes.push_back({ { 16.0f, 16.0f }, 16.0f, 16.0f });
		boxes.push_back({ { 48.0f, 32.0f }, 16.0f, 32.0f });
		break;
	}
	return boxes;
}