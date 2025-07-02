
#pragma once

#include "PathfindNode.h"

#include "Base/Vector2.h"
#include "TilemapData.h"

#include <iostream>
#include <cmath>

// ----------------
// 
// This class processes all of the raw pathfinding logic for an object, made 
// separate from the main object itself for more organised code
// 
// ----------------

class Pathfinder
{
	public:
		// Setup
		void loadMap(
			std::vector<std::vector<int>> new_map, 
			Vector2 new_bounds);
		
		// Functions for actual pathfinding logic
		void findPath(
			Vector2 start_pos, 
			Vector2 end_pos,
			Vector2 offset);
		bool checkList(
			std::vector<std::shared_ptr<PathfindNode>>& list, 
			Vector2 target_coord);
		std::vector<std::shared_ptr<PathfindNode>> getNeighbours(
			std::shared_ptr<PathfindNode>, 
			Vector2 end_pos);
		void retracePath(
			std::vector<std::shared_ptr<PathfindNode>> closed_list, 
			std::shared_ptr<PathfindNode> end_node);

		// Data functions
		float heuristic(Vector2 position_a, Vector2 position_b);
		Vector2 getTilePosition(
			Vector2 position,
			int tile_size,
			int tile_scale);
		bool checkForSolidTile(
			std::vector<std::vector<int>> map,
			Vector2 tile_coord, int tile_size, int tile_scale);

		// The main path an object will traverse through
		std::vector<Vector2> current_path;

	private:
		std::vector<std::vector<int>> current_map;
		Vector2 current_bounds = Vector2(0, 0);
		Vector2 current_offset = Vector2(0, 0);
		std::shared_ptr<PathfindNode> current_node = nullptr;
};