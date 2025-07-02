
#pragma once

#include "Base/Enemy.h"

#include <SFML/Graphics.hpp>

#include <random>
#include <iostream>

// ----------------
// 
// A common slime enemy
// 
// Will walk around in random directions, and get bounced around by the player
// until they run out of health
// 
// ----------------

class EnemySlime : public Enemy
{
	public:
		EnemySlime();

		// Main functions
		bool bounce(sf::FloatRect hit_obj);
		void knockBack(GameObject* obj);
		bool tileCollision(sf::FloatRect tile_bounds);

		void setMoveBounds(sf::FloatRect bounds);
		void setMoveTarget(sf::FloatRect tile_col = sf::FloatRect());
		void movement(float dt);
		void update(float dt) override;

		// Main toggles
		int max_health = 2;
		float move_speed = 1;
		float stop_speed = 0.5f;

		float max_move_dist = 64.0f;
		float min_move_dist = 16.0f;
		float decel_scale = 0.982f;
		float knockback_damping = 0.7f;

		float hurt_opacity = 127;

		// Movement states
		bool is_moving = false;
		bool is_knocked_back = false;

	private:
		std::mt19937 random_seed;

		float t = 0;

		Vector2 current_move_dir = Vector2(0, 0);
		float current_move_dist = 0;

		sf::FloatRect move_bounds;

		Vector2 prev_target = Vector2(0, 0);
		Vector2 move_target = Vector2(0, 0);
};