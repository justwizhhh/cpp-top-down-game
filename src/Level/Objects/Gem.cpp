
#include "Gem.h"

Gem::Gem()
{
	initSprite("Data/Graphics/Objects/gem.png");
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}