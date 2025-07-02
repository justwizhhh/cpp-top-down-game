
#pragma once

#include "Base/Enemy.h"

#include <SFML/Graphics.hpp>

#include <random>
#include <iostream>

// ----------------
// 
// A large, stationary enemy that gets knocked around the player and 
// not much else
// 
// ----------------

class EnemyHeavy : public Enemy
{
	public:
		EnemyHeavy();

		// Main functions
		bool bounce(sf::FloatRect hit_obj);
		void knockBack(GameObject* obj);
		bool tileCollision(sf::FloatRect tile_bounds);

		void update(float dt) override;

		// Main toggles
		int max_health = 3;
		float decel_scale = 0.98f;
		float knockback_damping = 0.075f;
};