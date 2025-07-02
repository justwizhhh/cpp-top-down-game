
#include "Bush.h"

Bush::Bush()
{
	int variant = rand() % 2;
	
	switch (variant)
	{
		default:	
		case 0:
			initSprite("Data/Graphics/Objects/bush1.png");
			break;
		case 1:
			initSprite("Data/Graphics/Objects/bush2.png");
			break;
	}
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

/*
	If the bush stores any object, this function sets it up so that it pops
	out of the bush upon being destroyed
*/
void Bush::initStoredObject(GameObject& object)
{
	if (stored_object == nullptr)
	{
		stored_object = std::shared_ptr<GameObject>(&object);

		stored_object->start_active_state = false;
		stored_object->start_pos = position;

		stored_object->is_active = false;
		stored_object->position = position;
		stored_object->updatePos();
	}
}

/*
	If the bush stores an object, re-activate it
*/
void Bush::revealObject()
{
	if (stored_object != nullptr)
	{
		stored_object->is_active = true;
	}
}
