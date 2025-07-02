
#pragma once

#include "Base/GameState.h"

#include "UI/UIButton.h"

#include <SFML/Graphics.hpp>

#include <iostream>

// ----------------
// 
// This state will simply display a tutorial slideshow for how to use the 
// game's controls and play the game
// 
// ----------------

class InstructionsState : public GameState
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

	int panel_progress = 0;

	// Graphics
	std::vector<std::shared_ptr<sf::Texture>> panel_textures;
	std::vector<std::shared_ptr<sf::Sprite>> panel_sprites;
};
