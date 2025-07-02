
#pragma once

#include "Base/GameObject.h"

#include <SFML/Graphics.hpp>

#include <iostream>

// ----------------
// 
// A collectible object which restores the player's health
// 
// ----------------

class Heart : public GameObject
{
	public:
		Heart();
};