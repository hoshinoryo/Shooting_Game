# 2D Action Game (DirectX C++)

## Overview
This is a 2D game project written in **C++** using **DirectX**.  
The system is based on world coordinates, with rendering converted into screen coordinates through a `ViewRect` (camera).  
The player, enemies, bullets, and map all use **world-based collision detection**.

---

## Core Systems

### Player
- Controlled by the user.
- Stores **world position** and calculates **screen position** from the camera.
- Can shoot bullets.
- Has circle and box collision shapes.
- Becomes disabled when colliding with enemies.

### Enemy
- Spawned by the **Enemy Spawner**.
- Stores **world position** and **screen position**.
- Moves toward the player using simple AI.
- Collides with the map (boxes) and with player/bullets.
- Has lifetime control (e.g., invincible for 1 seconds after spawn, disappears after 30 seconds).
- Can be destroyed when HP reaches 0.

### Bullets
- Created at the player’s world position.
- Move in the shooting direction.
- Collide with enemies.
- Disabled upon collision or when leaving the screen.

### Collision
- **World-based collision system**:
  - Circles (for player, enemies, bullets).
  - Boxes (for map tiles, enemy body).
- Functions check circle-vs-circle, box-vs-map, etc.
- Debug mode (`DEBUG` build) shows collision shapes.

### Map
- Tile-based map.
- Each tile can have one or multiple collision boxes.
- Used for preventing player/enemy movement through walls or obstacles.

### Camera / ViewRect
- `ViewRect` represents the visible area of the camera.
- Converts world coordinates into screen coordinates for rendering.
- All `Draw()` functions subtract the camera offset when rendering.

---

## Current Features
- Player movement and shooting.
- Enemies chase the player and collide with walls.
- Bullets destroy enemies upon collision.
- Player destroyed when colliding with enemies.
- Debug drawing for collision shapes.

---

## Planned Improvements
- Better bullet patterns.
- UI elements (HP bar, score).
- Sound effects.
