
#pragma once

#include "Base/GameObject.h"

#include <SFML/Graphics.hpp>

#include <iostream>

// ----------------
// 
// Template class for an interactive menu UI button
// 
// ----------------

class UIButton : public GameObject
{
	public:
		bool is_hovered;
		sf::Texture mask_texture;
		sf::Sprite mask;
		
		void setup(std::string texture_path);
		void updateTexture(Vector2 mouse_pos);
};