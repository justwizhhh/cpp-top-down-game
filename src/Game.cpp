
#include "Game.h"

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
	srand(time(NULL));
}

Game::~Game()
{

}

bool Game::init()
{
	initStates();

	return true;
}

/*
	Create all of the game's possible states
*/
bool Game::initStates()
{
	start_state.setWindow(window);
	instructions_state.setWindow(window);
	gameplay_state.setWindow(window);
	win_state.setWindow(window);
	lose_state.setWindow(window);
	
	game_states.push_back(std::unique_ptr<GameState>(&start_state));
	game_states.push_back(std::unique_ptr<GameState>(&instructions_state));
	game_states.push_back(std::unique_ptr<GameState>(&gameplay_state));
	game_states.push_back(std::unique_ptr<GameState>(&win_state));
	game_states.push_back(std::unique_ptr<GameState>(&lose_state));

	game_states.at(current_state)->beginState();

	return true;
}

/*
	Passes keyboard-based inputs to the currently-loaded game state
*/
void Game::keyInput(sf::Event event)
{
	game_states.at(current_state)->keyInput(event);
}

/*
	Passes mouse-based inputs to the currently-loaded game state
*/
void Game::mouseInput(sf::Event event)
{
	game_states.at(current_state)->mouseInput(event);
}

void Game::update(float dt)
{
	// Update the current state
	enum States state_buffer = 
		States(game_states.at(current_state)->update(dt));

	// Change the current state if a transition has been triggered
	if (current_state != state_buffer)
	{
		game_states.at(prev_state)->endState();
		current_state = state_buffer;
		game_states.at(current_state)->beginState();
	}
}

void Game::render()
{
	game_states.at(current_state)->render();
}


