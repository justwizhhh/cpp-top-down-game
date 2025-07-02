
#include "Pathfinder.h"

/*
	Loads in the relevant level map info for the object to pathfind through
*/
void Pathfinder::loadMap(
	std::vector<std::vector<int>> new_map, 
	Vector2 new_bounds)
{
	current_map = new_map;
	current_bounds = new_bounds;
}

/*
	This uses A* pathfinding to find the quickest path towards 'end_pos',
	considering values such as the distance and 'cost' of moving between 
	multiple tiles
*/
void Pathfinder::findPath(
	Vector2 start_pos, 
	Vector2 end_pos,
	Vector2 offset)
{
	current_path.clear();

	current_offset = offset;
	
	// Get the starting/ending locations relative to the tile grid
	Vector2 start_tile_pos = 
		getTilePosition(start_pos - current_offset, 
			TilemapData::tile_size, TilemapData::tilemap_scale);
	Vector2 end_tile_pos = 
		getTilePosition(end_pos - current_offset, 
			TilemapData::tile_size, TilemapData::tilemap_scale);
	
	// This is the expanding area that gets scanned for valid tiles
	std::vector<std::shared_ptr<PathfindNode>> open;
	open.push_back(
		std::make_shared<PathfindNode>(
			start_tile_pos, 
			start_tile_pos, 
			end_tile_pos, 
			1));

	// This is the area that we've already explored, and can go backwards from
	std::vector<std::shared_ptr<PathfindNode>> closed;

	bool end_found = false;

	while (!open.empty())
	{
		current_node = open.front();
		closed.push_back(current_node);
		open.erase(open.begin());

		// Stop searching if end location has been found, and create the full 
		// path from all of our explored tiles
		if (current_node->current_coord == end_tile_pos)
		{
			end_found = true;
			retracePath(closed, current_node);
			break;
		}

		// One-by-one, check all neighbouring tiles on the current
		auto neighbours = 
			getNeighbours(current_node, end_tile_pos);
		for (const auto neighbour : neighbours)
		{
			// Get current tile info if we haven't explored this tile yet
			if (!checkList(closed, neighbour->current_coord))
			{
				int current_cost = current_node->move_cost + 1;
				float current_distance = heuristic(
					current_node->current_coord, end_tile_pos);

				// Expand the scannable area towards an area of lowest cost, 
				// while getting physically closer to the end position
				if (!checkList(open, neighbour->current_coord)
					|| current_cost < neighbour->move_cost
					|| current_distance < neighbour->distance)
				{
					neighbour->move_cost = current_cost;
					neighbour->parent_coord = current_node->current_coord;
					open.push_back(
						std::make_shared<PathfindNode>(
							neighbour->current_coord, 
							current_node->current_coord, 
							end_tile_pos, 
							1));
				}
			}
		}
	}

	if (!end_found)
	{
		std::cout << "Path not found for this object!" << std::endl;
	}
}

/*
	Check if the current position is stored somewhere in this node list 
*/
bool Pathfinder::checkList(
	std::vector<std::shared_ptr<PathfindNode>> &node_list, 
	Vector2 target_coord)
{
	for (const std::shared_ptr<PathfindNode> node : node_list)
	{
		if (node->current_coord == target_coord)
		{
			return true;
		}
	}
	return false;
}

/*
	Take a path node, and find all neighbouring tiles that the object can 
	move into
*/
std::vector<std::shared_ptr<PathfindNode>> Pathfinder::getNeighbours(
	std::shared_ptr<PathfindNode> current_node, 
	Vector2 end_pos)
{
	std::vector<std::shared_ptr<PathfindNode>> neighbours;
	neighbours.push_back(std::make_shared<PathfindNode>(Vector2(
		current_node->current_coord.x + 48,
		current_node->current_coord.y), 
		current_node->current_coord, end_pos, 1));
	neighbours.push_back(std::make_shared<PathfindNode>(Vector2(
		current_node->current_coord.x,
		current_node->current_coord.y + 48), 
		current_node->current_coord, end_pos, 1));
	neighbours.push_back(std::make_shared<PathfindNode>(Vector2(
		current_node->current_coord.x - 48,
		current_node->current_coord.y), 
		current_node->current_coord, end_pos, 1));
	neighbours.push_back(std::make_shared<PathfindNode>(Vector2(
		current_node->current_coord.x,
		current_node->current_coord.y - 48), 
		current_node->current_coord, end_pos, 1));

	std::vector<float> neighbour_distances =
	{
		heuristic(neighbours[0]->current_coord, end_pos),
		heuristic(neighbours[1]->current_coord, end_pos),
		heuristic(neighbours[2]->current_coord, end_pos),
		heuristic(neighbours[3]->current_coord, end_pos)
	};

	for (int i = 0; i < neighbours.size(); i++)
	{
		Vector2& coord = neighbours[i]->current_coord;

		if (coord.x < 0 || coord.y < 0 ||
			coord.x >= current_bounds.x || coord.y >= current_bounds.y ||
			checkForSolidTile(current_map,
				getTilePosition(coord, 
					TilemapData::tile_size, TilemapData::tilemap_scale), 
				TilemapData::tile_size, TilemapData::tilemap_scale))
		{
			neighbours.erase(neighbours.begin() + i);
			if (i != 0)
			{
				i -= 1;
			}
		}
	};

	return neighbours;
}

/*
	Run through the entire explorable area and make a traversable path from it
*/
void Pathfinder::retracePath(
	std::vector<std::shared_ptr<PathfindNode>> closed_list, 
	std::shared_ptr<PathfindNode> end_node)
{
	current_node = end_node;

	while (current_node->current_coord != current_node->parent_coord)
	{
		current_path.push_back(Vector2(
			current_node->current_coord.x + current_offset.x, 
			current_node->current_coord.y + current_offset.y));

		for (const auto& node : closed_list)
		{
			if (node->current_coord == current_node->parent_coord)
			{
				current_node = node;
				break;
			}
		}
	}

	current_path.push_back(Vector2(
		current_node->current_coord.x + current_offset.x,
		current_node->current_coord.y + current_offset.y));
	std::reverse(current_path.begin(), current_path.end());
}

/*
	Give an estimate on the distance two node positions (usually the 
	currently-inspected node, and the final destination node)
*/
float Pathfinder::heuristic(
	Vector2 position_a,
	Vector2 position_b)
{
	return
		std::fabs(position_a.x - position_b.x) +
		std::fabs(position_a.y - position_b.y);
}

/*
	Convert this object position into a tile coordinate position
*/
Vector2 Pathfinder::getTilePosition(
	Vector2 position, 
	int tile_size, 
	int tile_scale)
{
	return Vector2(
		int(position.x / (tile_size * tile_scale)) * (tile_size * tile_scale),
		int(position.y / (tile_size * tile_scale)) * (tile_size * tile_scale)
	);
}

/*
	Check on this coordinate if there is a solid tile or not
*/
bool Pathfinder::checkForSolidTile(
	std::vector<std::vector<int>> map,
	Vector2 tile_coord, 
	int tile_size, 
	int tile_scale)
{
	return map
		[std::clamp((int)tile_coord.y / (tile_size * tile_scale),
			0, (int)map.size() - 1)]
		[std::clamp((int)tile_coord.x / (tile_size * tile_scale),
			0, (int)map[0].size() - 1)] 
			!= 0;
}
