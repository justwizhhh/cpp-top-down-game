
#pragma once

#include "Base/GameObject.h"

#include <SFML/Graphics.hpp>

#include <iostream>

// ----------------
// 
// This object follows the player's cursor, and acts as a guide for moving the 
// player around in ball mode
// 
// ----------------

class PlayerCue : public GameObject
{
	public:
		PlayerCue();
};