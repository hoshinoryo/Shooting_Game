// ==========================================================================================
// 
// File Name: map.h
// Date: 2025/08/09
// Author: Gu Anyi
// Description: Map class and collision subclass header file
// 
// ==========================================================================================

#ifndef MAP_H
#define MAP_H

#include "camera.h"
#include "collision.h"
#include "texture.h"

#include <DirectXMath.h>
#include <string>
#include <vector>

using namespace DirectX;

class Map
{
protected:

	static constexpr unsigned int MAPCHIP_WIDTH = 64;
	static constexpr unsigned int MAPCHIP_HEIGHT = 64;
	static constexpr unsigned int MAP_H_COUNT = 50;
	static constexpr unsigned int MAP_V_COUNT = 30;

	int mapWidth;
	int mapHeight;

	int mapArray[MAP_H_COUNT * MAP_V_COUNT];

	Texture mapTex;

public:

	Map();
	~Map();

	void Initialize(const std::string& filePath);
	void Finalize();

	bool LoadMapFromCSV(const std::string& filePath);
	void Update(float elapsed_time);
	void Draw(const ViewRect& viewRect);

	int GetMapWidth() const { return mapWidth; }
	int GetMapHeight() const { return mapHeight; }
};


class Collision_Map : public Map
{
private:

	std::vector<std::vector<Box>> chipBoxCollision;

	std::vector<Box> GenerateChipBox(float chipPosX, float chipPosY, int chipId);

public:

	Collision_Map();

	void Initialize(const std::string& filePath);
	void Draw(const ViewRect& viewRect);

	int GetWorldToMapX(float x);
	int GetWorldToMapY(float y);
	int GetMapChip(int map_x, int map_y);

	std::vector<Box> GetChipBoxCollision(float chipPosX, float chipPosY, float mapX, float mapY);
};

#endif // MAP_H