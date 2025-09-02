// ==========================================================================================
// 
// File Name: render_queue.h
// Date: 2025/09/02
// Author: Gu Anyi
// Description: Make render queueing
// 
// ==========================================================================================

#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include <vector>
#include <functional>

struct RenderItem
{
	float y;
	std::function<void()> drawFunc;
};

// render manager
class RenderQueue
{
private:

	static std::vector<RenderItem> items;

public:

	static void Clear();
	static void Add(float y, std::function<void()> drawFunc);
	static void DrawAll();

};

#endif // RENDER_QUEUE_H
