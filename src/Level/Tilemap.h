
#pragma once

#include "Base/Vector2.h"

#include "Objects/Player.h"
#include "Objects/Heart.h"
#include "Objects/Gem.h"
#include "Objects/Bush.h"
#include "Objects/EndPotion.h"
#include "Objects/Enemies/EnemySlime.h"
#include "Objects/Enemies/EnemyMetal.h"
#include "Objects/Enemies/EnemyHeavy.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// ------------
//
// This object stores a list of all currently-loaded tiles and objects in a 
// level room
// 
// Code taken, and modified, from my 'Platformer Game' source code 
// (Year 1 Games in C++ assigment game)
//
// ------------

class Tilemap
{
	public:
		// Main functions
		void loadMap(const std::string& tile_file_path);
		void loadObjects(const std::string& objects_file_path);
		void loadTileTextures(const std::string& file_path);

		void createMap(float tile_size, float map_scale, sf::Color bg_color);
		void setNonSolidTile(int tile_id);

		// Object creation functions
		void createObjects(
			float tile_size, 
			float map_scale, 
			std::vector<std::shared_ptr<GameObject>>& objects_list);

		std::vector<std::vector<int>> cost_map;
		std::vector<sf::FloatRect> colliders;

		// Rendering info
		int width = 0;
		int height = 0;
		Vector2 bounds = Vector2(0, 0);

		int size = 0;
		int scale = 0;

		sf::RenderTexture texture;
		sf::Sprite sprite;

	private:
		std::vector<std::vector<std::string>> map_data;
		std::vector<std::vector<std::string>> objects_data;

		std::vector<int> non_solid_tiles;

		sf::Texture tilemap_texture;
		std::string tilemap_texture_path;

		std::mt19937 random_seed;
};
