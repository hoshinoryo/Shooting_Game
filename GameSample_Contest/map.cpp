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
#include "render_queue.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>

// Debug output
#include "debug_text.h"
#include "debug_ostream.h"


// Map class

Map::Map()
{
	std::fill(std::begin(mapArray), std::end(mapArray), -1); // default are all -1

	chipWidth = 0;
	chipHeight = 0;
	chipsPerRow = 1;
	chipsPerCol = 1;

	mapWidth = chipWidth * MAP_H_COUNT;
	mapHeight = chipHeight * MAP_V_COUNT;
}

Map::~Map()
{
	Finalize();
}

void Map::Initialize(const std::string& filePath, const wchar_t* texturePath, int chipW, int chipH)
{
	mapTex.Initialize(Direct3D_GetDevice(), texturePath);

	if (!LoadMapFromCSV(filePath))
	{
		MessageBox(nullptr, "マップファイルの読み込みに失敗しました", "Error", MB_OK);
	}

	chipWidth = chipW;
	chipHeight = chipH;

	int texWidth = mapTex.GetWidth();
	int texHeight = mapTex.GetHeight();

	chipsPerRow = texWidth  / chipWidth;
	chipsPerCol = texHeight / chipHeight;

	mapWidth = chipWidth * MAP_H_COUNT;
	mapHeight = chipHeight * MAP_V_COUNT;
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
	float offsetX = viewRect.rectPosition.x / chipWidth;
	float offsetY = viewRect.rectPosition.y / chipHeight;

	int tileOffsetX = static_cast<int>(offsetX);
	int tileOffsetY = static_cast<int>(offsetY);

	// Local offset from starting map chip
	float localOffsetX = -(viewRect.rectPosition.x - tileOffsetX * chipWidth);
	float localOffsetY = -(viewRect.rectPosition.y - tileOffsetY * chipHeight);

	int horizontalCount = static_cast<int>(viewRect.rectWidth / chipWidth) + 2;
	int verticalCount = static_cast<int>(viewRect.rectHeight / chipHeight) + 2;

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
			float chipPosX = (float)(x * chipWidth) + localOffsetX;
			float chipPosY = (float)(y * chipHeight) + localOffsetY;

			int chipIndexX = chipId % chipsPerRow;
			int chipIndexY = chipId / chipsPerRow;

			Sprite_Draw(mapTex, chipPosX, chipPosY, chipWidth, chipHeight,
				chipWidth * chipIndexX, chipHeight * chipIndexY, 64.0f, 64.0f);
		}
	}
}

void Map::QueueDraw(const ViewRect& viewRect)
{
	//const int layerHeight = 32;

	int startX = static_cast<int>(viewRect.rectPosition.x / chipWidth);
	int startY = static_cast<int>(viewRect.rectPosition.y / chipHeight);

	int horizontalCount = static_cast<int>(viewRect.rectWidth / chipWidth) + 2;
	int verticalCount = static_cast<int>(viewRect.rectHeight / chipHeight) + 2;

	// clamp
	if (startX < 0) startX = 0;
	if (startY < 0) startY = 0;
	if (startX + horizontalCount > MAP_H_COUNT) horizontalCount = MAP_H_COUNT - startX;
	if (startY + verticalCount > MAP_V_COUNT) verticalCount = MAP_V_COUNT - startY;

	std::vector<std::vector<bool>> visited(MAP_V_COUNT, std::vector<bool>(MAP_H_COUNT, false)); // visited mapArray

	std::queue<std::pair<int, int>> q;
	std::vector<std::pair<int, int>> blockGroup;

	const int dx[4] = { 1, -1, 0, 0 };
	const int dy[4] = { 0, 0, 1, -1 };

	auto isValid = [&](int y, int x)
		{
			return (x >= 0 && x < MAP_H_COUNT &&
				y >= 0 && y < MAP_V_COUNT &&
				mapArray[y * MAP_H_COUNT + x] != -1 && !visited[y][x]);
		};

	for (int y = startY; y < startY + verticalCount; y++)
	{
		for (int x = startX; x < startX + horizontalCount; x++)
		{
			if (!isValid(y, x)) continue;

			while (!q.empty()) q.pop();
			blockGroup.clear();

			q.push(std::make_pair(y, x));
			visited[y][x] = true;

			float groupMaxY = y * chipHeight;

			while (!q.empty())
			{
				std::pair<int, int> cur = q.front(); q.pop();
				int cy = cur.first;
				int cx = cur.second;

				blockGroup.push_back(cur);

				float chipY = static_cast<float>(cy) * chipHeight;
				if (chipY > groupMaxY) groupMaxY = chipY;

				for (int dir = 0; dir < 4; dir++)
				{
					int ny = cy + dy[dir];
					int nx = cx + dx[dir];
					if (isValid(ny, nx))
					{
						visited[ny][nx] = true;
						q.push(std::make_pair(ny, nx));
					}
				}
			}

			RenderQueue::Add(groupMaxY, [this, blockGroup, viewRect]()
				{
					int tileOffsetX = static_cast<int>(viewRect.rectPosition.x / chipWidth);
					int tileOffsetY = static_cast<int>(viewRect.rectPosition.y / chipHeight);

					float localOffsetX = -(viewRect.rectPosition.x - tileOffsetX * chipWidth);
					float localOffsetY = -(viewRect.rectPosition.y - tileOffsetY * chipHeight);

					for (size_t i = 0; i < blockGroup.size(); ++i)
					{
						int by = blockGroup[i].first;
						int bx = blockGroup[i].second;

						int chipId = mapArray[by * MAP_H_COUNT + bx];

						int chipIndexX = chipId % chipsPerRow;
						int chipIndexY = chipId / chipsPerRow;
						//float chipPosX = (float)(bx * chipWidth) + localOffsetX;
						//float chipPosY = (float)(by * chipHeight) + localOffsetY;
						float chipPosX = (float)(bx * chipWidth - viewRect.rectPosition.x);
						float chipPosY = (float)(by * chipHeight - viewRect.rectPosition.y);

						Sprite_Draw(mapTex, chipPosX, chipPosY, chipWidth, chipHeight,
							chipWidth * chipIndexX, chipHeight * chipIndexY, 64.0f, 64.0f);
					}
				});
		}
	}
}


