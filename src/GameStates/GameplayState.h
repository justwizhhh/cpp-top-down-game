
#pragma once

#include "Base/GameState.h"

#include "Level/Objects/Player.h"
#include "Level/Objects/PlayerCue.h"
#include "Level/LevelRoom.h"
#include "UI/GameplayUI.h"

#include "Level/Objects/Heart.h"
#include "Level/Objects/Gem.h"
#include "Level/Objects/EndPotion.h"
#include "Level/Objects/Bush.h"
#include "Level/Objects/Enemies/EnemySlime.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

// ----------------
// 
// This state simulates and handles all gameplay in the game, with all of the 
// main levels and interactive objects being loaded and updated here
// 
// ----------------

class GameplayState : public GameState
{
public:
	// Toggleable variables
	// Ensure 'margin' is smaller than 'offset', so that the player does not
	// get stuck between loading two different rooms
	float cam_trans_margin = 20;
	float cam_trans_offset = 40;
	
	// Function overrides
	virtual void beginState() override;

	virtual void keyInput(sf::Event event) override;
	virtual void mouseInput(sf::Event event) override;
	virtual int update(float dt) override;

	virtual void render() override;
	virtual void endState() override;

	// Main functions
	bool initObjects();
	bool initLevel();
	bool initUI();
	void cameraReset();

	void loadNewRoom();
	void cameraUpdate();

private:
	float pi = 3.141592f;
	Vector2 mouse_pos = Vector2(0, 0);
	
	// Level objects
	Player player;
	PlayerCue player_cue;
	sf::CircleShape player_cue_area;

	// Level assets
	std::vector<std::shared_ptr<LevelRoom>> level_rooms;
	std::shared_ptr<LevelRoom> current_loaded_room;

	// Rendering
	sf::View camera;
	GameplayUI ui;
	sf::View ui_view;

	// Misc.
	bool transition_to_title = false;
	bool transition_to_win = false;
	bool transition_to_gameover = false;
};

