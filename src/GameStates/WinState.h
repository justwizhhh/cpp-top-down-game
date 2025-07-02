
#pragma once

#include "Base/GameState.h"

#include "UI/UIButton.h"

#include <SFML/Graphics.hpp>

#include <iostream>

// ----------------
// 
// The game's win-screen state
// 
// ----------------

class WinState : public GameState
{
public:
	// Function overrides
	virtual void beginState() override;

	virtual void keyInput(sf::Event event) override;
	virtual void mouseInput(sf::Event event) override;

	virtual int update(float dt) override;
	virtual void render() override;

	virtual void endState() override;

private:
	Vector2 mouse_pos = Vector2(0, 0);

	// State triggers
	bool is_back_to_title = false;
	
	// UI assets
	UIButton back_button;
	UIButton quit_button;

	sf::Text main_text;
	sf::Text sub_text1;
	sf::Text sub_text2;

	sf::Texture background_texture;
	sf::Sprite background;
};
