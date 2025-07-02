
#pragma once

#include "Vector2.h"

#include <SFML/Graphics.hpp>

#include <iostream>

// ----------------
// 
// The base class for all game-states in the game
// 
// All of these gamestates will be stored and accessed inside of Game.cpp
// 
// ----------------

class GameState
{
	public:
		GameState();

		// Main overidden functions
		virtual void beginState();
		void setWindow(sf::RenderWindow& window);

		virtual void keyInput(sf::Event event);
		virtual void mouseInput(sf::Event event);

		virtual int update(float dt);
		virtual void render();

		virtual void endState();

		// Rendering and extra visuals
		sf::RenderWindow* window = nullptr;

		sf::Font main_font;
		void initTextObj(sf::Text* text, sf::Font* font,
			const Vector2& position, int scale, sf::Color color);
};
