
#pragma once

#include "Base/Vector2.h"
#include "Base/GameObject.h"

#include "Level/Objects/PlayerCue.h"

#include <SFML/Graphics.hpp>
#include <iostream>

// ----------------
// 
// The base class for game's player object
// 
// Handles all physics and movement capabilities in conjunction with code 
// inside of 'GameplayState'
// 
// ----------------

class Player : public GameObject
{
	public:
		Player();

		// Public toggles
		int max_health = 3;

		float move_speed = 175;

		float ball_hit_range = 185;
		float ball_speed_scale = 6;
		float ball_decel_scale = 0.975f;

		float ball_stop_speed = 0.5f;
		float ball_attack_speed = 300;
		float ball_knockback_speed = 150;
		float invincibility_time = 2;
		float invincibility_opacity = 125;

		// Main functions
		void changeHealth(int value);
		void changeScore(int value);

		bool getCue(PlayerCue* cue_obj);

		void setMoveTarget(Vector2 new_target);
		void offsetMoveTarget(Vector2 offset);
		void cancelMove();

		void setToBall(bool ball_state);
		void hitBall(Vector2 hit_pos);
		void knockBackBall(Vector2 hit_pos);
		bool bounceWithBall(sf::FloatRect hit_obj);

		bool tileCollision(sf::FloatRect tile_bounds);

		void update(float dt) override;

		// Misc. variables
		int health = 0;
		int score = 0;
		int mana = 0;

		bool is_ball = false;
		bool is_attacking = false;
		bool is_invincibile = false;
		bool is_dead = false;

		std::shared_ptr<PlayerCue> cue = nullptr;
		bool is_cue_nearby = false;

		sf::Texture attack_texture;
		sf::Sprite attack_sprite;

	private:
		float pi = 3.141592f;

		void moveToTarget(float dt);

		bool is_moving = false;
		Vector2 move_target = Vector2(0, 0);

		sf::FloatRect tile_buffer;

		sf::Clock invincibility;

		// Animation data
		sf::Texture human_texture;
		sf::Texture ball_texture;
};