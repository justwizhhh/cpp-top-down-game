
#pragma once

#include "Base/Vector2.h"
#include "Base/Enemy.h"

#include "Level/Objects/Player.h"
#include "Level/Pathfinder.h"

#include <SFML/Graphics.hpp>
#include <thread>

// ----------------
// 
// A slime enemy with a heightened desire to kill
// 
// Will try to use A* pathfinding to move directly towards the player.
// Since it's made of metal, the player cannot bounce it around. Rather, the
// enemy will simply be stunned for a few seconds when hit.
// 
// ----------------

class EnemyMetal : public Enemy
{
	public:
		// Main functions
		EnemyMetal(std::vector<std::vector<int>> map, Vector2 bounds);

		void getPlayer(std::shared_ptr<GameObject> player);
		void setPath();
		void initialiseNewPath();
		void stun();

		void update(float dt) override;

		// Main toggles
		int max_health = 3;
		float move_speed = 4;
		float move_radius = 350;
		float stun_time = 1.8f;

		float hurt_opacity = 127;

		// Pathfinding data
		std::shared_ptr<GameObject> player_tracker = nullptr;
		std::vector<Vector2> move_path;

		// Misc. variables
		Vector2 room_offset = Vector2(0, 0);
		bool is_stunned = false;

	private:
		std::vector<std::vector<int>> current_map;
		Vector2 current_room_bounds;
		Vector2 current_offset;
		
		Pathfinder pathfinder;
		std::thread pathfinding_thread;
		bool is_thread_active = false;

		Vector2 current_pos;
		Vector2 next_pos;
		float step_progress = 0.0f;

		sf::Clock stun_timer;

		sf::Texture chase_texture;
};