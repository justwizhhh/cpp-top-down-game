
#include "PlayerCue.h"

PlayerCue::PlayerCue()
{
	initSprite("Data/Graphics/Player/playerCue.png");
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}