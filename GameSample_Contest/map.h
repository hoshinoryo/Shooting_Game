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

#include <DirectXMath.h>
#include <string>

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

	int mapTexId;
	int mapArray[MAP_H_COUNT * MAP_V_COUNT];

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

	Box chipBoxCollision;

public:

	Collision_Map();

	void Initialize(const std::string& filePath);
	void Draw(const ViewRect& viewRect);

	Box GetChipBoxCollision(float chipPosX, float chipPosY);
	bool MapCollision();
};

#endif // MAP_H