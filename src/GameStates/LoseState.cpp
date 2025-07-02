
#include "Game.h"

#include "LoseState.h"

void LoseState::beginState()
{
	window->setView(window->getDefaultView());
	
	// Load in all graphical assets
	if (!background_texture.loadFromFile(
		"Data/Graphics/Test/placeholderTitlescreen.png"))
	{
		std::cout << "Titlescreen background texture not found!" << std::endl;
	}
	else
	{
		background.setTexture(background_texture);
		background.setScale(3, 3);
		background.setPosition(154, 0);
	}

	main_text.setString("Game Over!");
	initTextObj(
		&main_text,
		&main_font,
		Vector2(
			window->getView().getSize().x / 2, 
			(window->getView().getSize().y / 2) - 200),
		40,
		sf::Color::White
	);
	sub_text1.setString("You've lost all your health, and now");
	initTextObj(
		&sub_text1,
		&main_font,
		Vector2(
			window->getView().getSize().x / 2, 
			(window->getView().getSize().y / 2) - 120),
		18,
		sf::Color::White
	);
	sub_text2.setString("you won't be able to regain your human form...");
	initTextObj(
		&sub_text2,
		&main_font,
		Vector2(
			window->getView().getSize().x / 2, 
			(window->getView().getSize().y / 2) - 90),
		18,
		sf::Color::White
	);

	// Set up all interactive buttons
	back_button.position = Vector2(
		window->getView().getSize().x / 2, 
		window->getView().getSize().y / 2 + 150);
	back_button.updatePos();
	back_button.setup("Data/Graphics/UI/ui_backToMenuButton.png");

	is_back_to_title = false;
}

void LoseState::keyInput(sf::Event event)
{

}

void LoseState::mouseInput(sf::Event event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (back_button.is_hovered)
			{
				is_back_to_title = true;
			}
		}
	}
}

int LoseState::update(float dt)
{
	// Retreiving mouse info
	sf::Vector2i mouse_pixels = static_cast<sf::Vector2i>(
		window->mapPixelToCoords(
			sf::Mouse::getPosition(*window), window->getView()));

	mouse_pos = Vector2(mouse_pixels.x, mouse_pixels.y);

	// Menu button interactions
	back_button.updateTexture(mouse_pos);

	if (is_back_to_title)
	{
		return Game::States::START;
	}
	return Game::States::GAMEOVER;
}

void LoseState::render()
{
	window->draw(main_text);
	window->draw(sub_text1);
	window->draw(sub_text2);

	window->draw(back_button.sprite);
	window->draw(back_button.mask);
}

void LoseState::endState()
{
	is_back_to_title = false;
}