// Collision_Map class

Collision_Map::Collision_Map()
{
	std::fill(std::begin(mapArray), std::end(mapArray), -1);

	chipWidth = 0;
	chipHeight = 0;
	chipsPerRow = 1;
	chipsPerCol = 1;

	chipBoxCollision.resize(MAP_H_COUNT * MAP_V_COUNT);
}


void Collision_Map::Initialize(const std::string& filePath, int chipW, int chipH)
{
	mapTex.Initialize(Direct3D_GetDevice(), L"resources/Collision_full.png");

	if (!LoadMapFromCSV(filePath))
	{
		MessageBox(nullptr, "マップファイルの読み込みに失敗しました", "Error", MB_OK);
	}

	chipWidth = chipW;
	chipHeight = chipH;

	int texWidth = mapTex.GetWidth();
	int texHeight = mapTex.GetHeight();

	chipsPerRow = texWidth / chipWidth;
	chipsPerCol = texHeight / chipHeight;

	mapWidth = chipWidth * MAP_H_COUNT;
	mapHeight = chipHeight * MAP_V_COUNT;

	for (int y = 0; y < MAP_V_COUNT; y++)
	{
		for (int x = 0; x < MAP_H_COUNT; x++)
		{
			int chipId = mapArray[y * MAP_H_COUNT + x];
			if (chipId == -1) continue;

			float chipPosX = (float)(x * chipWidth);
			float chipPosY = (float)(y * chipHeight);

			chipBoxCollision[y * MAP_H_COUNT + x] = GenerateChipBox(chipPosX, chipPosY, chipId);
		}
	}

}

void Collision_Map::Draw(const ViewRect& viewRect)
{
	float offsetX = viewRect.rectPosition.x / chipWidth;
	float offsetY = viewRect.rectPosition.y / chipHeight;

	int tileOffsetX = static_cast<int>(offsetX);
	int tileOffsetY = static_cast<int>(offsetY);

	float localOffsetX = -(viewRect.rectPosition.x - tileOffsetX * chipWidth);
	float localOffsetY = -(viewRect.rectPosition.y - tileOffsetY * chipHeight);

	int horizontalCount = static_cast<int>(viewRect.rectWidth / chipWidth) + 2;
	int verticalCount = static_cast<int>(viewRect.rectHeight / chipHeight) + 2;

	for (int y = 0; y < verticalCount; y++)
	{
		for (int x = 0; x < horizontalCount; x++)
		{
			int mapX = offsetX + x;
			int mapY = offsetY + y;

			if (mapX < 0 || mapX >= MAP_H_COUNT || mapY < 0 || mapY >= MAP_V_COUNT) continue;

			int chipId = mapArray[mapY * MAP_H_COUNT + mapX];

			if (chipId == -1) continue;

			float chipPosX = (float)(x * chipWidth) + localOffsetX;
			float chipPosY = (float)(y * chipHeight) + localOffsetY;

			int chipIndexX = chipId % 4;
			int chipIndexY = chipId / 4;

			Sprite_Draw(mapTex, chipPosX, chipPosY, chipWidth, chipHeight, 64 * chipIndexX, 64 * chipIndexY, 64.0f, 64.0f);

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
	return (int)(x / chipWidth) + (x < 0 ? -1 : 0);
}

int Collision_Map::GetWorldToMapY(float y)
{
	return (int)(y / chipHeight) + (y < 0 ? -1 : 0);
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