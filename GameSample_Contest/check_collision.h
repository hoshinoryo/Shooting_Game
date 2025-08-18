// ==========================================================================================
// 
// File Name: collision_check.h
// Date: 2025/08/15
// Author: Gu Anyi
// Description: Collision check header file
// 
// ==========================================================================================

#ifndef CHECK_COLLISION_H
#define CHECK_COLLISION_H

#include "map.h"
#include "camera.h"

//void CheckCollision_BulletVSEnemy();
//void CheckCollision_PlayerVSEnemy(Player& player);

bool CheckCollision_PlayerBoxVSMap(const Box& playerBox, Collision_Map& map, const ViewRect& viewRect);

#endif // CHECK_COLLISION_H
