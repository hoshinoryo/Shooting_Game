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
            )) // world coordinate
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
            player.Damage();
            g_Enemies[ei].Destroy(ByPlayerCollision);
        }
    }
}

void CheckCollision_EnemyVSEnemy()
{
    for (int i = 0; i < ENEMIES_MAX; i++)
    {
        if (!g_Enemies[i].GetIsEnable()) continue;

        for (int j = i + 1; j < ENEMIES_MAX; j++)
        {
            if (!g_Enemies[j].GetIsEnable()) continue;

            Circle c1 = g_Enemies[i].GetCircleCollision();
            Circle c2 = g_Enemies[j].GetCircleCollision();

            if (Collision_CheckCircle(c1, c2))
            {
                float dx = c2.center.x - c1.center.x;
                float dy = c2.center.y - c1.center.y;
                float dist = sqrtf(dx * dx + dy * dy);
                if (dist < 0.0001f) dist = 0.0001f;

                float overlap = (c1.radius + c2.radius - dist) * 0.5f;
                float nx = dx / dist;
                float ny = dy / dist;

                g_Enemies[i].Move(-nx * overlap, -ny * overlap);
                g_Enemies[j].Move(nx * overlap, ny * overlap);
            }
        }
    }
}


bool CheckCollision_BoxVSMap(const Box& worldBox, Collision_Map& map)
{
    //Box worldPlayerBox = box;
    //worldPlayerBox.center.x += viewRect.rectPosition.x;
    //worldPlayerBox.center.y += viewRect.rectPosition.y;

    // Map chip around the player
    int leftTile   = map.GetWorldToMapX(worldBox.center.x - worldBox.half_width);
    int rightTile  = map.GetWorldToMapX(worldBox.center.x + worldBox.half_width);
    int topTile    = map.GetWorldToMapY(worldBox.center.y - worldBox.half_height);
    int bottomTile = map.GetWorldToMapY(worldBox.center.y + worldBox.half_height);

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
                if (Collision_CheckBox(worldBox, box))
                {
                    return true;
                }
            }
        }
    }

    return false;
}