
#include "GameState.h"

GameState::GameState()
{
    if (!main_font.loadFromFile("Data/Fonts/Prince.ttf"))
    {
        std::cout << "GameState text font cannot not found!" << std::endl;
    }
}

/*
    This takes the current text object, and give it a set size (scale),
    position, and optionally a new colour as well
*/
void GameState::initTextObj(sf::Text* text, sf::Font* font, 
    const Vector2& position, int scale, sf::Color color)
{
    text->setFont(*font);
    text->setCharacterSize(scale);
    text->setPosition(position.x, position.y);
    text->setOrigin(
        text->getGlobalBounds().width / 2, 
        text->getGlobalBounds().height / 2);
    text->setFillColor(color);
    font->setSmooth(false);
}

void GameState::beginState()
{

}

void GameState::setWindow(sf::RenderWindow& window)
{
    this->window = &window;
}

void GameState::keyInput(sf::Event event)
{

}

void GameState::mouseInput(sf::Event event)
{

}

int GameState::update(float dt)
{
	return 0;
}

void GameState::render()
{

}

void GameState::endState()
{

}

