// ==========================================================================================
// 
// File Name: render_queue.cpp
// Date: 2025/09/02
// Author: Gu Anyi
// Description: Make render queueing with y sorting
// 
// ==========================================================================================

#include "render_queue.h"

#include <algorithm>

std::vector<RenderItem> RenderQueue::items;

void RenderQueue::Clear()
{
	items.clear();
}

void RenderQueue::Add(float y, std::function<void()> drawFunc)
{
	items.push_back({ y, drawFunc });
}

static bool CompareByY(const RenderItem& a, const RenderItem& b)
{
	return a.y < b.y;
}

void RenderQueue::DrawAll()
{
	// Y sorting
	std::sort(items.begin(), items.end(), CompareByY);
	
	for (auto& item : items)
	{
		item.drawFunc();
	}

	items.clear();
}
