
#include "Game.h"

#include "WinState.h"

void WinState::beginState()
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

	main_text.setString(
		"You win! Thank you for playing!");
	initTextObj(
		&main_text,
		&main_font,
		Vector2(
			window->getView().getSize().x / 2, 
			(window->getView().getSize().y / 2) - 180),
		32,
		sf::Color::White
	);
	sub_text1.setString(
		"You've finally found the cure to undo your ball-shaped curse!");
	initTextObj(
		&sub_text1,
		&main_font,
		Vector2(
			window->getView().getSize().x / 2, 
			(window->getView().getSize().y / 2) - 120),
		14,
		sf::Color::White
	);
	sub_text2.setString(
		"... although maybe this newfound power isn't so bad after all...");
	initTextObj(
		&sub_text2,
		&main_font,
		Vector2(
			window->getView().getSize().x / 2, 
			(window->getView().getSize().y / 2) - 85),
		14,
		sf::Color::White
	);

	// Set up all interactive buttons
	back_button.position = Vector2(
		window->getView().getSize().x / 2, 
		window->getView().getSize().y / 2 + 100);
	back_button.updatePos();
	back_button.setup("Data/Graphics/UI/ui_backToMenuButton.png");
	quit_button.position = Vector2(
		window->getView().getSize().x / 2, 
		window->getView().getSize().y / 2 + 180);
	quit_button.updatePos();
	quit_button.setup("Data/Graphics/UI/ui_quitButton.png");

	is_back_to_title = false;
}

void WinState::keyInput(sf::Event event)
{

}

void WinState::mouseInput(sf::Event event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (back_button.is_hovered)
			{
				is_back_to_title = true;
			}
			else if (quit_button.is_hovered)
			{
				exit(0);
			}
		}
	}
}

int WinState::update(float dt)
{
	// Retreiving mouse info
	sf::Vector2i mouse_pixels = static_cast<sf::Vector2i>(
		window->mapPixelToCoords(
			sf::Mouse::getPosition(*window), window->getView()));

	mouse_pos = Vector2(mouse_pixels.x, mouse_pixels.y);

	// Menu button interactions
	back_button.updateTexture(mouse_pos);
	quit_button.updateTexture(mouse_pos);

	if (is_back_to_title)
	{
		return Game::States::START;
	}
	return Game::States::WIN;
}

void WinState::render()
{
	//window->draw(background);

	window->draw(main_text);
	window->draw(sub_text1);
	window->draw(sub_text2);

	window->draw(back_button.sprite);
	window->draw(back_button.mask);
	window->draw(quit_button.sprite);
	window->draw(quit_button.mask);
}

void WinState::endState()
{
	is_back_to_title = false;
}