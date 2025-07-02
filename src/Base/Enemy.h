
#pragma once

#include "Base/GameObject.h"

#include <SFML/Graphics.hpp>

// ----------------
// 
// The base class for enemy objects in the game
// 
// These objects will attempt to find the player and hit them to reduce their 
// health
// 
// ----------------

class Enemy : public GameObject
{
	public:
		// Main functions
		Enemy();

		virtual void onHurt();
		virtual void onDeath();

		// Main variables
		int max_health = 1;

		int health = 0;
};