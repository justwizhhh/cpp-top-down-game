
#include "Game.h"

#include "StartState.h"

void StartState::beginState()
{
	window->setView(window->getDefaultView());
	
	// Load in all graphical assets
	if (!background_texture.loadFromFile(
		"Data/Graphics/UI/titlescreen_bg.png"))
	{
		std::cout << "Titlescreen background texture not found!" << std::endl;
	}
	else
	{
		background.setTexture(background_texture);
		background.setScale(3, 3);
		background.setPosition(154, 0);
	}

	logo_text.setString("Pocket Adventure");
	initTextObj(
		&logo_text,
		&main_font,
		Vector2(
			window->getView().getSize().x / 2, 
			(window->getView().getSize().y / 2) - 250),
		36,
		sf::Color::Black);

	// Set up all interactive buttons
	start_button.position = 
		Vector2(
			window->getView().getSize().x / 2, 
			window->getView().getSize().y / 2);
	start_button.updatePos();
	start_button.setup("Data/Graphics/UI/ui_startButton.png");
	
	instructions_button.position =
		Vector2(
			window->getView().getSize().x / 2,
			window->getView().getSize().y / 2 + 80);
	instructions_button.updatePos();
	instructions_button.setup("Data/Graphics/UI/ui_instructionsButton.png");
	
	quit_button.position = 
		Vector2(
			window->getView().getSize().x / 2, 
			(window->getView().getSize().y / 2) + 160);
	quit_button.updatePos();
	quit_button.setup("Data/Graphics/UI/ui_quitButton.png");

	is_starting_game = false;
	is_instructions = false;
	is_quitting_game = false;
}

void StartState::keyInput(sf::Event event)
{
	
}

void StartState::mouseInput(sf::Event event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (start_button.is_hovered)
			{
				is_starting_game = true;
			}

			if (instructions_button.is_hovered)
			{
				is_instructions = true;
			}

			if (quit_button.is_hovered)
			{
				is_quitting_game = true;
			}
		}
	}
}

int StartState::update(float dt)
{
	// Retreiving mouse info
	sf::Vector2i mouse_pixels = static_cast<sf::Vector2i>(
		window->mapPixelToCoords(
		sf::Mouse::getPosition(*window), window->getView()));

	mouse_pos = Vector2(mouse_pixels.x, mouse_pixels.y);
	
	// Menu button interactions
	start_button.updateTexture(mouse_pos);
	instructions_button.updateTexture(mouse_pos);
	quit_button.updateTexture(mouse_pos);

	if (is_starting_game)
	{
		return Game::States::GAMEPLAY;
	}
	if (is_instructions)
	{
		return Game::States::INSTRUCTIONS;
	}
	if (is_quitting_game)
	{
		exit(0);
	}
	return Game::States::START;
}

void StartState::render()
{
	window->draw(background);

	window->draw(logo_text);

	window->draw(start_button.sprite);
	window->draw(start_button.mask);

	window->draw(instructions_button.sprite);
	window->draw(instructions_button.mask);

	window->draw(quit_button.sprite);
	window->draw(quit_button.mask);
}

void StartState::endState()
{
	is_starting_game = false;
	is_instructions = false;
	is_quitting_game = false;
}