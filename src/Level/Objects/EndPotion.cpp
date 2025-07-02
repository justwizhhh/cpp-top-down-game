
#include "EndPotion.h"

EndPotion::EndPotion()
{
	initSprite("Data/Graphics/Objects/transformPotion.png");
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}
