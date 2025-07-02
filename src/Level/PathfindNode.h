
#pragma once

#include "Base/Vector2.h"

#include <cmath>

// ----------------
// 
// This represents a single trajectory point that an object will try to 
// pathfind towards in its path
// 
// ----------------

class PathfindNode
{
	public:
		PathfindNode(
			Vector2 current, 
			Vector2 parent, 
			Vector2 goal, 
			int cost);

		Vector2 current_coord = Vector2(0, 0);
		Vector2 parent_coord = Vector2(0, 0);
		Vector2 goal_coord = Vector2(0, 0);
		int move_cost = 0;

		float distance = 0.0f;
};