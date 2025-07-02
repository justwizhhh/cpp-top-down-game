
#include "Heart.h"

Heart::Heart()
{
	initSprite("Data/Graphics/Objects/heart.png");
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}