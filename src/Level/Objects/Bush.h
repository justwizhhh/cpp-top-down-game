
#pragma once

#include "Base/GameObject.h"

#include <SFML/Graphics.hpp>

#include <iostream>

// ----------------
// 
// A breakable object that has a chance to store a gem or heart object inside
// 
// ----------------

class Bush : public GameObject
{
	public:
		Bush();

		void initStoredObject(GameObject& object);
		void revealObject();

		float heart_chance = 0.1f;
		float gem_chance = 0.2f;

	private:
		std::shared_ptr<GameObject> stored_object;
};