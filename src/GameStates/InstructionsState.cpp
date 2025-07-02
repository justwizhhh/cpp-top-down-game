
#include "Game.h"

#include "InstructionsState.h"

void InstructionsState::beginState()
{
	window->setView(window->getDefaultView());

	// Load in all graphical assets
	for (int i = 0; i < 4; ++i)
	{
		auto new_texture = std::make_shared<sf::Texture>();
		panel_textures.push_back(new_texture);
	}

	if (!panel_textures[0]->loadFromFile(
			"Data/Graphics/UI/Instructions/instructionPanel1.png") ||
		!panel_textures[1]->loadFromFile(
			"Data/Graphics/UI/Instructions/instructionPanel2.png") ||
		!panel_textures[2]->loadFromFile(
			"Data/Graphics/UI/Instructions/instructionPanel3.png") ||
		!panel_textures[3]->loadFromFile(
			"Data/Graphics/UI/Instructions/instructionPanel4.png"))
	{
		std::cout << "Instructions panel textures not found!" << std::endl;
	}

	for (int i = 0; i < 4; ++i) 
	{
		auto new_sprite = std::make_shared<sf::Sprite>(*panel_textures[i]);
		new_sprite->setScale(3, 3);
		new_sprite->setPosition(154, 0);
		panel_sprites.push_back(new_sprite);
	}

	panel_progress = 0;
	is_back_to_title = false;
}

void InstructionsState::keyInput(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Enter)
		{
			panel_progress++;
		}
	}
}

void InstructionsState::mouseInput(sf::Event event)
{
	
}

int InstructionsState::update(float dt)
{
	// Retreiving mouse info
	sf::Vector2i mouse_pixels = static_cast<sf::Vector2i>(
		window->mapPixelToCoords(
			sf::Mouse::getPosition(*window), window->getView()));

	mouse_pos = Vector2(mouse_pixels.x, mouse_pixels.y);

	if (panel_progress >= 4)
	{
		is_back_to_title = true;
	}

	if (is_back_to_title)
	{
		return Game::States::START;
	}
	return Game::States::INSTRUCTIONS;
}

void InstructionsState::render()
{
	window->draw(*panel_sprites[panel_progress]);
}

void InstructionsState::endState()
{
	is_back_to_title = false;
}