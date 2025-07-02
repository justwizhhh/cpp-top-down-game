
#include "UIButton.h"

/*
	Loads in a main texture for the button, as well as a mask rendered above it
	for selecting the button
*/
void UIButton::setup(std::string texture_path)
{
	GameObject::initSprite(texture_path);
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

	mask_texture.loadFromImage(sprite.getTexture()->copyToImage());
	mask.setTexture(mask_texture);
	mask.setPosition(
		sprite.getGlobalBounds().left, 
		sprite.getGlobalBounds().top);
	mask.setScale(sprite.getScale());
	mask.setColor(sf::Color(0, 0, 0, 0));
}

void UIButton::updateTexture(Vector2 mouse_pos)
{
	if (contains(mouse_pos))
	{
		is_hovered = true;
		mask.setColor(sf::Color(0, 0, 0, 128));
	}
	else
	{
		is_hovered = false;
		mask.setColor(sf::Color(0, 0, 0, 0));
	}
}
