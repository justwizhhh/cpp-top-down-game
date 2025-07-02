
#pragma once

#include "Base/GameState.h"

#include "GameStates/StartState.h"
#include "GameStates/InstructionsState.h"
#include "GameStates/GameplayState.h"
#include "GameStates/WinState.h"
#include "GameStates/LoseState.h"

#include <SFML/Graphics.hpp>

#include <iostream>

class Game
{
	public:
		// Game states
		enum States
		{
			START,
			INSTRUCTIONS,
			GAMEPLAY,
			WIN,
			GAMEOVER
		};
		enum States current_state = States::START;
		enum States prev_state = States::START;

		// Functions
		Game(sf::RenderWindow& window);
		~Game();

		bool init();
		bool initStates();

		void keyInput(sf::Event event);
		void mouseInput(sf::Event event);

		void update(float dt);
		void render();

	private:
		StartState start_state;
		InstructionsState instructions_state;
		GameplayState gameplay_state;
		WinState win_state;
		LoseState lose_state;
		
		std::vector<std::shared_ptr<GameState>> game_states;

		sf::RenderWindow& window;
};
