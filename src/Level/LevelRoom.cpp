
#include "LevelRoom.h"

LevelRoom::LevelRoom(
	std::string tile_map_file, 
	std::string objects_map_file, 
	Vector2 room_id)
{
	if (tile_map_file != "")
	{
		initTilemap(tile_map_file);
		if (objects_map_file != "")
		{
			initObjects(objects_map_file);
		}

		setRoomId(room_id);
	}
}

/*
	Generate all of the tiles of the room
*/
void LevelRoom::initTilemap(std::string file)
{
	level_tiles.loadMap(file);
	level_tiles.loadTileTextures("Data/Graphics/Level/levelTiles.png");
	level_tiles.setNonSolidTile(29);
	level_tiles.setNonSolidTile(30);
	level_tiles.setNonSolidTile(72);
	level_tiles.setNonSolidTile(73);
	level_tiles.setNonSolidTile(74);
	level_tiles.setNonSolidTile(84);
	level_tiles.createMap(
		TilemapData::tile_size, 
		TilemapData::tilemap_scale, 
		sf::Color(255, 219, 162));
}

/*
	Spawn in all of the objects inside of the room
*/
void LevelRoom::initObjects(std::string file)
{
	level_tiles.loadObjects(file);
	level_tiles.createObjects(
		TilemapData::tile_size,
		TilemapData::tilemap_scale, 
		level_objects);
}

/*
	Reset all of room's objects to their original states, when the game is 
	reset or the room is reloaded
*/
void LevelRoom::reEnableObjects()
{
	if (!level_objects.empty())
	{
		for (auto& obj : level_objects)
		{
			obj->pointer->is_active = obj->pointer->start_active_state;
			obj->pointer->velocity = Vector2(0, 0);
			obj->pointer->position = obj->pointer->start_pos;

			std::shared_ptr<EnemySlime> slime_enemy =
				std::dynamic_pointer_cast<EnemySlime>(obj->pointer);

			if (slime_enemy != nullptr)
			{
				slime_enemy->health = slime_enemy->max_health;
				slime_enemy->velocity = Vector2(0, 0);
				slime_enemy->is_moving = false;
				slime_enemy->is_knocked_back = false;
				slime_enemy->setMoveBounds(
					level_tiles.sprite.getGlobalBounds());

				sf::Color cache = slime_enemy->sprite.getColor();
				slime_enemy->sprite.setColor(
					sf::Color(cache.r, cache.g, cache.b, 255));
			}

			std::shared_ptr<EnemyMetal> metal_enemy =
				std::dynamic_pointer_cast<EnemyMetal>(obj->pointer);

			if (metal_enemy != nullptr)
			{
				metal_enemy->health = metal_enemy->max_health;
				metal_enemy->velocity = Vector2(0, 0);
				metal_enemy->is_stunned = false;
				metal_enemy->move_path.clear();
				if (!metal_enemy->move_path.empty())
				{
					metal_enemy->initialiseNewPath();
				}

				sf::Color cache = metal_enemy->sprite.getColor();
				metal_enemy->sprite.setColor(
					sf::Color(cache.r, cache.g, cache.b, 255));
			}

			std::shared_ptr<EnemyHeavy> heavy_enemy =
				std::dynamic_pointer_cast<EnemyHeavy>(obj->pointer);

			if (heavy_enemy != nullptr)
			{
				heavy_enemy->health = heavy_enemy->max_health;
				heavy_enemy->velocity = Vector2(0, 0);
			}
		}
	}
}

/*
	Allocate the room a position on the level "grid", 
	and then move it to that new position
*/
void LevelRoom::setRoomId(Vector2 new_id)
{
	room_id = new_id;

	Vector2 level_size(
		level_tiles.texture.getSize().x, 
		level_tiles.texture.getSize().y);
	if (!level_size.checkIfNull())
	{
		room_position = Vector2(
			room_id.x * level_size.x * level_tiles.scale,
			room_id.y * level_size.y * level_tiles.scale);

		level_tiles.sprite.setPosition(
			Vector2::convertToSFFloat(room_position));

		for (auto& col : level_tiles.colliders)
		{
			col.left += room_position.x;
			col.top += room_position.y;
		}

		if (!level_objects.empty())
		{
			for (auto& obj : level_objects)
			{
				obj->pointer->start_pos += room_position;
				obj->pointer->position += room_position;
				obj->pointer->updatePos();

				std::shared_ptr<EnemyMetal> metal_enemy =
					std::dynamic_pointer_cast<EnemyMetal>(obj->pointer);

				if (metal_enemy != nullptr)
				{
					metal_enemy->room_offset = room_position;
				}
			}
		}
	}
	else
	{
		std::cout << "Cannot set room position, no level has been loaded!" 
				  << std::endl;
	}
}
