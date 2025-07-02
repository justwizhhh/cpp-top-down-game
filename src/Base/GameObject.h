
#pragma once

#include <Base/Vector2.h>

#include <SFML/Graphics.hpp>

#include <iostream>

// ----------------
// 
// The base class for all interactive game-objects in the game
// 
// Code taken, and modified, from 'Point and Click Game' source code (More Games in C++ assigment game)
// 
// ----------------

class GameObject
{
	public:
		GameObject();
		~GameObject();

		// Public toggles
		float sprite_scale = 3;
		bool can_be_held = false;

		// General behaviour
		bool start_active_state = false;
		bool is_active = false;
		std::shared_ptr<GameObject> pointer = nullptr;

		sf::Texture texture;
		sf::Sprite sprite;
		bool initSprite(std::string file_path);

		Vector2 start_pos = Vector2(0, 0);
		Vector2 position = Vector2(0, 0);
		Vector2 prev_position = Vector2(0, 0);
		Vector2 velocity = Vector2(0, 0);
		Vector2 getCentrePosition();
		Vector2 getPrevCentrePosition();

		bool intersects(
			sf::FloatRect obj_bounds, 
			sf::FloatRect* overlap = nullptr);
		bool contains(Vector2& pos);
		bool collide(sf::FloatRect obj_bounds, Vector2* col_side = nullptr);

		virtual void update(float dt);
		void updatePos();
};
