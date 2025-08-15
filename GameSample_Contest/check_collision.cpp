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

#include "debug_text.h"
#include "debug_ostream.h"

#include <iostream>
#include <string>

/*
void CheckCollision_BulletVSEnemy()
{
    for (int bi = 0; bi < BULLETS_MAX; bi++)
    {
        if (!Bullet_IsEnable(bi)) continue;

        for (int ei = 0; ei < ENEMIES_MAX; ei++)
        {
            if (!Enemy_IsEnable(ei)) continue;
            if (Collision_CheckCircle(
                Bullet_GetCollision(bi),
                Enemy_GetCollision(ei)
            ))
            {
                // ƒqƒbƒg‚³‚ê‚½‚ç
                Bullet_Destroy(bi);
                Enemy_Damage(ei);
            }
        }
    }
}

void CheckCollision_PlayerVSEnemy(Player& player)
{
    if (!player.IsEnable()) return;

    for (int ei = 0; ei < ENEMIES_MAX; ei++)
    {
        if (!Enemy_IsEnable(ei)) continue;

        if (Collision_CheckCircle(
            player.GetCircleCollision(),
            Enemy_GetCollision(ei)
        )) // ƒqƒbƒg‚³‚ê‚½‚ç
        {
            player.Destroy();
            Enemy_Destroy(ei);
        }
    }
}
*/

bool CheckCollision_PlayerBoxVSMap(Box& playerBox, Collision_Map& map, const ViewRect& viewRect)
{
    float playerBoxWorldX = playerBox.center.x + viewRect.rectPosition.x;
    float playerBoxWorldY = playerBox.center.y + viewRect.rectPosition.y;

    playerBox.center.x = playerBoxWorldX;
    playerBox.center.y = playerBoxWorldY;

    // Map chip around the player
    int leftTile   = map.GetWorldToMapX(playerBoxWorldX - playerBox.half_width);
    int rightTile  = map.GetWorldToMapX(playerBoxWorldX + playerBox.half_width);
    int topTile    = map.GetWorldToMapY(playerBoxWorldY - playerBox.half_height);
    int bottomTile = map.GetWorldToMapY(playerBoxWorldY + playerBox.half_height);

    for (int ty = topTile; ty <= bottomTile; ty++)
    {
        for (int tx = leftTile; tx <= rightTile; tx++)
        {
            int chipId = map.GetMapChip(tx, ty);

            if (chipId == -1) continue;

            float chipPosX = tx * 64.0f;
            float chipPosY = ty * 64.0f;

            Box chipBox = map.GetChipBoxCollision(chipPosX, chipPosY);

            if (Collision_CheckBox(playerBox, chipBox))
            {
                return true;
            }
        }
    }

    return false;
}