
#pragma once

#include "Base/GameState.h"

#include "UI/UIButton.h"

#include <SFML/Graphics.hpp>

#include <iostream>


class StartState : public GameState
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
		bool is_starting_game = false;
		bool is_instructions = false;
		bool is_quitting_game = false;

		// UI assets
		UIButton start_button;
		UIButton instructions_button;
		UIButton quit_button;

		sf::Text logo_text;

		sf::Texture background_texture;
		sf::Sprite background;
};

