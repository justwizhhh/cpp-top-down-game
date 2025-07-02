
#pragma once

#include "Base/GameObject.h"

#include <SFML/Graphics.hpp>

#include <iostream>

// ----------------
// 
// Class for keeping track of, and updating, all user interface (HUD) info 
// during the gameplay state
// 
// ----------------

class GameplayUI
{
	public:
		// Main functions
		GameplayUI();

		void initValues(int* health, int* score);
		void updateHealth();
		void updateScore();
		void reset();

		// Toggles
		int max_health_icons = 4;
		size_t score_zfill = 6;

		// Visible UI objects
		sf::Sprite gem_icon;
		sf::Text score_display;
		std::vector<std::shared_ptr<sf::Sprite>> health_display;

	private:
		std::shared_ptr<int> health_ptr = nullptr;
		std::shared_ptr<int> score_ptr = nullptr;

		sf::Font font;

		sf::Texture gem_icon_texture;
		sf::Texture health_icon_texture;

		sf::Color full_colour = sf::Color(255, 255, 255, 255);
		sf::Color empty_colour = sf::Color(0, 0, 0, 255);
};