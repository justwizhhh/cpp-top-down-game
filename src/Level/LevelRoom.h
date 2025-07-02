
#pragma once

#include "Base/Vector2.h"
#include "Base/GameObject.h"

#include "TilemapData.h"
#include "Tilemap.h"
#include "Level/Objects/Player.h"

#include <SFML/Graphics.hpp>

// ------------
//
// This object represents a room the player can explore in the game
// 
// The room's tilemap, and all of its loaded objects are stored here
//
// ------------

class LevelRoom
{
	public:
		// Main functions
		LevelRoom(
			std::string tile_map_file = "", 
			std::string objects_map_file = "", 
			Vector2 room_id = Vector2(0, 0));
		void reEnableObjects();

		// Main variables
		Tilemap level_tiles;
		std::vector<std::shared_ptr<GameObject>> level_objects;

		Vector2 room_id;
		Vector2 room_position;

	private:
		void initTilemap(std::string file);
		void initObjects(std::string file);

		void setRoomId(Vector2 new_id);

		std::shared_ptr<Player> player_tracker;
};