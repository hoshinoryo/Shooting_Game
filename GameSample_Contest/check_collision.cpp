// ==========================================================================================
// 
// File Name: collision_check.h
// Date: 2025/08/15
// Author: Gu Anyi
// Description: Check collision between two things
// 
// ==========================================================================================

#include "check_collision.h"
#include "collision.h"
#include "enemy.h"
#include "bullet.h"

#include <iostream>
#include <string>
#include <vector>


void CheckCollision_BulletVSEnemy()
{
    for (int bi = 0; bi < BULLETS_MAX; bi++)
    {
        if (!Bullet_IsEnable(bi)) continue;

        for (int ei = 0; ei < ENEMIES_MAX; ei++)
        {
            if (!g_Enemies[ei].GetIsEnable()) continue;

            if (Collision_CheckCircle(
                Bullet_GetCollision(bi),
                g_Enemies[ei].GetCircleCollision()
            ))
            {
                Bullet_Destroy(bi);
                g_Enemies[ei].Damage();
            }
        }
    }
}

void CheckCollision_PlayerVSEnemy(Player& player)
{
    if (!player.GetIsEnable()) return;

    for (int ei = 0; ei < ENEMIES_MAX; ei++)
    {
        if (!g_Enemies[ei].GetIsEnable()) continue;

        if (Collision_CheckCircle(
            player.GetCircleCollision(),
            g_Enemies[ei].GetCircleCollision()
        ))
        {
            player.Destroy();
            g_Enemies[ei].Destroy();
        }
    }
}


bool CheckCollision_BoxVSMap(const Box& box, Collision_Map& map, const ViewRect& viewRect)
{
    Box worldPlayerBox = box;
    worldPlayerBox.center.x += viewRect.rectPosition.x;
    worldPlayerBox.center.y += viewRect.rectPosition.y;

    // Map chip around the player
    int leftTile   = map.GetWorldToMapX(worldPlayerBox.center.x - worldPlayerBox.half_width);
    int rightTile  = map.GetWorldToMapX(worldPlayerBox.center.x + worldPlayerBox.half_width);
    int topTile    = map.GetWorldToMapY(worldPlayerBox.center.y - worldPlayerBox.half_height);
    int bottomTile = map.GetWorldToMapY(worldPlayerBox.center.y + worldPlayerBox.half_height);

    for (int ty = topTile; ty <= bottomTile; ty++)
    {
        for (int tx = leftTile; tx <= rightTile; tx++)
        {
            int chipId = map.GetMapChip(tx, ty);
            if (chipId == -1) continue;

            float chipPosX = tx * 64.0f;
            float chipPosY = ty * 64.0f;

            const std::vector<Box>& chipBoxes = map.GetChipBoxCollision(chipPosX, chipPosY, tx, ty);

            for (const auto& box : chipBoxes)
            {
                if (Collision_CheckBox(worldPlayerBox, box))
                {
                    return true;
                }
            }
        }
    }

    return false;
}